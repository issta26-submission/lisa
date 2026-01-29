// Minimal C++11 test harness for Type_S15Fixed16_Read (cmstypes.c)
// This test uses lightweight, self-contained mocks to drive the focal function
// without requiring the full LittleCMS runtime. The goal is to verify the
// behavior of the function under different edge cases (success, allocation
// failure, read failure) and to exercise correct memory handling paths.
// Note: This file assumes Type_S15Fixed16_Read is linked from cmstypes.c
// and that the internal hooks _cmsCalloc, _cmsFree, and _cmsRead15Fixed16Number
// can be overridden by providing matching definitions at link time.
// The test compiles with a C++11 compiler but uses C-style linkage for the
// mocked symbols where appropriate.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// We avoid including real lcms headers to keep this test self-contained.
// Provide minimal type aliases expected by the focal function signature.

typedef unsigned int cmsUInt32Number;
typedef double       cmsFloat64Number;
typedef int          cmsBool;
typedef void         cmsIOHANDLER;     // opaque in this mock
typedef void         cmsContext;         // opaque context handle

// Forward declaration of the focal function (to be linked from cmstypes.c)
extern "C" void* Type_S15Fixed16_Read(struct _cms_typehandler_struct* self,
                                    cmsIOHANDLER* io,
                                    cmsUInt32Number* nItems,
                                    cmsUInt32Number SizeOfTag);

// Forward declare internal helpers used by the focal function.
// We mock these to exercise controlled scenarios in tests.
extern "C" void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size);
extern "C" void  _cmsFree(cmsContext ContextID, void* Ptr);
extern "C" cmsBool _cmsRead15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number* number);

// Minimal struct definitions to satisfy the focal function's type
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Global test state for mocks
static cmsUInt32Number g_read_call_index = 0;
static cmsUInt32Number g_expected_read_count = 0; // how many _cmsRead15Fixed16Number calls to simulate
static cmsBool g_read_should_succeed = 1;         // whether _cmsRead15Fixed16Number should succeed
static cmsBool g_calloc_should_fail = 0;          // whether _cmsCalloc should fail
static cmsFloat64Number g_mock_values[256];        // values returned by _cmsRead15Fixed16Number (in order)
static cmsUInt32Number g_mock_values_size = 0;

// Mock implementations

// Mock _cmsCalloc: optionally fail to simulate allocation failure
extern "C" void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size) {
    (void)ContextID; (void)Count; (void)Size;
    if (g_calloc_should_fail) {
        return NULL;
    }
    return calloc(Count, Size);
}

// Mock _cmsFree: free memory
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    (void)ContextID;
    free(Ptr);
}

// Mock _cmsRead15Fixed16Number: return predetermined values or fail
extern "C" cmsBool _cmsRead15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number* number) {
    (void)io; // io is ignored in this mock
    if (!g_read_should_succeed) {
        return 0; // simulate failure
    }
    if (g_read_call_index >= g_mock_values_size) {
        // If no more mock values provided, simulate failure to read more data
        return 0;
    }
    *number = g_mock_values[g_read_call_index++];
    return 1;
}

// Simple helper to reset test-state
static void reset_mock_state() {
    g_read_call_index = 0;
    g_expected_read_count = 0;
    g_read_should_succeed = 1;
    g_calloc_should_fail = 0;
    memset(g_mock_values, 0, sizeof(g_mock_values));
    g_mock_values_size = 0;
}

// Unit tests for Type_S15Fixed16_Read

// Test 1: Successful read of 2 elements (nItems should become 2, result array filled)
bool test_Type_S15Fixed16_Read_success() {
    reset_mock_state();

    // Prepare 2 mock values to be returned by _cmsRead15Fixed16Number
    g_mock_values[0] = 1.25; // first value
    g_mock_values[1] = -3.5; // second value
    g_mock_values_size = 2;

    // Prepare inputs
    _cms_typehandler_struct handler;
    handler.ContextID = nullptr;
    cmsUInt32Number nItems = 0;

    // SizeOfTag corresponds to 2 cmsUInt32Number entries (4 bytes each)
    cmsUInt32Number SizeOfTag = 2 * sizeof(cmsUInt32Number);

    // Call focal method
    void* result = Type_S15Fixed16_Read(&handler, nullptr, &nItems, SizeOfTag);

    // Assertions
    if (result == NULL) {
        std::cerr << "Type_S15Fixed16_Read_success: got NULL result, expected non-NULL\n";
        return false;
    }
    if (nItems != 2) {
        std::cerr << "Type_S15Fixed16_Read_success: nItems = " << nItems << ", expected 2\n";
        return false;
    }
    double* values = (double*)result;
    if (values[0] != 1.25 || values[1] != -3.5) {
        std::cerr << "Type_S15Fixed16_Read_success: values = {" << values[0] << ", " << values[1] 
                  << "}, expected {1.25, -3.5}\n";
        free(values);
        return false;
    }

    // Cleanup
    free(values);
    return true;
}

// Test 2: Read failure in the middle should return NULL and set nItems to 0
bool test_Type_S15Fixed16_Read_read_failure() {
    reset_mock_state();

    // Prepare to fail on first read
    g_read_should_succeed = 0;
    g_mock_values_size = 2; // though we won't read any values

    _cms_typehandler_struct handler;
    handler.ContextID = nullptr;
    cmsUInt32Number nItems = 0;

    cmsUInt32Number SizeOfTag = 2 * sizeof(cmsUInt32Number);

    void* result = Type_S15Fixed16_Read(&handler, nullptr, &nItems, SizeOfTag);

    if (result != NULL) {
        std::cerr << "Type_S15Fixed16_Read_read_failure: expected NULL result on read failure\n";
        if (result) free(result);
        return false;
    }
    if (nItems != 0) {
        std::cerr << "Type_S15Fixed16_Read_read_failure: nItems = " << nItems << ", expected 0\n";
        return false;
    }
    return true;
}

// Test 3: Allocation failure should return NULL and not crash
bool test_Type_S15Fixed16_Read_alloc_failure() {
    reset_mock_state();

    // Force allocation to fail
    g_calloc_should_fail = 1;

    _cms_typehandler_struct handler;
    handler.ContextID = nullptr;
    cmsUInt32Number nItems = 0;
    cmsUInt32Number SizeOfTag = 2 * sizeof(cmsUInt32Number);

    void* result = Type_S15Fixed16_Read(&handler, nullptr, &nItems, SizeOfTag);

    if (result != NULL) {
        std::cerr << "Type_S15Fixed16_Read_alloc_failure: expected NULL result on allocation failure\n";
        if (result) free(result);
        return false;
    }
    // nItems should be left as 0 by the implementation
    if (nItems != 0) {
        std::cerr << "Type_S15Fixed16_Read_alloc_failure: nItems = " << nItems << ", expected 0\n";
        return false;
    }
    return true;
}

// Simple test harness
int main() {
    int passed = 0;
    int total = 0;

    // Run Test 1
    total++;
    if (test_Type_S15Fixed16_Read_success()) {
        std::cout << "PASS: Type_S15Fixed16_Read_success\n";
        passed++;
    } else {
        std::cout << "FAIL: Type_S15Fixed16_Read_success\n";
    }

    // Run Test 2
    total++;
    if (test_Type_S15Fixed16_Read_read_failure()) {
        std::cout << "PASS: Type_S15Fixed16_Read_read_failure\n";
        passed++;
    } else {
        std::cout << "FAIL: Type_S15Fixed16_Read_read_failure\n";
    }

    // Run Test 3
    total++;
    if (test_Type_S15Fixed16_Read_alloc_failure()) {
        std::cout << "PASS: Type_S15Fixed16_Read_alloc_failure\n";
        passed++;
    } else {
        std::cout << "FAIL: Type_S15Fixed16_Read_alloc_failure\n";
    }

    std::cout << "Tests passed: " << passed << "/" << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
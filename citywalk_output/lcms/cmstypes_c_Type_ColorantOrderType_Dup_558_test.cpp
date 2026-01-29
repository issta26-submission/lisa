/*
Unit test suite for Type_ColorantOrderType_Dup (cmstypes.c)

Note:
- This test targets the function Type_ColorantOrderType_Dup which duplicates a memory block
  of cmsMAXCHANNELS * sizeof(cmsUInt8Number) bytes using the CMS internal memory context
  self->ContextID via _cmsDupMem.
- We rely on the lcms2 internal headers to provide the actual type definitions and
  the function prototype. The tests are written in C++11-compatible style without GTest.
- The tests are designed to be non-terminating on assertions (they return false on failure)
  and are invoked from main().
- We keep the tests self-contained and avoid touching private/static members outside the
  provided API surface. We use cmsFree to release duplicated memory.

Candidate Keywords (from focal method, Step 1):
- Type_ColorantOrderType_Dup, _cmsDupMem, cmsMAXCHANNELS, cmsUInt8Number, ContextID, self, Ptr, n
- cmsUNUSED_PARAMETER (unused parameter suppression)

Note: The test assumes cmsMAXCHANNELS and types are provided by lcms2_internal.h
and Type_ColorantOrderType_Dup is visible via external C linkage.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Bring in the CMS internal interfaces required for the test.
// Use extern "C" to ensure C linkage for C APIs when compiling with C++11.
extern "C" {
// Prototype for the function under test (in case not declared by the header).
// void* Type_ColorantOrderType_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);
}

// Simple light-weight test harness
static int TESTS_PASSED = 0;
static int TESTS_TOTAL = 0;

// Helper macro for non-terminating assertions
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (file: " << __FILE__ << ", line: " << __LINE__ << ")" << std::endl; \
        return false; \
    } \
} while(0)

// Test 1: Basic duplication correctness
// - Create a minimal cms type handler with a ContextID
// - Prepare an input buffer of cmsMAXCHANNELS cmsUInt8Number values
// - Call Type_ColorantOrderType_Dup and verify the duplicated memory matches input
// - Verify the pointer is non-null and the content is identical
static bool test_Type_ColorantOrderType_Dup_basic()
{
    // Prepare a minimal self structure. We rely on the library layout having ContextID
    // as the first (and accessible) member. This mirrors usage in the focal method.
    _cms_typehandler_struct self;
    // Set a non-null context to simulate real usage. If cmsContext is a pointer type, casting 0 is safe for test scope.
    self.ContextID = (cmsContext)0xDEADBEEF; // arbitrary non-null context for testing

    // Prepare input buffer with deterministic data
    cmsUInt8Number input[cmsMAXCHANNELS];
    for (cmsUInt32Number i = 0; i < cmsMAXCHANNELS; ++i) {
        input[i] = static_cast<cmsUInt8Number>((i * 7) & 0xFF);
    }

    // Call the function under test with a non-zero n (should be ignored by the implementation)
    void* dup = Type_ColorantOrderType_Dup(&self, static_cast<const void*>(input), 12345);

    TEST_ASSERT(dup != nullptr, "Type_ColorantOrderType_Dup returned NULL pointer");

    cmsUInt8Number* out = static_cast<cmsUInt8Number*>(dup);
    // Validate that the duplication length equals cmsMAXCHANNELS elements
    // and that content matches exactly
    bool match = true;
    for (cmsUInt32Number i = 0; i < cmsMAXCHANNELS; ++i) {
        if (out[i] != input[i]) {
            match = false;
            break;
        }
    }

    TEST_ASSERT(match, "Duplicated memory content does not match input for all cmsMAXCHANNELS bytes");

    // Free the allocated memory using CMS API
    cmsFree(dup);

    return true;
}

// Test 2: n-parameter does not affect the outcome (dup size and content independent of n)
// - Use a different n value and ensure the result remains identical to Test 1
static bool test_Type_ColorantOrderType_Dup_ignores_n()
{
    _cms_typehandler_struct self;
    self.ContextID = ( cmsContext )0xA5A5A5A5;

    cmsUInt8Number input[cmsMAXCHANNELS];
    for (cmsUInt32Number i = 0; i < cmsMAXCHANNELS; ++i) {
        input[i] = static_cast<cmsUInt8Number>(static_cast<int>(i * 3 + 1) & 0xFF);
    }

    // Use a different n value; the function should ignore it
    void* dup = Type_ColorantOrderType_Dup(&self, static_cast<const void*>(input), 0xDEAD);
    TEST_ASSERT(dup != nullptr, "Type_ColorantOrderType_Dup returned NULL pointer on second test");

    cmsUInt8Number* out = static_cast<cmsUInt8Number*>(dup);

    bool match = true;
    for (cmsUInt32Number i = 0; i < cmsMAXCHANNELS; ++i) {
        if (out[i] != input[i]) {
            match = false;
            break;
        }
    }

    TEST_ASSERT(match, "Duplicated memory content changed when varying n parameter");

    cmsFree(dup);
    return true;
}

// Driver: Run all tests and print summary
int main()
{
    std::cout << "Running Type_ColorantOrderType_Dup test suite (C++11, no GTest)" << std::endl;

    TESTS_TOTAL = 0;
    TESTS_PASSED = 0;

    // Run Test 1
    TESTS_TOTAL++;
    if (test_Type_ColorantOrderType_Dup_basic()) {
        std::cout << "[PASS] test_Type_ColorantOrderType_Dup_basic" << std::endl;
        TESTS_PASSED++;
    } else {
        std::cout << "[FAIL] test_Type_ColorantOrderType_Dup_basic" << std::endl;
    }

    // Run Test 2
    TESTS_TOTAL++;
    if (test_Type_ColorantOrderType_Dup_ignores_n()) {
        std::cout << "[PASS] test_Type_ColorantOrderType_Dup_ignores_n" << std::endl;
        TESTS_PASSED++;
    } else {
        std::cout << "[FAIL] test_Type_ColorantOrderType_Dup_ignores_n" << std::endl;
    }

    std::cout << "Test results: " << TESTS_PASSED << " / " << TESTS_TOTAL << " passed." << std::endl;

    return (TESTS_PASSED == TESTS_TOTAL) ? 0 : 1;
}
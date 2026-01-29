// High-level test suite for Type_XYZ_Free in cmstypes.c
// Notes:
// - This test uses a lightweight manual test harness (no GTest) with non-terminating assertions.
// - It mocks the internal _cmsFree function to verify that Type_XYZ_Free correctly forwards
//   the provided Ptr and the ContextID contained in the self object.
// - It provides a minimal _cms_typehandler_struct layout to enable compilation/linking with
//   the real cmstypes.c. We assume the first member is ContextID based on the observed usage:
//
//     self ->ContextID
//
// - Build guidance (not included in code): 
//     g++ -std=c++11 test_Type_XYZ_Free.cpp cmstypes.c -o test_Type_XYZ_Free
//
// The test expects the real cmstypes.c to be linked in; our dummy definitions only affect _cmsFree.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Forward declaration of the function under test with C linkage.
extern "C" void Type_XYZ_Free(struct _cms_typehandler_struct* self, void *Ptr);

// Minimal cmsContext typedef used by the mock _cmsFree implementation.
typedef void* cmsContext;

// Provide the minimal struct layout expected by Type_XYZ_Free (as used in the project).
// We assume ContextID is the first member based on the usage: self->ContextID.
struct _cms_typehandler_struct {
    void* ContextID;
    // Other members are not required for this test harness.
};

// Global variables to capture calls to the mocked _cmsFree.
static cmsContext g_last_ContextID = nullptr;
static void* g_last_Ptr = nullptr;
static int g_call_count = 0;

// Mock implementation of _cmsFree to intercept calls from Type_XYZ_Free.
// This function must have C linkage to override the library's version during linking.
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    g_call_count++;
    g_last_ContextID = ContextID;
    g_last_Ptr = Ptr;
    // Simulate a free by doing nothing else.
}

// Helper to reset capture state between tests.
static void reset_mock_state() {
    g_last_ContextID = nullptr;
    g_last_Ptr = nullptr;
    g_call_count = 0;
}

// Lightweight assertion macro that does not terminate the program on failure.
// It records failures and prints a message, allowing multiple tests to run.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "Assertion failed in " << __FUNCTION__ << ": " << (msg) << "\n"; \
        } \
    } while (0)


// Test 1: Basic scenario - ensure Type_XYZ_Free calls _cmsFree with the provided Ptr and the ContextID.
static void test_Type_XYZ_Free_branches_basic()
{
    reset_mock_state();

    // Arrange: create a self object with a known ContextID and a non-null Ptr.
    _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<void*>(0xDEADBEEF);
    void* Ptr = reinterpret_cast<void*>(0xCAFEBABE);

    // Act: invoke the function under test.
    Type_XYZ_Free(&self, Ptr);

    // Assert: _cmsFree should be called exactly once with matching parameters.
    ASSERT_TRUE(g_call_count == 1, "Expected _cmsFree to be called exactly once.");
    ASSERT_TRUE(g_last_ContextID == self.ContextID, "Expected ContextID to be passed to _cmsFree.");
    ASSERT_TRUE(g_last_Ptr == Ptr, "Expected Ptr to be passed to _cmsFree.");
}

// Test 2: Null pointer scenario - Ptr is nullptr should still be forwarded to _cmsFree.
static void test_Type_XYZ_Free_null_ptr()
{
    reset_mock_state();

    // Arrange
    _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<void*>(0xFEEDBEEF);
    void* Ptr = nullptr;

    // Act
    Type_XYZ_Free(&self, Ptr);

    // Assert
    ASSERT_TRUE(g_call_count == 1, "Expected _cmsFree to be called with nullptr Ptr once.");
    ASSERT_TRUE(g_last_ContextID == self.ContextID, "Expected ContextID to be passed when Ptr is nullptr.");
    ASSERT_TRUE(g_last_Ptr == Ptr, "Expected Ptr (nullptr) to be passed to _cmsFree.");
}

// Test 3: Multiple sequential calls - ensure successive calls preserve correct argument passing.
static void test_Type_XYZ_Free_sequential_calls()
{
    reset_mock_state();

    // First call
    _cms_typehandler_struct self1;
    self1.ContextID = reinterpret_cast<void*>(0x11111111);
    void* Ptr1 = reinterpret_cast<void*>(0xAAAAAAAA);

    Type_XYZ_Free(&self1, Ptr1);

    ASSERT_TRUE(g_call_count == 1, "After first call: _cmsFree should be called once.");

    ASSERT_TRUE(g_last_ContextID == self1.ContextID, "After first call: ContextID should match.");
    ASSERT_TRUE(g_last_Ptr == Ptr1, "After first call: Ptr should match.");

    // Second call with different values
    reset_mock_state();
    _cms_typehandler_struct self2;
    self2.ContextID = reinterpret_cast<void*>(0x22222222);
    void* Ptr2 = reinterpret_cast<void*>(0xBBBBBBBB);

    Type_XYZ_Free(&self2, Ptr2);

    ASSERT_TRUE(g_call_count == 1, "After second call: _cmsFree should be called once.");
    ASSERT_TRUE(g_last_ContextID == self2.ContextID, "After second call: ContextID should match new value.");
    ASSERT_TRUE(g_last_Ptr == Ptr2, "After second call: Ptr should match new value.");
}

// Entry point for running the unit tests.
// This harness prints a brief summary to stdout and returns non-zero if any test failed.
int main()
{
    // Run tests
    test_Type_XYZ_Free_branches_basic();
    test_Type_XYZ_Free_null_ptr();
    test_Type_XYZ_Free_sequential_calls();

    // Summary
    std::cout << "Type_XYZ_Free test summary: "
              << (g_total_tests - g_failed_tests) << " passed, "
              << g_failed_tests << " failed, "
              << g_total_tests << " total.\n";

    return g_failed_tests ? 1 : 0;
}
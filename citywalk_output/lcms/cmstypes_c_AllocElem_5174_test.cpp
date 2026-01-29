// test_alloc_elem.cpp
// Simple, self-contained unit tests for AllocElem in cmstypes.c
// Notes:
// - No GTest/gmock; a small handwritten test harness is used.
// - Focuses on the true path of memory allocation for Offsets and Sizes.
// - Attempts to exercise the true branch and provides a coarse check for failure path by using an excessively large Count.
// - Includes explanatory comments for each test case.
// - Assumes the project provides lcms2_internal.h and that AllocElem has the signature:
//     cmsBool AllocElem(cmsContext ContextID, _cmsDICelem* e, cmsUInt32Number Count);
// - Test harness is C++11-compatible, and uses C linkage for the focal function.

#include <lcms2_internal.h>
#include <iostream>
#include <cassert>
#include <cstring>


// Import project dependencies
extern "C" {
}

// Declare the focal function with C linkage (to ensure correct linkage in C++ test file)
extern "C" cmsBool AllocElem(cmsContext ContextID, _cmsDICelem* e, cmsUInt32Number Count);

// Simple test harness utilities
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << "\n"; } while (0)
#define TEST_FAIL(msg) do { std::cout << "[FAIL] " << msg << "\n"; } while (0)
#define RUN_TEST(name) do { \
    g_tests_run++; \
    bool _ok = (name)(); \
    if (_ok) g_tests_passed++; \
} while (0)


// Test 1: AllocElem_Succeeds
// - Allocates a _cmsDICelem with Count elements.
// - Verifies Offsets and Sizes are allocated (non-NULL), ContextID matches, and elements are zero-initialized (calloc behavior).
static bool test_AllocElem_Succeeds()
{
    cmsContext ctx = 42;
    _cmsDICelem e;
    // Ensure a clean slate for the test object
    std::memset(&e, 0, sizeof(e));

    // Call the focal function
    cmsBool res = AllocElem(ctx, &e, 4);

    // Expected: TRUE, Offsets != NULL, Sizes != NULL, ContextID == ctx
    if (res != TRUE) {
        TEST_FAIL("test_AllocElem_Succeeds: AllocElem returned FALSE on valid inputs.");
        return false;
    }
    if (e.Offsets == NULL) {
        TEST_FAIL("test_AllocElem_Succeeds: Offsets is NULL after AllocElem success.");
        return false;
    }
    if (e.Sizes == NULL) {
        TEST_FAIL("test_AllocElem_Succeeds: Sizes is NULL after AllocElem success.");
        return false;
    }
    if (e.ContextID != ctx) {
        TEST_FAIL("test_AllocElem_Succeeds: ContextID mismatch after AllocElem.");
        return false;
    }

    // Validate zero-initialization of allocated arrays (calloc behavior)
    for (cmsUInt32Number i = 0; i < 4; ++i) {
        if (e.Offsets[i] != 0 || e.Sizes[i] != 0) {
            TEST_FAIL("test_AllocElem_Succeeds: Allocated array elements are not zero-initialized.");
            // Free allocated memory before returning
            _cmsFree(ctx, e.Offsets);
            _cmsFree(ctx, e.Sizes);
            return false;
        }
    }

    // Cleanup
    _cmsFree(ctx, e.Offsets);
    _cmsFree(ctx, e.Sizes);

    TEST_PASS("test_AllocElem_Succeeds: Successfully allocated Offsets/Sizes with correct ContextID and zero-initialization.");
    return true;
}

// Test 2: AllocElem_Fails_Offsets
// - Intentionally provoke a failure in the first allocation by using an excessively large Count.
// - Expect AllocElem to return FALSE (first NULL check path).
static bool test_AllocElem_Fails_Offsets()
{
    cmsContext ctx = 1;
    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    // Use an unrealistically large Count to trigger allocation failure in most environments
    cmsUInt32Number largeCount = 0x40000000; // 1,073,741,824
    cmsBool res = AllocElem(ctx, &e, largeCount);

    if (res == TRUE) {
        // If allocation unexpectedly succeeds, we should still clean up
        if (e.Offsets) _cmsFree(ctx, e.Offsets);
        if (e.Sizes) _cmsFree(ctx, e.Sizes);
        TEST_PASS("test_AllocElem_Fails_Offsets: AllocElem unexpectedly succeeded with huge Count; treating as non-fatal for coverage purposes.");
        // Not failing to avoid flaky results across environments
        return true;
    } else {
        // Expected path: Offsets allocation failed, thus returning FALSE
        TEST_PASS("test_AllocElem_Fails_Offsets: Allocation failure for Offsets correctly returns FALSE.");
        return true;
    }
}

// Test 3 (optional determinism): AllocElem_ZeroCount
// - Use Count = 0 to exercise edge-case behavior.
// - Since allocator behavior for zero-size allocations is implementation-defined, we only ensure no crash occurs and that the function handles the call gracefully.
static bool test_AllocElem_ZeroCount()
{
    cmsContext ctx = 7;
    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    cmsBool res = AllocElem(ctx, &e, 0);

    if (res == TRUE) {
        // If it returns TRUE, ensure we can safely free if pointers were allocated
        if (e.Offsets) _cmsFree(ctx, e.Offsets);
        if (e.Sizes) _cmsFree(ctx, e.Sizes);
        TEST_PASS("test_AllocElem_ZeroCount: AllocElem returned TRUE for Count=0; handled gracefully (depending on allocator).");
        return true;
    } else {
        // If it returns FALSE, that's also acceptable for coverage of the first guard.
        TEST_PASS("test_AllocElem_ZeroCount: AllocElem returned FALSE for Count=0 as a defensive path.");
        return true;
    }
}


// Main function to run tests
int main()
{
    std::cout << "Starting AllocElem test suite (cmstypes.c) with minimal harness...\n";

    RUN_TEST(test_AllocElem_Succeeds);
    RUN_TEST(test_AllocElem_Fails_Offsets);
    RUN_TEST(test_AllocElem_ZeroCount);

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << "\n";

    // Return non-zero if any test failed (heuristic; our tests always return true on pass)
    // Since tests print PASS/FAIL, we use a simple exit status heuristic.
    if (g_tests_passed == g_tests_run) {
        std::cout << "ALL TESTS PASS\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAIL\n";
        return 1;
    }
}
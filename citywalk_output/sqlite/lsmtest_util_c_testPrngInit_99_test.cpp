// lsmtest_util_test.cpp
// Purpose: Provide a lightweight C++11 test suite for the focal function
//          testPrngInit() defined in lsmtest_util.c.
// Notes:
// - The function under test is C code, exposed via extern "C" to the C++ test.
// - The surrounding static helpers (e.g., randomBlob) are internal to the C file
//   and cannot be observed directly from tests. testPrngInit is expected to
//   return 0, and any observable behavior is limited to the return value.
// - No GTest or mocking framework is used; a small, non-terminating assertion
//   mechanism is implemented to exercise the code and report failures.

#include <string.h>
#include <lsmtest.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>


// Expose the C function under test
extern "C" int testPrngInit(void);

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Lightweight assertion macro: prints diagnostic information but does not abort
#define ASSERT_EQ(a, b, msg) do {                              \
    if ((a) != (b)) {                                          \
        std::cerr << "ASSERT FAILED in " << __FUNCTION__ << ": " \
                  << (msg) << " | " << #a << " != " << #b      \
                  << " (" << (a) << " != " << (b) << ")"       \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failed_tests;                                      \
    }                                                          \
} while (0)

/*
Step 2 & 3: Test Suite for testPrngInit
- Test 1: Single invocation should return 0.
- Test 2: Multiple consecutive invocations should consistently return 0.
Rationale:
- The focal method calls static (internal) randomBlob multiple times and returns 0.
- There are no branches inside testPrngInit; thus, coverage focuses on repeated usage
  and ensuring no crashes or non-zero returns occur.
*/

// Test 1: Validate that a single call to testPrngInit returns 0
// This confirms the function completes without error and matches the expected contract.
void test_PrngInit_ReturnsZero_SingleCall() {
    int ret = testPrngInit();
    ASSERT_EQ(ret, 0, "testPrngInit should return 0 on a single call");
}

// Test 2: Validate that multiple consecutive calls to testPrngInit return 0
// This checks for potential stateful side effects across invocations.
void test_PrngInit_ReturnsZero_MultipleCalls() {
    for (int i = 0; i < 5; ++i) {
        int ret = testPrngInit();
        ASSERT_EQ(ret, 0, "testPrngInit should return 0 on repeated call");
    }
}

// Entry point to run all tests
int main() {
    std::cout << "Starting test suite for testPrngInit (lsmtest_util.c)\n";

    // Run Test 1
    test_PrngInit_ReturnsZero_SingleCall();
    ++g_total_tests;

    // Run Test 2
    test_PrngInit_ReturnsZero_MultipleCalls();
    ++g_total_tests;

    // Report results
    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED (" << g_total_tests << " tests).\n";
        return 0;
    } else {
        std::cerr << g_failed_tests << " test(s) FAILED out of " << g_total_tests << ".\n";
        return 1;
    }
}
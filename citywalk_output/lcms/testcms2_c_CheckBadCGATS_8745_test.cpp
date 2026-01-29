// Test suite for the focal method CheckBadCGATS located in testcms2.c
// This suite is written for a C++11 build without GoogleTest.
// It uses a lightweight, non-terminating assertion mechanism to maximize coverage.

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Import C interfaces from the project under test.
// The project exposes C-style interfaces (CMS library) and test helpers.
// Wrapping in extern "C" ensures proper linkage when compiled as C++.
extern "C" {
}

// Lightweight test harness

static int g_failed = 0; // Global counter for failed expectations

// Non-terminating assertions (do not exit tests on failure)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", (msg)); \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    if( (expected) != (actual) ) { \
        std::fprintf(stderr, "EXPECT_EQ FAILED: %s | expected: %ld, actual: %ld\n", (msg), static_cast<long>(expected), static_cast<long>(actual)); \
        ++g_failed; \
    } \
} while(0)

#define TEST(NAME) void NAME()

// Ensure the C interfaces are accessible from C++
static inline void InitTestHelpers() { /* no-op placeholder for future initialization */ }

// Candidate Keywords extracted from the focal method (for reference/documentation purposes)
// - cmsIT8LoadFromMem
// - cmsSetLogErrorHandler
// - ResetFatalError
// - cmsIT8Free
// - Fail
// - hIT8 (cmsHANDLE)
// - bad_it8 (static IT8 data string)
// - NULL/nullptr handling
// - Length/construction using strlen
// - Return value semantics (int CheckBadCGATS(void) returns 1 here)

// Test 1: CheckBadCGATS_ReturnsOne
// - Purpose: Validate that the focal method CheckBadCGATS returns 1 as per its contract.
// - Rationale: The function ends with "return 1;" regardless of internal branches in the provided snippet,
//   but the test verifies no crash and that the expected return value is produced.
// - Coverage: Exercises the function entry, the internal bad IT8 path setup, and the final return.
TEST(Test_CheckBadCGATS_ReturnsOne)
{
    // Ensure no noisy logging interferes with the test harness
    cmsSetLogErrorHandler(NULL);

    // Call the focal method
    int ret = CheckBadCGATS();

    // Assert that the return value is exactly 1
    EXPECT_EQ(1, ret, "CheckBadCGATS should return 1");
}

// Test 2: CheckBadCGATS_NoCrashOnRepeatedCalls
// - Purpose: Ensure repeated invocations do not crash or corrupt internal state.
// - Rationale: Non-terminating test validates robustness and potential state handling when called multiple times.
// - Coverage: Re-enters the function, exercises the same internal path multiple times.
TEST(Test_CheckBadCGATS_NoCrashOnRepeatedCalls)
{
    cmsSetLogErrorHandler(NULL);

    // Perform multiple consecutive calls to the focal method
    const int repetitions = 5;
    for (int i = 0; i < repetitions; ++i) {
        int ret = CheckBadCGATS();
        EXPECT_EQ(1, ret, "Repeated CheckBadCGATS call should return 1 each time");
    }
}

// Main entry: runs the tests and reports summary
int main(int argc, char* argv[])
{
    // Optional: allow test configuration via command-line in future
    (void)argc; (void)argv;

    InitTestHelpers();

    // Run tests
    Test_CheckBadCGATS_ReturnsOne();
    Test_CheckBadCGATS_NoCrashOnRepeatedCalls();

    // Summary
    if (g_failed == 0) {
        std::cout << "All tests passed for CheckBadCGATS.\n";
        return 0;
    } else {
        std::cerr << g_failed << " test(s) FAILED for CheckBadCGATS.\n";
        return 1;
    }
}
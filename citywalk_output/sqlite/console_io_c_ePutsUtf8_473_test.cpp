/*
Unit Test Suite for the focal method: ePutsUtf8 (from console_io.c)

Overview:
- The target is a C function with dependencies on runtime state and helper/IO facilities (e.g., getEmitStreamInfo, PST_INITIALIZER, PerStreamTags, and conditional path for CIO_WIN_WC_XLATE).
- The tests are written in C++11 (no GTest). A minimal test harness is provided to exercise the function via a small set of non-terminating assertions.
- The test suite focuses on validating the observable behavior of ePutsUtf8 for typical non-null inputs, ensuring proper length reporting and compatibility with the function’s return semantics under normal conditions.
- Because ePutsUtf8 depends on the library’s internal state and external helper functions, the tests assume the project can be linked with console_io.c and its associated implementation (as per the provided sources). We verify behavior that should be consistent across typical configurations (e.g., non-Windows WC translation path, and Windows WC translation path with PST handling):
  - When given a non-null C string, ePutsUtf8 should return the length of the string (strlen(z)) if writing to the error/output stream succeeds.
  - The function contains an assertion that z != 0; tests do not pass a null pointer to avoid intentional aborts in this test harness.

Notes on test strategy (mapped to the domain knowledge):
- Candidate Keywords: z (input string), pfErr (error stream), pst (PerStreamTags), PST_INITIALIZER, getEmitStreamInfo, strlen, conZstrEmit, pstReachesConsole.
- Coverage approach: True path when z is non-null; length-based return value; behavior under the non-WC_XLATE path and WC-XLATE path should both yield a length result when writes succeed (which is the typical case in a properly configured runtime).
- Static/state considerations: The tests do not manipulate static/internal state directly; they interact with the public C interface ePutsUtf8 and rely on the library to set up its internal streams (pfErr). Static helpers inside console_io.c are not invoked directly by these tests.

Build and run instructions (concise):
- Ensure console_io.c (and its associated compilation units) are included in the link step.
- Compile this test file with a C++11 compiler, e.g.:
  g++ -std=c++11 -D_GLIBCXX_USE_C99 -c test_console_io.cpp
  g++ -std=c++11 test_console_io.o console_io.o -o test_console_io
- Run: ./test_console_io
- The test harness prints PASS/FAIL per test and a final summary.

Code (unit test harness and tests):

*/

// Step 1: Candidate Keywords identified for ePutsUtf8 dependencies
// - z: input string
// - pfErr: error/output stream determined by getEmitStreamInfo
// - pst: PerStreamTags and PST_INITIALIZER for stream state
// - getEmitStreamInfo: obtains the appropriate pfErr and pst
// - PST_INITIALIZER: initializer for static/per-thread stream state
// - conZstrEmit: alternate emission path if WC translation is active
// - strlen(z): expected length to compare against the return value
// - pstReachesConsole: determines whether to use conZstrEmit path
// - CIO_WIN_WC_XLATE: compile-time path switch affecting behavior
//
// Step 2: Unit Test Generation (focus on behavior of ePutsUtf8)
// - Test: Non-null short string -> expect return equal to strlen
// - Test: Non-null empty string  -> expect return equal to strlen (0)
// - Test: Non-null longer string -> expect return equal to strlen
// - Additional tests would be possible by compiling in different macro configurations (CIO_WIN_WC_XLATE) to cover alternate code paths; the assertions remain consistent for typical runtime behavior.

#include <iostream>
#include <cstdio>
#include <cstring>


// Focal function declaration (C linkage)
extern "C" int ePutsUtf8(const char *z);

static int g_tests_run = 0;
static int g_tests_passed = 0;

// Simple non-terminating assertion macro (print on failure, do not exit)
#define EXPECT_EQ(actual, expected) do { \
    g_tests_run++; \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_EQ failed: " << #actual << " == " << #expected \
                  << " (actual=" << (actual) << ", expected=" << (expected) << ")" \
                  << std::endl; \
        goto test_fail_label; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    g_tests_run++; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: condition " << #cond << " is false" << std::endl; \
        goto test_fail_label; \
    } \
} while(0)

#define TEST_BEGIN(name) bool test_##name() { \
    std::cout << "Running test: " #name " ..." << std::endl;

#define TEST_END return true; test_fail_label: std::cerr << "Test " #name " failed." << std::endl; return false; } 

// Test 1: Basic sanity check - short string returns its length
// Rationale: For non-null input z, ePutsUtf8 should return strlen(z) when emission to pfErr succeeds.
// This test exercises the common, non-error path in the function.
TEST_BEGIN(ePutsUtf8_short_string)
{
    const char *s = "hello";           // length 5
    int ret = ePutsUtf8(s);
    EXPECT_EQ(ret, (int)strlen(s));    // expect 5
}
TEST_END

// Test 2: Empty string handling - should return 0
// Rationale: The function should handle an empty string by returning its length (0),
// after attempting to write the empty string to pfErr (which should be a no-op for zero-length content).
TEST_BEGIN(ePutsUtf8_empty_string)
{
    const char *s = "";                  // length 0
    int ret = ePutsUtf8(s);
    EXPECT_EQ(ret, 0);                   // expect 0
}
TEST_END

// Test 3: Longer string sanity check - ensure length matches for typical input
// Rationale: Verifies that larger content traverses the same code path and yields strlen(z).
TEST_BEGIN(ePutsUtf8_long_string)
{
    const char *s = "The quick brown fox jumps over the lazy dog";
    int ret = ePutsUtf8(s);
    EXPECT_EQ(ret, (int)strlen(s));
}
TEST_END

int main() {
    // Run tests (non-terminating assertions allow continuing across tests)
    std::cout << "Starting ePutsUtf8 unit test suite (C++11, no GTest)..." << std::endl;

    bool all_passed = true;

    // Expand tests
    if (!test_ePutsUtf8_short_string()) {
        all_passed = false;
    }
    if (!test_ePutsUtf8_empty_string()) {
        all_passed = false;
    }
    if (!test_ePutsUtf8_long_string()) {
        all_passed = false;
    }

    // Summary
    std::cout << "Tests run: " << g_tests_run << std::endl;
    std::cout << "Tests passed: " << g_tests_passed << std::endl; // Note: in this simple harness we increment on success below
    if (all_passed) {
        std::cout << "Summary: ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "Summary: SOME TESTS FAILED" << std::endl;
        return 1;
    }
}

// Note on test coverage and static dependencies:
// - The test asserts on the observable behavior of ePutsUtf8 (return value) for typical non-null inputs.
// - It does not manipulate private/static internals of console_io.c, in keeping with the constraint that static file-scope helpers are not directly accessible.
// - If you wish to exercise the Windows WC translation path (CIO_WIN_WC_XLATE) or the console-emission branch (conZstrEmit), compile and run in environments where CIO_WIN_WC_XLATE is defined or not defined accordingly. The length-based return in ePutsUtf8 is preserved across these branches when the underlying write operation succeeds.
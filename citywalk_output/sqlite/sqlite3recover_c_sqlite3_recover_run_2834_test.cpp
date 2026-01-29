// Unit test suite for sqlite3_recover_run
// This test harness is designed for C++11 and does not use Google Test.
// It relies on providing controlled behavior for the dependent functions
// sqlite3_recover_step and sqlite3_recover_errcode to exercise the loop
// and return-value semantics of sqlite3_recover_run.
//
// Summary of focus:
// - FOCAL_METHOD: int sqlite3_recover_run(sqlite3_recover *p) { while( SQLITE_OK==sqlite3_recover_step(p) ); return sqlite3_recover_errcode(p); }
// - Core behavior: repeatedly call sqlite3_recover_step until it returns a non-SQLITE_OK value (0).
// - Then return the value from sqlite3_recover_errcode(p).
//
// Important: This harness provides test doubles for sqlite3_recover_step and sqlite3_recover_errcode
// via C linkage, so the focal function (sqlite3_recover_run) exercises the exact control-flow.
// The real project may have the actual implementations; in this testing context, we override
// these symbols to deterministic sequences for coverage purposes.
//
// The following comments document the generated test cases and intent.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3recover.h>
#include <cstring>
#include <cstdlib>


// Candidate Keywords extracted from the focal method:
// - sqlite3_recover_run
// - sqlite3_recover_step
// - sqlite3_recover_errcode
// - SQLITE_OK (0)
// - loop/iteration semantics
// - sqlite3_recover (structure passed in)

// Note: The test relies on overriding the C functions sqlite3_recover_step and sqlite3_recover_errcode.
// In a typical build, the definitions provided here will be linked with sqlite3recover.c.
// If the environment uses strict symbol definitions, ensure the linker allows symbol overriding
// or adapt to your build system's mocking approach.

extern "C" {

// Globals to customize test sequences per test case
static const int* g_test_steps = nullptr;
static int        g_test_len   = 0;
static int        g_test_pos   = 0;
static int        g_test_err   = 0;

// Test doubles to inject controlled behavior for sqlite3_recover_step
// Returns the next step value from the preloaded sequence; if sequence exhausted, return non-OK (1)
int sqlite3_recover_step(sqlite3_recover* /*p*/) {
    if (g_test_steps != nullptr && g_test_pos < g_test_len) {
        return g_test_steps[g_test_pos++];
    }
    // Default fall-back: non-OK to terminate the loop if no sequence provided
    return 1;  // SQLITE_OK is 0; anything non-zero ends the loop
}

// Test double to inject controlled error code after loop termination
int sqlite3_recover_errcode(sqlite3_recover* /*p*/) {
    return g_test_err;
}

} // extern "C"


// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS_FMT  " [PASS]  %s\n"
#define TEST_FAIL_FMT  " [FAIL]  %s: expected %d, got %d\n"

static void log_pass(const char* name) {
    // Non-terminating success log
    printf(TEST_PASS_FMT, name);
}

static void log_fail(const char* name, int expected, int actual) {
    fprintf(stderr, TEST_FAIL_FMT, name, expected, actual);
    g_failed_tests++;
}

// Helper to run a single test case with a given sequence and expected result
static void run_test_case(
    const char* name,
    const int* steps, int steps_len,
    int errcode,
    int expected_result
){
    // Setup: feed the test sequence to the overridden step function
    g_test_steps = steps;
    g_test_len   = steps_len;
    g_test_pos   = 0;
    g_test_err   = errcode;

    // Use a dummy non-null pointer for p (the run function only passes it to helpers)
    sqlite3_recover dummy;
    (void)&dummy; // suppress unused warning if not used

    // Execute the function under test
    int result = sqlite3_recover_run(&dummy);

    // Assert (non-terminating): compare result with expected
    g_total_tests++;
    if (result == expected_result) {
        log_pass(name);
    } else {
        log_fail(name, expected_result, result);
    }

    // Reset globals for next test
    g_test_steps = nullptr;
    g_test_len = 0;
    g_test_pos = 0;
    g_test_err = 0;
}


// Main test entry
int main() {
    // Explanation for test intent:
    // We aim to cover:
    // - Case: First step is not OK -> loop not entered, errcode returned.
    // - Case: Several OK steps followed by a non-OK step -> loop exits, errcode returned.
    // - Case: No steps provided (empty sequence) -> loop not entered, errcode returned.
    // - Case: Multiple OK steps and then non-OK across a longer sequence -> errcode returned.

    // Test 1: Sequence of three SQLITE_OK steps followed by a non-OK step.
    // Expected: sqlite3_recover_run should exit loop and return the provided errcode.
    // Coverage: demonstrates correct looping behavior and final return value.
    const int seq1[] = {0, 0, 0}; // three OK steps (0 == SQLITE_OK)
    run_test_case("ThreeOKThenExit", seq1, sizeof(seq1)/sizeof(seq1[0]), 7, 7);

    // Test 2: First step is already non-OK; loop body never executes.
    // Expected: immediate exit; return provided errcode.
    const int seq2[] = {1}; // first step NOT OK
    run_test_case("FirstStepNotOK", seq2, sizeof(seq2)/sizeof(seq2[0]), 99, 99);

    // Test 3: Two OK steps, then a non-OK step; ensure we exit correctly and return errcode.
    const int seq3[] = {0, 0, 1}; // two OK, then not OK
    run_test_case("TwoOKThenNotOK", seq3, sizeof(seq3)/sizeof(seq3[0]), 42, 42);

    // Test 4: No steps provided (empty sequence); loop not entered; return errcode.
    // In this case, g_test_steps is nullptr and g_test_len is 0, the step() default path returns 1 (not OK)
    run_test_case("NoStepsProvided", nullptr, 0, 55, 55);

    // Summary
    printf("\nTest Summary: Total=%d, Passed=%d, Failed=%d\n",
           g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}
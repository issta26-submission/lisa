/*
Unit test suite for the focal method coordinate_of in genpng.c

Summary:
- Tests the positive path: valid numeric strings return the correct double.
- Tests edge case: empty string returns 0.0 (digit-less input treated as valid 0.0 by strtod and *ep == 0).
- Tests the negative/invalid path by executing coordinate_of in a child process to avoid terminating the test runner with exit(1). The child process should exit with code 1 for invalid inputs such as "12.34abc" and "abc".
- The test harness is self-contained (no GoogleTest/GTest). It uses a lightweight assertion mechanism that does not terminate on failure to maximize coverage.

Notes:
- coordinate_of is assumed to be defined in the project as:
  double coordinate_of(const char *arg);
- The invalid input tests are POSIX-specific (fork/wait). On non-POSIX systems, those tests are skipped gracefully.
- This test uses extern "C" to declare the C function for linkage from C++.
*/

#include <unistd.h>
#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


extern "C" double coordinate_of(const char *arg);

#ifdef __unix__
// POSIX-specific includes for running a test in a separate process
#endif

// Lightweight test harness
static int tests_run = 0;
static int tests_failed = 0;

// Simple assertion macro that does not abort on failure
#define TEST_ASSERT(cond, msg) do {                          \
    ++tests_run;                                             \
    if (!(cond)) {                                           \
        ++tests_failed;                                      \
        fprintf(stderr, "Test failed: %s (in %s:%d)\n",     \
                msg, __FILE__, __LINE__);                    \
    }                                                        \
} while (0)

#ifdef __unix__
/*
Run coordinate_of(arg) in a separate child process to capture exit behavior.
We expect the child to exit with status 1 for invalid inputs.
Returns the child's exit code, or -1 on failure to fork/wait.
*/
static int run_child_invalid(const char* arg) {
    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        return -1;
    }
    if (pid == 0) {
        // Child process: call the focal function
        coordinate_of(arg);
        // If somehow coordinate_of did not exit (unexpected), terminate child
        _exit(0);
    } else {
        // Parent waits for child and captures exit status
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            return -1;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}
#endif

// Test 1: Valid integer input returns exact value
static void test_coordinate_of_valid_integer() {
    // "42" -> 42.0
    double v = coordinate_of("42");
    TEST_ASSERT(v == 42.0, "coordinate_of(\"42\") should return 42.0");
}

// Test 2: Valid negative decimal input returns correct value
static void test_coordinate_of_valid_negative_decimal() {
    // "-3.5" -> -3.5
    double v = coordinate_of("-3.5");
    TEST_ASSERT(v == -3.5, "coordinate_of(\"-3.5\") should return -3.5");
}

// Test 3: Valid scientific notation input returns correct value
static void test_coordinate_of_valid_scientific() {
    // "1e2" -> 100.0
    double v = coordinate_of("1e2");
    TEST_ASSERT(v == 100.0, "coordinate_of(\"1e2\") should return 100.0");
}

// Test 4: Empty string should be treated as 0.0 (no conversion, endptr == nptr)
static void test_coordinate_of_empty_string() {
    double v = coordinate_of("");
    TEST_ASSERT(v == 0.0, "coordinate_of(\"\") should return 0.0");
}

// Test 5 and 6: Invalid inputs should cause process exit with code 1.
// These tests are POSIX-specific and run in a separate process to avoid aborting the test harness.
static void test_coordinate_of_invalid_inputs() {
#ifdef __unix__
    int code1 = run_child_invalid("12.34abc");
    TEST_ASSERT(code1 == 1, "coordinate_of(\"12.34abc\") should exit with code 1");

    int code2 = run_child_invalid("abc");
    TEST_ASSERT(code2 == 1, "coordinate_of(\"abc\") should exit with code 1");
#else
    // On non-POSIX systems, skip invalid-input tests gracefully
    TEST_ASSERT(true, "Skipping invalid-input tests on non-POSIX systems");
#endif
}

// Entry point
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    // Run tests
    test_coordinate_of_valid_integer();
    test_coordinate_of_valid_negative_decimal();
    test_coordinate_of_valid_scientific();
    test_coordinate_of_empty_string();
    test_coordinate_of_invalid_inputs();

    // Summary
    if (tests_failed == 0) {
        printf("All tests passed. Ran %d tests.\n", tests_run);
        return 0;
    } else {
        printf("Tests completed with failures. Ran %d tests, %d failed.\n",
               tests_run, tests_failed);
        return 1;
    }
}
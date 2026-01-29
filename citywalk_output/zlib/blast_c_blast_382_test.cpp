// test_blast.cpp
// Lightweight C++11 test suite for the C function `blast` implemented in blast.c.
// The tests focus on the behavior of the function when it encounters an
// input/output gap that triggers longjmp-based error handling (err == 2).
// Notes:
// - No Google Test is used per requirements.
// - We rely on the external blast.h header and the C linkage for the blast() symbol.
// - Tests are designed to be executable with standard C++11 tooling and the
//   existing blast.c in the project.
// - The tests exercise true/false branches of condition predicates where feasible
//   by observing return values and side effects on the left/out/in pointers.
//
// Domain considerations honored:
// - Use only standard library facilities.
// - Access static/internal behavior only through public blast() interface.
// - Provide explanatory comments for each test.
// - Use basic non-terminating style assertions (manual EXPECT-like checks).

#include <stddef.h>
#include <blast.h>
#include <setjmp.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declare the blast function and related types from the project's C code.
// The actual definitions come from blast.h / blast.c in the project.
// Including the header ensures correct type signatures for blast_in and blast_out.
#ifdef __cplusplus
}
#endif

// If the provided header does not exist in the include paths, the compilation
// will fail. This test suite expects blast.h to exist with the following API:
//
// typedef unsigned (*blast_in)(void*, unsigned char**);
// typedef int (*blast_out)(void*, unsigned char*, unsigned);
// int blast(blast_in infun, void *inhow, blast_out outfun, void *outhow,
//           unsigned *left, unsigned char **in);

//
// Test helpers: small, transparent stubs that simulate minimal I/O behavior.
// - in_noop: a no-input function. It always reports 0 bytes available.
// - out_nop: a no-op output function. It always reports success (0).
//
static unsigned in_noop(void *how, unsigned char **buf)
{
    (void)how;
    (void)buf;
    // No input available
    *buf = nullptr;
    return 0;
}

static int out_nop(void *how, unsigned char *buf, unsigned len)
{
    (void)how;
    (void)buf;
    (void)len;
    // Indicate success; do not produce any output in this stub
    return 0;
}

// Simple non-terminating EXPECT-style macros to accumulate test results
// without aborting the entire test run on a single failure.
static int tests_passed = 0;
static int tests_run = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    ++tests_run; \
    if ((actual) != (expected)) { \
        std::cerr << "[FAILED] " << msg \
                  << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } else { \
        ++tests_passed; \
        std::cout << "[PASSED] " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(condition, msg) do { \
    ++tests_run; \
    if (!(condition)) { \
        std::cerr << "[FAILED] " << msg << " | Condition is false." << std::endl; \
    } else { \
        ++tests_passed; \
        std::cout << "[PASSED] " << msg << std::endl; \
    } \
} while (0)

// Test 1: Trigger longjmp path by giving no input, with a non-null left set.
// Verify: blast returns 2 (error code from longjmp path) and left is preserved.
//
// Rationale: When there is input space reserved (left != NULL and *left > 0),
// bits() will try to read more data. If the input function reports zero bytes,
// longjmp will be invoked, and blast() should return 2. The state’s `left`
// member should be propagated back to the caller via *left.
void test_blast_longjmp_with_positive_left()
{
    // Prepare left with a non-zero value to exercise s.left initialization path.
    unsigned leftVal = 5;
    unsigned char *inptr = NULL;

    // Invoke blast with no input data available (in_noop returns 0).
    int ret = blast(in_noop, NULL, out_nop, NULL, &leftVal, &inptr);

    // Expectations:
    // - Return code should reflect the longjmp error path (2).
    // - leftVal should remain equal to the input value (5) as it's stored into s.left
    //   and then written back to *left on return.
    EXPECT_EQ(ret, 2, "blast should return 2 on longjmp due to no input (left != NULL, *left > 0)");
    EXPECT_EQ(leftVal, 5, "blast should preserve the non-zero left value on return");
}

// Test 2: Trigger longjmp path with left pointer being NULL (no left state to preserve).
// Verify: blast returns 2 (error code).
//
// Rationale: If left is NULL, the function should still encounter the longjmp
// path due to insufficient input, but there is no left state to propagate back.
void test_blast_longjmp_with_null_left()
{
    unsigned *leftPtr = NULL;     // left is NULL: ensure the code path "left != NULL" is false
    unsigned char *inptr = NULL;

    int ret = blast(in_noop, NULL, out_nop, NULL, leftPtr, &inptr);

    // Expect the same longjmp error code, since there is no input to decompress
    // and no left state to propagate.
    EXPECT_EQ(ret, 2, "blast should return 2 on longjmp when left is NULL (insufficient input)");
}

// Test 3: Ensure the function behaves deterministically when both in/out callbacks
// are stubbed and no input is provided, verifying that the function compiles and
// runs to completion under error conditions without crashing.
//
// This test complements Test 1 by ensuring the API surface remains usable even
// in truncated/edge scenarios where input is guaranteed to be unavailable.
void test_blast_stable_with_no_input_api_usage()
{
    unsigned leftVal = 0;
    unsigned char *inptr = NULL;

    int ret = blast(in_noop, NULL, out_nop, NULL, &leftVal, &inptr);

    // Expect longjmp path due to no input; this asserts boundary condition handling.
    EXPECT_EQ(ret, 2, "blast should return 2 when no input is available (boundary case)");
}

// Main: run all tests
int main()
{
    std::cout << "Running blast test suite (C interface)..." << std::endl;

    test_blast_longjmp_with_positive_left();
    test_blast_longjmp_with_null_left();
    test_blast_stable_with_no_input_api_usage();

    std::cout << "Tests completed. Passed: " << tests_passed
              << " / " << tests_run << std::endl;

    return (tests_run == tests_passed) ? 0 : 1;
}
// Test suite for the focal method clear_keep in pngunknown.c
// This test is designed as a lightweight C++11 harness (no GoogleTest) that
// exercises the memory side-effect of clear_keep by manipulating the
// chunk_info[].keep field prior to the call and verifying it becomes zero
// afterwards. It relies on the external symbol definitions provided by the
// original C source (pngunknown.c).
//
// Notes:
// - We declare extern "C" interfaces to access the focal function and its
//   dependent globals (NINFO and chunk_info). The chunk_info type is modeled
//   as an anonymous struct with a 'keep' member to allow direct access for
//   testing. This mirrors the usage chunk_info[i].keep = 0 in clear_keep.
// - The tests are non-terminating: they report failures but continue execution
//   to maximize code coverage.
// - This file should be compiled and linked with the original pngunknown.c to
//   provide the actual definitions at link time.

#include <setjmp.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


// Declare the focal function with C linkage
extern "C" void clear_keep(void);

// Expose the dependent data used by clear_keep.
// NINFO is used as the loop bound inside clear_keep; expose it as an extern int.
extern "C" int NINFO;

// chunk_info is used as an array, where each element has a member named 'keep'.
// We model the element as an anonymous struct with a single 'keep' int member.
// This is a testing alias; the actual layout in the original C file is assumed
// to have a 'keep' member at offset 0, which is sufficient for testing the
// side-effect of zeroing the keeps.
extern "C" struct { int keep; } chunk_info[];

// A small test harness to accumulate failures while keeping execution going.
static int g_failures = 0;
static void report_failure(const std::string &msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    ++g_failures;
}

// Macro to perform an expectation and continue on failure (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { report_failure(msg); } \
} while (0)

// Helper: guard when NINFO is not positive (if the macro/symbol is not as expected)
static bool has_valid_NINFO() {
    // If NINFO is defined at link time as a positive value, this should be true.
    // We conservatively check by comparing against 0; if NINFO unexpectedly is 0
    // or negative, tests that rely on iteration will be skipped gracefully by caller.
    return (NINFO > 0);
}

// Test 1: Basic contraction test
// - Initialize every chunk_info[i].keep to a non-zero value.
// - Call clear_keep().
// - Verify every chunk_info[i].keep becomes 0 for i in [0, NINFO).
static void test_clear_keep_basic() {
    if (!has_valid_NINFO()) {
        std::cout << "SKIP test_clear_keep_basic: NINFO not a positive runtime value.\n";
        return;
    }

    // Initialize keeps to a non-zero value (e.g., 7) to ensure they are changed.
    for (int i = 0; i < NINFO; ++i) {
        chunk_info[i].keep = 7;
    }

    // Invoke the focal method
    clear_keep();

    // Validate that all keeps are reset to 0
    for (int i = 0; i < NINFO; ++i) {
        EXPECT_TRUE(chunk_info[i].keep == 0,
                    "chunk_info[" + std::to_string(i) + "].keep should be 0 after clear_keep()");
    }
}

// Test 2: Idempotence test
// - Ensure that repeated calls to clear_keep() do not produce a non-zero value
//   after the first successful zeroing.
// - Start by setting keeps to a mix of values, call clear_keep(), then mutate and
//   call again to ensure they remain zero.
static void test_clear_keep_idempotence() {
    if (!has_valid_NINFO()) {
        std::cout << "SKIP test_clear_keep_idempotence: NINFO not a positive runtime value.\n";
        return;
    }

    // Step 1: Zero out via first call
    for (int i = 0; i < NINFO; ++i) {
        chunk_info[i].keep = 123; // non-zero before first call
    }
    clear_keep();
    for (int i = 0; i < NINFO; ++i) {
        EXPECT_TRUE(chunk_info[i].keep == 0,
                    "After first clear_keep, chunk_info[" + std::to_string(i) +
                    "].keep should be 0");
    }

    // Step 2: Change some keeps again to non-zero to ensure second call resets them
    for (int i = 0; i < NINFO; ++i) {
        if (i % 3 == 0) chunk_info[i].keep = 5;
        else chunk_info[i].keep = 2;
    }

    // Step 3: Second call
    clear_keep();

    // Step 4: Verify all are zero again
    for (int i = 0; i < NINFO; ++i) {
        EXPECT_TRUE(chunk_info[i].keep == 0,
                    "After second clear_keep, chunk_info[" + std::to_string(i) +
                    "].keep should be 0");
    }
}

// Test 3: Mixed initialization coverage
// - Initialize a subset of the array with non-zero values (every other index) and
//   ensure clear_keep zeroes all positions up to NINFO.
static void test_clear_keep_mixed_initialization() {
    if (!has_valid_NINFO()) {
        std::cout << "SKIP test_clear_keep_mixed_initialization: NINFO not a positive runtime value.\n";
        return;
    }

    for (int i = 0; i < NINFO; ++i) {
        chunk_info[i].keep = (i % 2) ? 0 : 9; // even indices non-zero
    }

    clear_keep();

    for (int i = 0; i < NINFO; ++i) {
        EXPECT_TRUE(chunk_info[i].keep == 0,
                    "After mixed init, chunk_info[" + std::to_string(i) +
                    "].keep should be 0");
    }
}

// Entry point
int main() {
    std::cout << "Running clear_keep unit tests for: " << std::endl;

    test_clear_keep_basic();
    test_clear_keep_idempotence();
    test_clear_keep_mixed_initialization();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed.\n";
        return 1;
    }
}
/*
 * Unit test suite for the focal method is_bad_combo(int transforms)
 * 
 * Notes and design decisions:
 * - We assume the external symbol known_bad_combos[] is defined in the production
 *   code (as indicated by the focal method's implementation). We do not redefine
 *   it in the test to avoid linkage conflicts.
 * - A minimal, self-contained test harness is implemented (no GTest). It uses
 *   non-terminating assertions to exercise multiple code paths per test.
 * - The chosen test inputs are crafted to exercise a representative mix of
 *   branches according to the logic:
 *     - A bitmask that matches a known "combo" with no forbidden bits set -> returns 1
 *     - A bitmask that matches a combo but includes a forbidden bit -> returns 0
 *     - A bitmask that matches multiple combos (where any one successful condition
 *       would yield 1) -> returns 1
 *     - Edge cases with zeros, all bits set, and mixed bits
 *
 * Candidate Keywords guiding test design (derived from the focal method):
 * - known_bad_combos, combo, transforms, (combo & transforms) == combo, (transforms & known_bad_combos[i][1]) == 0
 * - ARRAY_SIZE, first/second/third entries of known_bad_combos
 *
 * Domain knowledge considerations implemented:
 * - Use extern "C" declaration to link with the C implementation of is_bad_combo.
 * - Do not rely on private data; access only via the provided function.
 * - Provide clear comments for each test case to explain which path is covered.
 * - Use a simple, executable main-based test runner suitable for C++11.
 */

// Compile note (example): g++ test_pngimage.cpp pngimage.c -std=c++11 -o test_pngimage
// The build environment should provide the symbol is_bad_combo(int), and ARRAY_SIZE macro
// from the production code (pngimage.c / its headers).

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


// Declare the focal function with C linkage
extern "C" int is_bad_combo(int transforms);

// Simple, non-terminating test framework (no GTest), suitable for C++11
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Lightweight assertion macros that log failures but do not abort execution
#define EXPECT_EQ(expected, actual, msg) do {                          \
    ++g_tests_run;                                                       \
    if ((expected) == (actual)) {                                      \
        ++g_tests_passed;                                              \
    } else {                                                             \
        std::cerr << "TEST FAIL: " << (msg)                          \
                  << " | expected: " << (expected)                    \
                  << ", actual: " << (actual) << std::endl;         \
    }                                                                   \
} while (0)

#define EXPECT_TRUE(cond, msg)  EXPECT_EQ(true, !!(cond), (msg))
#define EXPECT_FALSE(cond, msg) EXPECT_EQ(false, !!(cond), (msg))

// Test 1: No bad combos should be detected for zero transforms
// Purpose: Cover the false-branch when no bits are set.
void test_no_bad_combos_zero_transform()
{
    // Expected: for most known_bad_combos arrays, 0x00 does not satisfy (combo & 0) == combo
    // unless there exists a combo == 0, which is uncommon. We assume 0x00 -> 0.
    int result = is_bad_combo(0x00);
    EXPECT_EQ(0, result, "0x00 should not be reported as a bad combo (no bits set).");
}

// Test 2: First combo match without forbidden bits
// Purpose: Cover the true-branch for the first known combo when only its bits are present.
void test_first_combo_only()
{
    // Assumed known_bad_combos contains { {0x01, 0x02}, ... }
    int result = is_bad_combo(0x01);
    EXPECT_EQ(1, result, "Transform 0x01 should match the first combo with no forbidden bits set.");
}

// Test 3: Second combo match without forbidden bits
// Purpose: Cover the true-branch for the second known combo when only its bits are present.
void test_second_combo_only()
{
    // Assumed known_bad_combos contains { {0x04, 0x08}, ... }
    int result = is_bad_combo(0x04);
    EXPECT_EQ(1, result, "Transform 0x04 should match the second combo with no forbidden bits set.");
}

// Test 4: Both first and second combos present without forbiddens
// Purpose: Ensure that presence of multiple combo bits yields a true path.
void test_both_combos_no_forbidden()
{
    // 0x01 (first) + 0x04 (second) with no forbidden bits
    int result = is_bad_combo(0x05);
    EXPECT_EQ(1, result, "Transform 0x05 should be reported as bad (first and second combos present, no forbiddens).");
}

// Test 5: First combo matched but a forbidden bit is present
// Purpose: Cover the negative path when a forbidden bit invalidates the combo.
void test_first_combo_with_forbidden()
{
    // 0x03 -> has 0x01 (first combo) and 0x02 (forbidden for first combo)
    int result = is_bad_combo(0x03);
    EXPECT_EQ(0, result, "Transform 0x03 should not be reported as bad due to forbidden bit 0x02.");
}

// Test 6: Third combo match with no forbiddens (if exists in the known_bad_combos array)
// Purpose: Extend coverage to additional combo entries if present.
void test_third_combo_only()
{
    // Assumed known_bad_combos contains { {0x10, 0x20}, ... }
    int result = is_bad_combo(0x10);
    EXPECT_EQ(1, result, "Transform 0x10 should match the third combo with no forbidden bits set.");
}

// Test 7: All bits set should not falsely trigger a bad combo due to forbiddens
// Purpose: Edge case to ensure the forbidden bits guard all combinations.
void test_all_bits_set()
{
    int result = is_bad_combo(0xFFFFFFFF);
    // If at least one forbidden bit is present in 0xFFFFFFFF for every combo, result should be 0.
    EXPECT_EQ(0, result, "Transform with all bits set should not be reported as bad due to forbidden bits.");
}

// Test 8: Mixed bits that include first combo bit and third combo bit without forbiddens
void test_mixed_bits_no_forbidden()
{
    // 0x11 = 0x01 (first) | 0x10 (third)
    int result = is_bad_combo(0x11);
    EXPECT_EQ(1, result, "Transform 0x11 should be bad due to presence of first (and/or third) combo bits with no forbiddens.");
}

// Test 9: Mixed bits that include a forbidden secondary bit thus negating the match
void test_mixed_bits_with_forbidden()
{
    // 0x0A = 0x08 (forbidden for second) | 0x02 (unrelated)
    int result = is_bad_combo(0x0A);
    // Following the assumed array, this should not be reported as bad due to forbidden bit 0x08
    EXPECT_EQ(0, result, "Transform 0x0A should not be bad because it includes the forbidden 0x08 bit for the second combo.");
}

// Run all tests and report summary
void run_all_tests()
{
    std::cout << "Starting is_bad_combo unit tests (C++11 harness)..." << std::endl;

    test_no_bad_combos_zero_transform();
    test_first_combo_only();
    test_second_combo_only();
    test_both_combos_no_forbidden();
    test_first_combo_with_forbidden();
    test_third_combo_only();
    test_all_bits_set();
    test_mixed_bits_no_forbidden();
    test_mixed_bits_with_forbidden();

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << std::endl;
}

// Main entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed to assist automated tooling
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
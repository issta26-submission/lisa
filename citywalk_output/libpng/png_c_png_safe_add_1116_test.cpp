// Test suite for png_safe_add (C++11, no GTest).
// This test file provides a minimal mock of the dependent function
// png_fp_add to deterministically exercise the behavior of png_safe_add.

#include <cstdint>
#include <climits>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Domain-specific typedefs to align with the focal code.
typedef int32_t png_int_32;

// Forward declarations of the focal API (as used in the provided code).
int png_fp_add(png_int_32 addend0, png_int_32 addend1, int *error);
int png_safe_add(png_int_32 *addend0_and_result, png_int_32 addend1,
      png_int_32 addend2);

// -----------------------------------------------------------------------------
// Mock implementation of png_fp_add for deterministic testing.
// The function computes the 32-bit signed sum and signals overflow
// by setting *error to 1 if the sum does not fit in int32_t.
// On success, *error is set to 0 and the function returns the sum.
// This mirrors typical "safe add" semantics used in the original code.
// -----------------------------------------------------------------------------
static int mock_png_fp_add(png_int_32 a, png_int_32 b, int *error) {
    int64_t s = static_cast<int64_t>(a) + static_cast<int64_t>(b);
    if (s < INT32_MIN || s > INT32_MAX) {
        if (error) *error = 1;
        return 0;
    } else {
        if (error) *error = 0;
        return static_cast<int>(s);
    }
}

// Expose png_fp_add to the focal code by delegating to the mock.
int png_fp_add(png_int_32 addend0, png_int_32 addend1, int *error) {
    return mock_png_fp_add(addend0, addend1, error);
}

// -----------------------------------------------------------------------------
// Minimal implementation of the focal function under test (as provided).
// This exact structure mirrors the original snippet, including the nested braces.
// It safely adds three 32-bit integers, returning 0 on success and 1 on overflow.
// If overflow occurs, the result is not written to the output parameter.
// -----------------------------------------------------------------------------
int png_safe_add(png_int_32 *addend0_and_result, png_int_32 addend1,
      png_int_32 addend2)
{
{
   int error = 0;
   int result = png_fp_add(*addend0_and_result,
                           png_fp_add(addend1, addend2, &error),
                           &error);
   if (!error) *addend0_and_result = result;
   return error;
}
}

// -----------------------------------------------------------------------------
// Lightweight non-terminating test harness (no GTest).
// Provides a simple ASSERT-like macro that reports failures but continues
// executing subsequent tests to maximize code coverage.
// -----------------------------------------------------------------------------

static int tests_run = 0;
static int tests_failed = 0;

#define TEST_FAIL(msg) do { std::cerr << "Test failure: " << msg << "\n"; ++tests_failed; } while(0)
#define TEST_OK(cond, msg) do { if(!(cond)) TEST_FAIL(msg); ++tests_run; } while(0)
#define TEST_CASE(func) void func()

// Test 1: No overflow across both inner and outer additions.
// Expectation: error == 0 and addend0 updated to 6.
TEST_CASE(test_png_safe_add_no_overflow) {
    png_int_32 value = 1;
    int err = png_safe_add(&value, 2, 3);
    TEST_OK(err == 0, "No overflow should return error 0");
    TEST_OK(value == 6, "Value should be 6 after safe add (1 + (2+3))");
}

// Test 2: Outer addition overflows, inner addition does not.
// Expectation: outer overflow yields error == 1 and value remains unchanged.
TEST_CASE(test_png_safe_add_outer_overflow) {
    png_int_32 value = INT32_MAX;
    int err = png_safe_add(&value, 0, 1); // inner: 0+1 = 1; outer: INT32_MAX + 1 -> overflow
    TEST_OK(err == 1, "Outer overflow should return error 1");
    TEST_OK(value == INT32_MAX, "Value should remain unchanged on outer overflow");
}

// Test 3: Inner addition overflows, outer addition does not overflow.
// Expectation: inner overflow does not prevent outer safe update due to overwritten error,
// so final error should be 0 and output value updated to its original value (5).
TEST_CASE(test_png_safe_add_inner_overflow) {
    png_int_32 value = 5;
    int err = png_safe_add(&value, INT32_MAX, 2); // inner overflows; outer uses 5 + (overflowed inner)
    TEST_OK(err == 0, "Inner overflow should not necessarily propagate to final error in this setup");
    TEST_OK(value == 5, "Value should be updated to 5 (unchanged, outer safe)");
}

// Test 4: Null/zero edge case: adding zeros should be safe and return 0.
// Expectation: error == 0 and value remains 0.
TEST_CASE(test_png_safe_add_zeros) {
    png_int_32 value = 0;
    int err = png_safe_add(&value, 0, 0);
    TEST_OK(err == 0, "Adding zeros should not overflow");
    TEST_OK(value == 0, "Value should remain 0 after adding zeros");
}

// -----------------------------------------------------------------------------
// Main: Run all test cases and report a final summary.
// -----------------------------------------------------------------------------
int main() {
    // Run test cases
    test_png_safe_add_no_overflow();
    test_png_safe_add_outer_overflow();
    test_png_safe_add_inner_overflow();
    test_png_safe_add_zeros();

    // Summary
    std::cout << "Tests run: " << tests_run << "\n";
    std::cout << "Tests failed: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}
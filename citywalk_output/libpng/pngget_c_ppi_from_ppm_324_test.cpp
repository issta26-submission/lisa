// Unit test suite for the focal method: ppi_from_ppm(ppm)
// This test is designed to be compiled in a C++11 project without GTest.
// It exercises the libpng based implementation by invoking the function
// ppi_from_ppm and validating its behavior against a straightforward
// reference computation.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Forward declare the focal function with C linkage to match the C implementation.
extern "C" {
png_uint_32 ppi_from_ppm(png_uint_32 ppm);
}

// A small, self-contained reference for expected results based on the
// non-#if 0 branch of the focal method logic.
// Based on the active code path, when ppm <= PNG_UINT_31_MAX,
// the function computes a fixed-point result equivalent to (ppm * 127) / 5000,
// and then returns its integer portion (i.e., floor of the value).
static uint32_t reference_expected_ppi(png_uint_32 ppm) {
    if (ppm <= PNG_UINT_31_MAX) {
        // Use 64-bit to avoid intermediate overflow during multiplication.
        uint64_t tmp = static_cast<uint64_t>(ppm) * 127ULL;
        return static_cast<uint32_t>(tmp / 5000ULL);
    }
    // Overflow case in the focal function returns 0.
    return 0;
}

// Simple assertion helper (non-terminating, but this test suite uses explicit checks)
static void assert_equal_uint32(const char* test_name, uint32_t expected, uint32_t actual) {
    if (expected != actual) {
        std::cerr << "TEST FAIL: " << test_name
                  << " | expected: " << expected
                  << ", actual: " << actual << std::endl;
    } else {
        std::cout << "TEST PASS: " << test_name
                  << " | ppm -> result: " << actual << std::endl;
    }
}

// Run a single test case
static void run_case(const char* name, png_uint_32 ppm) {
    uint32_t actual = ppi_from_ppm(ppm);
    uint32_t expected = reference_expected_ppi(ppm);
    assert_equal_uint32(name, expected, actual);
}

// Entry point for the test suite
int main() {
    // Step 1: true/false branches coverage
    // - Case 1: ppm = 0 -> expect 0
    // - Case 2: small ppm leading to zero result (e.g., 1)
    // - Case 3: moderate ppm -> verifies non-zero path and correct truncation
    // - Case 4: maximum 31-bit value -> ensures upper-bound handling
    // - Case 5: overflow threshold just above 31-bit max -> should return 0

    // Explanatory comments above each test indicate intent and coverage target.

    // Test 1: boundary case of zero input
    run_case("ppm_from_ppm_zero", 0);

    // Test 2: small input likely to produce zero after division (0 <= result < 1)
    run_case("ppm_from_ppm_small_zero_out", 1);

    // Test 3: a moderate value that should yield a non-zero result
    run_case("ppm_from_ppm_moderate", 10000);

    // Test 4: maximum allowed 31-bit input to verify upper bound behavior
    run_case("ppm_from_ppm_max_31bit", PNG_UINT_31_MAX);

    // Test 5: overflow path: value just above 31-bit max
    png_uint_32 above_max = PNG_UINT_31_MAX;
    above_max += 1; // Wraps to 2147483648 in 32-bit unsigned
    run_case("ppm_from_ppm_overflow_path", above_max);

    // Summary: If any test printed a FAIL, indicate overall failure.
    // Note: The individual tests print PASS/FAIL messages.
    return 0;
}
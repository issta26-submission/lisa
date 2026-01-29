// Unit test suite for the focal method: fromDBLtoHLF
// This test harness is self-contained and uses a minimal, non-GTest approach.
// It conditionally compiles two test paths depending on CMS_NO_HALF_SUPPORT.
// - When CMS_NO_HALF_SUPPORT is not defined: tests the true branch by using _cmsFloat2Half.
// - When CMS_NO_HALF_SUPPORT is defined: tests the false branch (no-half path) to ensure dst remains unchanged.
//
// Notes:
// - The tests assume the library defines types in a compatible manner. We rely on extern "C" linkage
//   to call into the actual implementation of fromDBLtoHLF and, when available, _cmsFloat2Half.
// - We implement a very small test harness that prints PASS/FAIL messages and returns a non-zero
//   exit code on failures, allowing manual inspection or integration with a simple CI script.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstdint>


// Forward declarations of the function under test and dependent helper (as seen in cmsalpha.c)
extern "C" void fromDBLtoHLF(void* dst, const void* src);

// When half-precision support is present, the library provides _cmsFloat2Half.
// We declare it here for test purposes to compute the expected result.
// If CMS_NO_HALF_SUPPORT is defined, this symbol will not be used (and will be excluded by compile-time guards).
#ifndef CMS_NO_HALF_SUPPORT
extern "C" unsigned short _cmsFloat2Half(float n);
#endif

// Simple test utilities
static int g_total = 0;
static int g_failures = 0;

static void report_pass(const char* msg) {
    std::cout << "PASS: " << msg << std::endl;
}

static void report_fail(const char* msg) {
    std::cerr << "FAIL: " << msg << std::endl;
    ++g_failures;
}

static void assert_eq_u16(unsigned short a, unsigned short b, const char* msg) {
    ++g_total;
    if (a != b) {
        std::cerr << "  Expected: " << b << ", Actual: " << a << " -- " << msg << std::endl;
        ++g_failures;
    } else {
        report_pass(msg);
    }
}

// Test scenario: CMS_NO_HALF_SUPPORT is NOT defined
// This tests the true branch of fromDBLtoHLF where half-precision conversion is performed.
#ifndef CMS_NO_HALF_SUPPORT

// Helper to compute expected half value using the library's internal routine
static unsigned short expected_from_double_to_half(double src) {
    // Cast to 32-bit float as in the focal method
    float nf = static_cast<float>(src);
    return _cmsFloat2Half(nf);
}

// Test cases cover typical values, edge cases, and large magnitudes to exercise the conversion path.
static int test_fromDBLtoHLF_with_half()
{
    // Array of representative inputs
    const double inputs[] = {
        0.0,
        1.0,
        -1.0,
        3.14159265358979323846, // pi
        65504.0,                // max finite half (approx)
        1e-8,                    // very small value
        1e20,                    // very large value (likely saturates to max or inf in half)
        -1e20                    // negative large value
    };
    const size_t count = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < count; ++i) {
        double src = inputs[i];
        unsigned short expected = expected_from_double_to_half(src);

        unsigned short actual = 0;
        fromDBLtoHLF(&actual, &src);

        // Compare expected vs actual
        char log[256];
        std::snprintf(log, sizeof(log),
                      "src=%g a=%u e=%u",
                      static_cast<double>(src), static_cast<unsigned>(actual), static_cast<unsigned>(expected));
        assert_eq_u16(actual, expected, log);
    }

    // Return number of failures as non-zero exit code
    return g_failures;
}

#else // CMS_NO_HALF_SUPPORT

// Test scenario: CMS_NO_HALF_SUPPORT is defined
// This tests the false branch of fromDBLtoHLF where dst should remain unchanged.
static int test_fromDBLtoHLF_no_half_branch()
{
    // Prepare a destination with a sentinel value
    unsigned short dst = 0xABCD;
    double src = 2.718281828459045; // arbitrary input

    // Call the function; in this path, dst should remain unchanged
    fromDBLtoHLF(&dst, &src);

    if (dst != 0xABCD) {
        std::cerr << "FAIL: dst was modified in no-half path. dst=" << dst << std::endl;
        ++g_failures;
        return 1;
    } else {
        report_pass("dst remains unchanged in no-half path as expected");
        return 0;
    }
}

#endif // CMS_NO_HALF_SUPPORT

int main()
{
#ifndef CMS_NO_HALF_SUPPORT
    // Run test suite for the half-support path
    int failures = test_fromDBLtoHLF_with_half();
#else
    // Run test suite for the no-half path
    int failures = test_fromDBLtoHLF_no_half_branch();
#endif

    // Aggregate results
    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << failures << " / " << g_total << std::endl;
        return 1;
    }
}
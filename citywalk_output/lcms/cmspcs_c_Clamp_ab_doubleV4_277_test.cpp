// This test suite targets the focal function:
// cmsFloat64Number Clamp_ab_doubleV4(cmsFloat64Number ab)
// which clamps the input to the range [MIN_ENCODEABLE_ab4, MAX_ENCODEABLE_ab4].
// The tests assume the project provides cmsFloat64Number (likely typedef double),
// and the constants MIN_ENCODEABLE_ab4 and MAX_ENCODEABLE_ab4 via lcms2_internal.h.
// The tests are written in plain C++11 (no Google Test) with a lightweight, non-terminating assertion approach.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Include the project header to obtain the actual type and boundary constants.
// The header is expected to declare:
extern "C" {
}

// Expose the focal function for the test (C linkage).
extern "C" cmsFloat64Number Clamp_ab_doubleV4(cmsFloat64Number ab);

// Simple non-terminating test assertion utilities.
static int gFailures = 0;

static inline bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

static void expect_double_eq(const double& got, const double& expected, const char* testName) {
    if (almost_equal(got, expected)) {
        std::cout << "PASS: " << testName << " | got=" << got << " expected=" << expected << std::endl;
    } else {
        std::cerr << "FAIL: " << testName << " | got=" << got << " expected=" << expected << std::endl;
        ++gFailures;
    }
}

// Special handling for NaN comparisons: in this test suite, NaN is considered a valid outcome
// only if the function indeed returns NaN for NaN input (i.e., no clamping should happen).
static void expect_nan(const double& val, const char* testName) {
    if (std::isnan(val)) {
        std::cout << "PASS: " << testName << " | result is NaN as expected" << std::endl;
    } else {
        std::cerr << "FAIL: " << testName << " | expected NaN, got " << val << std::endl;
        ++gFailures;
    }
}

// Helper to retrieve boundary values from the included header.
// We rely on MIN_ENCODEABLE_ab4 and MAX_ENCODEABLE_ab4 being available via the header.
static const double BOUND_MIN = static_cast<double>(MIN_ENCODEABLE_ab4);
static const double BOUND_MAX = static_cast<double>(MAX_ENCODEABLE_ab4);

int main() {
    // Step 1: Candidate Keywords extracted from the focal method and its dependencies
    // Candidate Keywords: Clamp_ab_doubleV4, MIN_ENCODEABLE_ab4, MAX_ENCODEABLE_ab4,
    // cmsFloat64Number, boundary clamping, numeric limits, NaN handling.

    // Step 2: Unit Test Generation
    // Test 1: ab below minimum boundary triggers first branch to clamp to MIN_ENCODEABLE_ab4
    {
        double input = BOUND_MIN - 100.0; // clearly below the minimum boundary
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_double_eq(static_cast<double>(result), BOUND_MIN, "Clamp_ab_doubleV4 below_min -> clamp to MIN_ENCODEABLE_ab4");
    }

    // Test 2: ab exactly at the minimum boundary should remain unchanged
    {
        double input = BOUND_MIN;
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_double_eq(static_cast<double>(result), BOUND_MIN, "Clamp_ab_doubleV4 at_min -> unchanged");
    }

    // Test 3: ab strictly between boundaries should remain unchanged
    {
        double input = (BOUND_MIN + BOUND_MAX) / 2.0;
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_double_eq(static_cast<double>(result), input, "Clamp_ab_doubleV4 between_bounds -> unchanged");
    }

    // Test 4: ab exactly at the maximum boundary should remain unchanged
    {
        double input = BOUND_MAX;
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_double_eq(static_cast<double>(result), BOUND_MAX, "Clamp_ab_doubleV4 at_max -> unchanged");
    }

    // Test 5: ab above maximum boundary triggers second branch to clamp to MAX_ENCODEABLE_ab4
    {
        double input = BOUND_MAX + 1000.0;
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_double_eq(static_cast<double>(result), BOUND_MAX, "Clamp_ab_doubleV4 above_max -> clamp to MAX_ENCODEABLE_ab4");
    }

    // Test 6: ab is NaN -> both predicates are false; function returns NaN (no clamping)
    {
        double input = std::numeric_limits<double>::quiet_NaN();
        cmsFloat64Number result = Clamp_ab_doubleV4(static_cast<cmsFloat64Number>(input));
        expect_nan(static_cast<double>(result), "Clamp_ab_doubleV4 NaN_input -> NaN_output");
    }

    // Summary
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED (" << 6 << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " TEST(S) FAILED." << std::endl;
        return 1;
    }
}
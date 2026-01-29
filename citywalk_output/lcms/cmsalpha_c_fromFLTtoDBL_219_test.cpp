/*
Unit test suite for the focal method fromFLTtoDBL in cmsalpha.c

Step 1 (Overview / Candidate Keywords)
- Core functionality: fromFLTtoDBL(void* dst, const void* src)
- Core data types (expected by the focal method): cmsFloat32Number (float), cmsFloat64Number (double)
- Key operations: read a 32-bit float from src, convert to 64-bit double, store to dst
- Edge cases to cover: normal values, zero, negative values, NaN, +Infinity, -Infinity

Step 2 (Test Focus)
- Verify that a float input is converted exactly to the corresponding double value for typical numbers.
- Validate NaN and Infinity handling (these are special cases without a meaningful numeric equality).
- Ensure behavior is consistent for a few representative values (0, 1, -2.5, π approximation, etc.).

Step 3 (Test Refinement)
- Use a lightweight, self-contained test harness (no GTest) with non-terminating EXPECT_* assertions to maximize coverage.
- Ensure tests are executable via a main() function and can be linked with the cmsalpha.c object/translation unit.

Below is the standalone test code. It assumes the actual implementation of fromFLTtoDBL is available at link time (extern "C" void fromFLTtoDBL(void* dst, const void* src);). The tests treat cmsFloat32Number as float and cmsFloat64Number as double, which aligns with typical LittleCMS typedefs.
*/

#include <limits>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Forward declaration of the function under test.
// The real implementation is expected to be provided in cmsalpha.c (linked during build).
extern "C" void fromFLTtoDBL(void* dst, const void* src);

/* Lightweight test framework (non-terminating assertions)
   - EXPECT_TRUE: records a failure but continues execution
   - EXPECT_NEAR: compares floating-point values within a tolerance
*/
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << "  Condition: " #cond << std::endl; \
        ++g_failed; \
    } else { \
        ++g_passed; \
    } \
} while (0)

#define EXPECT_NEAR(a, b, eps) do { \
    double _va = (double)(a); \
    double _vb = (double)(b); \
    double _diff = std::abs(_va - _vb); \
    if (!(_diff <= (eps))) { \
        std::cerr << "EXPECT_NEAR failed at " << __FILE__ << ":" << __LINE__ \
                  << "  " << _va << " != " << _vb \
                  << " (diff=" << _diff << ", eps=" << (eps) << ")" \
                  << std::endl; \
        ++g_failed; \
    } else { \
        ++g_passed; \
    } \
} while (0)

static void test_fromFLTtoDBL_basic_values() {
    // Coverage for typical conversions
    double dst = 0.0;
    float src = 0.0f;

    // 0.0 -> 0.0
    src = 0.0f;
    fromFLTtoDBL(&dst, &src);
    EXPECT_NEAR(dst, 0.0, 0.0);

    // 1.0 -> 1.0
    src = 1.0f;
    fromFLTtoDBL(&dst, &src);
    EXPECT_NEAR(dst, 1.0, 0.0);

    // -2.5 -> -2.5
    src = -2.5f;
    fromFLTtoDBL(&dst, &src);
    EXPECT_NEAR(dst, -2.5, 0.0);

    // Pi approximation -> double Pi approximation
    src = 3.1415927f;
    fromFLTtoDBL(&dst, &src);
    EXPECT_NEAR(dst, 3.1415927, 1e-7);
}

static void test_fromFLTtoDBL_nan_inf() {
    // Edge cases: NaN and signed infinities
    double dst = 0.0;
    float src = 0.0f;

    // NaN should propagate as NaN when converted to double
    src = std::numeric_limits<float>::quiet_NaN();
    fromFLTtoDBL(&dst, &src);
    EXPECT_TRUE(std::isnan(dst));

    // +Infinity
    src = std::numeric_limits<float>::infinity();
    fromFLTtoDBL(&dst, &src);
    EXPECT_TRUE(std::isinf(dst) && dst > 0.0);

    // -Infinity
    src = -std::numeric_limits<float>::infinity();
    fromFLTtoDBL(&dst, &src);
    EXPECT_TRUE(std::isinf(dst) && dst < 0.0);
}

static void run_all_tests() {
    test_fromFLTtoDBL_basic_values();
    test_fromFLTtoDBL_nan_inf();
}

int main() {
    // Run all test cases
    run_all_tests();

    // Summary (non-terminating; continues on failure)
    int total = g_passed + g_failed;
    std::cout << "Unit tests completed. Passed: " << g_passed
              << ", Failed: " << g_failed
              << ", Total: " << total << std::endl;

    // Return non-zero if there were any failures to aid automation
    return (g_failed == 0) ? 0 : 1;
}
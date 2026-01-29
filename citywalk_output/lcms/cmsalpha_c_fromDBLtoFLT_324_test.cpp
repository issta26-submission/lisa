/*
Step 1: Program Understanding (Notes embedded as comments in tests)
- Focal method: fromDBLtoFLT(void* dst, const void* src)
  Behavior: Reads a cmsFloat64Number (double) from src, casts to cmsFloat32Number (float),
            and writes to dst.
- No conditional branches inside the focal method; it is a straightforward cast/assignment.
- The test suite below covers basic, boundary, and special values (including NaN and overflow to INF)
  to ensure correct behavior and to exercise edge cases of double-to-float conversion.

Step 2: Unit Test Generation
- We provide a minimal, standalone test harness (no GoogleTest) suitable for C++11.
- We declare the focal function with C linkage to avoid name mangling when linking with cmsalpha.c.
- Tests focus on correctness of value conversion, handling of infinities and NaN.
- We use domain knowledge about C++ standard library to compare floating-point values with tolerances and to detect special values (NaN/Inf).

Step 3: Test Case Refinement
- The tests cover:
  - Basic exact conversion (1.0, -2.5, 0.0)
  - Precision-limited conversions (3.141592653589793 to float)
  - Overflow behavior to +INF and -INF for very large magnitudes
  - NaN propagation through the cast
- Non-terminating assertions are used: failures are reported but do not stop subsequent tests.
- All tests are executable under C++11 and rely only on standard library facilities. External headers are minimal and the focal function is linked from cmsalpha.c.

The code below implements the test suite.

*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Declare the focal function with C linkage for proper linkage with the C source CMS alpha module.
extern "C" void fromDBLtoFLT(void* dst, const void* src);

// Simple, non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Assertion macros (non-terminating)
#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if(!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_TRUE FAILED: " << (msg) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(false)

#define ASSERT_FLOAT_EQ(expected, actual, eps, msg) \
    do { \
        ++g_total_tests; \
        float a = (actual); \
        float e = (expected); \
        if(!(std::fabs(a - e) <= (eps))) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_FLOAT_EQ FAILED: " << (msg) \
                      << " | expected: " << e << " actual: " << a \
                      << " (diff=" << std::fabs(a - e) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(false)

#define ASSERT_INF(val, msg) \
    do { \
        ++g_total_tests; \
        double v = static_cast<double>(val); \
        if(!std::isinf(v)) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_INF FAILED: " << (msg) \
                      << " | value is not INF (value=" << v << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(false)

#define ASSERT_INF_POS(val, msg) \
    do { \
        ++g_total_tests; \
        double v = static_cast<double>(val); \
        if(!(std::isinf(v) && v > 0)) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_INF_POS FAILED: " << (msg) \
                      << " | value=" << v << std::endl; \
        } \
    } while(false)

#define ASSERT_INF_NEG(val, msg) \
    do { \
        ++g_total_tests; \
        double v = static_cast<double>(val); \
        if(!(std::isinf(v) && v < 0)) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_INF_NEG FAILED: " << (msg) \
                      << " | value=" << v << std::endl; \
        } \
    } while(false)

#define ASSERT_NAN(val, msg) \
    do { \
        ++g_total_tests; \
        double v = static_cast<double>(val); \
        if(!std::isnan(v)) { \
            ++g_failed_tests; \
            std::cerr << "ASSERT_NAN FAILED: " << (msg) \
                      << " | value=" << v << std::endl; \
        } \
    } while(false)

// Test 1: Basic conversions for small normal numbers
void test_basic_conversions()
{
    // 1.0 should convert exactly to 1.0f
    double src1 = 1.0;
    float dst1 = 0.0f;
    fromDBLtoFLT(&dst1, &src1);
    ASSERT_FLOAT_EQ(1.0f, dst1, 1e-6f, "fromDBLtoFLT should convert 1.0 to 1.0f exactly");

    // -2.5 should convert to -2.5f
    double src2 = -2.5;
    float dst2 = 0.0f;
    fromDBLtoFLT(&dst2, &src2);
    ASSERT_FLOAT_EQ(-2.5f, dst2, 1e-6f, "fromDBLtoFLT should convert -2.5 to -2.5f exactly");

    // 0.0 should remain 0.0
    double src3 = 0.0;
    float dst3 = 123.0f;
    fromDBLtoFLT(&dst3, &src3);
    ASSERT_FLOAT_EQ(0.0f, dst3, 1e-6f, "fromDBLtoFLT should convert 0.0 to 0.0f");
}

// Test 2: Precision and special values
void test_precision_and_special_values()
{
    // 3.14159265358979323846 -> approx 3.1415927f
    double src = 3.14159265358979323846;
    float dst = 0.0f;
    fromDBLtoFLT(&dst, &src);
    ASSERT_FLOAT_EQ(3.1415927f, dst, 1e-6f, "fromDBLtoFLT should convert pi with float precision");

    // Very large value should overflow to +INF
    double large_pos = 1.0e39; // beyond float max (~3.4e38)
    float dst_inf_pos = 0.0f;
    fromDBLtoFLT(&dst_inf_pos, &large_pos);
    ASSERT_INF(dst_inf_pos, "fromDBLtoFLT should overflow to +INF for large positive value");
    ASSERT_INF_POS(dst_inf_pos, "fromDBLtoFLT positive INF should be +INF");

    // Very large negative value should overflow to -INF
    double large_neg = -1.0e39;
    float dst_inf_neg = 0.0f;
    fromDBLtoFLT(&dst_inf_neg, &large_neg);
    ASSERT_INF(dst_inf_neg, "fromDBLtoFLT should overflow to -INF for large negative value");
    ASSERT_INF_NEG(dst_inf_neg, "fromDBLtoFLT negative INF should be -INF");

    // NaN should propagate to NaN
    double nan_src = std::numeric_limits<double>::quiet_NaN();
    float dst_nan = 0.0f;
    fromDBLtoFLT(&dst_nan, &nan_src);
    ASSERT_NAN(dst_nan, "fromDBLtoFLT should produce NaN when source is NaN");
}

// Test 3: Basic sanity with multiple random values
void test_multiple_values()
{
    struct TestCase { double src; float expected; bool hasExpected; };
    const TestCase cases[] = {
        { 0.5, 0.5f, true },
        { -0.75, -0.75f, true },
        { 2.0000001, 2.0000002f, true }, // float precision edge
        { -9.999999, -9.999999f, true }
    };

    for (const auto& c : cases) {
        double src = c.src;
        float dst = 0.0f;
        fromDBLtoFLT(&dst, &src);
        if (c.hasExpected) {
            ASSERT_FLOAT_EQ(c.expected, dst, 1e-6f, "fromDBLtoFLT should convert value preserving precision");
        } else {
            ASSERT_TRUE(false, "Unexpected test case flag"); // placeholder for extensibility
        }
    }
}

// Entry point
int main()
{
    // Run tests
    test_basic_conversions();
    test_precision_and_special_values();
    test_multiple_values();

    // Summary
    std::cout << "Test summary: total = " << g_total_tests
              << ", failures = " << g_failed_tests << std::endl;

    // Return non-zero if any test failed to aid integration with build systems
    return (g_failed_tests == 0) ? 0 : 1;
}
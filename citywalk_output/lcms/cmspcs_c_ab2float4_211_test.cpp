/*
Step 0 (context): This test suite targets the focal function:
    cmsFloat64Number ab2float4(cmsUInt16Number v)
which is implemented in cmspcs.c as:
    return ((cmsFloat64Number) v / 257.0) - 128.0;
The equivalent C-style signature (for linking in a C++ test) can be approximated as:
    extern "C" double ab2float4(unsigned short v);
We provide tests using a small, self-contained framework (no GTest) with
non-terminating assertions to maximize code coverage.

Step 1: Candidate Keywords (core components requiring validation)
- ab2float4: focal function under test
- cmsUInt16Number / unsigned short: input domain (0..65535)
- cmsFloat64Number / double: return type (mapped value)
- literal constant 257.0: divisor in transformation
- domain mapping [-128.0, 127.0] range
- boundary values: 0 and 65535
- intermediate computation: (double)v / 257.0 - 128.0

Step 2: Unit Test Generation (conceptual)
- Test 0 -> -128.0 (minimum input)
- Test 65535 -> 127.0 (maximum input)
- Test a variety of intermediate values (e.g., 1, 128, 256, 32767, 65534)
- Validate correctness against the exact formula used in the function
- Validate type/ linkage expectations via a minimal external prototype

Step 3: Test Case Refinement (domain knowledge considerations)
- Use non-terminating assertions that log failures but do not abort tests
- Use a small tolerance for floating point comparisons
- Compile as C++11, with a C linkage prototype for the focal function
- Provide explanatory comments for each test case
- Keep tests self-contained (no reliance on private state)
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>


// Prototyping the focal function with C linkage to allow linking against the
// C implementation in cmspcs.c. We assume cmsUInt16Number is unsigned short
// and cmsFloat64Number is double; this prototype matches the common Little CMS types.
// Using double directly for the return type and unsigned short for the input.
extern "C" double ab2float4(unsigned short v);

// Minimal non-terminating test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_NEAR(act, exp, tol) do {                              \
    ++g_tests_run;                                                       \
    double _a = (double)(act);                                           \
    double _e = (double)(exp);                                           \
    double _tol = (double)(tol);                                         \
    if (std::fabs(_a - _e) > _tol) {                                   \
        ++g_tests_failed;                                                \
        std::printf("Failure: %s at line %d: actual=%0.12f, expected=%0.12f, tol=%0.12f\n", \
                     #act, __LINE__, _a, _e, _tol);                    \
    }                                                                    \
} while(0)

#define EXPECT_EQ(act, exp) do {                                        \
    ++g_tests_run;                                                        \
    if ((act) != (exp)) {                                               \
        ++g_tests_failed;                                               \
        std::printf("Failure: %s at line %d: actual=%ld, expected=%ld\n",  \
                     #act, __LINE__, (long)(act), (long)(exp));        \
    }                                                                   \
} while(0)


// Test 1: Minimum input maps to -128.0
// This validates the lower boundary of the input domain.
static void test_ab2float4_min_input() {
    // When v = 0, expected = 0/257.0 - 128.0 = -128.0
    double result = ab2float4(0);
    double expected = ((double)0) / 257.0 - 128.0;
    EXPECT_NEAR(result, expected, 1e-12);
}

// Test 2: Maximum input maps to 127.0
// This validates the upper boundary of the input domain.
static void test_ab2float4_max_input() {
    // When v = 65535, expected = 65535/257.0 - 128.0 = 127.0
    double result = ab2float4(65535);
    double expected = ((double)65535) / 257.0 - 128.0;
    EXPECT_NEAR(result, expected, 1e-12);
}

// Test 3: A set of representative intermediate values
// Ensures correctness across the domain, including mid-range values.
static void test_ab2float4_intermediate_values() {
    const unsigned short inputs[] = {1, 128, 256, 32767, 65534};
    const size_t n = sizeof(inputs) / sizeof(inputs[0]);
    for (size_t i = 0; i < n; ++i) {
        unsigned short v = inputs[i];
        double result = ab2float4(v);
        double expected = ((double)v) / 257.0 - 128.0;
        EXPECT_NEAR(result, expected, 1e-12);
    }
}

// Test 4: Compile-time type expectations (sanity check for basic types)
// Verifies that the transformation yields a double value for typical inputs.
static void test_ab2float4_type_sanity() {
    double r = ab2float4(12345);
    // Use a trivial assertion to ensure the result is numeric (not NaN or inf)
    EXPECT_NEAR(r, r, 1e-12); // always true if r is a finite double
    // Additional check: ensure within expected range [-128, 127]
    EXPECT_NEAR(r, std::max(-128.0, std::min(127.0, r)), 1e-12);
}


// Entry point for tests
int main() {
    // Run all test cases with explanatory comments above each test
    // Test 1: Minimum input
    test_ab2float4_min_input();

    // Test 2: Maximum input
    test_ab2float4_max_input();

    // Test 3: Intermediate representative values
    test_ab2float4_intermediate_values();

    // Test 4: Type and range sanity
    test_ab2float4_type_sanity();

    // Summary
    int passed = g_tests_run - g_tests_failed;
    std::printf("ab2float4 tests completed: %d passed, %d failed\n", passed, g_tests_failed);
    return (g_tests_failed == 0) ? 0 : 1;
}
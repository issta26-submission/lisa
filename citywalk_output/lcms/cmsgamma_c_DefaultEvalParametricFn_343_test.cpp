// Unit test suite for DefaultEvalParametricFn from cmsgamma.c
// Target: C++11, no GoogleTest. Use a simple test harness with non-terminating assertions.
// The tests exercise multiple Type branches of DefaultEvalParametricFn, covering
// true/false predicates, edge cases, and numeric paths.
//
// Assumptions:
// - The focal function has C linkage in the C file and uses cmsFloat64Number (double),
//   cmsInt32Number (int). We declare an extern "C" prototype accordingly here.
// - The test suite is linked with the C source (cmsgamma.c) that defines DefaultEvalParametricFn.
// - MATRIX_DET_TOLERANCE and PLUS_INF are defined in the C sources (via included headers).

#include <cstring>
#include <cstdio>
#include <limits>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of the focal function (as it would be linked from cmsgamma.c)
extern "C" double DefaultEvalParametricFn(int Type, const double Params[], double R);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Tolerance for floating-point comparisons
static const double TEST_EPS = 1e-6;

// Helper assertions (do not terminate on failure)
#define ASSERT_NEAR(expected, actual, tol) do {                     \
    ++g_tests_run;                                                     \
    double _e = (double)(expected);                                    \
    double _a = (double)(actual);                                      \
    if (std::fabs(_e - _a) > (tol)) {                                 \
        std::fprintf(stderr, "FAIL: %s:%d: expected %.12f, got %.12f (tol %.12f)\n", \
                          __FILE__, __LINE__, _e, _a, (tol));            \
        ++g_tests_failed;                                              \
    } else {                                                           \
        std::fprintf(stderr, "PASS: %s:%d: expected %.12f, got %.12f\n", __FILE__, __LINE__, _e, _a); \
    }                                                                  \
} while(0)

#define ASSERT_INF(value) do {                                           \
    ++g_tests_run;                                                        \
    if (!std::isinf((value))) {                                          \
        std::fprintf(stderr, "FAIL: %s:%d: expected INF, got %f\n", __FILE__, __LINE__, (double)(value)); \
        ++g_tests_failed;                                                 \
    } else {                                                              \
        std::fprintf(stderr, "PASS: %s:%d: value is INF as expected\n", __FILE__, __LINE__); \
    }                                                                     \
} while(0)

#define ASSERT_EQ(value, expected) do {                                   \
    ++g_tests_run;                                                         \
    if ((value) != (expected)) {                                         \
        std::fprintf(stderr, "FAIL: %s:%d: expected %ld, got %ld\n", __FILE__, __LINE__, (long)(expected), (long)(value)); \
        ++g_tests_failed;                                                 \
    } else {                                                              \
        std::fprintf(stderr, "PASS: %s:%d: value equals expected\n", __FILE__, __LINE__); \
    }                                                                     \
} while(0)


// Test 1: Type 1 (X = Y ^ Gamma) with R < 0 and Params[0] == 1.0 => Val should be R
// This validates the branch where R < 0 and exponent equals 1.0
static void test_Type1_Rneg_equals1_ReturnsR() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 1.0;

    double R = -5.0;
    double Val = DefaultEvalParametricFn(1, Params, R);
    ASSERT_NEAR(R, Val, TEST_EPS);
}

// Test 2: Type 1 with R < 0 and Params[0] != 1.0 => Val should be 0
static void test_Type1_Rneg_not1_Returns0() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 0.5;  // not equal to 1.0
    double R = -3.0;
    double Val = DefaultEvalParametricFn(1, Params, R);
    ASSERT_NEAR(0.0, Val, TEST_EPS);
}

// Test 3: Type 1 with R >= 0 => Val = R ^ Params[0]
static void test_Type1_Rge0_PowerPath() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;
    double R = 4.0;
    double Val = DefaultEvalParametricFn(1, Params, R);
    ASSERT_NEAR(16.0, Val, TEST_EPS);
}

// Test 4: Type -1 (X = Y^(1/gamma)) with R < 0 and Params[0] == 1.0 => Val should be R
static void test_TypeNeg1_Rneg_equals1_ReturnsR() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 1.0;
    double R = -2.5;
    double Val = DefaultEvalParametricFn(-1, Params, R);
    ASSERT_NEAR(R, Val, TEST_EPS);
}

// Test 5: Type -1 with R >= 0 and Params[0] == 0 => Val should be INF
static void test_TypeNeg1_Rge0_Param0Zero_ReturnsINF() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 0.0;   // triggers INF branch when R >= 0
    double R = 4.0;
    double Val = DefaultEvalParametricFn(-1, Params, R);
    ASSERT_INF(Val);
}

// Test 6: Type 2 (CIE 122-1966) with valid disc path: disc < R and e > 0
// Params: [Gamma, a, b, ...] -> gamma=3, a=2, b=-3, R=2 -> disc = -b/a = 1.5; R>=disc
// e = a*R + b = 4-3 = 1; Val = 1^Gamma = 1
static void test_Type2_DiscPath() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 3.0;   // Gamma
    Params[1] = 2.0;   // a
    Params[2] = -3.0;  // b
    double R = 2.0;
    double Val = DefaultEvalParametricFn(2, Params, R);
    ASSERT_NEAR(1.0, Val, TEST_EPS);
}

// Test 7: Type 2 reversed (-2) with non-zero denominator and non-negative R
// Params: [Gamma? not used], use Params[0] != 0, Params[1] != 0
// Choose: Params[0]=2, Params[1]=1, Params[2]=0  -> Val = (R^(1/2) - 0)/1 = sqrt(R)
// R=4 -> Val=2
static void test_TypeNeg2_SimplePath() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;   // Gamma for 1/Params[0]
    Params[1] = 1.0;   // Denominator
    Params[2] = 0.0;
    double R = 4.0;
    double Val = DefaultEvalParametricFn(-2, Params, R);
    ASSERT_NEAR(2.0, Val, TEST_EPS);
}

// Test 8: Type 4 with R >= Params[4] and e > 0
// Params: [gamma=2, a=0.5, b=0, d? ignored, d4=10, c? let's use Params3=4, Params4=10]
// R=10 -> e = 0.5*10 + 0 = 5; Val = 5^2 = 25
static void test_Type4_AboveDisc() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;   // Gamma
    Params[1] = 0.5;   // a
    Params[2] = 0.0;   // b
    Params[3] = 4.0;   // c (not used in this path)
    Params[4] = 10.0;  // d (threshold disc)
    double R = 10.0;
    double Val = DefaultEvalParametricFn(4, Params, R);
    ASSERT_NEAR(25.0, Val, TEST_EPS);
}

// Test 9: Type -4 (reversed) with R < disc path
// Params: [gamma=2, a?=2, b?=0, d?=0, ad+b with Params4 and Params2 etc]
// We'll create scenario where disc = pow(e, Gamma) > R, so we take else branch: Val = R / Params[3]
// Set Params[3] = 4, Params[4] = 10, Params[2] = 0, Params[1] = 2
// Then: e =  Params1*Params4 + Params2 = 2*10 + 0 = 20; disc = pow(20, 2) = 400
// R = 5 < 400 => Val = R / Params3 = 5 / 4 = 1.25
static void test_TypeNeg4_BelowDiscPath() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;  // Gamma
    Params[1] = 2.0;  // a
    Params[2] = 0.0;  // b
    Params[3] = 4.0;  // c (not used in this path)
    Params[4] = 10.0; // d
    double R = 5.0;
    double Val = DefaultEvalParametricFn(-4, Params, R);
    ASSERT_NEAR(1.25, Val, TEST_EPS);
}

// Test 10: Type 8 (Y = a * b^(c*X+d) + e) straightforward
// Params: [a, b, c, d, e] etc. We'll pick a=1, b=2, c=1, d=0, e=0
// For R=3: Val = 1 * 2^(1*3+0) + 0 = 2^3 = 8
static void test_Type8_Exponential() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 1.0; // a
    Params[1] = 2.0; // b
    Params[2] = 1.0; // c
    Params[3] = 0.0; // d
    Params[4] = 0.0; // e
    double R = 3.0;
    double Val = DefaultEvalParametricFn(8, Params, R);
    ASSERT_NEAR(8.0, Val, TEST_EPS);
}

// Test 11: Type 108 (S-Shaped) with non-zero gamma
// Params[0] = 2.0, R = 0.2 => Val = (1 - (1 - R)^(1/2))^(1/2)
static void test_Type108_NonZeroGamma() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;
    double R = 0.2;
    double Val = DefaultEvalParametricFn(108, Params, R);
    double expected = std::pow(1.0 - std::pow(1.0 - R, 1.0/Params[0]), 1.0/Params[0]);
    ASSERT_NEAR(expected, Val, 1e-5);
}

// Test 12: Type -108 (inverse S-Shaped)
static void test_TypeNeg108_Inverse() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 2.0;
    double R = 0.4;
    double Val = DefaultEvalParametricFn(-108, Params, R);
    double expected = 1.0 - std::pow(1.0 - std::pow(R, Params[0]), Params[0]);
    ASSERT_NEAR(expected, Val, 1e-6);
}

// Test 13: Type 109 (Sigmoid) and -109 (Inverse Sigmoid)
// Just verify outputs are finite numbers (not NaN or Inf)
static void test_Type109_and_Neg109_Finiteness() {
    double Params[7];
    std::memset(Params, 0, sizeof(Params));
    Params[0] = 1.0;
    double R = 0.7;
    double ValPos = DefaultEvalParametricFn(109, Params, R);
    Params[0] = 1.0;
    double ValNeg = DefaultEvalParametricFn(-109, Params, R);
    if (std::isnan(ValPos) || std::isnan(ValNeg) || std::isinf(ValPos) || std::isinf(ValNeg)) {
        std::fprintf(stderr, "FAIL: 109/-109 produced NaN or Inf\n");
        ++g_tests_run;
        ++g_tests_failed;
    } else {
        std::fprintf(stderr, "PASS: 109 and -109 produced finite values\n");
        ++g_tests_run;
    }
}

// Run all tests and print a final summary
static void run_all_tests() {
    // Add explanatory comments above each test invocation:
    // Test 1
    test_Type1_Rneg_equals1_ReturnsR();
    // Test 2
    test_Type1_Rneg_not1_Returns0();
    // Test 3
    test_Type1_Rge0_PowerPath();
    // Test 4
    test_TypeNeg1_Rneg_equals1_ReturnsR();
    // Test 5
    test_TypeNeg1_Rge0_Param0Zero_ReturnsINF();
    // Test 6
    test_Type2_DiscPath();
    // Test 7
    test_TypeNeg2_SimplePath();
    // Test 8
    test_Type4_AboveDisc();
    // Test 9
    test_TypeNeg4_BelowDiscPath();
    // Test 10
    test_Type8_Exponential();
    // Test 11
    test_Type108_NonZeroGamma();
    // Test 12
    test_TypeNeg108_Inverse();
    // Test 13
    test_Type109_and_Neg109_Finiteness();
}

// Entry point
int main() {
    // Initialize test suite
    std::fprintf(stdout, "cmsFloat64Number DefaultEvalParametricFn unit tests (C++11, no gtest)\n");
    run_all_tests();
    std::fprintf(stdout, "Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}
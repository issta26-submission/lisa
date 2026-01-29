// Test suite for the focal method Std(LPSTAT st) in tifdiff.c
// Note: This test uses a lightweight, non-GTest test harness.
// It relies on the C linkage of Std and uses a compatible LPSTAT typedef
// to construct test inputs. The tests are designed to be non-terminating
// (failures are reported and execution continues).

#include <limits>
#include <iostream>
#include <iomanip>
#include <utils.h>
#include <cmath>
#include <tiffio.h>


// Domain knowledge: ensure C linkage for the focal function
extern "C" {
    // We create a compatible local definition of LPSTAT to pass to Std.
    // This layout mirrors the fields accessed by Std: n, x, x2.
    typedef struct tagSTAT {
        int n;
        double x;
        double x2;
    } STAT;

    // LPSTAT is a pointer to the STAT structure.
    typedef STAT* LPSTAT;

    // Focal method under test (extern C to ensure proper linkage)
    double Std(LPSTAT st);
}

// Simple, non-terminating test harness
static int g_tests = 0;
static int g_failures = 0;

#define CHECK(cond, msg) \
    do { \
        ++g_tests; \
        if (!(cond)) { \
            ++g_failures; \
            std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
        } \
    } while (0)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper to wrap the expected value calculation for Std
static inline double expected_Std_value(int n, double x, double x2) {
    // Guard against division by zero in tests that intentionally exercise that case
    return std::sqrt((n * x2 - x * x) / (static_cast<double>(n) * (n - 1)));
}

// Test 1: Basic valid input where result should be a finite double
void test_Std_basic()
{
    // Prepare input: n=5, x=2.0, x2=7.0
    // Expected = sqrt((5*7 - 2*2) / (5*(5-1))) = sqrt(31/20)
    STAT s;
    s.n = 5;
    s.x = 2.0;
    s.x2 = 7.0;

    LPSTAT st = &s;
    double result = Std(st);
    double expected = expected_Std_value(s.n, s.x, s.x2);

    // Compare with a tight tolerance
    double diff = std::fabs(result - expected);
    CHECK(diff < 1e-12, "Std basic value mismatch");
}

// Test 2: n == 1 should lead to division by zero (NaN expected)
void test_Std_n_equals_one_nan()
{
    STAT s;
    s.n = 1;
    s.x = 3.0;
    s.x2 = 9.0;

    LPSTAT st = &s;
    double result = Std(st);

    CHECK(std::isnan(result), "Std with n=1 should be NaN due to division by zero");
    CHECK(!std::isfinite(result), "Std with n=1 should be non-finite");
}

// Test 3: Negative numerator yields NaN due to sqrt of negative
void test_Std_negative_numerator_nan()
{
    // Choose values that yield negative (n*x2 - x^2)
    // Example: n=3, x=10, x2=20 -> 3*20 - 100 = -40
    STAT s;
    s.n = 3;
    s.x = 10.0;
    s.x2 = 20.0;

    LPSTAT st = &s;
    double result = Std(st);

    CHECK(std::isnan(result), "Std with negative numerator should be NaN");
}

// Test 4: Large values to ensure numerical stability and correct computation
void test_Std_large_values()
{
    // n=1000, x=1, x2=1 => (1000*1 - 1) / (1000*(999)) = 999 / 999000 ≈ 0.001
    STAT s;
    s.n = 1000;
    s.x = 1.0;
    s.x2 = 1.0;

    LPSTAT st = &s;
    double result = Std(st);
    double expected = expected_Std_value(s.n, s.x, s.x2);

    double diff = std::fabs(result - expected);
    CHECK(diff < 1e-12, "Std large values mismatch");
}

// Test 5: n = 0 leading to NaN due to zero denominator
void test_Std_n_zero_nan()
{
    STAT s;
    s.n = 0;
    s.x = 0.0;
    s.x2 = 0.0;

    LPSTAT st = &s;
    double result = Std(st);

    CHECK(std::isnan(result), "Std with n=0 should be NaN due to zero denominator");
}

// Run all tests
void run_all_tests()
{
    test_Std_basic();
    test_Std_n_equals_one_nan();
    test_Std_negative_numerator_nan();
    test_Std_large_values();
    test_Std_n_zero_nan();
}

int main()
{
    // Run tests
    run_all_tests();

    // Summary
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Test results: " << g_tests << " total, "
              << g_failures << " failures." << std::endl;
    return g_failures ? 1 : 0;
}
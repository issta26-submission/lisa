// Minimal C++11 unit test suite for ITU2Lab (no GTest usage)
// Tests focus on the focal method ITU2Lab defined in itufax.c.
// The tests use a small non-terminating assertion framework (EXPECT_*) and run from main.
//
// Notes:
// - The test relies on the public interface/types from lcms.h (WORD, LPcmsCIELab, cmsCIELab).
// - ITU2Lab is declared with C linkage to avoid name mangling when called from C++.
// - Test cases cover boundary and representative values to ensure correct mapping.

#include <iostream>
#include <cmath>
#include <lcms.h>


// Declare the focal function with C linkage to ensure correct linking
extern "C" void ITU2Lab(WORD In[3], LPcmsCIELab Lab);

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Expectation helper: floating point near comparison
#define EXPECT_NEAR(expected, actual, tol, msg) \
    do { \
        g_total_tests++; \
        double _e = (double)(expected); \
        double _a = (double)(actual); \
        double _diff = std::fabs(_e - _a); \
        if (_diff > (tol)) { \
            ++g_failed_tests; \
            std::cout << "[FAIL] " << (msg) << " | expected ~" << _e << ", got " << _a \
                      << " (diff=" << _diff << ")" << std::endl; \
        } else { \
            std::cout << "[OK]   " << (msg) << std::endl; \
        } \
    } while(0)


// Test 1: Boundary low values
// In = {0, 32768, 24576} should yield L=0, a=0, b=0
void test_ITU2Lab_boundary_low()
{
    // Prepare input
    WORD In[3] = {0, 32768, 24576};
    cmsCIELab Lab; // structure filled by ITU2Lab
    ITU2Lab(In, &Lab);

    // Expected values using the exact same formula as ITU2Lab
    double expL = (double)In[0] / 655.35;
    double expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    double expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;

    EXPECT_NEAR(expL, Lab.L, 1e-6, "Test 1 - L (boundary low)");
    EXPECT_NEAR(expA, Lab.a, 1e-6, "Test 1 - a (boundary low)");
    EXPECT_NEAR(expB, Lab.b, 1e-6, "Test 1 - b (boundary low)");
}

// Test 2: Boundary high values
// In = {65535, 32768, 24576} should yield L≈100, a≈0, b≈0
void test_ITU2Lab_boundary_high()
{
    WORD In[3] = {65535, 32768, 24576};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL = (double)In[0] / 655.35;
    double expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    double expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;

    EXPECT_NEAR(expL, Lab.L, 1e-6, "Test 2 - L (boundary high)");
    EXPECT_NEAR(expA, Lab.a, 1e-6, "Test 2 - a (boundary high)");
    EXPECT_NEAR(expB, Lab.b, 1e-6, "Test 2 - b (boundary high)");
}

// Test 3: Small values to verify negative ranges for a and b
// In = {0, 0, 0} should yield L=0, a≈-85, b≈-75
void test_ITU2Lab_negative_components()
{
    WORD In[3] = {0, 0, 0};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL = (double)In[0] / 655.35;
    double expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    double expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;

    EXPECT_NEAR(expL, Lab.L, 1e-6, "Test 3 - L (all zeros)");
    EXPECT_NEAR(expA, Lab.a, 1e-6, "Test 3 - a (all zeros)");
    EXPECT_NEAR(expB, Lab.b, 1e-6, "Test 3 - b (all zeros)");
}

// Test 4: All maximum inputs to exercise positive a and b
// In = {65535, 65535, 65535} => L≈100, a≈85, b≈125
void test_ITU2Lab_all_max()
{
    WORD In[3] = {65535, 65535, 65535};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL = (double)In[0] / 655.35;
    double expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    double expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;

    EXPECT_NEAR(expL, Lab.L, 1e-6, "Test 4 - L (all max)");
    EXPECT_NEAR(expA, Lab.a, 1e-6, "Test 4 - a (all max)");
    EXPECT_NEAR(expB, Lab.b, 1e-6, "Test 4 - b (all max)");
}

// Test 5: Mixed values to verify non-trivial mapping
// In = {32768, 65535, 0} => L≈50, a≈85, b≈-75
void test_ITU2Lab_mixed_values()
{
    WORD In[3] = {32768, 65535, 0};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL = (double)In[0] / 655.35;
    double expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    double expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;

    EXPECT_NEAR(expL, Lab.L, 1e-6, "Test 5 - L (mixed values)");
    EXPECT_NEAR(expA, Lab.a, 1e-6, "Test 5 - a (mixed values)");
    EXPECT_NEAR(expB, Lab.b, 1e-6, "Test 5 - b (mixed values)");
}


// Main: run all tests and report summary
int main()
{
    std::cout << "Starting ITU2Lab unit tests (C++11)..." << std::endl;

    test_ITU2Lab_boundary_low();
    test_ITU2Lab_boundary_high();
    test_ITU2Lab_negative_components();
    test_ITU2Lab_all_max();
    test_ITU2Lab_mixed_values();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero on any failure to signal issues to CI or build systems
    return (g_failed_tests == 0) ? 0 : 1;
}
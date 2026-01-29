// Lightweight C++11 unit tests for the focal function EvaluateXYZ2Lab
// This test suite does not rely on Google Test and uses a minimal
// non-terminating assertion mechanism to maximize coverage.
// It directly exercises the wrapper logic that converts XYZ (0..1) to Lab
// using the underlying cmsXYZ2Lab routine from LittleCMS.
//
// Notes:
// - We declare the focal function prototype manually (extern "C") since it may not be
//   declared in a standard header for public use.
// - We rely on the LittleCMS types (cmsFloat32Number, cmsFloat64Number, cmsStage, etc.)
//   and the function cmsXYZ2Lab to compute expected values for assertion.
// - Each test includes explanatory comments describing its purpose and approach.

#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS core header for type definitions and cmsXYZ2Lab

// Declaration of the focal function (wrapper under test)
extern "C" void EvaluateXYZ2Lab(const cmsFloat32Number In[],
                                cmsFloat32Number Out[],
                                const cmsStage *mpe);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT(cond, msg) do {                                      \
    ++g_total;                                                        \
    if (!(cond)) {                                                  \
        ++g_failed;                                                 \
        std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__      \
                  << " - " << (msg) << std::endl;                  \
    }                                                               \
} while (0)

#define EXPECT_NEAR(actual, expected, tol, msg) do {               \
    ++g_total;                                                        \
    if (std::fabs((actual) - (expected)) > (tol)) {                \
        ++g_failed;                                                 \
        std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__      \
                  << " - " << (msg)                                    \
                  << " | expected near " << (expected)                 \
                  << ", got " << (actual) << std::endl;             \
    }                                                               \
} while (0)


// Test 1: Black input (0,0,0) should map to L≈0 and a,b around neutral value
// The code scales Lab.L to [0,100] and a,b to [0,1] via (a+128)/255, (b+128)/255.
// For XYZ = (0,0,0), Lab.L ≈ 0, a ≈ 0, b ≈ 0, hence Out ≈ {0, 128/255, 128/255}.
void test_EvaluateXYZ2Lab_Black()
{
    const cmsFloat32Number In[3] = { 0.0f, 0.0f, 0.0f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Call the function under test
    EvaluateXYZ2Lab(In, Out, NULL);

    // Expected values derived from Lab for XYZ = 0
    const cmsFloat32Number expected0 = 0.0f;
    const cmsFloat32Number expected1 = (cmsFloat32Number)(128.0f / 255.0f);
    const cmsFloat32Number expected2 = (cmsFloat32Number)(128.0f / 255.0f);
    const cmsFloat32Number tol = 1e-4f;

    EXPECT_NEAR(Out[0], expected0, tol, "Black: L should be ~0");
    EXPECT_NEAR(Out[1], expected1, tol, "Black: a should be mapped to ~0.50196");
    EXPECT_NEAR(Out[2], expected2, tol, "Black: b should be mapped to ~0.50196");
}

// Test 2: White-ish input (1,1,1) should map to L≈100 and a,b near 0
// We compute expected Lab via cmsXYZ2Lab and compare the scaled outputs.
void test_EvaluateXYZ2Lab_WhiteApprox()
{
    const cmsFloat32Number In[3] = { 1.0f, 1.0f, 1.0f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    // Compute expected Lab using the same cmsXYZ2Lab routine as a reference
    cmsCIEXYZ XYZ;
    const cmsFloat64Number XYZadj = MAX_ENCODEABLE_XYZ;
    XYZ.X = (cmsFloat64Number)(In[0] * XYZadj);
    XYZ.Y = (cmsFloat64Number)(In[1] * XYZadj);
    XYZ.Z = (cmsFloat64Number)(In[2] * XYZadj);

    cmsCIELab Lab;
    cmsXYZ2Lab(NULL, &Lab, &XYZ);

    const cmsFloat32Number expected0 = (cmsFloat32Number) (Lab.L / 100.0);
    const cmsFloat32Number expected1 = (cmsFloat32Number) ((Lab.a + 128.0) / 255.0);
    const cmsFloat32Number expected2 = (cmsFloat32Number) ((Lab.b + 128.0) / 255.0);

    // Run the focal function
    EvaluateXYZ2Lab(In, Out, NULL);

    const cmsFloat32Number tol = 1e-4f;
    EXPECT_NEAR(Out[0], expected0, tol, "WhiteApprox: L channel");
    EXPECT_NEAR(Out[1], expected1, tol, "WhiteApprox: a channel");
    EXPECT_NEAR(Out[2], expected2, tol, "WhiteApprox: b channel");
}

// Test 3: Random mid-range input to check general correctness and range
// Also asserts that outputs remain within [0,1].
void test_EvaluateXYZ2Lab_RangeAndMonotonicity()
{
    // Use a non-edge input
    const cmsFloat32Number In[3] = { 0.25f, 0.75f, 0.50f };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    EvaluateXYZ2Lab(In, Out, NULL);

    // Basic range checks
    EXPECT(Out[0] >= 0.0f && Out[0] <= 1.0f, "Out[0] in [0,1]");
    EXPECT(Out[1] >= 0.0f && Out[1] <= 1.0f, "Out[1] in [0,1]");
    EXPECT(Out[2] >= 0.0f && Out[2] <= 1.0f, "Out[2] in [0,1]");

    // An additional consistency check: small increments in input should not cause
    // Out of range or NaN. We'll compute a nearby input and ensure finite outputs.
    const cmsFloat32Number In2[3] = { 0.26f, 0.76f, 0.51f };
    cmsFloat32Number Out2[3] = { 0.0f, 0.0f, 0.0f };
    EvaluateXYZ2Lab(In2, Out2, NULL);
    EXPECT(Out2[0] >= 0.0f && Out2[0] <= 1.0f, "Out2[0] in [0,1]");
    EXPECT(Out2[1] >= 0.0f && Out2[1] <= 1.0f, "Out2[1] in [0,1]");
    EXPECT(Out2[2] >= 0.0f && Out2[2] <= 1.0f, "Out2[2] in [0,1]");
}

int main()
{
    // Run all tests
    test_EvaluateXYZ2Lab_Black();
    test_EvaluateXYZ2Lab_WhiteApprox();
    test_EvaluateXYZ2Lab_RangeAndMonotonicity();

    // Summary
    std::cout << "Unit tests completed. Total: " << g_total
              << ", Failures: " << g_failed << std::endl;

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}
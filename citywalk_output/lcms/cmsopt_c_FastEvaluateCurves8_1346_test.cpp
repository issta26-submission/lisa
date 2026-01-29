// Test suite for the focal method FastEvaluateCurves8 in cmsopt.c
// Purpose: validate correct functionality of the 8-bit fast evaluation path.
// Note: This test uses a minimal Curves16Data structure that mirrors the
// expected layout used by FastEvaluateCurves8: { nCurves, Curves[i][x] }.

// The test is written for C++11, without GoogleTest, using a lightweight
// EXPECT-like assertion mechanism that does not terminate on failure.

#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge: define the minimal types expected by FastEvaluateCurves8.
// We purposely re-declare only what is necessary for testing.
typedef unsigned short cmsUInt16Number; // 16-bit unsigned
typedef unsigned int   cmsUInt32Number;

// CMSREGISTER macro used in the focal method; we assume empty on test side.
// The actual macro in the library can differ, but the signature below should
// be compatible for linking with cmsopt.c when CMSREGISTER expands similarly.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward declaration of the focal function with C linkage for the test host.
// We intentionally use plain types matching the common lcms typedefs so that
// the linker can bind to the function defined in cmsopt.c.
extern "C" void FastEvaluateCurves8(CMSREGISTER const cmsUInt16Number In[],
                                   CMSREGISTER cmsUInt16Number Out[],
                                   CMSREGISTER const void* D);

// Minimal Curves16Data structure mirroring the usage within FastEvaluateCurves8.
typedef struct {
    cmsUInt32Number nCurves;
    cmsUInt16Number** Curves; // Curves[i] is an array of cmsUInt16Number
} Curves16Data;

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILURE] " << (msg) << " (Line: " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if( (static_cast<cmsUInt16Number>(a)) != (static_cast<cmsUInt16Number>(b)) ) { \
        std::cerr << "[TEST FAILURE] " << (msg) \
                  << " | expected: " << (b) << " got: " << (a) \
                  << " (Line: " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define RUN_TEST(testFunc) do { \
    std::cout << "Running " #testFunc "..." << std::endl; \
    testFunc(); \
} while(0)

// Test 1: Basic operation with distinct indices within range
void test_FastEvaluateCurves8_basic_case() {
    // Prepare data: 3 curves, each with 4 elements (indices 0..3)
    Curves16Data data;
    data.nCurves = 3;
    data.Curves = new cmsUInt16Number*[data.nCurves];
    const cmsUInt32Number len = 4;

    // Allocate and initialize curves
    for (cmsUInt32Number i = 0; i < data.nCurves; ++i) {
        data.Curves[i] = new cmsUInt16Number[len];
        for (cmsUInt32Number j = 0; j < len; ++j) {
            data.Curves[i][j] = static_cast<cmsUInt16Number>(i * 100 + j);
        }
    }

    // In values: x = In[i] >> 8
    // We'll choose x = 1, 2, 3 respectively
    cmsUInt16Number In[3]  = { static_cast<cmsUInt16Number>(0x0100),
                               static_cast<cmsUInt16Number>(0x0200),
                               static_cast<cmsUInt16Number>(0x0300) };
    cmsUInt16Number Out[3] = { 0, 0, 0 };

    // Expected:
    // Out[0] should be data.Curves[0][1] = 0*100+1 = 1
    // Out[1] should be data.Curves[1][2] = 1*100+2 = 102
    // Out[2] should be data.Curves[2][3] = 2*100+3 = 203
    // Adjust expected values based on our initialization above
    // Correcting initialization to match expected values:
    // data.Curves[0] = [0,1,2,3]
    // data.Curves[1] = [100,101,102,103]
    // data.Curves[2] = [200,201,202,203]
    // Re-initialize to the intended values:
    // Overwrite to exact values for clarity
    data.Curves[0][0] = 0; data.Curves[0][1] = 1; data.Curves[0][2] = 2; data.Curves[0][3] = 3;
    data.Curves[1][0] = 100; data.Curves[1][1] = 101; data.Curves[1][2] = 102; data.Curves[1][3] = 103;
    data.Curves[2][0] = 200; data.Curves[2][1] = 201; data.Curves[2][2] = 202; data.Curves[2][3] = 203;

    // Execute
    FastEvaluateCurves8(In, Out, &data);

    // Verify
    EXPECT_EQ(Out[0], 1, "Out[0] should be Curves[0][1]");
    EXPECT_EQ(Out[1], 102, "Out[1] should be Curves[1][2]");
    EXPECT_EQ(Out[2], 203, "Out[2] should be Curves[2][3]");

    // Cleanup
    for (cmsUInt32Number i = 0; i < data.nCurves; ++i) {
        delete[] data.Curves[i];
    }
    delete[] data.Curves;
}

// Test 2: Boundary values for x = 0, 1, 2 with distinct curves
void test_FastEvaluateCurves8_boundary_cases() {
    Curves16Data data;
    data.nCurves = 3;
    data.Curves = new cmsUInt16Number*[data.nCurves];
    const cmsUInt32Number len = 4;

    for (cmsUInt32Number i = 0; i < data.nCurves; ++i) {
        data.Curves[i] = new cmsUInt16Number[len];
    }

    // Set known values
    // Curve 0: [0, 7, 8, 9]
    data.Curves[0][0] = 0; data.Curves[0][1] = 7; data.Curves[0][2] = 8; data.Curves[0][3] = 9;
    // Curve 1: [0, 70, 71, 72]
    data.Curves[1][0] = 0; data.Curves[1][1] = 70; data.Curves[1][2] = 71; data.Curves[1][3] = 72;
    // Curve 2: [0, 170, 171, 172]
    data.Curves[2][0] = 0; data.Curves[2][1] = 170; data.Curves[2][2] = 171; data.Curves[2][3] = 172;

    cmsUInt16Number In[3]  = { static_cast<cmsUInt16Number>(0x0000), // x=0
                            static_cast<cmsUInt16Number>(0x0100), // x=1
                            static_cast<cmsUInt16Number>(0x0200)  // x=2
                          };
    cmsUInt16Number Out[3] = { 0, 0, 0 };

    FastEvaluateCurves8(In, Out, &data);

    EXPECT_EQ(Out[0], 0, "Out[0] should be Curves[0][0] (x=0)");
    EXPECT_EQ(Out[1], 7, "Out[1] should be Curves[0][1] for second curve? (Wait: mapping: Out[1] corresponds to Curves[1][1])");
    // A correction: Out[1] maps to data.Curves[1][1] = 70
    EXPECT_EQ(Out[1], 70, "Out[1] should be Curves[1][1] (x=1)");
    EXPECT_EQ(Out[2], 171, "Out[2] should be Curves[2][2] (x=2)");

    // Cleanup
    for (cmsUInt32Number i = 0; i < data.nCurves; ++i) {
        delete[] data.Curves[i];
    }
    delete[] data.Curves;
}

// Test 3: Degenerate case with zero curves
void test_FastEvaluateCurves8_zero_curves() {
    Curves16Data data;
    data.nCurves = 0;
    data.Curves = nullptr;

    cmsUInt16Number In[1] = { 0x0100 };
    cmsUInt16Number Out[1] = { 0 };

    // Should perform no work and not crash
    FastEvaluateCurves8(In, Out, &data);

    // Out should remain unchanged (we set to 0 prior)
    EXPECT_EQ(Out[0], 0, "Out should remain unchanged for zero curves");
}

// Test 4: Single-curve case to ensure function handles minimal non-empty data
void test_FastEvaluateCurves8_single_curve() {
    Curves16Data data;
    data.nCurves = 1;
    data.Curves = new cmsUInt16Number*[data.nCurves];
    data.Curves[0] = new cmsUInt16Number[4];
    data.Curves[0][0] = 10;
    data.Curves[0][1] = 20;
    data.Curves[0][2] = 30;
    data.Curves[0][3] = 40;

    cmsUInt16Number In[1] = { static_cast<cmsUInt16Number>(0x0300) }; // x=3
    cmsUInt16Number Out[1] = { 0 };

    FastEvaluateCurves8(In, Out, &data);

    EXPECT_EQ(Out[0], 40, "Out[0] should be Curves[0][3] (single curve case)");

    // Cleanup
    delete[] data.Curves[0];
    delete[] data.Curves;
}

int main() {
    std::cout << "Starting unit tests for FastEvaluateCurves8..." << std::endl;

    RUN_TEST(test_FastEvaluateCurves8_basic_case);
    RUN_TEST(test_FastEvaluateCurves8_boundary_cases);
    RUN_TEST(test_FastEvaluateCurves8_zero_curves);
    RUN_TEST(test_FastEvaluateCurves8_single_curve);

    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] FastEvaluateCurves8" << std::endl;
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] FastEvaluateCurves8: "
                  << g_failures << " failure(s) detected." << std::endl;
        return 1;
    }
}
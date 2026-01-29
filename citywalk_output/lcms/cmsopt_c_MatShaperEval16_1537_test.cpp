// C++11 unit tests for MatShaperEval16 in cmsopt.c without using GTest.
// This test suite uses a small in-house test harness with non-terminating
// assertions to maximize coverage.

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include public LCMS2 types to define cmsUInt16Number, cmsS1Fixed14Number, etc.

extern "C" {
    // Function under test. Declared with C linkage to ensure proper linkage.
    void MatShaperEval16(CMSREGISTER const cmsUInt16Number In[],
                       CMSREGISTER cmsUInt16Number Out[],
                       CMSREGISTER const void* D);
}

// Candidate Keywords (Step 1): Shaper1R, Shaper1G, Shaper1B, Mat, Off, Shaper2R, Shaper2G, Shaper2B

// We don't know the internal MatShaper8Data type in this test harness,
// so we reproduce the memory layout exactly as used by MatShaperEval16
// to pass a correctly structured object via the D pointer.
struct MatShaper8DataTest {
    // 8-bit input shapers (mapping 0..255 to 1.14 fixed-point [0..1.0])
    cmsS1Fixed14Number Shaper1R[256];
    cmsS1Fixed14Number Shaper1G[256];
    cmsS1Fixed14Number Shaper1B[256];

    // 3x3 color transformation matrix and offsets (1.14 fixed-point)
    cmsS1Fixed14Number Mat[3][3];
    cmsS1Fixed14Number Off[3];

    // Second-stage shapers (mapping 0..16384 to 16-bit output)
    cmsUInt16Number Shaper2R[16385];
    cmsUInt16Number Shaper2G[16385];
    cmsUInt16Number Shaper2B[16385];
};

// Simple test harness
static int gFailures = 0;

#define TEST_FAIL(msg) do { \
    std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
    ++gFailures; \
} while (0)

#define EXPECT_EQ_INT(expected, actual) do { \
    int e = static_cast<int>(expected); \
    int a = static_cast<int>(actual); \
    if (e != a) { \
        TEST_FAIL("expected " + std::to_string(e) + ", got " + std::to_string(a)); \
    } \
} while (0)

#define EXPECT_EQ_HEX(expected, actual) do { \
    unsigned int e = static_cast<unsigned int>(expected); \
    unsigned int a = static_cast<unsigned int>(actual); \
    if (e != a) { \
        TEST_FAIL("expected 0x" + std::to_string(e) + ", got 0x" + std::to_string(a)); \
    } \
} while (0)

// Helper to run a single MatShaperEval16 call
static void runMatShaperTest(const cmsUInt16Number in0, const cmsUInt16Number in1, const cmsUInt16Number in2,
                             const MatShaper8DataTest& data, cmsUInt16Number* out)
{
    cmsUInt16Number In[3] = { in0, in1, in2 };
    cmsUInt16Number Out[3] = { 0, 0, 0 };
    MatShaperEval16(In, Out, &data);
    out[0] = Out[0];
    out[1] = Out[1];
    out[2] = Out[2];
}

// Test 1: Identity-like matrix with 1.0 shapers -> output should equal the second stage at index 16384
static void test_identity_matrix()
{
    // Arrange
    MatShaper8DataTest data;
    // Initialize first-stage shapers to 1.0 (16384 in 1.14 fixed-point)
    for (int i = 0; i < 256; ++i) {
        data.Shaper1R[i] = 16384;
        data.Shaper1G[i] = 16384;
        data.Shaper1B[i] = 16384;
    }
    // Identity matrix
    data.Mat[0][0] = 16384; data.Mat[0][1] = 0;      data.Mat[0][2] = 0;
    data.Mat[1][0] = 0;      data.Mat[1][1] = 16384; data.Mat[1][2] = 0;
    data.Mat[2][0] = 0;      data.Mat[2][1] = 0;      data.Mat[2][2] = 16384;
    data.Off[0] = 0;
    data.Off[1] = 0;
    data.Off[2] = 0;

    // Second-stage shapers: identity-like mapping
    for (int i = 0; i <= 16384; ++i) {
        data.Shaper2R[i] = i;
        data.Shaper2G[i] = i;
        data.Shaper2B[i] = i;
    }

    // Input minimal values
    cmsUInt16Number in0 = 0, in1 = 0, in2 = 0;
    cmsUInt16Number out[3];
    runMatShaperTest(in0, in1, in2, data, out);

    // Expected: 16384 mapped through identity second-stage
    EXPECT_EQ_INT(16384, out[0]);
    EXPECT_EQ_INT(16384, out[1]);
    EXPECT_EQ_INT(16384, out[2]);
}

// Test 2: Negative matrix entries should clip to 0 for the first stage result
static void test_negative_clipping()
{
    // Arrange
    MatShaper8DataTest data;
    for (int i = 0; i < 256; ++i) {
        data.Shaper1R[i] = 16384;
        data.Shaper1G[i] = 16384;
        data.Shaper1B[i] = 16384;
    }
    // Negative diagonal to force l1 < 0
    data.Mat[0][0] = -16384; data.Mat[0][1] = 0; data.Mat[0][2] = 0;
    data.Mat[1][0] = 0; data.Mat[1][1] = 0; data.Mat[1][2] = 0;
    data.Mat[2][0] = 0; data.Mat[2][1] = 0; data.Mat[2][2] = 0;
    data.Off[0] = 0;
    data.Off[1] = 0;
    data.Off[2] = 0;

    data.Shaper2R[0] = 0; data.Shaper2G[0] = 0; data.Shaper2B[0] = 0;
    // For clip test we'll ensure upper indices are not used
    data.Shaper2R[0] = 0;
    data.Shaper2G[0] = 0;
    data.Shaper2B[0] = 0;

    cmsUInt16Number in0 = 0, in1 = 0, in2 = 0;
    cmsUInt16Number out[3];
    runMatShaperTest(in0, in1, in2, data, out);

    // Expected: clipping to 0 for all channels
    EXPECT_EQ_INT(0, out[0]);
    EXPECT_EQ_INT(0, out[1]);
    EXPECT_EQ_INT(0, out[2]);
}

// Test 3: Upper clipping when matrix pushes values beyond 1.0
static void test_upper_clipping_to_max_output()
{
    // Arrange
    MatShaper8DataTest data;
    for (int i = 0; i < 256; ++i) {
        data.Shaper1R[i] = 16384;
        data.Shaper1G[i] = 16384;
        data.Shaper1B[i] = 16384;
    }
    // Large positive diagonal to push l1..l3 to max
    data.Mat[0][0] = 32768; data.Mat[0][1] = 0; data.Mat[0][2] = 0;
    data.Mat[1][0] = 0; data.Mat[1][1] = 32768; data.Mat[1][2] = 0;
    data.Mat[2][0] = 0; data.Mat[2][1] = 0; data.Mat[2][2] = 32768;
    data.Off[0] = 0;
    data.Off[1] = 0;
    data.Off[2] = 0;

    // Set output at index 16384 to known values after clipping
    data.Shaper2R[16384] = 12345;
    data.Shaper2G[16384] = 54321;
    data.Shaper2B[16384] = 7777;

    cmsUInt16Number in0 = 0, in1 = 0, in2 = 0;
    cmsUInt16Number out[3];
    runMatShaperTest(in0, in1, in2, data, out);

    EXPECT_EQ_INT(12345, out[0]);
    EXPECT_EQ_INT(54321, out[1]);
    EXPECT_EQ_INT(7777, out[2]);
}

// Test 4: Ensure proper handling when inputs map to non-zero Shaper1 values for G/B while Mat is zero
static void test_nonzero_shapers_with_zero_matrix()
{
    // Arrange
    MatShaper8DataTest data;
    // Set r/g/b shapers to produce 0 on first channel, and 0 elsewhere to isolate effect
    data.Shaper1R[0] = 0;
    data.Shaper1R[1] = 0;
    data.Shaper1G[0] = 0;
    data.Shaper1G[1] = 0;
    data.Shaper1B[0] = 0;
    data.Shaper1B[1] = 0;

    for (int i = 0; i < 256; ++i) {
        data.Shaper1R[i] = 0;
        data.Shaper1G[i] = 0;
        data.Shaper1B[i] = 0;
    }

    // Zero matrix (no color transformation)
    data.Mat[0][0] = 0; data.Mat[0][1] = 0; data.Mat[0][2] = 0;
    data.Mat[1][0] = 0; data.Mat[1][1] = 0; data.Mat[1][2] = 0;
    data.Mat[2][0] = 0; data.Mat[2][1] = 0; data.Mat[2][2] = 0;
    data.Off[0] = 0; data.Off[1] = 0; data.Off[2] = 0;

    // Define Shaper2 outputs for index 0
    data.Shaper2R[0] = 7;
    data.Shaper2G[0] = 8;
    data.Shaper2B[0] = 9;

    // Input that maps to ri=0, gi=1, bi=2 -> since Shaper1* are 0, all l1/l2/l3 stay 0
    cmsUInt16Number in0 = 0, in1 = 1, in2 = 2;
    cmsUInt16Number out[3];
    runMatShaperTest(in0, in1, in2, data, out);

    EXPECT_EQ_INT(7, out[0]);
    EXPECT_EQ_INT(8, out[1]);
    EXPECT_EQ_INT(9, out[2]);
}

int main()
{
    test_identity_matrix();
    test_negative_clipping();
    test_upper_clipping_to_max_output();
    test_nonzero_shapers_with_zero_matrix();

    if (gFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << gFailures << " test(s) failed.\n";
        return 1;
    }
}
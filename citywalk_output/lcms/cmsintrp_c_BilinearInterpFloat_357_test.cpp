// Unit test suite for BilinearInterpFloat in cmsintrp.c
// Target: C++11, no GoogleTest, simple in-file test runner with explanatory comments.
// This test relies on LittleCMS internal header (lcms2_internal.h) for the
// cms types (cmsFloat32Number, cmsInterpParams, etc.) and the BilinearInterpFloat signature.
//
// Important assumptions:
// - cmsFloat32Number is a typedef for float.
// - cmsInterpParams is defined in lcms2_internal.h with fields used by BilinearInterpFloat:
//   Table, nOutputs, Domain[2], opta[2], among possibly others.
// - BilinearInterpFloat has C linkage.
// - The test uses a small 2x2 LUT with 2 outputs to exercise both branches
//   of the interpolation (X1/Y1 increments and boundary checks).

#include <cstring>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Import LittleCMS internal types to match the actual structure expected by cmsintrp.c
// This header must be available in the include path when compiling the test.
extern "C" {
}

// Declare the BilinearInterpFloat function with C linkage for linking against cmsintrp.c
extern "C" void BilinearInterpFloat(const cmsFloat32Number Input[],
                                  cmsFloat32Number Output[],
                                  const cmsInterpParams* p);

// Simple non-terminating test framework (collect and report failures, continue running tests)
static int g_failures = 0;

static void log_failure(const char* test_name, const char* msg) {
    std::cerr << "[TEST FAILED] " << test_name << ": " << msg << std::endl;
    ++g_failures;
}
static void log_pass(const char* test_name) {
    std::cout << "[TEST PASSED] " << test_name << std::endl;
}
static bool almost_equal_float(float a, float b, float tol) {
    return std::fabs(a - b) <= tol;
}
#define EXPECT_NEAR(val, expected, tol) do { \
    if (!almost_equal_float((val), (expected), (tol))) { \
        log_failure(__func__, #val " != " #expected " (actual=" + std::to_string(val) + ")"); \
    } else { \
        /* optional per-value pass print can be enabled if desired */ \
    } \
} while(0)
#define EXPECT_EQ_INT(val, expected) do { \
    if ((val) != (expected)) { \
        log_failure(__func__, std::string(#val " != " #expected).c_str()); \
    } else { \
        /* pass */ \
    } \
} while(0)


// Test 1: Center of the cell with two outputs (Out0 and Out1) using a simple LUT.
// LUT layout for 2 outputs with indexing: LutTable[(i)+(j)+OutChan]
//
// Design:
// - Domain = [1.0, 1.0] so px = Input[0], py = Input[1]
// - opta[0] = opta[1] = 1 so X0 = 0 or 1, Y0 = 0 or 1 depending on floor(Input)
// - Total outputs = 2
// - LUT values: For OutChan 0: Lut[0]=0, Lut[1]=2, Lut[2]=4
//                 For OutChan 1: Lut[1]=2, Lut[2]=4, Lut[3]=6
// This arrangement allows straightforward bilinear interpolation:
// d00= Lut[ X0+Y0+chan ], d01= Lut[ X0+Y1+chan ], d10= Lut[ X1+Y0+chan ], d11= Lut[ X1+Y1+chan ]
void test_bilinear_center_two_outputs() {
    const int nOutputs = 2;
    float lut[4] = {0.0f, 2.0f, 4.0f, 6.0f}; // size enough for both channels
    cmsInterpParams p;
    // Initialize the cmsInterpParams according to the expected layout.
    // If the actual layout differs, adjust to match lcms2_internal.h
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.nOutputs = nOutputs;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;

    cmsFloat32Number Input[2] = {0.5f, 0.5f};
    cmsFloat32Number Output[2] = {0.0f, 0.0f};

    BilinearInterpFloat(Input, Output, &p);

    // Expected results:
    // For Out0: d00=0, d01=2, d10=2, d11=4 -> dx0=1.0, dx1=2.0, dxy=1.5? Wait with fx=0.5, fy=0.5:
    // dx0 = (1-0.5)*0 + 0.5*2 = 1.0
    // dx1 = (1-0.5)*2 + 0.5*4 = 1.0 + 2.0 = 3.0
    // dxy = (1-0.5)*1.0 + 0.5*3.0 = 0.5 + 1.5 = 2.0
    // For Out1: d00=2, d01=4, d10=4, d11=6 -> similar interpolation yields 3.5
    EXPECT_NEAR(Output[0], 2.0f, 1e-5f);
    EXPECT_NEAR(Output[1], 3.5f, 1e-5f);

    if (g_failures == 0) log_pass(__func__);
}

// Test 2: Origin of the cell (Input = {0.0, 0.0}) with two outputs
// Expect simple bilinear blend at (0,0): Output should equal d00 for each channel.
void test_bilinear_origin_two_outputs() {
    const int nOutputs = 2;
    float lut[4] = {0.0f, 2.0f, 4.0f, 6.0f}; // same LUT as Test 1
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.nOutputs = nOutputs;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;

    cmsFloat32Number Input[2] = {0.0f, 0.0f};
    cmsFloat32Number Output[2] = {0.0f, 0.0f};

    BilinearInterpFloat(Input, Output, &p);

    // For Out0: d00 = Lut[0] = 0
    // For Out1: d00 = Lut[1] = 2
    EXPECT_NEAR(Output[0], 0.0f, 1e-5f);
    EXPECT_NEAR(Output[1], 2.0f, 1e-5f);

    if (g_failures == 0) log_pass(__func__);
}

// Test 3: Corner of the cell with Input = {1.0, 1.0}, two outputs
// All interpolation corners collapse to d11 for each channel when fx=fy=0 in this corner scenario.
void test_bilinear_corner_two_outputs() {
    const int nOutputs = 2;
    float lut[4] = {0.0f, 2.0f, 4.0f, 6.0f};
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.nOutputs = nOutputs;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;

    cmsFloat32Number Input[2] = {1.0f, 1.0f};
    cmsFloat32Number Output[2] = {0.0f, 0.0f};

    BilinearInterpFloat(Input, Output, &p);

    // For Out0: d11 = Lut[2] = 4
    // For Out1: d11 = Lut[3] = 6
    EXPECT_NEAR(Output[0], 4.0f, 1e-5f);
    EXPECT_NEAR(Output[1], 6.0f, 1e-5f);

    if (g_failures == 0) log_pass(__func__);
}

// Test 4: Multichannel with a different interpolation pattern by plugging a different Input (0.5, 0.25)
// and ensuring both outputs still interpolate independently according to their LUT entries.
void test_bilinear_center_mixed_inputs_two_outputs() {
    const int nOutputs = 2;
    // Same LUT as before
    float lut[4] = {0.0f, 2.0f, 4.0f, 6.0f};
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.nOutputs = nOutputs;
    p.Domain[0] = 1.0f;
    p.Domain[1] = 1.0f;
    p.opta[0] = 1.0f;
    p.opta[1] = 1.0f;

    cmsFloat32Number Input[2] = {0.5f, 0.25f};
    cmsFloat32Number Output[2] = {0.0f, 0.0f};

    BilinearInterpFloat(Input, Output, &p);

    // For Out0: d00=0, d01=2, d10=2, d11=4
    // fx=0.5, fy=0.25
    // dx0 = 0.5*2 = 1.0
    // dx1 = 0.5*2 + 0.5*4 = 1 + 2 = 3
    // dxy = (1-fy)*dx0 + fy*dx1 = 0.75*1.0 + 0.25*3 = 0.75 + 0.75 = 1.5
    // For Out1: d00=2, d01=4, d10=4, d11=6
    // dx0 = 0.5*4 = 2
    // dx1 = 0.5*4 + 0.5*6 = 2 + 3 = 5
    // dxy = 0.75*2 + 0.25*5 = 1.5 + 1.25 = 2.75
    EXPECT_NEAR(Output[0], 1.5f, 1e-5f);
    EXPECT_NEAR(Output[1], 2.75f, 1e-5f);

    if (g_failures == 0) log_pass(__func__);
}

int main() {
    // Run tests. Each test prints its own pass/fail result.
    test_bilinear_center_two_outputs();
    test_bilinear_origin_two_outputs();
    test_bilinear_corner_two_outputs();
    test_bilinear_center_mixed_inputs_two_outputs();

    if (g_failures != 0) {
        std::cerr << "Total failures: " << g_failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}
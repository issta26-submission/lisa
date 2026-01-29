// Minimal, self-contained test-suite for Eval1InputFloat (no GTest, pure C++11)

#include <cstring>
#include <cstdio>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// ----------------------------------------------------------------------------------
// Lightweight typedefs and structures to mirror the focal function's expectations
// ----------------------------------------------------------------------------------

typedef float cmsFloat32Number;
typedef uint32_t cmsUInt32Number;

// Minimal cmsInterpParams structure with only the members used by Eval1InputFloat
struct cmsInterpParams {
    void* Table;                 // pointer to LUT (cast to cmsFloat32Number* inside function)
    cmsFloat32Number Domain[1];  // domain array (we use Domain[0] in tests)
    cmsFloat32Number opta[1];    // sampling step (we use opta[0])
    cmsUInt32Number nOutputs;    // number of output channels
};

// ----------------------------------------------------------------------------------
// Dependencies used by focal method (stubbed/minimal implementations)
// ----------------------------------------------------------------------------------

static cmsFloat32Number fclamp(cmsFloat32Number v) {
    // Simple clamp to [0, 1], matching typical clamped input handling
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

// The focal method under test (copied and adapted to be self-contained for this
// unit-test environment). It performs either direct LUT copy or 1D bilinear
// interpolation based on the provided parameters.
void Eval1InputFloat(const cmsFloat32Number Value[],
                     cmsFloat32Number Output[],
                     const cmsInterpParams* p)
{
{
    cmsFloat32Number y1, y0;
    cmsFloat32Number val2, rest;
    int cell0, cell1;
    cmsUInt32Number OutChan;
    const cmsFloat32Number* LutTable = (cmsFloat32Number*) p ->Table;
    val2 = fclamp(Value[0]);
    // last value or zero-domain: direct LUT fetch
    if (val2 == 1.0 || p->Domain[0] == 0) {
        cmsUInt32Number start = p->Domain[0] * p->opta[0];
        for (OutChan = 0; OutChan < p->nOutputs; OutChan++) {
            Output[OutChan] = LutTable[start + OutChan];
        }        
    }
    else
    {
        val2 *= p->Domain[0];
        cell0 = (int)floor(val2);
        cell1 = (int)ceil(val2);
        // Rest is fractional part in [0,1)
        rest = val2 - cell0;
        cell0 *= p->opta[0];
        cell1 *= p->opta[0];
        for (OutChan = 0; OutChan < p->nOutputs; OutChan++) {
            y0 = LutTable[cell0 + OutChan];
            y1 = LutTable[cell1 + OutChan];
            Output[OutChan] = y0 + (y1 - y0) * rest;
        }
    }
}
}

// ----------------------------------------------------------------------------------
// Lightweight test harness (no external test framework)
// - Uses simple floating-point comparisons with a small epsilon
// - Prints per-test results and a final summary
// ----------------------------------------------------------------------------------

static int g_failures = 0;

static bool almost_equal(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) <= eps;
}

#define EXPECT_FLOAT_EQ(actual, expected) do { \
    if (!almost_equal((actual), (expected))) { \
        printf("  [FAIL] %s:%d: Expected %f, got %f\n", __FILE__, __LINE__, (expected), (actual)); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("  [FAIL] %s:%d: Condition is false\n", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

static void test_Eval1InputFloat_LastValueBranch() {
    // Verify the branch: val2 == 1.0 OR Domain[0] == 0 yields direct LUT copy
    // Set up: Domain[0] = 0.5, opta[0] = 2, nOutputs = 2, Table with enough elements
    cmsFloat32Number Value[1] = { 1.0f }; // clamped to 1.0
    cmsFloat32Number Output[2] = { 0.0f, 0.0f };
    cmsFloat32Number lut[4] = { 100.0f, 101.0f, 200.0f, 201.0f }; // indices: [0,1,2,3]
    cmsInterpParams p;
    p.Table = lut;
    p.Domain[0] = 0.5f;
    p.opta[0] = 2.0f;
    p.nOutputs = 2;

    Eval1InputFloat(Value, Output, &p);

    // Expected: start = Domain[0] * opta[0] = 1, so Output = [lut[1], lut[2]] = [101, 200]
    EXPECT_FLOAT_EQ(Output[0], 101.0f);
    EXPECT_FLOAT_EQ(Output[1], 200.0f);
}

static void test_Eval1InputFloat_Interpolation() {
    // Verify 1D interpolation when Domain[0] > 0 and Value[0] < 1
    // Set up: Domain[0] = 0.5, opta[0] = 2, nOutputs = 2
    // lut = {10, 14, 20, 24}
    // Value[0] = 0.6 -> val2 = clamp(0.6) = 0.6; val2 * Domain[0] = 0.3
    // cell0 = floor(0.3) = 0; cell1 = ceil(0.3) = 1; rest = 0.3
    // Output[0] = lut[0] + (lut[2] - lut[0]) * 0.3 = 10 + 10 * 0.3 = 13
    // Output[1] = lut[1] + (lut[3] - lut[1]) * 0.3 = 14 + 10 * 0.3 = 17
    cmsFloat32Number Value[1] = { 0.6f };
    cmsFloat32Number Output[2] = { 0.0f, 0.0f };
    cmsFloat32Number lut[4] = { 10.0f, 14.0f, 20.0f, 24.0f };
    cmsInterpParams p;
    p.Table = lut;
    p.Domain[0] = 0.5f;
    p.opta[0] = 2.0f;
    p.nOutputs = 2;

    Eval1InputFloat(Value, Output, &p);

    EXPECT_FLOAT_EQ(Output[0], 13.0f);
    EXPECT_FLOAT_EQ(Output[1], 17.0f);
}

static void test_Eval1InputFloat_DomainZero() {
    // Verify the direct LUT path when Domain[0] == 0
    // Set up: Domain[0] = 0, opta[0] = 3, nOutputs = 2
    // lut = {5, 6, 7, 8}; Output should be {lut[0], lut[1]}
    cmsFloat32Number Value[1] = { 0.75f };
    cmsFloat32Number Output[2] = { 0.0f, 0.0f };
    cmsFloat32Number lut[4] = { 5.0f, 6.0f, 7.0f, 8.0f };
    cmsInterpParams p;
    p.Table = lut;
    p.Domain[0] = 0.0f;
    p.opta[0] = 3.0f;
    p.nOutputs = 2;

    Eval1InputFloat(Value, Output, &p);

    EXPECT_FLOAT_EQ(Output[0], 5.0f);
    EXPECT_FLOAT_EQ(Output[1], 6.0f);
}

// ----------------------------------------------------------------------------------
// Main entry: run all tests and report summary
// ----------------------------------------------------------------------------------

static void run_all_tests() {
    printf("Running Eval1InputFloat unit tests...\n");

    test_Eval1InputFloat_LastValueBranch();
    test_Eval1InputFloat_Interpolation();
    test_Eval1InputFloat_DomainZero();

    if (g_failures == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Total failures: %d\n", g_failures);
    }
}

int main() {
    run_all_tests();
    return g_failures;
}
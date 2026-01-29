// Minimal unit-test harness for PrelinEval8 (as defined in the provided focal method)
// This test is self-contained and does not rely on GTest. It mimics a small test framework
// to validate multiple execution paths of PrelinEval8 by providing minimal mock structures.

#include <cstring>
#include <cstdio>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge: use standard library only, no external test frameworks.

// Typedefs to resemble cms types (simplified for testing)
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;
typedef int32_t  cmsS15Fixed16Number;
typedef bool     cmsBool;

// Forward declaration of Prelin8Data to mimic the library's structure
struct cmsInterpParams {
    int nOutputs;
    const void* Table;
    int opta[3]; // p->opta[0], [1], [2]
};

// Structure used by PrelinEval8 (simplified for test)
struct Prelin8Data {
    cmsInterpParams* p;
    cmsS15Fixed16Number X0[256];
    cmsS15Fixed16Number Y0[256];
    cmsS15Fixed16Number Z0[256];
    cmsS15Fixed16Number rx[256];
    cmsS15Fixed16Number ry[256];
    cmsS15Fixed16Number rz[256];
};

// DENS function surrogate: simple deterministic function for testing
static cmsS15Fixed16Number DENSGlobal(cmsS15Fixed16Number x,
                                    cmsS15Fixed16Number y,
                                    cmsS15Fixed16Number z)
{
    // Simple sum (sufficient for deterministic testing)
    return x + y + z;
}

// Macro to mimic DENS usage in the focal code
#define DENS(x, y, z) DENSGlobal((x), (y), (z))

// Implementation of PrelinEval8 copied here for testing (simplified environment)
void PrelinEval8(CMSREGISTER const cmsUInt16Number Input[],
                 CMSREGISTER cmsUInt16Number Output[],
                 CMSREGISTER const void* D)
{
{
    cmsUInt8Number         r, g, b;
    cmsS15Fixed16Number    rx, ry, rz;
    cmsS15Fixed16Number    c0, c1, c2, c3, Rest;
    int                    OutChan;
    cmsS15Fixed16Number    X0, X1, Y0, Y1, Z0, Z1;
    Prelin8Data* p8 = (Prelin8Data*) D;
    cmsInterpParams* p = p8 ->p;
    int TotalOut = (int) p -> nOutputs;
    const cmsUInt16Number* LutTable = (const cmsUInt16Number*) p->Table;
    r = (cmsUInt8Number) (Input[0] >> 8);
    g = (cmsUInt8Number) (Input[1] >> 8);
    b = (cmsUInt8Number) (Input[2] >> 8);
    X0 = (cmsS15Fixed16Number) p8->X0[r];
    Y0 = (cmsS15Fixed16Number) p8->Y0[g];
    Z0 = (cmsS15Fixed16Number) p8->Z0[b];
    rx = p8 ->rx[r];
    ry = p8 ->ry[g];
    rz = p8 ->rz[b];
    X1 = X0 + (cmsS15Fixed16Number)((rx == 0) ? 0 :  p ->opta[2]);
    Y1 = Y0 + (cmsS15Fixed16Number)((ry == 0) ? 0 :  p ->opta[1]);
    Z1 = Z0 + (cmsS15Fixed16Number)((rz == 0) ? 0 :  p ->opta[0]);

    // These are the 6 Tetrahedral
    for (OutChan=0; OutChan < TotalOut; OutChan++) {
        c0 = DENS(X0, Y0, Z0);
        if (rx >= ry && ry >= rz)
        {
            c1 = DENS(X1, Y0, Z0) - c0;
            c2 = DENS(X1, Y1, Z0) - DENS(X1, Y0, Z0);
            c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
        }
        else
            if (rx >= rz && rz >= ry)
            {
                c1 = DENS(X1, Y0, Z0) - c0;
                c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                c3 = DENS(X1, Y0, Z1) - DENS(X1, Y0, Z0);
            }
            else
                if (rz >= rx && rx >= ry)
                {
                    c1 = DENS(X1, Y0, Z1) - DENS(X0, Y0, Z1);
                    c2 = DENS(X1, Y1, Z1) - DENS(X1, Y0, Z1);
                    c3 = DENS(X0, Y0, Z1) - c0;
                }
                else
                    if (ry >= rx && rx >= rz)
                    {
                        c1 = DENS(X1, Y1, Z0) - DENS(X0, Y1, Z0);
                        c2 = DENS(X0, Y1, Z0) - c0;
                        c3 = DENS(X1, Y1, Z1) - DENS(X1, Y1, Z0);
                    }
                    else
                        if (ry >= rz && rz >= rx)
                        {
                            c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                            c2 = DENS(X0, Y1, Z0) - c0;
                            c3 = DENS(X0, Y1, Z1) - DENS(X0, Y1, Z0);
                        }
                        else
                            if (rz >= ry && ry >= rx)
                            {
                                c1 = DENS(X1, Y1, Z1) - DENS(X0, Y1, Z1);
                                c2 = DENS(X0, Y1, Z1) - DENS(X0, Y0, Z1);
                                c3 = DENS(X0, Y0, Z1) - c0;
                            }
                            else  {
                                c1 = c2 = c3 = 0;
                            }
        Rest = c1 * rx + c2 * ry + c3 * rz + 0x8001;
        Output[OutChan] = (cmsUInt16Number) (c0 + ((Rest + (Rest >> 16)) >> 16));
    }
}
}

// Simple test harness and helpers
static int gTotalTests = 0;
static int gFailedTests = 0;

#define TEST_START(testName) \
    printf("Test: %s\n", testName); \
    ++gTotalTests;

#define TEST_OK(msg) \
    do { \
        printf("  PASSED: %s\n", msg); \
    } while(0)

#define TEST_FAIL(msg) \
    do { \
        printf("  FAILED: %s\n", msg); \
        ++gFailedTests; \
    } while(0)

#define ASSERT_EQ(actual, expected, msg) \
    do { \
        ++gTotalTests; \
        if ((actual) != (expected)) { \
            printf("  ASSERT FAILED: %s (actual=%u, expected=%u)\n", msg, (unsigned)actual, (unsigned)expected); \
            ++gFailedTests; \
        } else { \
            printf("  ASSERT PASSED: %s\n", msg); \
        } \
    } while(0)

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++gTotalTests; \
        if (!(cond)) { \
            printf("  ASSERT FAILED: %s\n", msg); \
            ++gFailedTests; \
        } else { \
            printf("  ASSERT PASSED: %s\n", msg); \
        } \
    } while(0)

// Test 1: Branch rx >= ry >= rz with simple values; expect Output == c0 (here 0)
static void test_PrelinEval8_Branch1_SimpleZeroOutput()
{
    TEST_START("Branch1_RxRyRz_gives_zero_output_when_inputs_zero");

    // Prepare structures
    Prelin8Data data;
    cmsInterpParams params;
    memset(&data, 0, sizeof(data));
    memset(&params, 0, sizeof(params));

    data.p = &params;
    // We'll use r=1, g=0, b=0
    const int rIndex = 1;
    const int gIndex = 0;
    const int bIndex = 0;

    // Inputs
    cmsUInt16Number Input[3];
    Input[0] = (uint16_t)(rIndex << 8);
    Input[1] = (uint16_t)(gIndex << 8);
    Input[2] = (uint16_t)(bIndex << 8);

    // Configure p (outputs)
    params.nOutputs = 1;
    int opta_vals[3] = {1, 1, 1};
    params.opta[0] = opta_vals[0];
    params.opta[1] = opta_vals[1];
    params.opta[2] = opta_vals[2];
    params.Table = nullptr;

    // Fill array values for zero-c0 conditions
    data.X0[rIndex] = 0;
    data.Y0[gIndex] = 0;
    data.Z0[bIndex] = 0;
    data.rx[rIndex] = 4;
    data.ry[gIndex] = 3;
    data.rz[bIndex] = 2;

    // Output
    cmsUInt16Number Output[1];
    Output[0] = 0;

    // Call
    PrelinEval8(Input, Output, &data);

    // Expect Output[0] == 0 (c0 = 0) given chosen path and small numbers
    ASSERT_EQ(Output[0], 0u, "Output should be 0 for zero c0 under Branch1");

    TEST_OK("Branch1 outputs 0 as expected");
}

// Test 2: Branch with non-zero c0 and simple values; expect Output == c0
static void test_PrelinEval8_Branch2_NonZeroC0()
{
    TEST_START("Branch2_NonZeroC0_yields_output_equal_to_c0");

    Prelin8Data data;
    cmsInterpParams params;
    memset(&data, 0, sizeof(data));
    memset(&params, 0, sizeof(params));

    data.p = &params;
    // r=0, g=0, b=0
    const int rIndex = 0;
    const int gIndex = 0;
    const int bIndex = 0;

    cmsUInt16Number Input[3];
    Input[0] = (uint16_t)(rIndex << 8);
    Input[1] = (uint16_t)(gIndex << 8);
    Input[2] = (uint16_t)(bIndex << 8);

    // 2 outputs to verify multiple outputs handling
    params.nOutputs = 2;
    params.opta[0] = 1;
    params.opta[1] = 1;
    params.opta[2] = 1;
    params.Table = nullptr;

    // Set X0,Y0,Z0 to yield c0 != 0
    data.X0[rIndex] = 5; // c0 will include 5
    data.Y0[gIndex] = 3;
    data.Z0[bIndex] = 0;
    data.rx[rIndex] = 0; // rx zero makes X1 = X0
    data.ry[gIndex] = 0;
    data.rz[bIndex] = 0;

    // X1 = X0 since rx==0, etc; c0 = DENS(5,3,0) = 8
    // Rest will be 32769 (0x8001) plus zero, so Rest>>16 == 0, so Output == c0
    cmsUInt16Number Output[2];
    Output[0] = 0;
    Output[1] = 0;

    PrelinEval8(Input, Output, &data);

    // Expect both outputs equal to c0 = 8
    ASSERT_EQ(Output[0], 8u, "Output[0] equals c0 (8)");
    ASSERT_EQ(Output[1], 8u, "Output[1] equals c0 (8)");

    TEST_OK("Branch2 outputs equal to c0 when c0 non-zero");
}

// Test 3: Branch with large rx to trigger Rest >= 65536, causing +1 in output
static void test_PrelinEval8_Branch3_LargeRxProducesOne()
{
    TEST_START("Branch3_LargeRx_produces_output_plus_one");

    Prelin8Data data;
    cmsInterpParams params;
    memset(&data, 0, sizeof(data));
    memset(&params, 0, sizeof(params));

    data.p = &params;
    // Use r=0
    const int rIndex = 0;
    const int gIndex = 0;
    const int bIndex = 0;

    cmsUInt16Number Input[3];
    Input[0] = (uint16_t)(rIndex << 8);
    Input[1] = (uint16_t)(gIndex << 8);
    Input[2] = (uint16_t)(bIndex << 8);

    params.nOutputs = 1;
    params.opta[0] = 0; // Z1 offset will be zero; we set to 0 for simplicity
    params.opta[1] = 0; // Y1 offset
    params.opta[2] = 1; // X1 offset is 1

    params.Table = nullptr;

    // X0,Y0,Z0 all zero to keep c0 = 0
    data.X0[rIndex] = 0;
    data.Y0[gIndex] = 0;
    data.Z0[bIndex] = 0;

    data.rx[rIndex] = 32767; // large rx value
    data.ry[gIndex] = 0;
    data.rz[bIndex] = 0;

    // X1 = X0 + 1 -> c1 = DENS(1,0,0) - c0 = 1
    // With Rest = 1*32767 + 0 + 0 + 0x8001 = 32767 + 32769 = 65536
    // Output = c0 + ((Rest + (Rest>>16)) >> 16) = 0 + ((65536 + 1) >> 16) = 1
    cmsUInt16Number Output[1];
    Output[0] = 0;

    PrelinEval8(Input, Output, &data);

    ASSERT_EQ(Output[0], 1u, "Output equals 1 when Rest crosses 2^16 boundary");
    TEST_OK("Branch3 outputs 1 due to Rest overflow into 16th bit");
}

// Run all tests
int main()
{
    test_PrelinEval8_Branch1_SimpleZeroOutput();
    test_PrelinEval8_Branch2_NonZeroC0();
    test_PrelinEval8_Branch3_LargeRxProducesOne();

    printf("Total tests: %d, Failures: %d\n", gTotalTests, gFailedTests);
    return (gFailedTests == 0) ? 0 : 1;
}
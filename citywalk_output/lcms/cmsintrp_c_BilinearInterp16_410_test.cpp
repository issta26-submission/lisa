// Lightweight C++11 unit tests for BilinearInterp16 in cmsintrp.c
// No GTest usage. A minimal test harness with non-terminating EXPECT_ assertions.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Simple non-terminating test macros
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ FAILED: " #a " (" << (a) << ") != " #b " (" << (b) \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: condition " #cond " is false at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE FAILED: condition " #cond " is true at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

// Helper to run BilinearInterp16 with provided parameters and capture outputs
static void RunBilinearInterp16(
    const cmsUInt16Number Input0, const cmsUInt16Number Input1,
    int nOutputs,
    cmsUInt16Number Domain0, cmsUInt16Number Domain1,
    cmsUInt16Number opta0, cmsUInt16Number opta1,
    std::vector<cmsUInt16Number>& Table, // interpolation table
    std::vector<cmsUInt16Number>& Output) {

    // Prepare input array
    cmsUInt16Number Input[2] = { Input0, Input1 };

    // Prepare cmsInterpParams
    cmsInterpParams p;
    // Zero initialize to avoid using uninitialized fields
    std::memset(&p, 0, sizeof(p));

    // Attach table and parameters
    p.Table     = Table.data();
    p.nOutputs  = static_cast<cmsUInt32Number>(nOutputs);
    p.Domain[0] = Domain0;
    p.Domain[1] = Domain1;
    p.opta[0]   = opta0;
    p.opta[1]   = opta1;

    // Prepare output buffer
    Output.assign(nOutputs, 0);

    // Call the focal method
    BilinearInterp16(Input, Output.data(), &p);
}

// Test 1: Basic behavior with zero interpolation table (expect all outputs = 0)
static void Test_BilinearInterp16_ZeroTable_NoInterpolation() {
    // Configure: no interpolation effect due to zero table
    int nOutputs = 1;
    std::vector<cmsUInt16Number> Table(1, 0); // LutTable[0] = 0
    std::vector<cmsUInt16Number> Output;

    // Domain set to 0 to simplify internal calculations (x0,y0 remain 0)
    cmsUInt16Number Domain0 = 0;
    cmsUInt16Number Domain1 = 0;
    cmsUInt16Number opta0 = 1;
    cmsUInt16Number opta1 = 1;

    // Inputs chosen to exercise normal path (non-FFFU)
    cmsUInt16Number Input0 = 0; // not 0xFFFFU
    cmsUInt16Number Input1 = 0; // not 0xFFFFU

    RunBilinearInterp16(Input0, Input1, nOutputs, Domain0, Domain1, opta0, opta1, Table, Output);

    // Expect the single output to be 0
    EXPECT_EQ(Output.size(), static_cast<size_t>(nOutputs));
    EXPECT_EQ(Output[0], 0);
}

// Test 2: Branch true for Input0 (Input0 == 0xFFFFU) with Domain0 = Domain1 = 0
// Interpolation reduces to DENS(0,0) for OutChan, so Output should equal Table[OutChan]
static void Test_BilinearInterp16_Input0FFFU_BranchTrue() {
    int nOutputs = 1;
    std::vector<cmsUInt16Number> Table(1, 123); // LutTable[0] = 123
    std::vector<cmsUInt16Number> Output;

    cmsUInt16Number Domain0 = 0;
    cmsUInt16Number Domain1 = 0;
    cmsUInt16Number opta0 = 1;
    cmsUInt16Number opta1 = 1;

    cmsUInt16Number Input0 = 0xFFFFU; // triggers X1 = X0 (branch true)
    cmsUInt16Number Input1 = 0;       // Y branch not taken for 0 => still 0 rest

    RunBilinearInterp16(Input0, Input1, nOutputs, Domain0, Domain1, opta0, opta1, Table, Output);

    // Expect the single output to equal LutTable[0] = 123
    EXPECT_EQ(Output.size(), static_cast<size_t>(nOutputs));
    EXPECT_EQ(Output[0], 123);
}

// Test 3: Multi-output support with same simple conditions as Test 2
// Verifies that separate channels fetch their corresponding table entries
static void Test_BilinearInterp16_MultiOutput_Input0FFFU() {
    int nOutputs = 2;
    std::vector<cmsUInt16Number> Table(2, 0);
    Table[0] = 7;  // OutChan 0 value at (0,0)
    Table[1] = 9;  // OutChan 1 value at (0,0)

    std::vector<cmsUInt16Number> Output;

    cmsUInt16Number Domain0 = 0;
    cmsUInt16Number Domain1 = 0;
    cmsUInt16Number opta0 = 1;
    cmsUInt16Number opta1 = 1;

    cmsUInt16Number Input0 = 0xFFFFU; // trigger X1 branch true
    cmsUInt16Number Input1 = 0;       // Y branch simple

    RunBilinearInterp16(Input0, Input1, nOutputs, Domain0, Domain1, opta0, opta1, Table, Output);

    // Expect outputs to follow LutTable per channel: 7 for channel 0, 9 for channel 1
    EXPECT_EQ(Output.size(), static_cast<size_t>(nOutputs));
    EXPECT_EQ(Output[0], 7);
    EXPECT_EQ(Output[1], 9);
}

// Test 4: Branch true for Input0 and Input1 (both 0xFFFFU) with Domain0 = Domain1 = 0
// This ensures the Y-branch is exercised as well; Output should equal Table[OutChan]
static void Test_BilinearInterp16_Input0FFFU_Input1FFFU() {
    int nOutputs = 1;
    std::vector<cmsUInt16Number> Table(1, 555);

    std::vector<cmsUInt16Number> Output;

    cmsUInt16Number Domain0 = 0;
    cmsUInt16Number Domain1 = 0;
    cmsUInt16Number opta0 = 2;
    cmsUInt16Number opta1 = 3;

    cmsUInt16Number Input0 = 0xFFFFU;
    cmsUInt16Number Input1 = 0xFFFFU;

    RunBilinearInterp16(Input0, Input1, nOutputs, Domain0, Domain1, opta0, opta1, Table, Output);

    // With both inputs = 0xFFFFU and domains 0, interpolation collapses to d00 = Table[0]
    EXPECT_EQ(Output.size(), static_cast<size_t>(nOutputs));
    EXPECT_EQ(Output[0], 555);
}

// Runner that executes all tests and reports summary
int main() {
    std::cout << "Starting BilinearInterp16 unit tests (CMS LittleCMS integration)" << std::endl;

    Test_BilinearInterp16_ZeroTable_NoInterpolation();
    Test_BilinearInterp16_Input0FFFU_BranchTrue();
    Test_BilinearInterp16_MultiOutput_Input0FFFU();
    Test_BilinearInterp16_Input0FFFU_Input1FFFU();

    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << "TESTS FAILED: " << g_failed_tests << " assertion(s) failed" << std::endl;
    }
    return g_failed_tests;
}
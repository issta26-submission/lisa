/*
 * Unit test harness for the XFormSampler16 function (cmsInt32Number XFormSampler16(...))
 * 
 * Notes:
 * - This test suite targets the focal method XFormSampler16 located in cmsopt.c.
 * - It relies on the public/internal LCMS data structures (e.g., cmsPipeline) and
 *   typical LCMS behavior (identity-like evaluation for an empty pipeline).
 * - The tests avoid terminating on assertion failures and use a lightweight
 *   testing macro set defined herein (non-terminating EXPECT_*).
 * - The test suite is designed to compile with C++11 and without GoogleTest.
 *
 * Important: This test assumes that the LCMS environment provides:
 * - cmsPipeline type with at least fields InputChannels, OutputChannels, Elements.
 * - A sane default for cmsPipelineEvalFloat when Elements is NULL (identity).
 * - The function XFormSampler16 is linked from the cmsopt.c translation unit.
 * - CMSREGISTER, cmsUInt16Number, cmsInt32Number, cmsBool, TRUE, etc., are defined
 *   via lcms2_internal.h (included below).
 *
 * The tests exercise:
 *  - Basic identity-path behavior (empty/identity pipeline)
 *  - Correct 16-bit to floating-point conversion and back (extreme values 0 and 65535)
 *  - Handling of multiple channels (up to 3 channels in tests)
 *  - Return value semantics (TRUE is returned)
 *  - Non-terminating assertions via EXPECT_* (no abort on failure)
 *
 * The test harness prints a summary of passed/failed tests.
 */

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Include LCMS internal headers to obtain types like cmsPipeline, cmsUInt16Number, etc.

// Declaration for the focal function (extern "C" to avoid C++ mangling)
extern "C" cmsInt32Number XFormSampler16(CMSREGISTER const cmsUInt16Number In[],
                                       CMSREGISTER cmsUInt16Number Out[],
                                       CMSREGISTER void* Cargo);

/* Lightweight test harness - non-terminating assertions */
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        tests_failed++; \
    } else { \
        tests_passed++; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        tests_failed++; \
    } else { \
        tests_passed++; \
    } \
} while(0)

#define EXPECT_APPROX(a, b, tol) do { \
    if (std::abs((a) - (b)) > (tol)) { \
        std::cerr << "EXPECT_APPROX failed: " #a " ~ " #b " (|a-b|=" << std::fabs((a)-(b)) \
                  << ", tol=" << (tol) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        tests_failed++; \
    } else { \
        tests_passed++; \
    } \
} while(0)

// Global counters for test results
static int tests_passed = 0;
static int tests_failed = 0;

/*
 * Helper: make a simple identity-like Lut (pipeline) to drive XFormSampler16.
 * 
 * We rely on the LCMS convention that an empty/identity pipeline (Elements == NULL)
 * yields a no-op transformation. We construct a minimal cmsPipeline with
 * InputChannels and OutputChannels set. Elements is assumed to be NULL by default.
 *
 * Note: The real LCMS pipeline struct can have more fields; we only touch the
 * fields used by XFormSampler16 (InputChannels, OutputChannels, Elements).
 */
static inline void prepare_identity_lut(cmsPipeline& Lut, cmsUInt16Number inChans, cmsUInt16Number outChans)
{
    Lut.InputChannels = inChans;
    Lut.OutputChannels = outChans;
    Lut.Elements = NULL;           // null-Elements implies identity-like behavior for eval
    // Other fields are intentionally left at default/zero.
}

/*
 * Test 1: Single-channel identity path
 * - Build a 1->1 pipeline with identity behavior.
 * - In: 0 maps to 0; 65535 maps to 65535
 * - Verifies: Out equals In after transformation.
 */
static void test_XFormSampler16_SingleChannelIdentity()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 1, 1);

    cmsUInt16Number In[1]  = { 0 };
    cmsUInt16Number Out[1] = { 0 };

    cmsInt32Number ret = XFormSampler16(In, Out, &Lut);

    // Expect function always returns TRUE
    EXPECT_TRUE(ret == TRUE);

    // Expected identity behavior for 0
    EXPECT_EQ(Out[0], In[0]);
}

static void test_XFormSampler16_SingleChannelIdentity_Max()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 1, 1);

    cmsUInt16Number In[1]  = { 65535 };
    cmsUInt16Number Out[1] = { 0 };

    cmsInt32Number ret = XFormSampler16(In, Out, &Lut);

    // Expect function always returns TRUE
    EXPECT_TRUE(ret == TRUE);

    // Expected identity behavior for max value
    EXPECT_EQ(Out[0], In[0]);
}

/*
 * Test 2: Multi-channel identity path (2 channels)
 * - Build a 2->2 pipeline with identity behavior.
 * - In: {0, 65535} -> Out: {0, 65535}
 */
static void test_XFormSampler16_TwoChannelsIdentity()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 2, 2);

    cmsUInt16Number In[2]  = { 0, 65535 };
    cmsUInt16Number Out[2] = { 0, 0 };

    cmsInt32Number ret = XFormSampler16(In, Out, &Lut);

    // Basic return value check
    EXPECT_TRUE(ret == TRUE);

    // Validate per-channel identity
    EXPECT_EQ(Out[0], In[0]);
    EXPECT_EQ(Out[1], In[1]);
}

/*
 * Test 3: Multi-channel identity path (3 channels)
 * - Build a 3->3 pipeline with identity behavior.
 * - In: {34000, 20000, 65535} -> Out should match Input.
 */
static void test_XFormSampler16_ThreeChannelsIdentity()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 3, 3);

    cmsUInt16Number In[3]  = { 34000, 20000, 65535 };
    cmsUInt16Number Out[3] = { 0, 0, 0 };

    cmsInt32Number ret = XFormSampler16(In, Out, &Lut);

    // Return value check
    EXPECT_TRUE(ret == TRUE);

    // Identity mapping per channel
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(Out[i], In[i]);
    }
}

/*
 * Test 4: Extreme values test (bounds check for 0 and 65535)
 * - Ensures conversion to float and back saturates correctly at boundaries.
 */
static void test_XFormSampler16_Boundaries()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 1, 1);

    // Test lower boundary
    cmsUInt16Number InLow[1]  = { 0 };
    cmsUInt16Number OutLow[1] = { 1234 }; // initialize to non-zero to verify overwrite
    cmsInt32Number retLow = XFormSampler16(InLow, OutLow, &Lut);
    EXPECT_TRUE(retLow == TRUE);
    EXPECT_EQ(OutLow[0], InLow[0]);

    // Test upper boundary
    cmsUInt16Number InHigh[1]  = { 65535 };
    cmsUInt16Number OutHigh[1] = { 0 };
    cmsInt32Number retHigh = XFormSampler16(InHigh, OutHigh, &Lut);
    EXPECT_TRUE(retHigh == TRUE);
    EXPECT_EQ(OutHigh[0], InHigh[0]);
}

/*
 * Test 5: Mixed channels with different inputs to validate general correctness
 * - 2->2 pipeline with 0, 32768 inputs
 * - Verify that Out equals In when the pipeline is identity.
 */
static void test_XFormSampler16_MixedInputsIdentity()
{
    cmsPipeline Lut;
    prepare_identity_lut(Lut, 2, 2);

    cmsUInt16Number In[2]  = { 0, 32768 };
    cmsUInt16Number Out[2] = { 0, 0 };

    cmsInt32Number ret = XFormSampler16(In, Out, &Lut);

    // Return value
    EXPECT_TRUE(ret == TRUE);

    // Check per-channel equality
    EXPECT_EQ(Out[0], In[0]);
    EXPECT_EQ(Out[1], In[1]);
}

/*
 * Runner: executes all tests and reports summary.
 */
int main()
{
    std::cout << "XFormSampler16 unit tests (cmsopt.c) - C++11 test harness\n";

    // Run tests
    test_XFormSampler16_SingleChannelIdentity();
    test_XFormSampler16_SingleChannelIdentity_Max();
    test_XFormSampler16_TwoChannelsIdentity();
    test_XFormSampler16_ThreeChannelsIdentity();
    test_XFormSampler16_Boundaries();
    test_XFormSampler16_MixedInputsIdentity();

    // Summary
    std::cout << "Tests run: " << (tests_passed + tests_failed)
              << ", Passed: " << tests_passed
              << ", Failed: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}
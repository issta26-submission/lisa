// This test suite targets the FromFloatTo16 function in cmslut.c.
// It exercises conversion from cmsFloat32Number In[] to cmsUInt16Number Out[]
// using the exact mapping In[i] * 65535.0 followed by saturation via _cmsQuickSaturateWord.
// The test suite avoids GTest and uses a lightweight, self-contained harness.
// <Candidate Keywords> FromFloatTo16, In[], Out[], n, 65535.0, _cmsQuickSaturateWord, Saturation, Casting

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


#ifdef __cplusplus
extern "C" {
#endif
    // Include the LittleCMS headers to obtain the type definitions.
    // These headers are assumed to be available in the build environment.
    #include "lcms2.h"
    // Prototype for the focal function under test (C linkage)
    void FromFloatTo16(const cmsFloat32Number In[], cmsUInt16Number Out[], cmsUInt32Number n);
#ifdef __cplusplus
}
#endif

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

static void ExpectEqInt(const char* desc, uint16_t expected, uint16_t actual) {
    if (expected != actual) {
        ++g_failures;
        std::cerr << "FAIL: " << desc
                  << " | Expected: " << static_cast<uint32_t>(expected)
                  << ", Actual: " << static_cast<uint32_t>(actual) << std::endl;
    }
}

static void ExpectEqBool(const char* desc, bool expected, bool actual) {
    if (expected != actual) {
        ++g_failures;
        std::cerr << "FAIL: " << desc
                  << " | Expected: " << (expected ? "TRUE" : "FALSE")
                  << ", Actual: " << (actual ? "TRUE" : "FALSE") << std::endl;
    }
}

static void ExpectTrue(const char* desc, bool condition) {
    ExpectEqBool(desc, true, condition);
}

// Test 1: Basic mapping of boundary and mid values.
// Verifies 0.0 -> 0, 0.5 -> either 32767 or 32768 depending on rounding mode,
// and 1.0 -> 65535. Also checks that input is not modified.
static bool Test_FromFloatTo16_BasicMapping()
{
    // Candidate Keywords: In, Out, n, 0.0f, 0.5f, 1.0f, 65535
    cmsFloat32Number In[3] = { 0.0f, 0.5f, 1.0f };
    cmsUInt16Number Out[3] = { 0, 0, 0 };

    // Run the function under test
    FromFloatTo16(In, Out, 3);

    // Validate outputs
    ExpectEqInt("FromFloatTo16: 0.0 -> 0", 0, Out[0]);

    // 0.5 * 65535.0 = 32767.5 -> result may be 32767 or 32768 depending on rounding
    if (Out[1] != 32767 && Out[1] != 32768) {
        ++g_failures;
        std::cerr << "FAIL: FromFloatTo16: 0.5 mapping expected 32767 or 32768, got " << Out[1] << std::endl;
    }

    ExpectEqInt("FromFloatTo16: 1.0 -> 65535", 65535, Out[2]);

    // Input immutability check
    cmsFloat32Number InCopy[3] = {0.0f, 0.5f, 1.0f};
    // Create a copy and call again to ensure input buffer isn't modified by the function
    cmsUInt16Number dummyOut[3];
    FromFloatTo16(InCopy, dummyOut, 3);
    // The function should not modify InCopy
    bool inputsUnmodified = (std::memcmp(InCopy, (const cmsFloat32Number[]) {0.0f, 0.5f, 1.0f}, sizeof(InCopy)) == 0);
    // Note: std::memcmp usage here relies on exact bit representation for these values;
    // since 0.0, 0.5 and 1.0 are exact in IEEE-754, this is reliable.
    ExpectTrue("FromFloatTo16 should not modify input In array (immutability)", inputsUnmodified);

    return g_failures == 0;
}

// Test 2: Boundary and mixed values including negatives and values > 1.
// Verifies saturating behavior: negative maps to 0, >1 maps to 65535, 0.5 maps as in Test 1.
static bool Test_FromFloatTo16_BoundsAndRounding()
{
    // Candidate Keywords: In, Out, n, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, saturation
    cmsFloat32Number In[5] = { -0.5f, 0.0f, 0.5f, 1.0f, 2.0f };
    cmsUInt16Number Out[5] = { 0, 0, 0, 0, 0 };

    FromFloatTo16(In, Out, 5);

    // -0.5 -> 0 (negative values saturate to 0)
    ExpectEqInt("FromFloatTo16: -0.5 -> 0", 0, Out[0]);
    // 0.0 -> 0
    ExpectEqInt("FromFloatTo16: 0.0 -> 0", 0, Out[1]);
    // 0.5 -> 32767 or 32768
    if (Out[2] != 32767 && Out[2] != 32768) {
        ++g_failures;
        std::cerr << "FAIL: FromFloatTo16: 0.5 mapping expected 32767 or 32768, got " << Out[2] << std::endl;
    }
    // 1.0 -> 65535
    ExpectEqInt("FromFloatTo16: 1.0 -> 65535", 65535, Out[3]);
    // 2.0 -> saturates to 65535
    ExpectEqInt("FromFloatTo16: 2.0 -> 65535", 65535, Out[4]);

    // Immutability check on this test as well
    cmsFloat32Number InCopy[5] = { -0.5f, 0.0f, 0.5f, 1.0f, 2.0f };
    cmsUInt16Number dummyOut2[5];
    FromFloatTo16(InCopy, dummyOut2, 5);
    bool inputsUnmodified = (std::memcmp(InCopy, (const cmsFloat32Number[]){-0.5f, 0.0f, 0.5f, 1.0f, 2.0f}, sizeof(InCopy)) == 0);
    ExpectTrue("FromFloatTo16 should not modify input In array (immutability) - Test 2", inputsUnmodified);

    return g_failures == 0;
}

// Test 3: Zero-length input array should be handled gracefully (no writes, no crashes)
static bool Test_FromFloatTo16_ZeroLength()
{
    // Candidate Keywords: In, Out, n=0
    cmsFloat32Number In[1] = { 0.0f };
    cmsUInt16Number Out[1] = { 0 };

    FromFloatTo16(In, Out, 0); // Should not crash or write
    // Since n==0, Out should remain unchanged (still 0)
    if (Out[0] != 0) {
        ++g_failures;
        std::cerr << "FAIL: FromFloatTo16 with n=0 should not modify Out; got " << Out[0] << std::endl;
    }
    return g_failures == 0;
}

// Entry point: run all tests and report summary
int main()
{
    std::cout << "Starting FromFloatTo16 unit tests (CMS LittleCMS)..." << std::endl;

    bool t1 = Test_FromFloatTo16_BasicMapping();
    bool t2 = Test_FromFloatTo16_BoundsAndRounding();
    bool t3 = Test_FromFloatTo16_ZeroLength();

    int totalTests = 3;
    int failedTests = (t1 ? 0 : 1) + (t2 ? 0 : 1) + (t3 ? 0 : 1);
    if (failedTests == 0) {
        std::cout << "ALL TESTS PASSED (" << totalTests << " tests)." << std::endl;
    } else {
        std::cout << failedTests << " test(s) failed out of " << totalTests << " tests." << std::endl;
        std::cout << "Total assertion failures across tests: " << g_failures << std::endl;
    }

    // Return non-zero if any test failed
    return (failedTests == 0) ? 0 : 1;
}
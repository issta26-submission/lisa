// Test suite for UnrollFloatTo16 in cmspack.c
// Note: This test harness relies on LittleCMS internal macros and types.
// It uses a lightweight, non-terminating assertion style to maximize coverage.
// If the required macros are not available in the environment, the tests are skipped gracefully.

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Forward-declare types/macros if available from LittleCMS headers

// Ensure we can reference the function under test
// The function is defined in C; expose with C linkage in C++ test file.
extern "C" {
    cmsUInt8Number* UnrollFloatTo16(CMSREGISTER _cmsTRANSFORM* info,
                                    CMSREGISTER cmsUInt16Number wIn[],
                                    CMSREGISTER cmsUInt8Number* accum,
                                    CMSREGISTER cmsUInt32Number Stride);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

static void logFail(const char* testName, const char* msg, int line)
{
    fprintf(stderr, "FAIL: %s (line %d): %s\n", testName, line, msg);
    g_failures++;
}

#define EXPECT_TRUE(cond, testName, msg) do { \
    if(!(cond)) logFail(testName, msg, __LINE__); \
} while(false)

#define EXPECT_EQ(a, b, testName, msg) do { \
    if((a) != (b)) logFail(testName, msg, __LINE__); \
} while(false)

static void test_BasicSingleChannel()
{
    const char* testName = "UnrollFloatTo16_BasicSingleChannel";

    // Build an info structure with 1 channel
    _cmsTRANSFORM info;
    // CHANNELS_SH is the LittleCMS macro to encode the number of channels.
    // We rely on the library's macros; if not defined, this test will be skipped gracefully.
#if defined(CHANNELS_SH) && defined(DOSWAP_SH) && defined(FLAVOR_SH) && defined(SWAPFIRST_SH) && defined(EXTRA_SH) && defined(PLANAR_SH) && defined(ISINKSPACE)
    info.InputFormat = CHANNELS_SH(1); // 1 channel, default flags
#else
    // If the macros aren't available, skip this test gracefully.
    fprintf(stderr, "SKIP: BasicSingleChannel test skipped (macros CHANNELS_SH etc. not available).\n");
    return;
#endif

    cmsUInt16Number wIn[1] = { 0 };
    // accum buffer holding 4 cmsFloat32Number values
    cmsFloat32Number accumVals[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accumVals);
    cmsUInt32Number Stride = 4; // one sample per channel, one channel

    cmsUInt8Number* next = UnrollFloatTo16(&info, wIn, accum, Stride);

    // Expect: wIn[0] saturated to max (65535)
    EXPECT_EQ( wIn[0], 65535u, testName, "Single channel should saturate to 65535");
    // Expect: function advances accum by sizeof(float)
    EXPECT_TRUE(next == accum + sizeof(cmsFloat32Number), testName,
                "Return pointer should advance by one float when Planar is false and Extra is 0");
}

static void test_BasicTwoChannels()
{
    const char* testName = "UnrollFloatTo16_TwoChannels";

#if defined(CHANNELS_SH) && defined(DOSWAP_SH) && defined(FLAVOR_SH) && defined(SWAPFIRST_SH) && defined(EXTRA_SH) && defined(PLANAR_SH)
    // Compose InputFormat for 2 channels
    _cmsTRANSFORM info;
    info.InputFormat = CHANNELS_SH(2); // 2 channels
#else
    fprintf(stderr, "SKIP: TwoChannels test skipped (macros CHANNELS_SH etc. not available).\n");
    return;
#endif

    cmsUInt16Number wIn[2] = { 0, 0 };
    cmsFloat32Number accumVals[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accumVals);
    cmsUInt32Number Stride = 4;

    cmsUInt8Number* next = UnrollFloatTo16(&info, wIn, accum, Stride);

    // Expect: both channels saturated to max since accum values are 1.0
    EXPECT_EQ(wIn[0], 65535u, testName, "First channel saturates to 65535");
    EXPECT_EQ(wIn[1], 65535u, testName, "Second channel saturates to 65535");
    EXPECT_TRUE(next == accum + 2 * sizeof(cmsFloat32Number),
                testName, "Return pointer should advance by nChan*sizeof(float) when Extra==0");
}

static void test_ExtraNonZero_ReturnOffset()
{
    const char* testName = "UnrollFloatTo16_ExtraNonZero_ReturnOffset";

#if defined(CHANNELS_SH) && defined(EXTRA_SH) && defined(DOSWAP_SH) && defined(FLAVOR_SH) && defined(SWAPFIRST_SH) && defined(PLANAR_SH)
    _cmsTRANSFORM info;
    // 1 channel with Extra = 1
    info.InputFormat = CHANNELS_SH(1) | EXTRA_SH(1);
#else
    fprintf(stderr, "SKIP: ExtraNonZero_ReturnOffset test skipped (macros not available).\n");
    return;
#endif

    cmsUInt16Number wIn[1] = { 0 };
    cmsFloat32Number accumVals[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accumVals);
    cmsUInt32Number Stride = 4;

    cmsUInt8Number* next = UnrollFloatTo16(&info, wIn, accum, Stride);

    // Expect: output channel saturated, and return pointer advanced by (nChan+Extra)*sizeof(float) = (1+1)*4 = 8
    EXPECT_EQ(wIn[0], 65535u, testName, "Channel saturates to max for Extra case");
    EXPECT_TRUE(next == accum + 8, testName, "Return pointer should advance by (nChan+Extra)*sizeof(float) when Extra>0");
}

// Entry point
int main()
{
    // Run tests; tests guarded by macro availability
    test_BasicSingleChannel();
    test_BasicTwoChannels();
    test_ExtraNonZero_ReturnOffset();

    if (g_failures > 0) {
        fprintf(stderr, "Total test failures: %d\n", g_failures);
        return 1;
    } else {
        printf("All tests completed successfully.\n");
        return 0;
    }
}

// Explanatory notes:
// - This test suite focuses on verifying the UnrollFloatTo16 path under a few representative
//   scenarios. It uses a non-terminating assertion style to allow multiple checks per test.
// - The tests rely on LittleCMS internal macros to configure the InputFormat (e.g., CHANNELS_SH,
//   EXTRA_SH, etc.). If those macros are not present in the environment, individual tests are skipped
//   gracefully with a corresponding SKIP message.
// - The tests exercise:
//   1) Basic single-channel processing and output saturation to 65535.
//   2) Basic two-channel processing with saturation in both channels.
//   3) Handling of Extra samples (Extra != 0) which affects the returned accumulator pointer.
// - This harness uses a minimal test framework and prints failures to stderr while continuing execution
//   to maximize coverage. If a static or runtime environment lacks the required macros, the tests
//   will gracefully skip rather than fail.
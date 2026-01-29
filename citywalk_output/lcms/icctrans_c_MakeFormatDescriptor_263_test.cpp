// Lightweight unit tests for the focal method MakeFormatDescriptor (icctrans.c)
// This test harness is written for C++11 without any external test framework.
// It uses a minimal in-house assertion mechanism that does not terminate on failure,
// to maximize code coverage and allow all tests to run.
//
// Assumptions:
// - The build includes lcMS2 headers (lcms2.h) and links against the lcMS2 library.
// - MakeFormatDescriptor is implemented in icctrans.c and has C linkage.
// - We test the bit-field composition via existing macros: FLOAT_SH, COLORSPACE_SH,
//   BYTES_SH, CHANNELS_SH, PLANAR_SH, and the helper functions cmsChannelsOf and
//   _cmsLCMScolorSpace (indirectly through CHANNELS_SH and COLORSPACE_SH).
//
// Notes:
// - Only public interfaces from lcms2.h are used.
// - We test that the function correctly reflects the IsFloat decision, the bytes field,
//   the channels field, and the planar flag for two color spaces (RGB and Gray).

#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>
#include <string.h>


// Declare the focal function with C linkage to ensure proper linkage.
// The actual implementation resides in icctrans.c.
extern "C" cmsUInt32Number MakeFormatDescriptor(cmsColorSpaceSignature ColorSpace, int Bytes);

// Simple non-terminating assertion framework
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; ++g_failures; } } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { if(((a) != (b))) { std::cerr << "ASSERT_EQ failed: " << (msg) \
        << " | Expected: " << (b) << " | Actual: " << (a) << std::endl; ++g_failures; } } while(0)

// Test 1: Validate IsFloat bit behavior for RGB color space across Bytes values.
// - IsFloat should be set (1) when Bytes is 0 or 4.
// - IsFloat should be 0 when Bytes is other values (e.g., 2).
void test_IsFloat_behavior_rgb()
{
    // RGB color space
    cmsColorSpaceSignature cs = cmsSigRgbData;

    uint32_t res0 = MakeFormatDescriptor(cs, 0);
    uint32_t res4 = MakeFormatDescriptor(cs, 4);
    uint32_t res2 = MakeFormatDescriptor(cs, 2);

    // IsFloat should be set for Bytes 0 and 4
    ASSERT_TRUE((res0 & FLOAT_SH(1)) == FLOAT_SH(1),
                "IsFloat should be set when Bytes == 0");
    ASSERT_TRUE((res4 & FLOAT_SH(1)) == FLOAT_SH(1),
                "IsFloat should be set when Bytes == 4");

    // IsFloat should not be set for other Bytes values
    ASSERT_TRUE((res2 & FLOAT_SH(1)) == 0,
                "IsFloat should be 0 when Bytes == 2");
}

// Test 2: Validate BYTES_SH masking for a range of Bytes values.
// Ensures that the BYTES_SH(Bytes) portion is present in the result.
void test_Bytes_masking_rgb()
{
    cmsColorSpaceSignature cs = cmsSigRgbData;
    int testBytes[] = {0, 1, 2, 5, 16};

    for (int b : testBytes)
    {
        uint32_t res = MakeFormatDescriptor(cs, b);
        uint32_t mask = BYTES_SH(b);
        ASSERT_TRUE((res & mask) == mask,
                    "BYTES_SH(mask) should be present for Bytes=" << b);
    }
}

// Test 3: Validate CHANNELS_SH masking and PLANAR bit for RGB color space.
// Ensures the channel count is reflected and the planar flag is set.
void test_Channels_and_Planar_rgb()
{
    cmsColorSpaceSignature cs = cmsSigRgbData;
    int channels = cmsChannelsOf(cs);

    uint32_t res = MakeFormatDescriptor(cs, 3);

    uint32_t chMask = CHANNELS_SH(channels);
    // CHANNELS_SH should reflect the actual channel count
    ASSERT_TRUE((res & chMask) == chMask,
                "CHANNELS_SH should reflect channel count for RGB");

    // PLANAR_SH(1) should be set
    ASSERT_TRUE((res & PLANAR_SH(1)) == PLANAR_SH(1),
                "PLANAR_SH(1) should be present in descriptor");
}

// Test 4: Validate behavior with Gray color space (single channel).
void test_with_gray()
{
    cmsColorSpaceSignature cs = cmsSigGrayData;
    int b = 4;
    uint32_t res = MakeFormatDescriptor(cs, b);

    int channels = cmsChannelsOf(cs);
    uint32_t chMask = CHANNELS_SH(channels);

    // Gray should report 1 channel and bytes 4 should set IsFloat
    ASSERT_TRUE((res & chMask) == chMask,
                "GRAY channels should be correctly reflected in descriptor");

    ASSERT_TRUE((res & FLOAT_SH(1)) == FLOAT_SH(1),
                "IsFloat should be set for Gray with Bytes == 4");
}

int main()
{
    std::cout << "Starting unit tests for MakeFormatDescriptor...\n";

    test_IsFloat_behavior_rgb();
    test_Bytes_masking_rgb();
    test_Channels_and_Planar_rgb();
    test_with_gray();

    if (g_failures == 0)
    {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    else
    {
        std::cerr << g_failures << " test(s) failed\n";
        return 1;
    }
}
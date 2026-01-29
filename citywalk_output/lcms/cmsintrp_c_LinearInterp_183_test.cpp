// High-quality C++11 unit tests for the focal method LinearInterp in cmsintrp.c
// Note: This test harness uses a lightweight, non-GTest approach with
// non-terminating assertions to maximize code coverage.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Domain knowledge and type alignment with the focal method
typedef int32_t cmsS15Fixed16Number;   // signed 16.16 fixed-point
typedef uint16_t cmsUInt16Number;      // unsigned 16-bit
typedef uint32_t cmsUInt32Number;      // unsigned 32-bit

// Declaration of the focal function under test.
// We assume C linkage to match the definition in cmsintrp.c
extern "C" cmsUInt16Number LinearInterp(cmsS15Fixed16Number a,
                                      cmsS15Fixed16Number l,
                                      cmsS15Fixed16Number h);

// Lightweight non-terminating assertion helper
static int g_failures = 0;

static void expect_uint16(const char* test_name,
                          cmsUInt16Number expected,
                          cmsUInt16Number actual)
{
    if (expected != actual)
    {
        std::printf("FAIL  %s: expected %u, got %u\n",
                    test_name, (unsigned)expected, (unsigned)actual);
        ++g_failures;
    }
    else
    {
        std::printf("PASS  %s\n", test_name);
    }
}

// Test 1: a = 0 -> LinearInterp should return l
static void test_linearinterp_zero_factor()
{
    // Arrange
    cmsS15Fixed16Number a = 0;                 // 0.0 in 16.16
    cmsS15Fixed16Number l = 12345;             // some sample lower bound
    cmsS15Fixed16Number h = 54321;             // some sample upper bound

    // Act
    cmsUInt16Number result = LinearInterp(a, l, h);

    // Assert
    expect_uint16("LinearInterp with a=0 returns l", (cmsUInt16Number)l, result);
}

// Test 2: a = 1.0 (0x10000) -> LinearInterp should return h
static void test_linearinterp_unit_factor()
{
    // Arrange
    cmsS15Fixed16Number a = 0x00010000; // 1.0 in 16.16 fixed
    cmsS15Fixed16Number l = 12345;
    cmsS15Fixed16Number h = 54321;

    // Act
    cmsUInt16Number result = LinearInterp(a, l, h);

    // Assert
    expect_uint16("LinearInterp with a=1.0 returns h", (cmsUInt16Number)h, result);
}

// Test 3: a = 0.5 (0x8000) -> result should be floor(l + (h-l)/2)
static void test_linearinterp_half_factor()
{
    // Arrange
    cmsS15Fixed16Number a = 0x00008000; // 0.5 in 16.16 fixed
    cmsS15Fixed16Number l = 1000;
    cmsS15Fixed16Number h = 3000;

    // Expected: l + floor((h - l) / 2) = 1000 + 1000 = 2000
    cmsUInt16Number expected = 2000;

    // Act
    cmsUInt16Number result = LinearInterp(a, l, h);

    // Assert
    expect_uint16("LinearInterp with a=0.5 returns midpoint", expected, result);
}

// Test 4: l == h should always return l regardless of a
static void test_linearinterp_flat_segment()
{
    // Arrange
    cmsS15Fixed16Number a = 0x00012345; // arbitrary a
    cmsS15Fixed16Number l = 5000;
    cmsS15Fixed16Number h = 5000;        // equal to l

    // Act
    cmsUInt16Number result = LinearInterp(a, l, h);

    // Assert
    expect_uint16("LinearInterp with l==h returns l", (cmsUInt16Number)l, result);
}

// Test 5: a = 0x8000, l = 0, h = 0xFFFF -> expect floor((h-l)/2) = 0x7FFF
static void test_linearinterp_half_range()
{
    // Arrange
    cmsS15Fixed16Number a = 0x00008000; // 0.5
    cmsS15Fixed16Number l = 0;
    cmsS15Fixed16Number h = 0xFFFF;     // 65535

    // Expected: floor((65535 - 0) / 2) = 32767 = 0x7FFF
    cmsUInt16Number expected = 0x7FFF;

    // Act
    cmsUInt16Number result = LinearInterp(a, l, h);

    // Assert
    expect_uint16("LinearInterp with a=0.5 on [0,0xFFFF] yields 0x7FFF", expected, result);
}

// Main test runner
int main()
{
    std::printf("Starting LinearInterp unit tests (cmsintrp.c)\n");

    test_linearinterp_zero_factor();
    test_linearinterp_unit_factor();
    test_linearinterp_half_factor();
    test_linearinterp_flat_segment();
    test_linearinterp_half_range();

    if (g_failures == 0)
    {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    }
    else
    {
        std::printf("TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }
}
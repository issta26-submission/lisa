/*
Step 1 - Program Understanding and Candidate Keywords
- Focused method: fromHLFto8(void* dst, const void* src)
- Core dependencies (based on <FOCAL_CLASS_DEP> block and typical CMS/LCMS context):
  - _cmsHalf2Float: convert a 16-bit half-precision value to a double/float
  - _cmsQuickSaturateByte: clamp/convert floating result to an unsigned 8-bit value (0..255)
  - cmsUInt16Number (src type), cmsUInt8Number (dst type)
  - The conversion path is active only when CMS_NO_HALF_SUPPORT is not defined
  - Behavior: read a 16-bit half from src, convert to float, multiply by 255.0, saturate to 0..255, store into dst
- Key dependencies to consider for test cases:
  - Behavior for 0.0 (half 0x0000) -> 0
  - Behavior for 1.0 (half 0x3C00) -> 255
  - Behavior for -1.0 (half 0xBC00) -> 0 (due to non-negative dst after saturate)
  - Behavior for 2.0 (half 0x4000) -> 255
  - Path assumes CMS_NO_HALF_SUPPORT is not defined (i.e., half support is present)

Candidate Keywords representing method dependencies:
- fromHLFto8, CMS_NO_HALF_SUPPORT, _cmsHalf2Float, _cmsQuickSaturateByte
- cmsUInt16Number, cmsUInt8Number, cmsFloat32Number, cmsFloat64Number
- Saturation, half-to-byte conversion, boundary behavior (0, 255)
- Memory operations via dst, src pointers
- Endianness considerations for 16-bit half in src

Step 2 - Generated Unit Test Suite (no GTest, single C++11 file)
The test harness below provides a minimal in-house testing framework with non-terminating assertions (via a counter) and a small set of test cases that exercise true branches of the conversion logic. It assumes CMS_NO_HALF_SUPPORT is not defined in the library build so the conversion path is executed. The tests call the focal function fromHLFto8 via C linkage.

Note: This test relies on the library providing the actual _cmsHalf2Float and _cmsQuickSaturateByte implementations and the fromHLFto8 symbol, and it targets little-endian platforms (usual for x86/x86-64). If your environment uses a different endianness or the library is built with CMS_NO_HALF_SUPPORT, some tests may need adjustment or may be skipped.

To keep the test self-contained, the code uses extern "C" to declare fromHLFto8 and performs direct byte-level checks on the returned value.

Code (test suite):

*/
#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Candidate keywords context (for documentation within tests)
//
// fromHLFto8: converts a 16-bit half (src) to an 8-bit byte (dst) by:
//   - converting half -> float
//   - multiplying by 255.0
//   - saturating to [0, 255]
//   - storing result into dst
// CMS_NO_HALF_SUPPORT: if defined, the function becomes a no-op (dst/src unused)


// Step 2: Declare the focal function with C linkage so it can be linked from the CMS library.
// We don't include a CMS header here to keep dependencies minimal for the test harness.
// The function signature matches the one in cmsalpha.c.
extern "C" void fromHLFto8(void* dst, const void* src);

// Simple in-house test framework (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        ++g_failures; \
        std::fprintf(stderr, "Test failure: %s | Actual: %u, Expected: %u\n", (msg), (unsigned)(actual), (unsigned)(expected)); \
    } \
} while(0)

static void test_fromHLFto8_zero()
{
    // src = 0x0000 represents 0.0 in half-precision
    uint16_t src = 0x0000;
    uint8_t dst = 0; // destination initialized to non-zero to detect writes (though we expect a write)
    fromHLFto8(&dst, &src);
    EXPECT_EQ(dst, (uint8_t)0, "fromHLFto8 should convert 0.0 half to 0");
}

static void test_fromHLFto8_one()
{
    // src = 0x3C00 represents 1.0 in half-precision
    uint16_t src = 0x3C00;
    uint8_t dst = 0;
    fromHLFto8(&dst, &src);
    EXPECT_EQ(dst, (uint8_t)255, "fromHLFto8 should convert 1.0 half to 255");
}

static void test_fromHLFto8_minus_one()
{
    // src = 0xBC00 represents -1.0 in half-precision
    uint16_t src = 0xBC00;
    uint8_t dst = 0;
    fromHLFto8(&dst, &src);
    // Negative value times 255 saturates to 0 for unsigned byte
    EXPECT_EQ(dst, (uint8_t)0, "fromHLFto8 should clamp negative half to 0");
}

static void test_fromHLFto8_two()
{
    // src = 0x4000 represents 2.0 in half-precision
    uint16_t src = 0x4000;
    uint8_t dst = 0;
    fromHLFto8(&dst, &src);
    // 2.0 * 255 = 510 -> saturate to 255
    EXPECT_EQ(dst, (uint8_t)255, "fromHLFto8 should saturate values >= ~1.0 to 255");
}

static void run_all_tests()
{
    test_fromHLFto8_zero();
    test_fromHLFto8_one();
    test_fromHLFto8_minus_one();
    test_fromHLFto8_two();
}

int main()
{
    // Run the tests and report summary
    run_all_tests();

    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("Total failures: %d\n", g_failures);
        return 1;
    }
}
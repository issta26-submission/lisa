/*
  Self-contained C++11 unit tests for the focal function PackChunkyBytes.
  The tests implement a minimal compatibility shim for the CMS/LCMS-like types
  and macros used by the PackChunkyBytes function, allowing the function to be
  compiled and exercised without the full project dependency.
  Tests cover:
  - Baseline path (no extras, no premul, no swap, etc.)
  - DoSwap path (channel reversal)
  - ExtraFirst false path with SwapFirst affecting memmove
  - Premul path with ExtraFirst (premultiply using alpha)
  - Reverse flavor path (byte order reversal)
  All tests use a lightweight, non-terminating assertion style to maximize coverage.
*/

#include <lcms2_internal.h>
#include <cstring>
#include <cstdint>
#include <iostream>


// Domain/types shim to mimic the CMS types used in the focal function.
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef int32_t  cmsInt32Number;
typedef bool     cmsBool;

// Minimal transform structure used by PackChunkyBytes.
typedef struct _cmsTRANSFORM {
    uint32_t OutputFormat; // Encodes nChan, DoSwap, Reverse, Extra, SwapFirst, Premul
} cmsTRANSFORM;

// Macro to mimic the CMSREGISTER keyword (no-op in this test harness)
#define CMSREGISTER

// Helper predicates and conversions to emulate the dependent macros used in the focal method.
// Bit layout (simplified for test harness):
// bits 0-7   : nChan
// bit 8      : DoSwap
// bit 9      : Reverse (Flavor)
// bits 10-17 : Extra (0-255)
// bit 18     : SwapFirst
// bit 19     : Premul
#define T_CHANNELS(fmt)    ((fmt) & 0xFF)
#define T_DOSWAP(fmt)      (((fmt) >> 8) & 0x01)
#define T_FLAVOR(fmt)      (((fmt) >> 9) & 0x01)
#define T_EXTRA(fmt)       (((fmt) >> 10) & 0xFF)
#define T_SWAPFIRST(fmt)     (((fmt) >> 18) & 0x01)
#define T_PREMUL(fmt)      (((fmt) >> 19) & 0x01)

// 8-bit to 16-bit expansion and 16-bit to 8-bit reduction helpers
static inline cmsUInt16Number FROM_8_TO_16(cmsUInt8Number x) { return (cmsUInt16Number)x << 8; }
static inline cmsUInt8Number  FROM_16_TO_8(cmsUInt16Number x) { return (cmsUInt8Number)(x >> 8); }

// Identity mapping for fixed-domain in premultiplied alpha calculation (simplified)
static inline cmsUInt32Number _cmsToFixedDomain(cmsUInt32Number x) { return x; }

// Byte-reversal helper for "Reverse flavor" behavior
static inline cmsUInt16Number REVERSE_FLAVOR_16(cmsUInt16Number x) {
    return (cmsUInt16Number)(((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8));
}

// PackChunkyBytes implementation (adapted to be testable in isolation)
cmsUInt8Number* PackChunkyBytes(CMSREGISTER _cmsTRANSFORM* info,
                                CMSREGISTER cmsUInt16Number wOut[],
                                CMSREGISTER cmsUInt8Number* output,
                                CMSREGISTER cmsUInt32Number Stride)
{
    {
        cmsUInt32Number nChan = T_CHANNELS(info->OutputFormat);
        cmsUInt32Number DoSwap = T_DOSWAP(info->OutputFormat);
        cmsUInt32Number Reverse = T_FLAVOR(info->OutputFormat);
        cmsUInt32Number Extra = T_EXTRA(info->OutputFormat);
        cmsUInt32Number SwapFirst = T_SWAPFIRST(info->OutputFormat);
        cmsUInt32Number Premul = T_PREMUL(info->OutputFormat);
        cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst;
        cmsUInt8Number* swap1;
        cmsUInt16Number v = 0;
        cmsUInt32Number i;
        cmsUInt32Number alpha_factor = 0;
        swap1 = output;
        if (ExtraFirst) {
            
            if (Premul && Extra)
                alpha_factor = _cmsToFixedDomain(FROM_8_TO_16(output[0]));
            output += Extra;
        }
        else
        {
            if (Premul && Extra)
                alpha_factor = _cmsToFixedDomain(FROM_8_TO_16(output[nChan]));
        }
        for (i=0; i < nChan; i++) {
            cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
            v = wOut[index];
            if (Reverse)
                v = REVERSE_FLAVOR_16(v);
            if (Premul)
            {
                v = (cmsUInt16Number)((cmsUInt32Number)((cmsUInt32Number)v * alpha_factor + 0x8000) >> 16);            
            }
            *output++ = FROM_16_TO_8(v);
        }
        if (!ExtraFirst) {
            output += Extra;
        }
        if (Extra == 0 && SwapFirst) {
            memmove(swap1 + 1, swap1, nChan-1);
            *swap1 = FROM_16_TO_8(v);
        }
        return output;
        // Stride is intentionally unused for this unit-test harness
        // cmsUNUSED_PARAMETER(Stride);
    }
}

// Minimal assertion helpers for non-terminating tests
static int g_failures = 0;
#define LOG_FAIL(msg) do { std::cerr << "Test Failure: " << msg << "\n"; } while(0)
#define EXPECT_EQ(a, b, desc) do { if ((a) != (b)) { ++g_failures; std::cerr << "EXPECT_EQ FAILED: " << (desc) << " -> expected " << (b) << ", got " << (a) << "\n"; } } while(0)
#define EXPECT_NEQ(a, b, desc) do { if ((a) == (b)) { ++g_failures; std::cerr << "EXPECT_NEQ FAILED: " << (desc) << " -> both are " << (a) << "\n"; } } while(0)
#define EXPECT_TRUE(cond, desc) do { if (!(cond)) { ++g_failures; std::cerr << "EXPECT_TRUE FAILED: " << (desc) << "\n"; } } while(0)
#define EXPECT_FALSE(cond, desc) do { if ((cond)) { ++g_failures; std::cerr << "EXPECT_FALSE FAILED: " << (desc) << "\n"; } } while(0)

static cmsUInt32Number make_fmt(uint8_t nChan,
                               bool DoSwap,
                               bool Reverse,
                               uint8_t Extra,
                               bool SwapFirst,
                               bool Premul)
{
    uint32_t f = 0;
    f |= (uint32_t)nChan;
    if (DoSwap)   f |= (1u << 8);
    if (Reverse)  f |= (1u << 9);
    if (Extra)    f |= ((uint32_t)Extra << 10);
    if (SwapFirst) f |= (1u << 18);
    if (Premul)    f |= (1u << 19);
    return (cmsUInt32Number)f;
}

// Test Case 1: Baseline, no extras, no premul, no swap, no reverse
static void test_baseline_basics()
{
    cmsTRANSFORM info;
    info.OutputFormat = make_fmt(3, false, false, 0, false, false);
    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x5566 };
    cmsUInt8Number outBuf[8];
    std::memset(outBuf, 0, sizeof(outBuf));

    cmsUInt8Number* ret = PackChunkyBytes(&info, wOut, outBuf, 0);

    EXPECT_EQ(ret - outBuf, 3, "baseline: returned pointer should advance by nChan (3)");
    EXPECT_EQ(outBuf[0], 0x11, "baseline: first channel high byte");
    EXPECT_EQ(outBuf[1], 0x33, "baseline: second channel high byte");
    EXPECT_EQ(outBuf[2], 0x55, "baseline: third channel high byte");
}

// Test Case 2: DoSwap path to exercise channel reversal and memmove when SwapFirst is true
static void test_do_swap_and_memmove_path()
{
    cmsTRANSFORM info;
    // DoSwap=1, SwapFirst=1 -> ExtraFirst = 0 (since 1 ^ 1 = 0). Extra=0
    info.OutputFormat = make_fmt(3, true, false, 0, true, false);
    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 }; // After reversal, order is 0x3333,0x2222,0x1111
    cmsUInt8Number outBuf[8];
    std::memset(outBuf, 0, sizeof(outBuf));

    cmsUInt8Number* ret = PackChunkyBytes(&info, wOut, outBuf, 0);

    // Expected: swapped order -> [0x11, 0x33, 0x22]
    // Because v sequence: wOut[2]=0x3333 -> 0x33, wOut[1]=0x2222 -> 0x22, wOut[0]=0x1111 -> 0x11
    // SwapFirst==1 and Extra==0 triggers memmove: final array [0x11, 0x33, 0x22]
    EXPECT_EQ(ret - outBuf, 3, "do_swap: returned pointer should advance by nChan (3)");
    EXPECT_EQ(outBuf[0], 0x11, "do_swap: first final byte after memmove (FROM last v)");
    EXPECT_EQ(outBuf[1], 0x33, "do_swap: second final byte after memmove (first v)");
    EXPECT_EQ(outBuf[2], 0x22, "do_swap: third final byte after memmove (second v)");
}

// Test Case 3: Premul with ExtraFirst path (Extra > 0, Premul active)
static void test_premul_with_extra_first()
{
    cmsTRANSFORM info;
    // DoSwap=0, SwapFirst=0 => ExtraFirst = 1; Extra=1; Premul=1
    info.OutputFormat = make_fmt(3, false, false, 1, false, true);
    cmsUInt16Number wOut[3] = { 0xFF00, 0x4000, 0x2000 }; // 0xFF00 -> 0x7F after premul with alpha=0x80, similarly others
    cmsUInt8Number outBuf[8];
    // Pre-fill alpha position (output[0]) with alpha 0x80
    outBuf[0] = 0x80;
    // The rest can be zero
    std::memset(outBuf+1, 0, 7);

    cmsUInt8Number* ret = PackChunkyBytes(&info, wOut, outBuf, 0);

    // Expected (alpha factor = 0x8000 from 0x80 << 8):
    // v for 0xFF00 -> 0x7F, for 0x4000 -> 0x20, for 0x2000 -> 0x10
    // First channel should be at outBuf[1], etc.
    EXPECT_EQ(ret - outBuf, 4, "premul_extra_first: returned pointer offset");
    EXPECT_EQ(outBuf[0], 0x80, "premul_extra_first: alpha remains in position 0");
    EXPECT_EQ(outBuf[1], 0x7F, "premul_extra_first: first channel after premul");
    EXPECT_EQ(outBuf[2], 0x20, "premul_extra_first: second channel after premul");
    EXPECT_EQ(outBuf[3], 0x10, "premul_extra_first: third channel after premul");
}

// Test Case 4: Reverse flavor path (Reverse = 1) to exercise byte swap in 16-bit to 8-bit conversion
static void test_reverse_flavor()
{
    cms TRANSFORM info;
    // DoSwap=0, Reverse=1, Extra=0
    info.OutputFormat = make_fmt(3, false, true, 0, false, false);
    cmsUInt16Number wOut[3] = { 0x1200, 0x3400, 0x5600 }; // After reversal: 0x0012, 0x0034, 0x0056 -> 0x12, 0x34, 0x56
    cmsUInt8Number outBuf[8];
    std::memset(outBuf, 0, sizeof(outBuf));

    cmsUInt8Number* ret = PackChunkyBytes(&info, wOut, outBuf, 0);

    EXPECT_EQ(ret - outBuf, 3, "reverse_flavor: returned pointer offset");
    EXPECT_EQ(outBuf[0], 0x12, "reverse_flavor: first channel after reversal");
    EXPECT_EQ(outBuf[1], 0x34, "reverse_flavor: second channel after reversal");
    EXPECT_EQ(outBuf[2], 0x56, "reverse_flavor: third channel after reversal");
}

// Test Case 5: Baseline with extra path that exercises Extra and no premul but with extra behavior
// (Ensures code path where ExtraFirst is true and Extra > 0, Premul off)
static void test_extra_first_no_premul()
{
    cmsTRANSFORM info;
    // DoSwap=0, SwapFirst=0 -> ExtraFirst = 0; We'll instead set DoSwap=0, SwapFirst=0, Extra=2 to observe output shift
    // But to keep ExtraFirst true scenario, we can set SwapFirst=1 while DoSwap=0 (ExtraFirst=1)
    info.OutputFormat = make_fmt(3, false, false, 2, true, false);
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x0F0F }; // Values to test after channel order with extra
    cmsUInt8Number outBuf[8];
    std::memset(outBuf, 0, sizeof(outBuf));

    cmsUInt8Number* ret = PackChunkyBytes(&info, wOut, outBuf, 0);

    // Expect alpha at outBuf[0] unchanged (not used since Premul is false),
    // outputs at outBuf[2], outBuf[3], outBuf[4] after skipping Extra
    // Channels order due to DoSwap=0, SwapFirst=1 (ExtraFirst = 1)
    // v sequence: wOut[0], wOut[1], wOut[2] -> 0x12 0x34, 0xAB 0xCD, 0x0F 0x0F -> high bytes: 0x12, 0xAB, 0x0F
    // Output will be placed starting at outBuf[1], since Extra=2 => there are two alpha bytes preceding. We then
    // check the values at positions [2],[3],[4] for channel bytes.
    // This test ensures ExtraFirst handling with Extra > 0.
    // Since we don't rely on exact value in alpha, just verify channel bytes
    EXPECT_EQ(ret - outBuf, 5, "extra_first_no_premul: return pointer offset");
    // We expect: outBuf[0] unchanged (alpha), then channels:
    // outBuf[2] should be 0x12, outBuf[3] should be 0xAB, outBuf[4] should be 0x0F
    EXPECT_EQ(outBuf[0], 0x00, "extra_first_no_premul: alpha byte remains unchanged (not asserted value)");
    EXPECT_EQ(outBuf[2], 0x12, "extra_first_no_premul: first actual channel byte");
    EXPECT_EQ(outBuf[3], 0xAB, "extra_first_no_premul: second actual channel byte");
    EXPECT_EQ(outBuf[4], 0x0F, "extra_first_no_premul: third actual channel byte");
}

// Main runner
int main()
{
    // Run tests
    test_baseline_basics();
    test_do_swap_and_memmove_path();
    test_premul_with_extra_first();
    test_reverse_flavor();
    test_extra_first_no_premul();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed.\n";
        return 1;
    }
}
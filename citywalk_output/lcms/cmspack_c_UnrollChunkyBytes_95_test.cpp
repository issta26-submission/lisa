// Test suite for UnrollChunkyBytes (cmspack.c) - standalone C++11 test harness
// This test is designed to be compiled alongside the focal library (cmspack.c) in a
// project that uses the CMSLCMS types. It provides a lightweight test harness in
// place of Google Test per instructions.

// Step 1: Candidate Keywords (conceptual summary of the focal method's behavior)
// - nChan: number of color channels from information format
// - DoSwap / SwapFirst: permutation of channel order
// - Reverse: optional byte/word flavor reversal per channel
// - Extra / ExtraFirst: handle extra bytes trailing/leading the channel data
// - Premul / alpha_factor: premultiplied alpha adjustment and clamping
// - accum: input/output working memory (8-bit per element), advanced by Extra and nChan
// - wIn: destination buffer for unrolled chunky bytes (as 16-bit per channel)
// - Swap of first channel when Extra == 0 and SwapFirst is set
// The tests below exercise true/false branches for these conditions with a minimal
// mock environment that aligns to the function's interface.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Lightweight test framework (no GTest). Non-terminating assertions with summary.
static int g_test_failures = 0;
#define TEST_PRINT(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        TEST_PRINT("EXPECT_EQ FAILED: %s | %lld != %lld", (msg), (long long)(a), (long long)(b)); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if ((void*)(a) != (void*)(b)) { \
        TEST_PRINT("EXPECT_PTR_EQ FAILED: %s | %p != %p", (msg), (void*)(a), (void*)(b)); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        TEST_PRINT("EXPECT_TRUE FAILED: %s", (msg)); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPAND(x) x

// Step 2: Prepare a minimal external interface compatible with the focal function.
// We declare the UnrollChunkyBytes with C linkage to avoid C++ name mangling issues.
// The real function signature uses a _cmsTRANSFORM*; we declare void* here to keep
// the test self-contained while matching the runtime memory layout sufficiently for testing.
// Note: This approach assumes the first member of the actual _cmsTRANSFORM matches
// our test struct layout (InputFormat). This is a pragmatic approach for isolated unit tests.
extern "C" {
    // Prototype kept generic to minimize coupling with private library headers.
    cmsUInt8Number* UnrollChunkyBytes(void* info,
                                      cmsUInt16Number wIn[],
                                      cmsUInt8Number* accum,
                                      cmsUInt32Number Stride);
}

// Typedefs for CMS-like types (subset for test harness)
typedef uint8_t cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Minimal _cmsTRANSFORM layout we mirror for the test's info param.
// We only care about the InputFormat field as used by the focal method.
struct _cmsTRANSFORM {
    uint32_t InputFormat;
};

// Expose a tiny helper to construct InputFormat bitfields in a readable way.
static inline uint32_t MAKE_INPUT_FMT(uint8_t nChan,
                                    uint8_t DoSwap,
                                    uint8_t Reverse,
                                    uint8_t SwapFirst,
                                    uint8_t Extra,
                                    uint8_t Premul)
{
    // Layout assumption for test (matches test macros below):
    // bits 0-7:   nChan
    // bit 8:      DoSwap
    // bit 9:      Reverse
    // bit 10:     SwapFirst
    // bits 11-13:  Extra (3 bits)
    // bit 14:      Premul
    // rest unused
    uint32_t fmt = 0;
    fmt |= (nChan & 0xFF);
    fmt |= ((DoSwap & 0x1)     << 8);
    fmt |= ((Reverse & 0x1)    << 9);
    fmt |= ((SwapFirst & 0x1)  << 10);
    fmt |= ((Extra & 0x7)      << 11);
    fmt |= ((Premul & 0x1)     << 14);
    return fmt;
}

// Minimal bit-extractors corresponding to the focal macros (mocked for test)
#define T_CHANNELS(fmt)     ((uint32_t)((fmt) & 0xFF))
#define T_DOSWAP(fmt)        ((uint32_t)(((fmt) >> 8) & 0x1))
#define T_FLAVOR(fmt)         ((uint32_t)(((fmt) >> 9) & 0x1))
#define T_SWAPFIRST(fmt)      ((uint32_t)(((fmt) >> 10) & 0x1))
#define T_EXTRA(fmt)          ((uint32_t)(((fmt) >> 11) & 0x7))
#define T_PREMUL(fmt)          ((uint32_t)(((fmt) >> 14) & 0x1))

// Helpers to mirror CMS-like fixed-point behavior used by the focal method.
// We'll keep simple: alpha_factor is a 32-bit integer; no scaling in this test setup.
// FROM_8_TO_16: 8-bit to 16-bit expansion by simple zero-extend
#define FROM_8_TO_16(x) (uint32_t)((uint16_t)(x))

static inline uint16_t REVERSE_FLAVOR_16(uint16_t v) {
    // Byte-reverse 16-bit
    return (uint16_t)((v << 8) | (v >> 8));
}

// _cmsToFixedDomain: keep identity in tests
static inline uint32_t _cmsToFixedDomain(uint32_t x) { return x; }

// Step 3: Implement tests
// We rely on very small, deterministic inputs to verify key branches.

static void test_basic_passthrough_basic_case_ok()
{
    // Case 1: Basic path with 2 channels, no extra, no premul, no swap/reverse
    // Expected: wIn[0] = accum[0], wIn[1] = accum[1]
    _cmsTRANSFORM info;
    info.InputFormat = MAKE_INPUT_FMT(2, 0, 0, 0, 0, 0); // nChan=2, all flags false

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number accum_buf[8];
    accum_buf[0] = 0x12;  // accum[0]
    accum_buf[1] = 0x34;  // accum[1]
    // UnrollChunkyBytes will read accum[0..nChan-1] and write to wIn
    cmsUInt8Number* ret = UnrollChunkyBytes(&info, wIn, accum_buf, 0);

    // Validate results
    EXPECT_EQ((uint16_t)wIn[0], (uint16_t)0x0012, "Basic passthrough: wIn[0] should equal first input byte");
    EXPECT_EQ((uint16_t)wIn[1], (uint16_t)0x0034, "Basic passthrough: wIn[1] should equal second input byte");
    // Returned accum pointer should be unchanged (Extra=0, ExtraFirst=false)
    EXPECT_PTR_EQ(ret, accum_buf, "Returned accum pointer unchanged for basic case");
}

static void test_extra_first_branch()
{
    // Case 2: ExtraFirst true (DoSwap ^ SwapFirst == 1) and Extra > 0
    // nChan=2, DoSwap=1, SwapFirst=0, Extra=1, Premul=0
    _cmsTRANSFORM info;
    info.InputFormat = MAKE_INPUT_FMT(2, 1, 0, 0, 1, 0);

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number accum_buf[8];
    // Prepare: [ignored, accum[0], accum[1], ...]
    accum_buf[0] = 0xAA; // to be skipped due to ExtraFirst
    accum_buf[1] = 0x01;
    accum_buf[2] = 0x02;

    cmsUInt8Number* ret = UnrollChunkyBytes(&info, wIn, accum_buf, 0);

    // Due to DoSwap=1, index = (nChan - i - 1)
    // i=0 -> index=1, v=accum[1]=0x01 => wIn[1]=1
    // i=1 -> index=0, v=accum[2]=0x02 => wIn[0]=2
    EXPECT_EQ((uint16_t)wIn[0], (uint16_t)0x0002, "ExtraFirst: wIn[0] should be Accum[2]");
    EXPECT_EQ((uint16_t)wIn[1], (uint16_t)0x0001, "ExtraFirst: wIn[1] should be Accum[1]");

    // Return pointer should be advanced by Extra (1) and by nChan (2): total +3
    EXPECT_PTR_EQ(ret, accum_buf + 3, "Returned accum pointer after ExtraFirst case");
}

static void test_swap_first_when_extra_zero()
{
    // Case 3: Extra==0 and SwapFirst==1
    // nChan=2, DoSwap=0, SwapFirst=1, Extra=0
    _cmsTRANSFORM info;
    info.InputFormat = MAKE_INPUT_FMT(2, 0, 0, 1, 0, 0);

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number accum_buf[8];
    accum_buf[0] = 0x11;
    accum_buf[1] = 0x22;

    cmsUInt8Number* ret = UnrollChunkyBytes(&info, wIn, accum_buf, 0);

    // Normal read before swap: wIn becomes [0x11, 0x22]
    // SwapFirst with nChan==2 should rotate to [0x22, 0x11]
    EXPECT_EQ((uint16_t)wIn[0], (uint16_t)0x0022, "SwapFirst: wIn[0] after swap should be second input");
    EXPECT_EQ((uint16_t)wIn[1], (uint16_t)0x0011, "SwapFirst: wIn[1] after swap should be first input");

    // Since Extra==0 and SwapFirst==1, accum is not advanced by Extra
    EXPECT_PTR_EQ(ret, accum_buf, "Returned accum pointer unchanged for SwapFirst case with Extra=0");
}

static void test_premul_alpha_factor_sat()
{
    // Case 4: Premul enabled with alpha_factor > 0, Extra=1, nChan=2
    // DoSwap=0, SwapFirst=0, Extra=1, Premul=1
    _cmsTRANSFORM info;
    info.InputFormat = MAKE_INPUT_FMT(2, 0, 0, 0, 1, 1);

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number accum_buf[8];
    // accum: [a, b, alpha], alpha factor is from accum[2]
    accum_buf[0] = 0x40; // a
    accum_buf[1] = 0x80; // b
    accum_buf[2] = 0x01; // alpha (non-zero -> alpha_factor > 0)
    // We expect v after premul to saturate to 0xffff for both channels since (v<<16)/1 >> clamp
    cmsUInt8Number* ret = UnrollChunkyBytes(&info, wIn, accum_buf, 0);

    EXPECT_EQ((uint16_t)wIn[0], (uint16_t)0xffff, "Premul: wIn[0] should saturate to 0xFFFF");
    EXPECT_EQ((uint16_t)wIn[1], (uint16_t)0xffff, "Premul: wIn[1] should saturate to 0xFFFF");

    // With Extra=1 and ExtraFirst false, accum should advance by Extra after loop
    EXPECT_PTR_EQ(ret, accum_buf + 3, "Returned accum pointer advanced by Extra + nChan for Premul case");
}

int main()
{
    // Run tests
    TEST_PRINT("Starting UnrollChunkyBytes unit tests (mock environment)");
    test_basic_passthrough_basic_case_ok();
    test_extra_first_branch();
    test_swap_first_when_extra_zero();
    test_premul_alpha_factor_sat();

    if (g_test_failures == 0) {
        TEST_PRINT("All UnrollChunkyBytes tests PASSED.");
        return 0;
    } else {
        TEST_PRINT("UnrollChunkyBytes tests completed with failures: %d", g_test_failures);
        return 1;
    }
}
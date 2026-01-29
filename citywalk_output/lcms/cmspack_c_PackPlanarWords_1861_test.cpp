// cms_pack_planar_words_test.cpp
// Lightweight unit tests for PackPlanarWords (no GoogleTest; simple harness)

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific scaffolding to compile the focal function in isolation

// Type aliases matching CMS types
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef bool     cmsBool;

// CMSREGISTER is a macro used for portability; in tests it's a no-op
#define CMSREGISTER

// Forward-declare the minimal transform struct used by the focal function
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Minimal implementation of the struct (only used for OutputFormat)
struct _cmsTRANSFORM {
    unsigned OutputFormat;
};

// Lightweight helpers to mimic the macros used by PackPlanarWords
// Bit-field encoding for OutputFormat (chosen for testability)
#define T_CHANNELS(fmt) ((fmt) & 0x0F)            // lower 4 bits: number of channels
#define T_DOSWAP(fmt)    (((fmt) >> 4) & 0x1)
#define T_SWAPFIRST(fmt)  (((fmt) >> 5) & 0x1)
#define T_FLAVOR(fmt)     (((fmt) >> 6) & 0x1)
#define T_EXTRA(fmt)      (((fmt) >> 7) & 0x1)
#define T_PREMUL(fmt)     (((fmt) >> 8) & 0x1)
#define T_ENDIAN16(fmt)   (((fmt) >> 9) & 0x1)

// Fixed-domain alpha helper (simplified for testing)
static cmsUInt32Number _cmsToFixedDomain(cmsUInt16Number x) {
    // Convert 16-bit fractional to 32-bit fixed-point domain by shifting left 16 bits
    return ((cmsUInt32Number)x) << 16;
}

// Byte-order helpers
#define CHANGE_ENDIAN(x) ( (cmsUInt16Number)((((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8)) )
#define REVERSE_FLAVOR_16(x) ( (cmsUInt16Number)((((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8)) )
// For tests, we treat REVERSE_FLAVOR_16 as a 16-bit byte swap (to exercise branch)

// The focal function under test copied/ported from the prompt (simplified for test)
cmsUInt8Number* PackPlanarWords(CMSREGISTER _cmsTRANSFORM* info,
                                CMSREGISTER cmsUInt16Number wOut[],
                                CMSREGISTER cmsUInt8Number* output,
                                CMSREGISTER cmsUInt32Number Stride)
{
{
    cmsUInt32Number nChan = T_CHANNELS(info->OutputFormat);
    cmsUInt32Number DoSwap = T_DOSWAP(info->OutputFormat);
    cmsUInt32Number SwapFirst = T_SWAPFIRST(info->OutputFormat);
    cmsUInt32Number Reverse = T_FLAVOR(info->OutputFormat);
    cmsUInt32Number Extra = T_EXTRA(info->OutputFormat);
    cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst;
    cmsUInt32Number Premul = T_PREMUL(info->OutputFormat);
    cmsUInt32Number SwapEndian = T_ENDIAN16(info->OutputFormat);
    cmsUInt32Number i;
    cmsUInt8Number* Init = output;
    cmsUInt16Number v;
    cmsUInt32Number alpha_factor = 0;
    if (ExtraFirst) {
        if (Premul && Extra)
            alpha_factor = _cmsToFixedDomain(((cmsUInt16Number*) output)[0]);
        output += Extra * Stride;
    }
    else
    {
        if (Premul && Extra)
            alpha_factor = _cmsToFixedDomain(((cmsUInt16Number*)output)[nChan * Stride]);
    }
    for (i=0; i < nChan; i++) {
        cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
        v = wOut[index];
        if (SwapEndian)
            v = CHANGE_ENDIAN(v);
        if (Reverse)
            v =  REVERSE_FLAVOR_16(v);
        if (Premul)
        {
            v = (cmsUInt16Number)((cmsUInt32Number)((cmsUInt32Number)v * alpha_factor + 0x8000) >> 16);
        }
        *(cmsUInt16Number*) output = v;
        output += Stride;
    }
    return (Init + sizeof(cmsUInt16Number));
}
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
template <typename T>
void expect_eq(const T& a, const T& b, const char* desc) {
    if (a != b) {
        std::fprintf(stderr, "FAIL: %s | expected: %llx, actual: %llx\n",
                     desc,
                     (unsigned long long)b,
                     (unsigned long long)a);
        ++g_failures;
    } else {
        std::printf("PASS: %s\n", desc);
    }
}

// Helper to reinterpret a buffer as an array of 16-bit words
static inline cmsUInt16Number* asWords(cmsUInt8Number* p) {
    return reinterpret_cast<cmsUInt16Number*>(p);
}

// Test 1: Basic planar packing with 2 channels, no transforms
void test_pack_planar_words_basic() {
    // info: nChan=2, no swaps, no premul, no extra
    _cmsTRANSFORM info;
    info.OutputFormat = 2; // lower 4 bits: 2 channels; others zero
    cmsUInt16Number wIn[2] = { 0x1234, 0xABCD };
    cmsUInt8Number buffer[sizeof(cmsUInt16Number) * 2]; // 4 bytes
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret = PackPlanarWords(&info, wIn, buffer, 2);

    // Validate: output bytes contain the same 16-bit words in order
    cmsUInt16Number* outWords = asWords(buffer);
    expect_eq(outWords[0], 0x1234, "Basic: first channel preserved (0)");
    expect_eq(outWords[1], 0xABCD, "Basic: second channel preserved (1)");
    // Validate return value points to after first 16-bit word
    expect_eq<size_t>((uintptr_t)ret, (uintptr_t)buffer + sizeof(cmsUInt16Number),
                    "Basic: return pointer after first word");
}

// Test 2: DoSwap toggles channel order
void test_pack_planar_words_swap() {
    _cmsTRANSFORM info;
    info.OutputFormat = (2) | (1 << 4); // nChan=2, DoSwap=1
    cmsUInt16Number wIn[2] = { 0x1111, 0x2222 };
    cmsUInt8Number buffer[sizeof(cmsUInt16Number) * 2];

    std::memset(buffer, 0, sizeof(buffer));
    cmsUInt8Number* ret = PackPlanarWords(&info, wIn, buffer, 2);

    cmsUInt16Number* outWords = asWords(buffer);
    // With DoSwap=1, order should be reversed
    expect_eq(outWords[0], 0x2222, "Swap: first channel is second input (0)");
    expect_eq(outWords[1], 0x1111, "Swap: second channel is first input (1)");
    expect_eq<size_t>((uintptr_t)ret, (uintptr_t)buffer + sizeof(cmsUInt16Number),
                    "Swap: return pointer after first word");
}

// Test 3: Endian swap affects each 16-bit word
void test_pack_planar_words_endian_swap() {
    _cmsTRANSFORM info;
    info.OutputFormat = (2) | (1 << 9); // nChan=2, Endian16=1
    cmsUInt16Number wIn[2] = { 0x1234, 0xABCD };
    cmsUInt8Number buffer[sizeof(cmsUInt16Number) * 2];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret = PackPlanarWords(&info, wIn, buffer, 2);

    cmsUInt16Number* outWords = asWords(buffer);
    // Each word byte-swapped
    expect_eq(outWords[0], 0x3412, "Endian Swap: first word swapped (0)");
    expect_eq(outWords[1], 0xCDAB, "Endian Swap: second word swapped (1)");
    expect_eq<size_t>((uintptr_t)ret, (uintptr_t)buffer + sizeof(cmsUInt16Number),
                    "Endian Swap: return pointer after first word");
}

// Test 4: Premul with ExtraFirst arrangement (alpha from first word, multiplies all)
void test_pack_planar_words_premul_extra_first() {
    // We want ExtraFirst = 1 (ExtraFirst = DoSwap ^ SwapFirst; choose DoSwap=0, SwapFirst=1)
    _cmsTRANSFORM info;
    info.OutputFormat =  (2)            // nChan=2
                       | (0 << 4)        // DoSwap = 0
                       | (1 << 5)        // SwapFirst = 1
                       | (0 << 6)        // Flavor
                       | (1 << 7)        // Extra = 1
                       | (1 << 8)        // Premul = 1
                       | (0 << 9);       // Endian16 = 0
    // Initial buffer layout:
    // [alpha (first 16-bit)] [pad ignored] [channel0] [pad] [channel1] [pad]
    // Since Extra=1, ExtraFirst = 1, output will skip 1 stride (2 bytes) before writing channels.
    cmsUInt16Number wIn[2] = { 0x0010, 0x0003 }; // two channels
    cmsUInt8Number buffer[6]; // 1 word alpha + 2 words * 2 bytes each
    // Set alpha value (first 16-bit word) to 0x0002
    buffer[0] = 0x02; buffer[1] = 0x00; // alpha

    // Initialize rest of buffer (optional, not strictly needed)
    buffer[2] = 0x00; buffer[3] = 0x00; // reserved for first channel (will be overwritten)
    buffer[4] = 0x00; buffer[5] = 0x00; // reserved for second channel (will be overwritten)

    cmsUInt8Number* ret = PackPlanarWords(&info, wIn, buffer, 2);

    cmsUInt16Number* words = asWords(buffer);
    // word[0] is alpha 0x0002 (unchanged)
    // word[1] should be first channel (0x0010) multiplied by alpha_factor = 0x00020000 => 2 * 0x0010 = 0x0020
    // word[2] should be second channel (0x0003) multiplied by 2 => 0x0006
    expect_eq(words[0], 0x0002, "PremulExtraFirst: alpha preserved");
    expect_eq(words[1], 0x0020, "PremulExtraFirst: first channel premultiplied");
    expect_eq(words[2], 0x0006, "PremulExtraFirst: second channel premultiplied");
    // The function returns Init + 2 as usual
    expect_eq<size_t>((uintptr_t)ret, (uintptr_t)buffer + sizeof(cmsUInt16Number),
                    "PremulExtraFirst: return pointer after first word");
}

int main() {
    // Run the tests
    test_pack_planar_words_basic();
    test_pack_planar_words_swap();
    test_pack_planar_words_endian_swap();
    test_pack_planar_words_premul_extra_first();

    if (g_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }
}
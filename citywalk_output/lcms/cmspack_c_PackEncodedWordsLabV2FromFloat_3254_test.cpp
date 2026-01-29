// Lightweight C++11 unit test harness for the focal function PackEncodedWordsLabV2FromFloat
// This test suite provides minimal stubs and a small test framework (no external libs).
// It validates both planar and chunky (non-planar) paths, as well as the extra word scenario.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Domain-specific type aliases (simplified for test harness)
typedef unsigned char  cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int   cmsUInt32Number;
typedef float          cmsFloat32Number;
typedef double         cmsFloat64Number;

// Minimal representation of CMS transform info used by focal method
typedef struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat; // flags controlling planar vs chunky and extra word behavior
} cmsTRANSFORM;

// Lightweight Lab structure used by conversion function
typedef struct {
    cmsFloat64Number L;
    cmsFloat64Number a;
    cmsFloat64Number b;
} cmsCIELab;

// Lightweight 16-bit Lab encoding function for tests
// This is a mock / substitute for cmsFloat2LabEncodedV2 used by PackEncodedWordsLabV2FromFloat.
// We encode Lab values into 3 cmsUInt16Number words deterministically for testing.
// L -> direct, a -> shifted into 0..255 via +128, b -> shifted similarly.
static void cmsFloat2LabEncodedV2(cmsUInt16Number wlab[3], const cmsCIELab* Lab)
{
    // Basic, deterministic encoding suitable for testing
    cmsFloat64Number L = Lab->L;
    cmsFloat64Number a = Lab->a;
    cmsFloat64Number b = Lab->b;

    // Round to nearest integer and clamp to reasonable 16-bit range
    long Lround = llround(L);
    if (Lround < 0) Lround = 0;
    if (Lround > 65535) Lround = 65535;

    long arnd = llround(a + 128.0);
    if (arnd < 0) arnd = 0;
    if (arnd > 65535) arnd = 65535;

    long brnd = llround(b + 128.0);
    if (brnd < 0) brnd = 0;
    if (brnd > 65535) brnd = 65535;

    wlab[0] = static_cast<cmsUInt16Number>(Lround);
    wlab[1] = static_cast<cmsUInt16Number>(arnd);
    wlab[2] = static_cast<cmsUInt16Number>(brnd);
}

// Lightweight macros matching the behavior described in the focal method
#define T_PLANAR(fmt) (((fmt) & 0x1) != 0)
#define PixelSize(fmt) (2) // 16-bit per channel in test
#define T_EXTRA(fmt) (((fmt) & 0x2) ? 1 : 0)

// The focal function under test (rewritten here with simplified dependencies)
typedef cmsUInt8Number* _PackEncodedWordsLabV2FromFloat_Param; // just for readability

cmsUInt8Number* PackEncodedWordsLabV2FromFloat(_cmsTRANSFORM* Info,
                                             cmsFloat32Number wOut[],
                                             cmsUInt8Number* output,
                                             cmsUInt32Number Stride)
{
    cmsCIELab Lab;
    cmsUInt16Number wlab[3];
    Lab.L = (cmsFloat64Number)(wOut[0] * 100.0);
    Lab.a = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0);
    Lab.b = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0);
    cmsFloat2LabEncodedV2(wlab, &Lab);

    if (T_PLANAR(Info -> OutputFormat)) {
        Stride /= PixelSize(Info->OutputFormat);

        ((cmsUInt16Number*) output)[0]        = wlab[0];
        ((cmsUInt16Number*) output)[Stride]   = wlab[1];
        ((cmsUInt16Number*) output)[Stride*2] = wlab[2];
        return output + sizeof(cmsUInt16Number);
    }
    else {
         ((cmsUInt16Number*) output)[0] = wlab[0];
         ((cmsUInt16Number*) output)[1] = wlab[1];
         ((cmsUInt16Number*) output)[2] = wlab[2];
        return output + (3 + T_EXTRA(Info ->OutputFormat)) * sizeof(cmsUInt16Number);
    }
}

// --- Lightweight test harness (non-terminating assertions) ---

static std::vector<std::string> g_failures;
static void recordFailure(const std::string& msg) {
    g_failures.push_back(msg);
}
#define CHECK(cond, msg) do { if(!(cond)) recordFailure(msg); } while(0)
#define CHECK_EQ_U16(a, b, msg) do { if((a) != (b)) { char buf[256]; std::snprintf(buf, sizeof(buf), "%s (expected %u, got %u)", msg, static_cast<unsigned>(b), static_cast<unsigned>(a)); recordFailure(buf); } } while(0)
#define CHECK_PTR_EQ(a, b, msg) do { if((void*)(a) != (void*)(b)) { char buf[256]; std::snprintf(buf, sizeof(buf), "%s (expected ptr %p, got %p)", msg, (void*)(b), (void*)(a)); recordFailure(buf); } } while(0)

// Helper to print summary
static void printSummary(const char* title) {
    std::cout << title << ": " << g_failures.size() << " failure(s) detected.\n";
    for (size_t i = 0; i < g_failures.size(); ++i) {
        std::cout << "  - " << g_failures[i] << "\n";
    }
}

// Test 1: Non-planar path (planar flag = 0), no extra words (T_EXTRA = 0)
static void test_PackEncodedWordsLabV2FromFloat_NonPlanar_Basic() {
    // Prepare info to simulate chunky (non-planar) packing
    cmsTRANSFORM Info;
    Info.OutputFormat = 0x0; // Bit 0 = 0 (non-planar), Bit 1 = 0 (no extra)

    // Input values correspond to L=50, a=-128, b=-128
    cmsFloat32Number wOut[3] = { 0.5f, 0.0f, 0.0f };

    // Allocate output buffer large enough
    cmsUInt8Number outputBuf[64];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    cmsUInt8Number* ret = PackEncodedWordsLabV2FromFloat(&Info, wOut, outputBuf, 12);

    // Expected encoded values via cmsFloat2LabEncodedV2 mock
    cmsUInt16Number wlab[3];
    cmsCIELab Lab;
    Lab.L = (cmsFloat64Number)(wOut[0] * 100.0);
    Lab.a = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0);
    Lab.b = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0);
    cmsFloat2LabEncodedV2(wlab, &Lab);
    // wlab should be: [L≈50, a+128≈0, b+128≈0]

    cmsUInt16Number* outW = reinterpret_cast<cmsUInt16Number*>(outputBuf);
    CHECK_EQ_U16(outW[0], wlab[0], "Non-planar: first word mismatch");
    CHECK_EQ_U16(outW[1], wlab[1], "Non-planar: second word mismatch");
    CHECK_EQ_U16(outW[2], wlab[2], "Non-planar: third word mismatch");
    // Return pointer should be after first two bytes (size of cmsUInt16Number)
    CHECK_PTR_EQ(ret, outputBuf + sizeof(cmsUInt16Number), "Non-planar: return pointer mismatch");
}

// Test 2: Planar path (planar flag = 1), with Stride adjustment
static void test_PackEncodedWordsLabV2FromFloat_Planar_Basic() {
    cmsTRANSFORM Info;
    Info.OutputFormat = 0x1; // Bit 0 = 1 (planar), Bit 1 = 0 (no extra)

    // Use same Lab as Test 1 for determinism: L≈50, a≈0, b≈0
    cmsFloat32Number wOut[3] = { 0.5f, 0.0f, 0.0f };

    cmsUInt8Number outputBuf[64];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    // Stride chosen to test planar unrolling: Stride = 6, PixelSize(Format)=2 -> Stride/PixelSize = 3
    cmsUInt8Number* ret = PackEncodedWordsLabV2FromFloat(&Info, wOut, outputBuf, 6);

    cmsUInt16Number* wOut16 = reinterpret_cast<cmsUInt16Number*>(outputBuf);
    cmsUInt16Number wlab[3];
    cmsCIELab Lab;
    Lab.L = (cmsFloat64Number)(wOut[0] * 100.0);
    Lab.a = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0);
    Lab.b = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0);
    cmsFloat2LabEncodedV2(wlab, &Lab);

    // In planar mode, indices 0, Stride, Stride*2 should hold wlab[0], wlab[1], wlab[2]
    // Stride after division is 3
    // Expect: outputBuf[0..1] = wlab[0], outputBuf[3*2..3*2+1] = wlab[1]? Actually using cmsUInt16Number indexing:
    // (cmsUInt16Number*)outputBuf -> w[0], w[Stride], w[Stride*2] with Stride = 3
    CHECK_EQ_U16(wOut16[0], wlab[0], "Planar: first word mismatch");
    CHECK_EQ_U16(wOut16[3], wlab[1], "Planar: second word mismatch at index 3");
    CHECK_EQ_U16(wOut16[6], wlab[2], "Planar: third word mismatch at index 6");

    // Return pointer should be after one cmsUInt16Number (2 bytes)
    CHECK_PTR_EQ(ret, outputBuf + sizeof(cmsUInt16Number), "Planar: return pointer mismatch");
}

// Test 3: Non-planar path with extra words (T_EXTRA = 1)
static void test_PackEncodedWordsLabV2FromFloat_NonPlanar_WithExtra() {
    cmsTRANSFORM Info;
    Info.OutputFormat = 0x2; // Bit 0 = 0 (non-planar), Bit 1 = 1 (extra word)

    cmsFloat32Number wOut[3] = { 0.0f, 0.0f, 0.0f }; // L=0, a= -128, b= -128
    cmsUInt8Number outputBuf[64];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    cmsUInt8Number* ret = PackEncodedWordsLabV2FromFloat(&Info, wOut, outputBuf, 12);

    cmsUInt16Number wlab[3];
    cmsCIELab Lab;
    Lab.L = (cmsFloat64Number)(wOut[0] * 100.0);
    Lab.a = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0);
    Lab.b = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0);
    cmsFloat2LabEncodedV2(wlab, &Lab);

    cmsUInt16Number* outW = reinterpret_cast<cmsUInt16Number*>(outputBuf);
    CHECK_EQ_U16(outW[0], wlab[0], "Non-planar-extra: first word mismatch");
    CHECK_EQ_U16(outW[1], wlab[1], "Non-planar-extra: second word mismatch");
    CHECK_EQ_U16(outW[2], wlab[2], "Non-planar-extra: third word mismatch");
    // Extra word means total 4 words are written (since 3 + 1) times 2 bytes each.
    // Return should be output + 4*sizeof(uint16)
    CHECK_PTR_EQ(ret, outputBuf + 4 * sizeof(cmsUInt16Number), "Non-planar-extra: return pointer mismatch");
}

// Main function to run tests
int main() {
    // Run tests
    test_PackEncodedWordsLabV2FromFloat_NonPlanar_Basic();
    test_PackEncodedWordsLabV2FromFloat_Planar_Basic();
    test_PackEncodedWordsLabV2FromFloat_NonPlanar_WithExtra();

    // Print test results
    if (g_failures.empty()) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        printSummary("PackEncodedWordsLabV2FromFloatTestSuite");
        return 1;
    }
}
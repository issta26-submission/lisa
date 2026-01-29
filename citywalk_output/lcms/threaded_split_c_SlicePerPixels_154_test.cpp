// Minimal C++11 test-suite for the focal method SlicePerPixels
// This test-suite is self-contained (no GTest) and uses light-weight
// assertions that never terminate the test run to maximize coverage.

#include <cstring>
#include <threaded_internal.h>
#include <cstdio>
#include <cstdint>


// Domain-specific lightweight CMS-like types (subset for unit tests)
using cmsInt32Number   = int;
using cmsUInt32Number  = unsigned int;
using cmsUInt8Number   = unsigned char;
using cmsHTRANSFORM     = void*;

// Forward declaration of the focal method under test (re-implemented here for self-containment)
void SlicePerPixels(const struct _cmsWorkSlice* master, cmsInt32Number nslices,
                    cmsInt32Number PixelsPerSlice, struct _cmsWorkSlice slices[]);

// Minimal stand-ins for dependencies used by SlicePerPixels
static cmsUInt32Number g_spacing_turn = 0; // used to simulate alternating spacing

static cmsUInt32Number PixelSpacing(cmsUInt32Number /*format*/)
{
    // Alternate spacing between 1 and 2 to exercise different memory strides
    cmsUInt32Number v = (g_spacing_turn == 0) ? 1 : 2;
    g_spacing_turn = 1 - g_spacing_turn;
    return v;
}

static cmsUInt32Number cmsGetTransformInputFormat(void* /*transform*/) { return 0; }
static cmsUInt32Number cmsGetTransformOutputFormat(void* /*transform*/) { return 0; }

static cmsUInt32Number minimum(cmsUInt32Number a, cmsUInt32Number b)
{
    return (a < b) ? a : b;
}

struct _cmsWorkSlice {
    const cmsUInt8Number* InputBuffer;
    cmsUInt8Number* OutputBuffer;
    cmsInt32Number PixelsPerLine;
    void* CMMcargo;
};

// Implement the focal function (self-contained copy for testing)
void SlicePerPixels(const _cmsWorkSlice* master, cmsInt32Number nslices,
                    cmsInt32Number PixelsPerSlice, _cmsWorkSlice slices[])
{
{
    cmsInt32Number i;
    cmsInt32Number TotalPixels = master->PixelsPerLine; // As this works on one line only
    cmsUInt32Number PixelSpacingIn = PixelSpacing(cmsGetTransformInputFormat((cmsHTRANSFORM)master->CMMcargo));
    cmsUInt32Number PixelSpacingOut = PixelSpacing(cmsGetTransformOutputFormat((cmsHTRANSFORM)master->CMMcargo));
    for (i = 0; i < nslices; i++) {
        const cmsUInt8Number* PtrInput = master->InputBuffer;
        cmsUInt8Number* PtrOutput = master->OutputBuffer;
        cmsInt32Number pixels = minimum(PixelsPerSlice, TotalPixels);
        memcpy(&slices[i], master, sizeof(_cmsWorkSlice));
        slices[i].InputBuffer = PtrInput + i * PixelsPerSlice * PixelSpacingIn;
        slices[i].OutputBuffer = PtrOutput + i * PixelsPerSlice * PixelSpacingOut;
        slices[i].PixelsPerLine = pixels;
        TotalPixels -= pixels;
    }
    // Add left pixels because rounding
    if (nslices > 0) slices[nslices - 1].PixelsPerLine += TotalPixels;
}
}

// Lightweight test harness with non-terminating assertions
static int g_failures = 0;

static void log_fail(const char* msg, const char* file, int line)
{
    std::fprintf(stderr, "TEST_FAIL: %s (at %s:%d)\n", msg, file, line);
}

#define CHECK_TRUE(cond, msg) do { if(!(cond)) { log_fail(msg, __FILE__, __LINE__); ++g_failures; } } while(false)
#define CHECK_EQ(expected, actual, msg) do { \
    if (!((expected) == (actual))) { \
        std::fprintf(stderr, "TEST_FAIL: %s (expected %lld, actual %lld) at %s:%d\n", \
                     msg, (long long)(expected), (long long)(actual), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(false)
#define CHECK_PTR_EQ(expected, actual, msg) do { \
    if ((void*)(expected) != (void*)(actual)) { \
        std::fprintf(stderr, "TEST_FAIL: %s (expected %p, actual %p) at %s:%d\n", \
                     msg, (void*)(expected), (void*)(actual), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(false)

static void test_basic_partition_spacing_in_out()
{
    // Basic scenario: 1 line of 10 pixels, 3 slices, 4 pixels per slice
    // Space: In = 1, Out = 2 (via PixelSpacing function toggling)
    const cmsInt32Number masterPixelsPerLine = 10;
    const cmsInt32Number nslices = 3;
    const cmsInt32Number PixelsPerSlice = 4;

    unsigned char inputBuffer[64];
    unsigned char outputBuffer[256];

    for (int i = 0; i < 64; ++i) inputBuffer[i] = static_cast<unsigned char>(i);

    _cmsWorkSlice master;
    master.InputBuffer = inputBuffer;
    master.OutputBuffer = outputBuffer;
    master.PixelsPerLine = masterPixelsPerLine;
    master.CMMcargo = nullptr;

    _cmsWorkSlice slices[3] = {}; // will hold results

    // Reset spacing toggle for deterministic behavior within this test
    g_spacing_turn = 0;

    SlicePerPixels(&master, nslices, PixelsPerSlice, slices);

    // Verify per-slice metadata
    // Slice 0
    CHECK_EQ(4, slices[0].PixelsPerLine, "Slice 0 PixelsPerLine should be 4");
    CHECK_PTR_EQ(inputBuffer + 0, slices[0].InputBuffer, "Slice 0 InputBuffer offset 0");
    CHECK_PTR_EQ(outputBuffer + 0, slices[0].OutputBuffer, "Slice 0 OutputBuffer offset 0");

    // Slice 1
    // InputBuffer should be at +4
    CHECK_PTR_EQ(inputBuffer + 4, slices[1].InputBuffer, "Slice 1 InputBuffer offset 4");
    // OutputBuffer should be at +4 * 2 = +8
    CHECK_PTR_EQ(outputBuffer + 8, slices[1].OutputBuffer, "Slice 1 OutputBuffer offset 8");
    CHECK_EQ(4, slices[1].PixelsPerLine, "Slice 1 PixelsPerLine should be 4");

    // Slice 2
    // InputBuffer should be at +8
    CHECK_PTR_EQ(inputBuffer + 8, slices[2].InputBuffer, "Slice 2 InputBuffer offset 8");
    // OutputBuffer should be at +8 * 2 = +16
    CHECK_PTR_EQ(outputBuffer + 16, slices[2].OutputBuffer, "Slice 2 OutputBuffer offset 16");
    CHECK_EQ(2, slices[2].PixelsPerLine, "Slice 2 PixelsPerLine should be 2");

    // Ensure no rounding leftovers are added (TotalPixels becomes 0)
    // Last slice's PixelsPerLine should be 2 (unchanged by the rounding path)
    CHECK_EQ(2, slices[2].PixelsPerLine, "Leftover handling (no extra pixels) check");
}

static void test_partition_with_leftover_and_large_slice()
{
    // Scenario: 2 slices, PixelsPerSlice larger than total pixels
    // master line = 3 pixels, PixelsPerSlice = 5
    const cmsInt32Number masterPixelsPerLine = 3;
    const cmsInt32Number nslices = 2;
    const cmsInt32Number PixelsPerSlice = 5;

    unsigned char inputBuffer[32];
    unsigned char outputBuffer[64];

    for (int i = 0; i < 32; ++i) inputBuffer[i] = static_cast<unsigned char>(i + 1);

    _cmsWorkSlice master;
    master.InputBuffer = inputBuffer;
    master.OutputBuffer = outputBuffer;
    master.PixelsPerLine = masterPixelsPerLine;
    master.CMMcargo = nullptr;

    _cmsWorkSlice slices[2] = {};

    // Reset spacing toggle for deterministic behavior
    g_spacing_turn = 0;

    SlicePerPixels(&master, nslices, PixelsPerSlice, slices);

    // Expected:
    // Slice 0: InputBuffer at +0, OutputBuffer at +0, PixelsPerLine = 3
    CHECK_PTR_EQ(inputBuffer + 0, slices[0].InputBuffer, "Slice 0 InputBuffer offset 0");
    CHECK_PTR_EQ(outputBuffer + 0, slices[0].OutputBuffer, "Slice 0 OutputBuffer offset 0");
    CHECK_EQ(3, slices[0].PixelsPerLine, "Slice 0 PixelsPerLine = 3");

    // Slice 1: InputBuffer at +5, OutputBuffer at +10 (since Out spacing = 2)
    CHECK_PTR_EQ(inputBuffer + 5, slices[1].InputBuffer, "Slice 1 InputBuffer offset 5");
    CHECK_PTR_EQ(outputBuffer + 10, slices[1].OutputBuffer, "Slice 1 OutputBuffer offset 10");
    // PixelsPerLine for slice 1 should be 0 (no remaining pixels)
    CHECK_EQ(0, slices[1].PixelsPerLine, "Slice 1 PixelsPerLine = 0");
}

static void test_zero_slices_branch()
{
    // Ensure the function handles nslices = 0 without errors (branch coverage)
    const cmsInt32Number masterPixelsPerLine = 8;
    const cmsInt32Number nslices = 0;
    const cmsInt32Number PixelsPerSlice = 4;

    unsigned char inputBuffer[16];
    unsigned char outputBuffer[16];
    for (int i = 0; i < 16; ++i) inputBuffer[i] = static_cast<unsigned char>(i);

    _cmsWorkSlice master;
    master.InputBuffer = inputBuffer;
    master.OutputBuffer = outputBuffer;
    master.PixelsPerLine = masterPixelsPerLine;
    master.CMMcargo = nullptr;

    _cmsWorkSlice slices[1] = {};

    // Reset spacing toggle
    g_spacing_turn = 0;

    SlicePerPixels(&master, nslices, PixelsPerSlice, slices);

    // Expect no changes to slices since nslices == 0
    // We still perform a minimal sanity check: no crash occurred and values remain default
    CHECK_TRUE(slices[0].PixelsPerLine == 0, "Slice[0] PixelsPerLine remains default when nslices==0");
}

int main()
{
    g_failures = 0;

    test_basic_partition_spacing_in_out();
    test_partition_with_leftover_and_large_slice();
    test_zero_slices_branch();

    if (g_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("SOME TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}
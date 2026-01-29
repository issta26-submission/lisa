// VisualPng_DisplayImage_UnitTest.cpp
// A lightweight C++11 test suite for the DisplayImage function from VisualPng.c
// No GoogleTest/GMock; uses a small custom EXPECT-style framework.
// The tests cover basic allocation, header correctness, and pixel placement
// with and without stretching.

#include <cstdint>
#include <string.h>
#include <windows.h>
#include <zlib.h>
#include <resource.h>
#include <stdlib.h>
#include <cstdio>
#include <pngfile.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Forward declare Windows types to be used with the focal method.
// The actual project VisualPng.c is expected to be compiled together with this test.

extern "C" {
    // Focal method under test
    // BOOL DisplayImage (HWND hwnd, BYTE **ppDib,
    //        BYTE **ppDiData, int cxWinSize, int cyWinSize,
    //        BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
    //        BOOL bStretched);
    BOOL DisplayImage (HWND hwnd, BYTE **ppDib,
        BYTE **ppDiData, int cxWinSize, int cyWinSize,
        BYTE *pbImage, int cxImgSize, int cyImgSize, int cImgChannels,
        BOOL bStretched);
}

// Lightweight test harness
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::printf("EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_NOT_NULL(p) \
    EXPECT_TRUE((p) != nullptr)

#define EXPECT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            std::printf("EXPECT_EQ failed: %s == %s (%lld vs %lld) at %s:%d\n", #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_BYTES_EQ(ptr, val, len) \
    do { \
        bool _eq = true; \
        const unsigned char* _p = (const unsigned char*)(ptr); \
        for (size_t _i = 0; _i < (len); ++_i) { \
            if (_p[_i] != (unsigned char)(val)) { _eq = false; break; } \
        } \
        if (!_eq) { \
            std::printf("EXPECT_BYTES_EQ failed at %s:%d (byte %zu != %u)\n", __FILE__, __LINE__, 0, (unsigned)(val)); \
            ++g_failures; \
        } \
    } while (0)

// Helper: search for a specific B,G,R triple in the DIB data region.
// Returns true if found; also outputs coordinates if requested (via outX/outY).
static bool findTripleInDIB(BYTE* pDiData, int cxWinSize, int cyWinSize, WORD wDIRowBytes,
                            uint8_t B, uint8_t G, uint8_t R,
                            int* outX = nullptr, int* outY = nullptr)
{
    for (int y = 0; y < cyWinSize; ++y) {
        BYTE* row = pDiData + y * wDIRowBytes;
        for (int x = 0; x < cxWinSize; ++x) {
            uint8_t b = row[x * 3 + 0];
            uint8_t g = row[x * 3 + 1];
            uint8_t r = row[x * 3 + 2];
            if (b == B && g == G && r == R) {
                if (outX) *outX = x;
                if (outY) *outY = y;
                return true;
            }
        }
    }
    return false;
}

// Test 1: Basic allocation path (pbImage == NULL), ensure header is correct
static void test_DisplayImage_basic_no_image()
{
    std::printf("Running test_DisplayImage_basic_no_image...\n");
    BYTE *pDib = NULL;
    BYTE *pDiData = NULL;

    // Small image, no content
    BOOL ok = DisplayImage(NULL, &pDib, &pDiData,
                           10, 10,          // window size
                           NULL, 0, 0, 0,     // no image
                           FALSE);

    EXPECT_TRUE(ok);
    if (!ok) {
        // Nothing more to test
        return;
    }

    EXPECT_NOT_NULL(pDib);
    EXPECT_NOT_NULL(pDiData);

    BITMAPINFOHEADER *pbmih = reinterpret_cast<BITMAPINFOHEADER*>(pDib);
    EXPECT_EQ(pbmih->biWidth, 10);
    EXPECT_EQ(pbmih->biHeight, -10);
    EXPECT_EQ(pbmih->biBitCount, 24);
    EXPECT_EQ(pbmih->biCompression, 0);

    // InitBitmap should fill the first pixel with gray value 127,127,127
    // so the first three bytes of pDiData should be 127
    EXPECT_EQ(static_cast<int>(pDiData[0]), 127);
    EXPECT_EQ(static_cast<int>(pDiData[1]), 127);
    EXPECT_EQ(static_cast<int>(pDiData[2]), 127);

    // Cleanup
    free(pDib);
    pDib = NULL;
    pDiData = NULL;

    std::printf("test_DisplayImage_basic_no_image completed.\n");
}

// Test 2: Non-stretched path with a small 2x2 RGB image. Verify B,G,R ordering in output.
static void test_DisplayImage_with_image_no_stretch()
{
    std::printf("Running test_DisplayImage_with_image_no_stretch...\n");

    // Prepare a 2x2 RGB image
    // Pixels in row-major order: (0,0) (1,0) / (0,1) (1,1)
    // Colors: (R,G,B) values
    // (0,0): 10,20,30
    // (1,0): 40,50,60
    // (0,1): 70,80,90
    // (1,1): 100,110,120
    unsigned char image[12] = {
        10,20,30,  40,50,60,
        70,80,90,  100,110,120
    };

    BYTE *pDib = NULL;
    BYTE *pDiData = NULL;

    BOOL ok = DisplayImage(NULL, &pDib, &pDiData,
                           10, 10,         // window size
                           image, 2, 2, 3,    // 2x2 RGB image
                           FALSE);            // not stretched

    EXPECT_TRUE(ok);
    if (!ok) return;

    EXPECT_NOT_NULL(pDib);
    EXPECT_NOT_NULL(pDiData);

    BITMAPINFOHEADER *pbmih = reinterpret_cast<BITMAPINFOHEADER*>(pDib);
    EXPECT_EQ(pbmih->biWidth, 10);
    EXPECT_EQ(pbmih->biHeight, -10);

    // Compute row width in DIB bytes as done in DisplayImage
    WORD wDIRowBytes = (WORD) ((3 * 10 + 3L) >> 2) << 2; // should be 32

    // Search for the first pixel's B,G,R bytes: [30, 20, 10]
    int foundX = -1, foundY = -1;
    bool found = findTripleInDIB(pDiData, 10, 10, wDIRowBytes, 30, 20, 10, &foundX, &foundY);
    EXPECT_TRUE(found);
    if (found) {
        // We can also verify that a different pixel exists for sanity (60,50,40) etc.
        bool found2 = findTripleInDIB(pDiData, 10, 10, wDIRowBytes, 60, 50, 40);
        EXPECT_TRUE(found2);
    }

    // Cleanup
    free(pDib);
    pDib = NULL;
    pDiData = NULL;

    std::printf("test_DisplayImage_with_image_no_stretch completed.\n");
}

// Test 3: Stretched path with a small 2x2 RGB image on a larger window.
// Verify that color of the first stretched pixel matches the first input pixel.
static void test_DisplayImage_with_image_stretched()
{
    std::printf("Running test_DisplayImage_with_image_stretched...\n");

    // Same 2x2 RGB image as Test 2
    unsigned char image[12] = {
        10,20,30,  40,50,60,
        70,80,90,  100,110,120
    };

    BYTE *pDib = NULL;
    BYTE *pDiData = NULL;

    // Large window to exercise stretching branch
    BOOL ok = DisplayImage(NULL, &pDib, &pDiData,
                           40, 40,           // window size
                           image, 2, 2, 3,     // 2x2 RGB image
                           TRUE);               // stretched

    EXPECT_TRUE(ok);
    if (!ok) return;

    EXPECT_NOT_NULL(pDib);
    EXPECT_NOT_NULL(pDiData);

    BITMAPINFOHEADER *pbmih = reinterpret_cast<BITMAPINFOHEADER*>(pDib);
    EXPECT_EQ(pbmih->biWidth, 40);
    EXPECT_EQ(pbmih->biHeight, -40);

    // Compute wDIRowBytes for cxWinSize=40
    WORD wDIRowBytes = (WORD) ((3 * 40 + 3L) >> 2) << 2; // should be 120

    // In stretched path, the first input pixel (0,0) should appear at the first
    // position of the stretched image, which starts at cyImgPos (MARGIN) and cxImgPos (MARGIN).
    // We don't know MARGIN value here, so we search for the first occurrence of
    // the first pixel's B,G,R triple: [30, 20, 10]
    int foundX = -1, foundY = -1;
    bool found = findTripleInDIB(pDiData, 40, 40, wDIRowBytes, 30, 20, 10, &foundX, &foundY);
    EXPECT_TRUE(found);
    if (found) {
        // Optional: ensure that the found location is within the expected bounds (not necessary but helpful)
        if (foundX < 0 || foundY < 0) {
            g_failures++;
            std::printf("Warning: Found location out of expected bounds.\n");
        }
    }

    // Cleanup
    free(pDib);
    pDib = NULL;
    pDiData = NULL;

    std::printf("test_DisplayImage_with_image_stretched completed.\n");
}

int main()
{
    std::printf("Starting DisplayImage unit tests for VisualPng DisplayImage...\n");

    test_DisplayImage_basic_no_image();
    test_DisplayImage_with_image_no_stretch();
    test_DisplayImage_with_image_stretched();

    if (g_failures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("Tests completed with %d failure(s).\n", g_failures);
        return 1;
    }
}
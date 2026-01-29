/*
Unit Test Suite for CmpImages (tifdiff.c)
- Implemented as a standalone test executable (no Google Test).
- Uses libtiff to create in-memory TIFFs on disk and to invoke the focal function:
  int CmpImages(TIFF* tiff1, TIFF* tiff2, TIFF* diff)
- Three test cases cover core dependent pathways:
  1) Grayscale (MINISBLACK) difference and diff file output
  2) RGB difference and diff file output
  3) CMYK (SEPARATED) difference and diff file output
- Each test writes small TIFFs, runs CmpImages, then validates the produced diff content
  to ensure deterministic, known outputs.
- Includes a tiny test harness with non-terminating assertions (print-only) to maximize
  coverage while keeping tests executable without GTest.
- The tests rely on the environment having libtiff available (link against -ltiff).

Notes:
- The test code declares the C linkage for CmpImages to ensure correct linking with tifdiff.c.
- The tests create and clean up temporary TIFF files in the working directory.
- Each unit test is annotated with comments describing which code branches and data paths are exercised.
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utils.h>
#include <cstdint>
#include <tiffio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal method under test (from tifdiff.c)
int CmpImages(TIFF* tiff1, TIFF* tiff2, TIFF* diff);
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion macros
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << " (line " << __LINE__ << ")\n"; \
        ++g_tests_failed; \
    } else { \
        /* pass, do nothing */ \
    } \
} while(0)

#define ASSERT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if((expected) != (actual)) { \
        std::cerr << "TEST_FAIL: " << (msg) << " - expected: " << (expected) \
                  << ", actual: " << (actual) << " (line " << __LINE__ << ")\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// Helpers to write small TIFFs using libtiff
static void writeGrayTIFF(const std::string& path, int width, int height, const uint8_t* data) {
    TIFF* tif = TIFFOpen(path.c_str(), "w8");
    if (!tif) {
        std::cerr << "Failed to create TIFF: " << path << std::endl;
        return;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    tsize_t linebytes = static_cast<tsize_t>(width * 1);
    for (int row = 0; row < height; ++row) {
        const uint8_t* rowptr = data + row * width;
        if (TIFFWriteScanline(tif, (void*)rowptr, row, 0) < 0) {
            std::cerr << "Failed to write scanline " << row << " for " << path << std::endl;
            break;
        }
    }
    TIFFClose(tif);
}

static void writeRGBTIFF(const std::string& path, int width, int height, const uint8_t* data) {
    TIFF* tif = TIFFOpen(path.c_str(), "w8");
    if (!tif) {
        std::cerr << "Failed to create TIFF: " << path << std::endl;
        return;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    for (int row = 0; row < height; ++row) {
        const uint8_t* rowptr = data + row * width * 3;
        if (TIFFWriteScanline(tif, (void*)rowptr, row, 0) < 0) {
            std::cerr << "Failed to write RGB scanline " << row << " for " << path << std::endl;
            break;
        }
    }
    TIFFClose(tif);
}

static void writeCMYKTIFF(const std::string& path, int width, int height, const uint8_t* data) {
    TIFF* tif = TIFFOpen(path.c_str(), "w8");
    if (!tif) {
        std::cerr << "Failed to create TIFF: " << path << std::endl;
        return;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED);
    for (int row = 0; row < height; ++row) {
        const uint8_t* rowptr = data + row * width * 4;
        if (TIFFWriteScanline(tif, (void*)rowptr, row, 0) < 0) {
            std::cerr << "Failed to write CMYK scanline " << row << " for " << path << std::endl;
            break;
        }
    }
    TIFFClose(tif);
}

// Read a single scanline (row 0) from an existing TIFF (diff output) into outbuf
static bool readDiffRow(const std::string& path, int width, uint8_t* outbuf) {
    TIFF* tif = TIFFOpen(path.c_str(), "r");
    if (!tif) {
        std::cerr << "Failed to open diff TIFF: " << path << std::endl;
        return false;
    }
    if (TIFFReadScanline(tif, outbuf, 0, 0) < 0) {
        std::cerr << "Failed to read diff scanline 0 from: " << path << std::endl;
        TIFFClose(tif);
        return false;
    }
    TIFFClose(tif);
    return true;
}

// Test 1: Grayscale MINISBLACK difference; verify diff values [190, 5] for two pixels
static void test_grayscale_diff() {
    const int W = 2;
    const int H = 1;
    uint8_t a1[W * H] = { 10, 20 };     // tiff1 grayscale pixels
    uint8_t a2[W * H] = { 200, 15 };    // tiff2 grayscale pixels differing per-pixel
    const std::string p1 = "test_gray1.tif";
    const std::string p2 = "test_gray2.tif";
    const std::string pdiff = "test_gray_diff.tif";

    writeGrayTIFF(p1, W, H, a1);
    writeGrayTIFF(p2, W, H, a2);

    TIFF* t1 = TIFFOpen(p1.c_str(), "r");
    TIFF* t2 = TIFFOpen(p2.c_str(), "r");
    TIFF* diff = TIFFOpen(pdiff.c_str(), "w8");
    ASSERT_TRUE(t1 != nullptr && t2 != nullptr && diff != nullptr, "Grayscale test: opening TIFFs");

    int rc = CmpImages(t1, t2, diff);

    TIFFClose(t1);
    TIFFClose(t2);
    TIFFClose(diff);

    // Expected return value is non-zero (success)
    ASSERT_EQ(1, rc, "Grayscale test: CmpImages should return 1 on success");

    // Read back diff to verify per-pixel diffs: [190, 5]
    uint8_t diffRow[W];
    bool ok = readDiffRow(pdiff, W, diffRow);
    ASSERT_TRUE(ok, "Grayscale test: reading diff row");
    ASSERT_EQ(190, diffRow[0], "Grayscale test: first diff pixel should be 190");
    ASSERT_EQ(5, diffRow[1], "Grayscale test: second diff pixel should be 5");

    // Cleanup
    std::remove(p1.c_str());
    std::remove(p2.c_str());
    std::remove(pdiff.c_str());
}

// Test 2: RGB difference; verify diff values [6, 6] for two pixels
static void test_rgb_diff() {
    const int W = 2;
    const int H = 1;
    // Pixel 1: tiff1(10,0,0)  vs tiff2(20,0,0) -> dR=10, dG=0, dB=0 => dE ~ 5.77 -> 6
    // Pixel 2: tiff1(0,10,0) vs tiff2(0,0,10) -> dR=0, dG=10, dB=10 -> dE ~ sqrt(100+100)/sqrt(3) ~ 14.14/1.732 ~ 8.16 -> 8
    uint8_t a1[W * 3] = { 10,0,0,  0,10,0 };
    uint8_t a2[W * 3] = { 20,0,0,  0,0,10 };
    // expected diff per pixel: [6, 8]
    const std::string p1 = "test_rgb1.tif";
    const std::string p2 = "test_rgb2.tif";
    const std::string pdiff = "test_rgb_diff.tif";

    writeRGBTIFF(p1, W, H, a1);
    writeRGBTIFF(p2, W, H, a2);

    TIFF* t1 = TIFFOpen(p1.c_str(), "r");
    TIFF* t2 = TIFFOpen(p2.c_str(), "r");
    TIFF* diff = TIFFOpen(pdiff.c_str(), "w8");
    ASSERT_TRUE(t1 != nullptr && t2 != nullptr && diff != nullptr, "RGB test: opening TIFFs");

    int rc = CmpImages(t1, t2, diff);

    TIFFClose(t1);
    TIFFClose(t2);
    TIFFClose(diff);

    // Validate rc
    ASSERT_EQ(1, rc, "RGB test: CmpImages should return 1 on success");

    // Read diff row: two bytes [6, 8]
    uint8_t diffRow[W];
    bool ok = readDiffRow(pdiff, W, diffRow);
    ASSERT_TRUE(ok, "RGB test: reading diff row");
    ASSERT_EQ(6, diffRow[0], "RGB test: first diff pixel should be 6");
    ASSERT_EQ(8, diffRow[1], "RGB test: second diff pixel should be 8");

    // Cleanup
    std::remove(p1.c_str());
    std::remove(p2.c_str());
    std::remove(pdiff.c_str());
}

// Test 3: CMYK SEPARATED difference; verify diff values [1, 2] for two pixels
static void test_cmyk_diff() {
    const int W = 2;
    const int H = 1;
    // Pixel 1: tiff1(0,0,0,0) vs tiff2(2,0,0,0) -> dC=2 => dE=2/2=1
    // Pixel 2: tiff1(0,0,0,0) vs tiff2(0,4,0,0) -> dM=4 => dE=4/2=2
    uint8_t a1[W * 4] = { 0,0,0,0,  0,0,0,0 };
    uint8_t a2[W * 4] = { 2,0,0,0,  0,4,0,0 };
    const std::string p1 = "test_cmyk1.tif";
    const std::string p2 = "test_cmyk2.tif";
    const std::string pdiff = "test_cmyk_diff.tif";

    writeCMYKTIFF(p1, W, H, a1);
    writeCMYKTIFF(p2, W, H, a2);

    TIFF* t1 = TIFFOpen(p1.c_str(), "r");
    TIFF* t2 = TIFFOpen(p2.c_str(), "r");
    TIFF* diff = TIFFOpen(pdiff.c_str(), "w8");
    ASSERT_TRUE(t1 != nullptr && t2 != nullptr && diff != nullptr, "CMYK test: opening TIFFs");

    int rc = CmpImages(t1, t2, diff);

    TIFFClose(t1);
    TIFFClose(t2);
    TIFFClose(diff);

    // Validate rc
    ASSERT_EQ(1, rc, "CMYK test: CmpImages should return 1 on success");

    // Read diff row: two bytes [1, 2] expected
    uint8_t diffRow[W];
    bool ok = readDiffRow(pdiff, W, diffRow);
    ASSERT_TRUE(ok, "CMYK test: reading diff row");
    ASSERT_EQ(1, diffRow[0], "CMYK test: first diff pixel should be 1");
    ASSERT_EQ(2, diffRow[1], "CMYK test: second diff pixel should be 2");

    // Cleanup
    std::remove(p1.c_str());
    std::remove(p2.c_str());
    std::remove(pdiff.c_str());
}

int main() {
    std::cout << "Starting CmpImages unit tests (no GTest):\n";

    test_grayscale_diff();
    test_rgb_diff();
    test_cmyk_diff();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
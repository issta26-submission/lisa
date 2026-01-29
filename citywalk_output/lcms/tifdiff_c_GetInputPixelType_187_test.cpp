// Test suite for GetInputPixelType (tiffdiff.c) using real libtiff
// Notes:
// - This test suite creates small TIFFs with controlled header fields using libtiff,
//   then calls GetInputPixelType on a read handle to verify the bitfield-derived
//   information matches expectations.
// - We rely on the project's macros (COLORSPACE_SH, PLANAR_SH, EXTRA_SH, CHANNELS_SH,
//   BYTES_SH, FLAVOR_SH) to build the expected value in a way consistent with the focal code.
// - No Google Test; a lightweight, non-terminating assertion approach is used.
// - The tests are designed to cover true/false branches of the focal method where feasible
//   without triggering FatalError paths (e.g., avoid bilevel or unsupported configurations).

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utils.h>
#include <cstdint>
#include <tiffio.h>



// The focal function signature is cmsUInt32Number GetInputPixelType(TIFF *Bank);
// We declare it here to call from test code. cmsUInt32Number is typically a 32-bit type.
// If the project provides a typedef, this should resolve to the same underlying type.
extern "C" uint32_t GetInputPixelType(TIFF *Bank);

// Include project macro definitions to build expected bitfield values.
// This header is expected to define COLORSPACE_SH, PLANAR_SH, EXTRA_SH,
// CHANNELS_SH, BYTES_SH, FLAVOR_SH, and pixel-type constants (PT_GRAY, PT_RGB, PT_YCbCr, PT_YUV, PT_Lab, etc.)

// Simple non-terminating assertion helper
static inline void expect_eq_uint32(const char* test_name, uint32_t expected, uint32_t actual) {
    if (expected != actual) {
        std::cerr << "[FAIL] " << test_name << " — expected: " << expected
                  << ", actual: " << actual << "\n";
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

// Create a temporary TIFF file with minimal required fields set.
// This function focuses on header fields needed by GetInputPixelType.
static bool create_test_tiff(const std::string& path,
                             uint16 Photometric,
                             uint16 bps,
                             uint16 spp,
                             uint16 PlanarConfig,
                             uint16 extra = 0,
                             uint16 compression = 0,
                             uint16 subx = 0,
                             uint16 suby = 0)
{
    TIFF* tif = TIFFOpen(path.c_str(), "w");
    if (!tif) {
        std::cerr << "Failed to create TIFF: " << path << "\n";
        return false;
    }

    // Minimal image metadata
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PlanarConfig);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, Photometric);

    if (extra > 0) {
        // Extrasamples count
        TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, extra);
    }

    if (compression != 0) {
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        // If YCbCr, we may provide YCbCr subsampling
        if (subx != 0 || suby != 0) {
            TIFFSetField(tif, TIFFTAG_YCBCRSUBSAMPLING, subx, suby);
        }
    }

    // Write a minimal strip to ensure the file is well-formed.
    // Some writers require a strip to be present; we provide a tiny buffer.
    tsize_t rowbytes = (tsize_t)((bps / 8) * spp);
    if (rowbytes == 0) rowbytes = 1; // safeguard
    char* buf = new char[(size_t)rowbytes];
    std::memset(buf, 0, (size_t)rowbytes);

    TIFFWriteScanline(tif, buf, 0, 0);
    delete[] buf;

    TIFFClose(tif);
    return true;
}

// Helper to run a test case:
// - Creates a TIFF with given header fields
// - Reopens read-only
// - Calls GetInputPixelType
// - Asserts against expected combined bitfield using project macros
static void run_case_gray8() {
    const std::string path = "/tmp/tiff_test_gray8.tif";

    // Gray, 8-bit, 1 sample per pixel, contig
    if (!create_test_tiff(path, PHOTOMETRIC_MINISBLACK, 8, 1, PLANARCONFIG_CONTIG, 0)) {
        std::cerr << "[SKIP] Gray8 TIFF creation failed\n";
        return;
    }

    TIFF* tifRead = TIFFOpen(path.c_str(), "r");
    if (!tifRead) {
        std::cerr << "[SKIP] Gray8 could not reopen TIFF\n";
        return;
    }

    uint32_t actual = GetInputPixelType(tifRead);

    // expected: Gray color space, 1 channel, 8-bit -> BYTES_SH(1)
    uint32_t expected = COLORSPACE_SH(PT_GRAY) | PLANAR_SH(0) | EXTRA_SH(0) | CHANNELS_SH(1) | BYTES_SH(1) | FLAVOR_SH(0);

    expect_eq_uint32("GetInputPixelType Gray8", expected, actual);

    TIFFClose(tifRead);
    std::remove(path.c_str());
}

static void run_case_rgb8() {
    const std::string path = "/tmp/tiff_test_rgb8.tif";

    // RGB, 8-bit, 3 samples per pixel, contig
    if (!create_test_tiff(path, PHOTOMETRIC_RGB, 8, 3, PLANARCONFIG_CONTIG, 0)) {
        std::cerr << "[SKIP] RGB8 TIFF creation failed\n";
        return;
    }

    TIFF* tifRead = TIFFOpen(path.c_str(), "r");
    if (!tifRead) {
        std::cerr << "[SKIP] RGB8 could not reopen TIFF\n";
        return;
    }

    uint32_t actual = GetInputPixelType(tifRead);

    // expected: RGB color space, 3 channels, 8-bit
    uint32_t expected = COLORSPACE_SH(PT_RGB) | PLANAR_SH(0) | EXTRA_SH(0) | CHANNELS_SH(3) | BYTES_SH(1) | FLAVOR_SH(0);

    expect_eq_uint32("GetInputPixelType RGB8", expected, actual);

    TIFFClose(tifRead);
    std::remove(path.c_str());
}

static void run_case_ycbcr_1_1() {
    const std::string path = "/tmp/tiff_test_ycbcr.tif";

    // YCbCr, 8-bit, 3 samples per pixel, contig, compression defined, subsampling 1x1
    if (!create_test_tiff(path, PHOTOMETRIC_YCBCR, 8, 3, PLANARCONFIG_CONTIG,
                          0,  COMPRESSION_JPEG /* typical for YCbCr */, 1, 1)) {
        std::cerr << "[SKIP] YCbCr TIFF creation failed\n";
        return;
    }

    TIFF* tifRead = TIFFOpen(path.c_str(), "r");
    if (!tifRead) {
        std::cerr << "[SKIP] YCbCr could not reopen TIFF\n";
        return;
    }

    uint32_t actual = GetInputPixelType(tifRead);

    // expected: YCbCr color space, 3 channels, 8-bit
    uint32_t expected = COLORSPACE_SH(PT_YCbCr) | PLANAR_SH(0) | EXTRA_SH(0) | CHANNELS_SH(3) | BYTES_SH(1) | FLAVOR_SH(0);

    expect_eq_uint32("GetInputPixelType YCbCr_1_1", expected, actual);

    TIFFClose(tifRead);
    std::remove(path.c_str());
}

static void run_case_lo_gluv8() {
    const std::string path = "/tmp/tiff_test_logluv8.tif";

    // LOGLUV: 8-bit input but code forces 16-bit for internal processing
    // Use Photometric_LOGLUV to trigger the LOGLUV branch
    if (!create_test_tiff(path, PHOTOMETRIC_LOGLUV, 8, 1, PLANARCONFIG_CONTIG, 0, 0, 0, 0)) {
        std::cerr << "[SKIP] LOGLUV TIFF creation failed\n";
        return;
    }

    TIFF* tifRead = TIFFOpen(path.c_str(), "r");
    if (!tifRead) {
        std::cerr << "[SKIP] LOGLUV could not reopen TIFF\n";
        return;
    }

    uint32_t actual = GetInputPixelType(tifRead);

    // expected: YUV color space (PT_YUV) due to LOGLUV path, 1 channel, 16-bit
    uint32_t expected = COLORSPACE_SH(PT_YUV) | PLANAR_SH(0) | EXTRA_SH(0) | CHANNELS_SH(1) | BYTES_SH(2) | FLAVOR_SH(0);

    expect_eq_uint32("GetInputPixelType LOGLUV8", expected, actual);

    TIFFClose(tifRead);
    std::remove(path.c_str());
}

// Main entry point to run all tests
int main() {
    std::cout << "Starting GetInputPixelType unit tests...\n";

    run_case_gray8();
    run_case_rgb8();
    run_case_ycbcr_1_1();
    run_case_lo_gluv8();

    std::cout << "GetInputPixelType tests completed.\n";
    return 0;
}
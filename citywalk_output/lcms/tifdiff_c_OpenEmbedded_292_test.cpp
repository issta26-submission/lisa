/*
Unit tests for OpenEmbedded in tifdiff.c
- Purpose: exercise both branches (with and without embedded ICC profile) of OpenEmbedded.
- Environment: C++11, no GoogleTest. Link against libtiff and lcms2 in your build.
- Approach:
  - Test 1: Create a minimal TIFF without an ICC profile and verify OpenEmbedded returns 0 and sets both output pointers to NULL.
  - Test 2: Create a minimal TIFF with an embedded ICC profile (using TIFFSetField for TIFFTAG_ICCPROFILE) and verify OpenEmbedded returns a non-zero format and both output pointers become non-NULL.
- Notes:
  - The tests rely on the real OpenEmbedded implementation; a real libtiff and lcms2 are expected to be available in the build environment.
  - The tests create temporary TIFF files on disk to simplify TIFF interactions.
  - This code is self-contained and does not depend on GTest; tests are executed from main().
  - We provide explanatory comments for each unit test as requested.
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utils.h>
#include <tiffio.h>



// Forward declarations for types used by OpenEmbedded from tifdiff.c (as seen in the focal code)
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;

// We declare the focal function; it uses C linkage
extern "C" cmsUInt32Number OpenEmbedded(TIFF* tiff, cmsHPROFILE* PtrProfile, cmsHTRANSFORM* PtrXform);

// Helper: create a minimal 1x1 TIFF image without ICC profile
static bool createMinimalTIFFNoICC(const std::string& path) {
    FILE* f = nullptr;
    // Use libtiff for proper TIFF file creation
    TIFF* tif = TIFFOpen(path.c_str(), "w");
    if (!tif) {
        std::cerr << "Failed to create TIFF file: " << path << std::endl;
        return false;
    }

    // Basic image metadata for a minimal 1x1 image
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samplesPerPixel = 1;
    uint16_t bitsPerSample = 8;
    uint16_t photo = PHOTOMETRIC_MINISWHITE; // arbitrary for 1x1
    uint16_t model = 0; // not used; placeholder
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitsPerSample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photo);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Write a single pixel
    unsigned char pixel = 0x7F; // arbitrary value
    if (TIFFWriteScanline(tif, &pixel, 0, 0) < 0) {
        std::cerr << "Failed to write TIFF scanline." << std::endl;
        TIFFClose(tif);
        return false;
    }

    // Close
    TIFFClose(tif);
    return true;
}

// Helper: create a minimal TIFF image with an embedded ICC profile
static bool createMinimalTIFFWithICC(const std::string& path, const unsigned char* embed, uint32_t embedLen) {
    TIFF* tif = TIFFOpen(path.c_str(), "w");
    if (!tif) {
        std::cerr << "Failed to create TIFF file: " << path << std::endl;
        return false;
    }

    // Basic image metadata for a minimal 1x1 image
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samplesPerPixel = 1;
    uint16_t bitsPerSample = 8;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitsPerSample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Attach ICC profile data
    // Note: The ICC profile data provided must live at least for the duration of OpenEmbedded call.
    // The library will copy the memory as needed; here we pass a pointer to embed data directly.
    if (embed && embedLen > 0) {
        // In libtiff, TIFFTAG_ICCPROFILE expects (uint32* length, uint8** data)
        TIFFSetField(tif, TIFFTAG_ICCPROFILE, &embedLen, (const cmsUInt8Number**)&embed);
    }

    // Write a single pixel
    unsigned char pixel = 0xAA;
    if (TIFFWriteScanline(tif, &pixel, 0, 0) < 0) {
        std::cerr << "Failed to write TIFF scanline with ICC." << std::endl;
        TIFFClose(tif);
        return false;
    }

    TIFFClose(tif);
    return true;
}

// Test 1: No embedded ICC profile -> OpenEmbedded should return 0 and both outputs NULL
static bool test_OpenEmbedded_NoICC() {
    std::cout << "Test 1: OpenEmbedded with no embedded ICC profile" << std::endl;

    const char* tmpPath = "test_no_icc.tif";
    // Ensure old file removed
    remove(tmpPath);

    if (!createMinimalTIFFNoICC(tmpPath)) {
        std::cerr << "Test 1 failed: could not create TIFF without ICC." << std::endl;
        return false;
    }

    // Open the TIFF using the real OpenEmbedded
    FILE* fcheck = fopen(tmpPath, "rb");
    if (!fcheck) {
        std::cerr << "Test 1 failed: cannot open produced TIFF." << std::endl;
        return false;
    }
    fclose(fcheck);

    TIFF* tiff = TIFFOpen(tmpPath, "r");
    if (!tiff) {
        std::cerr << "Test 1 failed: could not reopen TIFF file for OpenEmbedded." << std::endl;
        return false;
    }

    cmsHPROFILE ptrProfile = NULL;
    cmsHTRANSFORM ptrXform = NULL;
    cmsUInt32Number fmt = OpenEmbedded(tiff, &ptrProfile, &ptrXform);

    TIFFClose(tiff);
    remove(tmpPath);

    bool ok = (fmt == 0) && (ptrProfile == NULL) && (ptrXform == NULL);
    if (!ok) {
        std::cerr << "Test 1 failed: expected dwFormat==0 and NULL outputs; got dwFormat=" << fmt
                  << " ptrProfile=" << (void*)ptrProfile << " ptrXform=" << (void*)ptrXform << std::endl;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }
    return ok;
}

// Test 2: Embedded ICC profile -> OpenEmbedded should return non-zero format and non-NULL outputs
static bool test_OpenEmbedded_WithICC() {
    std::cout << "Test 2: OpenEmbedded with embedded ICC profile" << std::endl;

    const char* tmpPath = "test_with_icc.tif";
    remove(tmpPath);

    // Simple embedded ICC profile data (fake content, just for testing flow)
    const unsigned char iccDummy[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    uint32_t iccLen = sizeof(iccDummy);

    if (!createMinimalTIFFWithICC(tmpPath, iccDummy, iccLen)) {
        std::cerr << "Test 2 failed: could not create TIFF with ICC." << std::endl;
        return false;
    }

    FILE* fcheck = fopen(tmpPath, "rb");
    if (!fcheck) {
        std::cerr << "Test 2 failed: cannot open produced TIFF." << std::endl;
        return false;
    }
    fclose(fcheck);

    TIFF* tiff = TIFFOpen(tmpPath, "r");
    if (!tiff) {
        std::cerr << "Test 2 failed: could not reopen TIFF file for OpenEmbedded." << std::endl;
        remove(tmpPath);
        return false;
    }

    cmsHPROFILE ptrProfile = NULL;
    cmsHTRANSFORM ptrXform = NULL;
    cmsUInt32Number fmt = OpenEmbedded(tiff, &ptrProfile, &ptrXform);

    TIFFClose(tiff);
    remove(tmpPath);

    bool ok = (fmt != 0) && (ptrProfile != NULL) && (ptrXform != NULL);
    if (!ok) {
        std::cerr << "Test 2 failed: expected dwFormat!=0 and non-NULL outputs; got dwFormat=" << fmt
                  << " ptrProfile=" << (void*)ptrProfile << " ptrXform=" << (void*)ptrXform << std::endl;
        // Best effort: try to report more info
    } else {
        std::cout << "Test 2 passed." << std::endl;
        // Note: We should free resources if necessary, but we rely on the consumer to manage the LittleCMS objects.
        // If desired, insert cmsCloseProfile(ptrProfile) and cmsDeleteTransform(ptrXform) when available.
    }
    return ok;
}

int main() {
    // Step 3: Execute tests with explanatory comments
    bool all_ok = true;

    // Test 1: verify false-branch behavior (no embedded ICC)
    if (!test_OpenEmbedded_NoICC()) {
        all_ok = false;
    }

    // Test 2: verify true-branch behavior (embedded ICC present)
    if (!test_OpenEmbedded_WithICC()) {
        all_ok = false;
    }

    if (all_ok) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
}
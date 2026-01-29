// Test suite for PngLoadImage in PngFile.c
// This test suite is designed for a C/C++11 environment on Windows-like
// platforms where libpng is available. It does not rely on GTest.
// Each test prints its result and the main function aggregates the results.

#include <commdlg.h>
#include <windows.h>
#include <cstring>
#include <zlib.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <pngfile.h>
#include <iostream>
#include <stdio.h>
#include <cexcept.h>
#include <png.h>


// Include the header that declares PngLoadImage and related functions.
// The project provides pngfile.h via the <FOCAL_CLASS_DEP> section.
extern "C" {
}

// Helper: simple test harness result reporting
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define LOG_TEST_RESULT(test_name, passed) do { \
    ++g_tests_run; \
    if (passed) { \
        ++g_tests_passed; \
        std::cout << "[PASS] " << test_name << "\n"; \
    } else { \
        std::cout << "[FAIL] " << test_name << "\n"; \
    } \
} while (0)

// Utility: write binary data to a file
static bool writeBinaryFile(const char* path, const unsigned char* data, size_t len)
{
    FILE* f = fopen(path, "wb");
    if (!f) return false;
    size_t written = fwrite(data, 1, len, f);
    fclose(f);
    return written == len;
}

// Test 1: PngLoadImage should return FALSE and not modify image data when pstrFileName == NULL
static bool test_PngLoadImage_NullFileName()
{
    unsigned char* pData = NULL;
    int width = 0, height = 0, channels = 0;
    png_color bkgColor = {0, 0, 0};

    BOOL result = PngLoadImage(NULL, &pData, &width, &height, &channels, &bkgColor);

    // Expect FALSE and pData remains NULL
    if (result != FALSE) return false;
    if (pData != NULL) return false;
    // width/height/channels should remain untouched (0)
    if (width != 0 || height != 0 || channels != 0) return false;

    return true;
}

// Test 2: PngLoadImage should fail gracefully for a non-existent file
static bool test_PngLoadImage_FileNotFound()
{
    unsigned char* pData = NULL;
    int width = 0, height = 0, channels = 0;
    png_color bkgColor = {0, 0, 0};

    BOOL result = PngLoadImage("nonexistent_file_123456.png", &pData, &width, &height, &channels, &bkgColor);

    if (result != FALSE) return false;
    if (pData != NULL) return false;
    // width/height/channels should remain untouched (0)
    if (width != 0 || height != 0 || channels != 0) return false;

    return true;
}

// Test 3: PngLoadImage should fail on an invalid PNG signature
static bool test_PngLoadImage_InvalidSignature()
{
    // Create a small invalid file (not a PNG)
    const char* path = "invalid_signature.bin";
    const unsigned char trash[] = { 'N','O','T','A','P','N','G','!' };
    if (!writeBinaryFile(path, trash, sizeof(trash))) {
        std::cerr << "Failed to write invalid signature test file.\n";
        return false;
    }

    unsigned char* pData = NULL;
    int width = 0, height = 0, channels = 0;
    png_color bkgColor = {0, 0, 0};

    BOOL result = PngLoadImage(path, &pData, &width, &height, &channels, &bkgColor);

    // Cleanup test file
    remove(path);

    if (result != FALSE) return false;
    if (pData != NULL) return false;
    if (width != 0 || height != 0 || channels != 0) return false;

    return true;
}

// Test 4: End-to-end: Save a tiny 1x1 red image with PngSaveImage and read it back with PngLoadImage
static bool test_PngLoadImage_SuccessRead()
{
    // Prepare a tiny 1x1 red pixel
    const int w = 1;
    const int h = 1;
    const int channels_expected = 3;
    unsigned char pixel[3] = {255, 0, 0}; // Red

    const char* savedPath = "test_1x1_red.png";

    // Save the image using the provided PngSaveImage (through the file's API)
    png_color dummyColor = {0, 0, 0};
    if (!PngSaveImage((PTSTR)savedPath, pixel, w, h, dummyColor)) {
        std::cerr << "PngSaveImage failed to write test PNG.\n";
        return false;
    }

    // Now load the image back
    unsigned char* loadedData = NULL;
    int widthLoaded = 0, heightLoaded = 0, channelsLoaded = 0;
    png_color loadedBkColor = {0, 0, 0};

    BOOL readResult = PngLoadImage((PTSTR)savedPath, &loadedData, &widthLoaded, &heightLoaded, &channelsLoaded, &loadedBkColor);

    // Cleanup saved file
    remove(savedPath);

    if (readResult != TRUE) return false;
    if (widthLoaded != w || heightLoaded != h) return false;
    if (channelsLoaded != channels_expected) return false;
    if (loadedData == NULL) return false;

    // Validate pixel data equals what we saved
    if (loadedData[0] != pixel[0] || loadedData[1] != pixel[1] || loadedData[2] != pixel[2]) {
        free(loadedData);
        return false;
    }

    // Free allocated image data
    free(loadedData);

    return true;
}

int main()
{
    // Run tests; in case of failure, continue to collect results
    bool t;

    t = test_PngLoadImage_NullFileName();
    LOG_TEST_RESULT("PngLoadImage_NullFileName", t);

    t = test_PngLoadImage_FileNotFound();
    LOG_TEST_RESULT("PngLoadImage_FileNotFound", t);

    t = test_PngLoadImage_InvalidSignature();
    LOG_TEST_RESULT("PngLoadImage_InvalidSignature", t);

    t = test_PngLoadImage_SuccessRead();
    LOG_TEST_RESULT("PngLoadImage_SuccessRead", t);

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << "\n";

    // Return 0 if all tests passed, non-zero otherwise
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
// High-quality C++11 unit tests for the focal function readpng_get_image
// Test harness uses real libpng to generate small PNG inputs and then exercises
// readpng_init / readpng_get_image / readpng_cleanup to ensure correct behavior.
// No GTest; a lightweight, self-contained test runner is used with non-terminating
// assertions (checks that log failures but continue execution).

#include <vector>
#include <string>
#include <zlib.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <readpng.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Link against libpng; ensure this is available in the build environment.

extern "C" {
}

static int g_failures = 0;  // global failure counter for non-terminating assertions

// Simple non-terminating assertion helper
static bool expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::fprintf(stderr, "Test Failure: %s\n", msg);
        ++g_failures;
        return false;
    }
    return true;
}

// Helper: write a minimal PNG image using libpng
// color_type & bit_depth must be supported by libpng
static int write_png(const char* filename,
                     int width, int height,
                     int color_type, int bit_depth,
                     const unsigned char* data)
{
    FILE* fp = std::fopen(filename, "wb");
    if (!fp) {
        std::perror("fopen");
        return -1;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::fclose(fp);
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        std::fclose(fp);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        std::fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // For simplicity, no gamma, sRGB chunks etc.
    png_write_info(png_ptr, info_ptr);

    // Determine bytes per pixel
    int channels = 0;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:            channels = 1; break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:      channels = 2; break;
        case PNG_COLOR_TYPE_RGB:             channels = 3; break;
        case PNG_COLOR_TYPE_RGB_ALPHA:       channels = 4; break;
        default: channels = 3; // fallback
    }

    // Write rows
    const unsigned char* row = data;
    for (int y = 0; y < height; ++y) {
        png_write_row(png_ptr, const_cast<unsigned char*>(row));
        row += width * channels;
    }

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    std::fclose(fp);
    return 0;
}

// Test 1: RGBA 1x1 PNG should return 4 channels and rowbytes = 4
static bool test_readpng_get_image_rgba() {
    const char* filename = "test_rgba.png";

    // Prepare a 1x1 red pixel with full alpha
    const int width = 1, height = 1;
    const int color_type = PNG_COLOR_TYPE_RGBA;
    const int bit_depth = 8;
    unsigned char pixel[4] = { 255, 0, 0, 255 }; // RGBA: Red, opaque
    std::vector<unsigned char> data(pixel, pixel + 4);

    if (write_png(filename, width, height, color_type, bit_depth, data.data()) != 0) {
        std::fprintf(stderr, "Failed to write test RGBA PNG data\n");
        return false;
    }

    FILE* infile = std::fopen(filename, "rb");
    if (!infile) {
        std::perror("fopen test_rgba.png");
        std::remove(filename);
        return false;
    }

    ulg w = 0, h = 0;
    int init_res = readpng_init(infile, &w, &h);
    // Expect successful initialization
    bool ok = expect_true(init_res == 0, "readpng_init RGBA failed to initialize");

    // Only proceed if initialization succeeded
    int channels = 0;
    ulg rowbytes = 0;
    uch* image_data = readpng_get_image(1.0, &channels, &rowbytes); // display exponent, channels, rowbytes

    // Validate outputs
    ok = ok && expect_true(image_data != NULL, "readpng_get_image RGBA returned NULL data");
    ok = ok && expect_true(channels == 4, "readpng_get_image RGBA channels != 4");
    ok = ok && expect_true(rowbytes == 4, "readpng_get_image RGBA rowbytes != 4");

    // Cleanup
    readpng_cleanup(1); // free image_data if allocated
    std::fclose(infile);
    std::remove(filename);

    return ok;
}

// Test 2: GRAY (1-channel) PNG expanded to RGB should return 3 channels and rowbytes = 3
static bool test_readpng_get_image_gray_expanded() {
    const char* filename = "test_gray.png";

    // Prepare a 1x1 gray pixel (value 128) - PNG_GRAY will be expanded to RGB
    const int width = 1, height = 1;
    const int color_type = PNG_COLOR_TYPE_GRAY;
    const int bit_depth = 8;
    unsigned char pixel[1] = { 128 };
    std::vector<unsigned char> data(pixel, pixel + 1);

    if (write_png(filename, width, height, color_type, bit_depth, data.data()) != 0) {
        std::fprintf(stderr, "Failed to write test Gray PNG data\n");
        return false;
    }

    FILE* infile = std::fopen(filename, "rb");
    if (!infile) {
        std::perror("fopen test_gray.png");
        std::remove(filename);
        return false;
    }

    ulg w = 0, h = 0;
    int init_res = readpng_init(infile, &w, &h);
    bool ok = expect_true(init_res == 0, "readpng_init Gray failed to initialize");

    int channels = 0;
    ulg rowbytes = 0;
    uch* image_data = readpng_get_image(1.0, &channels, &rowbytes);

    ok = ok && expect_true(image_data != NULL, "readpng_get_image Gray returned NULL data");
    ok = ok && expect_true(channels == 3, "readpng_get_image Gray did not expand to 3 channels");
    ok = ok && expect_true(rowbytes == 3, "readpng_get_image Gray rowbytes != 3");

    readpng_cleanup(1);
    std::fclose(infile);
    std::remove(filename);

    return ok;
}

int main() {
    std::cout << "Starting readpng_get_image unit tests (C++11, no GTest)..." << std::endl;

    bool t1 = test_readpng_get_image_rgba();
    std::cout << "Test RGBA 1x1: " << (t1 ? "PASS" : "FAIL") << std::endl;

    bool t2 = test_readpng_get_image_gray_expanded();
    std::cout << "Test Gray 1x1 expanded to RGB: " << (t2 ? "PASS" : "FAIL") << std::endl;

    if (g_failures > 0) {
        std::cout << "UNIT TEST RESULT: FAILS = " << g_failures << std::endl;
        return 1;
    } else {
        std::cout << "UNIT TEST RESULT: ALL TESTS PASSED" << std::endl;
        return 0;
    }
}
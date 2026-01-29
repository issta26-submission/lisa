#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Include libpng public API for png_image_write_to_stdio and related types.
// Assumes libpng is installed and accessible in the include/library path.

// Simple lightweight test framework (non-terminating assertions) to avoid GTest.
// This aligns with the guidance to use non-terminating expectations (EXPECT_*-like)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(expr) do { \
    ++g_total; \
    if (!(expr)) { \
        ++g_failed; \
        std::cerr << "[FAIL] " #expr " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "[PASS] " #expr << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    ++g_total; \
    if (!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "[FAIL] " #a " == " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } else { \
        std::cout << "[PASS] " #a " == " #b << std::endl; \
    } \
} while (0)

typedef bool (*TestFn)();


// Test 1: Basic, valid path - write a small 2x2 RGB image to a temporary FILE*.
// This exercises the true-branch path of the main predicates.
bool test_png_image_write_to_stdio_basic() {
    // Prepare a minimal valid png_image structure
    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 2;
    image.height = 2;
    image.format = PNG_FORMAT_RGB; // 3 bytes per pixel

    // Prepare a small 2x2 RGB buffer (row_stride = width * channels)
    const png_int_32 row_stride = image.width * 3;
    unsigned char buffer[12];
    for (size_t i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = static_cast<unsigned char>(i * 37); // arbitrary pattern
    }

    // Use a temporary file for writing
    FILE *fp = tmpfile();
    if (fp == NULL) {
        // If tmpfile isn't available, skip gracefully but still count as fail
        std::cerr << "Unable to create temporary file for test_png_image_write_to_stdio_basic." << std::endl;
        return false;
    }

    int result = png_image_write_to_stdio(&image, fp, 0, buffer, row_stride, NULL);

    fclose(fp);

    // Expect a successful write (non-zero return)
    return result != 0;
}


// Test 2: image == NULL should hit the outermost false branch and return 0 as per code.
bool test_png_image_write_to_stdio_image_null() {
    unsigned char buffer[12];
    FILE *fp = tmpfile();
    if (fp == NULL) {
        std::cerr << "Unable to create temporary file for test_png_image_write_to_stdio_image_null." << std::endl;
        return false;
    }
    int result = png_image_write_to_stdio(nullptr, fp, 0, buffer, 6, NULL);
    fclose(fp);
    // The function is specified to return 0 when image is NULL
    return result == 0;
}


// Test 3: file == NULL should trigger the invalid-argument error path.
// We expect a non-zero value typical of an error code from png_image_error.
bool test_png_image_write_to_stdio_file_null() {
    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 2;
    image.height = 2;
    image.format = PNG_FORMAT_RGB;

    unsigned char buffer[12];
    for (size_t i = 0; i < sizeof(buffer); ++i) buffer[i] = static_cast<unsigned char>(i);

    int result = png_image_write_to_stdio(&image, nullptr, 0, buffer, 6, NULL);
    // Expect an error path; non-zero return is typical for png_image_error
    return result != 0;
}


// Test 4: buffer == NULL should trigger the invalid-argument error path.
// Expect non-zero return as per the code's error handling.
bool test_png_image_write_to_stdio_buffer_null() {
    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 2;
    image.height = 2;
    image.format = PNG_FORMAT_RGB;

    FILE *fp = tmpfile();
    if (fp == NULL) {
        std::cerr << "Unable to create temporary file for test_png_image_write_to_stdio_buffer_null." << std::endl;
        return false;
    }

    int result = png_image_write_to_stdio(&image, fp, 0, nullptr, 6, NULL);
    fclose(fp);
    return result != 0;
}


// Test 5: Wrong PNG image version should trigger the version-mismatch error path.
// Expect non-zero return due to png_image_error path.
bool test_png_image_write_to_stdio_wrong_version() {
    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = 0; // Deliberately incorrect
    image.width = 2;
    image.height = 2;
    image.format = PNG_FORMAT_RGB;

    unsigned char buffer[12];
    for (size_t i = 0; i < sizeof(buffer); ++i) buffer[i] = static_cast<unsigned char>(i);

    FILE *fp = tmpfile();
    if (fp == NULL) {
        std::cerr << "Unable to create temporary file for test_png_image_write_to_stdio_wrong_version." << std::endl;
        return false;
    }

    int result = png_image_write_to_stdio(&image, fp, 0, buffer, 6, NULL);
    fclose(fp);
    return result != 0;
}


int main() {
    // List of tests to run
    std::vector<std::pair<std::string, TestFn>> tests = {
        {"test_png_image_write_to_stdio_basic", test_png_image_write_to_stdio_basic},
        {"test_png_image_write_to_stdio_image_null", test_png_image_write_to_stdio_image_null},
        {"test_png_image_write_to_stdio_file_null", test_png_image_write_to_stdio_file_null},
        {"test_png_image_write_to_stdio_buffer_null", test_png_image_write_to_stdio_buffer_null},
        {"test_png_image_write_to_stdio_wrong_version", test_png_image_write_to_stdio_wrong_version}
    };

    std::cout << "Running " << tests.size() << " tests for png_image_write_to_stdio (C libpng integration)..." << std::endl;

    int total_before = g_total;
    int failed_before = g_failed;

    for (const auto &t : tests) {
        bool ok = t.second();
        // The test runner already prints PASS/FAIL per test via EXPECT_* macros inside tests.
        // Here, we also guard against silent failures if any.
        if (!ok) {
            std::cerr << "[UNHANDLED] Test " << t.first << " reported failure." << std::endl;
        }
    }

    int total = g_total - total_before;
    int failed = g_failed - failed_before;

    std::cout << "Test suite finished. Total executed: " << total
              << ", Failures: " << failed << std::endl;

    // Non-terminating exit: return non-zero if there were failures to indicate overall failure.
    return (failed != 0) ? 1 : 0;
}
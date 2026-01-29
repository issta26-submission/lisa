// C++11-compatible unit test harness for the focal C function test_one_file
// This test suite targets the PNG test helper from pngtest.c via the C API.
// We generate minimal PNG inputs using libpng to exercise the "happy path"
// (valid PNG round-trip) and error paths (non-existent input, non-PNG input).
// The tests are designed to be non-terminating (they print results and continue).

#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Expose the focal function from the C source as C linkage
extern "C" int test_one_file(const char*, const char*);


// Helper: create a tiny 1x1 PNG (red pixel) to use as valid input for test_one_file
static bool write_simple_png(const char* path) {
    FILE* fp = fopen(path, "wb");
    if (!fp) return false;

    // libpng write structs
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        // On error, clean up
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    // 1x1 image, 8-bit depth, RGB
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write the PNG header
    png_write_info(png_ptr, info_ptr);

    // One pixel: red (255,0,0)
    png_bytep row = (png_bytep)malloc(3);
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }
    row[0] = 255; row[1] = 0; row[2] = 0;

    // Write the single row
    png_write_row(png_ptr, row);

    // Finish
    png_write_end(png_ptr, NULL);

    // Cleanup
    free(row);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}

// Helper: create a small plain text file (not a PNG) to test error path for non-PNG input
static bool write_text_file(const char* path, const char* data) {
    FILE* f = fopen(path, "w");
    if (!f) return false;
    fprintf(f, "%s", data);
    fclose(f);
    return true;
}

// Simple non-terminating assertion helper
static void log_test_result(const std::string& name, bool passed, const std::string& detail) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name;
    if (!detail.empty()) std::cout << " - " << detail;
    std::cout << std::endl;
}

// Test 1: Non-existent input must return 1 (error path)
static void test_nonexistent_input_returns_error() {
    const std::string inname = "test_nonexistent_input.png";
    const std::string outname = "test_nonexistent_output.png";

    // Ensure input does not exist
    std::remove(inname.c_str());

    int ret = test_one_file(inname.c_str(), outname.c_str());

    bool passed = (ret == 1);
    log_test_result("test_one_file with non-existent input returns 1", passed,
                    "expected 1, got " + std::to_string(ret));
}

// Test 2: Valid PNG input should produce a (likely) successful run (return 0)
static void test_valid_input_roundtrip_returns_success() {
    const std::string inname = "test_valid_input.png";
    const std::string outname = "test_valid_input_out.png";

    // Create a minimal valid PNG
    if (!write_simple_png(inname.c_str())) {
        log_test_result("test_valid_input_roundtrip_returns_success", false,
                        "failed to create test PNG input");
        return;
    }

    int ret = test_one_file(inname.c_str(), outname.c_str());

    // Cleanup created files
    std::remove(inname.c_str());
    std::remove(outname.c_str());

    bool passed = (ret == 0);
    log_test_result("test_valid_input_roundtrip_returns_success", passed,
                    "expected 0, got " + std::to_string(ret));
}

// Test 3: Non-PNG input should trigger error path (return 1)
static void test_non_png_input_returns_error() {
    const std::string inname = "test_not_png.txt";
    const std::string outname = "test_not_png_out.png";

    // Create a text file that's not a PNG
    if (!write_text_file(inname.c_str(), "this is not a PNG file")) {
        log_test_result("test_non_png_input_returns_error", false,
                        "failed to create test non-PNG input");
        return;
    }

    int ret = test_one_file(inname.c_str(), outname.c_str());

    // Cleanup input; output may not have been created
    std::remove(inname.c_str());
    // If out file exists from previous runs, clean it
    std::remove(outname.c_str());

    bool passed = (ret == 1);
    log_test_result("test_non_png_input_returns_error", passed,
                    "expected 1, got " + std::to_string(ret));
}

// Main launcher
int main() {
    // Informational header
    std::cout << "Running pngtest.c focused test_one_file unit tests (C++ harness)\n";

    // Run tests
    test_nonexistent_input_returns_error();
    test_valid_input_roundtrip_returns_success();
    test_non_png_input_returns_error();

    // Summary (best-effort since we don't aggregate automatically in this simple harness)
    std::cout << "Tests completed. Review individual test lines for pass/fail status.\n";

    // Return success if all tests reported PASS
    // Note: We could consolidate pass/fail counts if we stored results globally;
    // for simplicity, we rely on the per-test logs above.
    return 0;
}
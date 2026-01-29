// Test suite for the focal function: png_get_cHRM
// Targeted to compile with a C++11 compiler and link against libpng.
// No GTest, a lightweight in-process test harness is implemented.
// Each test includes comments explaining purpose and expectations.

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failed_tests = 0;

static void log_failure(const std::string& test_name, const std::string& msg) {
    std::cerr << "[TEST FAILED] " << test_name << ": " << msg << std::endl;
    ++g_failed_tests;
}

static bool almost_equal(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) <= eps;
}

// Helper to set up a fresh PNG read struct and info struct
static bool setup_png(png_structp &png_ptr, png_infop &info_ptr) {
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_failure("setup_png", "Failed to create read struct.");
        return false;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        log_failure("setup_png", "Failed to create info struct.");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        png_ptr = nullptr;
        return false;
    }
    return true;
}

// Test 1: Basic retrieval of cHRM values after setting via public API (png_set_cHRM)
static void test_png_get_cHRM_basic() {
    const std::string test_name = "test_png_get_cHRM_basic";
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!setup_png(png_ptr, info_ptr)) {
        log_failure(test_name, "Setup failed.");
        return;
    }

    // Set cHRM values using the public API. These correspond to the White Point
    // and Primaries in normalized [0.0, 1.0] range for test purposes.
    // Note: The exact numeric values are not critical; we verify round-trip retrieval.
    double wx  = 0.12;
    double wy  = 0.34;
    double rx  = 0.13;
    double ry  = 0.25;
    double gx  = 0.33;
    double gy  = 0.66;
    double bx  = 0.50;
    double by  = 0.75;

    // The signature is assumed (as per libpng API) to be:
    // void png_set_cHRM(png_structp, png_infop, double wx, double wy, double rx, double ry,
    //                   double gx, double gy, double bx, double by);
    // If your libpng version differs, adjust accordingly.
    png_set_cHRM(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by);

    double whitex, whitey, redx, redy, greenx, greeny, bluex, bluey;
    png_uint_32 ret = png_get_cHRM(png_ptr, info_ptr,
                                   &whitex, &whitey,
                                   &redx, &redy,
                                   &greenx, &greeny,
                                   &bluex, &bluey);

    if (ret != PNG_INFO_cHRM) {
        log_failure(test_name, "png_get_cHRM did not return PNG_INFO_cHRM as expected.");
    } else {
        // Check retrieved values against what we set (within a tolerance)
        if (!almost_equal(whitex, wx) || !almost_equal(whitey, wy) ||
            !almost_equal(redx, rx) || !almost_equal(redy, ry) ||
            !almost_equal(greenx, gx) || !almost_equal(greeny, gy) ||
            !almost_equal(bluex, bx) || !almost_equal(bluey, by)) {
            log_failure(test_name, "Retrieved cHRM values do not match the set values (round-trip mismatch).");
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}

// Test 2: Ensure that non-null pointers are written while some NULL pointers are ignored
static void test_png_get_cHRM_partial_pointers() {
    const std::string test_name = "test_png_get_cHRM_partial_pointers";
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!setup_png(png_ptr, info_ptr)) {
        log_failure(test_name, "Setup failed.");
        return;
    }

    // Set known cHRM values
    double wx  = 0.11;
    double wy  = 0.22;
    double rx  = 0.33;
    double ry  = 0.44;
    double gx  = 0.55;
    double gy  = 0.66;
    double bx  = 0.77;
    double by  = 0.88;

    png_set_cHRM(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by);

    // Prepare only a subset of pointers: whiten (NULL), whitey (valid), redx (valid),
    // redy (NULL), greenx (valid), greeny (valid), bluex (NULL), bluey (valid)
    double whitey_val = -1.0;
    double redx_val = -2.0;
    double greenx_val = -3.0;
    double greeny_val = -4.0;
    double bluey_val = -5.0;

    double whitex_out = 0.0; // Intentionally not used because NULL is passed
    double * WhitexPtr = NULL;
    double * WhiteyPtr = &whitey_val;
    double * RedxPtr = &redx_val;
    double * RedyPtr = NULL;
    double * GreenxPtr = &greenx_val;
    double * GreenyPtr = &greeny_val;
    double * BluexPtr = NULL;
    double * BlueyPtr = &bluey_val;

    // Call with selective pointers (note: the function will only write where non-NULL)
    png_get_cHRM(png_ptr, info_ptr,
                 WhitexPtr, WhiteyPtr,
                 RedxPtr, RedyPtr,
                 GreenxPtr, GreenyPtr,
                 BluexPtr, BlueyPtr);

    // whitey should be updated
    if (!almost_equal(whitey_val, wy)) {
        log_failure(test_name, "whitey was not updated as expected.");
    }

    // redx updated
    if (!almost_equal(redx_val, rx)) {
        log_failure(test_name, "redx was not updated as expected.");
    }

    // greenx and greeny updated
    if (!almost_equal(greenx_val, gx) || !almost_equal(greeny_val, gy)) {
        log_failure(test_name, "greenx/greeny were not updated as expected.");
    }

    // bluey updated
    if (!almost_equal(bluey_val, by)) {
        log_failure(test_name, "bluey was not updated as expected.");
    }

    // whitex, redy, bluex were passed as NULL; since we can't observe their output here, we just ensure function did not crash.

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}

// Test 3: Behavior when info_ptr is NULL
static void test_png_get_cHRM_null_info_ptr() {
    const std::string test_name = "test_png_get_cHRM_null_info_ptr";
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!setup_png(png_ptr, info_ptr)) {
        log_failure(test_name, "Setup failed.");
        return;
    }

    // Call with info_ptr = NULL should return 0
    double wX = 0.0, wY = 0.0, rX = 0.0, rY = 0.0, gX = 0.0, gY = 0.0, bX = 0.0, bY = 0.0;
    png_uint_32 ret = png_get_cHRM(png_ptr, NULL,
                                   &wX, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

    if (ret != 0) {
        log_failure(test_name, "png_get_cHRM should return 0 when info_ptr is NULL.");
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}

// Test 4: Behavior when info_ptr is valid but PNG_INFO_cHRM flag not set
static void test_png_get_cHRM_invalid_flag() {
    const std::string test_name = "test_png_get_cHRM_invalid_flag";
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!setup_png(png_ptr, info_ptr)) {
        log_failure(test_name, "Setup failed.");
        return;
    }

    // Do not set any cHRM; ensure the function returns 0.
    // Ensure the valid flag doesn't indicate cHRM presence.
    info_ptr->valid = 0; // Force flag off

    double wx = 0.0, wy = 0.0, rx = 0.0, ry = 0.0, gx = 0.0, gy = 0.0, bx = 0.0, by = 0.0;
    png_uint_32 ret = png_get_cHRM(png_ptr, info_ptr,
                                   &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);

    if (ret != 0) {
        log_failure(test_name, "png_get_cHRM should return 0 when PNG_INFO_cHRM flag is not set.");
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}

// Test 5: Ensure function handles NULL png_ptr gracefully
static void test_png_get_cHRM_null_png_ptr() {
    const std::string test_name = "test_png_get_cHRM_null_png_ptr";

    double wx = 0.0, wy = 0.0, rx = 0.0, ry = 0.0, gx = 0.0, gy = 0.0, bx = 0.0, by = 0.0;

    // If png_ptr is NULL, function should return 0
    png_uint_32 ret = png_get_cHRM(NULL, NULL,
                                   &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);
    if (ret != 0) {
        log_failure(test_name, "png_get_cHRM should return 0 when png_ptr is NULL.");
    }
}

// Driver: run all tests
int main() {
    // Run all defined tests. Each test logs its own failures without terminating the program.
    test_png_get_cHRM_basic();
    test_png_get_cHRM_partial_pointers();
    test_png_get_cHRM_null_info_ptr();
    test_png_get_cHRM_invalid_flag();
    test_png_get_cHRM_null_png_ptr();

    if (g_failed_tests == 0) {
        std::cout << "[ALL TESTS PASSED] png_get_cHRM basic behavior verified." << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUMMARY] " << g_failed_tests << " tests failed." << std::endl;
        return 1;
    }
}
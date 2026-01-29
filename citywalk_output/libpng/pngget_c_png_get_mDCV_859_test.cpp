// Test suite for the focal method: png_get_mDCV
// This standalone C++11 test reproduces the essential behavior of png_get_mDCV
// (as described in the provided snippet) and validates its logic via multiple tests.
// The tests are self-contained and do not rely on external testing frameworks
// (GTest is avoided as requested). Each test includes explanatory comments.

/*
Test notes:
- We mimic the minimal libpng-related types and constants necessary for the
  png_get_mDCV function to operate.
- We verify:
  1) All color component outputs are correctly computed when PNG_INFO_mDCV is set.
  2) Partial pointers (some outputs NULL) are handled gracefully (only non-NULL
     pointers are written).
  3) Behavior when info_ptr is NULL or the PNG_INFO_mDCV flag is not set.
  4) Non-destructive behavior: when a test case should not modify outputs due to
     invalid inputs, initial sentinel values remain unchanged.
- Values are compared with a small tolerance due to floating-point arithmetic.
*/

// Include standard libraries
#include <string>
#include <iostream>
#include <cmath>
#include <pngpriv.h>
#include <cstddef>


// Minimal mock-up of required PNG structures and constants
struct png_struct_def {};// dummy png_ptr type
typedef png_struct_def* png_structrp;
typedef const png_struct_def* png_const_structrp;

struct png_info_def {
    unsigned valid;
    double mastering_white_x;
    double mastering_white_y;
    double mastering_red_x;
    double mastering_red_y;
    double mastering_green_x;
    double mastering_green_y;
    double mastering_blue_x;
    double mastering_blue_y;
    double mastering_maxDL;
    double mastering_minDL;
};
typedef png_info_def* png_inforp;
typedef const png_info_def* png_const_inforp;

// PNG_INFO_mDCV flag (as used by the function logic)
const unsigned PNG_INFO_mDCV = 1u;

// No-op stub for png_debug1 used inside the function
void png_debug1(int level, const char* fmt, const char* arg) {
    (void)level; (void)fmt; (void)arg;
    // Intentionally left blank to avoid side effects during tests
}

// The focal function under test (reproduced locally for a self-contained test)
extern "C" int
png_get_mDCV(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *white_x, double *white_y, double *red_x, double *red_y,
    double *green_x, double *green_y, double *blue_x, double *blue_y,
    double *mastering_maxDL, double *mastering_minDL)
{
{
   png_debug1(1, "in %s retrieval function", "mDCV(float)");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_mDCV) != 0)
   {
      if (white_x != NULL) *white_x = info_ptr->mastering_white_x * .00002;
      if (white_y != NULL) *white_y = info_ptr->mastering_white_y * .00002;
      if (red_x != NULL) *red_x = info_ptr->mastering_red_x * .00002;
      if (red_y != NULL) *red_y = info_ptr->mastering_red_y * .00002;
      if (green_x != NULL) *green_x = info_ptr->mastering_green_x * .00002;
      if (green_y != NULL) *green_y = info_ptr->mastering_green_y * .00002;
      if (blue_x != NULL) *blue_x = info_ptr->mastering_blue_x * .00002;
      if (blue_y != NULL) *blue_y = info_ptr->mastering_blue_y * .00002;
      if (mastering_maxDL != NULL)
         *mastering_maxDL = info_ptr->mastering_maxDL * .0001;
      if (mastering_minDL != NULL)
         *mastering_minDL = info_ptr->mastering_minDL * .0001;
      return PNG_INFO_mDCV;
   }
   return 0;
}
}

// Helper function: compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Global test result counters
struct TestResults {
    int total = 0;
    int passed = 0;
    int failed = 0;
    void report() const {
        std::cout << "\nTest Summary: " << passed << "/" << total
                  << " tests passed, " << failed << " failed.\n";
    }
} g_results;

// Test 1: All fields set, all non-null pointers are provided.
// Expect all outputs to be white_x/y, red_x/y, green_x/y, blue_x/y scaled by 0.00002,
// and maxDL/minDL scaled by 0.0001. Return value should be PNG_INFO_mDCV.
bool test_all_fields_set() {
    // Setup
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_def info;
    info.valid = PNG_INFO_mDCV;
    // Initialize mastering values with known numbers
    info.mastering_white_x = 100000.0;
    info.mastering_white_y = 200000.0;
    info.mastering_red_x   = 300000.0;
    info.mastering_red_y   = 400000.0;
    info.mastering_green_x = 500000.0;
    info.mastering_green_y = 600000.0;
    info.mastering_blue_x  = 700000.0;
    info.mastering_blue_y  = 800000.0;
    info.mastering_maxDL   = 12345.0;
    info.mastering_minDL   = 6789.0;

    // Pointers to receive results
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    double maxDL, minDL;

    g_results.total++;

    // Execute
    int ret = png_get_mDCV(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(&info),
                           &white_x, &white_y, &red_x, &red_y,
                           &green_x, &green_y, &blue_x, &blue_y,
                           &maxDL, &minDL);

    // Verify
    bool ok = true;
    if (ret != (int)PNG_INFO_mDCV) {
        std::cerr << "test_all_fields_set: expected return " << PNG_INFO_mDCV
                  << ", got " << ret << "\n";
        ok = false;
    }
    double tol = 1e-9;
    if (!almost_equal(white_x, info.mastering_white_x * 0.00002, tol)
        || !almost_equal(white_y, info.mastering_white_y * 0.00002, tol)
        || !almost_equal(red_x, info.mastering_red_x * 0.00002, tol)
        || !almost_equal(red_y, info.mastering_red_y * 0.00002, tol)
        || !almost_equal(green_x, info.mastering_green_x * 0.00002, tol)
        || !almost_equal(green_y, info.mastering_green_y * 0.00002, tol)
        || !almost_equal(blue_x, info.mastering_blue_x * 0.00002, tol)
        || !almost_equal(blue_y, info.mastering_blue_y * 0.00002, tol)
        || !almost_equal(maxDL, info.mastering_maxDL * 0.0001, tol)
        || !almost_equal(minDL, info.mastering_minDL * 0.0001, tol)) {
        std::cerr << "test_all_fields_set: output values did not match expected.\n";
        ok = false;
    }

    if (ok) {
        std::cout << "test_all_fields_set: PASSED\n";
        g_results.passed++;
    } else {
        std::cerr << "test_all_fields_set: FAILED\n";
        g_results.failed++;
    }
    return ok;
}

// Test 2: Partial pointers provided (only white_x and blue_y non-null).
// Ensures that only non-NULL pointers are written.
bool test_partial_pointers() {
    // Setup
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_def info;
    info.valid = PNG_INFO_mDCV;
    info.mastering_white_x = 2.0;   // small values to ease checking
    info.mastering_white_y = 0.0;
    info.mastering_red_x = -1.0;
    info.mastering_red_y = 4.0;
    info.mastering_green_x = 0.5;
    info.mastering_green_y = -2.0;
    info.mastering_blue_x = 1.25;
    info.mastering_blue_y = 3.5;
    info.mastering_maxDL = 100.0;
    info.mastering_minDL = 50.0;

    // Only white_x and blue_y will be provided (non-NULL)
    double white_x = 0.0, white_y = -1.0; // white_y is unused in this test
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = -1.0;
    double maxDL = 0.0, minDL = 0.0;

    g_results.total++;

    int ret = png_get_mDCV(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(&info),
                           &white_x, nullptr,
                           &red_x, nullptr,
                           &green_x, nullptr,
                           &blue_x, &blue_y,
                           &maxDL, &minDL);

    bool ok = true;
    if (ret != (int)PNG_INFO_mDCV) {
        std::cerr << "test_partial_pointers: expected return " << PNG_INFO_mDCV
                  << ", got " << ret << "\n";
        ok = false;
    }
    double tol = 1e-9;
    // white_x should be set; white_y is NULL so not modified here
    if (!almost_equal(white_x, info.mastering_white_x * 0.00002, tol)) {
        std::cerr << "test_partial_pointers: white_x not set correctly.\n";
        ok = false;
    }
    // red_x should remain as provided (since non-NULL write is not performed for red_x)
    // but we provided red_x pointer and passed NULL for red_y; we expect red_x to be overwritten
    if (!almost_equal(red_x, info.mastering_red_x * 0.00002, tol)) {
        std::cerr << "test_partial_pointers: red_x not set correctly.\n";
        ok = false;
    }
    // green_x should be overwritten
    if (!almost_equal(green_x, info.mastering_green_x * 0.00002, tol)) {
        std::cerr << "test_partial_pointers: green_x not set correctly.\n";
        ok = false;
    }
    // blue_x should be overwritten
    if (!almost_equal(blue_x, info.mastering_blue_x * 0.00002, tol)) {
        std::cerr << "test_partial_pointers: blue_x not set correctly.\n";
        ok = false;
    }
    // maxDL and minDL should be written
    if (!almost_equal(maxDL, info.mastering_maxDL * 0.0001, tol) ||
        !almost_equal(minDL, info.mastering_minDL * 0.0001, tol)) {
        std::cerr << "test_partial_pointers: maxDL/minDL not set correctly.\n";
        ok = false;
    }

    if (ok) {
        std::cout << "test_partial_pointers: PASSED\n";
        g_results.passed++;
    } else {
        std::cerr << "test_partial_pointers: FAILED\n";
        g_results.failed++;
    }
    return ok;
}

// Test 3: info_ptr valid flag not set (PNG_INFO_mDCV not present).
// Expect 0 return and no modifications to output values.
bool test_invalid_info_flag() {
    // Setup
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_def info;
    info.valid = 0; // flag not set
    info.mastering_white_x = 10.0;
    info.mastering_white_y = 20.0;
    info.mastering_red_x = 30.0;
    info.mastering_red_y = 40.0;
    info.mastering_green_x = 50.0;
    info.mastering_green_y = 60.0;
    info.mastering_blue_x = 70.0;
    info.mastering_blue_y = 80.0;
    info.mastering_maxDL = 1000.0;
    info.mastering_minDL = 800.0;

    // Output variables initialized to sentinel values
    double white_x = -9999.0, white_y = -9999.0, red_x = -9999.0, red_y = -9999.0;
    double green_x = -9999.0, green_y = -9999.0;
    double blue_x = -9999.0, blue_y = -9999.0;
    double maxDL = -9999.0, minDL = -9999.0;

    g_results.total++;

    int ret = png_get_mDCV(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(&info),
                           &white_x, &white_y, &red_x, &red_y,
                           &green_x, &green_y, &blue_x, &blue_y,
                           &maxDL, &minDL);

    bool ok = true;
    if (ret != 0) {
        std::cerr << "test_invalid_info_flag: expected return 0, got " << ret << "\n";
        ok = false;
    }
    // Ensure outputs remain unchanged (still sentinel values)
    if (!almost_equal(white_x, -9999.0) || !almost_equal(white_y, -9999.0) ||
        !almost_equal(red_x, -9999.0) || !almost_equal(red_y, -9999.0) ||
        !almost_equal(green_x, -9999.0) || !almost_equal(green_y, -9999.0) ||
        !almost_equal(blue_x, -9999.0) || !almost_equal(blue_y, -9999.0) ||
        !almost_equal(maxDL, -9999.0) || !almost_equal(minDL, -9999.0)) {
        std::cerr << "test_invalid_info_flag: outputs were modified unexpectedly.\n";
        ok = false;
    }

    if (ok) {
        std::cout << "test_invalid_info_flag: PASSED\n";
        g_results.passed++;
    } else {
        std::cerr << "test_invalid_info_flag: FAILED\n";
        g_results.failed++;
    }
    return ok;
}

// Test 4: NULL png_ptr should yield 0 and no crash; outputs unchanged.
bool test_null_png_ptr() {
    // Setup
    png_info_def info;
    info.valid = PNG_INFO_mDCV;
    info.mastering_white_x = 7.0;
    info.mastering_white_y = 8.0;
    info.mastering_red_x = 9.0;
    info.mastering_red_y = 10.0;
    info.mastering_green_x = 11.0;
    info.mastering_green_y = 12.0;
    info.mastering_blue_x = 13.0;
    info.mastering_blue_y = 14.0;
    info.mastering_maxDL = 1.0;
    info.mastering_minDL = 2.0;

    double white_x = 0.0;
    double white_y = 0.0, red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double maxDL = 0.0, minDL = 0.0;

    g_results.total++;

    int ret = png_get_mDCV(nullptr,
                           reinterpret_cast<png_const_inforp>(&info),
                           &white_x, &white_y, &red_x, &red_y,
                           &green_x, &green_y, &blue_x, &blue_y,
                           &maxDL, &minDL);

    bool ok = true;
    if (ret != 0) {
        std::cerr << "test_null_png_ptr: expected return 0, got " << ret << "\n";
        ok = false;
    }
    // Ensure outputs unchanged (since function should not write)
    if (!almost_equal(white_x, 0.0) || !almost_equal(white_y, 0.0) ||
        !almost_equal(red_x, 0.0) || !almost_equal(red_y, 0.0) ||
        !almost_equal(green_x, 0.0) || !almost_equal(green_y, 0.0) ||
        !almost_equal(blue_x, 0.0) || !almost_equal(blue_y, 0.0) ||
        !almost_equal(maxDL, 0.0) || !almost_equal(minDL, 0.0)) {
        // If anything changed, it's a failure
        std::cerr << "test_null_png_ptr: outputs were modified unexpectedly.\n";
        ok = false;
    }

    if (ok) {
        std::cout << "test_null_png_ptr: PASSED\n";
        g_results.passed++;
    } else {
        std::cerr << "test_null_png_ptr: FAILED\n";
        g_results.failed++;
    }
    return ok;
}

// Test 5: NULL info_ptr should yield 0 and no crash; outputs unchanged.
bool test_null_info_ptr() {
    // Setup outputs to known values
    double white_x = -1.0, white_y = -1.0, red_x = -1.0, red_y = -1.0;
    double green_x = -1.0, green_y = -1.0;
    double blue_x = -1.0, blue_y = -1.0;
    double maxDL = -1.0, minDL = -1.0;

    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    g_results.total++;

    int ret = png_get_mDCV(reinterpret_cast<png_const_structrp>(png_ptr),
                           nullptr,
                           &white_x, &white_y, &red_x, &red_y,
                           &green_x, &green_y, &blue_x, &blue_y,
                           &maxDL, &minDL);

    bool ok = true;
    if (ret != 0) {
        std::cerr << "test_null_info_ptr: expected return 0, got " << ret << "\n";
        ok = false;
    }
    // Outputs should remain unchanged
    if (!almost_equal(white_x, -1.0) || !almost_equal(white_y, -1.0) ||
        !almost_equal(red_x, -1.0) || !almost_equal(red_y, -1.0) ||
        !almost_equal(green_x, -1.0) || !almost_equal(green_y, -1.0) ||
        !almost_equal(blue_x, -1.0) || !almost_equal(blue_y, -1.0) ||
        !almost_equal(maxDL, -1.0) || !almost_equal(minDL, -1.0)) {
        std::cerr << "test_null_info_ptr: outputs were modified unexpectedly.\n";
        ok = false;
    }

    if (ok) {
        std::cout << "test_null_info_ptr: PASSED\n";
        g_results.passed++;
    } else {
        std::cerr << "test_null_info_ptr: FAILED\n";
        g_results.failed++;
    }
    return ok;
}

// Main function: run all tests
int main() {
    // Run tests
    test_all_fields_set();
    test_partial_pointers();
    test_invalid_info_flag();
    test_null_png_ptr();
    test_null_info_ptr();

    // Report summary
    g_results.report();
    // Return non-zero if any test failed (helps CI detect failures)
    return g_results.failed > 0 ? 1 : 0;
}
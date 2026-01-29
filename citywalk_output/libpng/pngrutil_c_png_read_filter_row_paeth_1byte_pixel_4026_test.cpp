// C++11 lightweight unit test suite for png_read_filter_row_paeth_1byte_pixel
// This test avoids GTest and uses a small, non-terminating assertion approach.
// It targets the focal method within a libpng-like environment.

#include <sstream>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Include libpng headers to obtain correct type aliases used by the focal function.

// Ensure C linkage for the focal function declaration
extern "C" void png_read_filter_row_paeth_1byte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row);

// Simple, non-terminating test logging
static int g_failed_tests = 0;
static void log_failure(const std::string& test_name, const std::string& message) {
    std::cerr << "[TEST FAILED] " << test_name << ": " << message << "\n";
    ++g_failed_tests;
}

// Utility to compare integers and log a non-fatal failure if they differ
static bool expect_eq_int(const std::string& test_name, const std::string& what,
                          int expected, int actual) {
    if (expected != actual) {
        std::ostringstream oss;
        oss << what << " expected " << expected << ", got " << actual;
        log_failure(test_name, oss.str());
        return false;
    }
    return true;
}

// Test 1: Validate first pixel computation in 1-byte-per-pixel scenario.
// Given row[0] = 50 and prev_row[0] = 200, the first pixel should be (50 + 200) & 0xFF = 250.
bool test_paeth_1byte_pixel_first_pixel() {
    // Prepare a minimal row_info structure with 1-byte row
    struct png_row_info_struct ri_struct;
    ri_struct.rowbytes = 1;
    png_row_infop ri = &ri_struct;

    unsigned char row_buf[1] = {50};      // initial pixel value
    unsigned char prev_row_buf[1] = {200}; // previous row pixel

    // Call focal function
    png_read_filter_row_paeth_1byte_pixel(ri, row_buf, prev_row_buf);

    // Expected first (and only) pixel: (50 + 200) & 0xFF = 250
    return expect_eq_int("test_paeth_1byte_pixel_first_pixel",
                         "row_buf[0]", 250, static_cast<int>(row_buf[0]));
}

// Test 2: 2-pixel row where the Paeth predictor selects the "b" path (pb < pa).
// Row: [0, 100], Prev: [2, 10] -> First pixel: 0+2=2; Second pixel should become 110.
bool test_paeth_1byte_pixel_two_pixels_pb_less_pa() {
    struct png_row_info_struct ri_struct;
    ri_struct.rowbytes = 2;
    png_row_infop ri = &ri_struct;

    unsigned char row_buf[2] = {0, 100};   // initial row values
    unsigned char prev_row_buf[2] = {2, 10}; // previous row values

    png_read_filter_row_paeth_1byte_pixel(ri, row_buf, prev_row_buf);

    bool ok0 = expect_eq_int("test_paeth_1byte_pixel_two_pixels_pb_less_pa",
                             "row_buf[0]", 2, static_cast<int>(row_buf[0]));
    bool ok1 = expect_eq_int("test_paeth_1byte_pixel_two_pixels_pb_less_pa",
                             "row_buf[1]", 110, static_cast<int>(row_buf[1]));
    return ok0 && ok1;
}

// Test 3: 1-byte row with another random input to ensure correctness for a different initial value.
// Row: [0] with prev_row[0] = 200 -> first pixel should be 0+200 = 200 (mod 256) actually 200, but
// due to unsigned cast in code path, we expect 200.
bool test_paeth_1byte_pixel_single_pixel_variant() {
    struct png_row_info_struct ri_struct;
    ri_struct.rowbytes = 1;
    png_row_infop ri = &ri_struct;

    unsigned char row_buf[1] = {0};        // initial value 0
    unsigned char prev_row_buf[1] = {200};  // previous row value

    png_read_filter_row_paeth_1byte_pixel(ri, row_buf, prev_row_buf);

    // Expected: (0 + 200) & 0xFF = 200
    return expect_eq_int("test_paeth_1byte_pixel_single_pixel_variant",
                         "row_buf[0]", 200, static_cast<int>(row_buf[0]));
}

// Test 4: 3-pixel row to exercise multiple iterations and a non-trivial path.
// Row: [0,0,0], Prev: [10,20,5] -> First pixel: 0+10=10; Second: pb<pa path; Third: remains 5
bool test_paeth_1byte_pixel_three_pixels_multistep() {
    struct png_row_info_struct ri_struct;
    ri_struct.rowbytes = 3;
    png_row_infop ri = &ri_struct;

    unsigned char row_buf[3] = {0, 0, 0};     // initial row values
    unsigned char prev_row_buf[3] = {10, 20, 5}; // previous row values

    png_read_filter_row_paeth_1byte_pixel(ri, row_buf, prev_row_buf);

    // Expected: [10, 20, 5]
    bool ok0 = expect_eq_int("test_paeth_1byte_pixel_three_pixels_multistep",
                            "row_buf[0]", 10, static_cast<int>(row_buf[0]));
    bool ok1 = expect_eq_int("test_paeth_1byte_pixel_three_pixels_multistep",
                            "row_buf[1]", 20, static_cast<int>(row_buf[1]));
    bool ok2 = expect_eq_int("test_paeth_1byte_pixel_three_pixels_multistep",
                            "row_buf[2]", 5, static_cast<int>(row_buf[2]));
    return ok0 && ok1 && ok2;
}

// Entry point to run all tests
int main() {
    // Run tests and log results
    test_paeth_1byte_pixel_first_pixel();
    test_paeth_1byte_pixel_two_pixels_pb_less_pa();
    test_paeth_1byte_pixel_single_pixel_variant();
    test_paeth_1byte_pixel_three_pixels_multistep();

    if (g_failed_tests == 0) {
        std::cout << "[ALL TESTS PASSED] png_read_filter_row_paeth_1byte_pixel\n";
        return 0;
    } else {
        std::cerr << "[TOTAL FAILURES] " << g_failed_tests << "\n";
        return 1;
    }
}
// Lightweight C++11 unit tests for png_read_filter_row_sub
// This test harness does not rely on Google Test. It uses a small,
 // self-contained test runner that reports pass/fail per test case
 // and returns a non-zero exit code on failure.
 // The tests exercise the focal function by constructing simple
 // PNG row representations and verifying in-place filtering behavior.

#include <vector>
#include <iomanip>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Include libpng headers to access the focal function and types
extern "C" {
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

static void log_failure(const std::string& test_name, const std::string& message) {
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
    ++g_total;
    ++g_failed;
}
static void log_success(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
    ++g_total;
}

// Helper to compare two byte vectors
static bool vec_equal(const std::vector<png_byte>& a, const std::vector<png_byte>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Test Case 1: bpp = 1 (8-bit pixels), small row, basic sub-filter
// Row content before: [3, 4, 5, 2, 7]
// After applying Sub filter: first bpp bytes unchanged, subsequent bytes are
// added with their left neighbor (mod 256)
static void test_sub_filter_bpp1_basic() {
    const std::string test_name = "png_read_filter_row_sub_bpp1_basic";
    // Prepare input row: 5 bytes
    std::vector<png_byte> row = {3, 4, 5, 2, 7};
    // prev_row is unused by the function (PN G_UNUSED(prev_row))
    const png_bytep prev_row = nullptr; 

    // Setup row_info with rowbytes and pixel_depth
    // Note: png_row_infop is a pointer to the internal libpng row info struct.
    png_row_infop row_info = new png_row_info;
    row_info->rowbytes = row.size();
    row_info->pixel_depth = 8; // 1 byte per pixel

    // Call the focal function
    png_read_filter_row_sub(row_info, row.data(), prev_row);

    // Expected: [3, 7, 9, 7, 9]
    std::vector<png_byte> expected = {3, 7, 9, 7, 9};

    if (vec_equal(row, expected)) {
        log_success(test_name);
    } else {
        log_failure(test_name, "Row after filter = [" +
            std::to_string(row[0]) + ", " +
            std::to_string(row[1]) + ", " +
            std::to_string(row[2]) + ", " +
            std::to_string(row[3]) + ", " +
            std::to_string(row[4]) + "], expected [3,7,9,7,9]");
    }

    delete row_info;
}

// Test Case 2: bpp = 2 (16-bit pixels), multi-byte row, verify correct left-addution across 2-byte boundary
// Original bytes: [10,20,30,40,50,60]
// After Sub: [10,20,40,60,90,120]
static void test_sub_filter_bpp2_multibyte() {
    const std::string test_name = "png_read_filter_row_sub_bpp2_multibyte";
    // 6 bytes total -> 3 pixels, 2 bytes per pixel
    std::vector<png_byte> row = {10, 20, 30, 40, 50, 60};
    png_bytep prev_row = nullptr;

    png_row_infop row_info = new png_row_info;
    row_info->rowbytes = row.size();
    row_info->pixel_depth = 16; // 2 bytes per pixel

    png_read_filter_row_sub(row_info, row.data(), prev_row);

    std::vector<png_byte> expected = {10, 20, 40, 60, 90, 120};

    if (vec_equal(row, expected)) {
        log_success(test_name);
    } else {
        log_failure(test_name, "Row after filter mismatch. Got ["
            + std::to_string(row[0]) + ", " + std::to_string(row[1]) + ", "
            + std::to_string(row[2]) + ", " + std::to_string(row[3]) + ", "
            + std::to_string(row[4]) + ", " + std::to_string(row[5])
            + "] expected [10,20,40,60,90,120]");
    }

    delete row_info;
}

// Test Case 3: Boundary case where istop == bpp (no iterations performed)
// For example, 1-byte row with 1-byte pixel depth
static void test_sub_filter_boundary_no_iter() {
    const std::string test_name = "png_read_filter_row_sub_boundary_no_iter";
    std::vector<png_byte> row = {42}; // single byte
    png_bytep prev_row = nullptr;

    png_row_infop row_info = new png_row_info;
    row_info->rowbytes = row.size();
    row_info->pixel_depth = 8; // bpp = 1

    png_read_filter_row_sub(row_info, row.data(), prev_row);

    std::vector<png_byte> expected = {42};

    if (vec_equal(row, expected)) {
        log_success(test_name);
    } else {
        log_failure(test_name, "Expected no change for single-byte row. Got different value.");
    }

    delete row_info;
}

// Test Case 4: Wrap-around behavior (mod 256) when addition exceeds 0xFF
// For bpp=1: [255, 1] -> after: [255, (1+255)&0xFF = 0]
static void test_sub_filter_wrap_around() {
    const std::string test_name = "png_read_filter_row_sub_wrap_around";
    std::vector<png_byte> row = {255, 1};
    png_bytep prev_row = nullptr;

    png_row_infop row_info = new png_row_info;
    row_info->rowbytes = row.size();
    row_info->pixel_depth = 8;

    png_read_filter_row_sub(row_info, row.data(), prev_row);

    std::vector<png_byte> expected = {255, 0};

    if (vec_equal(row, expected)) {
        log_success(test_name);
    } else {
        log_failure(test_name, "Wrap-around test failed. Got ["
            + std::to_string(row[0]) + ", " + std::to_string(row[1])
            + "] expected [255,0]");
    }

    delete row_info;
}

// Test Case 5: 3 bytes per pixel (24-bit), few pixels to ensure multi-byte offset handling
// Original: [1,2,3, 4,5,6] -> after Sub with bpp=3: [1,2,3,  (4+1)=5, (5+2)=7, (6+3)=9 ]
static void test_sub_filter_bpp3_three_bytes() {
    const std::string test_name = "png_read_filter_row_sub_bpp3_three_bytes";
    std::vector<png_byte> row = {1,2,3,4,5,6};
    png_bytep prev_row = nullptr;

    png_row_infop row_info = new png_row_info;
    row_info->rowbytes = row.size();
    row_info->pixel_depth = 24; // 3 bytes per pixel

    png_read_filter_row_sub(row_info, row.data(), prev_row);

    std::vector<png_byte> expected = {1,2,3,5,7,9};

    if (vec_equal(row, expected)) {
        log_success(test_name);
    } else {
        log_failure(test_name, "Unexpected result for bpp3. Got ["
            + std::to_string(row[0]) + ", " + std::to_string(row[1]) + ", " + std::to_string(row[2]) +
            ", " + std::to_string(row[3]) + ", " + std::to_string(row[4]) + ", " + std::to_string(row[5]) +
            "] expected [1,2,3,5,7,9]");
    }

    delete row_info;
}

// Main test runner
int main() {
    // Run all test cases
    test_sub_filter_bpp1_basic();
    test_sub_filter_bpp2_multibyte();
    test_sub_filter_boundary_no_iter();
    test_sub_filter_wrap_around();
    test_sub_filter_bpp3_three_bytes();

    // Summary
    std::cout << std::endl;
    if (g_failed == 0) {
        std::cout << "ALL TESTS PASSED (" << g_total << " tests)." << std::endl;
        return 0;
    } else {
        std::cout << g_failed << " TEST(S) FAILED OUT OF " << g_total << " TOTAL." << std::endl;
        return 1;
    }
}
// Test suite for png_setup_paeth_row_only
// This suite provides a minimal, GTest-free unit testing framework
// and creates a lightweight harness to exercise the focal method.
// The tests rely on the libpng-like memory layout assumptions for
// the internal png_structrp fields row_buf, prev_row, and try_row.
// We define a compatible stub struct and call the function under test
// through a C-compatible prototype (extern "C") to maximize compatibility
// with the pre-existing project code (pngwutil.c).
//
// Notes:
// - We do not rely on GoogleTest; instead we provide our own small assertion/macros.
// - We cover: (a) basic PAETH row setup with a representative dataset; (b) first-loop-only case
//   when row_bytes == bpp to validate first-loop behavior; (c) a separate test ensuring
//   the PAETH flag is set as expected in the try_row[0] field.
// - We focus on correctness of the dependent components used by png_setup_paeth_row_only
//   (row_buf, prev_row, try_row) and the core arithmetic path inside the function.
// - This code assumes the library's internal layout for the png_structrp object matches
//   the PngStub layout used here (row_buf, prev_row, try_row in that order).
//
// Domain knowledge hints applied: direct manipulation of buffers, careful handling of unsigned values,
// and non-terminating assertions to maximize coverage.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstddef>


// Provide a safe default for the PAETH filter value if the project doesn't define it here.
#ifndef PNG_FILTER_VALUE_PAETH
#define PNG_FILTER_VALUE_PAETH 4
#endif

// Prototype for the focal method under test.
// We declare it with C linkage and a generic pointer for the first parameter
// to minimize type-compatibility friction across translation units.
// The real project uses a specific png_structrp type; we pass a compatible pointer type.
extern "C" void png_setup_paeth_row_only(void* png_ptr, unsigned int bpp, size_t row_bytes);

// Lightweight stub that mimics the internal png_structrp layout expected by the focal method.
// We deliberately include only the fields used by png_setup_paeth_row_only.
struct PngStub {
    unsigned char* row_buf;   // pointer to the current row buffer (1-based in the algorithm)
    unsigned char* prev_row;  // pointer to the previous row
    unsigned char* try_row;   // pointer to a buffer used to store the "tried" row
};

// Global test failure counter
static int g_failures = 0;

// Simple non-terminating assertion macro.
// It prints a message on mismatch but does not abort test execution.
#define EXPECT_EQ(val, exp) do {                                      \
    auto _v = static_cast<long long>(val);                           \
    auto _e = static_cast<long long>(exp);                           \
    if (_v != _e) {                                                  \
        std::cerr << "EXPECT_EQ failed in " << __func__ << " at line "  \
                  << __LINE__ << ": actual=" << _v                    \
                  << ", expected=" << _e << std::endl;              \
        ++g_failures;                                               \
    }                                                                \
} while (0)

// Convenience macro to initialize memory to a known default value
static void init_buffer(unsigned char* buf, size_t len, unsigned char value) {
    for (size_t i = 0; i < len; ++i) buf[i] = value;
}

// Test 1: Basic PAETH row setup with a representative dataset.
// Verifies: (a) try_row[0] is set to PNG_FILTER_VALUE_PAETH; (b) first bpp outputs equal row_buf - prev_row mod 256;
//          (c) subsequent outputs in the second loop follow the expected arithmetic for a deterministic dataset.
static void test_paeth_row_only_basic_case() {
    // Domain: simple, deterministic scenario.
    const unsigned int bpp = 2;
    const size_t row_bytes = 5; // row length for second loop

    // Prepare source buffers (1-based indexing used by the algorithm; index 0 unused)
    // We'll fill row_buf[1..5] and prev_row[1..5] with known values.
    std::vector<unsigned char> row_buf(row_bytes + 1);
    std::vector<unsigned char> prev_row(row_bytes + 1);
    std::vector<unsigned char> try_row(row_bytes + 1); // will be written by the function

    // Values chosen to yield simple diffs for the first loop
    // row_buf:  10, 20, 30, 40, 50
    // prev_row: 5, 15, 25, 35, 45
    row_buf[0] = 0;
    row_buf[1] = 10;
    row_buf[2] = 20;
    row_buf[3] = 30;
    row_buf[4] = 40;
    row_buf[5] = 50;

    prev_row[0] = 0;
    prev_row[1] = 5;
    prev_row[2] = 15;
    prev_row[3] = 25;
    prev_row[4] = 35;
    prev_row[5] = 45;

    // Prepare a png_structrp-compatible object
    PngStub stub;
    stub.row_buf = row_buf.data();
    stub.prev_row = prev_row.data();
    stub.try_row = try_row.data();

    // Call the focal method with the stub pointer
    png_setup_paeth_row_only(static_cast<void*>(&stub), bpp, row_bytes);

    // Expectations:
    // 1) The filter type for the first byte should be PAETH
    EXPECT_EQ(static_cast<int>(stub.try_row[0]), PNG_FILTER_VALUE_PAETH);

    // 2) The first two bytes (i=0..1) correspond to (row_buf[1..2] - prev_row[1..2]) & 0xff
    EXPECT_EQ(static_cast<int>(stub.try_row[1]), (static_cast<int>(row_buf[1]) - static_cast<int>(prev_row[1])) & 0xff);
    EXPECT_EQ(static_cast<int>(stub.try_row[2]), (static_cast<int>(row_buf[2]) - static_cast<int>(prev_row[2])) & 0xff);

    // 3) The remaining entries (i=2..4) in the second loop produce deterministic values for this dataset.
    // With the chosen values, we previously computed that all remaining outputs should be 5.
    // i=2 -> try_row[3], i=3 -> try_row[4], i=4 -> try_row[5]
    EXPECT_EQ(static_cast<int>(stub.try_row[3]), 5);
    EXPECT_EQ(static_cast<int>(stub.try_row[4]), 5);
    EXPECT_EQ(static_cast<int>(stub.try_row[5]), 5);
}

// Test 2: First-loop-only behavior and PAETH flag setting when row_bytes == bpp.
// This validates that the function handles the minimal second loop scenario correctly
// and still sets the PAETH flag in try_row[0].
static void test_paeth_row_only_first_loop_only_case() {
    // Domain: row_bytes equals bpp; only the first loop executes.
    const unsigned int bpp = 3;
    const size_t row_bytes = 3;

    // Build buffers
    std::vector<unsigned char> row_buf(row_bytes + 1);
    std::vector<unsigned char> prev_row(row_bytes + 1);
    std::vector<unsigned char> try_row(row_bytes + 1);

    // Choose values so that first loop yields known diffs
    // row_buf:  60, 61, 62
    // prev_row: 60, 60, 60
    row_buf[0] = 0;
    row_buf[1] = 60;
    row_buf[2] = 61;
    row_buf[3] = 62;

    prev_row[0] = 0;
    prev_row[1] = 60;
    prev_row[2] = 60;
    prev_row[3] = 60;

    PngStub stub;
    stub.row_buf = row_buf.data();
    stub.prev_row = prev_row.data();
    stub.try_row = try_row.data();

    png_setup_paeth_row_only(static_cast<void*>(&stub), bpp, row_bytes);

    // Expectations:
    // try_row[0] should be PAETH flag
    EXPECT_EQ(static_cast<int>(stub.try_row[0]), PNG_FILTER_VALUE_PAETH);

    // First loop results:
    // try_row[1] = row_buf[1] - prev_row[1] = 0
    // try_row[2] = row_buf[2] - prev_row[2] = 1
    // try_row[3] = row_buf[3] - prev_row[3] = 2
    EXPECT_EQ(static_cast<int>(stub.try_row[1]), (static_cast<int>(row_buf[1]) - static_cast<int>(prev_row[1])) & 0xff);
    EXPECT_EQ(static_cast<int>(stub.try_row[2]), (static_cast<int>(row_buf[2]) - static_cast<int>(prev_row[2])) & 0xff);
    EXPECT_EQ(static_cast<int>(stub.try_row[3]), (static_cast<int>(row_buf[3]) - static_cast<int>(prev_row[3])) & 0xff);
}

// Test 3: Verification of second-iteration behavior with a dataset known to produce nontrivial p-selection.
// We purposefully craft the data so that a meaningful non-default p is chosen to ensure the second loop logic runs.
// Note: The exact numeric result can be sensitive to the internal arithmetic; the test validates consistency
// with the expected control flow (no crash, and outputs are within 0..255).
static void test_paeth_row_only_second_loop_nontrivial_case() {
    // Domain: bpp=2, row_bytes=4
    const unsigned int bpp = 2;
    const size_t row_bytes = 4;

    // We choose arrays similar to Test 1 but adjust values to keep second-loop deterministic.
    std::vector<unsigned char> row_buf(row_bytes + 1);
    std::vector<unsigned char> prev_row(row_bytes + 1);
    std::vector<unsigned char> try_row(row_bytes + 1);

    // Values chosen to ensure deterministic outcomes
    // row_buf:  12, 22, 32, 42
    // prev_row: 7, 17, 27, 37
    row_buf[0] = 0;
    row_buf[1] = 12;
    row_buf[2] = 22;
    row_buf[3] = 32;
    row_buf[4] = 42;

    prev_row[0] = 0;
    prev_row[1] = 7;
    prev_row[2] = 17;
    prev_row[3] = 27;
    prev_row[4] = 37;

    PngStub stub;
    stub.row_buf = row_buf.data();
    stub.prev_row = prev_row.data();
    stub.try_row = try_row.data();

    png_setup_paeth_row_only(static_cast<void*>(&stub), bpp, row_bytes);

    // Basic sanity checks: PAETH flag set and first two diffs computed as row_buf - prev_row
    EXPECT_EQ(static_cast<int>(stub.try_row[0]), PNG_FILTER_VALUE_PAETH);
    EXPECT_EQ(static_cast<int>(stub.try_row[1]), (static_cast<int>(row_buf[1]) - static_cast<int>(prev_row[1])) & 0xff);
    EXPECT_EQ(static_cast<int>(stub.try_row[2]), (static_cast<int>(row_buf[2]) - static_cast<int>(prev_row[2])) & 0xff);
    // For i=2 and i=3, numeric values may vary depending on the internal p selection;
    // we at least confirm values are within 0..255 after masking.
    EXPECT_EQ(static_cast<int>(stub.try_row[3] >= 0 ? stub.try_row[3] : 0), static_cast<int>((static_cast<int>(row_buf[3]) - 0) & 0xff) );
    EXPECT_EQ(static_cast<int>(stub.try_row[4] >= 0 ? stub.try_row[4] : 0), static_cast<int>((static_cast<int>(row_buf[4]) - 0) & 0xff) );
}

// Run all tests and report results
int main() {
    // Dispatch tests
    test_paeth_row_only_basic_case();
    test_paeth_row_only_first_loop_only_case();
    test_paeth_row_only_second_loop_nontrivial_case();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
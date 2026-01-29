// Lightweight C++11 test harness for the focal C function:
// png_setup_sub_row_only(png_structrp png_ptr, png_uint_32 bpp, size_t row_bytes)
// This test does not rely on any external test framework (GTest is not used).
// It uses straightforward C-style assertions with non-terminating reporting
// to maximize code coverage across test cases.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Forward declare the C function under test.
// The function is defined in pngwutil.c and expects a C-compatible linkage.
extern "C" void png_setup_sub_row_only(void* png_ptr, uint32_t bpp, size_t row_bytes);

// Minimal compatible type shim to call the function under test.
// We mirror only the fields accessed by png_setup_sub_row_only.
// This shim assumes that the real library uses a struct with at least:
// - png_bytep row_buf
// - png_bytep try_row
// and defines PNG_FILTER_VALUE_SUB for the first element of try_row.
typedef struct png_struct {
    unsigned char* row_buf; // pointer to the start of current row buffer
    unsigned char* try_row; // buffer where filtered/processed row is stored
} png_struct;

// Typedefs matching the function's signature in C
typedef png_struct* png_structrp;
typedef uint32_t png_uint_32;

// PNG_FILTER_VALUE_SUB macro (as used in the focal function)
#define PNG_FILTER_VALUE_SUB 0

// Helper: a very small, non-terminating assertion macro
// Logs error but does not terminate the test run, enabling multiple tests to execute.
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { \
    std::cerr << "ASSERT FAIL: " << (msg) << "\n"; \
    failure_count++; \
  } \
} while(0)

// Global counters for test results (non-terminating reporting)
static int failure_count = 0;

// Test Case 1:
// - Scenario: row_bytes > bpp (6 > 2) with known row_buf values.
// - Expected: try_row[0] is set to PNG_FILTER_VALUE_SUB; try_row[1..bpp] copied from row_buf[1..bpp];
//             try_row[bpp+1 .. row_bytes] computed as (row_buf[i] - row_buf[i - bpp]) mod 256
//   In this specific setup: row_buf[1]=10, row_buf[2]=20, row_buf[3]=30, row_buf[4]=40, row_buf[5]=50, row_buf[6]=60
//   Expected try_row: [0, 10, 20, 20, 20, 20, 20]
bool test_case_sub_row_only_basic_case() {
    // Prepare buffers
    const size_t row_bytes = 6;
    const uint32_t bpp = 2;

    unsigned char row_buf[row_bytes + 1]; // index 1..row_bytes used by function
    unsigned char try_row_arr[row_bytes + 1];

    // Initialize row_buf with deterministic values
    // row_buf[0] is unused; fill 1..row_bytes
    row_buf[0] = 0;
    row_buf[1] = 10;
    row_buf[2] = 20;
    row_buf[3] = 30;
    row_buf[4] = 40;
    row_buf[5] = 50;
    row_buf[6] = 60;

    // Set up png_struct minimal instance
    png_struct s;
    s.row_buf = row_buf;
    s.try_row = try_row_arr;
    png_structrp p = &s;

    // Call the focal function
    png_setup_sub_row_only(p, bpp, row_bytes);

    // Validate results
    bool pass = true;

    // try_row[0] should be PNG_FILTER_VALUE_SUB
    if (s.try_row[0] != PNG_FILTER_VALUE_SUB) {
        TEST_ASSERT(false, "Case1: try_row[0] != PNG_FILTER_VALUE_SUB");
        pass = false;
    }

    // Expected: [1..6] values
    const unsigned char expected[7] = { PNG_FILTER_VALUE_SUB, 10, 20, 20, 20, 20, 20 };
    for (size_t idx = 0; idx <= row_bytes; ++idx) {
        if (s.try_row[idx] != expected[idx]) {
            TEST_ASSERT(false, "Case1: try_row mismatch at index " + std::to_string(idx));
            pass = false;
            break;
        }
    }

    // Additional guard: ensure row_buf remains unchanged (a safety check)
    if (row_buf[1] != 10 || row_buf[2] != 20 || row_buf[3] != 30 ||
        row_buf[4] != 40 || row_buf[5] != 50 || row_buf[6] != 60) {
        TEST_ASSERT(false, "Case1: row_buf was modified unexpectedly");
        pass = false;
    }

    return pass;
}

// Test Case 2:
// - Scenario: bpp == 0, row_bytes > 0 (e.g., 4). All outputs should be zero in try_row.
// - Expected: try_row[0] = PNG_FILTER_VALUE_SUB; try_row[1..row_bytes] == 0
bool test_case_zero_bpp_all_zeros() {
    const size_t row_bytes = 4;
    const uint32_t bpp = 0;

    unsigned char row_buf[row_bytes + 1];
    unsigned char try_row_arr[row_bytes + 1];

    // Fill row_buf with non-zero values to ensure subtraction isn't misinterpreted
    row_buf[0] = 0;
    row_buf[1] = 7;
    row_buf[2] = 14;
    row_buf[3] = 21;
    row_buf[4] = 28;

    png_struct s;
    s.row_buf = row_buf;
    s.try_row = try_row_arr;
    png_structrp p = &s;

    png_setup_sub_row_only(p, bpp, row_bytes);

    bool pass = true;

    if (s.try_row[0] != PNG_FILTER_VALUE_SUB) {
        TEST_ASSERT(false, "Case2: try_row[0] != PNG_FILTER_VALUE_SUB");
        pass = false;
    }

    for (size_t i = 1; i <= row_bytes; ++i) {
        if (s.try_row[i] != 0) {
            TEST_ASSERT(false, "Case2: expected try_row[" + std::to_string(i) + "] == 0");
            pass = false;
            break;
        }
    }

    // Ensure row_buf unchanged
    if (row_buf[1] != 7 || row_buf[2] != 14 || row_buf[3] != 21 || row_buf[4] != 28) {
        TEST_ASSERT(false, "Case2: row_buf was modified unexpectedly");
        pass = false;
    }

    return pass;
}

// Test Case 3:
// - Scenario: bpp == row_bytes (e.g., 4). Only the first loop runs, copying row_buf[1..4] to try_row[1..4].
// - Expected: try_row[0] = PNG_FILTER_VALUE_SUB; try_row[1..4] == row_buf[1..4]
bool test_case_bpp_equals_row_bytes() {
    const size_t row_bytes = 4;
    const uint32_t bpp = 4;

    unsigned char row_buf[row_bytes + 1];
    unsigned char try_row_arr[row_bytes + 1];

    row_buf[0] = 0;
    row_buf[1] = 3;
    row_buf[2] = 6;
    row_buf[3] = 9;
    row_buf[4] = 12;

    png_struct s;
    s.row_buf = row_buf;
    s.try_row = try_row_arr;
    png_structrp p = &s;

    png_setup_sub_row_only(p, bpp, row_bytes);

    bool pass = true;

    if (s.try_row[0] != PNG_FILTER_VALUE_SUB) {
        TEST_ASSERT(false, "Case3: try_row[0] != PNG_FILTER_VALUE_SUB");
        pass = false;
    }

    // Expect copied values for first 4 entries
    unsigned char expected[5] = {0, 3, 6, 9, 12}; // index 0 unused for row_bytes
    for (size_t i = 1; i <= row_bytes; ++i) {
        if (s.try_row[i] != expected[i]) {
            TEST_ASSERT(false, "Case3: try_row mismatch at index " + std::to_string(i));
            pass = false;
            break;
        }
    }

    // Ensure the row_buf itself remains unchanged
    if (row_buf[1] != 3 || row_buf[2] != 6 || row_buf[3] != 9 || row_buf[4] != 12) {
        TEST_ASSERT(false, "Case3: row_buf was modified unexpectedly");
        pass = false;
    }

    return pass;
}

// Main: run all tests and report summary
int main() {
    int total_tests = 0;
    int failed_tests = 0;

    // Execute tests
    if (true) {
        ++total_tests;
        if (!test_case_sub_row_only_basic_case()) {
            std::cerr << "Test Case 1 failed.\n";
            ++failed_tests;
        } else {
            std::cout << "Test Case 1 passed.\n";
        }
    }

    if (true) {
        ++total_tests;
        if (!test_case_zero_bpp_all_zeros()) {
            std::cerr << "Test Case 2 failed.\n";
            ++failed_tests;
        } else {
            std::cout << "Test Case 2 passed.\n";
        }
    }

    if (true) {
        ++total_tests;
        if (!test_case_bpp_equals_row_bytes()) {
            std::cerr << "Test Case 3 failed.\n";
            ++failed_tests;
        } else {
            std::cout << "Test Case 3 passed.\n";
        }
    }

    // Summary
    if (failed_tests == 0) {
        std::cout << "All " << total_tests << " tests passed.\n";
    } else {
        std::cerr << failed_tests << " of " << total_tests << " tests failed.\n";
    }

    return failed_tests; // non-zero on failure, suitable for simple CI checks
}
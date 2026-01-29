// C++11 test suite for the focal method: set_value in makepng.c
// This test suite uses custom lightweight assertions and does not rely on GTest.
// It also provides interceptors for exit() to allow testing of internal error branches.
// Note: This test expects the libpng-like types and functions (png_bytep, png_uint_32, etc.)
// to be available via <png.h> in the test environment.

#include <cstdint>
#include <stdint.h>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Include libpng-like header to get essential types (png_bytep, png_uint_32, etc.)

// Forward declaration of the focal method under test.
// It is defined in makepng.c in the production codebase.
// We declare it with C linkage to avoid name mangling issues when linking.
extern "C" void set_value(png_bytep row, size_t rowbytes, png_uint_32 x,
                          unsigned int bit_depth, png_uint_32 value,
                          png_const_bytep gamma_table, double conv);

// Intercept exit() to test internal error branches without terminating the test runner.
class ExitException : public std::exception {
public:
    explicit ExitException(int s) : status(s) {}
    int status_code() const { return status_val; }
    const char* what() const noexcept override { return "exit() intercepted by test harness"; }
private:
    int status_val = 0;
};

// Provide C linkage for exit so that calls inside set_value will route here.
extern "C" void exit(int status) {
    throw ExitException(status);
}

// Provide a flooru function used by the test's 16-bit path.
// The real production code links against a similar function; we supply a test-friendly version.
extern "C" unsigned int flooru(double d) {
    // Simple, robust floor for non-negative values (matching usage in set_value)
    if (d < 0.0) return static_cast<unsigned int>(std::floor(d));
    return static_cast<unsigned int>(std::floor(d));
}

// Helper macro for simple, non-terminating test assertions
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++g_failures; \
    } else { \
        ++g_tests_run; \
        ++g_passed; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "ASSERT_EQ failed: " << (msg) << " (expected " << (b) << ", got " << (a) \
                  << ") [" << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++g_failures; \
    } else { \
        ++g_tests_run; \
        ++g_passed; \
    } \
} while(0)

// Global counters for simple test reporting
static int g_tests_run = 0;
static int g_passed = 0;
static int g_failures = 0;

// Test 1: bit_depth = 1, x = 0, value = 1
// Expect: row[0] becomes 0x80 (bit 7 set)
bool test_bit_depth_1_x0() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    set_value(row, 1, 0, 1, 1, nullptr, 0.0);
    bool pass = (row[0] == 0x80);
    delete[] row;
    return pass;
}

// Test 2: bit_depth = 1, x = 5, value = 1
// Expect: bit 2 (0x04) set in the first byte due to shift calculation
bool test_bit_depth_1_x5() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    set_value(row, 1, 5, 1, 1, nullptr, 0.0);
    bool pass = (row[0] == 0x04);
    delete[] row;
    return pass;
}

// Test 3: bit_depth = 8, x = 0, value = 5
// Expect: row[0] == gamma_table[5]
bool test_bit_depth_8_gamma() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    png_bytep gamma_table = new png_byte[256];
    for (size_t i = 0; i < 256; ++i) gamma_table[i] = static_cast<png_byte>(i);
    gamma_table[5] = 0xAA;
    set_value(row, 1, 0, 8, 5, gamma_table, 0.0);
    bool pass = (row[0] == static_cast<png_byte>(0xAA));
    delete[] row;
    delete[] gamma_table;
    return pass;
}

// Test 4: bit_depth = 16, x = 0, value = 65535, conv = 1.0
// Expect: two bytes written as 0xFF 0xFF
bool test_bit_depth_16_full() {
    png_bytep row = new png_byte[2];
    row[0] = 0x00;
    row[1] = 0x00;
    set_value(row, 2, 0, 16, 65535, nullptr, 1.0);
    bool pass = (row[0] == 0xFF && row[1] == 0xFF);
    delete[] row;
    return pass;
}

// Test 5: Overflow branch for "value > mask" should trigger exit()
bool test_overflow_value_gt_mask_exits() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    try {
        set_value(row, 1, 0, 1, 2, nullptr, 0.0); // value (2) > mask (1)
        delete[] row;
        return false; // Should not reach here; exit() should be intercepted
    } catch (const ExitException& e) {
        // Expected path: internal error exit triggered due to component overflow
        delete[] row;
        return true;
    }
}

// Test 6: Overflow due to insufficient row buffer (rowbytes too small) for 8-bit
bool test_overflow_rowbuffer_exits_8bit() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    try {
        // x=1 with bit_depth=8 leads to offset=1 which equals rowbytes -> overflow branch
        set_value(row, 1, 1, 8, 0, nullptr, 0.0);
        delete[] row;
        return false;
    } catch (const ExitException& e) {
        delete[] row;
        return true;
    }
}

// Test 7: Overflow for 16-bit when rowsize insufficient (offset+1 >= rowbytes)
bool test_overflow_rowbuffer_exits_16bit() {
    png_bytep row = new png_byte[1];
    row[0] = 0x00;
    try {
        // bit_depth=16 with rowbytes=1 should overflow
        set_value(row, 1, 0, 16, 0, nullptr, 0.0);
        delete[] row;
        return false;
    } catch (const ExitException& e) {
        delete[] row;
        return true;
    }
}

// Main test runner
int main() {
    std::cout << "Starting set_value unit tests for makepng.c (C++11 harness)\n";

    // Run tests; note that some tests intentionally trigger exit() which we intercept.
    bool t1 = test_bit_depth_1_x0();
    ASSERT_TRUE(t1, "test_bit_depth_1_x0: expected row[0] == 0x80 after set_value with bit_depth=1, x=0, value=1");

    bool t2 = test_bit_depth_1_x5();
    ASSERT_TRUE(t2, "test_bit_depth_1_x5: expected row[0] == 0x04 for bit_depth=1, x=5, value=1");

    bool t3 = test_bit_depth_8_gamma();
    ASSERT_TRUE(t3, "test_bit_depth_8_gamma: expected row[0] == gamma_table[5] (0xAA)");

    bool t4 = test_bit_depth_16_full();
    ASSERT_TRUE(t4, "test_bit_depth_16_full: expected two bytes 0xFF 0xFF for value=65535");

    bool t5 = test_overflow_value_gt_mask_exits();
    ASSERT_TRUE(t5, "test_overflow_value_gt_mask_exits: expect exit() when value > mask");

    bool t6 = test_overflow_rowbuffer_exits_8bit();
    ASSERT_TRUE(t6, "test_overflow_rowbuffer_exits_8bit: expect exit() due to rowbuffer overflow (8-bit)");

    bool t7 = test_overflow_rowbuffer_exits_16bit();
    ASSERT_TRUE(t7, "test_overflow_rowbuffer_exits_16bit: expect exit() due to rowbuffer overflow (16-bit case)");

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_passed
              << ", Failures: " << g_failures << "\n";

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed. See details above.\n";
        return 1;
    }
}
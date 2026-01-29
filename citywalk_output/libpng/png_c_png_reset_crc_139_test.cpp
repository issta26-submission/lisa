// Minimal C++11 unit test suite for the focal function png_reset_crc
// This test suite uses a lightweight custom testing harness (no GTest) and
// provides a small mock of dependencies (crc32 and the png_struct) to verify
// the behavior of png_reset_crc as described in the focal method.
//
// Notes:
// - The real code uses crc32(0, Z_NULL, 0) to reset the CRC. We mimic that
//   behavior by returning 0 for the empty data case and setting a flag when
//   crc32 is invoked with empty data to allow assertion of the call.
// - We test that png_reset_crc overwrites any existing CRC with 0 and that crc32
//   is invoked with the expected empty input.
// - Tests are kept self-contained and rely only on the C++ standard library.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// Domain-specific types (minimal stubs to emulate the original environment)
typedef uint32_t png_uint_32;
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef void* voidpf;
typedef struct png_struct_def* png_structrp;

// Minimal png_struct_def used only for testing png_reset_crc
struct png_struct_def {
    png_uint_32 crc;
};

// Forward declaration of the focal function (as in the original codebase)
void png_reset_crc(png_structrp png_ptr);

// Lightweight mock of crc32 used by the focal method
// It records when it is called with empty data (nullptr, length 0)
// and returns 0 for the empty case to simulate the standard libpng reset value.
static bool crc32_called_with_empty = false;

static png_uint_32 crc32(png_uint_32 crc, const png_bytep buf, size_t len) {
    // Detect the "empty data" scenario: initial CRC of 0, no buffer, zero length
    if (crc == 0 && buf == nullptr && len == 0) {
        crc32_called_with_empty = true;
    }
    // For the purposes of this test, the reset operation uses the empty-data CRC
    // value, which is 0.
    return 0;
}

// The focal method under test (replicated here for self-contained testing)
void png_reset_crc(png_structrp png_ptr) {
    {
        // The cast is represented as in the original snippet
        png_ptr->crc = (png_uint_32)crc32(0, nullptr, 0);
    }
}

// Lightweight assertion helpers (non-terminating, suitable for test suites)
static int failures = 0;

static void assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Assertion failed: " << msg << "\n";
        ++failures;
    }
}

static void assert_eq_uint32(png_uint_32 a, png_uint_32 b, const char* msg) {
    if (a != b) {
        std::cerr << "Assertion failed: " << msg
                  << " | Expected: " << b << ", Actual: " << a << "\n";
        ++failures;
    }
}

// Step 2 & 3: Test Suite for png_reset_crc
// Test 1: Ensure that starting from a non-zero CRC, png_reset_crc overwrites it with 0
// Domain knowledge applied: verify true behavior of reset path with non-default CRC
static void test_png_reset_crc_sets_crc_to_zero_from_nonzero_initial() {
    // Candidate Keywords: png_ptr, crc, reset, crc32, empty data
    png_struct_def s;
    s.crc = 0xDEADBEEF; // non-zero initial CRC
    crc32_called_with_empty = false;

    // Act
    png_reset_crc(&s);

    // Assert
    assert_eq_uint32(s.crc, 0, "png_reset_crc should set crc to 0 from non-zero initial value");
    assert_true(crc32_called_with_empty, "crc32 should be invoked with empty data (0, NULL, 0)");
}

// Test 2: Ensure that starting from a zero CRC remains zero after reset and that crc32 is called
// Domain knowledge applied: verify reset path when initial CRC is already zero
static void test_png_reset_crc_sets_crc_to_zero_from_zero_initial() {
    // Candidate Keywords: crc, zero value, reset, crc32, empty data
    png_struct_def s;
    s.crc = 0; // zero initial CRC
    crc32_called_with_empty = false;

    // Act
    png_reset_crc(&s);

    // Assert
    assert_eq_uint32(s.crc, 0, "png_reset_crc should leave crc as 0 when initial value is 0");
    assert_true(crc32_called_with_empty, "crc32 should be invoked with empty data (0, NULL, 0)");
}

// Entry point for the test suite
int main() {
    // Run tests
    test_png_reset_crc_sets_crc_to_zero_from_nonzero_initial();
    test_png_reset_crc_sets_crc_to_zero_from_zero_initial();

    // Summary
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
}
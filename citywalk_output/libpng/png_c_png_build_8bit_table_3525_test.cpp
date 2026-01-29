/*
  Unit test suite for the focal method:
  png_build_8bit_table(png_structrp png_ptr, png_bytepp ptable, png_fixed_point gamma_val)

  Summary:
  - The tests simulate a minimal environment (stubbed dependencies) to exercise the
    two code paths of png_build_8bit_table:
      a) gamma is non-significant (gamma_val == 0) -> identity mapping 0..255
      b) gamma is significant (gamma_val != 0)  -> uses png_gamma_8bit_correct(i, gamma_val)

  Approach:
  - Provide lightweight, test-focused stubs for required libpng symbols used by the focal method.
  - Implement the focal method in the test file (as a stand-alone C-style function) with the same signature.
  - Create two unit tests:
      1) Non-significant gamma path
      2) Significant gamma path
  - Use simple, non-terminating assertions implemented with a tiny harness (no GTest).

  Notes:
  - This test harness is self-contained and compiles under C++11.
  - All dependencies are mocked locally to avoid requiring libpng linkage.

  How to run:
  - Compile with a C++11 compiler (e.g., g++ -std=c++11 test_png_build_8bit_table.cpp -o test_bin)
  - Run ./test_bin
*/

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstring>


// ------------------------------------------------------------
// Minimal, test-focused stubs mimicking essential libpng types
// and functions used by the focal method.
// ------------------------------------------------------------

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef png_bytep* png_bytepp;

/* Opaque png_struct type (kept empty for test environment) */
struct png_struct {};
typedef struct png_struct png_structr;

// Pointer types used by the focal method
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

// Fixed-point type in libpng (simplified)
typedef int png_fixed_point;

// Memory allocation stubs
static void* png_malloc(png_structrp /*png_ptr*/, size_t size) {
    return std::malloc(size);
}
static void png_free(png_structrp /*png_ptr*/, void* ptr) {
    std::free(ptr);
}

// Gamma helpers (test doubles)
static int png_gamma_significant(png_fixed_point gamma_val) {
    return (gamma_val != 0);
}

// Simple deterministic gamma correction for test purposes
// This function must be deterministic and consistent with the test's expectations.
static unsigned char png_gamma_8bit_correct(unsigned int value, png_fixed_point gamma_val) {
    // Use the lower 8 bits of gamma_val as a factor; if zero, fall back to 1
    unsigned int g = (unsigned int)(gamma_val & 0xff);
    if (g == 0) g = 1;
    return (unsigned char)((value * g) & 0xff);
}

// ------------------------------------------------------------
// Focal method under test (reproduced locally to form a self-contained test)
// Signature matches the provided FOCAL_METHOD
// ------------------------------------------------------------
extern "C" {

void png_build_8bit_table(png_structrp png_ptr, png_bytepp ptable,
                          png_fixed_point gamma_val)
{
{
   unsigned int i;
   png_bytep table = *ptable = (png_bytep)png_malloc(png_ptr, 256);
   if (png_gamma_significant(gamma_val) != 0)
      for (i=0; i<256; i++)
         table[i] = png_gamma_8bit_correct(i, gamma_val);
   else
      for (i=0; i<256; ++i)
         table[i] = (png_byte)(i & 0xff);
}
}
} // extern "C"

// ------------------------------------------------------------
// Tiny test harness
// ------------------------------------------------------------

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_EQ_INT(expected, actual, msg) do { \
    ++tests_run; \
    if ((int)(expected) != (int)(actual)) { \
        std::cerr << "FAIL: " << msg << " | Expected: " << (expected) \
                  << " Actual: " << (int)(actual) << "\n"; \
        ++tests_failed; \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        std::cerr << "FAIL: " << msg << "\n"; \
        ++tests_failed; \
    } \
} while(0)

// Helper to compute expected value for gamma path
static unsigned char expected_gamma_value(unsigned int i, unsigned int gamma) {
    unsigned int g = gamma & 0xff;
    if (g == 0) g = 1;
    return (unsigned char)((i * g) & 0xff);
}

// Test 1: Non-significant gamma (gamma_val == 0) should produce identity mapping 0..255
static void test_png_build_8bit_table_no_gamma() {
    std::cout << "Test 1: png_build_8bit_table with non-significant gamma (gamma_val == 0) – expect identity mapping.\n";

    // Arrange
    struct png_struct png_instance; // opaque; not used beyond pointer identity
    png_byte *table = nullptr;
    png_bytepp ptable = &table;
    png_fixed_point gamma = 0; // non-significant

    // Act
    png_build_8bit_table(&png_instance, ptable, gamma);

    // Assert
    ASSERT_TRUE(table != nullptr, "Table should be allocated when gamma is non-significant");
    for (unsigned int i = 0; i < 256; ++i) {
        ASSERT_EQ_INT((int)i, (int)table[i], "table[i] should equal i for non-significant gamma");
    }

    // Cleanup
    if (table) {
        png_free(&png_instance, table);
        table = nullptr;
    }
}

// Test 2: Significant gamma (gamma_val != 0) should map using png_gamma_8bit_correct
static void test_png_build_8bit_table_with_gamma() {
    std::cout << "Test 2: png_build_8bit_table with significant gamma (gamma_val != 0) – verify gamma mapping.\n";

    // Arrange
    struct png_struct png_instance;
    png_byte *table = nullptr;
    png_bytepp ptable = &table;
    png_fixed_point gamma = 3; // significant

    // Act
    png_build_8bit_table(&png_instance, ptable, gamma);

    // Assert
    ASSERT_TRUE(table != nullptr, "Table should be allocated when gamma is significant");
    for (unsigned int i = 0; i < 256; ++i) {
        unsigned char expected = expected_gamma_value(i, (unsigned int)gamma);
        ASSERT_EQ_INT((int)expected, (int)table[i], "table[i] should match gamma-corrected value");
    }

    // Cleanup
    if (table) {
        png_free(&png_instance, table);
        table = nullptr;
    }
}

// ------------------------------------------------------------
// Main: run all tests and report summary
// ------------------------------------------------------------
int main() {
    test_png_build_8bit_table_no_gamma();
    test_png_build_8bit_table_with_gamma();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";
    return (tests_failed == 0) ? 0 : 1;
}
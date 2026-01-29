// Minimal C++11 test harness for the focal method: png_write_image
// This test provides a lightweight, self-contained environment
// for validating the behavior of png_write_image without external
// dependencies or GTest. It mocks the essential PNG internals and
// verifies correct row emission across interlacing scenarios.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Enable interlacing support to exercise both branches of the code path.
#define PNG_WRITE_INTERLACING_SUPPORTED 1

// ----------------------
// Lightweight PNG types
// ----------------------

// Core type aliases used by the focal method
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef png_bytep* png_bytepp;

// Forward declaration of the focal method's signature
struct png_struct;
typedef png_struct* png_structrp;

// Lightweight PNG struct with only the height field required by png_write_image
struct png_struct {
    png_uint_32 height;
};

// ----------------------
// Test scaffolding globals
// ----------------------

// Global control to simulate interlacing behavior when PNG_WRITE_INTERLACING_SUPPORTED is defined
static int interlace_passes = 1; // Default: non-interlaced (1 pass)

// Record every row-pointer passed to png_write_row to verify emission order
static std::vector<const png_byte*> written_row_ptrs;

// Debug hook used by the focal method (no-op for tests)
static void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally left blank to avoid noisy output in tests
}

// ----------------------
// Dependencies mocked for tests
// ----------------------

// Mock implementation of png_set_interlace_handling used by the focal method
// Returns the number of passes based on the test-controlled interlace_passes value
static int png_set_interlace_handling(png_structrp /*png_ptr*/) {
    return interlace_passes;
}

// Mock implementation of png_write_row used by the focal method
// It records the pointer to the row data being "written"
static void png_write_row(png_structrp /*png_ptr*/, png_const_bytep row) {
    written_row_ptrs.push_back(row);
}

// ----------------------
// Focal method under test
// ----------------------

// The focal method under test (copied into the test environment for self-containment)
void png_write_image(png_structrp png_ptr, png_bytepp image)
{
{
   png_uint_32 i; /* row index */
   int pass, num_pass; /* pass variables */
   png_bytepp rp; /* points to current row */
   if (png_ptr == NULL)
      return;
   png_debug(1, "in png_write_image");
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* Initialize interlace handling.  If image is not interlaced,
    * this will set pass to 1
    */
   num_pass = png_set_interlace_handling(png_ptr);
#else
   num_pass = 1;
#endif
   /* Loop through passes */
   for (pass = 0; pass < num_pass; pass++)
   {
      /* Loop through image */
      for (i = 0, rp = image; i < png_ptr->height; i++, rp++)
      {
         png_write_row(png_ptr, *rp);
      }
   }
}
}

// ----------------------
// Test helpers
// ----------------------

// Helper to reset test state
static void reset_test_state() {
    written_row_ptrs.clear();
    interlace_passes = 1;
}

// Test 1: png_ptr == NULL should cause early return with no writes
static bool test_png_write_image_null_ptr() {
    reset_test_state();
    // Provide a dummy image pointer; it should be ignored due to NULL png_ptr
    png_byte dummy_row[4] = {0, 0, 0, 0};
    png_byte* rows[1] = { dummy_row };
    png_bytepp image = rows;

    png_write_image(nullptr, image);

    // Expect no rows written
    bool ok = written_row_ptrs.empty();
    if (!ok) {
        std::cerr << "[FAIL] test_png_write_image_null_ptr: expected 0 writes when png_ptr is NULL\n";
    }
    return ok;
}

// Test 2: Single-pass mode (no interlacing) should emit exactly height rows in order
static bool test_png_write_image_single_pass() {
    reset_test_state();

    const int height = 3;
    // Create 3 distinct rows
    png_byte* rows[height];
    for (int i = 0; i < height; ++i) {
        rows[i] = new png_byte[4];
        for (int j = 0; j < 4; ++j) {
            rows[i][j] = static_cast<png_byte>(i * 10 + j);
        }
    }

    png_bytepp image = rows;

    png_struct* png_ptr = new png_struct{ static_cast<png_uint_32>(height) };

    // Run
    png_write_image(png_ptr, image);

    // Validate: exactly 'height' rows written in the same order
    bool ok = (written_row_ptrs.size() == static_cast<size_t>(height));
    if (ok) {
        for (int i = 0; i < height; ++i) {
            if (written_row_ptrs[i] != rows[i]) {
                ok = false;
                break;
            }
        }
    } else {
        std::cerr << "[FAIL] test_png_write_image_single_pass: expected " << height
                  << " writes, got " << written_row_ptrs.size() << "\n";
    }

    // Cleanup
    for (int i = 0; i < height; ++i) delete[] rows[i];
    delete png_ptr;

    if (!ok) {
        std::cerr << "[FAIL] test_png_write_image_single_pass: row order mismatch or count mismatch\n";
    }
    return ok;
}

// Test 3: Multi-pass (interlaced) mode should emit height * passes rows in expected order across passes
static bool test_png_write_image_two_passes() {
    reset_test_state();
    interlace_passes = 2; // simulate interlaced image with 2 passes

    const int height = 2;
    // Create 2 distinct rows
    png_byte* rows[height];
    for (int i = 0; i < height; ++i) {
        rows[i] = new png_byte[4];
        for (int j = 0; j < 4; ++j) {
            rows[i][j] = static_cast<png_byte>(i + j);
        }
    }

    png_bytepp image = rows;

    png_struct* png_ptr = new png_struct{ static_cast<png_uint_32>(height) };

    // Run
    png_write_image(png_ptr, image);

    // Expect height * interlace_passes writes
    const int expected = height * interlace_passes;
    bool ok = (written_row_ptrs.size() == static_cast<size_t>(expected));
    if (ok) {
        // Expected sequence: rows[0], rows[1], rows[0], rows[1]
        for (int k = 0; k < expected; ++k) {
            const png_byte* expected_row = rows[k % height];
            if (written_row_ptrs[k] != expected_row) {
                ok = false;
                break;
            }
        }
    } else {
        std::cerr << "[FAIL] test_png_write_image_two_passes: expected " << expected
                  << " writes, got " << written_row_ptrs.size() << "\n";
    }

    // Cleanup
    for (int i = 0; i < height; ++i) delete[] rows[i];
    delete png_ptr;
    interlace_passes = 1; // restore default for other tests

    if (!ok) {
        std::cerr << "[FAIL] test_png_write_image_two_passes: row order mismatch or count mismatch across passes\n";
    }
    return ok;
}

// ----------------------
// Test runner
// ----------------------

int main() {
    int total = 0;
    int passed = 0;

    // Test 1
    ++total;
    if (test_png_write_image_null_ptr()) {
        ++passed;
        std::cout << "[PASS] test_png_write_image_null_ptr\n";
    } else {
        std::cout << "[FAIL] test_png_write_image_null_ptr\n";
    }

    // Test 2
    ++total;
    if (test_png_write_image_single_pass()) {
        ++passed;
        std::cout << "[PASS] test_png_write_image_single_pass\n";
    } else {
        std::cout << "[FAIL] test_png_write_image_single_pass\n";
    }

    // Test 3
    ++total;
    if (test_png_write_image_two_passes()) {
        ++passed;
        std::cout << "[PASS] test_png_write_image_two_passes\n";
    } else {
        std::cout << "[FAIL] test_png_write_image_two_passes\n";
    }

    std::cout << "Test results: " << passed << " / " << total << " passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
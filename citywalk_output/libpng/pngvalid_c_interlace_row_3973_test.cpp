// Test suite for the focal method interlace_row (C interface) using a lightweight
// C++11 test harness (no GTest).
// The tests are designed to validate the interlacing copy behavior by exercising
// various Adam7-like passes and pixel sizes. The tests do not terminate on failure
// and provide diagnostic output to aid debugging.
//
// Notes:
// - The tests assume the library provides interlace_row with a C linkage.
// - We declare a compatible extern "C" prototype to enable linking with the
//   actual implementation in pngvalid.c (or equivalent).
// - For simplicity and determinism, pixel_size is kept at 1 in all tests to avoid
//   endianness complications. Additional tests can be added for pixel_size > 1 if
//   the pixel_copy behavior is known.
//
// Step 1 (Contextual Keywords): interlace_row, PNG passes, Adam7 interlace, pixel_copy,
// buffer/imageRow copying, littleendian flag, w (width), pixel_size.

#include <cstdint>
#include <cstddef>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Provide a C-compatible prototype for the focal function.
// The real signature uses libpng typedefs (png_bytep, png_uint_32, etc.).
// Here we align with the common embedded types used in the focal code.
extern "C" void interlace_row(unsigned char* buffer,
                              const unsigned char* imageRow,
                              unsigned int pixel_size,
                              unsigned int w,
                              int pass,
                              int littleendian);

// Simple test harness utilities
static void print_test_header(const std::string& name) {
    std::printf("TEST: %s\n", name.c_str());
}

static bool buffers_equal(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Step 2 & 3: Unit tests
// Each test is implemented as a function returning true on success and false on failure.
// Explanatory comments describe the test purpose and the expected outcomes.

// Test 1: pass 0 (first Adam7 pass), w=8, pixel_size=1
// Explanation: For pass 0, xout copies only the first pixel at xin=0 (since xstep=8).
// Expected: buffer[0] equals imageRow[0]; buffer[1..] remains untouched (sentinel).
static bool test_interlace_row_pass0_w8_pixel1() {
    const unsigned int w = 8;
    const unsigned int pixel_size = 1;

    unsigned char imageRow[w];
    for (unsigned int i = 0; i < w; ++i) imageRow[i] = static_cast<unsigned char>(i);

    const unsigned int BUF_SIZE = w * pixel_size;
    unsigned char buffer[BUF_SIZE];
    // Initialize buffer with sentinel so we can detect non-written areas.
    for (unsigned int i = 0; i < BUF_SIZE; ++i) buffer[i] = 0xAA;

    interlace_row(buffer, imageRow, pixel_size, w, 0, 0);

    bool ok = true;
    // Only the first pixel should be copied
    if (buffer[0] != imageRow[0]) ok = false;
    // All other bytes should remain sentinel
    for (unsigned int i = 1; i < BUF_SIZE; ++i) {
        if (buffer[i] != 0xAA) {
            ok = false;
            break;
        }
    }

    if (!ok) {
        std::fprintf(stderr, "Failed: test_interlace_row_pass0_w8_pixel1\n");
    }
    return ok;
}

// Test 2: pass 6 (last Adam7 pass), w=5, pixel_size=1
// Explanation: For pass 6, xstep=1, so all w pixels should be copied in order.
// Expected: buffer[0..4] == imageRow[0..4], other bytes untouched (not present here).
static bool test_interlace_row_pass6_w5_pixel1() {
    const unsigned int w = 5;
    const unsigned int pixel_size = 1;

    unsigned char imageRow[w];
    for (unsigned int i = 0; i < w; ++i) imageRow[i] = static_cast<unsigned char>(i + 10);

    const unsigned int BUF_SIZE = w * pixel_size;
    unsigned char buffer[BUF_SIZE];
    for (unsigned int i = 0; i < BUF_SIZE; ++i) buffer[i] = 0xBB;

    interlace_row(buffer, imageRow, pixel_size, w, 6, 0);

    bool ok = true;
    for (unsigned int i = 0; i < w; ++i) {
        if (buffer[i] != imageRow[i]) { ok = false; break; }
    }
    if (!ok) {
        std::fprintf(stderr, "Failed: test_interlace_row_pass6_w5_pixel1\n");
    }
    return ok;
}

// Test 3: pass 2, w=7, pixel_size=1
// Explanation: For pass 2, start at xin=0 and xstep=4. Expected copies at xin=0 and xin=4.
// buffer[0] = imageRow[0], buffer[1] = imageRow[4], others sentinel.
static bool test_interlace_row_pass2_w7_pixel1() {
    const unsigned int w = 7;
    const unsigned int pixel_size = 1;

    unsigned char imageRow[w];
    for (unsigned int i = 0; i < w; ++i) imageRow[i] = static_cast<unsigned char>(i * 3); // distinct values

    const unsigned int BUF_SIZE = 2 * pixel_size; // Only two copies will be written
    unsigned char buffer[7 * pixel_size];
    for (unsigned int i = 0; i < 7 * pixel_size; ++i) buffer[i] = 0xCC;

    interlace_row(buffer, imageRow, pixel_size, w, 2, 0);

    bool ok = true;
    if (buffer[0] != imageRow[0]) ok = false;
    if (buffer[1] != imageRow[4]) ok = false;
    // The rest of the buffer should remain as initial values (0xCC) since only two copies happen
    for (unsigned int i = 2; i < 7; ++i) {
        if (buffer[i] != 0xCC) { ok = false; break; }
    }

    if (!ok) {
        std::fprintf(stderr, "Failed: test_interlace_row_pass2_w7_pixel1\n");
    }
    return ok;
}

// Test 4: pass 1, w=6, pixel_size=1
// Explanation: For pass 1, start at xin=4 with xstep typically 8 for Adam7.
// Since w=6 <= 4, only one copy occurs: buffer[0] = imageRow[4].
// Expected: buffer[0] equals imageRow[4], others unchanged.
static bool test_interlace_row_pass1_w6_pixel1() {
    const unsigned int w = 6;
    const unsigned int pixel_size = 1;

    unsigned char imageRow[w];
    for (unsigned int i = 0; i < w; ++i) imageRow[i] = static_cast<unsigned char>(i + 20);

    const unsigned int BUF_SIZE = w * pixel_size;
    unsigned char buffer[BUF_SIZE];
    for (unsigned int i = 0; i < BUF_SIZE; ++i) buffer[i] = 0xDD;

    interlace_row(buffer, imageRow, pixel_size, w, 1, 0);

    bool ok = true;
    if (buffer[0] != imageRow[4]) ok = false;
    for (unsigned int i = 1; i < BUF_SIZE; ++i) {
        if (buffer[i] != 0xDD) { ok = false; break; }
    }

    if (!ok) {
        std::fprintf(stderr, "Failed: test_interlace_row_pass1_w6_pixel1\n");
    }
    return ok;
}

// Test 5: pass 6, w=4, pixel_size=1 (additional full-copy check for small width)
// Explanation: With pass 6 and xstep=1, all w pixels should copy (0..3).
static bool test_interlace_row_pass6_w4_pixel1_fullcopy() {
    const unsigned int w = 4;
    const unsigned int pixel_size = 1;

    unsigned char imageRow[w];
    for (unsigned int i = 0; i < w; ++i) imageRow[i] = static_cast<unsigned char>(i + 5);

    const unsigned int BUF_SIZE = w * pixel_size;
    unsigned char buffer[BUF_SIZE];
    for (unsigned int i = 0; i < BUF_SIZE; ++i) buffer[i] = 0xEE;

    interlace_row(buffer, imageRow, pixel_size, w, 6, 0);

    bool ok = true;
    for (unsigned int i = 0; i < w; ++i) {
        if (buffer[i] != imageRow[i]) { ok = false; break; }
    }

    if (!ok) {
        std::fprintf(stderr, "Failed: test_interlace_row_pass6_w4_pixel1_fullcopy\n");
    }
    return ok;
}

// Main: Run all tests and report summary
int main() {
    // Run tests and print per-test results
    int total = 0;
    int passed = 0;

    struct {
        bool (*fn)();
        const char* name;
        const char* description;
    } tests[] = {
        { test_interlace_row_pass0_w8_pixel1, "test_interlace_row_pass0_w8_pixel1",
          "Verify pass 0 copies only the first pixel for w=8, pixel_size=1." },
        { test_interlace_row_pass6_w5_pixel1, "test_interlace_row_pass6_w5_pixel1",
          "Verify pass 6 copies all w pixels for w=5, pixel_size=1." },
        { test_interlace_row_pass2_w7_pixel1, "test_interlace_row_pass2_w7_pixel1",
          "Verify pass 2 copies pixels at xin=0 and xin=4 for w=7, pixel_size=1." },
        { test_interlace_row_pass1_w6_pixel1, "test_interlace_row_pass1_w6_pixel1",
          "Verify pass 1 copies a single pixel at xin=4 for w=6, pixel_size=1." },
        { test_interlace_row_pass6_w4_pixel1_fullcopy, "test_interlace_row_pass6_w4_pixel1_fullcopy",
          "Verify pass 6 copies all 4 pixels for w=4, pixel_size=1." }
    };

    const int test_count = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < test_count; ++i) {
        print_test_header(tests[i].name);
        bool ok = tests[i].fn();
        ++total;
        if (ok) {
            ++passed;
            std::printf("Result: PASS – %s\n\n", tests[i].name);
        } else {
            std::printf("Result: FAIL – %s\n\n", tests[i].name);
        }
        // Optional: print description
        // std::printf("  %s\n", tests[i].description);
    }

    std::printf("Summary: %d/%d tests passed.\n", passed, total);
    return (total == passed) ? 0 : 1;
}
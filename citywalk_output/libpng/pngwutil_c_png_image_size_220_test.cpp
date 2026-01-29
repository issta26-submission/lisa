// Test suite for png_image_size in pngwutil.c
// This file provides a small C++11 test harness (no GTest) to validate
// the png_image_size(png_structrp png_ptr) function.
// The tests exercise true/false branches of the internal predicates and
// ensure coverage for interlaced vs non-interlaced paths, as well as edge
// conditions near the 15-bit size limit.
//
// Notes:
// - We directly instantiate a libpng internal png_struct object (png_struct)
//   and populate only the fields used by png_image_size: height, rowbytes,
//   interlaced, width, and pixel_depth.
// - We rely on internal libpng macros (PNG_PASS_COLS, PNG_PASS_ROWS, PNG_ROWBYTES)
//   defined in pngpriv.h. This requires including pngpriv.h with C linkage.
// - The test uses a lightweight, non-terminating assertion style: failures are
//   logged but do not abort the test run, to maximize coverage.
//
// How to compile (example):
// g++ -std=c++11 -I<path-to-libpng-headers> test_png_image_size.cpp <library-objects> -o test_png_image_size
//
// It is assumed that pngwutil.c (providing png_image_size) is compiled and linked with the test.

#include <cstdint>
#include <cstring>
#include <pngpriv.h>
#include <cstdio>


// Bring in internal libpng definitions (png_struct, png_structrp, and macros)
extern "C" {

// Declare the focal function from pngwutil.c
png_uint_32 png_image_size(png_structrp png_ptr);
}

// Simple non-terminating test harness
static int g_failures = 0;
static int g_tests_run = 0;

static void log_pass(const char* msg) {
    // Lightweight pass log
    printf("PASS: %s\n", msg);
}
static void log_fail(const char* msg) {
    // Lightweight fail log
    printf("FAIL: %s\n", msg);
    ++g_failures;
}
static void expect_equal(uint32_t a, uint32_t b, const char* msg) {
    ++g_tests_run;
    if (a == b) {
        log_pass(msg);
    } else {
        printf("DETAIL: expected %u, got %u\n", (unsigned)b, (unsigned)a);
        log_fail(msg);
    }
}

// Helper: compute expected interlaced size using the same macros as the focal code
// So tests stay aligned with internal logic.
static png_uint_32 compute_interlaced_expected(png_uint_32 w, png_uint_32 h, unsigned int pd) {
    png_alloc_size_t cb_base = 0;
    for (int pass = 0; pass <= 6; ++pass) {
        png_uint_32 pw = PNG_PASS_COLS(w, pass);
        if (pw > 0) {
            cb_base += (PNG_ROWBYTES(pd, pw) + 1) * PNG_PASS_ROWS(h, pass);
        }
    }
    return static_cast<png_uint_32>(cb_base);
}

// Test 1: Non-interlaced path within 15-bit limits
// - Preconditions: rowbytes < 32768, height < 32768, interlaced == 0
// - Expected: (rowbytes + 1) * height
static void test_png_image_size_non_interlaced_basic() {
    // Initialize a fake png_struct
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    // Set up a simple case
    p->height        = 100;   // < 32768
    p->rowbytes      = 50;    // < 32768
    p->interlaced    = 0;     // non-interlaced
    p->width         = 200;   // arbitrary
    p->pixel_depth   = 8;     // typical depth

    uint32_t expected = (p->rowbytes + 1) * p->height; // 51 * 100 = 5100
    uint32_t got = png_image_size(p);

    expect_equal(got, expected, "Non-interlaced: (rowbytes+1)*height within 15-bit bounds");
}

// Test 2: Interlaced path with non-zero width resulting in multiple passes
// - Preconditions: height < 32768, rowbytes < 32768, interlaced != 0
// - Expected: sum over passes as computed by compute_interlaced_expected
static void test_png_image_size_interlaced_basic() {
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    p->height      = 40;      // < 32768
    p->rowbytes    = 120;     // < 32768
    p->interlaced  = 1;       // interlaced
    p->width       = 100;     // width that excites several passes
    p->pixel_depth = 8;       // typical

    uint32_t expected = compute_interlaced_expected(p->width, p->height, p->pixel_depth);
    uint32_t got = png_image_size(p);

    expect_equal(got, expected, "Interlaced: sum over passes matches compute_interlaced_expected");
}

// Test 3: Boundary path where height reaches the 15-bit limit boundary
// - height >= 32768 should yield 0xffffffffU per implementation
static void test_png_image_size_height_overflow() {
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    p->height      = 32768;  // equal to 2^15
    p->rowbytes    = 10;
    p->interlaced  = 0;
    p->width       = 50;
    p->pixel_depth = 8;

    uint32_t got = png_image_size(p);
    expect_equal(got, 0xffffffffU, "Height >= 32768 yields 0xffffffff (overflow)");
}

// Test 4: Boundary path where rowbytes reaches the 15-bit limit boundary
// - rowbytes >= 32768 should yield 0xffffffffU
static void test_png_image_size_rowbytes_overflow() {
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    p->height      = 20;
    p->rowbytes    = 32768;  // equal to 2^15
    p->interlaced  = 0;
    p->width       = 60;
    p->pixel_depth = 8;

    uint32_t got = png_image_size(p);
    expect_equal(got, 0xffffffffU, "Rowbytes >= 32768 yields 0xffffffff (overflow)");
}

// Test 5: Interlaced with width = 0 to exercise pw == 0 branches
// - Should compute using compute_interlaced_expected which should yield 0
static void test_png_image_size_interlaced_width_zero() {
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    p->height      = 10;
    p->rowbytes    = 20;
    p->interlaced  = 1;
    p->width       = 0;       // width zero
    p->pixel_depth = 8;

    uint32_t expected = compute_interlaced_expected(p->width, p->height, p->pixel_depth);
    uint32_t got = png_image_size(p);

    expect_equal(got, expected, "Interlaced width 0: expected 0 (pw==0 for all passes)");
}

// Test 6: Non-interlaced with height zero
// - Should yield 0 regardless of rowbytes
static void test_png_image_size_height_zero_non_interlaced() {
    png_struct png;
    std::memset(&png, 0, sizeof(png));
    png_structrp p = &png;

    p->height      = 0;
    p->rowbytes    = 123;
    p->interlaced  = 0;
    p->width       = 10;
    p->pixel_depth = 8;

    uint32_t got = png_image_size(p);
    uint32_t expected = 0;
    expect_equal(got, expected, "Height 0 with non-interlaced should yield 0");
}

// Main entry: run all tests and report summary
int main() {
    printf("Starting tests for png_image_size...\n");

    test_png_image_size_non_interlaced_basic();
    test_png_image_size_interlaced_basic();
    test_png_image_size_height_overflow();
    test_png_image_size_rowbytes_overflow();
    test_png_image_size_interlaced_width_zero();
    test_png_image_size_height_zero_non_interlaced();

    printf("Tests run: %d, Failures: %d\n", g_tests_run, g_failures);
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}
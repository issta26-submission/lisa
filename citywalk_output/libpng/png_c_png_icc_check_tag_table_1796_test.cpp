// C++11 unit test suite for the focal method: png_icc_check_tag_table
// This test suite does not rely on GoogleTest; it uses a small, self-contained harness.
// It constructs ICC profile-like byte buffers to exercise the dependent code paths
// inside png_icc_check_tag_table and uses libpng's error handling mechanism via setjmp/longjmp
// to safely test error paths without terminating the test process.

#include <cstdint>
#include <vector>
#include <setjmp.h>
#include <cstdlib>
#include <pngpriv.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <png.h>



// Forward declaration of the focal function from the project's codebase.
// We rely on the public libpng header types (png_const_structrp, etc.)
extern "C" int png_icc_check_tag_table(png_const_structrp png_ptr, png_const_charp name,
                                      png_uint_32 profile_length, png_const_bytep profile);

// Global jump buffer used to catch libpng fatal errors invoked via the test error/warning handlers.
static jmp_buf g_png_jmp_buf;

// Simple error handler that libpng will call on errors.
// It does not print to stdout/stderr to keep test output clean, just triggers a longjmp.
static void test_error_fn(png_structp, png_const_charp /*msg*/)
{
    longjmp(g_png_jmp_buf, 1);
}

// Simple warning handler that libpng may call for warnings.
// We also longjmp here to treat warnings as testable events (for coverage).
static void test_warning_fn(png_structp, png_const_charp /*msg*/)
{
    longjmp(g_png_jmp_buf, 1);
}

// Helper macro to print and count test results in a compact way.
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        std::cerr << "FAIL: " << msg << " (" << #cond << ")" << std::endl; \
        ++tests_failed; \
    } else { \
        // Uncomment to see successful expectations: std::cout << "PASS: " << msg << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++tests_run; \
    if (!((a) == (b))) { \
        std::cerr << "FAIL: " << msg << " (" << #a << " == " << #b << ") [" << (a) << " != " << (b) << "]" << std::endl; \
        ++tests_failed; \
    } \
} while(0)

// Utility to write a 32-bit unsigned value in big-endian form into a buffer at the given offset.
static void write_be_u32(std::vector<uint8_t> &buf, size_t off, uint32_t val)
{
    buf[off + 0] = static_cast<uint8_t>((val >> 24) & 0xFF);
    buf[off + 1] = static_cast<uint8_t>((val >> 16) & 0xFF);
    buf[off + 2] = static_cast<uint8_t>((val >> 8) & 0xFF);
    buf[off + 3] = static_cast<uint8_t>(val & 0xFF);
}

// Test 1: Success path - valid tag table should return 1 (success)
static bool test_png_icc_check_tag_table_success()
{
    // Build a synthetic ICC-like profile buffer.
    // Layout:
    // - 0..127: ICC header (unused by this test)
    // - 128..131: tag_count (32-bit BE)
    // - 132..(132+11): first tag entry (12 bytes: tag_id, tag_start, tag_length)
    // For simplicity, we only craft one tag and do not populate actual tag data.
    const uint32_t profile_length = 256;
    std::vector<uint8_t> profile(profile_length, 0);

    // tag_count = 1
    write_be_u32(profile, 128, 1);

    // Tag 0:
    // tag_id = 1
    write_be_u32(profile, 132, 1);
    // tag_start = 32 (must be multiple of 4)
    write_be_u32(profile, 136, 32);
    // tag_length = 4
    write_be_u32(profile, 140, 4);

    // Call the focal function with a NULL png_ptr (safe when no errors occur)
    int result = png_icc_check_tag_table(nullptr, "test", profile_length, profile.data());

    // Expect success (1)
    EXPECT_EQ(result, 1, "png_icc_check_tag_table should succeed for valid tag table");
    return tests_failed == 0;
}

// Test 2: Error path - tag_start > profile_length should trigger error handling.
// We use libpng's error mechanism with a setjmp/longjmp wrapper to catch the error
// and verify that an error condition is reported (i.e., control returns via longjmp).
static bool test_png_icc_check_tag_table_error_out_of_profile()
{
    // Construct a profile where the tag table references data outside the profile.
    const uint32_t profile_length = 256;
    std::vector<uint8_t> profile(profile_length, 0);

    // tag_count = 1
    write_be_u32(profile, 128, 1);

    // Tag 0:
    // tag_id = 1
    write_be_u32(profile, 132, 1);
    // tag_start = 260 (out of bounds)
    write_be_u32(profile, 136, 260);
    // tag_length = 4
    write_be_u32(profile, 140, 4);

    // Create a PNG struct with custom error/warning handlers
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, test_error_fn, test_warning_fn);
    if (png_ptr == NULL) {
        std::cerr << "ERROR: Unable to create libpng read struct" << std::endl;
        ++tests_failed;
        return false;
    }

    bool test_passed = false;
    if (setjmp(g_png_jmp_buf) == 0) {
        // Expect error path to trigger longjmp back here
        int res = png_icc_check_tag_table(reinterpret_cast<png_const_structrp>(png_ptr),
                                          "test", profile_length, profile.data());
        // If we reached here, no longjmp occurred; this means error path did not trigger as expected.
        (void)res;
        test_passed = false;
    } else {
        // We landed here due to an error; this is the expected path for an out-of-bounds tag.
        test_passed = true;
    }

    // Cleanup libpng struct
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    EXPECT_TRUE(test_passed, "png_icc_check_tag_table should invoke error handler on out-of-profile tag");
    return test_passed && (tests_failed == 0);
}

// Test 3: Error path - tag_start not aligned (alignment violation) triggers error handling.
// This ensures the false branch for the alignment condition is exercised.
// Depending on libpng's error policy, this may be treated as a warning or error.
// We still set up longjmp capture to observe the behavior.
static bool test_png_icc_check_tag_table_error_unaligned_start()
{
    // Profile with a misaligned tag_start (e.g., 5)
    const uint32_t profile_length = 256;
    std::vector<uint8_t> profile(profile_length, 0);

    // tag_count = 1
    write_be_u32(profile, 128, 1);

    // Tag 0:
    // tag_id = 1
    write_be_u32(profile, 132, 1);
    // tag_start = 5 (not multiple of 4)
    write_be_u32(profile, 136, 5);
    // tag_length = 4
    write_be_u32(profile, 140, 4);

    // Create a PNG struct with custom error/warning handlers
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, test_error_fn, test_warning_fn);
    if (png_ptr == NULL) {
        std::cerr << "ERROR: Unable to create libpng read struct" << std::endl;
        ++tests_failed;
        return false;
    }

    bool test_passed = false;
    if (setjmp(g_png_jmp_buf) == 0) {
        // Expect an error (or warning) path to trigger and longjmp back here
        int res = png_icc_check_tag_table(reinterpret_cast<png_const_structrp>(png_ptr),
                                          "test", profile_length, profile.data());
        // If we reached here, no longjmp occurred; treat as failure for coverage purposes.
        (void)res;
        test_passed = false;
    } else {
        // We landed here due to an error/warning.
        test_passed = true;
    }

    // Cleanup libpng struct
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    EXPECT_TRUE(test_passed, "png_icc_check_tag_table should handle unaligned tag_start (hit error path)");
    return test_passed && (tests_failed == 0);
}

int main()
{
    // Run tests in a sequence to maximize coverage of the focal method.
    bool a = test_png_icc_check_tag_table_success();
    int total_before = tests_run;
    EXPECT_TRUE(a, "Test 1: Success path should return 1 and not invoke error handling");

    bool b = test_png_icc_check_tag_table_error_out_of_profile();
    EXPECT_TRUE(b, "Test 2: Error path (tag outside profile) should invoke error handler and longjmp");

    bool c = test_png_icc_check_tag_table_error_unaligned_start();
    EXPECT_TRUE(c, "Test 3: Error path (unaligned tag_start) should invoke error handler and longjmp");

    // Summary
    std::cout << "Total tests run: " << tests_run << ", failures: " << tests_failed << std::endl;
    return (tests_failed == 0) ? 0 : 1;
}
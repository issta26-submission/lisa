// Unit test suite for png_set_compression_buffer_size in pngset.c
// This test suite is written for a C++11 project without Google Test.
// It uses a lightweight, non-terminating assertion approach and a small
// custom error-handling hook to catch libpng errors via setjmp/longjmp.
// The tests rely on libpng being available in the build environment.

#include <cstdint>
#include <setjmp.h>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <png.h>
#include <cstddef>


extern "C" {
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;
static const char* g_last_test_name = nullptr;

// Simple non-terminating assertion helper
static void test_fail(const char* name, const char* msg) {
    ++g_failed_tests;
    fprintf(stderr, "[FAIL] %s: %s\n", name, msg);
}
static void test_pass(const char* name) {
    fprintf(stdout, "[PASS] %s\n", name);
}
#define ASSERT_TRUE(cond, name, msg) do { if (cond) { test_pass(name); } else { test_fail(name, msg); } ++g_total_tests; } while(0)
#define ASSERT_EQ(a, b, name, msg) do { bool _eq = ((a) == (b)); if (_eq) test_pass(name); else test_fail(name, msg); ++g_total_tests; } while(0)

// Global jump buffer used by the error handler to simulate libpng error recovery
static jmp_buf g_test_jmpbuf;
static jmp_buf* g_current_jmp = nullptr;

// Custom error handler to intercept libpng errors during tests
static void test_png_error_fn(png_structp /*png_ptr*/, png_const_charp /*msg*/) {
    if (g_current_jmp) {
        longjmp(*g_current_jmp, 1);
    }
}

// Helper to safely destroy png structs if allocated
static void safe_destroy_write_struct(png_structpp pptr) {
    if (pptr && *pptr) {
        png_destroy_write_struct(pptr, nullptr);
        *pptr = nullptr;
    }
}
static void safe_destroy_read_struct(png_structpp pptr) {
    if (pptr && *pptr) {
        png_destroy_read_struct(pptr, nullptr, nullptr);
        *pptr = nullptr;
    }
}

/*
Test 1: Null pointer should be handled gracefully (no crash, no action).
This directly exercises the early return path in png_set_compression_buffer_size.
*/
static bool test_null_ptr_returns() {
    // Call with NULL png_ptr; expect no crash and no action.
    png_set_compression_buffer_size(nullptr, 123);
    // If execution returns here without crash, consider it a pass.
    return true;
}

/*
Test 2: When in read-struct mode (PNG_SEQUENTIAL_READ_SUPPORTED is defined),
the function should set IDAT_read_size and return early.
We rely on libpng to provide the read-struct with appropriate mode flags.
*/
static bool test_read_struct_sets_IDAT_read_size() {
#if defined(PNG_SEQUENTIAL_READ_SUPPORTED)
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        test_fail("test_read_struct_sets_IDAT_read_size", "failed to create read struct");
        ++g_total_tests;
        return false;
    }
    const size_t test_size = 0x12345;
    png_set_compression_buffer_size(png_ptr, test_size);
    bool ok = (png_ptr->IDAT_read_size == (png_uint_32)test_size);
    safe_destroy_read_struct(&png_ptr);
    ASSERT_TRUE(ok, "test_read_struct_sets_IDAT_read_size", "IDAT_read_size not set as expected");
    return ok;
#else
    // If the macro is not defined in the environment, skip this test gracefully.
    test_pass("test_read_struct_sets_IDAT_read_size (skipped: PNG_SEQUENTIAL_READ_SUPPORTED not defined)");
    return true;
#endif
}

/*
Test 3: When zowner != 0 (buffer in use), a warning should be issued and the function should return without changing state.
We verify that zbuffer_size remains unchanged and zbuffer_list remains intact.
*/
static bool test_write_zowner_in_use_no_change() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        test_fail("test_write_zowner_in_use_no_change", "failed to create write struct");
        ++g_total_tests;
        return false;
    }
    // Ensure we are in write mode (not a read struct)
    png_ptr->mode = 0;
    png_ptr->zowner = 1; // indicate buffer is in use
    png_ptr->zbuffer_size = 32;
    png_ptr->zbuffer_list = (png_uint_8*)0xDEADBEEF; // just a non-null placeholder

    png_set_compression_buffer_size(png_ptr, 64);

    bool ok = (png_ptr->zbuffer_size == 32) && (png_ptr->zbuffer_list == (png_uint_8*)0xDEADBEEF);
    safe_destroy_write_struct(&png_ptr);

    ASSERT_TRUE(ok, "test_write_zowner_in_use_no_change", "zbuffer_size or zbuffer_list changed unexpectedly when zowner != 0");
    return ok;
}

/*
Test 4: When size > ZLIB_IO_MAX, the size should be clamped to ZLIB_IO_MAX and applied.
We verify that zbuffer_size becomes ZLIB_IO_MAX.
*/
static bool test_write_size_clamped_to_max() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        test_fail("test_write_size_clamped_to_max", "failed to create write struct");
        ++g_total_tests;
        return false;
    }
    png_ptr->mode = 0;
    png_ptr->zowner = 0;
    png_ptr->zbuffer_size = 8;

    // Set size slightly larger than ZLIB_IO_MAX to trigger clamp
    size_t big_size = (size_t)ZLIB_IO_MAX + 1;
    png_set_compression_buffer_size(png_ptr, big_size);

    bool ok = (png_ptr->zbuffer_size == (uInt)ZLIB_IO_MAX);
    safe_destroy_write_struct(&png_ptr);

    ASSERT_TRUE(ok, "test_write_size_clamped_to_max", "size not clamped to ZLIB_IO_MAX as expected");
    return ok;
}

/*
Test 5: When size < 6 in write mode, a warning should be issued and function should return
without changing zbuffer_size.
*/
static bool test_write_size_too_small_no_change() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        test_fail("test_write_size_too_small_no_change", "failed to create write struct");
        ++g_total_tests;
        return false;
    }
    png_ptr->mode = 0;
    png_ptr->zowner = 0;
    png_ptr->zbuffer_size = 9;

    png_set_compression_buffer_size(png_ptr, 5);

    bool ok = (png_ptr->zbuffer_size == 9);
    safe_destroy_write_struct(&png_ptr);

    ASSERT_TRUE(ok, "test_write_size_too_small_no_change", "zbuffer_size changed when size < 6");
    return ok;
}

/*
Test 6: Valid size update should change zbuffer_size accordingly.
*/
static bool test_write_size_updates_buffer_size() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        test_fail("test_write_size_updates_buffer_size", "failed to create write struct");
        ++g_total_tests;
        return false;
    }
    png_ptr->mode = 0;
    png_ptr->zowner = 0;
    png_ptr->zbuffer_size = 8;

    png_set_compression_buffer_size(png_ptr, 12);

    bool ok = (png_ptr->zbuffer_size == 12);
    safe_destroy_write_struct(&png_ptr);

    ASSERT_TRUE(ok, "test_write_size_updates_buffer_size", "zbuffer_size did not update to 12");
    return ok;
}

/*
Test 7: Invalid size (0) should trigger an error path via longjmp.
We attach a custom error handler and use setjmp/longjmp to verify error handling.
*/
static bool test_invalid_size_zero_triggers_error() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, test_png_error_fn, nullptr);
    if (!png_ptr) {
        test_fail("test_invalid_size_zero_triggers_error", "failed to create write struct");
        ++g_total_tests;
        return false;
    }

    // Prepare a jump point to catch the error
    if (setjmp(g_test_jmpbuf)) {
        // Error occurred as expected
        safe_destroy_write_struct(&png_ptr);
        test_pass("test_invalid_size_zero_triggers_error");
        ++g_total_tests;
        return true;
    } else {
        // Run with error-triggering input
        g_current_jmp = &g_test_jmpbuf;
        png_set_compression_buffer_size(png_ptr, 0); // should trigger error
        g_current_jmp = nullptr;
        // If we reach here, error did not occur (unexpected)
        safe_destroy_write_struct(&png_ptr);
        test_fail("test_invalid_size_zero_triggers_error", "expected error did not occur");
        ++g_total_tests;
        return false;
    }
}

/*
Test 8: Invalid size (greater than PNG_UINT_32_MAX bound) to ensure overflow case is safe.
*/
static bool test_invalid_size_overflow_triggers_error() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, test_png_error_fn, nullptr);
    if (!png_ptr) {
        test_fail("test_invalid_size_overflow_triggers_error", "failed to create write struct");
        ++g_total_tests;
        return false;
    }

    if (setjmp(g_test_jmpbuf)) {
        // Error occurred as expected
        safe_destroy_write_struct(&png_ptr);
        test_pass("test_invalid_size_overflow_triggers_error");
        ++g_total_tests;
        return true;
    } else {
        g_current_jmp = &g_test_jmpbuf;
        // Create a size larger than PNG_UINT_31_MAX to force invalid path
        size_t bad_size = (size_t)PNG_UINT_31_MAX + 1;
        png_set_compression_buffer_size(png_ptr, bad_size);
        g_current_jmp = nullptr;
        safe_destroy_write_struct(&png_ptr);
        test_fail("test_invalid_size_overflow_triggers_error", "expected overflow error did not occur");
        ++g_total_tests;
        return false;
    }
}

/*
Main entry: run all tests and provide a summary.
*/
int main() {
    // Initialize counters
    g_total_tests = 0;
    g_failed_tests = 0;

    // Run tests
    test_null_ptr_returns();
    test_read_struct_sets_IDAT_read_size();
    test_write_zowner_in_use_no_change();
    test_write_size_clamped_to_max();
    test_write_size_too_small_no_change();
    test_write_size_updates_buffer_size();
    test_invalid_size_zero_triggers_error();
    test_invalid_size_overflow_triggers_error();

    // Summary
    fprintf(stdout, "\nTEST SUMMARY: total=%d, failed=%d, passed=%d\n",
            g_total_tests, g_failed_tests, g_total_tests - g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}
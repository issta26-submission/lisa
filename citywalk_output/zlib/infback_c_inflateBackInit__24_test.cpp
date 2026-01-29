/*
Unit test suite for inflateBackInit_ (C code) using a custom C++11 test harness.
No Google Test; tests run from main and use non-terminating assertions.
The tests cover multiple branches of inflateBackInit_ based on input validation
and basic successful initialization.

Notes:
- This test assumes the standard zlib-like headers are available: zutil.h, inflate.h, etc.
- Tests use external C linkage for inflateBackInit_ via extern "C" when including headers.
- A couple of tests provide a custom allocator to avoid depending on zlib's default allocators.
*/

#include <cstring>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Bring in the zlib-like API declarations (C linkage)
extern "C" {
}

// Global test counters
static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating assertion macros
#define EXPECT_EQ_INT(actual, expected, test_name) do { \
    ++g_total; \
    if ((actual) != (expected)) { \
        ++g_failed; \
        std::cerr << "[FAIL] " << test_name << ": expected " << (expected) \
                  << ", got " << (actual) << std::endl; \
    } \
} while(0)

#define TEST_PRINT_PASS(test_name) do { \
    std::cout << "[PASS] " << test_name << std::endl; \
} while(0)

// Forward declarations for tests
static void test_inflateBackInit_valid_basic();
static void test_inflateBackInit_version_null();
static void test_inflateBackInit_version_mismatch();
static void test_inflateBackInit_stream_size_mismatch();
static void test_inflateBackInit_strm_null();
static void test_inflateBackInit_window_null();
static void test_inflateBackInit_windowBits_out_of_range();
static void test_inflateBackInit_valid_with_custom_alloc();

// Custom allocator helpers (to avoid relying on zcalloc/zcfree)
static voidpf my_alloc(voidpf opaque, unsigned int items, unsigned int size) {
    (void)opaque; // unused
    return (voidpf)malloc(items * size);
}
static void my_free(voidpf opaque, voidpf address) {
    (void)opaque; // unused
    free(address);
}

// Main function to run tests
int main() {
    // Run tests
    test_inflateBackInit_valid_basic();
    test_inflateBackInit_version_null();
    test_inflateBackInit_version_mismatch();
    test_inflateBackInit_stream_size_mismatch();
    test_inflateBackInit_strm_null();
    test_inflateBackInit_window_null();
    test_inflateBackInit_windowBits_out_of_range();
    test_inflateBackInit_valid_with_custom_alloc();

    // Summary
    std::cout << "Tests run: " << g_total << ", Passed: "
              << (g_total - g_failed) << ", Failed: " << g_failed << std::endl;

    return g_failed == 0 ? 0 : 1;
}

/*
Test 1: Valid initialization path
- All checks pass: version correct, stream_size correct, strm and window non-null,
  windowBits within [8,15].
- Expect: Z_OK
*/
static void test_inflateBackInit_valid_basic() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;

    unsigned char window[32 * 1024]; // 32KB window buffer
    const char *version = ZLIB_VERSION; // correct version
    int windowBits = 13; // valid in [8,15]

    int result = inflateBackInit_(strm, windowBits, window, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_OK, "inflateBackInit_valid_basic");
    // If we reached here, allocated and initialized state pointers should be set by the callee
    if (result == Z_OK) TEST_PRINT_PASS("inflateBackInit_valid_basic");
}

/*
Test 2: version is NULL
- version == Z_NULL should trigger Z_VERSION_ERROR
*/
static void test_inflateBackInit_version_null() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;
    unsigned char window[1024];
    const char *version = Z_NULL; // null version
    int windowBits = 9;

    int result = inflateBackInit_(strm, windowBits, window, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_VERSION_ERROR, "inflateBackInit_version_null");
    if (result == Z_VERSION_ERROR) TEST_PRINT_PASS("inflateBackInit_version_null");
}

/*
Test 3: version string mismatch (first character differs)
- non-null version but first character not equal to ZLIB_VERSION[0] should trigger Z_VERSION_ERROR
*/
static void test_inflateBackInit_version_mismatch() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;
    unsigned char window[1024];
    // Create a version string with a different first character
    const char *version = "Y" ZLIB_VERSION + 1; // intentionally mismatched first char
    int windowBits = 9;

    int result = inflateBackInit_(strm, windowBits, window, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_VERSION_ERROR, "inflateBackInit_version_mismatch");
    if (result == Z_VERSION_ERROR) TEST_PRINT_PASS("inflateBackInit_version_mismatch");
}

/*
Test 4: stream_size mismatch
- stream_size not equal to sizeof(z_stream) should trigger Z_VERSION_ERROR
*/
static void test_inflateBackInit_stream_size_mismatch() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;
    unsigned char window[1024];
    const char *version = ZLIB_VERSION;
    int windowBits = 9;

    int result = inflateBackInit_(strm, windowBits, window, version,
                                  (int)sizeof(z_stream) + 1); // mismatch

    EXPECT_EQ_INT(result, Z_VERSION_ERROR, "inflateBackInit_stream_size_mismatch");
    if (result == Z_VERSION_ERROR) TEST_PRINT_PASS("inflateBackInit_stream_size_mismatch");
}

/*
Test 5: strm is NULL
- Passing NULL for the z_streamp should yield Z_STREAM_ERROR
*/
static void test_inflateBackInit_strm_null() {
    unsigned char window[1024];
    const char *version = ZLIB_VERSION;
    int windowBits = 9;

    int result = inflateBackInit_(Z_NULL, windowBits, window, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_STREAM_ERROR, "inflateBackInit_strm_null");
    if (result == Z_STREAM_ERROR) TEST_PRINT_PASS("inflateBackInit_strm_null");
}

/*
Test 6: window is NULL
- Passing NULL for the window buffer should yield Z_STREAM_ERROR
*/
static void test_inflateBackInit_window_null() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;
    const char *version = ZLIB_VERSION;
    int windowBits = 9;

    int result = inflateBackInit_(strm, windowBits, Z_NULL, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_STREAM_ERROR, "inflateBackInit_window_null");
    if (result == Z_STREAM_ERROR) TEST_PRINT_PASS("inflateBackInit_window_null");
}

/*
Test 7: windowBits out of range
- windowBits less than 8 or greater than 15 should yield Z_STREAM_ERROR
*/
static void test_inflateBackInit_windowBits_out_of_range() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;
    unsigned char window[1024];
    const char *version = ZLIB_VERSION;

    // Test with too small windowBits
    int result1 = inflateBackInit_(strm, 7, window, version,
                                   (int)sizeof(z_stream));
    EXPECT_EQ_INT(result1, Z_STREAM_ERROR, "inflateBackInit_windowBits_out_of_range_low");

    // Test with too large windowBits
    int result2 = inflateBackInit_(strm, 16, window, version,
                                   (int)sizeof(z_stream));
    EXPECT_EQ_INT(result2, Z_STREAM_ERROR, "inflateBackInit_windowBits_out_of_range_high");

    if (result1 == Z_STREAM_ERROR && result2 == Z_STREAM_ERROR) {
        TEST_PRINT_PASS("inflateBackInit_windowBits_out_of_range");
    }
}

/*
Test 8: Valid initialization path with a custom allocator
- Provide non-null zalloc/zfree so code uses them via ZALLOC/ZFREE path
- Expect: Z_OK
*/
static void test_inflateBackInit_valid_with_custom_alloc() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));
    z_streamp strm = &zstrm;

    // Assign custom alloc/free to avoid depending on zcalloc/zcfree
    strm->zalloc = (alloc_func)my_alloc;
    strm->zfree  = (free_func)my_free;
    strm->opaque = Z_NULL;

    unsigned char window[1024];
    const char *version = ZLIB_VERSION;
    int windowBits = 9; // valid

    int result = inflateBackInit_(strm, windowBits, window, version,
                                  (int)sizeof(z_stream));

    EXPECT_EQ_INT(result, Z_OK, "inflateBackInit_valid_with_custom_alloc");
    if (result == Z_OK) TEST_PRINT_PASS("inflateBackInit_valid_with_custom_alloc");
}

/* Note: If the library is compiled with Z_SOLO defined, the allocation/free blocks
   in inflateBackInit_ will take different paths and tests under that path are not
   included here. A dedicated test can be added under #ifdef Z_SOLO if needed. */
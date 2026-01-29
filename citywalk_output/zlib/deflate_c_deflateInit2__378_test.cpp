// High-quality C++11 unit test suite for the focal deflateInit2_ function
// This test suite does not rely on GoogleTest. It uses a lightweight, self-contained test harness.
// It targets the deflateInit2_ function as declared in zlib-compatible environments.

#include <iostream>
#include <zlib.h>
#include <deflate.h>
#include <cstring>


// Import zlib types and constants for correct test orchestration.
// We assume the project provides zlib headers and linkage.
extern "C" {
}

// Declaration of the focal function (internal variant) as visible to C++
// This mirrors the signature used inside the provided deflate.c snippet.
extern "C" int ZEXPORT deflateInit2_(z_streamp strm, int level, int method,
                                    int windowBits, int memLevel, int strategy,
                                    const char *version, int stream_size);

// Simple test harness for reporting
static int tests_run = 0;
static int tests_passed = 0;

// Utility: initialize a z_stream with a clean slate
static void init_clean_zstream(z_stream &strm) {
    std::memset(&strm, 0, sizeof(z_stream));
    strm.next_in = Z_NULL;
    strm.next_out = Z_NULL;
    strm.avail_in = 0;
    strm.avail_out = 0;
    strm.total_in = 0;
    strm.total_out = 0;
    strm.msg = Z_NULL;
    strm.zalloc = (alloc_func)0;
    strm.zfree  = (free_func)0;
    strm.state  = Z_NULL;
}

// Test 1: Version error when version pointer is NULL
// Expected: Z_VERSION_ERROR
static bool test_version_error_null_version() {
    z_stream strm;
    init_clean_zstream(strm);
    // Pass NULL version to trigger version check failure
    int ret = deflateInit2_(&strm, 5, Z_DEFLATED, 15, 8, Z_FIXED,
                            Z_NULL, (int)sizeof(z_stream));
    return ret == Z_VERSION_ERROR;
}

// Test 2: Version error when stream_size mismatches sizeof(z_stream)
// Expected: Z_VERSION_ERROR
static bool test_version_error_wrong_size() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 5, Z_DEFLATED, 15, 8, Z_FIXED,
                            ZLIB_VERSION, 0); // intentionally wrong size
    return ret == Z_VERSION_ERROR;
}

// Test 3: NULL stream pointer yields Z_STREAM_ERROR
// Expected: Z_STREAM_ERROR
static bool test_null_stream_pointer() {
    // Pass NULL as the stream pointer
    int ret = deflateInit2_(Z_NULL, 5, Z_DEFLATED, 15, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 4: Invalid memLevel triggers Z_STREAM_ERROR
// memLevel outside valid range
// Expected: Z_STREAM_ERROR
static bool test_invalid_memLevel() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 6, Z_DEFLATED, 15, 0, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 5: Invalid method (not Z_DEFLATED) triggers Z_STREAM_ERROR
// Expected: Z_STREAM_ERROR
static bool test_invalid_method() {
    z_stream strm;
    init_clean_zstream(strm);
    // Use a non-deflated method code
    int ret = deflateInit2_(&strm, 6, 999, 15, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 6: Negative windowBits that is less than -15 should yield Z_STREAM_ERROR
// windowBits = -16 triggers early error
// Expected: Z_STREAM_ERROR
static bool test_negative_windowBits_below_range() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 6, Z_DEFLATED, -16, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 7: windowBits > 15 should yield Z_STREAM_ERROR when GZIP is not enabled
// Expected: Z_STREAM_ERROR
static bool test_windowBits_exceeds_max() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 6, Z_DEFLATED, 16, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 8: level out of range (>9) triggers Z_STREAM_ERROR
// Expected: Z_STREAM_ERROR
static bool test_level_out_of_range() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 10, Z_DEFLATED, 15, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 9: strategy out of range (<0) triggers Z_STREAM_ERROR
// Expected: Z_STREAM_ERROR
static bool test_strategy_out_of_range() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 6, Z_DEFLATED, 15, 8, -1,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Test 10: Trigger the wrap=0 path with windowBits = -8 (wrap=0, windowBits=8) leading to error due to 8-bit window bug guard
// Expected: Z_STREAM_ERROR
static bool test_negative_windowBits_wrap_zero_case() {
    z_stream strm;
    init_clean_zstream(strm);
    int ret = deflateInit2_(&strm, 6, Z_DEFLATED, -8, 8, Z_FIXED,
                            ZLIB_VERSION, (int)sizeof(z_stream));
    return ret == Z_STREAM_ERROR;
}

// Main: run all tests with explanatory commentary.
// The test harness follows the directive: "If gtest isn't allowed, call test methods from the main function."
int main() {
    std::cout << "Starting unit tests for deflateInit2_ (C-style internal API) in C++11 test harness.\n";

    tests_run = 0;
    tests_passed = 0;

#define RUN_TEST(tname) do { \
    bool ok = (tname)(); \
    ++tests_run; \
    std::cout << "[" << (ok ? "PASS" : "FAIL") << "] " #tname "\n"; \
    if (ok) ++tests_passed; \
} while(0)

    // Execute tests with descriptive names
    RUN_TEST(test_version_error_null_version);
    RUN_TEST(test_version_error_wrong_size);
    RUN_TEST(test_null_stream_pointer);
    RUN_TEST(test_invalid_memLevel);
    RUN_TEST(test_invalid_method);
    RUN_TEST(test_negative_windowBits_below_range);
    RUN_TEST(test_windowBits_exceeds_max);
    RUN_TEST(test_level_out_of_range);
    RUN_TEST(test_strategy_out_of_range);
    RUN_TEST(test_negative_windowBits_wrap_zero_case);

#undef RUN_TEST

    std::cout << "Tests completed. Passed: " << tests_passed << "/" << tests_run << "\n";

    // Return non-zero if any test failed
    return (tests_passed == tests_run) ? 0 : 1;
}
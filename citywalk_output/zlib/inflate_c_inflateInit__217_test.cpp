// Unit Test Suite for inflateInit_ (focal method) without using GTest
// Target: validate behavior of inflateInit_(z_streamp strm, const char *version, int stream_size)
// This test uses a small, non-terminating assertion framework (no asserts) and prints
// results to stdout. It relies on the real zlib implementation linked in the project.

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>


// Include zlib to access types and constants used by inflateInit_.
// We rely on the project configuration to provide the full zlib integration.

// Ensure C linkage for the focal method when called from C++
extern "C" int inflateInit_(z_streamp strm, const char *version, int stream_size);

// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected, test_name)                                \
    do {                                                                        \
        ++g_total_tests;                                                        \
        if ((actual) != (expected)) {                                         \
            std::cerr << "[FAIL] " << test_name << ": Expected "            \
                      << (expected) << ", got " << (actual) << std::endl;     \
            ++g_failed_tests;                                                 \
        } else {                                                              \
            std::cout << "[OK]   " << test_name << std::endl;                \
        }                                                                     \
    } while (0)

#define EXPECT_NE(actual, not_expected, test_name)                             \
    do {                                                                        \
        ++g_total_tests;                                                        \
        if ((actual) == (not_expected)) {                                     \
            std::cerr << "[FAIL] " << test_name << ": Unexpected value "       \
                      << (actual) << std::endl;                              \
            ++g_failed_tests;                                                 \
        } else {                                                              \
            std::cout << "[OK]   " << test_name << std::endl;                \
        }                                                                     \
    } while (0)

///////////////////////////////////////////////////////////////////////////
// Test 1: Version is NULL -> should return Z_VERSION_ERROR
///////////////////////////////////////////////////////////////////////////
bool test_inflateInit_version_null() {
    // Prepare a clean z_stream structure
    z_stream s;
    std::memset(&s, 0, sizeof(s));

    // Call focal method with NULL version pointer
    int ret = inflateInit_(&s, NULL, (int)sizeof(z_stream));

    // Expected: Z_VERSION_ERROR
    bool ok = (ret == Z_VERSION_ERROR);
    if (!ok) {
        std::cerr << "[ERROR] inflateInit_ with NULL version did not return Z_VERSION_ERROR\n";
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////
// Test 2: Version has wrong first character vs ZLIB_VERSION[0] -> Z_VERSION_ERROR
///////////////////////////////////////////////////////////////////////////
bool test_inflateInit_version_bad_first_char() {
    z_stream s;
    std::memset(&s, 0, sizeof(s));

    // Provide a version string whose first character does not match ZLIB_VERSION[0]
    const char bad_version[] = "X.Y.Z";

    int ret = inflateInit_(&s, bad_version, (int)sizeof(z_stream));

    bool ok = (ret == Z_VERSION_ERROR);
    if (!ok) {
        std::cerr << "[ERROR] inflateInit_ with bad first char in version did not return Z_VERSION_ERROR\n";
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////
// Test 3: stream_size does not match sizeof(z_stream) -> Z_VERSION_ERROR
///////////////////////////////////////////////////////////////////////////
bool test_inflateInit_wrong_stream_size() {
    z_stream s;
    std::memset(&s, 0, sizeof(s));

    // Use correct version string but incorrect stream_size
    int wrong_size = 0; // intentionally incorrect
    int ret = inflateInit_(&s, ZLIB_VERSION, wrong_size);

    bool ok = (ret == Z_VERSION_ERROR);
    if (!ok) {
        std::cerr << "[ERROR] inflateInit_ with wrong stream_size did not return Z_VERSION_ERROR\n";
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////
// Test 4: Valid inputs should not immediately fail with a version error.
// This exercises the successful path through inflateInit2_ and (likely) inflateReset2_.
// We only require that the function does not return Z_VERSION_ERROR.
///////////////////////////////////////////////////////////////////////////
bool test_inflateInit_valid_path() {
    z_stream s;
    std::memset(&s, 0, sizeof(s));

    int ret = inflateInit_(&s, ZLIB_VERSION, (int)sizeof(z_stream));

    bool ok = (ret != Z_VERSION_ERROR);
    if (!ok) {
        std::cerr << "[ERROR] inflateInit_ with valid inputs unexpectedly returned Z_VERSION_ERROR\n";
    } else {
        // Best-effort cleanup if initialization succeeded to avoid leaks.
        // Inflate's end function should be called if initialization allocated resources.
        // We guard against any potential crash if inflateEnd is not appropriate here.
        // We attempt to call inflateEnd only if a valid state was created.
        // Note: inflateEnd is safe to call on uninitialized state per zlib semantics.
        inflateEnd(&s);
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////
// Main: Run all tests and report aggregate results
///////////////////////////////////////////////////////////////////////////
int main() {
    std::cout << "Running inflateInit_ unit tests (C++11, non-GTest):\n";

    bool t1 = test_inflateInit_version_null();
    EXPECT_EQ(t1, true, "test_inflateInit_version_null");

    bool t2 = test_inflateInit_version_bad_first_char();
    EXPECT_EQ(t2, true, "test_inflateInit_version_bad_first_char");

    bool t3 = test_inflateInit_wrong_stream_size();
    EXPECT_EQ(t3, true, "test_inflateInit_wrong_stream_size");

    bool t4 = test_inflateInit_valid_path();
    EXPECT_EQ(t4, true, "test_inflateInit_valid_path");

    // Summary
    std::cout << "\nTest Summary: " << (g_total_tests) << " tests, "
              << (g_failed_tests) << " failures.\n";

    return (g_failed_tests == 0) ? 0 : 1;
}
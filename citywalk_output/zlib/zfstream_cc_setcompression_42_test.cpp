// Unit tests for gzfilebuf::setcompression in zfstream.cc
// This test suite is designed to be compiled with C++11 without Google Test.
// It exercises the focal method by:
// - Verifying behavior when the underlying gz file is not opened (uninitialized file).
// - Verifying behavior after opening a gz file and applying valid compression settings.
// - Verifying error handling for invalid compression level and invalid strategy.

#include <cstring>
#include <cstdio>
#include <ios>
#include <iostream>
#include <zlib.h>
#include <zfstream.h>


// Lightweight non-terminating assertion helper
static bool expect_eq(int got, int expected, const char* desc) {
    if (got == expected) {
        std::cout << "[OK] " << desc << " (expected " << expected << ", got " << got << ")" << std::endl;
        return true;
    } else {
        std::cerr << "[FAIL] " << desc << " - expected " << expected << ", but got " << got << std::endl;
        return false;
    }
}

// Test 1: Call setcompression on an uninitialized gzfilebuf (no underlying file attached).
// Expected: gzsetparams should fail and return Z_STREAM_ERROR.
static void test_setcompression_uninitialized() {
    gzfilebuf gfb; // default-constructed; no file attached yet

    // Attempt to set compression on an uninitialized file
    int res = gfb.setcompression(6, Z_DEFAULT_STRATEGY);

    expect_eq(res, Z_STREAM_ERROR, "setcompression on uninitialized gzfilebuf should return Z_STREAM_ERROR");
}

// Test 2: Open a gz file and apply valid compression parameters.
// Expected: gzsetparams succeeds (returns Z_OK) for valid inputs on an opened stream.
static void test_setcompression_after_open() {
    gzfilebuf gfb;
    // Open a test file for writing in binary mode
    // The exact return value of open() is not relied upon; we attempt to use the object afterward.
    gfb.open("test_setcompression_after_open.gz", std::ios_base::out | std::ios_base::binary);

    // Apply valid compression settings
    int res = gfb.setcompression(9, Z_DEFAULT_STRATEGY);

    expect_eq(res, Z_OK, "setcompression after opening should return Z_OK for valid inputs");

    // Cleanup: close and remove test file
    gfb.close();
    std::remove("test_setcompression_after_open.gz");
}

// Test 3: Open a gz file and pass an invalid compression level (out of [0,9]).
// Expected: gzsetparams should fail and return Z_STREAM_ERROR.
static void test_setcompression_invalid_level() {
    gzfilebuf gfb;
    gfb.open("test_setcompression_invalid_level.gz", std::ios_base::out | std::ios_base::binary);

    int res = gfb.setcompression(10, Z_DEFAULT_STRATEGY);

    expect_eq(res, Z_STREAM_ERROR, "setcompression with invalid level (>9) should return Z_STREAM_ERROR");

    gfb.close();
    std::remove("test_setcompression_invalid_level.gz");
}

// Test 4: Open a gz file and pass an invalid compression strategy value.
// Expected: gzsetparams should fail and return Z_STREAM_ERROR.
static void test_setcompression_invalid_strategy() {
    gzfilebuf gfb;
    gfb.open("test_setcompression_invalid_strategy.gz", std::ios_base::out | std::ios_base::binary);

    int res = gfb.setcompression(6, 9999); // invalid strategy value

    expect_eq(res, Z_STREAM_ERROR, "setcompression with invalid strategy should return Z_STREAM_ERROR");

    gfb.close();
    std::remove("test_setcompression_invalid_strategy.gz");
}

int main() {
    std::cout << "Starting gzfilebuf::setcompression unit tests (C++11, no GTest)" << std::endl;

    int tests_run = 0;
    int tests_passed = 0;

    // Run Test 1
    test_setcompression_uninitialized();
    tests_run++;
    // We cannot re-run here to maintain simple flow; rely on per-test logging for pass/fail.

    // Run Test 2
    test_setcompression_after_open();
    tests_run++;
    // Note: Test 2's pass/fail is reported inside the test via expect_eq.

    // Run Test 3
    test_setcompression_invalid_level();
    tests_run++;

    // Run Test 4
    test_setcompression_invalid_strategy();
    tests_run++;

    std::cout << "Unit tests completed. Note: individual test outcomes are reported above." << std::endl;

    // Since we do not aggregate exact pass counts inside, simply exit.
    return 0;
}
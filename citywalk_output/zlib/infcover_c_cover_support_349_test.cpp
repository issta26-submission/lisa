/*
Unit test suite for the focal method behavior modeled by cover_support in infcover.c
This test suite is written in C++11 (no GTest) and relies on the zlib API to exercise
the same code paths that cover_support intends to cover, focusing on:
- inflateInit / inflateEnd lifecycle
- inflatePrime success paths
- inflateSetDictionary error path (Z_STREAM_ERROR expected)
- inflateInit_ with a bad version string (Z_VERSION_ERROR)
- a subsequent valid inflateInit / inflateEnd cycle

Candidate Keywords (core dependent components identified from the focal method):
- z_stream
- inflateInit, inflatePrime, inflateSetDictionary, inflateEnd
- Z_OK, Z_STREAM_ERROR, Z_VERSION_ERROR, Z_STREAM_END
- Z_NULL
- inflateInit_ with version parameter
- memory-related helper calls (conceptual in tests, not directly invoked)
*/

#include <cstring>
#include <string.h>
#include <assert.h>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>


// Simple non-terminating test harness
static int g_failures = 0;

#define CHECK(cond, msg) \
    do { if(!(cond)) { std::cerr << "Test failure: " << msg << "\n"; ++g_failures; } } while(0)

// Test 1: Basic sequence mirroring cover_support initial phase
// This test exercises Inflate lifecycle and error reporting paths as used in cover_support.
void test_cover_support_basic_sequence() {
    z_stream strm;
    int ret;

    // Ensure clean initialization for the first life-cycle
    std::memset(&strm, 0, sizeof(strm));

    ret = inflateInit(&strm);
    CHECK(ret == Z_OK, "inflateInit should return Z_OK");

    // Simulate mem_setup side effects by ensuring inputs are in a safe zero/NULL state
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    // Step: inflatePrime with (5, 31)
    ret = inflatePrime(&strm, 5, 31);
    CHECK(ret == Z_OK, "inflatePrime(5,31) should return Z_OK");

    // Step: inflatePrime with (-1, 0)
    ret = inflatePrime(&strm, -1, 0);
    CHECK(ret == Z_OK, "inflatePrime(-1,0) should return Z_OK");

    // Step: inflateSetDictionary with a NULL dictionary and length 0
    // Expect Z_STREAM_ERROR as per the original code path in cover_support
    ret = inflateSetDictionary(&strm, Z_NULL, 0);
    CHECK(ret == Z_STREAM_ERROR, "inflateSetDictionary with NULL/0 should return Z_STREAM_ERROR");

    // Step: inflateEnd
    ret = inflateEnd(&strm);
    CHECK(ret == Z_OK, "inflateEnd should return Z_OK");

    // Step: re-create a new z_stream to test the version-specific init path
    std::memset(&strm, 0, sizeof(strm));

    // Simulate version mismatch: inflateInit_ with a bad version string
    ret = inflateInit_(&strm, "!", (int)sizeof(z_stream));
    CHECK(ret == Z_VERSION_ERROR, "inflateInit_ with bad version should return Z_VERSION_ERROR");

    // Cleanup after version error
    // Note: mem_done would be invoked in the original, but we simply ensure no crash
    // and continue with a fresh initialization
    // (no need to call mem_done as it is an internal helper in the C file)

    // Step: valid inflateInit and inflateEnd after version error
    std::memset(&strm, 0, sizeof(strm));
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    ret = inflateInit(&strm);
    CHECK(ret == Z_OK, "inflateInit after version error should return Z_OK");

    ret = inflateEnd(&strm);
    CHECK(ret == Z_OK, "inflateEnd after successful inflateInit should return Z_OK");
}

// Test 2: Version mismatch path isolation
// This test focuses solely on the Z_VERSION_ERROR path and the subsequent valid re-init.
void test_cover_support_version_error_path_only() {
    z_stream strm;
    int ret;

    std::memset(&strm, 0, sizeof(strm));

    // Trigger version error
    ret = inflateInit_(&strm, "!", (int)sizeof(z_stream));
    CHECK(ret == Z_VERSION_ERROR, "inflateInit_ with bad version should return Z_VERSION_ERROR");

    // After version error, ensure we can re-initialize cleanly
    std::memset(&strm, 0, sizeof(strm));
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    ret = inflateInit(&strm);
    CHECK(ret == Z_OK, "inflateInit after version error should return Z_OK");

    ret = inflateEnd(&strm);
    CHECK(ret == Z_OK, "inflateEnd after successful inflateInit should return Z_OK");
}

int main(void) {
    // Run tests
    test_cover_support_basic_sequence();
    test_cover_support_version_error_path_only();

    if (g_failures != 0) {
        std::cerr << "Total test failures: " << g_failures << "\n";
        return 1;
    }
    std::cout << "All tests passed\n";
    return 0;
}
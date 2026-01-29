/*
  Unit test suite for the focal method zipOpen2 in zip.c

  Purpose:
  - Verify that zipOpen2 delegates correctly to zipOpen3 depending on whether
    pzlib_filefunc32_def is NULL or non-NULL.
  - Validate that globalcomment and append are properly forwarded.
  - Confirm the correct branching behavior (NULL vs non-NULL predicate).
  - Avoid termination on assertion failures; collect and report all failures.

  Approach:
  - Provide a minimal test harness (no GTest) with non-terminating ASSERT macro.
  - Mock the external dependency zipOpen3 with C linkage to capture inputs and return a sentinel.
  - Exercise at least two branches of the condition inside zipOpen2, plus a variation
    for globalcomment being NULL to increase coverage.
  - Buildable with C++11; relies on provided headers (zip.h, zlib.h) and the existing
    project compilation (zip.c plus this test file).

  Notes:
  - The test uses the real function signatures/types from the included headers.
  - zipOpen3 is mocked to observe parameters and simulate a return value; this avoids
    needing a full zlib/zip runtime.
  - The test asserts using a custom non-terminating macro to maximize code execution.
*/

#include <cstring>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Include project headers to obtain type definitions (zipFile, zipcharpc, etc.)

// Sentinel value used by the mock to indicate a successful path through zipOpen3
static zipFile SENTINEL_ZIPOPEN3 = (zipFile)(void*)0xDEADBEEF;

// Structure to capture the last call to the mocked zipOpen3
struct ZipOpen3CallInfo {
    const void* pathname;
    int append;
    zipcharpc* globalcomment;
    zlib_filefunc64_32_def* pzlib_filefunc64_32_def;
};

// Global capture for test verifications
static ZipOpen3CallInfo g_last_zipOpen3_call;
static int g_zipOpen3_call_count = 0;

// Helper to reset captured call info
static void reset_last_call_info() {
    g_last_zipOpen3_call.pathname = nullptr;
    g_last_zipOpen3_call.append = -1;
    g_last_zipOpen3_call.globalcomment = nullptr;
    g_last_zipOpen3_call.pzlib_filefunc64_32_def = nullptr;
    g_zipOpen3_call_count = 0;
}

// Mock implementation of zipOpen3 to verify how zipOpen2 delegates arguments.
// We declare it with C linkage to override the real (link-time) symbol during tests.
extern "C" zipFile ZEXPORT zipOpen3(const void* pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_32_def* pzlib_filefunc64_32_def) {
    g_last_zipOpen3_call.pathname = pathname;
    g_last_zipOpen3_call.append = append;
    g_last_zipOpen3_call.globalcomment = globalcomment;
    g_last_zipOpen3_call.pzlib_filefunc64_32_def = pzlib_filefunc64_32_def;
    ++g_zipOpen3_call_count;
    // Return a sentinel so tests can verify that zipOpen2 relayed the result
    return SENTINEL_ZIPOPEN3;
}

// Simple non-terminating assertion macro.
// If a condition fails, report but do not abort the test suite.
#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        ++g_test_failures; \
        std::cerr << "[ASSERT FAILED] " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
    } \
} while (0)

static int g_test_failures = 0;

// Test 1: Null zlib_filefunc32_def -> zipOpen3 should be called with NULL pzlib_filefunc64_32_def
static void test_zipOpen2_null_def_forwards_null_struct() {
    reset_last_call_info();

    const char* path = "test_null_def.zip";
    zipcharpc gc = (zipcharpc) "GLOBAL_TAG";
    zipcharpc* pgc = &gc;

    // Call the function under test
    zipFile result = zipOpen2(path, 0, pgc, NULL);

    // Validate behavior
    ASSERT(g_zipOpen3_call_count == 1, "zipOpen3 should be called exactly once when pzlib_filefunc32_def is NULL");
    ASSERT(g_last_zipOpen3_call.pathname == path, "zipOpen3 should receive the same pathname as passed to zipOpen2");
    ASSERT(g_last_zipOpen3_call.append == 0, "zipOpen3 should receive the same append flag as zipOpen2");
    ASSERT(g_last_zipOpen3_call.globalcomment == pgc, "zipOpen3 should receive the same globalcomment pointer as zipOpen2");
    ASSERT(g_last_zipOpen3_call.pzlib_filefunc64_32_def == NULL, "zipOpen3 pzlib_filefunc64_32_def must be NULL when input is NULL");
    ASSERT(result == SENTINEL_ZIPOPEN3, "zipOpen2 should return the value produced by zipOpen3 (sentinel)");
}

// Test 2: Non-NULL zlib_filefunc32_def -> zipOpen3 should be called with a non-NULL pzlib_filefunc64_32_def
static void test_zipOpen2_nonnull_def_forwards_nonnull_struct() {
    reset_last_call_info();

    const char* path = "test_nonnull_def.zip";
    zipcharpc gc = (zipcharpc) "NONNULL_GLOBAL";
    zipcharpc* pgc = &gc;
    zlib_filefunc_def fdef; // Provided to zipOpen2 to trigger the non-NULL path

    // Call the function under test
    zipFile result = zipOpen2(path, 1, pgc, &fdef);

    // Validate behavior
    ASSERT(g_zipOpen3_call_count == 1, "zipOpen3 should be called exactly once when pzlib_filefunc32_def is non-NULL");
    ASSERT(g_last_zipOpen3_call.pathname == path, "zipOpen3 should receive the same pathname as passed to zipOpen2");
    ASSERT(g_last_zipOpen3_call.append == 1, "zipOpen3 should receive the same append flag as zipOpen2");
    ASSERT(g_last_zipOpen3_call.globalcomment == pgc, "zipOpen3 should receive the same globalcomment pointer as zipOpen2");
    // The 64-bit function table pointer should be non-NULL after filling via zipOpen2
    ASSERT(g_last_zipOpen3_call.pzlib_filefunc64_32_def != NULL, "zipOpen3 should receive a non-NULL filled zlib_filefunc64_32_def when non-NULL input is provided");
    ASSERT(result == SENTINEL_ZIPOPEN3, "zipOpen2 should return the value produced by zipOpen3 (sentinel)");
}

// Test 3: Non-NULL def, NULL globalcomment -> ensure globalcomment forwarding works (NULL is tolerated)
static void test_zipOpen2_nonnull_def_with_null_globalcomment() {
    reset_last_call_info();

    const char* path = "test_nonnull_def_null_gc.zip";
    zipcharpc* pgc = nullptr; // NULL globalcomment
    zlib_filefunc_def fdef;

    zipFile result = zipOpen2(path, 0, pgc, &fdef);

    ASSERT(g_zipOpen3_call_count == 1, "zipOpen3 should be called exactly once when pzlib_filefunc32_def is non-NULL (NULL globalcomment)");
    ASSERT(g_last_zipOpen3_call.pathname == path, "zipOpen3 should receive the same pathname as passed to zipOpen2");
    ASSERT(g_last_zipOpen3_call.globalcomment == nullptr, "zipOpen3 should receive NULL globalcomment when caller passes NULL");
    ASSERT(result == SENTINEL_ZIPOPEN3, "zipOpen2 should return the value produced by zipOpen3 (sentinel)");
}

// Runner
int main() {
    // Explain test intent
    std::cout << "Starting unit tests for zipOpen2 behavior (mocked zipOpen3 observer)." << std::endl;

    test_zipOpen2_null_def_forwards_null_struct();
    test_zipOpen2_nonnull_def_forwards_nonnull_struct();
    test_zipOpen2_nonnull_def_with_null_globalcomment();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
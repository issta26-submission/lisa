/*
  Lightweight C++11 unit test suite for the focal method:
  extern int ZEXPORT zipOpenNewFileInZip64(zipFile file, const char* filename,
                                           const zip_fileinfo* zipfi,
                                           const void* extrafield_local, uInt size_extrafield_local,
                                           const void* extrafield_global, uInt size_extrafield_global,
                                           const char* comment, int method, int level, int zip64)

  The tests follow the domain knowledge constraints:
  - No Google Test; a small in-house test harness is used.
  - Tests exercise the behavior of zipOpenNewFileInZip64 by comparing its return value
    against a direct call to zipOpenNewFileInZip4_64 with identical parameters except
    for the fixed forwarding behavior (raw=0, windowBits=-MAX_WBITS, memLevel=DEF_MEM_LEVEL,
    strategy=Z_DEFAULT_STRATEGY, password=NULL, crcForCrypting=0, versionMadeBy=VERSIONMADEBY,
    flagBase=0, and the zip64 flag passed through).
  - True/false branches are covered by varying the zip64 flag (0 and 1).
  - Uses only the C++ standard library; relies on the provided zip.h / zip.c interfaces.
  - Non-terminating assertions implemented (do not exit on failure) so that multiple tests run.
  - Static members: not applicable here; we avoid private/public member access concerns.

  Build note:
  - Link with the corresponding C sources (e.g., zip.c) and include zip.h in compile units.
  - For example: g++ -std=c++11 -I<path-to-include> test_zip_open_new_file_in_zip64.cpp zip.c -o test_zip
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <time.h>
#include <string>
#include <functional>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sstream>
#include <stdlib.h>
#include <zip.h>


// Lightweight test harness
struct TestCase {
    std::string name;
    std::function<void()> run;
};

// Global test results
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion helpers
static void reportFailure(const char* file, int line, const std::string& msg) {
    std::cerr << "TEST FAIL: " << msg
              << " (" << file << ":" << line << ")" << std::endl;
    ++g_tests_failed;
}
#define TEST_ASSERT_TRUE(cond, msg) \
    do { if(!(cond)) { reportFailure(__FILE__, __LINE__, (msg)); } } while(0)
#define TEST_ASSERT_EQ(a, b, msg) \
    do { if(!((a) == (b))) { \
            std::ostringstream oss; \
            oss << (msg) << " | expected: " << (a) << ", actual: " << (b); \
            reportFailure(__FILE__, __LINE__, oss.str()); \
        } } while(0)

static std::vector<TestCase> g_tests;

// Helper to register a test
static void register_test(const std::string& name, std::function<void()> f) {
    g_tests.push_back(TestCase{name, f});
}

// Cleanup helper for test artifacts
static void cleanup_zip_file(const char* path) {
    if (path != nullptr) {
        std::remove(path);
    }
}

// Domain knowledge comments: test design rationale
// - We verify that zipOpenNewFileInZip64 forwards parameters to zipOpenNewFileInZip4_64
//   by comparing return codes for the same inputs with zip64 set to 0 and 1 respectively.
// - We run tests with real zip files opened via zipOpen to ensure the API behaves
//   under realistic usage (opening a zip, adding a file entry).
// - We avoid private/internal details and rely on the public API surface.


// Test 1: zipOpenNewFileInZip64 forwards to zipOpenNewFileInZip4_64 when zip64 = 0
static void test_zipOpenNewFileInZip64_forwards_to_4_64_zip64_off() {
    // Create a real zip archive to operate on
    const char* zip_path = "test_forward_zip64_off.zip";

    zipFile zf = zipOpen(zip_path, 0);
    TEST_ASSERT_TRUE(zf != NULL, "zipOpen should create a valid zipFile for zip_path");

    // Call the focal method with zip64 = 0
    int res_zip64 = zipOpenNewFileInZip64(
        zf,
        "entry_off.txt",
        NULL,              // zipfi
        NULL, 0,           // extrafield_local
        NULL, 0,           // extrafield_global
        "comment",           // comment
        0,                   // method
        0,                   // level
        0                    // zip64
    );

    // Direct call to the underlying implementation that the wrapper delegates to
    int res_4_64 = zipOpenNewFileInZip4_64(
        zf,
        "entry_off.txt",
        NULL,
        NULL, 0,
        NULL, 0,
        "comment",
        0,
        0,                    // raw
        -MAX_WBITS,           // windowBits
        DEF_MEM_LEVEL,         // memLevel
        Z_DEFAULT_STRATEGY,      // strategy
        NULL,                  // password
        0,                     // crcForCrypting
        0                      // zip64
    );

    // Both calls should produce the same return value if the wrapper forwards correctly
    TEST_ASSERT_EQ(res_zip64, res_4_64, "zipOpenNewFileInZip64 (zip64=0) should forward to zipOpenNewFileInZip4_64 with identical result");

    // Cleanup
    zipClose(zf, NULL);
    cleanup_zip_file(zip_path);
}

// Test 2: zipOpenNewFileInZip64 forwards to zipOpenNewFileInZip4_64 when zip64 = 1
static void test_zipOpenNewFileInZip64_forwards_to_4_64_zip64_on() {
    const char* zip_path = "test_forward_zip64_on.zip";

    zipFile zf = zipOpen(zip_path, 0);
    TEST_ASSERT_TRUE(zf != NULL, "zipOpen should create a valid zipFile for zip_path");

    // Call the focal method with zip64 = 1
    int res_zip64 = zipOpenNewFileInZip64(
        zf,
        "entry_on.txt",
        NULL,
        NULL, 0,
        NULL, 0,
        "comment2",
        0,
        0,
        1
    );

    int res_4_64 = zipOpenNewFileInZip4_64(
        zf,
        "entry_on.txt",
        NULL,
        NULL, 0,
        NULL, 0,
        "comment2",
        0,
        0,
        -MAX_WBITS,
        DEF_MEM_LEVEL,
        Z_DEFAULT_STRATEGY,
        NULL,
        0,
        1
    );

    TEST_ASSERT_EQ(res_zip64, res_4_64, "zipOpenNewFileInZip64 (zip64=1) should forward to zipOpenNewFileInZip4_64 with identical result");

    zipClose(zf, NULL);
    cleanup_zip_file(zip_path);
}

// Test 3: Consistency check with null file pointer (edge-case handling)
// Note: Depending on the underlying implementation, passing a NULL zipFile may fail.
// We ensure both wrappers return the same result for a NULL file pointer to validate
// forwarding path behaves consistently in error scenarios.
static void test_zipOpenNewFileInZip64_with_null_file_consistency() {
    // Call with NULL zipFile for both implementations
    int res_zip64 = zipOpenNewFileInZip64(
        NULL,
        "null_entry.txt",
        NULL, NULL, 0, NULL, 0,
        "comment-null",
        0, 0, 0
    );

    int res_4_64 = zipOpenNewFileInZip4_64(
        NULL,
        "null_entry.txt",
        NULL, NULL, 0, NULL, 0,
        "comment-null",
        0, 0, 0,
        -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
        NULL, 0, 0
    );

    TEST_ASSERT_EQ(res_zip64, res_4_64, "zipOpenNewFileInZip64 with NULL file should forward to zipOpenNewFileInZip4_64 with identical result");
}

// Runner
static void run_all_tests() {
    for (const auto& t : g_tests) {
        // Reset per-test state if needed
        std::cout << "[RUN] " << t.name << std::endl;
        t.run();
        ++g_tests_run;
    }

    int total = g_tests_run;
    int failed = g_tests_failed;
    std::cout << "\nTest results: " << (total - failed) << " passed, " << failed << " failed, " << total << " total." << std::endl;
}

// Macro-friendly helper for registering tests (executed at global scope)
static void register_all_tests() {
    register_test("zipOpenNewFileInZip64_forwards_to_4_64_zip64_off", test_zipOpenNewFileInZip64_forwards_to_4_64_zip64_off);
    register_test("zipOpenNewFileInZip64_forwards_to_4_64_zip64_on", test_zipOpenNewFileInZip64_forwards_to_4_64_zip64_on);
    register_test("zipOpenNewFileInZip64_with_null_file_consistency", test_zipOpenFileWithNull_consistency);
}

// Note: Corrected naming to reflect actual test function above
static void test_zipOpenFileWithNull_consistency() { test_zipOpenNewFileInZip64_with_null_file_consistency(); }

// Main
int main() {
    // Register tests
    register_all_tests();

    // Run tests
    run_all_tests();

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
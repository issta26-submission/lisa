// This test suite targets the focal function: unzGetGlobalInfo
// It is designed to be self-contained in a single translation unit.
// The test suite uses C++11 and does not rely on GTest. It executes tests from main()
// and reports pass/fail for each test case with explanatory comments.
//
// Important: To access the internal structures necessary for testing (e.g., unz64_s and its
// internal gi member), we include the original unzip.c file in this translation unit.
// This allows direct construction of an unz64_s instance and setting its fields prior
// to invoking unzGetGlobalInfo. The tests assume a typical minizip/unzip.c layout where
// unz64_s contains a member `gi` of type unz_global_info64 (which in turn contains
// number_entry and size_comment fields). The public API uses a public unz_global_info32
// (often named unz_global_info) to return these fields.

#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <unzip.c>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Include the unzip implementation to access internal types (e.g., unz64_s)
// so we can construct a mock file handle and verify behavior.


// Simple test harness utilities
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define LOG_PASS(test_name) do { std::cout << "[PASS] " << test_name << "\n"; } while(0)
#define LOG_FAIL(test_name, msg) do { std::cerr << "[FAIL] " << test_name << ": " << msg << "\n"; ++g_failed_tests; } while(0)

static void report_summary() {
    std::cout << "\nTest Summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures.\n";
}


// Test 1: NULL file should return UNZ_PARAMERROR
// This exercises the early parameter validation path of unzGetGlobalInfo.
static bool test_unzGetGlobalInfo_null_file() {
    const char* test_name = "test_unzGetGlobalInfo_null_file";
    ++g_total_tests;

    unz_global_info info32; // Out parameter to capture values (not used in this test)
    int ret = unzGetGlobalInfo(nullptr, &info32);

    if (ret != UNZ_PARAMERROR) {
        LOG_FAIL(test_name, "Expected UNZ_PARAMERROR when file is NULL, got different return code.");
        return false;
    } else {
        LOG_PASS(test_name);
        return true;
    }
}


// Test 2: Valid file handle with known gi values should copy number_entry and size_comment
// This ensures that the function correctly reads internal gi fields from the provided file
// handle and stores them into the public unz_global_info32 structure.
static bool test_unzGetGlobalInfo_copy_fields() {
    const char* test_name = "test_unzGetGlobalInfo_copy_fields";
    ++g_total_tests;

    // Create a real unz64_s instance (as provided by unzip.c)
    unz64_s s;

    // Initialize internal global info fields that unzGetGlobalInfo will read.
    // The exact structure of gi depends on unzip.c; here we rely on the common
    // minizip layout where gi.number_entry and gi.size_comment exist.
    s.gi.number_entry = 12345;   // example value
    s.gi.size_comment = 678;     // example value

    unz_global_info info32;
    int ret = unzGetGlobalInfo((unzFile)&s, &info32);

    if (ret != UNZ_OK) {
        LOG_FAIL(test_name, "Expected UNZ_OK for valid file handle, got different return code.");
        return false;
    }

    if (info32.number_entry != (uLong)s.gi.number_entry) {
        LOG_FAIL(test_name, "number_entry value mismatch after unzGetGlobalInfo call.");
        return false;
    }

    if (info32.size_comment != s.gi.size_comment) {
        LOG_FAIL(test_name, "size_comment value mismatch after unzGetGlobalInfo call.");
        return false;
    }

    LOG_PASS(test_name);
    return true;
}


// Test 3: Another combination of internal gi values to verify robustness
// Ensures the function does not accidentally alter values and correctly copies multiple fields.
static bool test_unzGetGlobalInfo_copy_another_set() {
    const char* test_name = "test_unzGetGlobalInfo_copy_another_set";
    ++g_total_tests;

    unz64_s s;

    // Different test values
    s.gi.number_entry = 0x1A2B3C4D;  // large-ish value to exercise handling
    s.gi.size_comment = 0xDEAD;       // another distinct value

    unz_global_info info32;
    int ret = unzGetGlobalInfo((unzFile)&s, &info32);

    if (ret != UNZ_OK) {
        LOG_FAIL(test_name, "Expected UNZ_OK with another set of gi values, got different return code.");
        return false;
    }

    if (info32.number_entry != (uLong)s.gi.number_entry) {
        LOG_FAIL(test_name, "number_entry mismatch for alternate gi values.");
        return false;
    }

    if (info32.size_comment != s.gi.size_comment) {
        LOG_FAIL(test_name, "size_comment mismatch for alternate gi values.");
        return false;
    }

    LOG_PASS(test_name);
    return true;
}


// Entry point: run tests and print summary
int main() {
    // Run tests
    bool t1 = test_unzGetGlobalInfo_null_file();
    bool t2 = test_unzGetGlobalInfo_copy_fields();
    bool t3 = test_unzGetGlobalInfo_copy_another_set();

    // Accumulate results in summary
    report_summary();

    // Return non-zero if any test failed
    if (g_failed_tests > 0) {
        return 1;
    }
    return 0;
}
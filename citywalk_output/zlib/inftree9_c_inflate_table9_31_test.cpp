// C++11 unit test suite for inflate_table9 (inftree9.c) without using Google Test.
// The tests are designed to compile with the existing project structure (C headers
// like inftree9.h and zutil.h) and call the C function from C++.
// Each test includes explanatory comments describing intent and coverage goals.

#include <iostream>
#include <string>
#include <inftree9.h>
#include <zutil.h>


// Bring in C headers and types used by inflate_table9.
// We rely on the project's existing headers to define codetype, code, and FAR.
// Use extern "C" to ensure correct linkage when calling the C function from C++.
extern "C" {
}

// Ensure we have a prototype in case the headers are not picked up exactly as-is.
extern "C" int inflate_table9(codetype type,
                             unsigned short FAR *lens,
                             unsigned codes,
                             code FAR * FAR *table,
                             unsigned FAR *bits,
                             unsigned short FAR *work);

///////////////////////////////////////////////////////////////////
// Minimal test framework (no external test libraries)
///////////////////////////////////////////////////////////////////

// Simple assertion helper
static bool assert_true(bool condition, const char* msg) {
    if (!condition) {
        std::cerr << "ASSERTION FAILED: " << msg << std::endl;
    }
    return condition;
}

// Global counters for test results
static int g_total = 0;
static int g_passed = 0;

static void report_result(const std::string& name, bool passed) {
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
}

///////////////////////////////////////////////////////////////////
// Test 1: No codes provided should return -1 (max == 0 branch)
// This exercises the path where there are no symbols to encode and
// ensures the function reports an error as specified.
///////////////////////////////////////////////////////////////////

bool test_inflate_table9_no_codes_returns_minus_one() {
    // Arrange
    codetype type = CODES;               // Using CODES to hit the single-root table path
    unsigned codes = 0;                    // No codes
    unsigned short lens[1] = {0};          // Lens array length matches codes (0)
    unsigned short work[256];              // Scratch work area (arbitrary size)
    unsigned long bits_local = 0;          // Output bits parameter (will be adjusted by function)
    code table_storage[1024];                // Root table storage
    code FAR *table_root = table_storage;
    code FAR * FAR *table = &table_root;     // Pointer-to-table as required by signature

    // Act
    int ret = inflate_table9(type, lens, codes, table, &bits_local, work);

    // Assert
    // Expected: -1 (no codes)
    return assert_true(ret == -1, "inflate_table9 with zero codes should return -1");
}

///////////////////////////////////////////////////////////////////
// Test 2: DISTS path with a single code to cover work[sym] > end branch.
// This exercises the DISTS branch where end = -1 and thus the
// code takes the (work[sym] > end) path, using base/dext from dbase/dext.
// We keep codes small to avoid requiring large static tables yet cover the path.
///////////////////////////////////////////////////////////////////

bool test_inflate_table9_dists_single_symbol() {
    // Arrange
    codetype type = DISTS;
    unsigned codes = 1;
    unsigned short lens[1] = {1};                    // length value (1 is valid)
    unsigned short work[256];
    unsigned short table_storage[512];                 // ample storage for table
    code FAR *table_root = table_storage;
    code FAR * FAR *table = &table_root;
    unsigned bits = 1;                                // root table size (2 entries)
    // Initialize work to a known state (the function will overwrite as needed)
    for (int i = 0; i < 256; ++i) work[i] = 0;

    // Act
    int ret = inflate_table9(type, lens, codes, table, &bits, work);

    // Assert
    // Expecting success for a minimal valid DISTS table setup
    return assert_true(ret == 0, "inflate_table9 with single DISTS code should return 0");
}

///////////////////////////////////////////////////////////////////
// Test 3: LENS path with a single code to cover the basic path.
// This exercises the LENS branch where end = 256, and (work[sym] < end)
// leading to the "short code" path (no extra bits handling).
///////////////////////////////////////////////////////////////////

bool test_inflate_table9_lens_single_symbol() {
    // Arrange
    codetype type = LENS;
    unsigned codes = 1;
    unsigned short lens[1] = {1};                     // minimal valid code length
    unsigned short work[256];
    unsigned short table_storage[512];
    code FAR *table_root = table_storage;
    code FAR * FAR *table = &table_root;
    unsigned bits = 2;                                // root table lots of entries (4)
    for (int i = 0; i < 256; ++i) work[i] = 0;

    // Act
    int ret = inflate_table9(type, lens, codes, table, &bits, work);

    // Assert
    return assert_true(ret == 0, "inflate_table9 with single LENS code should return 0");
}

///////////////////////////////////////////////////////////////////
// Main: Run all tests and report summary.
///////////////////////////////////////////////////////////////////

int main() {
    std::cout << "Running inflate_table9 unit tests (C++ harness, no GTest)..." << std::endl;

    // Run tests
    report_result("test_inflate_table9_no_codes_returns_minus_one",
                  test_inflate_table9_no_codes_returns_minus_one());

    report_result("test_inflate_table9_dists_single_symbol",
                  test_inflate_table9_dists_single_symbol());

    report_result("test_inflate_table9_lens_single_symbol",
                  test_inflate_table9_lens_single_symbol());

    // Summary
    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << std::endl;

    // Return non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}
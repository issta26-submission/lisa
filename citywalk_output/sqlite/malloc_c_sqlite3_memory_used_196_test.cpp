// Test suite for the focal method: sqlite3_memory_used (located in malloc.c)
// Goal: verify that sqlite3_memory_used correctly delegates to sqlite3_status64
// and returns the current memory usage value (as provided by the mock sqlite3_status64).
// Notes:
// - We provide a C-compatible mock for sqlite3_status64 to control the value returned
//   via the pointer parameters (pCur and pHigh). The test does not rely on static
//   internal state of the source file and only exercises the observable behavior.
// - This test is written for a C++11 environment without Google Test. It uses a tiny
//   in-house test harness with non-terminating assertions (print failures and continue).
// - Static/free functions inside the focal file are not invoked directly; the test
//   targets the public API: sqlite3_memory_used().

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <stdarg.h>


// Domain knowledge mapping (Candidate Keywords):
// - sqlite3_memory_used
// - sqlite3_status64
// - SQLITE_STATUS_MEMORY_USED
// - sqlite3_int64
// These are the core components required to test the focal method’s behavior.

// 1) Type alias that mirrors the sqlite3_int64 type used by the focal code.
typedef long long sqlite3_int64;

// 2) Forward declaration of the focal function under test.
//    The focal function is defined in a C source file (malloc.c). We declare it
//    here with C linkage to enable linkage with the C implementation.
extern "C" sqlite3_int64 sqlite3_memory_used(void);

// 3) Mock infrastructure for sqlite3_status64.
//    We provide a C-compatible definition so that malloc.c links against it.
//    The mock simply writes a controlled value into *pCur and *pHigh.
extern "C" {

// Global mock state, controlled by tests.
static sqlite3_int64 g_mock_res = 0;
static sqlite3_int64 g_mock_mx  = 0;

// Mock implementation of sqlite3_status64.
// Signature must match the one used by the focal code.
// We ignore 'op' and 'resetFlag' since we only need to supply the current/max values.
void sqlite3_status64(int op, sqlite3_int64 *pCur, sqlite3_int64 *pHigh, sqlite3_int64 resetFlag) {
    (void)op;      // unused in mock
    (void)resetFlag;// unused in mock
    if (pCur) *pCur = g_mock_res;
    if (pHigh) *pHigh = g_mock_mx;
}
} // extern "C"

// Helper: non-terminating assertion that reports mismatch but continues.
static bool expect_eq(sqlite3_int64 actual, sqlite3_int64 expected, const char* label) {
    if (actual == expected) {
        std::cout << "[OK] " << label << ": expected " << expected << ", got " << actual << "\n";
        return true;
    } else {
        std::cerr << "[FAIL] " << label << ": expected " << expected << ", got " << actual << "\n";
        return false;
    }
}

// Helper: test wrapper that sets mock values and invokes sqlite3_memory_used.
static bool test_memory_used_with_mock(sqlite3_int64 mockRes) {
    g_mock_res = mockRes;
    // We don't rely on g_mock_mx for this test; a consistent value is enough.
    g_mock_mx  = 0;
    sqlite3_int64 result = sqlite3_memory_used();
    return expect_eq(result, mockRes, "sqlite3_memory_used returns controlled mock value");
}

// Test 1: Basic correctness - ensure memory_used reflects the current mock 'res' value.
static bool test_memory_used_basic_case() {
    // Explanatory comment:
    // Set mock current memory used to 42 and verify sqlite3_memory_used returns 42.
    return test_memory_used_with_mock(42);
}

// Test 2: Negative and large values handling - ensure sign and large values propagate.
static bool test_memory_used_negative_and_large() {
    // Explanatory comment:
    // Use a negative value to ensure signed handling and a large value to ensure wide range.
    bool ok = true;
    g_mock_res = -7;
    g_mock_mx  = 1234567890123LL;
    sqlite3_int64 resultNeg = sqlite3_memory_used();
    ok &= expect_eq(resultNeg, -7, "sqlite3_memory_used handles negative value");

    // Change mock to a large positive value
    g_mock_res = 9223372036854775807LL; // max 64-bit signed
    sqlite3_int64 resultMax = sqlite3_memory_used();
    ok &= expect_eq(resultMax, 9223372036854775807LL, "sqlite3_memory_used handles max int64");
    return ok;
}

// Test 3: Multiple consecutive calls reflect updated mock values.
// Explanatory comment:
// Ensure successive calls return updated values when the mock state changes.
static bool test_memory_used_consecutive_updates() {
    g_mock_res = 100;
    sqlite3_int64 r1 = sqlite3_memory_used();
    bool ok1 = expect_eq(r1, 100, "First consecutive call returns 100");

    g_mock_res = 200;
    sqlite3_int64 r2 = sqlite3_memory_used();
    bool ok2 = expect_eq(r2, 200, "Second consecutive call returns 200");

    return ok1 && ok2;
}

// Minimal test runner
static void run_all_tests() {
    int tests_passed = 0;
    int total_tests  = 0;

    // Test 1
    total_tests++;
    if (test_memory_used_basic_case()) tests_passed++;

    // Test 2
    total_tests++;
    if (test_memory_used_negative_and_large()) tests_passed++;

    // Test 3
    total_tests++;
    if (test_memory_used_consecutive_updates()) tests_passed++;

    std::cout << "\nSummary: " << tests_passed << " / " << total_tests << " tests passed.\n";
}

int main() {
    // Run the suite. All tests exercise the focal method sqlite3_memory_used
    // via a controlled sqlite3_status64 mock.
    run_all_tests();
    return 0;
}

// End of test suite for sqlite3_memory_used
// This test is designed to be compiled together with the production malloc.c
// (which defines sqlite3_memory_used) to ensure correct linkage and behavior.
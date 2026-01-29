// Test suite for sqlite3ErrStr(int rc) implemented in main.c
// This file is designed for C++11 compilation without Google Test.
// It calls the focal function via C linkage and asserts expected string mappings.
// The tests are written to maximize execution coverage, including true/false branches
// of the function's internal switch/default logic.
//
// Domain knowledge notes applied:
// - We test all switch cases: SQLITE_ABORT_ROLLBACK, SQLITE_ROW, SQLITE_DONE
// - We test the default path with both in-range non-NULL messages and NULL entries (0)
// - We test the rc masked with 0xff path and "unknown error" default
// - We account for compile-time differences around SQLITE_NOLFS (defined by SQLITE_DISABLE_LFS)
// - We avoid accessing private/internal state; we only exercise the focal function's public behavior

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Forward declaration of the focal function (C linkage)
extern "C" const char* sqlite3ErrStr(int rc);

static int g_total = 0;
static int g_passed = 0;

// Simple non-terminating assertion helper for test cases
static void test_assert(const char* testName, bool condition) {
    ++g_total;
    if (condition) {
        ++g_passed;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// Helper to compare two C strings safely
static bool streq(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) return a == b;
    return std::strcmp(a, b) == 0;
}

// Test suite for sqlite3ErrStr
static void run_sqlite3ErrStr_tests() {
    // Test 1: rc = SQLITE_OK should map to "not an error"
    // True branch: aMsg[0] == "not an error" -> zErr should be that string
    test_assert("rc=SQLITE_OK -> 'not an error'", streq(sqlite3ErrStr(0), "not an error"));

    // Test 2: rc = SQLITE_ERROR should map to "SQL logic error"
    test_assert("rc=SQLITE_ERROR -> 'SQL logic error'", streq(sqlite3ErrStr(SQLITE_ERROR), "SQL logic error"));

    // Test 3: rc = SQLITE_PERM should map to "access permission denied"
    test_assert("rc=SQLITE_PERM -> 'access permission denied'",
                streq(sqlite3ErrStr(SQLITE_PERM), "access permission denied"));

    // Test 4: rc = SQLITE_ROW should map to "another row available" (special switch-case)
    test_assert("rc=SQLITE_ROW -> 'another row available'",
                streq(sqlite3ErrStr(SQLITE_ROW), "another row available"));

    // Test 5: rc = SQLITE_DONE should map to "no more rows available"
    test_assert("rc=SQLITE_DONE -> 'no more rows available'",
                streq(sqlite3ErrStr(SQLITE_DONE), "no more rows available"));

    // Test 6: rc = SQLITE_ABORT_ROLLBACK should map to "abort due to ROLLBACK" (special-case)
    test_assert("rc=SQLITE_ABORT_ROLLBACK -> 'abort due to ROLLBACK'",
                streq(sqlite3ErrStr(SQLITE_ABORT_ROLLBACK), "abort due to ROLLBACK"));

    // Test 7: rc with in-range value whose aMsg entry is 0 (e.g., SQLITE_INTERNAL)
    // Expected: "unknown error" since aMsg[SQLITE_INTERNAL] is 0
    test_assert("rc=SQLITE_INTERNAL -> 'unknown error'",
                streq(sqlite3ErrStr(SQLITE_INTERNAL), "unknown error"));

    // Test 8: rc outside the aMsg bounds after masking (e.g., rc = 0xff)
    // Expected: "unknown error"
    test_assert("rc=0xff (255) -> 'unknown error'",
                streq(sqlite3ErrStr(0xff), "unknown error"));

    // Test 9: rc = SQLITE_NOTFOUND should map to "unknown operation"
    test_assert("rc=SQLITE_NOTFOUND -> 'unknown operation'",
                streq(sqlite3ErrStr(SQLITE_NOTFOUND), "unknown operation"));

    // Test 10: rc = SQLITE_FULL should map to "database or disk is full"
    test_assert("rc=SQLITE_FULL -> 'database or disk is full'",
                streq(sqlite3ErrStr(SQLITE_FULL), "database or disk is full"));

    // Test 11: rc = SQLITE_AUTH should map to "authorization denied"
    test_assert("rc=SQLITE_AUTH -> 'authorization denied'",
                streq(sqlite3ErrStr(SQLITE_AUTH), "authorization denied"));

    // Test 12: NOLFS behavior (compile-time dependent)
    // Expect either the disabled message or the generic "unknown error" depending on build config.
    const char* s = sqlite3ErrStr(SQLITE_NOLFS);
#if defined(SQLITE_DISABLE_LFS)
    test_assert("rc=SQLITE_NOLFS -> 'large file support is disabled' (when LFS enabled flag is defined)",
                streq(s, "large file support is disabled"));
#else
    test_assert("rc=SQLITE_NOLFS -> either 'unknown error' or LFS-related string",
                streq(s, "unknown error"));
#endif

    // Test 13: rc negative (-1) should map to "unknown error" (bitwise-AND path)
    test_assert("rc=-1 -> 'unknown error'",
                streq(sqlite3ErrStr(-1), "unknown error"));
}

// Entry point
int main() {
    std::cout << "Starting sqlite3ErrStr unit tests (C++11)..." << std::endl;
    run_sqlite3ErrStr_tests();

    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed << std::endl;
    // Exit non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}
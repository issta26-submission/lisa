// SQLite sqlite3FindDb unit tests using a lightweight, self-contained harness.
// This test suite mocks the minimal external dependencies required by sqlite3FindDb
// (sqlite3NameFromToken, sqlite3FindDbName, sqlite3DbFree) and includes the focal code
// from build.c by including it in this translation unit.
// The tests are written in C++11 without GoogleTest and use a small, non-terminating
// assertion style to maximize coverage across test cases.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <build.c>


// Ensure SQLITE_NOINLINE present in case build.c uses it.
#ifndef SQLITE_NOINLINE
#define SQLITE_NOINLINE
#endif

// Forward declare minimal types used by the focal method and mocks.
struct sqlite3;           // Minimal stub for sqlite3
typedef struct Token { const char *zName; } Token; // Minimal Token compatible with test mocks

// Provide a simple sqlite3 type definition to satisfy build.c prototypes.
struct sqlite3 {
    int dummy;
};

// Global variables to verify memory management in mocks.
static char *g_lastAllocated = nullptr; // Pointer returned by sqlite3NameFromToken
static char *g_lastFreed = nullptr;      // Pointer passed to sqlite3DbFree

// Mock implementations of dependencies used by sqlite3FindDb
extern "C" {

// Return a freshly allocated C-string based on the token's name.
// We'll simulate sqlite3NameFromToken by duplicating the token's string.
char* sqlite3NameFromToken(sqlite3 *db, const Token *pName) {
    if (pName == nullptr) return nullptr;
    // Allocate and copy the token name to simulate SQLite behavior.
    g_lastAllocated = strdup(pName->zName);
    return g_lastAllocated;
}

// Simple mapping: if the name equals "main" return 0, otherwise return 1.
// This allows us to test both branches of sqlite3FindDbName.
int sqlite3FindDbName(sqlite3 *db, const char *zName) {
    if (zName && std::strcmp(zName, "main") == 0) {
        return 0;
    }
    return 1;
}

// Free the allocated token name and record the freed pointer for verification.
void sqlite3DbFree(sqlite3 *db, char *pName) {
    g_lastFreed = pName;
    std::free(pName);
}
}

// Include the focal implementation from build.c
// This will pull in the sqlite3FindDb function under test, using the mocked dependencies above.

// Lightweight test harness

// Utility macro for non-terminating assertions with log messages
#define TEST_ASSERT_EQ(expected, actual, msg)                           \
    do {                                                                 \
        if ((expected) != (actual)) {                                    \
            std::cerr << "[FAIL] " << (msg)                               \
                      << " | expected: " << (expected)                    \
                      << ", actual: " << (actual) << std::endl;        \
            return false;                                                \
        }                                                                \
        /* no else: continue on pass */                                 \
    } while (0)

// Test 1: When token name is "main", sqlite3FindDb should return 0
// and the allocated token string should be freed by sqlite3DbFree.
bool test_findDb_main_returns0() {
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    Token t;
    t.zName = "main";

    g_lastAllocated = nullptr;
    g_lastFreed = nullptr;

    int result = sqlite3FindDb(&db, &t);

    TEST_ASSERT_EQ(0, result, "sqlite3FindDb should return 0 for 'main'");

    // Memory sanity: last freed pointer should match the allocated pointer.
    TEST_ASSERT_EQ(g_lastAllocated, g_lastFreed, "sqlite3DbFree should free the same pointer returned by sqlite3NameFromToken");

    return true;
}

// Test 2: When token name is not "main" (e.g., "other"), sqlite3FindDb should return 1
// and the allocated token string should be freed.
bool test_findDb_otherReturns1() {
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    Token t;
    t.zName = "other";

    g_lastAllocated = nullptr;
    g_lastFreed = nullptr;

    int result = sqlite3FindDb(&db, &t);

    TEST_ASSERT_EQ(1, result, "sqlite3FindDb should return 1 for non-'main' names");

    // Memory sanity: last freed pointer should match the allocated pointer.
    TEST_ASSERT_EQ(g_lastAllocated, g_lastFreed, "sqlite3DbFree should free the same pointer returned by sqlite3NameFromToken");

    return true;
}

// Test 3: Sequential calls to sqlite3FindDb with different tokens should independently
// allocate and free memory without leaking or mixing pointers.
bool test_findDb_sequentialCallsMemoryManagement() {
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    // First token: "main" -> expect 0
    {
        Token t1; t1.zName = "main";
        g_lastAllocated = nullptr;
        g_lastFreed = nullptr;

        int r1 = sqlite3FindDb(&db, &t1);
        TEST_ASSERT_EQ(0, r1, "First call should return 0 for 'main'");
        TEST_ASSERT_EQ(g_lastAllocated, g_lastFreed, "First call: free pointer should match allocated pointer");
    }

    // Second token: "xyz" -> expect 1
    {
        Token t2; t2.zName = "xyz";
        g_lastAllocated = nullptr;
        g_lastFreed = nullptr;

        int r2 = sqlite3FindDb(&db, &t2);
        TEST_ASSERT_EQ(1, r2, "Second call should return 1 for non-'main' name");
        TEST_ASSERT_EQ(g_lastAllocated, g_lastFreed, "Second call: free pointer should match allocated pointer");
    }

    return true;
}

// Entry point
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running sqlite3FindDb unit tests (C++11, gtest-free harness)..." << std::endl;

    // Run Test 1
    total++;
    if (test_findDb_main_returns0()) {
        passed++;
        std::cout << "[PASS] test_findDb_main_returns0" << std::endl;
    } else {
        std::cout << "[FAIL] test_findDb_main_returns0" << std::endl;
    }

    // Run Test 2
    total++;
    if (test_findDb_otherReturns1()) {
        passed++;
        std::cout << "[PASS] test_findDb_otherReturns1" << std::endl;
    } else {
        std::cout << "[FAIL] test_findDb_otherReturns1" << std::endl;
    }

    // Run Test 3
    total++;
    if (test_findDb_sequentialCallsMemoryManagement()) {
        passed++;
        std::cout << "[PASS] test_findDb_sequentialCallsMemoryManagement" << std::endl;
    } else {
        std::cout << "[FAIL] test_findDb_sequentialCallsMemoryManagement" << std::endl;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
/*
Unit test suite for sqlite3DbMallocZero(sqlite3 *db, u64 n)

Notes:
- This test targets the focal method sqlite3DbMallocZero defined in malloc.c.
- We rely on the real sqlite3 memory/db infrastructure (sqlite3_open, sqlite3_free, etc.)
  being available in the test build (typical in the provided project with sqliteInt.h / sqlite3.h).
- We implement a lightweight, non-terminating test harness (no GTest) that uses
  simple EXPECT-style assertions to maximize coverage without halting on failure.
- Tests cover:
  - Successful allocation path (p != nullptr) and zero-initialization (memset to zero)
  - Large allocation edge case where the allocator may fail (p == nullptr) to exercise the false branch
  - Basic resource management (opening/closing a temporary in-memory DB)
- The tests are written to compile with C++11 (no GTest, no mocks).
*/

#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Include SQLite API headers (assumes the project provides sqlite3.h and sqliteInt.h)

// Declaration of the focal function under test (C linkage)
extern "C" void *sqlite3DbMallocZero(sqlite3 *db, unsigned long long n);

// Declarations for memory management used by tests (as in SQLite)
extern "C" void sqlite3_free(void *p);

// Lightweight assertion framework (non-terminating)
static int g_failures = 0;
static std::string g_currentTest;

// Expectation macro: log failure but continue execution
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT] " << g_currentTest << " - " << (msg) \
                  << " (condition: " << #cond << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic non-null allocation path
// - Open an in-memory DB
// - Allocate a non-trivial size using sqlite3DbMallocZero
// - Verify allocation succeeds and memory is zero-initialized
static void test_sqlite3DbMallocZero_basicZeroing() {
    g_currentTest = "test_sqlite3DbMallocZero_basicZeroing";

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr, "sqlite3_open should provide a valid db handle");
    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_open should return SQLITE_OK");

    if (db && rc == SQLITE_OK) {
        const unsigned long long N = 128;
        void *p = sqlite3DbMallocZero(db, N);
        EXPECT_TRUE(p != nullptr, "sqlite3DbMallocZero should return non-null pointer for valid db and non-zero size");

        if (p) {
            // Verify that all bytes have been zeroed
            unsigned char *bytes = static_cast<unsigned char *>(p);
            bool allZero = true;
            for (unsigned long long i = 0; i < N; ++i) {
                if (bytes[i] != 0) {
                    allZero = false;
                    break;
                }
            }
            EXPECT_TRUE(allZero, "Allocated memory should be zero-initialized by sqlite3DbMallocZero");

            // Free allocated memory
            sqlite3_free(p);
        }

        sqlite3_close(db);
    }
}

// Test 2: Edge-case large allocation to exercise false path (p == nullptr)
// - Attempt allocation with an extremely large size; expect NULL or handled failure
// - This exercises the else-path (no memset) in sqlite3DbMallocZero
static void test_sqlite3DbMallocZero_largeN_returnsNull() {
    g_currentTest = "test_sqlite3DbMallocZero_largeN_returnsNull";

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr, "sqlite3_open should provide a valid db handle");
    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_open should return SQLITE_OK");

    if (db && rc == SQLITE_OK) {
        // Use a very large value to trigger allocation failure in typical allocators
        unsigned long long largeN = 0xFFFFFFFFFFFFFFFFULL; // 64-bit extreme size
        void *p = sqlite3DbMallocZero(db, largeN);
        // Depending on allocator, this commonly returns NULL. We only assert non-crashing behavior.
        EXPECT_TRUE(p == nullptr, "Large allocation should fail and return NULL (p == nullptr)");
        if (p) {
            // If by any chance it returns non-null, ensure we free to avoid leaks
            sqlite3_free(p);
        }

        sqlite3_close(db);
    }
}

// Test 3: Reuse/consistency across multiple allocations
// - Allocate two blocks and ensure they are independently zero-initialized
static void test_sqlite3DbMallocZero_multipleAllocations() {
    g_currentTest = "test_sqlite3DbMallocZero_multipleAllocations";

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr, "sqlite3_open should provide a valid db handle");
    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_open should return SQLITE_OK");

    if (db && rc == SQLITE_OK) {
        const unsigned long long N1 = 64;
        const unsigned long long N2 = 256;

        void *p1 = sqlite3DbMallocZero(db, N1);
        void *p2 = sqlite3DbMallocZero(db, N2);

        EXPECT_TRUE(p1 != nullptr, "First allocation should succeed");
        EXPECT_TRUE(p2 != nullptr, "Second allocation should succeed");

        if (p1 && p2) {
            unsigned char *a = static_cast<unsigned char *>(p1);
            unsigned char *b = static_cast<unsigned char *>(p2);
            bool zeros1 = true;
            for (unsigned long long i = 0; i < N1; ++i) if (a[i] != 0) { zeros1 = false; break; }

            bool zeros2 = true;
            for (unsigned long long i = 0; i < N2; ++i) if (b[i] != 0) { zeros2 = false; break; }

            EXPECT_TRUE(zeros1, "First allocated block should be zero-initialized");
            EXPECT_TRUE(zeros2, "Second allocated block should be zero-initialized");

            sqlite3_free(p1);
            sqlite3_free(p2);
        }

        sqlite3_close(db);
    }
}

// Simple runner
static void run_all_tests() {
    test_sqlite3DbMallocZero_basicZeroing();
    test_sqlite3DbMallocZero_largeN_returnsNull();
    test_sqlite3DbMallocZero_multipleAllocations();
}

int main() {
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "[TEST SUITE] All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUITE] " << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}
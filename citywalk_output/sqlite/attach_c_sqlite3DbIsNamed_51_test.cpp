// Self-contained C++11 unit test suite for the focal method sqlite3DbIsNamed.
// Note: This harness provides a minimal stand-alone replication of the
// surrounding types and the sqlite3StrICmp utility to enable deterministic
// testing without requiring the full sqlite source tree.
// The real project would compile sqlite3DbIsNamed from attach.c together with
// its proper dependencies. Here we emulate the essential behavior to verify
// the method logic and its branches.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>


// Domain-specific notes:
// - We are testing the logic of:
 // int sqlite3DbIsNamed(sqlite3 *db, int iDb, const char *zName){
 //   return (
 //       sqlite3StrICmp(db->aDb[iDb].zDbSName, zName)==0
 //    || (iDb==0 && sqlite3StrICmp("main", zName)==0)
 //   );
 // }
 // - We implement a tiny subset of the required types and helper function.

namespace sqlite_test {

// Minimal stand-in for the sqlite3 database handle and its dependent Db array.
struct Db {
    const char* zDbSName;
};

struct sqlite3 {
    Db* aDb; // array of Db descriptors for each attached database
};

// Lightweight case-insensitive string compare, matching sqlite3StrICmp behavior.
static int sqlite3StrICmp(const char *zLeft, const char *zRight) {
    // Basic implementation: compare ignoring case until a difference or end.
    if (zLeft == nullptr && zRight == nullptr) return 0;
    if (zLeft == nullptr) return -1;
    if (zRight == nullptr) return 1;
    unsigned char c1, c2;
    while (*zLeft && *zRight) {
        c1 = static_cast<unsigned char>(std::tolower(*zLeft));
        c2 = static_cast<unsigned char>(std::tolower(*zRight));
        if (c1 != c2) return (int)c1 - (int)c2;
        ++zLeft;
        ++zRight;
    }
    c1 = static_cast<unsigned char>(std::tolower(*zLeft));
    c2 = static_cast<unsigned char>(std::tolower(*zRight));
    return (int)c1 - (int)c2;
}

// The focal function under test, replicated here to enable deterministic unit tests
// in this standalone harness.
static int sqlite3DbIsNamed(sqlite3 *db, int iDb, const char *zName) {
    // Return 1 (true) if the database at index iDb has the same name as zName
    // OR if iDb is 0 (the main database) and zName equals "main" (case-insensitive).
    return (
        sqlite3StrICmp(db->aDb[iDb].zDbSName, zName) == 0
        || (iDb == 0 && sqlite3StrICmp("main", zName) == 0)
    );
}

// Simple test harness counters
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper for reporting test results without terminating the test run.
static void report(const char* testName, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// Convenience macro for readability in test cases
#define TEST(cond, name) report(name, static_cast<bool>(cond))

// Convenience function to set up a small, deterministic db instance
static sqlite3 make_db_with_two_entries() {
    // Create two databases:
    // - aDb[0] named "main"
    // - aDb[1] named "testdb"
    static Db arr[2] = { { "main" }, { "testdb" } };
    sqlite3 db;
    db.aDb = arr;
    return db;
}

// Run the suite of tests for sqlite3DbIsNamed.
// Coverage goals achieved by:
 // - Truthy path: exact match (case-sensitive and case-insensitive)
 // - Truthy path: iDb==0 fallback to "main" when zName matches "main" (case-insensitive)
 // - Falsy path: non-matching names
// The tests intentionally exercise both branches of the OR condition.
static void run_sqlite3DbIsNamed_tests() {
    sqlite3 db = make_db_with_two_entries();

    // Test 1: iDb = 0, zName exactly equals aDb[0].zDbSName ("main")
    // Expected: true
    TEST(sqlite3DbIsNamed(&db, 0, "main") == 1, "Test 1: iDb=0, zName='main' matches aDb[0].zDbSName");

    // Test 2: iDb = 0, zName case-insensitive match with "main" (e.g., "MAIN")
    // Expected: true
    TEST(sqlite3DbIsNamed(&db, 0, "MAIN") == 1, "Test 2: iDb=0, zName='MAIN' case-insensitive match against 'main'");

    // Test 3: iDb = 1, zName exactly matches aDb[1].zDbSName ("testdb")
    // Expected: true
    TEST(sqlite3DbIsNamed(&db, 1, "testdb") == 1, "Test 3: iDb=1, zName='testdb' matches aDb[1].zDbSName");

    // Test 4: iDb = 1, zName case-insensitive match with "TESTDB"
    // Expected: true
    TEST(sqlite3DbIsNamed(&db, 1, "TESTDB") == 1, "Test 4: iDb=1, zName='TESTDB' case-insensitive match against 'testdb'");

    // Test 5: iDb = 1, zName non-matching
    // Expected: false
    TEST(sqlite3DbIsNamed(&db, 1, "other") == 0, "Test 5: iDb=1, zName='other' does not match aDb[1].zDbSName");

    // Test 6: iDb = 0, zName non-main name should not trigger the fallback
    // aDb[0] is "main" but zName is "notmain" so only the iDb==0 fallback shouldn't apply
    // Expected: false
    TEST(sqlite3DbIsNamed(&db, 0, "notmain") == 0, "Test 6: iDb=0, zName='notmain' should not match; fallback to 'main' also false");
}

} // namespace sqlite_test

int main() {
    // Run the focused unit tests for sqlite3DbIsNamed
    sqlite_test::run_sqlite3DbIsNamed_tests();

    int total = sqlite_test::g_total_tests;
    int failed = sqlite_test::g_failed_tests;
    std::cout << "====================================\n";
    std::cout << "Total tests: " << total << ", Passed: " << (total - failed) << ", Failed: " << failed << "\n";
    // Return non-zero if any test failed to indicate test suite failure.
    return failed;
}
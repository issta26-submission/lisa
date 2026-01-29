// Test suite for sqlite3_backup_remaining from backup.c
// This test relies on the real SQLite library headers and binary.
// It provides two build configurations controlled by macros:
//  - TEST_ARMOR: compile with SQLITE_ENABLE_API_ARMOR enabled to exercise the NULL-pointer path.
//  - TEST_NO_ARMOR: compile without API armor to exercise non-NULL path.
// To run both, build two separate executables, one with -DTEST_ARMOR and one with -DTEST_NO_ARMOR.
// The tests use a lightweight, non-terminating assertion approach and print a summary.

#include <cerrno>
#include <cstdio>
#include <btreeInt.h>
#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Include the SQLite public headers to access sqlite3_backup and the function prototype.

// Ensure correct linkage for the C function from C++ test code
extern "C" int sqlite3_backup_remaining(sqlite3_backup *p);

static int g_failures = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if(!(_a == _b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " \
                  << #b << " (" << _b << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#ifdef TEST_ARMOR
// Helper to create a sqlite3_backup object with a specified nRemaining.
// We allocate the object using the real size provided by the library's type.
static sqlite3_backup* makeBackupWithRemaining(int rem) {
    sqlite3_backup* p = (sqlite3_backup*)calloc(1, sizeof(sqlite3_backup));
    if(p) {
        // Accessing nRemaining is valid because sqlite3_backup is a defined struct
        p->nRemaining = rem;
    }
    return p;
}

// Test: In ARMOR-enabled builds, calling with NULL should return 0 (true branch of predicate)
static void test_armor_null_pointer() {
    // true branch: p == 0
    int rc = sqlite3_backup_remaining(nullptr);
    EXPECT_EQ(0, rc);
}

// Test: In ARMOR-enabled builds, non-NULL pointers should return the stored nRemaining.
// This covers the false branch of the if(p==0) predicate.
static void test_armor_non_null_values() {
    sqlite3_backup* p0 = makeBackupWithRemaining(0);
    sqlite3_backup* p1 = makeBackupWithRemaining(12345);

    EXPECT_TRUE(p0 != nullptr);
    EXPECT_EQ(0, sqlite3_backup_remaining(p0));

    EXPECT_TRUE(p1 != nullptr);
    EXPECT_EQ(12345, sqlite3_backup_remaining(p1));

    free(p0);
    free(p1);
}

// Run all ARMOR-specific tests
static void run_armor_tests() {
    test_armor_null_pointer();
    test_armor_non_null_values();
    std::cout << "[ARMOR TESTS] Completed with " << g_failures << " failure(s)." << std::endl;
}
#endif // TEST_ARMOR

#ifndef TEST_ARMOR
#ifdef TEST_NO_ARMOR
// Test: In non-ARMOR builds, the function should return p->nRemaining for non-NULL p.
// This validates the main functionality path without the API armor wrapper.
static void test_no_armor_non_null() {
    sqlite3_backup* p = (sqlite3_backup*)calloc(1, sizeof(sqlite3_backup));
    if(!p) {
        std::cerr << "Failed to allocate sqlite3_backup in test_no_armor_non_null" << std::endl;
        ++g_failures;
        return;
    }
    p->nRemaining = 42;
    int rc = sqlite3_backup_remaining(p);
    EXPECT_EQ(42, rc);
    free(p);
}

// Run all NO-ARMOR tests
static void run_no_armor_tests() {
    test_no_armor_non_null();
    std::cout << "[NO ARMOR TESTS] Completed with " << g_failures << " failure(s)." << std::endl;
}
#endif // TEST_NO_ARMOR
#endif // !TEST_ARMOR

int main(void) {
    // The test harness expects to be compiled with either TEST_ARMOR or TEST_NO_ARMOR.
    // Depending on the build, execute the appropriate suite.

#ifdef TEST_ARMOR
    g_failures = 0;
    run_armor_tests();
    if(g_failures == 0) {
        std::cout << "ALL ARMOR TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "ARMOR TESTS FAILED: " << g_failures << " failure(s)" << std::endl;
        return 1;
    }
#else
#ifdef TEST_NO_ARMOR
    g_failures = 0;
    run_no_armor_tests();
    if(g_failures == 0) {
        std::cout << "ALL NO-ARMOR TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "NO-ARMOR TESTS FAILED: " << g_failures << " failure(s)" << std::endl;
        return 1;
    }
#else
    std::cerr << "No test configuration defined. Build with -DTEST_ARMOR or -DTEST_NO_ARMOR." << std::endl;
    return 2;
#endif
#endif
}
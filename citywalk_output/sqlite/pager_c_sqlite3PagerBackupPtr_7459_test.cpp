// Minimal C++11 test suite for sqlite3PagerBackupPtr
// No GTest; use a lightweight test runner with non-terminating assertions.
// The tests rely on the real sqlite3PagerBackupPtr function and the real Pager type
// from the sqliteInt.h header included in the project.

#include <cstdio>
#include <wal.h>
#include <sqliteInt.h>
#include <cstdint>


// Include the SQLite internal declarations to obtain the real Pager type.
// Wrap the C header include to ensure C linkage when compiling as C++.
extern "C" {
}

// Forward declare sqlite3_backup to avoid pulling in its full definition.
// We only need pointer semantics for testing.
typedef struct sqlite3_backup sqlite3_backup;

// Declare the focal function with C linkage to avoid C++ name mangling.
extern "C" sqlite3_backup **sqlite3PagerBackupPtr(Pager *pPager);

// Lightweight test harness: track total and failed tests without terminating on failure.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        fprintf(stderr, "Test failure: %s == %s failed at %s:%d\n", #actual, #expected, __FILE__, __LINE__); \
        fprintf(stderr, "  Actual:   %p\n", (void*)(actual)); \
        fprintf(stderr, "  Expected: %p\n", (void*)(expected)); \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        fprintf(stderr, "Test failure: condition %s is false at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

#define TEST_COMMENT(msg) do { printf("TEST: %s\n", msg); } while(0)

// Test 1: Ensure sqlite3PagerBackupPtr returns the address of the pBackup field
// and that the value pointed to by that address matches the current pBackup.
// This validates both the address-of and value semantics of the getter.
static void test_sqlite3PagerBackupPtr_nonNullBackingStore() {
    TEST_COMMENT("Test 1: Non-null pBackup backing store");    

    // Use a sentinel address for the sqlite3_backup object.
    sqlite3_backup *backup1 = reinterpret_cast<sqlite3_backup*>(0x1000);
    
    // Obtain a real Pager instance from the project headers.
    Pager pPager;
    pPager.pBackup = backup1;

    // Call the focal function.
    sqlite3_backup **pp = sqlite3PagerBackupPtr(&pPager);

    // The returned pointer should be exactly the address of the pBackup field.
    EXPECT_EQ(pp, &(pPager.pBackup));

    // The value stored at that location should equal the current pBackup.
    EXPECT_EQ(*pp, backup1);

    // Sanity: modify the pBackup and ensure the getter reflects the new value.
    sqlite3_backup *backup2 = reinterpret_cast<sqlite3_backup*>(0x2000);
    pPager.pBackup = backup2;

    sqlite3_backup **pp2 = sqlite3PagerBackupPtr(&pPager);
    EXPECT_EQ(pp2, &(pPager.pBackup));
    EXPECT_EQ(*pp2, backup2);
}

// Test 2: Null backing store scenario. Ensure the getter still returns the address
// of the pBackup field and that the dereferenced value is NULL.
static void test_sqlite3PagerBackupPtr_nullBackingStore() {
    TEST_COMMENT("Test 2: Null pBackup backing store (NULL value)");    

    Pager pPager;
    pPager.pBackup = nullptr;

    sqlite3_backup **pp = sqlite3PagerBackupPtr(&pPager);

    // Address of the field should still be returned.
    EXPECT_EQ(pp, &(pPager.pBackup));

    // The content of the field is NULL; dereferencing should yield NULL.
    EXPECT_TRUE(*pp == nullptr);
}

// Main entry: run tests and report a summary.
int main() {
    printf("Starting sqlite3PagerBackupPtr tests (C++11, non-GTest).\n");

    test_sqlite3PagerBackupPtr_nonNullBackingStore();
    test_sqlite3PagerBackupPtr_nullBackingStore();

    printf("Tests completed. Total: %d, Passed: %d, Failed: %d\n",
           g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

    // Return non-zero if any test failed.
    return (g_failed_tests != 0) ? 1 : 0;
}
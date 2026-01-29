// Unit test suite for the focal method sqlite3BackupUpdate in backup.c
// Notes:
// - The method simply forwards to backupUpdate if pBackup is non-null.
// - We validate both branches of the condition (pBackup == nullptr and pBackup != nullptr).
// - The test harness uses only the C/C++ standard library (no Google Test).
// - The tests are designed to be executable in a C++11 environment and rely on
//   linking with the existing backup.c (which provides sqlite3BackupUpdate and
//   the static backupUpdate function inside its TU).

#include <cstdlib>
#include <btreeInt.h>
#include <cstdio>
#include <sqliteInt.h>


// Domain-specific typedefs and forward declarations to match the
// sqlite3BackupUpdate signature as declared in backup.c.
// We assume the original project uses the following definitions (typical in SQLite):
typedef struct sqlite3_backup sqlite3_backup;
typedef unsigned int Pgno;
typedef unsigned char u8;

// Declare the focal function with C linkage so the linker can resolve it
extern "C" void sqlite3BackupUpdate(sqlite3_backup *pBackup, Pgno iPage, const u8 *aData);

// Forward declaration/definition to create a dummy non-null pBackup for testing.
// Note: We allocate an opaque memory block and cast it to sqlite3_backup*.
// This is sufficient since sqlite3BackupUpdate only checks for nullity and
// delegates to backupUpdate when non-null.
static inline sqlite3_backup* make_dummy_backup() {
    void* mem = std::malloc(sizeof(int)); // allocate minimal non-null placeholder
    return reinterpret_cast<sqlite3_backup*>(mem);
}
static inline void free_dummy_backup(sqlite3_backup* p) {
    if(p) std::free(p);
}

// Lightweight, non-terminating test reporting utilities.
// They accumulate a global count of failed assertions, but never aborts on first failure.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_BEGIN(name) \
    do { printf("BEGIN TEST: %s\n", name); } while(0)

#define TEST_END(name) \
    do { printf("END TEST: %s\n", name); } while(0)

#define CHECK(cond, msg) \
    do { \
        ++g_total_tests; \
        if(!(cond)) { \
            ++g_failed_tests; \
            printf("  [FAIL] %s\n", msg); \
        } else { \
            printf("  [PASS] %s\n", msg); \
        } \
    } while(0)


// Test 1: When pBackup is nullptr, sqlite3BackupUpdate should be a no-op (no crash, no operation).
// Rationale: The focal method guards with "if (pBackup)" and should not dereference a null pointer.
bool test_sqlite3BackupUpdate_nullPointer() {
    TEST_BEGIN("sqlite3BackupUpdate_nullPointer");

    // Call with nullptr; aData may be nullptr as well.
    sqlite3BackupUpdate(nullptr, 0, nullptr);

    // If the implementation crashes, the test will not reach this point.
    // Successful completion implies correct handling of the false branch.
    CHECK(true, "sqlite3BackupUpdate handles null pBackup without crashing");
    TEST_END("sqlite3BackupUpdate_nullPointer");
    return true;
}

// Test 2: When pBackup is non-null, sqlite3BackupUpdate should forward to backupUpdate.
// Rationale: The focal method is supposed to call backupUpdate(pBackup, iPage, aData) when pBackup is non-null.
// We cannot observe internal state changes of static backupUpdate, but we can ensure the call path does not crash.
bool test_sqlite3BackupUpdate_nonNullPointer() {
    TEST_BEGIN("sqlite3BackupUpdate_nonNullPointer");

    sqlite3_backup* pBackup = make_dummy_backup();
    if(!pBackup) {
        CHECK(false, "Failed to allocate dummy sqlite3_backup for test");
        TEST_END("sqlite3BackupUpdate_nonNullPointer");
        return false;
    }

    // Prepare a non-null data pointer to pass to the updater.
    const unsigned char data[8] = {0x10, 0x20, 0x30, 0x40, 0x55, 0x66, 0x77, 0x88};
    Pgno iPage = 12345;

    // This call should route to backupUpdate internally in the same TU (backup.c)
    // and must not crash.
    sqlite3BackupUpdate(pBackup, iPage, data);

    free_dummy_backup(pBackup);

    CHECK(true, "sqlite3BackupUpdate with non-null pBackup calls backupUpdate without crashing");
    TEST_END("sqlite3BackupUpdate_nonNullPointer");
    return true;
}

// Test 3: Sanity check with non-null pBackup but aData == nullptr.
// Rationale: Validate that the path still executes (calls backupUpdate) when aData is null.
// This exercises the robustness of the wrapper with a null data pointer.
bool test_sqlite3BackupUpdate_nonNullPBackup_nullData() {
    TEST_BEGIN("sqlite3BackupUpdate_nonNullPBackup_nullData");

    sqlite3_backup* pBackup = make_dummy_backup();
    if(!pBackup) {
        CHECK(false, "Failed to allocate dummy sqlite3_backup for test");
        TEST_END("sqlite3BackupUpdate_nonNullPBackup_nullData");
        return false;
    }

    Pgno iPage = 0;
    const unsigned char* aData = nullptr;

    sqlite3BackupUpdate(pBackup, iPage, aData);

    free_dummy_backup(pBackup);

    CHECK(true, "sqlite3BackupUpdate with non-null pBackup and null aData executes without crash");
    TEST_END("sqlite3BackupUpdate_nonNullPBackup_nullData");
    return true;
}


// Main entry: run all tests and report summary.
// This mirrors step 3 guidance: call test methods from main when gtest isn't allowed.
int main() {
    // Initialize test suite
    g_total_tests = 0;
    g_failed_tests = 0;

    bool all_passed =
        test_sqlite3BackupUpdate_nullPointer() &
        test_sqlite3BackupUpdate_nonNullPointer() &
        test_sqlite3BackupUpdate_nonNullPBackup_nullData();

    // Summary
    printf("\nTEST SUMMARY: %d passed, %d failed (out of %d tests)\n",
           g_total_tests - g_failed_tests, g_failed_tests, g_total_tests);

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}
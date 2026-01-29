#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cstdint>


// Include internal sqlite definitions to align with sqlite3_errmsg's expectations.
// This header is typically present in the sqlite source tree.

// Lightweight test harness (no GTest). We implement simple, non-terminating checks.
#define SQLITE_NOMEM_BKPT 1
#define SQLITE_MISUSE_BKPT 2

// Forward declarations to satisfy the focal function's dependencies (overrides for test).
extern "C" const char* sqlite3ErrStr(int rc);
extern "C" int sqlite3SafetyCheckSickOrOk(sqlite3 *db);
extern "C" const unsigned char* sqlite3_value_text(void* p);
extern "C" void testcase(int);

extern "C" void sqlite3_mutex_enter(sqlite3_mutex *mutex);
extern "C" void sqlite3_mutex_leave(sqlite3_mutex *mutex);

// Global state to control test scenarios
static int g_safety_ok = 1;           // Controls sqlite3SafetyCheckSickOrOk result
static int g_use_value_text = 1;      // Controls sqlite3_value_text return value
static char g_errbuf[128];            // Buffer for sqlite3ErrStr results

// Minimal overrides to steer sqlite3_errmsg behavior during tests

extern "C" const char* sqlite3ErrStr(int rc){
    // Simple, deterministic error string for testing
    snprintf(g_errbuf, sizeof(g_errbuf), "ERR_%d", rc);
    return g_errbuf;
}
extern "C" int sqlite3SafetyCheckSickOrOk(sqlite3 *db){
    // Allow tests to toggle behavior via global flag
    (void)db; // unused parameter, kept for compatibility
    return g_safety_ok;
}
extern "C" const unsigned char* sqlite3_value_text(void* p){
    // Return a non-null pointer when requested, to exercise the pErr-path
    if( g_use_value_text )
        return (const unsigned char*)"PERR_TEXT";
    return nullptr;
}
extern "C" void testcase(int){
    // No-op placeholder used by sqlite's internal tests for coverage bookkeeping
}

// Dummy mutex implementation (no real synchronization needed for tests)
static sqlite3_mutex dummyMutex;

extern "C" void sqlite3_mutex_enter(sqlite3_mutex *mutex){
    (void)mutex;
}
extern "C" void sqlite3_mutex_leave(sqlite3_mutex *mutex){
    (void)mutex;
}

// Simple test runner
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define RUN_TEST(name) do {                       \
    printf("RUN: %s\n", #name);                    \
    const bool _ok = (name)();                       \
    tests_run++;                                     \
    if(_ok){ tests_passed++; printf("RESULT: PASS\n\n"); } \
    else { tests_failed++; printf("RESULT: FAIL\n\n"); } \
} while(0)

int main(){
    // Ensure the required symbols are linked with our test harness:
    // - sqlite3_errmsg is provided by the focal file main.c in the project under test.
    // - The following stubs override internal dependencies to enable isolated testing.

    // Test 1: NULL db pointer should take the first branch and return sqlite3ErrStr(SQLITE_NOMEM_BKPT)
    auto test_null_db = []() -> bool {
        const char* res = sqlite3_errmsg(nullptr);
        const char* exp = sqlite3ErrStr(SQLITE_NOMEM_BKPT);
        if(!res || !exp) return false;
        return strcmp(res, exp) == 0;
    };

    // Test 2: Safety check fails (misuse detection)
    auto test_safety_fail = []() -> bool {
        sqlite3 db;
        memset(&db, 0, sizeof(db));
        db.mutex = &dummyMutex;
        db.pErr = 0;
        db.errCode = SQLITE_MISUSE_BKPT; // Force MISUSE when safety check fails later
        g_safety_ok = 0; // sqlite3SafetyCheckSickOrOk will return 0
        const char* res = sqlite3_errmsg(&db);
        const char* exp = sqlite3ErrStr(SQLITE_MISUSE_BKPT);
        // Restore for subsequent tests
        g_safety_ok = 1;
        return res && exp && (strcmp(res, exp) == 0);
    };

    // Test 3: mallocFailed path
    auto test_malloc_failed = []() -> bool {
        sqlite3 db;
        memset(&db, 0, sizeof(db));
        db.mutex = &dummyMutex;
        db.pErr = 0;
        db.errCode = 0;
        db.mallocFailed = 1;
        g_safety_ok = 1;
        const char* res = sqlite3_errmsg(&db);
        const char* exp = sqlite3ErrStr(SQLITE_NOMEM_BKPT);
        // Cleanup
        db.mallocFailed = 0;
        return res && exp && (strcmp(res, exp) == 0);
    };

    // Test 4a: pErr == 0 and errCode != 0 -> uses errCode path
    auto test_pErr_zero_errCode_nonzero = []() -> bool {
        sqlite3 db;
        memset(&db, 0, sizeof(db));
        db.mutex = &dummyMutex;
        db.pErr = 0;
        db.errCode = 3; // Non-zero
        db.mallocFailed = 0;
        g_safety_ok = 1;
        g_use_value_text = 1; // not used since pErr == NULL and errCode != 0
        const char* res = sqlite3_errmsg(&db);
        const char* exp = sqlite3ErrStr(3);
        return res && exp && (strcmp(res, exp) == 0);
    };

    // Test 4b: pErr != 0 and value_text returns non-null -> uses pErr text
    auto test_pErr_text_path = []() -> bool {
        sqlite3 db;
        Mem m;
        memset(&db, 0, sizeof(db));
        db.mutex = &dummyMutex;
        db.pErr = &m;
        db.errCode = 123; // Non-zero so we fetch pErr text
        db.mallocFailed = 0;
        g_safety_ok = 1;
        g_use_value_text = 1; // sqlite3_value_text returns non-null
        const char* res = sqlite3_errmsg(&db);
        const char* exp = "PERR_TEXT";
        return res && exp && (strcmp(res, exp) == 0);
    };

    // Test 4c: pErr != 0 but value_text returns null -> fallback to errCode string
    auto test_pErr_text_null_fallback = []() -> bool {
        sqlite3 db;
        Mem m;
        memset(&db, 0, sizeof(db));
        db.mutex = &dummyMutex;
        db.pErr = &m;
        db.errCode = 5;
        db.mallocFailed = 0;
        g_safety_ok = 1;
        g_use_value_text = 0; // sqlite3_value_text returns null
        const char* res = sqlite3_errmsg(&db);
        const char* exp = sqlite3ErrStr(5);
        return res && exp && (strcmp(res, exp) == 0);
    };

    // Run tests
    // Note: We cast lambda results to bool for consistent RUN_TEST usage
    RUN_TEST(test_null_db);
    RUN_TEST(test_safety_fail);
    RUN_TEST(test_malloc_failed);
    RUN_TEST(test_pErr_zero_errCode_nonzero);
    RUN_TEST(test_pErr_text_path);
    RUN_TEST(test_pErr_text_null_fallback);

    printf("Summary: total=%d, passed=%d, failed=%d\n", tests_run, tests_passed, tests_failed);
    return (tests_failed == 0) ? 0 : 1;
}
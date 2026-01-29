// Lightweight C++11 unit tests for sqlite3_errmsg16 without GoogleTest.
// This test suite provides a minimal test harness with non-terminating
// assertions to maximize code coverage. It mocks necessary SQLite pieces
// to exercise the focal function in a self-contained manner.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Lightweight domain-specific helpers and types to compile in C++11.
using u16 = unsigned short;

// Minimal representation of the SQLite database object used by sqlite3_errmsg16.
struct sqlite3_mutex {};

struct sqlite3 {
    sqlite3_mutex *mutex;
    bool mallocFailed;
    void *pErr;
    int errCode;
};

// Forward declaration of the focal function (C linkage).
extern "C" const void *sqlite3_errmsg16(sqlite3 *db);

// --- Minimal SQLite-API stubs used by the focal method ---

// Simulate safety check: can be toggled to exercise true/false branches.
static bool g_safetyOk = true;
extern "C" int sqlite3SafetyCheckSickOrOk(sqlite3 *db) {
    return g_safetyOk ? 1 : 0;
}

extern "C" void sqlite3_mutex_enter(sqlite3_mutex *NotUsed) { (void)NotUsed; }
extern "C" void sqlite3_mutex_leave(sqlite3_mutex *NotUsed) { (void)NotUsed; }

extern "C" const void *sqlite3_value_text16(const void *p) {
    // In tests, the value is already a 16-bit string buffer, so return as-is.
    return p;
}

extern "C" void sqlite3ErrorWithMsg(sqlite3 *db, int rc, const char *zErr) {
    // For test purposes, store a static 16-bit "error" message in pErr.
    static const u16 mem[] = { 'e','r','r','o','r', 0 };
    (void)zErr; // zErr is not used by this minimal test harness.
    db->pErr = (void*)mem;
    db->errCode = rc;
}
extern "C" void sqlite3OomClear(sqlite3 *db) {
    // Clear malloc failure indicator.
    db->mallocFailed = false;
}
extern "C" const char *sqlite3ErrStr(int rc) {
    // Return a generic error string; not used for exact content in tests
    // except in error path 5 where we supply a static "error" string.
    (void)rc;
    return "error";
}

// The focal method under test. Implemented to mirror the provided snippet.
extern "C" const void *sqlite3_errmsg16(sqlite3 *db) {
    static const u16 outOfMem[] = {
        'o', 'u', 't', ' ', 'o', 'f', ' ', 'm', 'e', 'm', 'o', 'r', 'y', 0
    };
    static const u16 misuse[] = {
        'b', 'a', 'd', ' ', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', ' ',
        'o', 'r', ' ', 'o', 't', 'h', 'e', 'r', ' ', 'A', 'P', 'I', ' ',
        'm', 'i', 's', 'u', 's', 'e', 0
    };
    const void *z;
    if( !db ){
        return (void *)outOfMem;
    }
    if( !sqlite3SafetyCheckSickOrOk(db) ){
        return (void *)misuse;
    }
    sqlite3_mutex_enter(db->mutex);
    if( db->mallocFailed ){
        z = (void *)outOfMem;
    }else{
        z = sqlite3_value_text16(db->pErr);
        if( z==0 ){
            sqlite3ErrorWithMsg(db, db->errCode, sqlite3ErrStr(db->errCode));
            z = sqlite3_value_text16(db->pErr);
        }
        sqlite3OomClear(db);
    }
    sqlite3_mutex_leave(db->mutex);
    return z;
}

// --- Lightweight test harness infrastructure ---

static int g_failures = 0;
static std::vector<std::string> g_failure_msgs;

// Simple non-terminating assertion macro
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        g_failure_msgs.push_back(std::string("EXPECT_TRUE FAILED: ") + (msg)); \
        ++g_failures; \
    } \
} while(0)

static void logTestResultHeader() {
    std::cout << "Running sqlite3_errmsg16 unit tests (minimal harness)..." << std::endl;
}

// Test 1: db == nullptr should return "out of memory" message
void test_null_db_returns_out_of_mem() {
    const void *res = sqlite3_errmsg16(nullptr);
    const u16 *msg = (const u16 *)res;
    // Expect the message to start with "out of memory"
    const char expected[] = "out of memory";
    for(size_t i = 0; i < strlen(expected); ++i) {
        EXPECT_TRUE(msg[i] == (u16)expected[i],
                    "null db path should return 'out of memory' prefix");
    }
}

// Test 2: Safety check fails -> should return "misuse" message
void test_safety_check_false_returns_misuse() {
    // Force the safety check to fail
    extern "C" void set_safety_false() { g_safetyOk = false; }
    // Ensure we reset after test
    g_safetyOk = false;
    sqlite3 db;
    db.mutex = nullptr;
    db.pErr = nullptr;
    db.mallocFailed = false;
    db.errCode = 0;

    const void *res = sqlite3_errmsg16(&db);
    const u16 *msg = (const u16 *)res;
    // Expect the message to start with "bad"
    EXPECT_TRUE(msg[0] == (u16)'b' && msg[1] == (u16)'a' && msg[2] == (u16)'d',
                "safety check false should return 'bad' string");
    // Restore safety for other tests
    g_safetyOk = true;
}

// Test 3: mmap/malloc failed path
void test_malloc_failed_path() {
    sqlite3 db;
    db.mutex = nullptr;
    db.pErr = nullptr;
    db.mallocFailed = true;
    db.errCode = 123; // arbitrary
    const void *res = sqlite3_errmsg16(&db);
    const u16 *msg = (const u16 *)res;
    const char expected[] = "out of memory";
    for(size_t i = 0; i < strlen(expected); ++i) {
        EXPECT_TRUE(msg[i] == (u16)expected[i],
                    "mallocFailed path should return 'out of memory'");
    }
}

// Test 4: db->pErr non-null -> should return existing pErr text
void test_pErr_non_null_returns_pErr() {
    sqlite3 db;
    db.mutex = nullptr;
    db.mallocFailed = false;
    static const u16 customErr[] = { 'c','u','s','t','o','m',' ','e','r','r','o','r',0 };
    db.pErr = (void*)customErr;
    db.errCode = 0;
    const void *res = sqlite3_errmsg16(&db);
    // The returned pointer should be the same as db->pErr
    EXPECT_TRUE(res == (const void*)customErr, "non-null pErr should return the same pointer");
}

// Test 5: pErr == null -> should set error message and return it
void test_pErr_null_sets_error_message() {
    sqlite3 db;
    db.mutex = nullptr;
    db.mallocFailed = false;
    db.pErr = nullptr;
    db.errCode = 42; // arbitrary code
    const void *res = sqlite3_errmsg16(&db);
    const u16 *msg = (const u16 *)res;
    // Expect "error" string
    const char expected[] = "error";
    for(size_t i = 0; i < strlen(expected); ++i) {
        EXPECT_TRUE(msg[i] == (u16)expected[i],
                    "pErr null path should set and return 'error'");
    }
}

// Helper to run all tests and print summary
void run_all_tests() {
    logTestResultHeader();

    test_null_db_returns_out_of_mem();
    test_safety_check_false_returns_misuse();
    test_malloc_failed_path();
    test_pErr_non_null_returns_pErr();
    test_pErr_null_sets_error_message();

    if(g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
        for(const auto &m : g_failure_msgs) std::cout << m << std::endl;
    }
}

int main() {
    run_all_tests();
    return (g_failures == 0) ? 0 : 1;
}
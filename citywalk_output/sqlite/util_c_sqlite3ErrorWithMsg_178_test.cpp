// Minimal test harness for sqlite3ErrorWithMsg (C-style function) using C++11.
// This test suite does not rely on GTest and uses a lightweight, non-terminating
// assertion approach. It provides a controlled, isolated environment by supplying
// tiny stubs for the required SQLite-like interfaces used by sqlite3ErrorWithMsg.

#include <math.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Forward declarations to mimic the needed SQLite-like API used by sqlite3ErrorWithMsg
extern "C" {

// Minimal sqlite3 and sqlite3_value structures to support the focal function.
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3 sqlite3;

struct sqlite3_value {
    char *z;
};

struct sqlite3 {
    int errCode;
    sqlite3_value *pErr;
};

// Constants used by the original code
const int SQLITE_UTF8 = 1;
const int SQLITE_DYNAMIC = 2;

// Global (test-scoped) state for verifying interactions
static int g_lastSystemErrorRc = -1;
static int g_lastErrorRc = -1;

// The following stubs simulate the required internal SQLite helpers.

void sqlite3SystemError(sqlite3 *db, int rc) {
    // Record that a system error was reported for a given rc
    (void)db; // unused in test tracking beyond rc
    g_lastSystemErrorRc = rc;
}

void sqlite3Error(sqlite3 *db, int rc) {
    // Record that a generic error was reported for a given rc
    (void)db;
    g_lastErrorRc = rc;
}

sqlite3_value* sqlite3ValueNew(sqlite3 *db) {
    // Allocate a new sqlite3_value
    (void)db;
    sqlite3_value *v = (sqlite3_value*)std::malloc(sizeof(sqlite3_value));
    if(v) v->z = nullptr;
    return v;
}

char* sqlite3VMPrintf(sqlite3 *db, const char *zFormat, va_list ap) {
    (void)db;
    // Determine required length
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int len = std::vsnprintf(nullptr, 0, zFormat, ap_copy);
    va_end(ap_copy);
    if(len < 0) return nullptr;
    char *z = (char*)std::malloc((size_t)len + 1);
    if(!z) return nullptr;
    std::vsnprintf(z, (size_t)len + 1, zFormat, ap);
    return z;
}

void sqlite3ValueSetStr(sqlite3_value *p, int n, const char *z, int enc, int dynamic) {
    (void)n; (void)enc; (void)dynamic;
    // Replace the string inside the sqlite3_value
    if(p == nullptr) return;
    if(p->z) std::free(p->z);
    if(z)
        p->z = std::strdup(z);
    else
        p->z = nullptr;
}

// The focal method under test copied exactly as provided (adapted to this test harness)
void sqlite3ErrorWithMsg(sqlite3 *db, int err_code, const char *zFormat, ...) {
  assert( db!=0 );
  db->errCode = err_code;
  sqlite3SystemError(db, err_code);
  if( zFormat==0 ){
    sqlite3Error(db, err_code);
  }else if( db->pErr || (db->pErr = sqlite3ValueNew(db))!=0 ){
    char *z;
    va_list ap;
    va_start(ap, zFormat);
    z = sqlite3VMPrintf(db, zFormat, ap);
    va_end(ap);
    sqlite3ValueSetStr(db->pErr, -1, z, SQLITE_UTF8, SQLITE_DYNAMIC);
    // Free the temporary string after storing in the value
    if(z) std::free(z);
  }
}

} // extern "C"

///////////////////////
// Test harness helpers
///////////////////////

static void resetGlobals() {
    g_lastSystemErrorRc = -1;
    g_lastErrorRc = -1;
}

// Helper to free a potentially allocated db.pErr
static void freeDbPErr(sqlite3 *db) {
    if(db == nullptr) return;
    if(db->pErr) {
        if(db->pErr->z) std::free(db->pErr->z);
        std::free(db->pErr);
        db->pErr = nullptr;
    }
}

// Helper to print test result
static bool expectEqualsInt(const char* name, int got, int expected) {
    if(got != expected) {
        std::cerr << "Test failure: " << name << " expected " << expected << " but got " << got << "\n";
        return false;
    }
    return true;
}

// Test 1: zFormat == 0 should set errCode, call SystemError and Error, and NOT allocate pErr.
static bool test_case_zFormat_null() {
    resetGlobals();

    sqlite3 db;
    db.errCode = 0;
    db.pErr = nullptr;
    // Call focal function with zFormat == 0
    sqlite3ErrorWithMsg(&db, 42, nullptr);

    bool ok = true;
    ok &= expectEqualsInt("db.errCode", db.errCode, 42);
    ok &= expectEqualsInt("g_lastSystemErrorRc", g_lastSystemErrorRc, 42);
    ok &= expectEqualsInt("g_lastErrorRc", g_lastErrorRc, 42);
    ok &= (db.pErr == nullptr);
    // Cleanup (though pErr should be null)
    freeDbPErr(&db);
    return ok;
}

// Test 2: zFormat != 0 and pErr is initially null -> should allocate and set formatted string.
static bool test_case_alloc_when_null_pErr() {
    resetGlobals();

    sqlite3 db;
    db.errCode = 0;
    db.pErr = nullptr;

    // Call focal function with a format string and an argument
    sqlite3ErrorWithMsg(&db, 7, "Error: %s", "oom");

    bool ok = true;
    ok &= expectEqualsInt("db.errCode", db.errCode, 7);
    ok &= expectEqualsInt("g_lastSystemErrorRc", g_lastSystemErrorRc, 7);
    // pErr should now exist and contain formatted string
    if(db.pErr == nullptr || db.pErr->z == nullptr) {
        std::cerr << "Test failure: pErr not allocated or z is null\n";
        ok = false;
    } else {
        if(std::strcmp(db.pErr->z, "Error: oom") != 0) {
            std::cerr << "Test failure: pErr content expected 'Error: oom' but got '" << db.pErr->z << "'\n";
            ok = false;
        }
    }

    // Cleanup
    freeDbPErr(&db);
    return ok;
}

// Test 3: zFormat != 0 and pErr already exists -> should reuse existing pErr and overwrite content.
static bool test_case_existing_pErr_reused() {
    resetGlobals();

    sqlite3 db;
    db.errCode = 0;
    // Pre-create a pErr with some old content
    db.pErr = (sqlite3_value*)std::malloc(sizeof(sqlite3_value));
    if(db.pErr) {
        db.pErr->z = std::strdup("old");
    }

    // Call focal function; this should format "New: 3" into pErr
    sqlite3ErrorWithMsg(&db, 13, "New: %d", 3);

    bool ok = true;
    ok &= expectEqualsInt("db.errCode", db.errCode, 13);
    ok &= expectEqualsInt("g_lastSystemErrorRc", g_lastSystemErrorRc, 13);
    if(db.pErr == nullptr || db.pErr->z == nullptr) {
        std::cerr << "Test failure: pErr not allocated or z is null (expected to be overwritten)\n";
        ok = false;
    } else {
        if(std::strcmp(db.pErr->z, "New: 3") != 0) {
            std::cerr << "Test failure: pErr content expected 'New: 3' but got '" << db.pErr->z << "'\n";
            ok = false;
        }
    }

    // Cleanup
    if(db.pErr) {
        if(db.pErr->z) std::free(db.pErr->z);
        std::free(db.pErr);
        db.pErr = nullptr;
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "sqlite3ErrorWithMsg unit tests (C++11, no GTest)\n";

    total++; if(test_case_zFormat_null()) { ++passed; std::cout << "[PASS] Test 1: zFormat == 0 path\n"; } else { std::cout << "[FAIL] Test 1: zFormat == 0 path\n"; }
    total++; if(test_case_alloc_when_null_pErr()) { ++passed; std::cout << "[PASS] Test 2: allocate pErr when pErr was null\n"; } else { std::cout << "[FAIL] Test 2: allocate pErr when pErr was null\n"; }
    total++; if(test_case_existing_pErr_reused()) { ++passed; std::cout << "[PASS] Test 3: reuse existing pErr and overwrite content\n"; } else { std::cout << "[FAIL] Test 3: reuse existing pErr and overwrite content\n"; }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
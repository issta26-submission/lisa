// fts3_init_tests.cpp
// Lightweight C++11 test suite for sqlite3Fts3Init (no GoogleTest).
// This test suite uses a small in-house test framework and lightweight mocks
// to drive various execution paths of sqlite3Fts3Init in fts3.c.
// The aim is to exercise true/false branches, resource allocation paths, and
// error cleanup, without terminating tests on first failure.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// Lightweight, self-contained mock environment and test scaffolding.
// NOTE: This test file is designed to be compiled together with fts3.c in the
// same project so that sqlite3Fts3Init is linked against the mocked
// implementations below. The mocks intentionally keep behavior simple and
// controllable via global flags to cover multiple branches.

// Domain-specific constants (matching typical SQLite return codes used in fts3.c)
#define SQLITE_OK    0
#define SQLITE_NOMEM 1

// Define a minimal sqlite3 type used by the focal function.
typedef struct sqlite3 sqlite3;

//--------------------------------------------------------------------------------------
// Public interfaces expected by sqlite3Fts3Init (stubs/mocks below)
//--------------------------------------------------------------------------------------
extern "C" {
    typedef struct sqlite3_tokenizer_module { int dummy; } sqlite3_tokenizer_module;
    typedef struct sqlite3_module { int dummy; } sqlite3_module;

    // Prototype of focal function
    int sqlite3Fts3Init(sqlite3 *db);

    // Memory management stubs
    void* sqlite3_malloc(size_t);
    void  sqlite3_free(void*);

    // Init hooks (simplified)
    int sqlite3Fts3InitTerm(sqlite3 *db);
    int sqlite3Fts3InitAux(sqlite3 *db);

    // Tokenizer modules (simple stubs)
    void sqlite3Fts3SimpleTokenizerModule(const sqlite3_tokenizer_module **pp);
    void sqlite3Fts3PorterTokenizerModule(const sqlite3_tokenizer_module **pp);
#ifndef SQLITE_DISABLE_FTS3_UNICODE
    void sqlite3Fts3UnicodeTokenizer(const sqlite3_tokenizer_module **pp);
#endif

    // Hash/tokenizer infrastructure stubs
    int sqlite3Fts3HashInit(void *hash, int n, int m);
    int sqlite3Fts3HashInsert(void *hash, const char *z, int n, void *p);
    int sqlite3Fts3InitHashTable(sqlite3 *db, void *hash, const char *zName);
    void sqlite3Fts3HashClear(void *p);

    int sqlite3_overload_function(sqlite3 *db, const char *zName, int nVal);
    int sqlite3_create_module_v2(sqlite3 *db, const char *zName, const sqlite3_module *pModule,
                               void *pAux, void (*xDestroy)(void*));
    int sqlite3Fts3InitTok(sqlite3 *db, void *p, void (*xDestroy)(void*));
}

// Global flags to drive mock behavior and verify cleanup
int g_mallocFail      = 0; // simulate sqlite3_malloc failure
int g_hashInsertFail  = 0; // simulate failure on first/any hash insertion
int g_hashTableFail   = 0; // simulate failure during InitHashTable
int g_termError       = 0; // simulate sqlite3Fts3InitTerm failure
int g_auxError        = 0; // simulate sqlite3Fts3InitAux failure
int g_hashClearCalled = 0; // flag to verify cleanup path invoked

// Simple static-like structures used by mocks
static sqlite3_tokenizer_module simpleMod;
static sqlite3_tokenizer_module porterMod;
#ifndef SQLITE_DISABLE_FTS3_UNICODE
static sqlite3_tokenizer_module unicodeMod;
#endif
static sqlite3_module fts3Module; // module descriptor provided to create_module_v2

// Minimal Fts3HashWrapper replica to satisfy usage in sqlite3Fts3Init
struct Fts3HashWrapper { void* hash; int nRef; };

// Provide a dummy hash destroy callback (signature matches the real code)
static void hashDestroy(void *p){ (void)p; }

//--------------------------------------------------------------------------------------
// Mock implementations of required external functions (simple, controlled)
//--------------------------------------------------------------------------------------

extern "C" void* sqlite3_malloc(size_t sz) {
    if (g_mallocFail) return nullptr;
    return std::malloc(sz);
}
extern "C" void sqlite3_free(void* p) {
    std::free(p);
}

extern "C" int sqlite3Fts3InitTerm(sqlite3 *db) {
    (void)db;
    return g_termError ? SQLITE_NOMEM : SQLITE_OK;
}
extern "C" int sqlite3Fts3InitAux(sqlite3 *db) {
    (void)db;
    return g_auxError ? SQLITE_NOMEM : SQLITE_OK;
}

extern "C" void sqlite3Fts3SimpleTokenizerModule(const sqlite3_tokenizer_module **pp) {
    *pp = &simpleMod;
}
extern "C" void sqlite3Fts3PorterTokenizerModule(const sqlite3_tokenizer_module **pp) {
    *pp = &porterMod;
}
#ifndef SQLITE_DISABLE_FTS3_UNICODE
extern "C" void sqlite3Fts3UnicodeTokenizer(const sqlite3_tokenizer_module **pp) {
    *pp = &unicodeMod;
}
#endif

extern "C" int sqlite3Fts3HashInit(void *hash, int /*a*/, int /*b*/) {
    (void)hash; // no-op in mock
    return 0;
}
extern "C" int sqlite3Fts3HashInsert(void *hash, const char *z, int n, void *p) {
    (void)hash; (void)z; (void)n; (void)p;
    return g_hashInsertFail ? 1 : 0;
}
extern "C" int sqlite3Fts3InitHashTable(sqlite3 *db, void *hash, const char *zName) {
    (void)db; (void)hash; (void)zName;
    return g_hashTableFail ? 1 : SQLITE_OK;
}
extern "C" void sqlite3Fts3HashClear(void *p) {
    (void)p;
    g_hashClearCalled = 1;
}

extern "C" int sqlite3_overload_function(sqlite3 *db, const char *zName, int nVal) {
    (void)db; (void)zName; (void)nVal;
    return SQLITE_OK;
}
extern "C" int sqlite3_create_module_v2(sqlite3 *db, const char *zName,
                                      const sqlite3_module *pModule, void *pAux,
                                      void (*xDestroy)(void*)) {
    (void)db; (void)zName; (void)pModule; (void)pAux; (void)xDestroy;
    return SQLITE_OK;
}
extern "C" int sqlite3Fts3InitTok(sqlite3 *db, void *p, void (*xDestroy)(void*)) {
    (void)db; (void)p; (void)xDestroy;
    return SQLITE_OK;
}

//--------------------------------------------------------------------------------------
// Lightweight test framework
// Each test prints its result; failures do not abort subsequent tests.
//--------------------------------------------------------------------------------------
static void testLog(const char* header, bool ok, const char* details = "") {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << header
              << (details && details[0] ? " - " : "")
              << details << std::endl;
}

static int runAllTests();

static bool test_InitOkPath();
static bool test_AllocFailurePath();
static bool test_HashInsertFailurePath();
static bool test_HashTableFailurePath();
static bool test_CleanupOnErrorPath();

//--------------------------------------------------------------------------------------
// Test implementations
//--------------------------------------------------------------------------------------

bool test_InitOkPath() {
    // Reset global flags to default (success path)
    g_mallocFail = 0;
    g_hashInsertFail = 0;
    g_hashTableFail = 0;
    g_termError = 0;
    g_auxError = 0;
    g_hashClearCalled = 0;

    sqlite3 *db = nullptr; // dummy db; mocks ignore
    int rc = sqlite3Fts3Init(db);

    bool ok = (rc == SQLITE_OK);
    testLog("sqlite3Fts3Init should return SQLITE_OK on successful path", ok);
    if (!ok) {
        testLog("Details", false, "rc != SQLITE_OK on success path.");
    }
    // Also return rc so tests can aggregate outcomes if desired
    return ok;
}

bool test_AllocFailurePath() {
    // Force malloc failure to simulate OOM during hash wrapper allocation
    g_mallocFail = 1;
    g_hashClearCalled = 0;
    sqlite3 *db = nullptr;
    int rc = sqlite3Fts3Init(db);

    bool ok = (rc == SQLITE_NOMEM);
    testLog("sqlite3Fts3Init should return SQLITE_NOMEM when malloc fails", ok);
    if (!ok) {
        testLog("Details", false, "Expected SQLITE_NOMEM due to malloc failure.");
    }
    // cleanup: ensure no hash was allocated in this path; since allocation failed,
    // pHash is NULL and cleanup not invoked.
    return ok;
}

bool test_HashInsertFailurePath() {
    // Simulate failure during hash insertion after allocation succeeds
    g_mallocFail = 0;
    g_hashInsertFail = 1;
    g_hashTableFail = 0;
    g_termError = 0;
    g_auxError = 0;
    g_hashClearCalled = 0;

    sqlite3 *db = nullptr;
    int rc = sqlite3Fts3Init(db);

    bool ok = (rc == SQLITE_NOMEM);
    testLog("sqlite3Fts3Init should return SQLITE_NOMEM when hashInsert fails", ok);
    if (!ok) {
        testLog("Details", false, "Expected SQLITE_NOMEM due to hashInsert failure.");
    }
    return ok;
}

bool test_HashTableFailurePath() {
    // Simulate failure during InitHashTable after previous steps succeed
    g_mallocFail = 0;
    g_hashInsertFail = 0;
    g_hashTableFail = 1;
    g_termError = 0;
    g_auxError = 0;
    g_hashClearCalled = 0;

    sqlite3 *db = nullptr;
    int rc = sqlite3Fts3Init(db);

    bool ok = (rc != SQLITE_OK);
    testLog("sqlite3Fts3Init should return non-OK when InitHashTable fails", ok);
    if (!ok) {
        testLog("Details", false, "Expected non-OK rc due to InitHashTable failure.");
    }
    // Verify that cleanup path was invoked (hash cleared and freed)
    bool cleaned = (g_hashClearCalled != 0);
    testLog("Cleanup path invoked on InitHashTable failure", cleaned);
    if (!cleaned) {
        testLog("Details", false, "Expected cleanup (HashClear) to be called on error.");
    }
    return ok && cleaned;
}

bool test_CleanupOnErrorPath() {
    // Ensure that, when an error occurs after pHash is allocated, cleanup is performed.
    g_mallocFail = 0;
    g_hashInsertFail = 0;
    g_hashTableFail = 1; // Cause error after pHash creation to trigger cleanup
    g_termError = 0;
    g_auxError = 0;
    g_hashClearCalled = 0;

    sqlite3 *db = nullptr;
    int rc = sqlite3Fts3Init(db);

    bool ok = (rc != SQLITE_OK && g_hashClearCalled != 0);
    testLog("sqlite3Fts3Init should cleanup hash on failure after allocation", ok);
    if (!ok) {
        testLog("Details", false, "Expected non-OK rc and cleanup invocation.");
    }
    return ok;
}

//--------------------------------------------------------------------------------------
// Entry point
//--------------------------------------------------------------------------------------
int main() {
    int total = 0;
    int passed = 0;

    // Run tests
    if (test_InitOkPath()) { ++passed; } ++total;
    if (test_AllocFailurePath()) { ++passed; } ++total;
    if (test_HashInsertFailurePath()) { ++passed; } ++total;
    if (test_HashTableFailurePath()) { ++passed; } ++total;
    if (test_CleanupOnErrorPath()) { ++passed; } ++total;

    std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";

    // Return code: 0 if all tests passed, non-zero otherwise
    return (passed == total) ? 0 : 1;
}

// Implementations of the tests that forward to runAllTests() if desired
static int runAllTests() {
    // In this minimal harness, tests are invoked directly from main().
    // This function is provided for symmetry with larger frameworks and can
    // be wired to a test runner if needed.
    return 0;
}
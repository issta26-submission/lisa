/*
  Unit test suite for the focal method sqlite3WithAdd (C function) using C++11.
  The tests are designed to be non-terminating (do not call exit on failure)
  and report results via a simple in-process test harness.
  Notes:
  - This test suite provides minimal mock implementations for the dependencies
    used within sqlite3WithAdd (memory allocators, error reporter, string
    comparison, and CTE management) so the function under test can be exercised
    in isolation.
  - The test code relies on the actual production signature of sqlite3WithAdd
    and related types (Parse, With, Cte). If your environment provides the full
    sqlite3 internal headers, link against the real implementation. The mock
    functions are only to satisfy the linker and to observe behavior.
  - The tests cover: true and false branches, malloc failure path, and duplicate
    name handling as described in the problem statement.
*/

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <string>
#include <iostream>
#include <cstdlib>


// Lightweight, self-contained re-declarations of types and function signatures
// expected by sqlite3WithAdd. In a real environment, include the project's headers
// instead to ensure exact type layouts. Here we keep definitions minimal and
// compatible for the test harness.
extern "C" {

// Minimal sqlite3-like struct (only mallocFailed used by test)
struct sqlite3 { bool mallocFailed; };

// Minimal Parse, Cte, and With structures sufficient for sqlite3WithAdd usage.
// In the actual project these come from sqlite3.h + internal headers; tests rely
// on their presence and on the function prototype of sqlite3WithAdd.
struct Cte {
    char *zName;
};

struct With {
    int nCte;
    Cte a[1];  // flexible array member in production; used as a placeholder here
};

struct Parse {
    sqlite3 *db;
};

// Forward declaration of the function under test. The real implementation is assumed
// to be linked in at build time from build.c.
With* sqlite3WithAdd(Parse *pParse, With *pWith, Cte *pCte);

// External dependency stubs (will be provided by test harness)
void sqlite3ErrorMsg(Parse*, const char*, ...);
int  sqlite3StrICmp(const char*, const char*);
void* sqlite3DbMallocZero(sqlite3*, size_t);
void* sqlite3DbRealloc(sqlite3*, void*, size_t);
void  sqlite3DbFree(sqlite3*, void*);
void  sqlite3CteDelete(sqlite3*, Cte*);
}

// Test harness state
static std::string g_lastErrorMsg;
static bool        g_cteDeleted = false;
static bool        g_parserSetup = false;

// Mock implementations of dependencies used by sqlite3WithAdd

extern "C" void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
    (void)pParse; // suppress unused warning if not used in tests
    va_list ap;
    va_start(ap, zFormat);
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), zFormat, ap);
    va_end(ap);
    g_lastErrorMsg = buffer;
}

extern "C" int sqlite3StrICmp(const char* zLeft, const char* zRight) {
    if (zLeft == nullptr || zRight == nullptr) {
        if (zLeft == zRight) return 0;
        return (zLeft == nullptr) ? -1 : 1;
    }
    while (*zLeft && *zRight) {
        int c1 = std::tolower(static_cast<unsigned char>(*zLeft));
        int c2 = std::tolower(static_cast<unsigned char>(*zRight));
        if (c1 != c2) return c1 - c2;
        zLeft++; zRight++;
    }
    return std::tolower(static_cast<unsigned char()*zLeft?zLeft[0]:0>) - 
           std::tolower(static_cast<unsigned char()*zRight?zRight[0]:0>);
}

// Over-simplified malloc helpers to mimic sqlite3 memory mgmt for tests
static bool s_forceMallocFail = false;

extern "C" void* sqlite3DbMallocZero(sqlite3* db, size_t nByte) {
    (void)db;
    if (s_forceMallocFail) {
        db->mallocFailed = true;
        return nullptr;
    }
    void* p = calloc(1, nByte);
    return p;
}

extern "C" void* sqlite3DbRealloc(sqlite3* db, void* pOld, size_t nByte) {
    (void)db;
    void* p = realloc(pOld, nByte);
    return p;
}

extern "C" void sqlite3DbFree(sqlite3* db, void* p) {
    (void)db;
    free(p);
}

extern "C" void sqlite3CteDelete(sqlite3* db, Cte* p) {
    // Track deletion event for test assertions
    (void)db;
    g_cteDeleted = true;
    // Do not attempt to free embedded literal zName
    free(p);
}

/* Convenience test helpers */

static inline void reset_test_state() {
    g_lastErrorMsg.clear();
    g_cteDeleted = false;
    g_forceMallocFail = false;
}

// Test 1: If pCte == 0, sqlite3WithAdd should return pWith unchanged
void test_case_pCte_null_returns_input() {
    // Prepare a simple environment
    sqlite3 db;
    db.mallocFailed = false;
    Parse pParse;
    pParse.db = &db;

    // Existing WITH clause (pWith) with no CTEs
    With* pWith = (With*)malloc(sizeof(With));
    pWith->nCte = 0;
    // Create a trivial Cte to be safe if function accesses fields
    pWith->a[0].zName = nullptr;

    // pCte is NULL
    Cte* pCte = nullptr;

    // Call the function under test
    With* res = sqlite3WithAdd(&pParse, pWith, pCte);

    // Verifications
    bool ok = (res == pWith);
    std::cout << "[Test1] " << (ok ? "PASS" : "FAIL") << " - pCte==NULL should return pWith unchanged.\n";

    // Cleanup
    free(pWith);
}

// Test 2: Add a new CTE to an empty/NULL With (pWith == NULL)
void test_case_add_to_empty_with_null_pWith() {
    sqlite3 db;
    db.mallocFailed = false;
    Parse pParse; pParse.db = &db;

    // pWith is NULL
    With* pWith = nullptr;

    // pCte with name "cte1"
    Cte* pCte = (Cte*)malloc(sizeof(Cte));
    pCte->zName = (char*)"cte1";

    With* res = sqlite3WithAdd(&pParse, pWith, pCte);

    // Expect a non-null result with exactly one CTE copied
    bool ok = (res != nullptr) && (res->nCte == 1) && (strcmp(res->a[0].zName, "cte1") == 0);

    std::cout << "[Test2] " << (ok ? "PASS" : "FAIL") << " - Added CTE to NULL pWith should create new pWith with nCte=1.\n";

    // Cleanup
    if (res) {
        free(res);
    }
    // pCte was freed by sqlite3WithAdd as per code path
}

// Test 3: Duplicate CTE name should trigger an error message
void test_case_duplicate_name_error() {
    sqlite3 db;
    db.mallocFailed = false;
    Parse pParse; pParse.db = &db;

    // pWith with an existing CTE named "dup"
    With* pWith = (With*)malloc(sizeof(With));
    pWith->nCte = 1;
    pWith->a[0].zName = (char*)"dup";

    // pCte with same name "dup"
    Cte* pCte = (Cte*)malloc(sizeof(Cte));
    pCte->zName = (char*)"dup";

    With* res = sqlite3WithAdd(&pParse, pWith, pCte);

    // Expect an error message to be recorded
    bool errorSeen = !g_lastErrorMsg.empty() &&
                     g_lastErrorMsg.find("duplicate WITH table name: dup") != std::string::npos;

    std::cout << "[Test3] " << ((res != nullptr) && errorSeen ? "PASS" : "FAIL")
              << " - Duplicate name should trigger error message.\n";

    // Cleanup
    if (res && res != pWith) {
        free(res);
    }
    free(pWith);
    // pCte is freed by sqlite3WithAdd
}

// Test 4: Memory allocation failure path should cleanup and return NULL (or original pWith)
void test_case_malloc_failure_propagates_and_cleanup() {
    sqlite3 db;
    db.mallocFailed = false;
    Parse pParse; pParse.db = &db;

    // Force malloc failure in sqlite3DbMallocZero
    s_forceMallocFail = true;

    // pWith is NULL to go into mallocZero path
    With* pWith = nullptr;

    // pCte requires allocation
    Cte* pCte = (Cte*)malloc(sizeof(Cte));
    pCte->zName = (char*)"cteX";

    With* res = sqlite3WithAdd(&pParse, pWith, pCte);

    // Expect NULL result due to malloc failure and cleanup of pCte
    bool ok = (res == nullptr) && g_cteDeleted;

    std::cout << "[Test4] " << (ok ? "PASS" : "FAIL")
              << " - On malloc failure, function should cleanup and return NULL.\n";

    // Cleanup
    if (pWith) free(pWith);
}

// Main driver
int main() {
    // Run tests
    std::cout << "Running sqlite3WithAdd unit tests (C++ harness)\n";
    test_case_pCte_null_returns_input();
    test_case_add_to_empty_with_null_pWith();
    test_case_duplicate_name_error();
    test_case_malloc_failure_propagates_and_cleanup();

    std::cout << "Tests completed. Summary: (see individual test lines above)\n";
    return 0;
}
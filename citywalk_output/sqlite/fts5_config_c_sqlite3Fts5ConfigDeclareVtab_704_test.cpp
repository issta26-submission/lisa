// C++11 unit test suite for sqlite3Fts5ConfigDeclareVtab
// This test-suite provides minimal in-process mocks for the SQLite parts
// used by sqlite3Fts5ConfigDeclareVtab and exercises two scenarios:
// 1) OOM on first allocation (zSql becomes NULL, rc == SQLITE_NOMEM; no declare_vtab call)
// 2) Normal path with at least one column (zSql non-NULL, declare_vtab called with final string)

#include <fts5Int.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Include the internal FTS5 config header to obtain the Fts5Config type

// The test relies on standard sqlite3 error codes.
// If the environment doesn't provide sqlite3.h, we provide minimal fallbacks.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_NOMEM
#define SQLITE_NOMEM 7  // common fallback if not defined by the environment
#endif

// Forward declare sqlite3 type to satisfy the function signatures.
// We do not rely on full sqlite3 internals in tests.
struct sqlite3;

// Global variables to control/mock behavior in test scenarios
static bool g_force_oom_first = false;         // When true, first sqlite3Fts5Mprintf call returns NULL and sets rc to SQLITE_NOMEM
static int  g_mprintf_call_no = 0;              // Tracks number of calls to sqlite3Fts5Mprintf
static bool g_declared_called = false;          // Tracks if sqlite3_declare_vtab was invoked
static char *g_declared_sql_copy = NULL;        // Copy of zSql passed to sqlite3_declare_vtab

// Minimal stub for sqlite3 struct to satisfy type usage in declarations
struct sqlite3 {};

// Mock implementations for the SQLite-related functions used by the focal method.
// These are provided with C linkage to override/replace actual library implementations
// during unit testing in a single translation unit.

extern "C" {

// Mock: sqlite3Fts5Mprintf
// A deterministic mock that simulates memory allocation for zSql construction.
// Sequence:
//  - Call 0: returns "CREATE TABLE x("
//  - Call 1: returns a dummy string (content is not used by test assertions)
//  - Call 2+: returns "TEST_FINAL" used as the final zSql that gets passed to sqlite3_declare_vtab
char *sqlite3Fts5Mprintf(int *pRc, const char *zFormat, ...)
{
    // OOM injection on first allocation if requested
    if (g_force_oom_first && g_mprintf_call_no == 0) {
        if (pRc) *pRc = SQLITE_NOMEM;
        g_mprintf_call_no++;
        return NULL;
    }

    // Basic deterministic sequencing
    if (g_mprintf_call_no == 0) {
        g_mprintf_call_no++;
        if (pRc) *pRc = SQLITE_OK;
        return strdup("CREATE TABLE x(");
    } else if (g_mprintf_call_no == 1) {
        // One intermediate step for the loop body (content not asserted in tests)
        g_mprintf_call_no++;
        if (pRc) *pRc = SQLITE_OK;
        return strdup("DUMMY");
    } else {
        // Final step passes the string that will be given to sqlite3_declare_vtab
        g_mprintf_call_no++;
        if (pRc) *pRc = SQLITE_OK;
        // Distinguish between two-column scenarios could be added; for tests we use a fixed final string
        return strdup("TEST_FINAL");
    }
}

// Mock: sqlite3_declare_vtab
// Capture the final zSql passed to the declare vtab API and return SQLITE_OK
int sqlite3_declare_vtab(struct sqlite3 *db, const char *zSql)
{
    (void)db; // unused in test harness
    // Copy the provided SQL so tests can inspect it after function returns
    if (zSql) {
        g_declared_called = true;
        g_declared_sql_copy = strdup(zSql);
    } else {
        g_declared_called = true;
        g_declared_sql_copy = NULL;
    }
    return SQLITE_OK;
}

// Mock: sqlite3_free
// Simple wrapper around free to manage memory allocated by mocks
void sqlite3_free(void *p)
{
    free(p);
}

} // extern "C"


// Utility to reset test state before each test
static void reset_test_state()
{
    g_force_oom_first = false;
    g_mprintf_call_no = 0;
    g_declared_called = false;
    if (g_declared_sql_copy) {
        free(g_declared_sql_copy);
        g_declared_sql_copy = NULL;
    }
}

// Test 1: OOM on first allocation
// Expectation:
//  - sqlite3Fts5ConfigDeclareVtab returns SQLITE_NOMEM
//  - No call to sqlite3_declare_vtab (g_declared_called remains false)
bool test_oom_on_first_allocation()
{
    reset_test_state();
    g_force_oom_first = true; // inject OOM on first Mprintf

    // Prepare a minimal Fts5Config instance
    Fts5Config cfg;
    // nCol = 0 to keep test minimal; no azCol usage on this path
    cfg.nCol = 0;
    static const char *cols0[] = {};
    cfg.azCol = cols0;
    cfg.zName = "test_foo";
    cfg.db = (sqlite3*)&(sqlite3{}); // dummy db pointer for test

    int rc = sqlite3Fts5ConfigDeclareVtab(&cfg);

    // Validate outcome
    bool ok = true;
    if (rc != SQLITE_NOMEM) {
        std::cerr << "TEST-OOM-FIRST: Expected rc SQLITE_NOMEM, got " << rc << std::endl;
        ok = false;
    }
    if (g_declared_called) {
        std::cerr << "TEST-OOM-FIRST: sqlite3_declare_vtab should not have been called on OOM." << std::endl;
        ok = false;
    }
    return ok;
}


// Test 2: Normal path with at least one column
// Expectation:
//  - sqlite3Fts5ConfigDeclareVtab returns SQLITE_OK
//  - sqlite3_declare_vtab is called with the final SQL string produced by sqlite3Fts5Mprintf
//  - The final zSql passed to sqlite3_declare_vtab is captured (TEST_FINAL in this mock)
bool test_normal_path_one_column()
{
    reset_test_state();
    g_force_oom_first = false; // ensure normal path

    // Prepare a Fts5Config with a single column
    Fts5Config cfg;
    cfg.nCol = 1;
    const char *cols[1] = { "colA" };
    cfg.azCol = cols;
    cfg.zName = "myIndex";
    cfg.db = (sqlite3*)&(sqlite3{}); // dummy db pointer

    int rc = sqlite3Fts5ConfigDeclareVtab(&cfg);

    // Validate outcome
    bool ok = true;
    if (rc != SQLITE_OK) {
        std::cerr << "TEST-NORMAL-1: Expected rc SQLITE_OK, got " << rc << std::endl;
        ok = false;
    }
    if (!g_declared_called) {
        std::cerr << "TEST-NORMAL-1: Expected sqlite3_declare_vtab to be called, but it was not." << std::endl;
        ok = false;
    }
    if (g_declared_sql_copy == NULL) {
        std::cerr << "TEST-NORMAL-1: sqlite3_declare_vtab received NULL zSql." << std::endl;
        ok = false;
    } else {
        // Ensure the final string was the one produced by the mock
        const std::string expected = "TEST_FINAL";
        if (expected != g_declared_sql_copy) {
            std::cerr << "TEST-NORMAL-1: Expected final zSql \"" << expected
                      << "\", got \"" << g_declared_sql_copy << "\"." << std::endl;
            ok = false;
        } else {
            // Optional: print success info for clarity
            std::cout << "TEST-NORMAL-1: Declared vtab received expected final SQL: " << g_declared_sql_copy << std::endl;
        }
    }

    // Cleanup copied SQL
    if (g_declared_sql_copy) {
        free(g_declared_sql_copy);
        g_declared_sql_copy = NULL;
    }

    return ok;
}


// Main runner
int main()
{
    std::cout << "Running sqlite3Fts5ConfigDeclareVtab unit tests (C++11)..." << std::endl;

    bool all_ok = true;

    if (!test_oom_on_first_allocation()) {
        all_ok = false;
        std::cerr << "FAIL: test_oom_on_first_allocation" << std::endl;
    } else {
        std::cout << "PASS: test_oom_on_first_allocation" << std::endl;
    }

    if (!test_normal_path_one_column()) {
        all_ok = false;
        std::cerr << "FAIL: test_normal_path_one_column" << std::endl;
    } else {
        std::cout << "PASS: test_normal_path_one_column" << std::endl;
    }

    if (all_ok) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
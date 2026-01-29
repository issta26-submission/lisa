// Self-contained C++11 unit test suite for the focal method: db_include_search
// This test suite is designed to be compiled and linked in isolation.
// It provides minimal in-test stubs for the dependencies used by db_include_search
// (as per the given focal method) to enable deterministic testing without requiring
// an actual SQLite runtime.
//
// The tests cover:
// - True branch: a row is found and the function returns an allocated name.
// - False branch: no row is found; function returns nullptr.
// - Null text branch: the DB returns NULL for the text; function returns nullptr.
// - Allocation failure: sqlite3_mprintf returns nullptr and fatal is invoked.
// - Reuse of existing prepared statement: subsequent calls do not re-call db_prepare.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdexcept>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// -------------- Minimal in-test declarations to mimic the environment --------------
// We use C linkage for the focal function and dependent stubs to mirror the real project.

extern "C" {

// Forward-declare a minimal sqlite3_stmt type (opaque in this mock)
typedef void sqlite3_stmt;

// Simulated SQLITE_ROW return code (actual value from SQLite is 100; we use 100 here)
#define SQLITE_ROW 100

// Global state mirroring the real program's usage
typedef struct {
    sqlite3_stmt *inclSearch; // prepared statement handle
} StmtHolder;

typedef struct {
    StmtHolder stmt;
} GlobalState;

// The real project uses a global g variable. We declare it here to be used by the focal method.
// Note: In this isolated test, the test harness provides this symbol (definition follows in code).
extern GlobalState g;

// Helper: last bound text (for traceability if needed)
static const char *last_bound_text = nullptr;

// Helper: what the next db_step() should return (simulate SQLite behavior)
static int next_step_result = 0;

// Helper: what sqlite3_column_text() should return (simulate rowlice string)
static const unsigned char *mock_column_text_ptr = nullptr;

// Helper: track whether db_prepare() was invoked (for test 5)
static int prepare_call_count = 0;

// Tiny memory management for test lifetime of returned strings
static void reset_memory_scaffold() {
    next_step_result = 0;
    mock_column_text_ptr = nullptr;
    last_bound_text = nullptr;
    prepare_call_count = 0;
}

// Forward declarations of the dependency stubs used by db_include_search
void db_prepare(sqlite3_stmt **pStmt, const char *zSql, ...);
void db_bind_text(sqlite3_stmt *pStmt, int col, const char *zStr);
int  db_step(sqlite3_stmt *pStmt);
const unsigned char * sqlite3_column_text(sqlite3_stmt *pStmt, int iCol);
void sqlite3_clear_bindings(sqlite3_stmt *pStmt);
void sqlite3_reset(sqlite3_stmt *pStmt);
char * sqlite3_mprintf(const char *format, const unsigned char *z);

}

// -------------- Global state instance --------------
// Provide the actual global used by the focal method (as in the original project)
GlobalState g = { { nullptr } };

// -------------- Implementations of the test stubs (extern "C") --------------
extern "C" {

// Simple, opaque sqlite3_stmt representation (mock)
typedef void sqlite3_stmt;

// db_prepare: allocate a dummy statement if not already set; track invocation
void db_prepare(sqlite3_stmt **pStmt, const char *zSql, ...)
{
    (void)zSql; // zSql is not used in this mock
    if (*pStmt == nullptr) {
        // Use a non-null sentinel to simulate "prepared"
        *pStmt = (sqlite3_stmt*) (uintptr_t) 0xABCD; // mock handle
        // Mirror the real state's nested member
        g.stmt.inclSearch = *pStmt;
        prepare_call_count++;
    }
}

// db_bind_text: store bound text (for completeness)
void db_bind_text(sqlite3_stmt *pStmt, int col, const char *zStr)
{
    (void)pStmt; (void)col;
    last_bound_text = zStr;
}

// db_step: controlled by test via next_step_result
int db_step(sqlite3_stmt *pStmt)
{
    (void)pStmt;
    return next_step_result;
}

// sqlite3_column_text: return the provided mock text
const unsigned char * sqlite3_column_text(sqlite3_stmt *pStmt, int iCol)
{
    (void)pStmt; (void)iCol;
    return mock_column_text_ptr;
}

// sqlite3_clear_bindings: no-op in this test stub
void sqlite3_clear_bindings(sqlite3_stmt *pStmt)
{
    (void)pStmt;
}

// sqlite3_reset: no-op in this test stub
void sqlite3_reset(sqlite3_stmt *pStmt)
{
    (void)pStmt;
}

// sqlite3_mprintf: simulate allocation success/failure and return a duplicative string
char * sqlite3_mprintf(const char *format, const unsigned char *z)
{
    (void)format; // we only support "%s" usage in this test
    if (z == nullptr) return nullptr;

    // Simulate allocation failure if the bound string is "FAIL_ALLOC"
    const char* s = reinterpret_cast<const char*>(z);
    if (std::strcmp(s, "FAIL_ALLOC") == 0) {
        return nullptr; // simulate allocation failure
    }

    // Normal path: allocate and copy the string
    size_t len = std::strlen(s);
    char *out = static_cast<char*>(std::malloc(len + 1));
    if (!out) return nullptr;
    std::memcpy(out, s, len);
    out[len] = '\0';
    return out;
}

} // extern "C"

// -------------- The focal method under test (redeclared with C linkage) --------------
extern "C" {

char * db_include_search(const char * zKey)
{
    char * zName = 0;
    if(!g.stmt.inclSearch){
        db_prepare(&g.stmt.inclSearch,
                   "SELECT ?1 fn WHERE fileExists(fn) "
                   "UNION ALL SELECT * FROM ("
                   "SELECT replace(dir||'/'||?1, '//','/') AS fn "
                   "FROM inclpath WHERE fileExists(fn) ORDER BY seq"
                   ")");
    }
    db_bind_text(g.stmt.inclSearch, 1, zKey);
    if(SQLITE_ROW==db_step(g.stmt.inclSearch)){
        const unsigned char * z = sqlite3_column_text(g.stmt.inclSearch, 0);
        zName = z ? sqlite3_mprintf("%s", z) : 0;
        if(!zName) {
            // In real code this would call fatal; in test, throw to signal failure
            throw std::runtime_error("Alloc failed");
        }
    }
    sqlite3_clear_bindings(g.stmt.inclSearch);
    sqlite3_reset(g.stmt.inclSearch);
    return zName;
}

} // extern "C"

// -------------- Lightweight test harness --------------

static void expect_true(bool cond, const char *msg) {
    if (!cond) {
        throw std::runtime_error(std::string("ASSERT FAILED: ") + msg);
    }
}

// Test 1: True branch: a row is found and the function returns an allocated name
bool test_true_branch_returns_allocated_name() {
    // Reset state
    g.stmt.inclSearch = nullptr;
    prepare_call_count = 0;
    next_step_result = SQLITE_ROW;

    // Mock the column text to be returned
    mock_column_text_ptr = reinterpret_cast<const unsigned char*>("path/to/file.h");

    // Call the focal function
    char *res = db_include_search("someInput.h");

    // Verify: non-null and content equals the mock column text
    bool ok = (res != nullptr) && (std::strcmp(res, "path/to/file.h") == 0);

    // Free allocated result if any
    if (res) std::free(res);

    // Cleanup for next tests
    g.stmt.inclSearch = nullptr;
    mock_column_text_ptr = nullptr;
    return ok && (prepare_call_count == 1);
}

// Test 2: False branch: no row found, function returns nullptr
bool test_false_branch_no_row_found() {
    // Reset state
    g.stmt.inclSearch = nullptr;
    prepare_call_count = 0;
    next_step_result = 0; // NOT SQLITE_ROW

    mock_column_text_ptr = reinterpret_cast<const unsigned char*>(""); // should be ignored

    char *res = db_include_search("noRowInput.h");

    // Should be nullptr when no row is produced
    bool ok = (res == nullptr);

    if (res) std::free(res);
    g.stmt.inclSearch = nullptr;
    mock_column_text_ptr = nullptr;
    return ok && (prepare_call_count == 1); // db_prepare should still be called once for a first-time path
}

// Test 3: Null text branch: DB returns NULL text, function returns nullptr
bool test_null_text_branch_returns_null() {
    g.stmt.inclSearch = nullptr;
    prepare_call_count = 0;
    next_step_result = SQLITE_ROW;
    mock_column_text_ptr = nullptr; // simulate NULL text

    char *res = db_include_search("nullTextInput.h");

    bool ok = (res == nullptr);
    if (res) std::free(res);
    g.stmt.inclSearch = nullptr;
    mock_column_text_ptr = nullptr;
    return ok;
}

// Test 4: Allocation failure inside sqlite3_mprintf triggers fatal
bool test_allocation_failure_triggers_fatal() {
    // Reset state
    g.stmt.inclSearch = nullptr;
    prepare_call_count = 0;
    next_step_result = SQLITE_ROW;
    mock_column_text_ptr = reinterpret_cast<const unsigned char*>("FAIL_ALLOC"); // trigger alloc fail

    try {
        char *res = db_include_search("allocFail.h");
        if (res) {
            std::free(res);
            // If we get here, alloc didn't fail as designed
            return false;
        } else {
            // Should not happen because our fatal path throws
            return false;
        }
    } catch (const std::exception &) {
        // Expected path: fatal called
        g.stmt.inclSearch = nullptr;
        mock_column_text_ptr = nullptr;
        return true;
    }
}

// Test 5: Reuse existing prepared statement; db_prepare should not be called again
bool test_reuse_existing_prepared_statement_no_reinit() {
    // Pre-populate a non-null inclSearch to simulate existing prepared statement
    g.stmt.inclSearch = (sqlite3_stmt*) (uintptr_t) 0xDEAD;
    prepare_call_count = 0; // should not be incremented since we skip db_prepare
    next_step_result = SQLITE_ROW;
    mock_column_text_ptr = reinterpret_cast<const unsigned char*>("reuse/path/file.h");

    char *res = db_include_search("reuseInput.h");

    bool ok = (res != nullptr) && (std::strcmp(res, "reuse/path/file.h") == 0);
    if (res) std::free(res);

    // Cleanup
    g.stmt.inclSearch = nullptr;
    mock_column_text_ptr = nullptr;

    // We expect that db_prepare was not called (0)
    return ok && (prepare_call_count == 0);
}

// -------------- Test runner --------------
int main() {
    bool all_passed = true;

    auto run = [&](const char* name, bool (*test)()) {
        try {
            bool ok = test();
            if (ok) {
                std::cout << "[PASS] " << name << std::endl;
            } else {
                std::cout << "[FAIL] " << name << std::endl;
                all_passed = false;
            }
        } catch (const std::exception &e) {
            std::cout << "[EXC] " << name << " threw: " << e.what() << std::endl;
            all_passed = false;
        } catch (...) {
            std::cout << "[EXC] " << name << " threw an unknown exception" << std::endl;
            all_passed = false;
        }
    };

    run("db_include_search - true branch returns allocated name", test_true_branch_returns_allocated_name);
    run("db_include_search - false branch returns nullptr", test_false_branch_no_row_found);
    run("db_include_search - null text returns nullptr", test_null_text_branch_returns_null);
    run("db_include_search - allocation failure triggers fatal", test_allocation_failure_triggers_fatal);
    run("db_include_search - reuse existing prepared statement", test_reuse_existing_prepared_statement_no_reinit);

    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
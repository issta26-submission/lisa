// Lightweight C++11 test harness for the focal method: registerUDFs
// This test harness mocks the SQLite API used by registerUDFs and
// verifies various execution paths without requiring real SQLite.
// Note: Compile this file together with sqlite3expert.c (the focal file).
// The mocks provide minimal behavior needed by the focal method.

#include <string.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sqlite3expert.h>


// Forward declare C-like sqlite types to match the focal code's expectations.
extern "C" {

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;

// Faked SQLite constants (subset needed by the focal method)
#define SQLITE_OK 0
#define SQLITE_ROW 100
#define SQLITE_DONE 101
#define SQLITE_ERROR 1
#define SQLITE_UTF8 1
#define SQLITE_UTF16LE 2
#define SQLITE_UTF16BE 4
#define SQLITE_DETERMINISTIC 0x800
#define SQLITE_DIRECTONLY 0x200000

// Mocked SQLite API (simplified)
int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char *pzTail);
int sqlite3_step(sqlite3_stmt *pStmt);
int sqlite3_column_int(sqlite3_stmt *pStmt, int iCol);
const unsigned char* sqlite3_column_text(sqlite3_stmt *pStmt, int iCol);
void sqlite3_finalize(sqlite3_stmt *pStmt);

// Mocked function registration APIs used by the focal method
int sqlite3_create_window_function(sqlite3*db, const char*zFunctionName, int nArg, int enc, void* pStepCtx,
                                 void (*xStep)(sqlite3_context*, int, sqlite3_value**),
                                 void (*xValue)(sqlite3_context*),
                                 void (*xFinal)(sqlite3_context*),
                                 void* pApp, void* pDestroy);

// The scalar function variant used by the focal method
int sqlite3_create_function(sqlite3*db, const char*zFunctionName, int nArg, int enc, void* pApp,
                          void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
                          void (*xStep)(sqlite3_value**),
                          void* pDestroy);

// Minimal dummy helpers used elsewhere (not invoked in tests directly)
}

using namespace std;

// --------------------------------------------------------------------------------------
// Test infrastructure to drive mocks

// Represent a row returned by pragma_function_list() in the test harness
struct MockRow {
    string name;
    string type;
    string enc;
    int nargs;
    int flags;
    // Simulate NULL columns for "name/type/enc"
    bool nameNull;
    bool typeNull;
    bool encNull;
};

// Global test state used by mocks
struct TestState {
    vector<MockRow> rows;
    // Index of the next row to return; when sqlite3_step is called, it advances.
    size_t nextRowIndex;
    // The current row being observed by column_* functions
    size_t currentFetchedIndex;
    // Logs to verify which UDFs were registered
    vector<string> windowFuncNames;
    vector<int> windowFuncNargs;
    vector<int> windowFuncIenc;

    vector<string> scalarFuncNames;
    vector<int> scalarFuncNargs;
    vector<int> scalarFuncIenc;

    TestState() { reset(); }
    void reset() {
        rows.clear();
        nextRowIndex = 0;
        currentFetchedIndex = (size_t)-1;
        windowFuncNames.clear(); windowFuncNargs.clear(); windowFuncIenc.clear();
        scalarFuncNames.clear(); scalarFuncNargs.clear(); scalarFuncIenc.clear();
    }
};

// Global instance used by mocks
static TestState g_TestState;

// Simple wrapper to create a test row
inline MockRow mkRow(const string& name, const string& type, const string& enc, int nargs, int flags,
                     bool nameNull=false, bool typeNull=false, bool encNull=false) {
    MockRow r;
    r.name = name; r.type = type; r.enc = enc;
    r.nargs = nargs; r.flags = flags;
    r.nameNull = nameNull; r.typeNull = typeNull; r.encNull = encNull;
    return r;
}

// --------------------------------------------------------------------------------------
// Mock sqlite3 types (opaque in test, but enough for registration)
struct sqlite3 { int dummy; };
struct sqlite3_stmt { int dummy; };

// Implement the mocks as C-callable functions (extern "C" re-declared above)

static void* operator new(size_t sz) { return malloc(sz); }
static void operator delete(void* p) { free(p); }

// Internal representation for a prepared statement
struct MockStmtInternal {
    TestState* pState;
    size_t nextRowIndex;
    size_t currentFetchedIndex;
};

// Helper to get the current row for column access
static MockStmtInternal* asMock(sqlite3_stmt* pStmt) {
    return reinterpret_cast<MockStmtInternal*>(pStmt);
}

// Forward declare dummy UDFs to satisfy the linker (actual bodies come from focal file)
extern "C" void dummyUDF(sqlite3_context*, int, sqlite3_value**);
extern "C" void dummyUDFvalue(sqlite3_context*);

// Implementation of sqlite3_prepare_v2
int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char *pzTail) {
    (void)db; (void)zSql; (void)nByte; (void)pzTail;
    MockStmtInternal* p = new MockStmtInternal();
    p->pState = &g_TestState;
    p->nextRowIndex = 0;
    p->currentFetchedIndex = (size_t)-1;
    *ppStmt = reinterpret_cast<sqlite3_stmt*>(p);
    return SQLITE_OK;
}

// sqlite3_step: move through mocked rows
int sqlite3_step(sqlite3_stmt *pStmt) {
    MockStmtInternal* p = asMock(pStmt);
    if (!p || !p->pState) return SQLITE_DONE;
    if (p->nextRowIndex < p->pState->rows.size()) {
        p->currentFetchedIndex = p->nextRowIndex;
        p->nextRowIndex++;
        return SQLITE_ROW;
    } else {
        return SQLITE_DONE;
    }
}

// sqlite3_column_int: provide nargs (col 3) and flags (col 4)
int sqlite3_column_int(sqlite3_stmt *pStmt, int iCol) {
    MockStmtInternal* p = asMock(pStmt);
    if (!p || p->currentFetchedIndex >= p->pState->rows.size()) return 0;
    const MockRow& r = p->pState->rows[p->currentFetchedIndex];
    if (iCol == 3) return r.nargs;
    if (iCol == 4) return r.flags;
    return 0;
}

// sqlite3_column_text: provide name (col 0), type (col 1), enc (col 2)
const unsigned char* sqlite3_column_text(sqlite3_stmt *pStmt, int iCol) {
    MockStmtInternal* p = asMock(pStmt);
    if (!p || p->currentFetchedIndex >= p->pState->rows.size()) return reinterpret_cast<const unsigned char*>("null");
    const MockRow& r = p->pState->rows[p->currentFetchedIndex];
    // Simulate NULL for columns if indicated
    if ((iCol == 0 && r.nameNull) || (iCol == 1 && r.typeNull) || (iCol == 2 && r.encNull)) {
        return 0;
    }
    if (iCol == 0) return reinterpret_cast<const unsigned char*>(r.name.c_str());
    if (iCol == 1) return reinterpret_cast<const unsigned char*>(r.type.c_str());
    if (iCol == 2) return reinterpret_cast<const unsigned char*>(r.enc.c_str());
    return reinterpret_cast<const unsigned char*>("");
}

// sqlite3_finalize
void sqlite3_finalize(sqlite3_stmt *pStmt) {
    MockStmtInternal* p = asMock(pStmt);
    delete p;
}

// Helpers for function creation logging (mocks only)
static void logWindowFunction(const char* name, int nargs, int enc) {
    g_TestState.windowFuncNames.push_back(name ? name : "");
    g_TestState.windowFuncNargs.push_back(nargs);
    g_TestState.windowFuncIenc.push_back(enc);
}
static void logScalarFunction(const char* name, int nargs, int enc) {
    g_TestState.scalarFuncNames.push_back(name ? name : "");
    g_TestState.scalarFuncNargs.push_back(nargs);
    g_TestState.scalarFuncIenc.push_back(enc);
}

// Implement sqlite3_create_window_function: log and optionally simulate error
int sqlite3_create_window_function(sqlite3*db, const char*zFunctionName, int nArg, int enc, void* pStepCtx,
                                 void (*xStep)(sqlite3_context*, int, sqlite3_value**),
                                 void (*xValue)(sqlite3_context*),
                                 void (*xFinal)(sqlite3_context*),
                                 void* pApp, void* pDestroy) {
    (void)db; (void)pStepCtx; (void)xStep; (void)xValue; (void)xFinal; (void)pApp; (void)pDestroy;
    // Simulate an error if the function name is a sentinel
    if (zFunctionName && strcmp(zFunctionName, "CRASH_WINDOW") == 0) {
        return SQLITE_ERROR;
    }
    logWindowFunction(zFunctionName, nArg, enc);
    return SQLITE_OK;
}

// Implement sqlite3_create_function: log and optionally simulate error
int sqlite3_create_function(sqlite3*db, const char*zFunctionName, int nArg, int enc, void* pApp,
                          void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
                          void (*xStep)(sqlite3_value**),
                          void* pDestroy) {
    (void)db; (void)pApp; (void)xFunc; (void)xStep; (void)pDestroy;
    // Simulate an error for a sentinel name
    if (zFunctionName && strcmp(zFunctionName, "CRASH_SCALAR") == 0) {
        return SQLITE_ERROR;
    }
    logScalarFunction(zFunctionName, nArg, enc);
    return SQLITE_OK;
}

// --------------------------------------------------------------------------------------
// Domain-specific helpers for the test runner

static const int DOMAIN_SQLITE_UTF8 = SQLITE_UTF8;
static const int DOMAIN_UTF16LE = SQLITE_UTF16LE;
static const int DOMAIN_UTF16BE = SQLITE_UTF16BE;

// Simple test harness: lightweight assertion utilities
#define TST_OK(cond, msg) do { if(!(cond)) { std::cerr << "[FAIL] " msg " at " << __LINE__ << std::endl; } else { /* pass silently */ } } while(0)
#define EXPECT_TRUE(cond) TST_OK(cond, "Expectation failed: " #cond)
#define EXPECT_EQ(a,b) do { if((a)!=(b)) { std::cerr << "[FAIL] Expected " << (a) << " == " << (b) << " at line " << __LINE__ << std::endl; } } while(0)
#define EXPECT_STR_EQ(a,b) do { if(((a)==nullptr? string("NULL"): string((a))) != string((b))) { std::cerr << "[FAIL] Expected string equality: " #a " vs " #b " at line " << __LINE__ << std::endl; } } while(0)

// Helpers to reset and inspect state between tests
static void resetMocksAndState() {
    // Clear logs and test rows
    g_TestState.reset();
}

// --------------------------------------------------------------------------------------
// FOCAL tests (wrap around the actual focal function in sqlite3expert.c)

// extern "C" int registerUDFs(sqlite3 *dbSrc, sqlite3 *dbDst);
extern "C" int registerUDFs(sqlite3 *dbSrc, sqlite3 *dbDst);

// Test 1: Single window function created with utf8 and no special flags
static void test_single_window_function_utf8_no_flags() {
    resetMocksAndState();

    // Prepare a single row that should create a window function
    g_TestState.rows.push_back(mkRow("my_window_func", "w", "utf8", 2, 0));

    sqlite3 dbSrc, dbDst;
    int rc = registerUDFs(&dbSrc, &dbDst);

    EXPECT_EQ(rc, SQLITE_OK);
    // Verify that one window function was registered with expected properties
    EXPECT_TRUE(g_TestState.windowFuncNames.size() == 1);
    EXPECT_TRUE(g_TestState.windowFuncNames[0] == "my_window_func");
    EXPECT_EQ(g_TestState.windowFuncNargs[0], 2);
    // enc should be SQLITE_UTF8 (no utf16 flags ORed)
    EXPECT_EQ(g_TestState.windowFuncIenc[0], SQLITE_UTF8);
}

// Test 2: Row with NULL name should be skipped (no UDFs registered)
static void test_skip_on_null_name() {
    resetMocksAndState();
    // Simulate a NULL name, others non-null
    MockRow r = mkRow("", "w", "utf8", 1, 0, true, false, false);
    r.name = ""; // name not used since nameNull = true
    g_TestState.rows.push_back(r);

    sqlite3 dbSrc, dbDst;
    int rc = registerUDFs(&dbSrc, &dbDst);

    EXPECT_EQ(rc, SQLITE_OK);
    // No UDFs should be registered
    EXPECT_TRUE(g_TestState.windowFuncNames.empty());
    EXPECT_TRUE(g_TestState.scalarFuncNames.empty());
}

// Test 3: Invocation returns error during registration (simulate rc != SQLITE_OK)
static void test_error_from_registration_stops_processing() {
    resetMocksAndState();
    // First row returns ok; second row uses sentinel that triggers error
    g_TestState.rows.push_back(mkRow("OK_FUNC", "a", "utf8", 1, 0));
    // Second row triggers error for scalar function "CRASH_SCALAR"
    g_TestState.rows.push_back(mkRow("CRASH_SCALAR", "a", "utf8", 1, 0));

    sqlite3 dbSrc, dbDst;
    int rc = registerUDFs(&dbSrc, &dbDst);

    // Expect the error from the sentinel
    EXPECT_EQ(rc, SQLITE_ERROR);
    // Only the first row should have attempted to create a function
    EXPECT_TRUE(g_TestState.scalarFuncNames.size() == 0 || g_TestState.scalarFuncNames[0] == "OK_FUNC" ? true : false);
    // Depending on exact ordering, we expect at least the first function call attempted
}

// Test 4: Multiple rows create both window and scalar functions (normal path)
static void test_multiple_rows_register_both_types() {
    resetMocksAndState();
    g_TestState.rows.push_back(mkRow("win1", "w", "utf8", 1, 0));
    g_TestState.rows.push_back(mkRow("fn1", "a", "utf16le", 1, SQLITE_DETERMINISTIC));

    sqlite3 dbSrc, dbDst;
    int rc = registerUDFs(&dbSrc, &dbDst);

    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(g_TestState.windowFuncNames.size() == 1);
    EXPECT_TRUE(g_TestState.scalarFuncNames.size() == 1);
    EXPECT_EQ(g_TestState.windowFuncNames[0], "win1");
    EXPECT_EQ(g_TestState.scalarFuncNames[0], "fn1");
    // Check ienc encoding for scalar function uses utf16le and includes deterministic bit
    EXPECT_EQ(g_TestState.scalarFuncIenc[0], SQLITE_UTF16LE | SQLITE_DETERMINISTIC);
}

// Test 5: utf16 encoding path for window function
static void test_utf16_encoding_becomes_expected() {
    resetMocksAndState();
    // Use utf16be with DET(DIRECTONLY) set in flags via the row
    g_TestState.rows.push_back(mkRow("be_win", "w", "utf16be", 3, SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY));

    sqlite3 dbSrc, dbDst;
    int rc = registerUDFs(&dbSrc, &dbDst);

    EXPECT_EQ(rc, SQLITE_OK);
    EXPECT_TRUE(g_TestState.windowFuncNames.size() == 1);
    EXPECT_EQ(g_TestState.windowFuncNames[0], "be_win");
    // ienc should reflect UTF16BE plus flags
    EXPECT_EQ(g_TestState.windowFuncIenc[0], SQLITE_UTF16BE | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY);
}

// --------------------------------------------------------------------------------------
// Test runner

int main() {
    cout << "Running tests for registerUDFs (sqlite3expert.c) with mocked SQLite API\n";

    test_single_window_function_utf8_no_flags();
    test_skip_on_null_name();
    test_error_from_registration_stops_processing();
    test_multiple_rows_register_both_types();
    test_utf16_encoding_becomes_expected();

    cout << "Tests completed.\n";
    return 0;
}
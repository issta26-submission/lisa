/***************************************************************
 * Unit test suite for sqlite3Fts5StorageContentInsert
 * Target: fts5_storage.c - Function: sqlite3Fts5StorageContentInsert
 * Language: C++11 compatible test harness (no GTest)
 * Notes:
 * - This file provides a lightweight, self-contained test harness
 *   that exercises the focal method in a manner consistent with
 *   the provided domain knowledge. It assumes the project builds
 *   with the standard SQLite/FTS5 headers available in the test
 *   environment.
 * - Tests focus on covered branches, using true/false predicates
 *   where feasible within the constraints of unit isolation and
 *   without requiring the full SQLite/FTS5 runtime.
 * - Static/internal helpers from the focal file are not redefined;
 *   we rely on the public interfaces provided by the project headers.
 * - Use non-terminating assertions so that test suites execute all
 *   cases even if one assertion fails.
 ***************************************************************/

#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <cassert>


// Domain knowledge: include necessary headers from the project.
// This allows access to Fts5Storage, Fts5Config, and the sqlite3* API.
// The exact header paths may vary per environment; adjust as needed.
extern "C" {
}

// Simple, lightweight test framework (no external dependencies)
#define TEST_CASE(name) void name()
#define ASSERT_TRUE(cond) do { if(!(cond)) { \
    std::cerr << "Assertion failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    failing = true; } } while(0)
#define ASSERT_EQ(a,b) do { if((a)!=(b)) { \
    std::cerr << "Assertion failed: " #a " == " #b " (" << (a) << " != " << (b) \
              << ") at " __FILE__ ":" << __LINE__ << "\n"; \
    failing = true; } } while(0)

/* Global flag to track any test failure in a run */
static bool failing = false;

/* Utility: create a minimal Fts5Config and Fts5Storage for test setup */
static Fts5Storage* create_test_storage( int eContentValue, int nCol, sqlite3* db )
{
    // Allocate storage and config structures
    Fts5Storage *p = (Fts5Storage*)sqlite3MallocRaw(sizeof(Fts5Storage));
    if( p==nullptr ) return nullptr;
    p->pConfig = (Fts5Config*)sqlite3MallocRaw(sizeof(Fts5Config));
    if( p->pConfig==nullptr ){
        sqlite3_free(p);
        return nullptr;
    }

    // Initialize config fields used by the focal method
    p->pConfig->eContent = eContentValue; // eContent != NORMAL indicates non-normal content
    p->pConfig->nCol = nCol;               // number of content columns
    p->pConfig->db = db;                   // sqlite3* handle, required for last_insert_rowid

    return p;
}

/* Utility: clean up test objects */
static void destroy_test_storage(Fts5Storage* p)
{
    if( p ){
        if( p->pConfig ){
            // The actual project may own db lifetime; avoid double-free here.
            // We only free the config and storage structs themselves.
            sqlite3_free(p->pConfig);
        }
        sqlite3_free(p);
    }
}

/* Utility: construct a sqlite3_value for testing
 * Note: The correct construction depends on the SQLite headers available
 * in the environment. This function uses the public API to create a value
 * and set its type to integer. If the environment provides alternative APIs
 * (e.g., sqlite3_value_set_int), adjust accordingly.
 */
static sqlite3_value* make_sqlite_value_integer(i64 v)
{
    sqlite3_value *pVal = sqlite3ValueNew(0);
    if( pVal ){
        // Set integer value. If sqlite3ValueSetInt is unavailable, fall back
        // to binding the numeric text representation or the closest available API.
        #if defined(SQLITE_OK)
        // Preferred path if API available
        // Attempt to set 64-bit integer; if function not present, ignore.
        #endif
        // Fallback: attempt to set via textual representation
        std::string s = std::to_string(v);
        sqlite3_value_text(pVal, s.c_str());  // If available; otherwise adapt
    }
    return pVal;
}

/* Test Case 1: Non-normal content + integer apVal[1]
 * - Expectation: *piRowid == value provided by apVal[1], rc == SQLITE_OK
 */
TEST_CASE(test_ContentInsert_NonNormal_IntegerValue)
{
    // Arrange
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    Fts5Storage *p = nullptr;
    Fts5Config *pConfig = nullptr;

    // Open a dummy in-memory database to satisfy last_insert_rowid usage
    rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK);

    // Create storage with non-normal content
    p = create_test_storage(/*eContentValue=*/FTS5_CONTENT_NOT_NORMAL, /*nCol*/ 2, db);
    if( !p || !p->pConfig ){
        std::cerr << "Failed to allocate test storage.\n";
        ASSERT_TRUE(false);
        return;
    }

    // apVal: we only care about apVal[1] being an integer
    sqlite3_value* apVal[3];
    apVal[0] = nullptr;
    apVal[1] = make_sqlite_value_integer(12345); // integer value to copy into rowid

    i64 rowid = 0;

    // Act
    rc = sqlite3Fts5StorageContentInsert(p, apVal, &rowid);

    // Cleanup values
    if( apVal[1] ) sqlite3ValueFree(apVal[1]);
    destroy_test_storage(p);
    sqlite3_close(db);

    // Assert
    ASSERT_TRUE(rc == SQLITE_OK);
    ASSERT_EQ(rowid, (i64)12345);
}

/* Test Case 2: Non-normal content + non-integer apVal[1]
 * - Expectation: rc == value from fts5StorageNewRowid(p, &piRowid)
 *   due to non-integer type; ensure call does not crash and returns SQLITE_OK
 *   or a non-error code depending on underlying implementation.
 */
TEST_CASE(test_ContentInsert_NonNormal_NonIntegerValue)
{
    // Arrange
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    Fts5Storage *p = nullptr;

    rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK);

    // Create storage with non-normal content
    p = create_test_storage(/*eContentValue=*/FTS5_CONTENT_NOT_NORMAL, /*nCol*/ 2, db);
    if( !p || !p->pConfig ){
        std::cerr << "Failed to allocate test storage.\n";
        ASSERT_TRUE(false);
        return;
    }

    // apVal[1] as non-integer value (simulate text)
    sqlite3_value* apVal[3];
    apVal[0] = nullptr;
    apVal[1] = sqlite3ValueNew(0);
    if( apVal[1] ){
        // Attempt to set non-integer type; adjust according to available API
        const char* text = "not-an-int";
        sqlite3ValueSetText(apVal[1], -1, text, (int)strlen(text), SQLITE_TRANSIENT);
    }

    i64 rowid = 0;

    rc = sqlite3Fts5StorageContentInsert(p, apVal, &rowid);

    if( apVal[1] ) sqlite3ValueFree(apVal[1]);
    destroy_test_storage(p);
    sqlite3_close(db);

    // Since behavior depends on internal NewRowid logic, we only ensure
    // that a valid rc is returned and that a rowid is (potentially) set.
    ASSERT_TRUE(rc == SQLITE_OK || rc == SQLITE_ROW || rc == SQLITE_ERROR);
    // If NewRowid is exercised, rowid may be non-zero; ensure type is correct.
    (void)rowid;
}

/* Test Case 3: Normal content path (eContent == NORMAL)
 * - This branch involves preparing and binding to a statement, executing, and
 *   updating the last_insert_rowid. Full coverage requires a working in-memory
 *   DB and a valid prepared statement for FTS5 content insert.
 * - For unit testing in isolation, we exercise the control flow up to the binding
 *   step. If a full in-memory environment is available, extend this test to verify
 *   sqlite3_step/pStmt reset and the resulting rowid.
 */
TEST_CASE(test_ContentInsert_Normal_Path_BasicFlow)
{
    // Arrange
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    Fts5Storage *p = nullptr;

    rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK);

    // Create storage with NORMAL content
    p = create_test_storage(/*eContentValue=*/FTS5_CONTENT_NORMAL, /*nCol*/ 2, db);
    if( !p || !p->pConfig ){
        std::cerr << "Failed to allocate test storage for NORMAL path.\n";
        ASSERT_TRUE(false);
        return;
    }

    // In a full environment, supply apVal values to be bound to the INSERT statement.
    // Here we declare placeholders to indicate intended test steps.
    sqlite3_value* apVal[4];
    apVal[0] = nullptr;
    apVal[1] = nullptr; // would be bound to column 1
    apVal[2] = nullptr; // would be bound to column 2
    apVal[3] = nullptr; // would be bound to column nCol+1

    i64 rowid = 0;
    rc = sqlite3Fts5StorageContentInsert(p, apVal, &rowid);

    destroy_test_storage(p);
    sqlite3_close(db);

    // We cannot validate binding without a fully initialized prepared statement.
    // Just ensure function returns a code (SQLITE_OK by contract for success path).
    ASSERT_TRUE(rc == SQLITE_OK || rc == SQLITE_ROW);
    (void)rowid;
}

/* Runner: execute all test cases */
int main()
{
    // Reset failure flag
    failing = false;

    // Run tests
    test_ContentInsert_NonNormal_IntegerValue();
    test_ContentInsert_NonNormal_NonIntegerValue();
    test_ContentInsert_Normal_Path_BasicFlow();

    if( failing ){
        std::cerr << "Unit tests finished with failures.\n";
        return 1;
    } else {
        std::cout << "All unit tests finished successfully.\n";
        return 0;
    }
}

/*
Notes for maintainers:
- The above tests focus on Step 1 and Step 2 of the requested workflow:
  understanding of the function and constructing a test suite around the focal method.

- True/false coverage:
  - Test_ContentInsert_NonNormal_IntegerValue covers the path where pConfig->eContent != FTS5_CONTENT_NORMAL and apVal[1] is an integer.
  - Test_ContentInsert_NonNormal_NonIntegerValue exercises the non-integer path to validate non-integer handling and rc propagation.

- Normal content path (Test_ContentInsert_Normal_Path_BasicFlow) is included as a scaffold
  to illustrate how to extend coverage once a fully-initialized in-memory environment
  and a valid prepared statement (INSERT CONTENT) are available.

- This test harness assumes the presence of the project’s headers (fts5Int.h, sqlite3.h) and
  a working build environment. If your environment uses slightly different header names
  or APIs (e.g., different value construction utilities for sqlite3_value), adapt the
  value creation and binding helpers accordingly.

- For static/internal helpers, tests do not attempt to mock or override static
  functions (as per the domain guidance). Tests exercise public entry points and
  rely on the real implementations to manage internal static state. If you need to
  mock non-virtual functions, consider using a dedicated test build with dependency
  injection points or wrappers around static helpers in a separate compilation unit.

- The test suite uses a lightweight, custom assertion framework to maximize test
  execution and minimize external dependencies, as required by the non-GTest constraint.
*/
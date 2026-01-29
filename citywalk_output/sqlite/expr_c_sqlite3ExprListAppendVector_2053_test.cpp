// High-level C++11 unit test suite for the focal function
// sqlite3ExprListAppendVector located in expr.c
// NOTE: This test suite is designed to be compiled with the SQLite source (C) codebase.
// It does not use GoogleTest; instead, it provides a minimal, in-process test runner.
// The tests focus on the most deterministic, low-risk branches of the function:
// - Handling of NULL pColumns (the "vector_append_error" path triggered by NEVER(pColumns==0))
// - Handling of NULL pExpr (the "vector_append_error" path triggered early)
// The goal is to exercise code paths without requiring full vector-expression construction.
// Explanatory comments accompany each test case.

#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>


// Include SQLite internal headers to access Parse, ExprList, IdList, Expr types and APIs.
// These headers are part of the SQLite source tree and provide the necessary declarations.
// The code under test is C; we invoke it from C++ in a controlled environment.
extern "C" {
}

// Lightweight assertion helpers (non-terminating in case of failure is not mandated here,
// but we provide clear failure messages and exit on assertion failure for determinism).
#define ASSERT_PTR_EQ(a, b) do { if((a) != (b)) { \
  fprintf(stderr, "Assertion failed: %s == %p, expected %p\n", #a, (void*)(a), (void*)(b)); \
  exit(1); } } while(0)

#define ASSERT_NOT_NULL(a) do { if((a) == nullptr) { \
  fprintf(stderr, "Assertion failed: %s is not null\n", #a); \
  exit(1); } } while(0)

#define ASSERT_TRUE(cond) do { if(!(cond)) { \
  fprintf(stderr, "Assertion failed: %s is true\n", #cond); \
  exit(1); } } while(0)

#define TEST(name) static void name()

// Helper to create an in-memory SQLite database connection for the parsing context.
static sqlite3* createInMemoryDb() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open in-memory DB: %d\n", rc);
        return nullptr;
    }
    return db;
}

// Helper to allocate and initialize a Parse object with a valid db pointer.
// This mirrors how the focal function reads pParse->db.
static Parse* createParse(sqlite3* db) {
    if(!db) return nullptr;
    Parse* pParse = (Parse*)calloc(1, sizeof(Parse));
    if(!pParse) return nullptr;
    // The internal Parse struct has a member sqlite3 *db; ensure it is set.
    pParse->db = db;
    return pParse;
}

// Helper to create a minimal scalar Expr for tests that exercise early-error paths.
// We use sqlite3ExprAlloc to create a non-NULL expression. The exact op is not critical
// for the error-path tests because the early checks occur before vector-size processing.
static Expr* createScalarExpr(sqlite3* db) {
    // Use a benign operator (TK_ID is a typical scalar-expression operator) if available.
    // If TK_ID is not available in scope, TK_COLUMN or a generic non-SLECT token may be used.
    // We prefer TK_ID here; should be defined in sqlite3.h.
    int op = TK_ID; // scalar expression
    return sqlite3ExprAlloc(db, op, NULL, 0);
}

// TEST 1: Null pColumns triggers the vector_append_error path.
// We supply a valid Parse and a non-NULL Expr, but pColumns is NULL.
// Expected: The function should return the original pList after attempting cleanup.
TEST(test_null_columns_triggers_error_path) {
    printf("Running test_null_columns_triggers_error_path...\n");

    sqlite3* db = createInMemoryDb();
    ASSERT_NOT_NULL(db);

    Parse* pParse = createParse(db);
    ASSERT_NOT_NULL(pParse);

    // Start with a NULL pList to observe the return value directly.
    ExprList* pList = NULL;

    // pColumns is intentionally NULL to exercise the NEVER(pColumns==0) path.
    IdList* pColumns = NULL;

    // Create a non-NULL RHS expression to reach the early checks.
    Expr* pExpr = createScalarExpr(db);
    ASSERT_NOT_NULL(pExpr);

    // Call the focal function
    ExprList* pResult = sqlite3ExprListAppendVector(pParse, pList, pColumns, pExpr);

    // We expect the function to bail out via the error path and return the original pList (NULL)
    ASSERT_PTR_EQ(pResult, pList);

    // Cleanup: The function should have cleaned up pExpr in the error path.
    // We release the resources we created to avoid leaks.
    sqlite3ExprUnmapAndDelete(pParse, pExpr);
    sqlite3IdListDelete(db, pColumns); // pColumns is NULL; safe to call

    // Finalize database
    sqlite3_close(db);

    printf("test_null_columns_triggers_error_path passed.\n");
}

// TEST 2: Null pExpr triggers the vector_append_error path.
// We supply a valid Parse and a non-NULL pColumns (to avoid the NEVER(...) path),
// but pExpr is NULL. This exercises the early return path on NULL RHS.
TEST(test_null_expr_triggers_error_path) {
    printf("Running test_null_expr_triggers_error_path...\n");

    sqlite3* db = createInMemoryDb();
    ASSERT_NOT_NULL(db);

    Parse* pParse = createParse(db);
    ASSERT_NOT_NULL(pParse);

    ExprList* pList = NULL; // Start with empty list

    // Construct a minimal IdList with 0 entries to avoid additional complexity.
    IdList* pColumns = (IdList*)calloc(1, sizeof(IdList));
    // We do not rely on pColumns contents for this test since pExpr will be NULL.
    pColumns->nId = 0;
    pColumns->a = NULL;

    // pExpr is NULL to trigger the immediate error path.
    Expr* pExpr = NULL;

    ExprList* pResult = sqlite3ExprListAppendVector(pParse, pList, pColumns, pExpr);

    // Expect the function to return the original pList (NULL) after error handling.
    ASSERT_PTR_EQ(pResult, pList);

    // Cleanup
    sqlite3IdListDelete(db, pColumns); // frees pColumns contents if any
    sqlite3ExprUnmapAndDelete(pParse, pExpr); // safe for NULL

    sqlite3_free(pColumns);

    sqlite3_close(db);

    printf("test_null_expr_triggers_error_path passed.\n");
}

// Main: Run all tests
int main(void) {
    printf("Starting sqlite3ExprListAppendVector test suite (minimal, edge-path tests).\n");
    test_null_columns_triggers_error_path();
    test_null_expr_triggers_error_path();
    printf("All tests completed.\n");
    return 0;
}
/*
  Unit test suite for sqlite3ExprIsConstant (Expr *p)

  Purpose:
  - Validate that sqlite3ExprIsConstant correctly identifies constant expressions.
  - Exercise true (constant literals) and false (non-constant) branches.
  - Use internal SQLite interfaces to build AST expressions.
  - Implement a lightweight, non-terminating assertion framework (no GTest).

  Assumptions:
  - This test is intended to be compiled and linked against the SQLite
    source (internal headers included), in a C++11 environment.
  - Internal headers (e.g., sqliteInt.h) provide access to the Expr type and
    internal constructors (sqlite3ExprAlloc, sqlite3Expr, etc.) used to create
    test expressions.
  - The op codes TK_INTEGER, TK_NULL, TK_COLUMN, etc. are available through
    sqliteInt.h / sqlite3.h in the test build.
  - The database handle sqlite3 *db is created via sqlite3_open(":memory:", &db).
  - We avoid terminating the test on assertion failures; instead, failures are
    recorded and reported at the end.

  How to run (high level):
  - Build linking against the SQLite core library with internal headers available.
  - Run the produced executable; it will print a summary of passed/failed tests.
*/

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


extern "C" {
  // Internal headers (assumed to be available in the build environment)
  #include "sqlite3.h"
  #include "sqliteInt.h"
}

/* Lightweight non-terminating assertion helpers.
   They record failures but do not abort execution. */
static int g_test_failed = 0;

#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s is false\n", __FILE__, __LINE__, #cond); \
    g_test_failed++; \
  } \
} while(0)

#define EXPECT_FALSE(cond) do { \
  if(cond) { \
    fprintf(stderr, "EXPECT_FALSE failed at %s:%d: %s is true\n", __FILE__, __LINE__, #cond); \
    g_test_failed++; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if( (a) != (b) ) { \
    fprintf(stderr, "EXPECT_EQ failed at %s:%d: %s == %d, expected %d\n", __FILE__, __LINE__, #a, (int)(a), (int)(b)); \
    g_test_failed++; \
  } \
} while(0)

#define TEST_PASSED() do { \
  /* Intentionally empty: per-test hook for readability. */ \
} while(0)

/* Helper to create an expression easily.
   We use SQLite's internal constructors to create constants and non-constants. */
static Expr* make_expr_int(sqlite3 *db, const char *zVal) {
  // TK_INTEGER literal with value zVal
  return sqlite3Expr(db, TK_INTEGER, zVal);
}

static Expr* make_expr_null(sqlite3 *db) {
  // NULL literal
  return sqlite3Expr(db, TK_NULL, nullptr);
}

static Expr* make_expr_column(sqlite3 *db, const char *zCol) {
  // Column reference (non-constant)
  return sqlite3Expr(db, TK_COLUMN, zCol);
}

/* Test 1: Constant integer literal should be identified as constant. */
static void test_expr_is_constant_literal_integer(sqlite3 *db) {
  // Build an integer literal expression: 42
  Expr *p = make_expr_int(db, "42");
  int res = sqlite3ExprIsConstant(p);
  EXPECT_EQ(res, 1);
  sqlite3ExprDelete(db, p);
  TEST_PASSED();
}

/* Test 2: NULL literal should be identified as constant. */
static void test_expr_is_constant_null_literal(sqlite3 *db) {
  // Build a NULL literal expression
  Expr *p = make_expr_null(db);
  int res = sqlite3ExprIsConstant(p);
  EXPECT_EQ(res, 1);
  sqlite3ExprDelete(db, p);
  TEST_PASSED();
}

/* Test 3: Column reference should NOT be identified as constant. */
static void test_expr_is_constant_column(sqlite3 *db) {
  // Build a column reference expression (non-constant)
  Expr *p = make_expr_column(db, "mytable.mycolumn");
  int res = sqlite3ExprIsConstant(p);
  EXPECT_EQ(res, 0);
  sqlite3ExprDelete(db, p);
  TEST_PASSED();
}

/* Entry point: set up in-memory DB, run tests, report summary. */
int main(void) {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if(rc != SQLITE_OK || db == nullptr) {
    fprintf(stderr, "Failed to open in-memory database. rc=%d\n", rc);
    return EXIT_FAILURE;
  }

  // Run tests
  test_expr_is_constant_literal_integer(db);
  test_expr_is_constant_null_literal(db);
  test_expr_is_constant_column(db);

  sqlite3_close(db);

  if(g_test_failed == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("TESTS FAILED: %d failure(s)\n", g_test_failed);
  }

  // Return non-zero if any test failed
  return (g_test_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
// Specialized C++11 unit tests for the focal method:
// sqlite3ExprDeferredDelete in expr.c
// This test bypasses the full SQLite runtime by providing minimal
// stand-ins for necessary C interfaces and by intercepting the
// side-effect (cleanup registration) via a test double for sqlite3ParserAddCleanup.
// The tests are designed to be non-terminating (do not call exit) and report
// results through console output.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// Forward declare the C types used by the focal function.
// We keep them opaque to avoid pulling in the full SQLite type system.
struct Parse;
struct Expr;

// Provide a minimal sqlite3 type alias to satisfy the function pointer type.
// We only need the type to exist for pointer alignment in the tests.
struct sqlite3;
typedef struct sqlite3 sqlite3;

// Declarations of the production-facing C interfaces used by sqlite3ExprDeferredDelete.
// They are declared with C linkage to match the expected linkage in the production code.
extern "C" {
  // The focal function under test (declared forCTest usage)
  void sqlite3ExprDeferredDelete(Parse *pParse, Expr *pExpr);

  // Interfaces we intercept/mimic in tests
  void sqlite3ParserAddCleanup(Parse *pParse, void(*)(sqlite3*,void*), Expr *pExpr);
  void sqlite3ExprDelete(sqlite3 *db, Expr *pExpr);
  // Expose signature for calling the focal function
  // (Note: the actual implementation exists in expr.c)
  void sqlite3ExprDeferredDelete(Parse *pParse, Expr *pExpr);
}

// --- Test harness state to capture calls to sqlite3ParserAddCleanup ---

// Captured parameters from the last call to sqlite3ParserAddCleanup
static Parse *g_last_pParse = nullptr;
static Expr  *g_last_pExpr  = nullptr;
static void  (*g_last_cleanup)(sqlite3*, void*) = nullptr;

// Test double for sqlite3ParserAddCleanup to observe arguments passed by sqlite3ExprDeferredDelete
extern "C" void sqlite3ParserAddCleanup(Parse *pParse, void(*cleanup)(sqlite3*,void*), Expr *pExpr)
{
  g_last_pParse  = pParse;
  g_last_pExpr   = pExpr;
  g_last_cleanup = cleanup;
}

// Forward declare minimal object builders to allocate opaque test objects
static Parse* makeTestParse() {
  // Allocate a tiny chunk of memory to back an opaque Parse* pointer.
  return reinterpret_cast<Parse*>(std::malloc(1));
}
static Expr*  makeTestExpr()  {
  // Allocate a tiny chunk of memory to back an opaque Expr* pointer.
  return reinterpret_cast<Expr*>(std::malloc(1));
}
static void freeTestParse(Parse* p) { if(p) std::free(p); }
static void freeTestExpr(Expr* e)   { if(e) std::free(e); }

// Simple non-terminating test assertion macro
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define CHECK(cond, msg) do { \
  ++g_total_tests;                    \
  if(!(cond)) {                         \
    ++g_failed_tests;                   \
    std::cerr << "TEST FAILED: " << (msg)                 \
              << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
  } else {                              \
    std::cout << "TEST PASSED: " << (msg) << std::endl; \
  }                                     \
} while(0)

// Test 1: Validate that sqlite3ExprDeferredDelete forwards parameters correctly
// to sqlite3ParserAddCleanup and uses sqlite3ExprDelete as the cleanup function pointer.
static void test_sqlite3ExprDeferredDelete_forwards_arguments_once() {
  // Prepare two opaque test objects
  Parse *p1 = makeTestParse();
  Expr  *e1 = makeTestExpr();

  // Reset capture state
  g_last_pParse  = nullptr;
  g_last_pExpr   = nullptr;
  g_last_cleanup = nullptr;

  // Call the focal function
  sqlite3ExprDeferredDelete(p1, e1);

  // Verify that the parser cleanup registration captured the intended values
  CHECK(g_last_pParse == p1, "First parameter (Parse*) forwarded to sqlite3ParserAddCleanup");
  CHECK(g_last_pExpr == e1, "Second parameter (Expr*) forwarded to sqlite3ParserAddCleanup");
  CHECK(g_last_cleanup == reinterpret_cast<void(*)(sqlite3*,void*)>(sqlite3ExprDelete),
        "Cleanup function pointer equals sqlite3ExprDelete");

  // Cleanup allocated test objects
  freeTestParse(p1);
  freeTestExpr(e1);
}

// Test 2: Ensure independent subsequent calls are captured correctly (state resets between calls)
static void test_sqlite3ExprDeferredDelete_multiple_calls() {
  // First call
  Parse *pA = makeTestParse();
  Expr  *eA = makeTestExpr();
  sqlite3ExprDeferredDelete(pA, eA);
  CHECK(g_last_pParse == pA, "Second test: first call - Parse* matches pA");
  CHECK(g_last_pExpr  == eA, "Second test: first call - Expr* matches eA");
  CHECK(g_last_cleanup == reinterpret_cast<void(*)(sqlite3*,void*)>(sqlite3ExprDelete),
        "Second test: first call - cleanup pointer matches sqlite3ExprDelete");

  // Cleanup first
  freeTestParse(pA);
  freeTestExpr(eA);

  // Second call with new objects
  Parse *pB = makeTestParse();
  Expr  *eB = makeTestExpr();
  sqlite3ExprDeferredDelete(pB, eB);
  CHECK(g_last_pParse == pB, "Second test: second call - Parse* matches pB");
  CHECK(g_last_pExpr  == eB, "Second test: second call - Expr* matches eB");
  CHECK(g_last_cleanup == reinterpret_cast<void(*)(sqlite3*,void*)>(sqlite3ExprDelete),
        "Second test: second call - cleanup pointer matches sqlite3ExprDelete");

  // Cleanup second
  freeTestParse(pB);
  freeTestExpr(eB);
}

// Test 3: Verify that passing null pointers is handled (pParse, pExpr)
static void test_sqlite3ExprDeferredDelete_null_pointers() {
  // Reset capture state
  g_last_pParse  = nullptr;
  g_last_pExpr   = nullptr;
  g_last_cleanup = nullptr;

  // Call with null pointers (allowed by C pointer semantics in this mock)
  sqlite3ExprDeferredDelete(nullptr, nullptr);

  // Ensure the call still forwards to sqlite3ParserAddCleanup with nulls and proper cleanup
  CHECK(g_last_pParse == nullptr, "Null Parse* is forwarded as nullptr");
  CHECK(g_last_pExpr  == nullptr, "Null Expr* is forwarded as nullptr");
  CHECK(g_last_cleanup == reinterpret_cast<void(*)(sqlite3*,void*)>(sqlite3ExprDelete),
        "Null case still uses sqlite3ExprDelete as cleanup");
}

// Helper to report final summary
static void report_summary() {
  std::cout << "Summary: " << g_total_tests << " tests, "
            << g_failed_tests << " failures." << std::endl;
}

// Main entry point to run unit tests
int main() {
  // Run tests
  test_sqlite3ExprDeferredDelete_forwards_arguments_once();
  test_sqlite3ExprDeferredDelete_multiple_calls();
  test_sqlite3ExprDeferredDelete_null_pointers();

  // Final report
  report_summary();

  // Exit code: 0 if tests pass, non-zero otherwise
  return (g_failed_tests == 0) ? 0 : 1;
}
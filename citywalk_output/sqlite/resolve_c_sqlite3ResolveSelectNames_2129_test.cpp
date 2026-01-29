/*
  Unit test suite for the focal method:
  sqlite3ResolveSelectNames
  - Implemented in a single translation unit to enable isolated testing
  - No external test frameworks (GTest) used
  - Uses a lightweight harness with non-terminating assertions (failCount)
  - Contains two test cases:
    1) Basic wiring: verifies the Walker is populated correctly and sqlite3WalkSelect is invoked
    2) Null p (pParse context): verifies that an assertion is triggered when p is NULL

  Notes:
  - This test harness provides minimal stand-ins for the sqlite3 internal types
    (Parse, Select, NameContext, Walker, Expr) and the helper function sqlite3WalkSelect
    to exercise the focal method's wiring behavior without depending on the full
    SQLite codebase.
  - The implementation duplicates the relevant behavior of sqlite3ResolveSelectNames
    for testing purposes in this isolated environment.
*/

#include <setjmp.h>
#include <signal.h>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Lightweight stand-ins for SQLite internal types (minimal, test-focused)
typedef struct Parse Parse;
typedef struct NameContext NameContext;
typedef struct Expr Expr;
typedef struct Select Select;

// Forward declaration of callback types used in Walker
typedef int (*WalkerExprCallback)(struct Walker*, Expr*);
typedef int (*WalkerSelectCallback)(struct Walker*, Select*);

// Minimal Walker structure used by the focal function
typedef struct Walker {
  WalkerExprCallback xExprCallback;
  WalkerSelectCallback xSelectCallback;
  WalkerSelectCallback xSelectCallback2;
  Parse *pParse;
  struct {
    void *pNC;
  } u;
} Walker;

// Globals to capture state inside sqlite3WalkSelect (mock)
static Walker *g_lastWalker = nullptr;
static Select *g_lastSelect = nullptr;

// Forward-declare the static resolution step functions used by the focal method
static int resolveExprStep(Walker *pWalker, Expr *pExpr);
static int resolveSelectStep(Walker *pWalker, Select *p);

// Mock implementation of sqlite3WalkSelect used by the focal method
extern "C" int sqlite3WalkSelect(Walker *pWalker, Select *p) {
  // Capture the walker and select for verification in tests
  g_lastWalker = pWalker;
  g_lastSelect = p;
  // No further walking is performed in this simplified test harness
  return 0;
}

// Minimal declarations for the focal method's dependencies (test harness)
struct Parse { int dummy; };
struct NameContext { int dummy; };

// Static resolution step functions (defined below) used by the focal method
static int resolveExprStep(Walker *pWalker, Expr *pExpr) { (void)pWalker; (void)pExpr; return 0; }
static int resolveSelectStep(Walker *pWalker, Select *p) { (void)pWalker; (void)p; return 0; }

// The focal method under test (reproduced here for this isolated test)
extern "C" void sqlite3ResolveSelectNames(
  Parse *pParse,         /* The parser context */
  Select *p,             /* The SELECT statement being coded. */
  NameContext *pOuterNC  /* Name context for parent SELECT statement */
){
  Walker w;
  assert( p!=0 );
  w.xExprCallback = resolveExprStep;
  w.xSelectCallback = resolveSelectStep;
  w.xSelectCallback2 = 0;
  w.pParse = pParse;
  w.u.pNC = pOuterNC;
  sqlite3WalkSelect(&w, p);
}

// Simple non-terminating assertion helper
static int failCount = 0;
#define CHECK(cond, msg) do { if(!(cond)) { \
  std::cerr << "TEST FAIL: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
  ++failCount; \
}} while(0)

// Test 1: Basic wiring - verify Walker is populated correctly and sqlite3WalkSelect is invoked
static void test_basic_wiring() {
  // Arrange
  Parse pParseInstance;
  NameContext ncInstance;
  Select selectInstance;

  // Act
  // Reset globals before test
  g_lastWalker = nullptr;
  g_lastSelect = nullptr;
  sqlite3ResolveSelectNames(&pParseInstance, &selectInstance, &ncInstance);

  // Assert
  CHECK(g_lastWalker != nullptr, "sqlite3WalkSelect should have been called and walker captured");
  if (g_lastWalker != nullptr) {
    CHECK(g_lastWalker->xExprCallback == &resolveExprStep,
          "Walker.xExprCallback should point to resolveExprStep");
    CHECK(g_lastWalker->xSelectCallback == &resolveSelectStep,
          "Walker.xSelectCallback should point to resolveSelectStep");
    CHECK(g_lastWalker->xSelectCallback2 == 0,
          "Walker.xSelectCallback2 should be NULL (0)");
    CHECK(g_lastWalker->pParse == &pParseInstance,
          "Walker.pParse should be forwarded from input Parse");
    CHECK(g_lastWalker->u.pNC == &ncInstance,
          "Walker.u.pNC should be forwarded from input NameContext");
  }
}

// Test 2: Null p (Parse*) should trigger an assertion
static sigjmp_buf jumpBuf;
static void handleAbort(int) { siglongjmp(jumpBuf, 1); }

static void test_null_p_assertion() {
  // Arrange
  Parse dummyParse;
  NameContext ncInstance;

  // Install signal handler to catch assertion aborts (SIGABRT)
  struct sigaction oldAct, newAct;
  newAct.sa_handler = handleAbort;
  sigemptyset(&newAct.sa_mask);
  newAct.sa_flags = 0;
  sigaction(SIGABRT, &newAct, &oldAct);

  // Act & Assert: Expect an assertion abort when p is NULL
  if (sigsetjmp(jumpBuf, 1) == 0) {
    // This call should trigger an assert on p != 0
    sqlite3ResolveSelectNames(&dummyParse, nullptr, &ncInstance);
    CHECK(false, "Expected assertion abort when p (Parse*) is NULL");
  } else {
    // Aborted as expected
  }

  // Restore previous signal handler
  sigaction(SIGABRT, &oldAct, nullptr);
}

// Main entry point to run tests
int main() {
  // Run test cases
  test_basic_wiring();
  test_null_p_assertion();

  // Report results
  if (failCount == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cout << "Total failures: " << failCount << "\n";
    return 1;
  }
}
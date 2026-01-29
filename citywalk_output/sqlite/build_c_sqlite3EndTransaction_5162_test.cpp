// Minimal C++11 test harness for the focal function sqlite3EndTransaction
// This harness provides lightweight stubs for the SQLite environment and
// exercises true/false branches of the dependent predicates.
// No external testing framework is used (GTest is avoided). All tests run
// from main() and use non-terminating EXPECT-like assertions to maximize
// code coverage.

#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain-specific constants (mirroring the SQLite usage in the focal method)
#define TK_COMMIT 1
#define TK_END 2
#define TK_ROLLBACK 3

#define SQLITE_TRANSACTION 0
#define OP_AutoCommit 42  // Arbitrary value chosen for testing

// Lightweight test scaffolding
static int g_failures_in_current_test = 0;
static int g_total_failures = 0;

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        printf("  [FAIL] %s: expected %d, got %d\n", (msg), (int)(b), (int)(a)); \
        g_failures_in_current_test++; \
    } \
} while(0)

#define PRINT_TEST_RESULT(name) do { \
    if (g_failures_in_current_test == 0) { \
        printf("  [PASS] %s\n", (name)); \
    } else { \
        printf("  [FAIL] %s: %d failure(s)\n", (name), g_failures_in_current_test); \
        g_total_failures += g_failures_in_current_test; \
    } \
    g_failures_in_current_test = 0; \
} while(0)

// Forward-declare the minimal types used by sqlite3EndTransaction
typedef struct Parse Parse;
typedef struct Vdbe Vdbe;

// Global test-time mocks/state
static int g_authReturnValue = 0;
static Vdbe* g_vdbeInstanceForTest = nullptr;
static Vdbe g_vdbeInstanceStorage;     // storage for a dummy Vdbe
static int g_getVdbeCalled = 0;
static int g_addOpCalled = 0;
static int g_lastOp = 0;
static int g_lastP1 = 0;
static int g_lastP2 = 0;

// Lightweight Parse structure with minimal fields used by the focal method
struct Parse {
    void* db;  // Non-null pointer to simulate a valid database connection
};

// Lightweight Vdbe structure (contents are irrelevant for test other than pointer identity)
struct Vdbe {
    int dummy;
};

// Hooked, test-time implementations to replace real SQLite calls
extern "C" {

// Authorization check stub: returns controlled value to drive true/false branches
int sqlite3AuthCheck(Parse* pParse, int iDb, const char* zAuth, int iClient, int iDb2) {
    (void)pParse; (void)iDb; (void)iClient; (void)iDb2;
    return g_authReturnValue; // 0 means authorized, non-zero means denied
}

// Return a Vdbe object or nullptr depending on test setup
Vdbe* sqlite3GetVdbe(Parse* p) {
    (void)p;
    g_getVdbeCalled = 1;
    return g_vdbeInstanceForTest; // might be nullptr to simulate missing Vdbe
}

// Record the addition of an OP with two operands on the Vdbe
void sqlite3VdbeAddOp2(Vdbe* v, int op, int p1, int p2) {
    (void)v;
    g_addOpCalled = 1;
    g_lastOp = op;
    g_lastP1 = p1;
    g_lastP2 = p2;
}

// The focal function under test copied verbatim to allow isolated unit testing
void sqlite3EndTransaction(Parse *pParse, int eType){
  Vdbe *v;
  int isRollback;
  assert( pParse!=0 );
  assert( pParse->db!=0 );
  assert( eType==TK_COMMIT || eType==TK_END || eType==TK_ROLLBACK );
  isRollback = eType==TK_ROLLBACK;
  if( sqlite3AuthCheck(pParse, SQLITE_TRANSACTION,
       isRollback ? "ROLLBACK" : "COMMIT", 0, 0) ){
    return;
  }
  v = sqlite3GetVdbe(pParse);
  if( v ){
    sqlite3VdbeAddOp2(v, OP_AutoCommit, 1, isRollback);
  }
}
} // extern "C"

// Test helpers
static void reset_globals_for_test() {
    g_authReturnValue = 0;
    g_vdbeInstanceForTest = nullptr;
    g_getVdbeCalled = 0;
    g_addOpCalled = 0;
    g_lastOp = -1;
    g_lastP1 = -1;
    g_lastP2 = -1;
}

// Test 1: Authorization denied should not call GetVdbe or AddOp2
static void test_EndTransaction_AuthorizationDenied() {
    reset_globals_for_test();
    g_authReturnValue = 1; // Deny authorization

    Parse p;
    int dummyDb;
    p.db = &dummyDb; // non-null

    sqlite3EndTransaction(&p, TK_COMMIT);

    EXPECT_EQ(g_getVdbeCalled, 0, "GetVdbe should not be called when auth denies");
    EXPECT_EQ(g_addOpCalled, 0, "AddOp2 should not be called when auth denies");

    PRINT_TEST_RESULT("EndTransaction - Authorization Denied");
}

// Test 2: Authorization granted and Vdbe present, Commit -> should add AutoCommit with p2 = 0
static void test_EndTransaction_Authorized_Commit_VdbePresent() {
    reset_globals_for_test();
    g_authReturnValue = 0; // Allow

    // Prepare Vdbe to be returned
    g_vdbeInstanceForTest = &g_vdbeInstanceStorage;

    Parse p;
    int dummyDb;
    p.db = &dummyDb; // non-null

    sqlite3EndTransaction(&p, TK_COMMIT);

    EXPECT_EQ(g_getVdbeCalled, 1, "GetVdbe should be called when auth passes");
    EXPECT_EQ(g_addOpCalled, 1, "AddOp2 should be called when Vdbe is present");
    EXPECT_EQ(g_lastOp, OP_AutoCommit, "Expected OP_AutoCommit as the operation");
    EXPECT_EQ(g_lastP1, 1, "Expected p1 == 1 (AutoCommit flag)");
    EXPECT_EQ(g_lastP2, 0, "Expected p2 == 0 for non-rollback (commit)");

    PRINT_TEST_RESULT("EndTransaction - Authorized Commit with Vdbe present");
}

// Test 3: Authorization granted and Vdbe present, Rollback -> should add AutoCommit with p2 = 1
static void test_EndTransaction_Authorized_Rollback_VdbePresent() {
    reset_globals_for_test();
    g_authReturnValue = 0; // Allow
    g_vdbeInstanceForTest = &g_vdbeInstanceStorage;

    Parse p;
    int dummyDb;
    p.db = &dummyDb;

    sqlite3EndTransaction(&p, TK_ROLLBACK);

    EXPECT_EQ(g_getVdbeCalled, 1, "GetVdbe should be called for rollback case");
    EXPECT_EQ(g_addOpCalled, 1, "AddOp2 should be called for rollback case");
    EXPECT_EQ(g_lastOp, OP_AutoCommit, "Expected OP_AutoCommit as the operation");
    EXPECT_EQ(g_lastP1, 1, "Expected p1 == 1 (AutoCommit flag)");
    EXPECT_EQ(g_lastP2, 1, "Expected p2 == 1 for rollback");

    PRINT_TEST_RESULT("EndTransaction - Authorized Rollback with Vdbe present");
}

// Test 4: Authorized path but Vdbe is null -> should not call AddOp2
static void test_EndTransaction_Authorized_VdbeNull() {
    reset_globals_for_test();
    g_authReturnValue = 0; // Allow
    g_vdbeInstanceForTest = nullptr; // No Vdbe

    Parse p;
    int dummyDb;
    p.db = &dummyDb;

    sqlite3EndTransaction(&p, TK_COMMIT);

    EXPECT_EQ(g_getVdbeCalled, 1, "GetVdbe should be called even if Vdbe is null");
    EXPECT_EQ(g_addOpCalled, 0, "AddOp2 should not be called when Vdbe is null");

    PRINT_TEST_RESULT("EndTransaction - Authorized Commit with NULL Vdbe");
}

// Main: run all tests and report summary
int main() {
    printf("Starting unit tests for sqlite3EndTransaction (C++11 test harness)\n");

    test_EndTransaction_AuthorizationDenied();
    test_EndTransaction_Authorized_Commit_VdbePresent();
    test_EndTransaction_Authorized_Rollback_VdbePresent();
    test_EndTransaction_Authorized_VdbeNull();

    if (g_total_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_total_failures);
        return 1;
    }
}
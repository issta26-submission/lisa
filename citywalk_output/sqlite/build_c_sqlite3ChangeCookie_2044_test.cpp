// This test suite provides unit tests for the focal method:
//   void sqlite3ChangeCookie(Parse *pParse, int iDb)
// The tests are handcrafted in C++11 without Google Test (GTest).
// They define minimal stubs for the dependent SQLite structures and
// functions so that sqlite3ChangeCookie can be exercised in isolation.
//
// Step 1 (Program Understanding) and Step 2/3 (Test Generation & Refinement)
// are reflected by the inline comments and structure of the test cases.
// Candidate Keywords considered for test design include: Parse, sqlite3ChangeCookie,
// sqlite3SchemaMutexHeld, sqlite3VdbeAddOp3, Vdbe, OpCode (OP_SetCookie),
// BTREE_SCHEMA_VERSION, schema_cookie, sqlite3, pParse, iDb, pVdbe, db.
//
// Notes:
// - We test the success path where sqlite3SchemaMutexHeld returns true (as required by
//   the function via an assert), ensuring that a single OP_SetCookie is added with the
//   expected parameters.
// - The false-path of the assert (when sqlite3SchemaMutexHeld would return false) is
//   not exercised here because it would terminate the program via assert; the test
//   environment uses a deterministic true-returning stub to maximize code execution
//   and coverage for the non-assert portion.
// - The tests use simple non-terminating EXPECT-like macros to collect failures
//   without aborting execution, as GTest is not allowed per the task.
// - Static file-scope helpers in the production code are not directly tested here since
//   they are not visible outside their compilation unit. We reproduce the needed parts
//   locally for testability in this isolated harness.

#include <vector>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge hints:
// - Use of only standard library and provided methods
// - Static helpers therein are not accessible unless re-declared here for tests

// ----------------------------------------------------------------------
// Minimal test harness utilities (non-terminating assertions)
// ----------------------------------------------------------------------

static int g_test_failures = 0;

#define EXPECT_EQ(a, b) do { \
    auto _aa = (a); \
    auto _bb = (b); \
    if (!(_aa == _bb)) { \
        ++g_test_failures; \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - " << #a << " (=" << _aa << ") != " << #b \
                  << " (=" << _bb << ")\n"; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        ++g_test_failures; \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << " - condition " << #cond << " is false\n"; \
    } \
} while (0)

#define TEST_PASSED() do { \
    /* No-op: placeholder for per-test hooks if needed later */ \
} while (0)

// ----------------------------------------------------------------------
// Focal method and dependencies (minimal stubs required for testing)
// ----------------------------------------------------------------------

// Production constants (used by the focal method)
static const int OP_SetCookie = 99;          // chosen for testability
static const int BTREE_SCHEMA_VERSION = 7;   // chosen for testability

// Forward declarations of minimal types to mirror production structure
struct Vdbe;
struct sqlite3;

// Structure to mimic the cookie-bearing schema in the database
struct Schema {
    int schema_cookie;
};

// Database entry containing a pointer to a schema
struct DbEntry {
    Schema *pSchema;
};

// Minimal sqlite3-like database structure for test
struct sqlite3 {
    DbEntry *aDb; // array of database entries
};

// Vdbe operation descriptor
struct Op {
    int opcode;
    int p1;
    int p2;
    int p3;
};

// Vdbe: simplified exception-free VDBE-like object for tests
struct Vdbe {
    std::vector<Op> aOp;

    void addOp(int opcode, int p1, int p2, int p3) {
        aOp.push_back({opcode, p1, p2, p3});
    }
};

// Parse structure containing references to the database and a Vdbe
struct Parse {
    sqlite3 *db;
    Vdbe *pVdbe;
};

// Production-like function declarations used by the focal function
static int sqlite3SchemaMutexHeld(sqlite3 *db, int iDb, int dummy) {
    // In tests we guarantee the mutex is held to drive the successful path
    (void)dummy;
    (void)db;
    return 1;
}
static void sqlite3VdbeAddOp3(Vdbe *v, int op, int p1, int p2, int p3) {
    v->addOp(op, p1, p2, p3);
}

// The focal method under test, copied and adapted for the isolated test environment
void sqlite3ChangeCookie(Parse *pParse, int iDb){
  sqlite3 *db = pParse->db;
  Vdbe *v = pParse->pVdbe;
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_SCHEMA_VERSION,
                   (int)(1+(unsigned)db->aDb[iDb].pSchema->schema_cookie));
}

// ----------------------------------------------------------------------
// Test helpers to wire up a minimal environment for sqlite3ChangeCookie
// ----------------------------------------------------------------------

// Helper to build a minimal environment with a given cookie value
static void setupEnvironment(int cookieValue,
                             Parse &parseOut,
                             sqlite3 *&dbOut,
                             Vdbe *&vOut) {
    // Static storage so addresses remain valid for the test duration
    static Schema schemaObj;
    static DbEntry dbEntry;
    static sqlite3 dbObj;
    static Vdbe vObj;

    schemaObj.schema_cookie = cookieValue;
    dbEntry.pSchema = &schemaObj;
    dbObj.aDb = &dbEntry;

    dbOut = &dbObj;
    vOut = &vObj;
    parseOut.db = dbOut;
    parseOut.pVdbe = vOut;
}

// ----------------------------------------------------------------------
// Unit Tests for sqlite3ChangeCookie
// ----------------------------------------------------------------------

// Test 1: cookie value = 0 results in p3 == 1
static void test_changeCookie_basic_zero() {
    // Domain knowledge notes:
    // - True branch of the dependency check is exercised (mutex held)
    // - Verifies that a single SetCookie operation is added with expected fields
    // - Uses a zero cookie scenario to ensure (1+(unsigned)0) == 1
    Parse parse;
    sqlite3 *db;
    Vdbe *v;
    setupEnvironment(0, parse, db, v);

    // Call focal method
    sqlite3ChangeCookie(&parse, 0);

    // Validate one operation was pushed with expected values
    EXPECT_TRUE(!v->aOp.empty());
    EXPECT_EQ(v->aOp.size(), 1u);
    EXPECT_EQ(v->aOp[0].opcode, OP_SetCookie);
    EXPECT_EQ(v->aOp[0].p1, 0);                   // iDb
    EXPECT_EQ(v->aOp[0].p2, BTREE_SCHEMA_VERSION); // BTREE_SCHEMA_VERSION
    EXPECT_EQ(v->aOp[0].p3, 1);                   // 1 + (unsigned)cookie (0)
    TEST_PASSED();
    // Explanatory comment:
    // This test ensures the core contract: a single SetCookie op with correct
    // p1, p2, p3 derived from the cookie value is produced.
}

// Test 2: cookie value = 12345 results in p3 == 12346
static void test_changeCookie_basic_large_cookie() {
    // Domain knowledge notes:
    // - Verifies calculation of (int)(1+(unsigned)cookie) for non-zero cookie
    // - Ensures a second, different cookie value yields a different p3
    Parse parse;
    sqlite3 *db;
    Vdbe *v;
    setupEnvironment(12345, parse, db, v);

    // Call focal method
    sqlite3ChangeCookie(&parse, 0);

    // Validate one operation was pushed with expected values
    EXPECT_TRUE(!v->aOp.empty());
    EXPECT_EQ(v->aOp.size(), 1u);
    EXPECT_EQ(v->aOp[0].opcode, OP_SetCookie);
    EXPECT_EQ(v->aOp[0].p1, 0);
    EXPECT_EQ(v->aOp[0].p2, BTREE_SCHEMA_VERSION);
    EXPECT_EQ(v->aOp[0].p3, 12346); // 1 + 12345
    TEST_PASSED();
    // Explanatory comment:
    // Confirms that the function correctly handles non-zero cookie values and
    // computes the 3rd parameter accordingly.
}

// Step 3: Test runner
int main() {
    // Run tests
    test_changeCookie_basic_zero();
    test_changeCookie_basic_large_cookie();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}
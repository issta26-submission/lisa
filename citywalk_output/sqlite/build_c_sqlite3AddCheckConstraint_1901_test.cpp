// Comprehensive unit tests for sqlite3AddCheckConstraint (C-like API adapted for C++11 test harness)
// Note: This test harness intentionally mocks a subset of SQLite internals to exercise
// the control-flow of sqlite3AddCheckConstraint without requiring the full SQLite runtime.
// The tests are non-GTest, using a small, self-contained test runner with lightweight assertions.

#include <functional>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <iostream>


// Domain-specific: provide minimal stand-ins for required SQLite-like types and APIs.

// Lightweight Token type (mirroring SQLite's Token usage in this context)
struct Token {
    const char* z;
    int n;
};

// Forward declarations for test scaffolding
struct Expr;
struct ExprList;
struct Table;
struct Parse;
struct sqlite3;
struct Btree;
struct Db;

// Global test-tracking state
static int g_exprDeletedCount = 0;
static int g_exprAppendedCount = 0;
static std::string g_lastAssignedName; // captures the most recent name assigned via sqlite3ExprListSetName

static void resetGlobals() {
    g_exprDeletedCount = 0;
    g_exprAppendedCount = 0;
    g_lastAssignedName.clear();
}

// Minimal expression type
struct Expr {};

// Simple expression list to accumulate constraint expressions
struct ExprList {
    std::vector<Expr*> a;
    Token name;     // last assigned name (mirrors internal state)
    bool hasName;     // flag indicating if a name has been assigned

    ExprList() : name{nullptr,0}, hasName(false) {}
};

// Table object holding a constraint expression list
struct Table {
    ExprList* pCheck;
    Table() : pCheck(nullptr) {}
};

// B-tree mock with a readonly flag
struct Btree {
    bool isReadonly;
    Btree(bool ro = false) : isReadonly(ro) {}
};

// Db slot holding a pointer to a B-tree
struct Db {
    Btree* pBt;
    Db() : pBt(nullptr) {}
};

// Simple database hook info (initial database index)
struct Init {
    int iDb;
    Init() : iDb(0) {}
};

// SQLite-like database object (simplified)
struct sqlite3 {
    Db* aDb;
    Init init;
    sqlite3() : aDb(nullptr) {}
};

// Parse context used by the focal method
struct Parse {
    Table* pNewTable;
    sqlite3* db;
    Token constraintName; // represents pParse->constraintName
    Parse() : pNewTable(nullptr), db(nullptr) { constraintName = {nullptr, 0}; }
};

// Helpers to emulate SQLite API behavior used by sqlite3AddCheckConstraint

// Simulate whitespace check (classic sqlite3Isspace)
static int sqlite3Isspace(char c) {
    return std::isspace(static_cast<unsigned char>(c)) ? 1 : 0;
}

// Simulate appending an expression to an ExprList
static ExprList* sqlite3ExprListAppend(Parse* /*pParse*/, ExprList* pList, Expr* pExpr) {
    if (pList == nullptr) {
        pList = new ExprList();
    }
    pList->a.push_back(pExpr);
    ++g_exprAppendedCount;
    return pList;
}

// Simulate setting a name on an ExprList
static void sqlite3ExprListSetName(Parse* /*pParse*/, ExprList* pList, Token* pName, int /*iWhere*/) {
    if (!pList || !pName) return;
    // Capture the name string for test validation (copy semantics)
    g_lastAssignedName.assign(pName->z, static_cast<size_t>(pName->n));
    pList->name = *pName; // keep local copy in the list as well (not strictly needed for tests)
    pList->hasName = true;
}

// Simulate deleting an expression (track deletion)
static void sqlite3ExprDelete(sqlite3* /*db*/, Expr* /*pExpr*/) {
    ++g_exprDeletedCount;
    // Real code would free; here we only simulate deletion count
    delete nullptr; // safe no-op, helps static analyzers
}

// Simulate checking if a B-tree is readonly
static int sqlite3BtreeIsReadonly(Btree* pBt) {
    return pBt ? (pBt->isReadonly ? 1 : 0) : 0;
}

// The focal method under test (re-implemented for the test harness)
void sqlite3AddCheckConstraint(
  Parse *pParse,      /* Parsing context */
  Expr *pCheckExpr,   /* The check expression */
  const char *zStart, /* Opening "(" */
  const char *zEnd    /* Closing ")" */
){
#ifndef SQLITE_OMIT_CHECK
  Table *pTab = pParse->pNewTable;
  sqlite3 *db = pParse->db;
  if( pTab && /* !IN_DECLARE_VTAB */ 0
   && !sqlite3BtreeIsReadonly(db->aDb[db->init.iDb].pBt)
  ){
    pTab->pCheck = sqlite3ExprListAppend(pParse, pTab->pCheck, pCheckExpr);
    if( pParse->constraintName.n ){
      sqlite3ExprListSetName(pParse, pTab->pCheck, &pParse->constraintName, 1);
    }else{
      Token t;
      for(zStart++; sqlite3Isspace(zStart[0]); zStart++){}
      while( sqlite3Isspace(zEnd[-1]) ){ zEnd--; }
      t.z = zStart;
      t.n = (int)(zEnd - t.z);
      sqlite3ExprListSetName(pParse, pTab->pCheck, &t, 1);
    }
  }else
#endif
  {
    sqlite3ExprDelete(pParse->db, pCheckExpr);
  }
}

// Static (file-scope) helpers and mock dependencies are defined above.
// End of focal method implementation.

// ---------------------- Test Harness ----------------------

// Simple test runner
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << " - Condition failed: " #cond << "\n"; \
        ++g_failedTests; \
    } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << " - Expected " << (b) << " but got " << (a) << "\n"; \
        ++g_failedTests; \
    } \
} while(0)


// Helpers to create test DB/Tables

static sqlite3* createTestDb(bool readonly = false) {
    sqlite3* db = new sqlite3();
    db->aDb = new Db[1];
    db->init.iDb = 0;
    db->aDb[0].pBt = new Btree(readonly);
    return db;
}

// Helper to clean up test DB
static void destroyTestDb(sqlite3* db) {
    if (!db) return;
    if (db->aDb) {
        delete db->aDb[0].pBt;
        delete[] db->aDb;
    }
    delete db;
}

// Test 1A: pNewTable non-null, not readonly, constraintName.n > 0
static void test_AddCheckConstraint_ConstraintNameProvided() {
    resetGlobals();
    Parse p;
    sqlite3* db = createTestDb(false);
    p.db = db;
    p.pNewTable = new Table(); // non-null table
    // Constraint name is provided
    const char* cname = "NAME123";
    p.constraintName.z = cname;
    p.constraintName.n = (int)strlen(cname);

    Expr* ce = new Expr();

    // Call method
    sqlite3AddCheckConstraint(&p, ce, "(", ")");

    // Assertions
    EXPECT_TRUE(p.pNewTable->pCheck != nullptr, "Expected pCheck to be created");
    EXPECT_TRUE(p.pNewTable->pCheck->a.size() == 1, "Expected exactly one expression in pCheck");
    EXPECT_TRUE(p.pNewTable->pCheck->a[0] == ce, "Expected appended expression to be pCheckExpr");
    // Name should be set from constraintName
    EXPECT_TRUE(p.pNewTable->pCheck->hasName, "Expected name to be set via constraintName");
    EXPECT_EQ(g_lastAssignedName, std::string(cname), "Constraint name should be preserved as provided");
    // Cleanup
    delete p.pNewTable;
    destroyTestDb(db);
    ++g_totalTests;
}

// Test 1B: pNewTable non-null, not readonly, constraintName.n == 0; name derived from zStart/zEnd
static void test_AddCheckConstraint_NameDerivedFromParens() {
    resetGlobals();
    Parse p;
    sqlite3* db = createTestDb(false);
    p.db = db;
    p.pNewTable = new Table();

    // No explicit constraint name
    p.constraintName.n = 0;
    p.constraintName.z = nullptr;

    // Provide "(  myname  )" as the contents; zStart points to '(', zEnd to end of string
    const char* raw = "(  myname  )";
    // Allocate buffer to ensure memory persists during function
    char* buffer = new char[strlen(raw) + 1];
    std::strcpy(buffer, raw);

    const char* zStart = buffer;                 // points to '('
    const char* zEnd = buffer + std::strlen(buffer); // end after ')'

    Expr* ce = new Expr();

    sqlite3AddCheckConstraint(&p, ce, zStart, zEnd);

    // Validate that a name was derived from the inner content "myname"
    EXPECT_TRUE(p.pNewTable->pCheck != nullptr, "Expected pCheck to be created");
    EXPECT_TRUE(p.pNewTable->pCheck->a.size() == 1, "Expected exactly one expression in pCheck");
    EXPECT_TRUE(p.pNewTable->pCheck->hasName, "Expected derived name to be set on pCheck");
    EXPECT_EQ(g_lastAssignedName, std::string("myname"), "Derived constraint name should be 'myname'");

    // Outcome: ensure we appended expression
    // Cleanup
    delete p.pNewTable;
    delete ce;
    delete[] buffer;
    destroyTestDb(db);
    ++g_totalTests;
}

// Test 2: pNewTable == nullptr -> should delete the expression (non-leaking path)
static void test_AddCheckConstraint_NoTableDeletesExpr() {
    resetGlobals();
    Parse p;
    sqlite3* db = createTestDb(false);
    p.db = db;
    p.pNewTable = nullptr; // simulate not in a table context
    Expr* ce = new Expr();

    sqlite3AddCheckConstraint(&p, ce, "(", ")");

    EXPECT_TRUE(g_exprDeletedCount == 1, "Expected expression to be deleted when no table exists");
    // Cleanup
    delete p.db;
    destroyTestDb(db);
    ++g_totalTests;
}

// Test 3: pNewTable non-null but readonly DB -> should delete the expression
static void test_AddCheckConstraint_ReadOnlyDatabaseDeletesExpr() {
    resetGlobals();
    Parse p;
    sqlite3* db = createTestDb(true); // readonly
    p.db = db;
    p.pNewTable = new Table();

    Expr* ce = new Expr();
    sqlite3AddCheckConstraint(&p, ce, "(", ")");

    EXPECT_TRUE(g_exprDeletedCount == 1, "Expected expression to be deleted when DB is readonly");
    // Cleanup
    delete p.pNewTable;
    delete db;
    ++g_totalTests;
}

// Entry point: run all tests
int main() {
    // Run tests
    test_AddCheckConstraint_ConstraintNameProvided();
    test_AddCheckConstraint_NameDerivedFromParens();
    test_AddCheckConstraint_NoTableDeletesExpr();
    test_AddCheckConstraint_ReadOnlyDatabaseDeletesExpr();

    // Summary
    std::cout << "Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";
    return g_failedTests ? 1 : 0;
}
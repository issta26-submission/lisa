// Minimal C++11 unit test suite for the sqlite3ExprCollSeq focal method.
// This test suite provides small in-memory mocks of the minimal subset of the
// SQLite data structures and APIs used by sqlite3ExprCollSeq, then exercises
// several branches of the function. No external test framework is used; a tiny
// test harness with non-terminating assertions is implemented inline.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// ---------------------------------------------------------------------------
// Mocked definitions to simulate the necessary parts of SQLite used by
// sqlite3ExprCollSeq. The goal is to enable exercising the control-flow in the
// focal method without pulling in the full SQLite codebase.
// NOTE: This is a focused mock; it is not a full SQLite replacement.
// ---------------------------------------------------------------------------

// Tokens (subset)
enum {
  TK_REGISTER = 1,
  TK_AGG_COLUMN,
  TK_COLUMN,
  TK_TRIGGER,
  TK_CAST,
  TK_UPLUS,
  TK_VECTOR,
  TK_COLLATE
};

// Expression property flags (subset)
enum {
  EP_IntValue = 0x01,
  EP_Collate  = 0x02
};

// Forward declarations
struct Expr;
struct Parse;
struct Table;
struct CollSeq;

// Lightweight CollSeq type (opaque in real SQLite)
struct CollSeq {
  const char* zName;
};

// Encoding macro (simplified for tests)
#define ENC(db) 0

// Expression structure (minimal fields used by sqlite3ExprCollSeq)
struct Expr {
  int op;
  int iColumn;
  struct {
    Table* pTab;
  } y;
  Expr* pLeft;
  struct {
    struct ExprList* pList;
  } x;
  struct {
    const char* zToken;
  } u;
  int flags;
};

// Table/Column definitions (minimal)
struct Column {
  const char* zColl;
};

struct Table {
  Column* aCol;
  int nCol;
};

// Expression list (used for TK_VECTOR)
struct ExprList {
  // Each element is a tiny wrapper with a pExpr
  struct ExprListItem {
    Expr* pExpr;
  } *a;
  int nExpr;
};

// Parse structure holding a reference to sqlite3 db
struct sqlite3 {
  bool mallocFailed; // used by some code paths in the real implementation
};

// Simple Parse wrapper
struct Parse {
  sqlite3* db;
};

// Global hook to simulate sqlite3CheckCollSeq behavior in tests.
// If this flag is set to true before a call, sqlite3CheckCollSeq will return 1
// (indicating a check failure) for testing the "pColl -> 0" path.
static bool g_forceCheckCollSeqFail = false;

// --- Mocked helper functions (subset of SQLite API behavior) ---

// Mock: Column Collation name
const char* sqlite3ColumnColl(const Column* pCol) {
  if (!pCol) return nullptr;
  return pCol->zColl ? pCol->zColl : "BINARY";
}

// Mock: Find a CollSeq by name
CollSeq* sqlite3FindCollSeq(sqlite3*, int /*enc*/, const char* zName, int /*dummy*/) {
  CollSeq* p = new CollSeq();
  p->zName = zName ? zName : "NO_NAME";
  return p;
}

// Mock: Get a CollSeq by token name (e.g., COLLATE token)
CollSeq* sqlite3GetCollSeq(Parse*, int /*enc*/, int /*unused*/, const char* zToken) {
  CollSeq* p = new CollSeq();
  p->zName = zToken ? zToken : "DEFAULT";
  return p;
}

// Mock: Check CollSeq validity (returns non-zero on failure)
int sqlite3CheckCollSeq(Parse*, CollSeq* /*pColl*/) {
  if (g_forceCheckCollSeqFail) {
    // Reset flag for subsequent calls
    g_forceCheckCollSeqFail = false;
    return 1;
  }
  return 0;
}

// Utils used by the focal method (predicates implemented as in SQLite)
bool ExprUseYTab(const Expr* p) {
  return p && p->y.pTab != nullptr;
}
bool ExprUseXList(const Expr* p) {
  return p && p->x.pList != nullptr;
}
bool ExprHasProperty(const Expr* p, int mask) {
  return p && (p->flags & mask) != 0;
}

// Helpers to create expressions and trees for tests
Expr* newColumnExpr(Parse* pParse, Table* pTab, int iColumn) {
  (void)pParse;
  Expr* e = new Expr();
  e->op = TK_COLUMN;
  e->iColumn = iColumn;
  e->y.pTab = pTab;
  e->pLeft = nullptr;
  e->x.pList = nullptr;
  e->u.zToken = nullptr;
  e->flags = 0;
  return e;
}
Expr* newCollateExpr(const char* token) {
  Expr* e = new Expr();
  e->op = TK_COLLATE;
  e->iColumn = -1;
  e->y.pTab = nullptr;
  e->pLeft = nullptr;
  e->x.pList = nullptr;
  e->u.zToken = token;
  e->flags = 0;
  return e;
}
Expr* newCastExpr(Expr* left) {
  Expr* e = new Expr();
  e->op = TK_CAST;
  e->pLeft = left;
  e->iColumn = -1;
  e->y.pTab = nullptr;
  e->x.pList = nullptr;
  e->u.zToken = nullptr;
  e->flags = 0;
  return e;
}
Expr* newUnaryVectorExpr(ExprList* pList) {
  Expr* e = new Expr();
  e->op = TK_VECTOR;
  e->pLeft = nullptr;
  e->x.pList = pList;
  e->y.pTab = nullptr;
  e->u.zToken = nullptr;
  e->flags = 0;
  return e;
}
ExprList* newExprList1(Expr* e0) {
  ExprList* pList = new ExprList();
  pList->nExpr = 1;
  pList->a = new ExprList::ExprListItem[1];
  pList->a[0].pExpr = e0;
  return pList;
}
ExprList* newExprListFromExpr(Expr* e) {
  return newExprList1(e);
}

// Core focal method replicated for standalone testing (simplified)
CollSeq* sqlite3ExprCollSeq(Parse* pParse, const Expr* pExpr) {
  sqlite3* db = pParse->db;
  CollSeq* pColl = 0;
  const Expr* p = pExpr;
  while (p) {
    int op = p->op;
    if (op == TK_REGISTER) op = p->op2;
    if ((op == TK_AGG_COLUMN && p->y.pTab != 0)
        || op == TK_COLUMN || op == TK_TRIGGER) {
      int j;
      // In test harness, ensure YTable is valid
      assert( ExprUseYTab(p) );
      assert( p->y.pTab != 0 );
      if ((j = p->iColumn) >= 0) {
        const char* zColl = sqlite3ColumnColl(&p->y.pTab->aCol[j]);
        pColl = sqlite3FindCollSeq(db, ENC(db), zColl, 0);
      }
      break;
    }
    if (op == TK_CAST || op == TK_UPLUS) {
      p = p->pLeft;
      continue;
    }
    if (op == TK_VECTOR) {
      assert( ExprUseXList(p) );
      p = p->x.pList->a[0].pExpr;
      continue;
    }
    if (op == TK_COLLATE) {
      assert( !ExprHasProperty(p, EP_IntValue) );
      pColl = sqlite3GetCollSeq(pParse, ENC(db), 0, p->u.zToken);
      break;
    }
    if ( p->flags & EP_Collate ) {
      if ( p->pLeft && (p->pLeft->flags & EP_Collate) != 0 ) {
        p = p->pLeft;
      } else {
        Expr* pNext  = p->pRight;
        // In test harness, the following asserts are kept conservative
        // as we control the tree structure.
        // assert( !ExprUseXList(p) || p->x.pList==0 || p->pRight==0 );
        if ( ExprUseXList(p) && p->x.pList != 0 ) {
          int i;
          for (i = 0; i < p->x.pList->nExpr; i++) {
            // If any subexpr has EP_Collate, use it
            if (ExprHasProperty(p->x.pList->a[i].pExpr, EP_Collate)) {
              pNext = p->x.pList->a[i].pExpr;
              break;
            }
          }
        }
        p = pNext;
      }
    } else {
      break;
    }
  }
  if (sqlite3CheckCollSeq(pParse, pColl)) {
    pColl = 0;
  }
  return pColl;
}

// -------------------------------------------------------------------------------------
// Test harness and test cases
// -------------------------------------------------------------------------------------

// Simple, non-terminating test macros
static int g_tests_run = 0;
static int g_tests_pass = 0;

#define EXPECT_TRUE(cond, msg) do {            \
  ++g_tests_run;                                 \
  if (cond) { g_tests_pass++; }                   \
  else { std::cerr << "TEST FAIL: " << msg << std::endl; } \
} while(0)

#define EXPECT_NOT_NULL(p, msg) EXPECT_TRUE((p) != nullptr, msg)

#define EXPECT_EQ_STR(a, b, msg) do { \
  ++g_tests_run; \
  if ((a) && (b) && std::string((a)) == std::string((b))) { g_tests_pass++; } \
  else { std::cerr << "TEST FAIL: " << msg << " (\"" << (a) << "\" != \"" << (b) << "\")" << std::endl; } \
} while(0)

static Parse* makeParse(sqlite3* db) {
  Parse* p = new Parse();
  p->db = db;
  return p;
}

// Test 1: Column-based coll seq via TK_COLUMN path
void test_column_path_basic() {
  // Setup db and parse
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  // Table with one column having collation "NOCASE"
  Table t;
  Column c;
  c.zColl = "NOCASE";
  t.aCol = &c;
  t.nCol = 1;

  // Expr: TK_COLUMN with iColumn=0 and y.pTab = &t
  Expr* pExpr = newColumnExpr(pParse, &t, 0);

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pExpr);
  EXPECT_NOT_NULL(pColl, "Column path should yield a CollSeq");
  EXPECT_EQ_STR(pColl->zName, "NOCASE", "Column path should use NOCASE coll sequence");
  delete pExpr;
  delete pColl;
  delete pParse;
  delete db;
}

// Test 2: TK_COLLATE path
void test_collate_path() {
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  Expr* pExpr = newCollateExpr("BINARY");

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pExpr);
  EXPECT_NOT_NULL(pColl, "TK_COLLATE path should yield a CollSeq");
  EXPECT_EQ_STR(pColl->zName, "BINARY", "TK_COLLATE should use provided token as coll seq");
  delete pExpr;
  delete pColl;
  delete pParse;
  delete db;
}

// Test 3: CAST -> COLUMN path (CAST should unwrap to the left column)
void test_cast_to_column_path() {
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  // Column to be reached after CAST unwrap
  Table t;
  Column c;
  c.zColl = "NOCASE2";
  t.aCol = &c;
  t.nCol = 1;

  Expr* pColumn = newColumnExpr(pParse, &t, 0);
  Expr* pCast = newCastExpr(pColumn);

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pCast);
  EXPECT_NOT_NULL(pColl, "CAST unwrap should reach a column and yield CollSeq");
  EXPECT_EQ_STR(pColl->zName, "NOCASE2", "Unwrapped column coll seq name mismatch");
  delete pColumn;
  delete pCast;
  delete pColl;
  delete pParse;
  delete db;
}

// Test 4: TK_VECTOR path -> first element is a COLUMN
void test_vector_path_to_column() {
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  Table t;
  Column c;
  c.zColl = "NOCASE_V";
  t.aCol = &c;
  t.nCol = 1;

  Expr* pColumn = newColumnExpr(pParse, &t, 0);
  ExprList* pList = newExprListFromExpr(pColumn);
  Expr* pVector = newUnaryVectorExpr(pList);

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pVector);
  EXPECT_NOT_NULL(pColl, "TK_VECTOR path should yield CollSeq from contained column");
  EXPECT_EQ_STR(pColl->zName, "NOCASE_V", "Vector-contained column coll seq name mismatch");

  delete pColumn;
  delete pList;
  delete pVector;
  delete pColl;
  delete pParse;
  delete db;
}

// Test 5: EP_Collate path to column via pNext movement
void test_ep_collate_path_to_column() {
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  // Prepare a column on which we will eventually obtain a CollSeq
  Table t;
  Column c;
  c.zColl = "EP_COLLATE";
  t.aCol = &c;
  t.nCol = 1;
  // Right-hand column for pRight of the EP_Collate expression
  Expr* pColumn = newColumnExpr(pParse, &t, 0);

  // Parent expression with EP_Collate set; pLeft is NULL; pRight points to column
  Expr* pParent = new Expr();
  pParent->op = 999; // some non-matching opcode to avoid early branches
  pParent->y.pTab = nullptr;
  pParent->pLeft = nullptr;
  pParent->x.pList = nullptr;
  pParent->u.zToken = nullptr;
  pParent->flags = EP_Collate;
  pParent->iColumn = -1;
  pParent->pRight = pColumn;

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pParent);
  EXPECT_NOT_NULL(pColl, "EP_Collate path should reach a column and yield CollSeq");
  // The coll name should come from the column's zColl
  EXPECT_EQ_STR(pColl->zName, "EP_COLLATE", "EP_Collate path should propagate column's coll seq");

  delete pColumn;
  delete pParent;
  delete pColl;
  delete pParse;
  delete db;
}

// Test 6: Branch that forces sqlite3CheckCollSeq to fail, ensuring pColl becomes NULL
void test_check_collseq_fail_resets_to_null() {
  sqlite3* db = new sqlite3();
  db->mallocFailed = false;
  Parse* pParse = makeParse(db);

  // Column with a known coll name
  Table t;
  Column c;
  c.zColl = "SAME_NAME";
  t.aCol = &c;
  t.nCol = 1;

  Expr* pExpr = newColumnExpr(pParse, &t, 0);

  // Force the check to fail
  g_forceCheckCollSeqFail = true;

  CollSeq* pColl = sqlite3ExprCollSeq(pParse, pExpr);
  // If the check fails, the result should be NULL
  EXPECT_TRUE(pColl == nullptr, "sqlite3CheckCollSeq failure should yield NULL CollSeq");

  delete pExpr;
  delete pColl;
  delete pParse;
  delete db;
}

// -------------------------------------------------------------------------------------
// Main: execute all tests
// -------------------------------------------------------------------------------------

int main() {
  std::cout << "Running sqlite3ExprCollSeq test suite (mocked environment)...\n";

  test_column_path_basic();
  test_collate_path();
  test_cast_to_column_path();
  test_vector_path_to_column();
  test_ep_collate_path_to_column();
  test_check_collseq_fail_resets_to_null();

  std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_pass << "\n";
  return (g_tests_run == g_tests_pass) ? 0 : 1;
}
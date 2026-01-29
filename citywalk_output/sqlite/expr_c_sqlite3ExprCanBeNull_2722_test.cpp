// Lightweight C++11 unit test suite for sqlite3ExprCanBeNull
// This test suite is self-contained and does not rely on GTest.
// It re-implements a minimal subset of the necessary SQLite-like structures
// and the focal function sqlite3ExprCanBeNull exactly as provided,
// along with small helpers to create test scenarios.

// Compile with: g++ -std=c++11 -O2 -Wall sqlite3ExprCanBeNull_test.cpp -o sqlite3ExprCanBeNull_test

#include <memory>
#include <iostream>
#include <sqliteInt.h>
#include <vector>


// Domain: define minimal types and constants to compile the focal function.

typedef unsigned char u8;

// Focal op-code constants (subset sufficient for tests)
enum {
  TK_UPLUS = 1,
  TK_UMINUS = 2,
  TK_REGISTER = 3,
  TK_INTEGER = 4,
  TK_STRING = 5,
  TK_FLOAT = 6,
  TK_BLOB = 7,
  TK_COLUMN = 8
};

// Property flags
const int EP_CanBeNull = 0x01;

// Forward declarations
struct Table;

// Minimal Expr structure to mirror needed fields
struct Expr {
  int op;
  Expr *pLeft;
  int op2;            // used when op == TK_REGISTER
  struct {
    Table *pTab;
  } y;
  int iColumn;
  int iProp;          // bit-flag field for properties (e.g., EP_CanBeNull)
};

// Minimal Column and Table structures to support notNull checks
struct Col {
  int notNull; // 0 means nullable, non-zero means NOT NULL
};

struct Table {
  Col *aCol; // dynamic array of columns
};

// Helpers mimicking required SQLite functions (simplified)
static inline bool ExprUseYTab(const Expr *p) {
  return p->y.pTab != nullptr;
}
static inline bool ExprHasProperty(const Expr *p, int prop) {
  return (p->iProp & prop) != 0;
}

// The focal method re-implemented exactly as provided (adapted to this test environment)
int sqlite3ExprCanBeNull(const Expr *p){
  u8 op;
  // In actual code there is an assert(p!=0); we mirror behavior by returning 1 only for safety in tests
  if (p == nullptr) return 1; // should not happen in tests
  // Unwrap unary plus/minus
  while( p->op==TK_UPLUS || p->op==TK_UMINUS ){
    p = p->pLeft;
  }
  op = p->op;
  if( op==TK_REGISTER ) op = p->op2;
  switch( op ){
    case TK_INTEGER:
    case TK_STRING:
    case TK_FLOAT:
    case TK_BLOB:
      return 0;
    case TK_COLUMN:
      assert( ExprUseYTab(p) );
      return ExprHasProperty(p, EP_CanBeNull) ||
             p->y.pTab==0 ||  /* Reference to column of index on expression */
             (p->iColumn>=0
              && p->y.pTab->aCol!=0 /* Possible due to prior error */
              && p->y.pTab->aCol[p->iColumn].notNull==0);
    default:
      return 1;
  }
}

// Lightweight test harness (non-terminating checks)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    ++g_failures; \
    std::cerr << "EXPECT_TRUE failed: " << (msg) \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

#define EXPECT_EQ(a,b, msg) do { \
  if((a) != (b)) { \
    ++g_failures; \
    std::cerr << "EXPECT_EQ failed: " << (a) << " != " << (b) \
              << " (" << (msg) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

// Helper to create a simple Table with 'n' columns
Table* makeTable(int n) {
  Table* t = new Table();
  t->aCol = new Col[n];
  for(int i=0;i<n;i++) t->aCol[i].notNull = 0; // default nullable
  return t;
}

// Helper to clean up Table
void destroyTable(Table* t) {
  if(!t) return;
  delete[] t->aCol;
  delete t;
}

// Test 1: Basic literal integer should be non-nullable (0)
void test_basic_integer() {
  Expr e;
  e.op = TK_INTEGER;
  e.pLeft = nullptr;
  e.op2 = 0;
  e.y.pTab = nullptr;
  e.iColumn = -1;
  e.iProp = 0;
  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 0, "TK_INTEGER should produce 0 (not nullable)");
}

// Test 2: Unary wrapper around a string should propagate nullability = 0
void test_unary_wrap_propagates() {
  // Inner leaf: TK_STRING
  Expr inner;
  inner.op = TK_STRING;
  inner.pLeft = nullptr;
  inner.op2 = 0;
  inner.y.pTab = nullptr;
  inner.iColumn = -1;
  inner.iProp = 0;

  // Outer: TK_UMINUS wrapping inner
  Expr outer;
  outer.op = TK_UMINUS;
  outer.pLeft = &inner;
  outer.op2 = 0;
  outer.y.pTab = nullptr;
  outer.iColumn = -1;
  outer.iProp = 0;

  int r = sqlite3ExprCanBeNull(&outer);
  EXPECT_EQ(r, 0, "Unary minus wrapping a string should be non-nullable (0)");
}

// Test 3: TK_COLUMN with notNull == 0 should be nullable (1) when no EP_CanBeNull
void test_column_nullable_due_to_notNull_zero() {
  // Build a table with at least one column whose notNull == 0
  Table* tab = makeTable(1);
  tab->aCol[0].notNull = 0; // explicitly nullable
  Expr e;
  e.op = TK_COLUMN;
  e.pLeft = nullptr;
  e.op2 = 0;
  e.y.pTab = tab;
  e.iColumn = 0;
  e.iProp = 0; // no EP_CanBeNull

  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 1, "TK_COLUMN with notNull==0 should be nullable (1)");
  destroyTable(tab);
}

// Test 4: TK_COLUMN with iColumn < 0 should return 0 if no EP_CanBeNull and not index-lookup
void test_column_bad_index_no_null_prop() {
  Table* tab = makeTable(2);
  // Not setting any particular notNull
  tab->aCol[0].notNull = 1;
  tab->aCol[1].notNull = 1;
  Expr e;
  e.op = TK_COLUMN;
  e.pLeft = nullptr;
  e.op2 = 0;
  e.y.pTab = tab;
  e.iColumn = -1;  // invalid index
  e.iProp = 0;

  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 0, "TK_COLUMN with iColumn<0 and no EP_CanBeNull should be non-nullable (0)");
  destroyTable(tab);
}

// Test 5: TK_COLUMN with EP_CanBeNull property set should be nullable (1)
void test_column_nullable_due_to_property_flag() {
  Table* tab = makeTable(2);
  tab->aCol[0].notNull = 1; // NOT NULL, but we rely on EP_CanBeNull to override
  tab->aCol[1].notNull = 1;

  Expr e;
  e.op = TK_COLUMN;
  e.pLeft = nullptr;
  e.op2 = 0;
  e.y.pTab = tab;
  e.iColumn = 0;
  e.iProp = EP_CanBeNull; // explicitly mark as can be null

  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 1, "TK_COLUMN with EP_CanBeNull should be nullable (1)");
  destroyTable(tab);
}

// Test 6: TK_REGISTER should map to op2 and behave accordingly (e.g., TK_FLOAT => 0)
void test_register_mapping() {
  Expr e;
  e.op = TK_REGISTER;
  e.op2 = TK_FLOAT; // will be used as the underlying op
  e.pLeft = nullptr;
  e.y.pTab = nullptr;
  e.iColumn = -1;
  e.iProp = 0;

  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 0, "TK_REGISTER mapped to TK_FLOAT should be non-nullable (0)");
}

// Test 7: Unknown default operation should return 1 (nullable)
void test_default_branch() {
  Expr e;
  e.op = 9999; // some unknown opcode
  e.pLeft = nullptr;
  e.y.pTab = nullptr;
  e.iColumn = -1;
  e.iProp = 0;

  int r = sqlite3ExprCanBeNull(&e);
  EXPECT_EQ(r, 1, "Unknown opcode should be considered nullable (1)");
}

// Test 8: Wrap with UPLUS and a TK_COLUMN that is explicitly nullable via notNull
void test_unary_wrap_with_column_notNull_zero() {
  Table* tab = makeTable(1);
  tab->aCol[0].notNull = 0; // Column explicitly nullable

  Expr col;
  col.op = TK_COLUMN;
  col.pLeft = nullptr;
  col.op2 = 0;
  col.y.pTab = tab;
  col.iColumn = 0;
  col.iProp = 0;

  // Outer: TK_UPLUS wrapping the column
  Expr outer;
  outer.op = TK_UPLUS;
  outer.pLeft = &col;
  outer.op2 = 0;
  outer.y.pTab = nullptr;
  outer.iColumn = -1;
  outer.iProp = 0;

  int r = sqlite3ExprCanBeNull(&outer);
  EXPECT_EQ(r, 1, "UPLUS-wrapped TK_COLUMN with notNull==0 should be nullable (1)");
  destroyTable(tab);
}

// Main: run all tests
int main() {
  // Run tests
  test_basic_integer();
  test_unary_wrap_propagates();
  test_column_nullable_due_to_notNull_zero();
  test_column_bad_index_no_null_prop();
  test_column_nullable_due_to_property_flag();
  test_register_mapping();
  test_default_branch();
  test_unary_wrap_with_column_notNull_zero();

  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " test(s) failed." << std::endl;
    return 1;
  }
}
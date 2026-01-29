// Lightweight C++11 unit tests for the focal method sqlite3ExprCodeGeneratedColumn
// This test harness provides minimal stubs of the SQLite internal types and
// functions used by sqlite3ExprCodeGeneratedColumn in order to exercise its
// control flow (true/false branches) without requiring the full SQLite build.
// It uses a small, terminating-free "EXPECT_" style framework suitable for
// C++11, without any external testing library.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// -------------------------
// Minimal domain stubs & helpers
// -------------------------

// Constants mirroring essential SQLite values used by the focal method
static const int OP_IfNullRow = 1;
static const int OP_Affinity  = 2;
static const int SQLITE_AFF_TEXT = 1;

// Forward declare to mimic C-style usage in the focal method
struct Expr;

// Lightweight structures to emulate SQLite types used by the focal function
struct Column {
  int affinity; // Affinity value
};

struct Table {
  // Intentionally empty for stubbed usage
};

struct Db {
  int errByteOffset;
};

struct Vdbe {
  // Record a list of operation codes pushed by the dummy VDBE
  std::vector<int> ops;
  // Last jump target address seen by sqlite3VdbeJumpHere
  int lastJumpAddr;
  Vdbe() : errByteOffset(0), lastJumpAddr(0) {}
};

// Pseudo-Parse context required by sqlite3ExprCodeGeneratedColumn
struct Parse {
  Vdbe *pVdbe;
  int nErr;
  int iSelfTab; // must be non-zero per assertion in focal method
  Db *db;
  bool simulateErr; // if true, sqlite3ExprCodeCopy will simulate an error
  Parse() : pVdbe(nullptr), nErr(0), iSelfTab(0), db(nullptr), simulateErr(false) {}
};

// A dummy Expr to satisfy the call to sqlite3ColumnExpr and sqlite3ExprCodeCopy
struct Expr {
  // Intentionally empty for stub
};

// -------------------------
// Forward declarations of functions used by focal method (stubs)
// -------------------------

// Reproducer (stubbed) APIs used by the focal function
Expr* sqlite3ColumnExpr(Table* pTab, Column* pCol);
void sqlite3ExprCodeCopy(Parse* pParse, Expr* pExpr, int regOut);
int sqlite3VdbeAddOp3(Vdbe* v, int op, int p1, int p2, int p3);
void sqlite3VdbeAddOp4(Vdbe* v, int op, int p1, int p2, int p3, const void* p4, int p4type);
void sqlite3VdbeJumpHere(Vdbe* v, int iAddr);

// -------------------------
// Minimal, test-focused implementation of the focal method
// -------------------------

// The focal method under test (copied here for a self-contained unit test
// environment. It relies on the stubs above and operates on the lightweight
// Parse/Table/Column structures defined in this test file.)
void sqlite3ExprCodeGeneratedColumn(
  Parse *pParse,     /* Parsing context */
  Table *pTab,       /* Table containing the generated column */
  Column *pCol,      /* The generated column */
  int regOut         /* Put the result in this register */
){
  int iAddr;
  Vdbe *v = pParse->pVdbe;
  int nErr = pParse->nErr;
  assert( v!=0 );
  assert( pParse->iSelfTab!=0 );
  if( pParse->iSelfTab>0 ){
    iAddr = sqlite3VdbeAddOp3(v, OP_IfNullRow, pParse->iSelfTab-1, 0, regOut);
  }else{
    iAddr = 0;
  }
  sqlite3ExprCodeCopy(pParse, sqlite3ColumnExpr(pTab,pCol), regOut);
  if( pCol->affinity>=SQLITE_AFF_TEXT ){
    sqlite3VdbeAddOp4(v, OP_Affinity, regOut, 1, 0, &pCol->affinity, 1);
  }
  if( iAddr ) sqlite3VdbeJumpHere(v, iAddr);
  if( pParse->nErr>nErr ) pParse->db->errByteOffset = -1;
}

// -------------------------
// Stubbed helper implementations
// -------------------------

// The column expression is simply a singleton Expr reused for testing
Expr* sqlite3ColumnExpr(Table*, Column*) {
  static Expr dummy;
  (void)dummy; // suppress unused warning in case of optimization
  return &dummy;
}

void sqlite3ExprCodeCopy(Parse* pParse, Expr* /*pExpr*/, int /*regOut*/) {
  // In tests, optionally simulate an error by toggling pParse->simulateErr
  if( pParse->simulateErr ){
    // Simulate an error during code generation
    pParse->nErr += 1;
  }
  // No actual code emission needed for the test stubs
}

int sqlite3VdbeAddOp3(Vdbe* v, int op, int p1, int p2, int p3) {
  // Return a non-zero address to exercise the "jump here" path
  int addr = (int)v->ops.size();
  v->ops.push_back(op);
  // Return address offset as (addr + 1) to ensure non-zero
  return addr + 1;
}

void sqlite3VdbeAddOp4(Vdbe* v, int op, int p1, int p2, int p3, const void* p4, int /*p4type*/) {
  v->ops.push_back(op);
  // We don't model the constant payload in detail for this test
  (void)p4;
}

void sqlite3VdbeJumpHere(Vdbe* v, int iAddr) {
  v->lastJumpAddr = iAddr;
}

// -------------------------
// Lightweight testing framework (non-terminating, single-file)
// -------------------------

static int g_total = 0;
static int g_failed = 0;
static std::vector<std::string> g_messages;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_total; \
  if(!(cond)) { \
    ++g_failed; \
    g_messages.push_back("EXPECT_TRUE failed: " + std::string(msg)); \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_total; \
  if(!((a) == (b))) { \
    ++g_failed; \
    g_messages.push_back("EXPECT_EQ failed: " + std::string(msg) + " | got: " + std::to_string(a) + " vs expected: " + std::to_string(b)); \
  } \
} while(0)

#define TEST(name) void name();

// -------------------------
// Test cases
// -------------------------

// Test Case A: iSelfTab > 0, affinity TEXT, no artificial errors
TEST(test_generated_column_branch_ifnull_and_affinity_true){
  // Arrange
  Parse p;
  Vdbe v;
  Db db; db.errByteOffset = 0;
  p.pVdbe = &v;
  p.nErr = 0;
  p.iSelfTab = 2;      // >0 triggers OP_IfNullRow
  p.db = &db;
  p.simulateErr = false;

  Table tab;
  Column col; col.affinity = SQLITE_AFF_TEXT; // affinity >= TEXT triggers AFFINITY op

  int regOut = 5;

  // Act
  sqlite3ExprCodeGeneratedColumn(&p, &tab, &col, regOut);

  // Assert
  // Expect first op to be OP_IfNullRow
  EXPECT_TRUE(!v.ops.empty(), "OP list should not be empty");
  EXPECT_EQ(v.ops[0], OP_IfNullRow, "First op should be OP_IfNullRow when iSelfTab>0");

  // Expect an AFFINITY op to be emitted due to affinity >= TEXT
  bool hasAffinity = false;
  for(size_t i=0; i<v.ops.size(); ++i){
    if(v.ops[i] == OP_Affinity){ hasAffinity = true; break; }
  }
  EXPECT_TRUE(hasAffinity, "AFFINITY op should be emitted when column affinity >= TEXT");

  // Expect JumpHere to have been invoked with a non-zero address
  EXPECT_TRUE(v.lastJumpAddr != 0, "JumpHere should be invoked and receive a non-zero iAddr");

  // Expect no errByteOffset since simulateErr is false
  EXPECT_EQ(db.errByteOffset, 0, "errByteOffset should remain unchanged when no error");
}

// Test Case B: iSelfTab < 0 (negative), affinity TEXT (no IfNullRow), ensure AFFINITY still emitted
TEST(test_generated_column_branch_negative_selfTab_affinity_true){
  // Arrange
  Parse p;
  Vdbe v;
  Db db; db.errByteOffset = 0;
  p.pVdbe = &v;
  p.nErr = 0;
  p.iSelfTab = -1;     // <0, so iAddr = 0; no OP_IfNullRow
  p.db = &db;
  p.simulateErr = false;

  Table tab;
  Column col; col.affinity = SQLITE_AFF_TEXT;

  int regOut = 7;

  // Act
  sqlite3ExprCodeGeneratedColumn(&p, &tab, &col, regOut);

  // Assert
  // Expect AFFINITY op emitted
  bool hasAffinity = false;
  for(size_t i=0; i<v.ops.size(); ++i){
    if(v.ops[i] == OP_Affinity){ hasAffinity = true; break; }
  }
  EXPECT_TRUE(hasAffinity, "AFFINITY op should be emitted when affinity >= TEXT even if iSelfTab<0");

  // Since iSelfTab < 0, there should be no OP_IfNullRow
  for(size_t i=0; i<v.ops.size(); ++i){
    EXPECT_TRUE(v.ops[i] != OP_IfNullRow, "No OP_IfNullRow should be emitted when iSelfTab<0");
  }

  // JumpHere should not have been called (iAddr would be 0)
  EXPECT_TRUE(v.lastJumpAddr == 0, "JumpHere should not be invoked when iSelfTab<0");

  // No error
  EXPECT_EQ(db.errByteOffset, 0, "errByteOffset should remain unchanged when no error");
}

// Test Case C: Affinity is below TEXT (no AFFINITY op), iSelfTab > 0
TEST(test_generated_column_branch_affinity_below_text_no_affinity){
  // Arrange
  Parse p;
  Vdbe v;
  Db db; db.errByteOffset = 0;
  p.pVdbe = &v;
  p.nErr = 0;
  p.iSelfTab = 3;
  p.db = &db;
  p.simulateErr = false;

  Table tab;
  Column col; col.affinity = 0; // below TEXT, so no AFFINITY op

  int regOut = 9;

  // Act
  sqlite3ExprCodeGeneratedColumn(&p, &tab, &col, regOut);

  // Assert
  // First op should be OP_IfNullRow since iSelfTab>0
  EXPECT_TRUE(!v.ops.empty(), "OP list should contain at least one entry");
  EXPECT_EQ(v.ops[0], OP_IfNullRow, "First op should be OP_IfNullRow when iSelfTab>0");

  // There should be no AFFINITY op in the list
  bool hasAffinity = false;
  for(size_t i=0; i<v.ops.size(); ++i){
    if(v.ops[i] == OP_Affinity){ hasAffinity = true; break; }
  }
  EXPECT_TRUE(!hasAffinity, "AFFINITY op should NOT be emitted when affinity < TEXT");

  // Jump should occur because iAddr > 0
  EXPECT_TRUE(v.lastJumpAddr != 0, "JumpHere should be invoked with non-zero iAddr when iSelfTab>0");

  // No error
  EXPECT_EQ(db.errByteOffset, 0, "errByteOffset should remain unchanged when no error");
}

// Test Case D: Simulated error during code generation triggers errByteOffset = -1
TEST(test_generated_column_error_path_sets_errByteOffset){
  // Arrange
  Parse p;
  Vdbe v;
  Db db; db.errByteOffset = 0;
  p.pVdbe = &v;
  p.nErr = 0;
  p.iSelfTab = 2;
  p.db = &db;
  p.simulateErr = true; // cause sqlite3ExprCodeCopy to bump nErr

  Table tab;
  Column col; col.affinity = SQLITE_AFF_TEXT;

  int regOut = 4;

  // Act
  sqlite3ExprCodeGeneratedColumn(&p, &tab, &col, regOut);

  // Assert
  // After the simulated error, errByteOffset should be set to -1
  EXPECT_EQ(db.errByteOffset, -1, "errByteOffset should be -1 when an error is detected in code generation");
}

// Test Case E: iSelfTab > 0 with affinity TEXT but with simulated error to verify early exit state
TEST(test_generated_column_error_path_with_affinity_and_ifnull){
  // Arrange
  Parse p;
  Vdbe v;
  Db db; db.errByteOffset = 0;
  p.pVdbe = &v;
  p.nErr = 1; // pretend there was a pre-existing error before this call
  p.iSelfTab = 2;
  p.db = &db;
  p.simulateErr = false;

  Table tab;
  Column col; col.affinity = SQLITE_AFF_TEXT;

  int regOut = 2;

  // Act
  sqlite3ExprCodeGeneratedColumn(&p, &tab, &col, regOut);

  // Assert: Even if nErr was non-zero, the function only checks after potential copy
  // and may set errByteOffset if pParse->nErr increased. Our simulateErr=false here implies
  // nErr remains unchanged; thus the -1 path should not be taken.
  EXPECT_EQ(db.errByteOffset, 0, "errByteOffset should remain unchanged when no new error occurs");
}

// -------------------------
// Main: run tests and report summary
// -------------------------

int main() {
  // Run defined test cases
  test_generated_column_branch_ifnull_and_affinity_true();
  test_generated_column_branch_negative_selfTab_affinity_true();
  test_generated_column_branch_affinity_below_text_no_affinity();
  test_generated_column_error_path_sets_errByteOffset();
  test_generated_column_error_path_with_affinity_and_ifnull();

  // Report
  std::cout << "SQLiteGeneratedColumn tests: executed " << g_total
            << " checks, " << g_failed << " failures." << std::endl;
  for (const auto& m : g_messages) {
    std::cout << "  - " << m << std::endl;
  }
  return (g_failed == 0) ? 0 : 1;
}

/*
Notes for maintainers:
- This test harness intentionally provides minimal, isolated stubs for the
  dependencies used by sqlite3ExprCodeGeneratedColumn to enable unit testing
  of its control flow without requiring a full SQLite build.
- We exercised true/false branches:
  - iSelfTab > 0 (OP_IfNullRow emitted)
  - iSelfTab < 0 (no OP_IfNullRow emitted)
  - Affinity path (OP_Affinity emitted when affinity >= TEXT)
  - No AFFINITY when affinity < TEXT
  - JumpHere invocation (non-zero iAddr) when iSelfTab > 0
  - Error propagation via db.errByteOffset when sqlite3ExprCodeCopy simulates an error
- The tests avoid GTest and use a small, non-terminating assertion style (EXPECT_* macros)
- All static/global functions used by the focal method in this test are defined in this
  single translation unit to ensure deterministic behavior and easy review.
- If integrating into a larger C++ test suite, consider replacing the lightweight test
  harness with a dedicated framework, while preserving the test scenarios and asserts.
*/
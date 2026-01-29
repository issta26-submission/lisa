// High-quality C++11 unit test suite for sqlite3GenerateIndexKey (delete.c) without GTest
// This test suite uses a lightweight, self-contained harness with stubs for the sqlite3
// interfaces used by the focal function. It exercises multiple code paths and ensures
// executable test cases without depending on external testing frameworks.
//
// Notes:
// - The test focuses on behavior of sqlite3GenerateIndexKey: piPartIdxLabel handling,
//   MakeRecord emission, REAL affinity handling, and partial/index-prefix behavior.
// - We implement minimal, deterministic stubs for the sqlite3/Vdbe infrastructure.
// - Tests use non-terminating checks (log-based accumulators) to maximize coverage.

#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// ----------------------------
// Minimal sqlite-like scaffolding
// ----------------------------

// Forward declarations to satisfy the focal function's signatures
struct Parse;
struct Vdbe;
struct Expr;
struct Index;

// Operation codes (subset tailored for tests)
enum OpCode {
  OP_MakeRecord = 1,
  OP_RealAffinity = 2,
  OP_LoadIndexColumn = 3, // internal helper op to simulate loading a column
};

// Lightweight representation of a Vdbe operation
struct Op {
  int opcode;
  int p1;
  int p2;
  int p3;
};

// Lightweight Vdbe: stores a sequence of ops
struct Vdbe {
  std::vector<Op> ops;
  void AddOp3(int op, int p1, int p2, int p3) {
    ops.push_back(Op{op, p1, p2, p3});
  }
  // Remove the last occurrence of a given opcode (mimic sqlite3VdbeDeletePriorOpcode)
  void DeletePriorOpcode(int opcode) {
    for (int i = (int)ops.size() - 1; i >= 0; --i) {
      if (ops[i].opcode == opcode) {
        ops.erase(ops.begin() + i);
        break;
      }
    }
  }
  // Utility: check if an opcode exists in the current plan
  bool HasOpcode(int opcode) const {
    for (const auto& o : ops) if (o.opcode == opcode) return true;
    return false;
  }
  // Clear all operations (reset state)
  void Clear() { ops.clear(); }
};

// Parsing context
struct Parse {
  Vdbe* pVdbe;
  int iSelfTab;
  Parse() : pVdbe(nullptr), iSelfTab(0) {}
};

// Simple expression placeholder (unused in tests beyond presence)
struct Expr {
  int dummy;
  Expr() : dummy(0) {}
};

// Index descriptor used by sqlite3GenerateIndexKey
struct Index {
  int nColumn;   // total number of columns in the index
  int nKeyCol;   // key columns if index has a prefix (used for "prefixOnly")
  int uniqNotNull; // used in prefix computation
  int* aiColumn; // per-column source mapping; -1 (XN_EXPR) means expression
  Expr* pPartIdxWhere; // where clause for partial index (nullptr means no partial index)
  Index() : nColumn(0), nKeyCol(0), uniqNotNull(0), aiColumn(nullptr), pPartIdxWhere(nullptr) {}
};

// XN_EXPR sentinel (as in sqlite3)
static const int XN_EXPR = -1;

// Simple global test-state to observe behavior
static int g_tempRegCounter = 1000;

// Helpers to reset global test state
static void resetTestEnv() {
  g_tempRegCounter = 1000;
}

// sqlite3GetTempRange stub: allocate a range of temporary registers
static int sqlite3GetTempRange(Parse* pParse, int nCol) {
  (void)pParse;
  int base = g_tempRegCounter;
  g_tempRegCounter += nCol;
  return base;
}

// sqlite3ReleaseTempRange stub (no-op for test)
static void sqlite3ReleaseTempRange(Parse* pParse, int regBase, int nCol) {
  (void)pParse; (void)regBase; (void)nCol;
}

// sqlite3VdbeMakeLabel stub: return a unique label number
static int sqlite3VdbeMakeLabel(Parse* pParse) {
  static int s_label = 1;
  (void)pParse;
  return s_label++;
}

// sqlite3ExprIfFalseDup stub: no-op for tests
static void sqlite3ExprIfFalseDup(Parse* pParse, Expr* pExpr, int label, int jumpIfNull) {
  (void)pParse; (void)pExpr; (void)label; (void)jumpIfNull;
  // no-op in test harness; presence of partial index may be asserted via label value
}

// sqlite3VdbeAddOp3: wrap Vdbe op insert
static void sqlite3VdbeAddOp3(Vdbe* v, int op, int p1, int p2, int p3) {
  v->AddOp3(op, p1, p2, p3);
}

// sqlite3ExprCodeLoadIndexColumn: simulate loading a column data into register
static void sqlite3ExprCodeLoadIndexColumn(Parse* pParse, Index* pIdx, int iDataCur, int j, int reg) {
  // Record that we've loaded a column; we don't simulate actual data.
  // Push a dummy op to indicate a load occurred (could be used for verification)
  (void)pParse; (void)pIdx; (void)iDataCur; (void)j; (void)reg;
  // Use a specific opcode to mark loading
  // In practice, we push a no-op with a distinct opcode to reflect work done
  // We reuse OP_LoadIndexColumn for this purpose
  if (pParse && pParse->pVdbe) {
    pParse->pVdbe->AddOp3(OP_LoadIndexColumn, reg, j, iDataCur);
  }
}

// sqlite3VdbeDeletePriorOpcode: delete prior matching opcode
static void sqlite3VdbeDeletePriorOpcode(Vdbe* v, int opcode) {
  v->DeletePriorOpcode(opcode);
}

// Forward declaration of focal function (provided in the prompt)
int sqlite3GenerateIndexKey(
  Parse *pParse,       /* Parsing context */
  Index *pIdx,         /* The index for which to generate a key */
  int iDataCur,        /* Cursor number from which to take column data */
  int regOut,          /* Put the new key into this register if not 0 */
  int prefixOnly,      /* Compute only a unique prefix of the key */
  int *piPartIdxLabel, /* OUT: Jump to this label to skip partial index */
  Index *pPrior,       /* Previously generated index key */
  int regPrior         /* Register holding previous generated key */
){
{
  Vdbe *v = pParse->pVdbe;
  int j;
  int regBase;
  int nCol;
  if( piPartIdxLabel ){
    if( pIdx->pPartIdxWhere ){
      *piPartIdxLabel = sqlite3VdbeMakeLabel(pParse);
      pParse->iSelfTab = iDataCur + 1;
      sqlite3ExprIfFalseDup(pParse, pIdx->pPartIdxWhere, *piPartIdxLabel,
                            SQLITE_JUMPIFNULL);
      pParse->iSelfTab = 0;
      pPrior = 0; /* Ticket a9efb42811fa41ee 2019-11-02;
                  ** pPartIdxWhere may have corrupted regPrior registers */
    }else{
      *piPartIdxLabel = 0;
    }
  }
  nCol = (prefixOnly && pIdx->uniqNotNull) ? pIdx->nKeyCol : pIdx->nColumn;
  regBase = sqlite3GetTempRange(pParse, nCol);
  if( pPrior && (regBase!=regPrior || pPrior->pPartIdxWhere) ) pPrior = 0;
  for(j=0; j<nCol; j++){
    if( pPrior
     && pPrior->aiColumn[j]==pIdx->aiColumn[j]
     && pPrior->aiColumn[j]!=XN_EXPR
    ){
      /* This column was already computed by the previous index */
      continue;
    }
    sqlite3ExprCodeLoadIndexColumn(pParse, pIdx, iDataCur, j, regBase+j);
    if( pIdx->aiColumn[j]>=0 ){
      /* If the column affinity is REAL but the number is an integer, then it
      ** might be stored in the table as an integer (using a compact
      ** representation) then converted to REAL by an OP_RealAffinity opcode.
      ** But we are getting ready to store this value back into an index, where
      ** it should be converted by to INTEGER again.  So omit the
      ** OP_RealAffinity opcode if it is present */
      sqlite3VdbeDeletePriorOpcode(v, OP_RealAffinity);
    }
  }
  if( regOut ){
    sqlite3VdbeAddOp3(v, OP_MakeRecord, regBase, nCol, regOut);
  }
  sqlite3ReleaseTempRange(pParse, regBase, nCol);
  return regBase;
}
}

// Redefinition (since we can't rely on real static initializers in this harness)
static int sqlite3GenerateIndexKey_implementation(
  Parse *pParse,
  Index *pIdx,
  int iDataCur,
  int regOut,
  int prefixOnly,
  int *piPartIdxLabel,
  Index *pPrior,
  int regPrior
){
  // The actual function body is re-declared above; keep invocation aligned.
  return sqlite3GenerateIndexKey(pParse, pIdx, iDataCur, regOut, prefixOnly, piPartIdxLabel, pPrior, regPrior);
}

// Trait: expose function under the intended symbol for test usage
int main_placeholder = 0; // unused

// ----------------------------
// Test harness
// ----------------------------

// Simple non-terminating assertion mechanism
struct TestResult {
  std::string name;
  bool passed;
  std::string message;
  TestResult(const std::string& n, bool p, const std::string& m)
    : name(n), passed(p), message(m) {}
};

static std::vector<TestResult> g_results;

// helper to record test result
static void recordResult(const std::string& name, bool ok, const std::string& msg="") {
  g_results.emplace_back(name, ok, msg);
}

// Assertion helper: non-terminating, records failure and continues
static void expect_true(bool cond, const std::string& msg) {
  if (!cond) recordResult("ASSERT_FAIL", false, msg);
  else recordResult("ASSERT_PASS", true, "");
}

// Test 1: piPartIdxLabel should be set when pPartIdxWhere exists; iSelfTab should be toggled
static void test_partIdxLabel_set_and_iSelfTab_reset() {
  resetTestEnv();
  // Prepare parse and Vdbe
  Parse parse;
  Vdbe vdb;
  parse.pVdbe = &vdb;
  parse.iSelfTab = 0;

  // Prepare index with a partial index condition
  Index idx;
  idx.nColumn = 1;
  idx.nKeyCol = 1;
  idx.uniqNotNull = 0;
  int ai[1] = {0}; // one column maps to column 0
  idx.aiColumn = ai;
  Expr partWhere;
  idx.pPartIdxWhere = &partWhere;

  int label = 0;
  int ret = sqlite3GenerateIndexKey(&parse, &idx, 3, 0, 1, &label, nullptr, 0);
  // Assertions
  // piPartIdxLabel should be non-zero
  expect_true(label != 0, "PartIdxLabel should be non-zero when pPartIdxWhere exists");
  // iSelfTab should have been set to iDataCur+1 and then reset to 0
  // Since function resets iSelfTab after setting label, we expect 0 after call
  expect_true(parse.iSelfTab == 0, "parse.iSelfTab should be reset to 0 after partial index handling");
  // Basic sanity: labels must have been produced
  recordResult("test_partIdxLabel_set_and_iSelfTab_reset", true, "");
}

// Test 1b: piPartIdxLabel should be 0 when pPartIdxWhere is null
static void test_partIdxLabel_not_set_when_no_pPartIdxWhere() {
  resetTestEnv();
  Parse parse;
  Vdbe vdb;
  parse.pVdbe = &vdb;
  parse.iSelfTab = 0;

  Index idx;
  idx.nColumn = 1;
  idx.nKeyCol = 1;
  idx.uniqNotNull = 0;
  int ai[1] = {0};
  idx.aiColumn = ai;
  idx.pPartIdxWhere = nullptr; // no where clause for partial index

  int label = -1;
  int ret = sqlite3GenerateIndexKey(&parse, &idx, 3, 0, 1, &label, nullptr, 0);
  // When no pPartIdxWhere, *piPartIdxLabel should be set to 0
  expect_true(label == 0, "PartIdxLabel should be 0 when pPartIdxWhere is NULL");
  recordResult("test_partIdxLabel_not_set_when_no_pPartIdxWhere", true, "");
}

// Test 2: regOut non-zero yields a MakeRecord op with correct operands
static void test_makeRecord_emission() {
  resetTestEnv();
  Parse parse;
  Vdbe vdb;
  parse.pVdbe = &vdb;

  Index idx;
  idx.nColumn = 1;
  idx.nKeyCol = 1;
  idx.uniqNotNull = 0;
  int ai[1] = {0};
  idx.aiColumn = ai;
  idx.pPartIdxWhere = nullptr;

  int label = 0;
  int regOut = 7;
  int ret = sqlite3GenerateIndexKey(&parse, &idx, 4, regOut, 0, &label, nullptr, 0);

  // Verify that a MakeRecord op was emitted with p3 == regOut
  bool found = false;
  for (const auto& op : vdb.ops) {
    if (op.opcode == OP_MakeRecord && op.p3 == regOut) {
      found = true;
      break;
    }
  }
  expect_true(found, "MakeRecord should be emitted with (regBase, nCol, regOut)");
  recordResult("test_makeRecord_emission", true, "");
}

// Test 3: RealAffinity opcode should be deleted for columns with aiColumn[j] >= 0
static void test_realAffinity_deletion_when_needed() {
  resetTestEnv();
  Parse parse;
  Vdbe vdb;
  parse.pVdbe = &vdb;

  // Insert a RealAffinity op ahead of time to simulate the scenario that should be deleted
  vdb.AddOp3(OP_RealAffinity, 0, 0, 0);

  Index idx;
  idx.nColumn = 1;
  idx.nKeyCol = 1;
  idx.uniqNotNull = 0;
  int ai[1] = {0}; // aiColumn[0] >= 0 to trigger deletion
  idx.aiColumn = ai;
  idx.pPartIdxWhere = nullptr;

  int label = 0;
  int ret = sqlite3GenerateIndexKey(&parse, &idx, 5, 0, 0, &label, nullptr, 0);

  // Ensure there is no OP_RealAffinity left
  bool hasRealAffinity = vdb.HasOpcode(OP_RealAffinity);
  expect_true(!hasRealAffinity, "OP_RealAffinity should be deleted by GenerateIndexKey when appropriate");
  recordResult("test_realAffinity_deletion_when_needed", true, "");
}

// Test 4: When regOut == 0, there should be no MakeRecord emission
static void test_no_makeRecord_when_regOut_zero() {
  resetTestEnv();
  Parse parse;
  Vdbe vdb;
  parse.pVdbe = &vdb;

  Index idx;
  idx.nColumn = 1;
  idx.nKeyCol = 1;
  idx.uniqNotNull = 0;
  int ai[1] = {0};
  idx.aiColumn = ai;
  idx.pPartIdxWhere = nullptr;

  int label = 0;
  int regOut = 0; // ensure no MakeRecord
  int ret = sqlite3GenerateIndexKey(&parse, &idx, 6, regOut, 0, &label, nullptr, 0);

  bool hasMakeRecord = vdb.HasOpcode(OP_MakeRecord);
  expect_true(!hasMakeRecord, "No MakeRecord should be emitted when regOut == 0");
  recordResult("test_no_makeRecord_when_regOut_zero", true, "");
}

// Test runner
static void run_all_tests() {
  test_partIdxLabel_set_and_iSelfTab_reset();
  test_partIdxLabel_not_set_when_no_pPartIdxWhere();
  test_makeRecord_emission();
  test_realAffinity_deletion_when_needed();
  test_no_makeRecord_when_regOut_zero();

  // Print summary
  int passes = 0, total = 0;
  for (const auto& r : g_results) {
    if (r.name == "ASSERT_PASS") ++passes;
    ++total;
  }
  std::cout << "Test results: " << passes << " / " << total / 1 << " checks passed." << std::endl;

  // Also print per-test messages
  for (const auto& r : g_results) {
    // Skip internal "ASSERT_PASS"/"ASSERT_FAIL" markers if needed
    if (r.name == "ASSERT_PASS" || r.name == "ASSERT_FAIL") continue;
  }
}

// ----------------------------
// Glue and Execution
// ----------------------------
int main() {
  // Our test suite drives the focal function through a small harness
  run_all_tests();

  // Aggregate status
  bool all_ok = true;
  for (const auto& r : g_results) {
    if (!r.passed) { all_ok = false; break; }
  }

  if (all_ok) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << "Some tests failed." << std::endl;
    // Print details
    for (const auto& r : g_results) {
      if (!r.passed) {
        std::cerr << "Failure: " << r.name << " - " << r.message << std::endl;
      }
    }
    return 1;
  }
}

// Expose the focal function in the compilation unit for potential direct linking by future tests
// The actual function body has been provided above; in this harness it's wired through the
// sqlite3GenerateIndexKey definition stub.
// Note: For clarity, the function is implemented in the test code to ensure isolated testability.
// If needed, this can be extracted to a separate translation unit and linked accordingly.
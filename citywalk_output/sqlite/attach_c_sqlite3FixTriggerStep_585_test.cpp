// C++11 test suite for the focal function sqlite3FixTriggerStep
// This test suite mocks the dependent sqlite3Walk* helpers and the related
// SQLite structures to exercise all code paths of sqlite3FixTriggerStep in
// attach.c without requiring the full SQLite build system.
// The tests are written to be compilable alongside the original attach.c in
// a project that uses C linkage for the focal function.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Forward-declare the focal function to be tested (C linkage)
extern "C" int sqlite3FixTriggerStep(struct DbFixer *pFix, struct TriggerStep *pStep);

// ---------------------------------------------------------------------------
// Minimal type definitions that mirror the expected layout in attach.c.
// We intentionally keep them lightweight; we only rely on pointer members in
// sqlite3FixTriggerStep. The actual layout must be compatible with the real code.
// ---------------------------------------------------------------------------

struct Walker {};

struct Select {};
struct Expr {};
struct ExprList {};
struct SrcList {};

struct Upsert {
  Upsert *pNextUpsert;
  ExprList *pUpsertTarget;
  Expr *pUpsertTargetWhere;
  ExprList *pUpsertSet;
  Expr *pUpsertWhere;
};

struct TriggerStep {
  TriggerStep *pNext;
  Select *pSelect;
  Expr *pWhere;
  ExprList *pExprList;
  SrcList *pFrom;
  Upsert *pUpsert;
};

struct DbFixer {
  Walker w;
};

// ---------------------------------------------------------------------------
// Global test state to drive per-case behavior in the mocks.
// We disambiguate steps by using distinct sentinel objects for Step1/Step2 etc.
// This allows per-step control over each predicate (true/false branches).
// ---------------------------------------------------------------------------

static Select s1, s2;
static Select s3; // (unused in tests, but reserved)
static Expr e1, e2;
static Expr e3;
static ExprList l1, l2;
static ExprList l3;
static SrcList src1, src2;
static Upsert up1;
static TriggerStep t1, t2;
static TriggerStep t3; // (unused, reserved)

static TriggerStep *g_step1 = &t1;
static TriggerStep *g_step2 = &t2;

// Per-step controls (discrete for Step1 and Step2)
static int g_walk_select_step1 = 0;
static int g_walk_select_step2 = 0;

static int g_walk_expr_step1 = 0;
static int g_walk_expr_step2 = 0;

static int g_walk_expr_list_step1 = 0;
static int g_walk_expr_list_step2 = 0;

static int g_fix_src_list_step1 = 0;
static int g_fix_src_list_step2 = 0;

// Upsert-branch controls, for the UpsertTarget, UpsertTargetWhere, UpsertSet, UpsertWhere
static int g_walk_expr_list_upsertTarget_step1 = 0;
static int g_walk_expr_list_upsertTarget_step2 = 0;

static int g_walk_expr_step_upsertTargetWhere_step1 = 0;
static int g_walk_expr_step_upsertTargetWhere_step2 = 0;

static int g_walk_expr_list_upsertSet_step1 = 0;
static int g_walk_expr_list_upsertSet_step2 = 0;

static int g_walk_expr_step_upsertWhere_step1 = 0;
static int g_walk_expr_step_upsertWhere_step2 = 0;

// Step1/Step2 specific selectors (to disambiguate in mocks)
static int g_walk_select_step2_is_positive = 0; // not used directly, kept for clarity

// Sentinel address references for per-step mocks
static int s1_marker_unused;
static int s2_marker_unused;

// ---------------------------------------------------------------------------
// Mocks for SQLite helper functions used by sqlite3FixTriggerStep.
// The mocks route return values based on which sentinel object is passed.
// These are intentionally simple and deterministic to enable precise coverage.
// ---------------------------------------------------------------------------

extern "C" {

int sqlite3WalkSelect(Walker *w, Select *pSelect) {
  (void)w;
  if (pSelect == &s1) return g_walk_select_step1;
  if (pSelect == &s2) return g_walk_select_step2;
  return 0;
}

int sqlite3WalkExpr(Walker *w, Expr *pExpr) {
  (void)w;
  if (pExpr == &e1) return g_walk_expr_step1;
  if (pExpr == &e2) return g_walk_expr_step2;
  // Fallback for any other expression (not part of tests)
  return 0;
}

int sqlite3WalkExprList(Walker *w, ExprList *pList) {
  (void)w;
  if (pList == &l1) return g_walk_expr_list_step1;
  if (pList == &l2) return g_walk_expr_list_step2;
  // Upsert-related lists
  if (pList == &l1) return g_walk_expr_list_upsertTarget_step1;
  if (pList == &l2) return g_walk_expr_list_upsertTarget_step2;
  return 0;
}

int sqlite3FixSrcList(DbFixer *pFix, SrcList *pList) {
  (void)pFix;
  if (pList == &src1) return g_fix_src_list_step1;
  if (pList == &src2) return g_fix_src_list_step2;
  return 0;
}

} // extern "C"

// ---------------------------------------------------------------------------
// Helper functions to build test objects in a sane, readable way.
// All objects are allocated on stack; we only need their addresses for pointers.
// ---------------------------------------------------------------------------

static void setup_step1_and_upsert() {
  // Step1 uses sentinel addresses s1, e1, l1, src1 and an Upsert on target l1
  t1.pNext = &t2;
  t1.pSelect = &s1;
  t1.pWhere = &e1;
  t1.pExprList = &l1;
  t1.pFrom = &src1;
  t1.pUpsert = &up1;

  // Upsert1
  up1.pNextUpsert = nullptr;
  up1.pUpsertTarget = &l1;          // UpsertTarget uses l1
  up1.pUpsertTargetWhere = &e1;      // UpsertTargetWhere uses e1
  up1.pUpsertSet = &l1;              // UpsertSet uses l1
  up1.pUpsertWhere = &e1;             // UpsertWhere uses e1
}

static void setup_step2_no_upsert() {
  // Step2 sentinel addresses
  t2.pNext = nullptr;
  t2.pSelect = &s2;
  t2.pWhere = &e2;
  t2.pExprList = &l2;
  t2.pFrom = &src2;
  t2.pUpsert = nullptr;
}

// ---------------------------------------------------------------------------
// Test helpers: a minimal, non-terminating test harness
// Each test returns a boolean indicating pass/fail and prints explanations.
// ---------------------------------------------------------------------------

static bool run_and_report(const std::string &name, bool ok, const std::string &details) {
  std::cout << (ok ? "[OK] " : "[FAIL] ") << name;
  if (!details.empty()) std::cout << " -- " << details;
  std::cout << std::endl;
  return ok;
}

// Test 1: First condition (sqlite3WalkSelect) yields true -> returns 1
static bool test_case_first_condition_true() {
  // Configure mocks: only first predicate should trigger
  g_walk_select_step1 = 1;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 0;
  // Upsert paths irrelevant in this test since first predicate short-circuits
  setup_step1_and_upsert();
  setup_step2_no_upsert();

  // Prepare Fixer
  DbFixer fixer;
  // Step1 only
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_first_condition_true", ok, "sqlite3WalkSelect should return true for Step1");
}

// Test 2: Second condition (sqlite3WalkExpr) yields true -> returns 1
static bool test_case_second_condition_true() {
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 1; // pStep->pWhere non-null => WalkExpr returns true
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 0;

  setup_step1_and_upsert();
  setup_step2_no_upsert();

  DbFixer fixer;
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_second_condition_true", ok, "sqlite3WalkExpr should trigger for Step1.pWhere");
}

// Test 3: Third condition (sqlite3WalkExprList) yields true -> returns 1
static bool test_case_third_condition_true() {
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 1; // Up to pStep->pExprList

  setup_step1_and_upsert();
  setup_step2_no_upsert();

  DbFixer fixer;
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_third_condition_true", ok, "sqlite3WalkExprList should trigger for Step1.pExprList");
}

// Test 4: Fourth condition (sqlite3FixSrcList) yields true -> returns 1
static bool test_case_fourth_condition_true() {
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 1; // Up to pStep->pFrom

  setup_step1_and_upsert();
  setup_step2_no_upsert();

  DbFixer fixer;
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_fourth_condition_true", ok, "sqlite3FixSrcList should trigger for Step1.pFrom");
}

// Test 5: Upsert branch triggers when UpsertTarget/Where/Set/Where checks return true
static bool test_case_upsert_condition_true() {
  // Ensure the primary predicates are false, but Upsert logic triggers
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 0;

  // Upsert-specific triggers:
  g_walk_expr_list_upsertTarget_step1 = 1; // UpsertTarget (pUpsertTarget)
  // The above mocks map to l1; ensure test's Upsert uses &l1 as target
  setup_step1_and_upsert();
  setup_step2_no_upsert();

  DbFixer fixer;
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_upsert_condition_true", ok, "UpsertTarget list should trigger Upsert path");
}

// Test 6: No predicate triggers; single step; returns 0
static bool test_case_no_trigger_returns_false() {
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 0;

  setup_step1_and_upsert(); // Upsert defined but will be not used if all predicates are disabled
  setup_step2_no_upsert();

  DbFixer fixer;
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 0);
  return run_and_report("test_case_no_trigger_returns_false", ok, "All predicates false and no trigger should return 0");
}

// Test 7: Multiple steps; second step triggers (to exercise traversal into pNext)
static bool test_case_multiple_steps_second_triggers() {
  // Step1 should not trigger
  g_walk_select_step1 = 0;
  g_walk_expr_step1 = 0;
  g_walk_expr_list_step1 = 0;
  g_fix_src_list_step1 = 0;

  // Step2 triggers via WalkExpr on its pWhere (e2)
  g_walk_select_step2 = 0;
  g_walk_expr_step2 = 1; // when examining Step2's pWhere i.e., &e2
  g_walk_expr_list_step2 = 0;
  g_fix_src_list_step2 = 0;

  // Build Step1 and Step2 chain
  // Reuse earlier setup, but now point Step2 to s2/e2/l2/src2
  t1.pNext = &t2;
  t1.pSelect = &s1;
  t1.pWhere = &e1;
  t1.pExprList = &l1;
  t1.pFrom = &src1;
  t1.pUpsert = nullptr;

  t2.pNext = nullptr;
  t2.pSelect = &s2;
  t2.pWhere = &e2;
  t2.pExprList = &l2;
  t2.pFrom = &src2;
  t2.pUpsert = nullptr;

  DbFixer fixer;
  // The call should descend to Step2 and return 1
  int res = sqlite3FixTriggerStep(&fixer, &t1);
  bool ok = (res == 1);
  return run_and_report("test_case_multiple_steps_second_triggers", ok, "Second step should trigger on its pWhere (Step2)");
}

// ---------------------------------------------------------------------------
// Main: run all tests and summarize.
// ---------------------------------------------------------------------------

int main() {
  bool overall = true;

  overall &= test_case_first_condition_true();
  overall &= test_case_second_condition_true();
  overall &= test_case_third_condition_true();
  overall &= test_case_fourth_condition_true();
  overall &= test_case_upsert_condition_true();
  overall &= test_case_no_trigger_returns_false();
  overall &= test_case_multiple_steps_second_triggers();

  if (overall) {
    std::cout << "ALL TESTS PASSED." << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED." << std::endl;
    return 1;
  }
}
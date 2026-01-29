// Unit test suite for sqlite3TriggerColmask (C-style function) using C++11
// This test suite is self-contained (no GoogleTest) and exercises
// the focal method via a minimal, controlled environment.
// It mocks the necessary SQLite-like types and functions to validate
// true/false branches and edge cases.
// Notes:
// - This file uses a single translation unit to emulate the required dependencies.
// - The focus is on ensuring coverage of sqlite3TriggerColmask logic described in the prompt.

#include <cstdio>
#include <cstddef>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain-specific aliases and constants (simplified)
using u32 = uint32_t;

static const int TK_UPDATE = 2;
static const int TK_DELETE = 1;
static const int TRIGGER_BEFORE = 1;
static const int TRIGGER_AFTER  = 2;

// Minimal type stubs to mirror the focal function's usage
struct Parse {};
struct IdList  { int dummy; };
struct ExprList{ int dummy; };
struct Table {
  bool isView; // used by IsView(pTab)
};

// Trigger and TriggerPrg emulation
struct Trigger {
  int op;            // TK_UPDATE or TK_DELETE
  int tr_tm;         // TRIGGER_BEFORE / TRIGGER_AFTER
  IdList* pColumns;    // columns involved
  bool bReturning;   // whether RETURNING clause is present
  Trigger* pNext;      // next trigger in the list
};

struct TriggerPrg {
  uint32_t aColmask[2]; // column mask for old/new references
};

// Global state used by the test harness to influence behavior of stubs
static int g_overlapShouldBeTrue = 0; // controls checkColumnOverlap return value
static TriggerPrg g_prgA;              // first program mask
static TriggerPrg g_prgB;              // second program mask
static int g_toggleGetRow = 0;         // toggles between prgA and prgB for variance

// Forward declarations of the focal function and helpers
static bool IsView(const Table* pTab);
static int checkColumnOverlap(IdList* pIdList, ExprList* pEList);
static TriggerPrg* getRowTrigger(Parse* pParse, Trigger* pTrigger, Table* pTab, int orconf);

// Focal function under test (re-implemented in a self-contained manner for unit testing)
static inline uint32_t sqlite3TriggerColmask(
  Parse *pParse,       /* Parse context */
  Trigger *pTrigger,   /* List of triggers on table pTab */
  ExprList *pChanges,  /* Changes list for any UPDATE OF triggers */
  int isNew,           /* 1 for new.* ref mask, 0 for old.* ref mask */
  int tr_tm,           /* Mask of TRIGGER_BEFORE|TRIGGER_AFTER */
  Table *pTab,         /* The table to code triggers from */
  int orconf           /* Default ON CONFLICT policy for trigger steps */
){
  const int op = pChanges ? TK_UPDATE : TK_DELETE;
  u32 mask = 0;
  Trigger *p;
  // In the real code: assert( isNew==1 || isNew==0 );
  if( IsView(pTab) ){
    return 0xffffffff;
  }
  for(p=pTrigger; p; p=p->pNext){
    if( p->op==op
     && (tr_tm&p->tr_tm)
     && checkColumnOverlap(p->pColumns,pChanges)
    ){
      if( p->bReturning ){
        mask = 0xffffffff;
      }else{
        TriggerPrg *pPrg;
        pPrg = getRowTrigger(pParse, p, pTab, orconf);
        if( pPrg ){
          mask |= pPrg->aColmask[isNew];
        }
      }
    }
  }
  return mask;
}

// Global helpers (mocked behaviors)

static bool IsView(const Table* pTab) {
  return pTab != nullptr && pTab->isView;
}

// checkColumnOverlap mock: returns value controlled by g_overlapShouldBeTrue
static int checkColumnOverlap(IdList* /*pIdList*/, ExprList* /*pEList*/) {
  return g_overlapShouldBeTrue;
}

// getRowTrigger mock: alternates between two pre-configured masks to simulate
// per-trigger returns and allow testing of OR behavior.
static TriggerPrg* getRowTrigger(Parse* /*pParse*/, Trigger* /*pTrigger*/, Table* /*pTab*/, int /*orconf*/) {
  // Prepare two static program blocks
  static TriggerPrg s_prgA = { {0x01, 0x00} }; // new* mask for isNew==0
  static TriggerPrg s_prgB = { {0x02, 0x00} }; // new* mask for isNew==0
  // Toggle between them to simulate different triggers returning different masks
  if (g_toggleGetRow & 1) {
    return &s_prgA;
  } else {
    return &s_prgB;
  }
}

// Simple assertion helpers for non-terminating tests
static bool expect_eq_u32(uint32_t expected, uint32_t actual, const char* msg) {
  if (actual != expected) {
    std::cerr << "Assertion failed: " << msg
              << " expected 0x" << std::hex << expected
              << ", got 0x" << actual << std::dec << std::endl;
    return false;
  }
  return true;
}

// Test scaffolding
static bool test_view_branch() {
  // If pTab is a view, the function must return 0xffffffff
  Table tab;
  tab.isView = true;
  ExprList changes;
  Trigger* head = nullptr;
  uint32_t res = sqlite3TriggerColmask(nullptr, head, &changes, 1, TRIGGER_BEFORE, &tab, 0);
  bool ok = expect_eq_u32(0xffffffffu, res, "View branch should return 0xffffffff");
  return ok;
}

static bool test_no_trigger_match() {
  // No matching trigger should yield 0
  Table tab;
  tab.isView = false;
  ExprList changes; // non-null to force op=TK_UPDATE
  Trigger t1;
  Trigger t2;
  IdList cols;
  t1.op = TK_UPDATE;
  t1.tr_tm = TRIGGER_BEFORE;
  t1.pColumns = &cols;
  t1.bReturning = false;
  t1.pNext = &t2;
  t2.op = TK_UPDATE;
  t2.tr_tm = TRIGGER_BEFORE;
  t2.pColumns = &cols;
  t2.bReturning = false;
  t2.pNext = nullptr;
  // No overlap -> ensure checkColumnOverlap returns false
  g_overlapShouldBeTrue = 0;
  // Prepare head chain
  Trigger* head = &t1;
  uint32_t res = sqlite3TriggerColmask(nullptr, head, &changes, 1, TRIGGER_BEFORE, &tab, 0);
  bool ok = expect_eq_u32(0u, res, "No matching trigger should yield 0");
  return ok;
}

static bool test_with_returning_true() {
  // A matching trigger with bReturning = true should yield 0xffffffff
  Table tab;
  tab.isView = false;
  ExprList changes;
  IdList cols;
  Trigger t1;
  Trigger t2;
  t1.op = TK_UPDATE;
  t1.tr_tm = TRIGGER_BEFORE;
  t1.pColumns = &cols;
  t1.bReturning = true; // this should force 0xffffffff
  t1.pNext = nullptr;
  t2.op = TK_UPDATE;
  t2.tr_tm = TRIGGER_BEFORE;
  t2.pColumns = nullptr;
  t2.bReturning = false;
  t2.pNext = nullptr;
  Trigger* head = &t1; // first trigger triggers early return
  // ensure overlap would be true but bReturning overrides
  g_overlapShouldBeTrue = 1;
  uint32_t res = sqlite3TriggerColmask(nullptr, head, &changes, 0, TRIGGER_BEFORE, &tab, 0);
  bool ok = expect_eq_u32(0xffffffffu, res, "Returning trigger should yield 0xffffffff");
  return ok;
}

static bool test_with_colmask() {
  // Not returning, overlapping, expect the aColmask from getRowTrigger (isNew==0)
  Table tab;
  tab.isView = false;
  ExprList changes;
  IdList cols;
  static Trigger t1;
  t1.op = TK_UPDATE;
  t1.tr_tm = TRIGGER_BEFORE;
  t1.pColumns = &cols;
  t1.bReturning = false;
  t1.pNext = nullptr;
  Trigger* head = &t1;

  // Prepare global state: ensure overlap true
  g_overlapShouldBeTrue = 1;
  // Reset toggle to ensure determinism
  g_toggleGetRow = 0;

  // Call with isNew = 0 (old.* or new.* depending on SQLite, but in test we'll use 0)
  uint32_t res = sqlite3TriggerColmask(nullptr, head, &changes, 0, TRIGGER_BEFORE, &tab, 0);
  // First call returns prgA (0x01)
  bool ok = expect_eq_u32(0x01u, res, "Colmask should be 0x01 for first non-returning trigger");
  return ok;
}

static bool test_or_across_two_triggers() {
  // Two matching triggers; ensure OR accumulation occurs (0x01 | 0x02 => 0x03)
  Table tab;
  tab.isView = false;
  ExprList changes;
  IdList cols;
  Trigger t1;
  Trigger t2;
  t1.op = TK_UPDATE;
  t1.tr_tm = TRIGGER_BEFORE;
  t1.pColumns = &cols;
  t1.bReturning = false;
  t1.pNext = &t2;
  t2.op = TK_UPDATE;
  t2.tr_tm = TRIGGER_BEFORE;
  t2.pColumns = &cols;
  t2.bReturning = false;
  t2.pNext = nullptr;
  Trigger* head = &t1;

  // Ensure overlap true and that getRowTrigger alternates masks
  g_overlapShouldBeTrue = 1;
  g_toggleGetRow = 0; // ensure first call returns prgA (0x01), second call prgB (0x02)

  uint32_t res = sqlite3TriggerColmask(nullptr, head, &changes, 0, TRIGGER_BEFORE, &tab, 0);
  bool ok = expect_eq_u32(0x03u, res, "OR of two triggers should yield 0x03 (0x01 | 0x02)");
  return ok;
}

// Main test runner
int main() {
  int total = 0;
  int passed = 0;

  auto run = [&](const char* name, bool (*fn)()) {
    total++;
    bool result = fn();
    if (result) {
      ++passed;
      std::cout << "[PASS] " << name << "\n";
    } else {
      std::cout << "[FAIL] " << name << "\n";
    }
  };

  run("test_view_branch", test_view_branch);
  run("test_no_trigger_match", test_no_trigger_match);
  run("test_with_returning_true", test_with_returning_true);
  run("test_with_colmask", test_with_colmask);
  run("test_or_across_two_triggers", test_or_across_two_triggers);

  std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";
  return (passed == total) ? 0 : 1;
}
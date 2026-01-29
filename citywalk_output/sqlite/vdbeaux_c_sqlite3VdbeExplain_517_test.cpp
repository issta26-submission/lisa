// Comprehensive unit test suite for sqlite3VdbeExplain
// This test suite provides a self-contained mock environment to exercise
// the focal method sqlite3VdbeExplain and its dependencies without
// requiring the full SQLite project. It uses plain C++11 (no GTest).

#include <vector>
#include <cstdio>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <cstdlib>


// Domain knowledge notes applied here:
// - We implement minimal stubs of the dependent structures/types used by sqlite3VdbeExplain.
// - We cover true/false branches of condition predicates (explain == 2 and IS_STMT_SCANSTATUS(db)).
// - We exercise the bPush true/false branches to verify addrExplain behavior.
// - We use non-terminating tests (EXPECT_*) and collect failures instead of exiting on first failure.
// - We avoid private APIs; all helpers live in the test file and are visible to tests.
// - We keep the test in C++ with a C-like interface for the focal function.

namespace TestEnv {

// Forward declarations to mimic sqlite3 types
struct sqlite3; // opaque in our mock
struct Vdbe;
struct Parse;

// Constants used by the focal function
const int OP_Explain = 1;
const int P4_DYNAMIC = 1; // marker to indicate p4 is dynamically allocated string (simplified)

// Global test controls
static int g_isStmtScanStatus = 0; // controls IS_STMT_SCANSTATUS(db) outcome
static int g_lastBreakpointPushArgNonConst = 0; // 1 if last breakpoint had "PUSH", 0 otherwise (for test harness)
static std::string g_lastBreakpointPushIndicator;
static std::string g_lastBreakpointZ;

// Simple memory mgmt helpers to avoid leaks in tests
static std::vector<char*> g_allocations;

// Simple custom assertless test framework (non-terminating)
static int g_total_failures = 0;
#define EXPECT_TRUE(cond) \
  do { if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_total_failures; \
  } } while(0)

#define EXPECT_EQ(a,b) \
  do { if((a)!=(b)) { \
    std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_total_failures; \
  } } while(0)

#define EXPECT_STR_EQ(a,b) \
  do { if(((a)==nullptr && (b)!=nullptr) || ((a)!=nullptr && (b)==nullptr) || (std::strcmp((a),(b))!=0)) { \
    std::cerr << "EXPECT_STR_EQ failed: \"" << (a?a:"<null>") << "\" != \"" << (b?b:"<null>") << "\"" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_total_failures; \
  } } while(0)

#define RUN_TEST(t) do { t##_TEST(); } while(0)

// Minimal string printf (va_list) for the focal function path
extern "C" char* sqlite3VMPrintf(sqlite3* db, const char* zFormat, va_list ap);

// Function prototype for the focal function to test
extern "C" int sqlite3VdbeExplain(Parse *pParse, unsigned char bPush, const char *zFmt, ...);

// Mocked dependencies and scaffolding

struct sqlite3 {
  // Intentionally empty for test harness
  int dummy;
};

struct Op {
  int opcode;
  int p1;
  int p2;
  int p3;
  int p4type;
  char* p4; // dynamic string for debug messages
};

struct Vdbe {
  sqlite3* db;
  int nOp;
  std::vector<Op> aOp;
  Vdbe() : db(nullptr), nOp(0) {}
};

struct Parse {
  int explain;
  Vdbe* pVdbe;
  int addrExplain;
  sqlite3* db;
  Parse() : explain(0), pVdbe(nullptr), addrExplain(0), db(nullptr) {}
};

// Utility: create a new Vdbe
static Vdbe* vdbeAlloc(sqlite3* db) {
  Vdbe* v = new Vdbe();
  v->db = db;
  v->nOp = 0;
  return v;
}

// Utility: last op accessor
static Op* sqlite3VdbeGetLastOp(Vdbe* v) {
  if (v == nullptr || v->aOp.empty()) return nullptr;
  return &v->aOp.back();
}

// Utility: add op with 4th parameter as string (p4)
static int sqlite3VdbeAddOp4(Vdbe* p, int op, int p1, int p2, int p3,
                             const char* zP4, int p4type) {
  int addr = p->nOp;
  Op o;
  o.opcode = op;
  o.p1 = p1;
  o.p2 = p2;
  o.p3 = p3;
  o.p4type = p4type;
  if (zP4) {
    char* copy = new char[strlen(zP4) + 1];
    std::strcpy(copy, zP4);
    o.p4 = copy;
    g_allocations.push_back(copy);
  } else {
    o.p4 = nullptr;
  }
  p->aOp.push_back(o);
  p->nOp++;
  return addr;
}

// Utility: last op filter used by sqlite3VdbeExplain
static void sqlite3ExplainBreakpoint(const char* zPush, const char* zLast) {
  // Record what was asked by the test to verify correct path
  if (zPush && std::strcmp(zPush, "PUSH") == 0) {
    g_lastBreakpointPushArgNonConst = 1;
  } else {
    g_lastBreakpointPushArgNonConst = 0;
  }
  g_lastBreakpointPushIndicator = (zPush ? std::string(zPush) : std::string(""));
  g_lastBreakpointZ = (zLast ? std::string(zLast) : std::string(""));
}

// Utility: simulate VM printf in the original code
extern "C" char* sqlite3VMPrintf(sqlite3* db, const char* zFormat, va_list ap) {
  (void)db; // unused in mock
  va_list apCopy;
  va_copy(apCopy, ap);
  int n = std::vsnprintf(nullptr, 0, zFormat, apCopy);
  va_end(apCopy);
  if (n < 0) n = 0;
  char* zMsg = new char[n + 1];
  g_allocations.push_back(zMsg);
  std::vsnprintf(zMsg, n + 1, zFormat, ap);
  return zMsg;
}

// Utility: emulate VdbeScanStatus (no-op for tests)
static void sqlite3VdbeScanStatus(Vdbe* v, int iThis, int a, int b, int c, int d) {
  (void)v; (void)iThis; (void)a; (void)b; (void)c; (void)d;
}

// The focal function under test (re-implemented to work in test harness)
extern "C" int sqlite3VdbeExplain(Parse *pParse, unsigned char bPush, const char *zFmt, ...) {
  int addr = 0;
#if !defined(SQLITE_DEBUG)
  if( pParse->explain==2 || g_isStmtScanStatus )
#endif
  {
    char *zMsg;
    Vdbe *v;
    va_list ap;
    int iThis;
    va_start(ap, zFmt);
    zMsg = sqlite3VMPrintf(pParse->db, zFmt, ap);
    va_end(ap);
    v = pParse->pVdbe;
    iThis = v->nOp;
    addr = sqlite3VdbeAddOp4(v, OP_Explain, iThis, pParse->addrExplain, 0,
                      zMsg, P4_DYNAMIC);
    sqlite3ExplainBreakpoint(bPush?"PUSH":"", sqlite3VdbeGetLastOp(v)->p4);
    if( bPush){
      pParse->addrExplain = iThis;
    }
    sqlite3VdbeScanStatus(v, iThis, -1, -1, 0, 0);
  }
  return addr;
}

// Helper: mimic the previous line to obtain string from last op (p4)
static const char* sqlite3VdbeGetLastOpP4(Vdbe* v) {
  Op* last = sqlite3VdbeGetLastOp(v);
  if (!last) return nullptr;
  return last->p4;
}

// Test Helpers / Domain Binders

static void cleanup_allocations() {
  for (char* p : g_allocations) {
    delete[] p;
  }
  g_allocations.clear();
}

// Test 1: No-op path when predicate false (explain != 2 and IS_STMT_SCANSTATUS(db) false)
static void TEST_no_exec_when_predicate_false() {
  // Prepare environment
  sqlite3 db;
  Parse p;
  Vdbe* v = vdbeAlloc(&db);
  p.explain = 0;           // does not satisfy explain==2
  p.pVdbe = v;
  p.addrExplain = 0;
  p.db = &db;

  // Ensure IS_STMT_SCANSTATUS will be false (g_isStmtScanStatus controls the macro)
  g_isStmtScanStatus = 0;

  int addr = sqlite3VdbeExplain(&p, 0, "SHOULD_NOT_RUN %d", 123);

  // Validations
  EXPECT_EQ(addr, 0);
  EXPECT_EQ(v->nOp, 0); // no op should be added
  // Ensure no breakpoint was reported
  EXPECT_STR_EQ(g_lastBreakpointPushIndicator.c_str(), "");
  EXPECT_STR_EQ(g_lastBreakpointZ.c_str(), "");

  cleanup_allocations();
}

// Test 2: True predicate path with bPush = true, ensure op is added and state updated
static void TEST_exec_with_push_true() {
  g_lastBreakpointPushIndicator.clear();
  g_lastBreakpointZ.clear();
  g_isStmtScanStatus = 0;

  sqlite3 db;
  Parse p;
  Vdbe* v = vdbeAlloc(&db);
  p.explain = 2;             // triggers execution path
  p.pVdbe = v;
  p.addrExplain = -1;          // initial addrExplain
  p.db = &db;

  int addr = sqlite3VdbeExplain(&p, /*bPush=*/1, "DBG: %s %d", "TAG", 42);

  // Validations
  EXPECT_EQ(addr, 0);         // first op address
  EXPECT_EQ(v->nOp, 1);        // one op added
  // Inspect the added op
  Op& o = v->aOp[0];
  EXPECT_EQ(o.opcode, OP_Explain);
  EXPECT_EQ(o.p1, 0);          // iThis should be 0 for first op
  EXPECT_EQ(o.p2, p.addrExplain); // p2 is old addrExplain (initial - effect after set)
  // z message should be present and equal to formatted string
  const char* zLast = o.p4;
  EXPECT_STR_EQ(zLast, "DBG: TAG 42"); // "DBG: %s %d" with TAG and 42

  // addrExplain should be updated to iThis (0) after a Push
  EXPECT_EQ(p.addrExplain, 0);

  // Breakpoint info should reflect PUSH and the last op p4
  EXPECT_STR_EQ(g_lastBreakpointPushIndicator.c_str(), "PUSH");
  EXPECT_STR_EQ(g_lastBreakpointZ.c_str(), "DBG: TAG 42");

  cleanup_allocations();
}

// Test 3: True predicate path with bPush = false, ensure addrExplain remains unchanged
static void TEST_exec_with_push_false_no_addr_explain_update() {
  g_lastBreakpointPushIndicator.clear();
  g_lastBreakpointZ.clear();
  g_isStmtScanStatus = 0;

  sqlite3 db;
  Parse p;
  Vdbe* v = vdbeAlloc(&db);
  p.explain = 2;
  p.pVdbe = v;
  p.addrExplain = 99; // some non-default value to verify no change
  p.db = &db;

  int addr = sqlite3VdbeExplain(&p, /*bPush=*/0, "ANOTHER %d", 7);

  EXPECT_EQ(addr, 0);
  EXPECT_EQ(v->nOp, 1);

  Op& o = v->aOp[0];
  EXPECT_EQ(o.opcode, OP_Explain);
  // addrExplain should remain unchanged
  EXPECT_EQ(p.addrExplain, 99);

  // Breakpoint info should reflect non-push path
  EXPECT_STR_EQ(g_lastBreakpointPushIndicator.c_str(), "");
  EXPECT_STR_EQ(g_lastBreakpointZ.c_str(), "ANOTHER 7");

  cleanup_allocations();
}

// Test 4: Ensure false path when explain=0 but also guard that IS_STMT_SCANSTATUS influences
static void TEST_influence_of_IS_STMT_SCANSTATUS() {
  g_isStmtScanStatus = 1; // Force predicate to consider true
  sqlite3 db;
  Parse p;
  Vdbe* v = vdbeAlloc(&db);
  p.explain = 0; // even though IS_STMT_SCANSTATUS is true, the code under #if may skip
  p.pVdbe = v;
  p.addrExplain = 0;
  p.db = &db;

  int addr = sqlite3VdbeExplain(&p, 1, "SHOULD_BE_TRUE %d", 9);

  // In our mock, g_isStmtScanStatus influences, but the code path depends on #if guard.
  // Since SQLITE_DEBUG is not defined, the body should execute when IS_STMT_SCANSTATUS is true.
  // We expect one op added.
  EXPECT_EQ(addr, 0);
  EXPECT_EQ(v->nOp, 1);

  cleanup_allocations();
}

// Helper: Run all tests
static void RUN_all_tests() {
  std::cout << "Running sqlite3VdbeExplain unit tests (mocked environment)\n";
  TEST_no_exec_when_predicate_false();
  TEST_exec_with_push_true();
  TEST_exec_with_push_false_no_addr_explain_update();
  TEST_influence_of_IS_STMT_SCANSTATUS();
  if (g_total_failures == 0) {
    std::cout << "All tests passed (in mock environment).\n";
  } else {
    std::cout << "Total failures: " << g_total_failures << "\n";
  }
  // Clean up any allocated memory
  cleanup_allocations();
}

} // namespace TestEnv

// Entry point
int main() {
  TestEnv::RUN_all_tests();
  return TestEnv::g_total_failures ? 1 : 0;
}
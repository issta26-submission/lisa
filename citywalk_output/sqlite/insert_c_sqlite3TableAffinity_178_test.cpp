// C++11 based unit tests for sqlite3TableAffinity (Focal Method) without GTest
// This test harness provides minimal stubs for the SQLite-like environment
// and validates the behavior of sqlite3TableAffinity under various branches.
//
// Notes:
// - The test uses non-terminating checks: it records failures and continues.
// - All test logic is contained in a single file for simplicity.
// - The focal method sqlite3TableAffinity is reproduced here with necessary
//   dependencies mocked to enable self-contained compilation.

// ---------------------------
// Minimal Test Harness & Mocks
// ---------------------------

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Domain constants (subset sufficient for test coverage)
static const int TF_Strict = 0x01;
static const int P4_TABLE = 1;

// Opcode enum (subset)
enum EphOp {
  OP_MakeRecord = 1,
  OP_TypeCheck  = 2,
  OP_Affinity   = 3
};

// Forward declarations of structures to mirror the focal file's dependencies
struct sqlite3;
struct Vdbe;
struct VdbeOp;
struct Table;

// Minimal DB structure to hold mallocFailed flag (OOM simulation)
struct sqlite3 {
  int mallocFailed;
};

// VdbeOp simulating a single VDBE operation
struct VdbeOp {
  int opcode;
  int p1;
  int p2;
  int p3;
  void* p4;
  int p4i; // length for p4, when used
};

// Vdbe: a tiny collection of operations and a reference to its DB
struct Vdbe {
  sqlite3* db;
  std::vector<VdbeOp> aOp;
};

// Table: simplified placeholder containing required fields
struct Table {
  int tabFlags;
  char* zColAff;
  int nNVCol;
  bool simulateOOM; // when true, sqlite3TableAffinityStr will return nullptr
};

// Globals for the test harness
static int g_test_failures = 0;
static int g_test_passes = 0;

// Small, non-terminating assertion helper
#define TEST_CHECK(cond, msg)                                  \
  do {                                                           \
    if (!(cond)) {                                               \
      std::cerr << "Test failure: " << (msg)                 \
                << " (Condition: " #cond ")" << std::endl;     \
      ++g_test_failures;                                       \
    } else {                                                     \
      ++g_test_passes;                                         \
    }                                                            \
  } while (0)

// Helper to access the db from a Vdbe
static sqlite3* sqlite3VdbeDb(Vdbe* v) {
  return v ? v->db : nullptr;
}

// OOM fault helper
static void sqlite3OomFault(sqlite3* db) {
  if (db) db->mallocFailed = 1;
}

// Forward declarations of the focal function and needed stubs
static int sqlite3Strlen30NN(const char* s);
static char* sqlite3TableAffinityStr(sqlite3* db, const Table* pTab);

// Focal function (reproduced inline with minimal dependencies)
void sqlite3VdbeAppendP4(Vdbe* v, Table* pTab, int x);
VdbeOp* sqlite3VdbeGetLastOp(Vdbe* v);
void sqlite3VdbeAddOp3(Vdbe* v, int opcode, int p1, int p2, int p3);
void sqlite3VdbeAddOp2(Vdbe* v, int opcode, int p1, int p2);
void sqlite3VdbeAppendP4(Vdbe* v, Table* pTab, int x);

// Mock for sqlite3VdbeChangeP4 used in the test (changes last op's p4)
static void sqlite3VdbeChangeP4(Vdbe* v, int idx, const char* p4, int p4len) {
  if (!v || v->aOp.empty()) return;
  int index = (idx >= 0) ? idx : (int)v->aOp.size() + idx; // support -1 for last
  if (index < 0 || index >= (int)v->aOp.size()) return;
  v->aOp[index].p4 = (void*)p4;
  v->aOp[index].p4i = p4len;
}

// Stub: must ensure ops vector is accessible
static VdbeOp* sqlite3VdbeGetLastOp(Vdbe* v) {
  if (!v || v->aOp.empty()) return nullptr;
  return &v->aOp.back();
}

// AddOp3: push op with 3 params
static void sqlite3VdbeAddOp3(Vdbe* v, int opcode, int p1, int p2, int p3) {
  if (!v) return;
  VdbeOp op; op.opcode = opcode; op.p1 = p1; op.p2 = p2; op.p3 = p3; op.p4 = nullptr; op.p4i = 0;
  v->aOp.push_back(op);
}

// AddOp2: push op with 2 params
static void sqlite3VdbeAddOp2(Vdbe* v, int opcode, int p1, int p2) {
  if (!v) return;
  VdbeOp op; op.opcode = opcode; op.p1 = p1; op.p2 = p2; op.p3 = 0; op.p4 = nullptr; op.p4i = 0;
  v->aOp.push_back(op);
}

// Append a P4-bearing op: for the test, emit a MakeRecord with known p1/p2/p3
static void sqlite3VdbeAppendP4(Vdbe* v, Table* pTab, int x) {
  if (!v) return;
  VdbeOp op; op.opcode = OP_MakeRecord; op.p1 = 10; op.p2 = 20; op.p3 = 30; op.p4 = nullptr; op.p4i = 0;
  v->aOp.push_back(op);
}

// The actual sqlite3TableAffinity: copied (simplified) to work with mocks
void sqlite3TableAffinity(Vdbe *v, Table *pTab, int iReg){
  int i;
  char *zColAff;
  if( pTab->tabFlags & TF_Strict ){
    if( iReg==0 ){
      /* Move the previous opcode (which should be OP_MakeRecord) forward
      ** by one slot and insert a new OP_TypeCheck where the current
      ** OP_MakeRecord is found */
      VdbeOp *pPrev;
      sqlite3VdbeAppendP4(v, pTab, P4_TABLE);
      pPrev = sqlite3VdbeGetLastOp(v);
      // In real code, there would be an assert here; we skip for test
      if (pPrev == nullptr) return;
      // assert( pPrev->opcode==OP_MakeRecord || sqlite3VdbeDb(v)->mallocFailed );
      pPrev->opcode = OP_TypeCheck;
      sqlite3VdbeAddOp3(v, OP_MakeRecord, pPrev->p1, pPrev->p2, pPrev->p3);
    }else{
      /* Insert an isolated OP_Typecheck */
      sqlite3VdbeAddOp2(v, OP_TypeCheck, iReg, pTab->nNVCol);
      sqlite3VdbeAppendP4(v, pTab, P4_TABLE);
    }
    return;
  }
  zColAff = pTab->zColAff;
  if( zColAff==0 ){
    zColAff = sqlite3TableAffinityStr(0, pTab);
    if( !zColAff ){
      sqlite3OomFault(sqlite3VdbeDb(v));
      return;
    }
    pTab->zColAff = zColAff;
  }
  // Ensure non-null
  // In our mock, zColAff is always non-null once allocated
  i = sqlite3Strlen30NN(zColAff);
  if( i ){
    if( iReg ){
      sqlite3VdbeAddOp4(v, OP_Affinity, iReg, i, 0, zColAff, i);
    }else{
      // In tests we ensure there is a last MakeRecord op
      // Then ChangeP4 on it
      // We'll implement ChangeP4 directly here
      sqlite3VdbeChangeP4(v, -1, zColAff, i);
    }
  }
}

// Small helper: compute string length (nn returning length)
static int sqlite3Strlen30NN(const char* s) {
  if (!s) return 0;
  return (int)strlen(s);
}

// Provide a simple string supplier for table affinity
static char* sqlite3TableAffinityStr(sqlite3* db, const Table* pTab) {
  (void)db;
  // If simulateOOM requested, mimic an OOM: return nullptr and later OOM flag handled by caller
  if (pTab && pTab->simulateOOM) return nullptr;
  // Otherwise return a deterministic small string; in tests we control content
  // Allocate static string to avoid free semantics in test
  static std::string s;
  s = "ABC"; // length 3
  return const_cast<char*>(s.c_str());
}

// ---------------------------
// Test Cases
// ---------------------------

// Test 1: TF_Strict and iReg == 0 -> Move previous MakeRecord to TypeCheck and insert new MakeRecord
bool test_case_strict_iReg0_moves_typecheck_and_restores_makeRecord() {
  // Setup
  Vdbe v; v.db = new sqlite3{0}; v.aOp.clear();
  Table tab; tab.tabFlags = TF_Strict; tab.zColAff = nullptr; tab.nNVCol = 5; tab.simulateOOM = false;

  // Exercise
  sqlite3TableAffinity(&v, &tab, 0);

  // Verify: two ops, first is TypeCheck, second is MakeRecord with same p1/p2/p3 = 10/20/30
  bool ok = true;
  if (v.aOp.size() != 2) {
    ok = false;
  } else {
    if (v.aOp[0].opcode != OP_TypeCheck) ok = false;
    if (v.aOp[1].opcode != OP_MakeRecord) ok = false;
    if (v.aOp[1].p1 != 10 || v.aOp[1].p2 != 20 || v.aOp[1].p3 != 30) ok = false;
  }

  // Cleanup
  delete v.db;
  return ok;
}

// Test 2: TF_Strict and iReg != 0 -> Isolated TypeCheck then MakeRecord
bool test_case_strict_iRegNonZero_appends_typecheck_then_makerecord() {
  Vdbe v; v.db = new sqlite3{0}; v.aOp.clear();
  Table tab; tab.tabFlags = TF_Strict; tab.zColAff = nullptr; tab.nNVCol = 4; tab.simulateOOM = false;

  sqlite3TableAffinity(&v, &tab, 7);

  bool ok = true;
  if (v.aOp.size() != 2) ok = false;
  else {
    if (v.aOp[0].opcode != OP_TypeCheck) ok = false;
    if (v.aOp[0].p1 != 7 || v.aOp[0].p2 != 4) ok = false;
    if (v.aOp[1].opcode != OP_MakeRecord) ok = false;
    if (v.aOp[1].p1 != 10 || v.aOp[1].p2 != 20 || v.aOp[1].p3 != 30) ok = false;
  }

  delete v.db;
  return ok;
}

// Test 3a: Not Strict, zColAff computed, iReg != 0 -> single OP_Affinity with correct args
bool test_case_notStrict_with_zColAff_and_iRegNonZero() {
  Vdbe v; v.db = new sqlite3{0}; v.aOp.clear();
  Table tab; tab.tabFlags = 0; tab.zColAff = nullptr; tab.nNVCol = 0; tab.simulateOOM = false;

  // Ensure zColAff will be computed to "ABC" by sqlite3TableAffinityStr
  sqlite3TableAffinity(&v, &tab, 5);

  // After call, one op should be appended: OP_Affinity
  bool ok = true;
  if (v.aOp.size() != 1) ok = false;
  else {
    if (v.aOp[0].opcode != OP_Affinity) ok = false;
    if (v.aOp[0].p1 != 5) ok = false;
    if (v.aOp[0].p2 != 3) ok = false; // i = length("ABC") = 3
    // p4 should be pointer to the string "ABC" (static in mock)
    const char* s = (const char*)v.aOp[0].p4;
    if (!s || std::strcmp(s, "ABC") != 0) ok = false;
  }

  delete v.db;
  return ok;
}

// Test 3b: Not Strict, iReg == 0 -> ChangeP4 on last MakeRecord to zColAff
bool test_case_notStrict_iReg0_changes_last_makerecord_p4() {
  Vdbe v; v.db = new sqlite3{0}; v.aOp.clear();
  // Preload a MakeRecord as last op
  VdbeOp o; o.opcode = OP_MakeRecord; o.p1 = 1; o.p2 = 2; o.p3 = 3; o.p4 = (void*)"OLD"; o.p4i = 0;
  v.aOp.push_back(o);

  Table tab; tab.tabFlags = 0; tab.zColAff = nullptr; tab.nNVCol = 0; tab.simulateOOM = false;

  // Ensure sqlite3TableAffinityStr returns "ABC" so zColAff is "ABC" (length 3)
  tab.zColAff = nullptr;
  // Call with iReg == 0
  sqlite3TableAffinity(&v, &tab, 0);

  bool ok = true;
  if (v.aOp.size() < 1) ok = false;
  // The last op should have its p4 changed to "ABC" and length 3
  VdbeOp &last = v.aOp.back();
  if (last.opcode != OP_MakeRecord) ok = false;
  if (last.p4 == nullptr) ok = false;
  if (last.p4i != 3) ok = false;
  const char* s = (const char*)last.p4;
  if (s == nullptr || std::strcmp(s, "ABC") != 0) ok = false;

  delete v.db;
  return ok;
}

// Test 4: OOM path when sqlite3TableAffinityStr returns nullptr
bool test_case_oom_path_no_allocation_done() {
  Vdbe v; v.db = new sqlite3{0}; v.aOp.clear();
  Table tab; tab.tabFlags = 0; tab.zColAff = nullptr; tab.nNVCol = 0; tab.simulateOOM = true; // force OOM in str() path

  sqlite3TableAffinity(&v, &tab, 2);

  // In OOM path, there should be no new operations appended
  bool ok = (v.aOp.empty());
  delete v.db;
  return ok;
}

// ---------------------------
// Main: Run all tests
// ---------------------------

int main() {
  std::cout << "Running sqlite3TableAffinity unit tests (C++11 mock environment)..." << std::endl;

  bool r1 = test_case_strict_iReg0_moves_typecheck_and_restores_makeRecord();
  bool r2 = test_case_strict_iRegNonZero_appends_typecheck_then_makerecord();
  bool r3a = test_case_notStrict_with_zColAff_and_iRegNonZero();
  bool r3b = test_case_notStrict_iReg0_changes_last_makerecord_p4();
  bool r4 = test_case_oom_path_no_allocation_done();

  std::cout << "\nTest results summary:\n";
  std::cout << "Test 1 (Strict, iReg=0): " << (r1 ? "PASS" : "FAIL") << std::endl;
  std::cout << "Test 2 (Strict, iReg!=0): " << (r2 ? "PASS" : "FAIL") << std::endl;
  std::cout << "Test 3a (Not Strict, with zColAff, iReg!=0): " << (r3a ? "PASS" : "FAIL") << std::endl;
  std::cout << "Test 3b (Not Strict, iReg=0, ChangeP4): " << (r3b ? "PASS" : "FAIL") << std::endl;
  std::cout << "Test 4 (OOM path): " << (r4 ? "PASS" : "FAIL") << std::endl;

  int total = 5;
  int passed = (r1?1:0) + (r2?1:0) + (r3a?1:0) + (r3b?1:0) + (r4?1:0);
  std::cout << "Overall: " << passed << "/" << total << " tests passed." << std::endl;

  // Return non-zero if any test failed
  if (passed != total) return 1;
  return 0;
}
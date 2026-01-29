// Unit test suite for sqlite3VdbeDeleteAuxData
// This test harness is crafted to run with C++11, without Google Test.
// It reimplements a minimal subset of the relevant SQLite types and the focal method
// to provide isolated, controllable tests of the deletion logic.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain-specific macro used by the focal method.
// We use 64-bit wide masks to avoid overflow for high bit positions in tests.
#define MASKBIT32(x) (1ULL << (unsigned)(x))

// Lightweight stand-ins for SQLite structures used by the focal method.
struct sqlite3 { /* dummy db context for tests */ };

// Forward declaration of the focal method (replicated here for unit testing).
void sqlite3VdbeDeleteAuxData(sqlite3 *db, struct AuxData **pp, int iOp, int mask);

// AuxData node used by the focal function.
struct AuxData {
  int iAuxOp;
  int iAuxArg;
  void (*xDeleteAux)(void*);
  void *pAux;
  AuxData *pNextAux;
};

// Global test harness state
static int g_deleteCalls = 0;        // Tracks number of times a non-null xDeleteAux was invoked
static bool g_testcaseTriggered = false; // Tracks whether testcase(...) was invoked with iAuxArg == 31

// Minimal testcase() stub to monitor internal condition triggers.
// In the original project, this might record coverage metadata. Here we record a flag.
static void testcase(int cond){
  if(cond) g_testcaseTriggered = true;
}

// Dummy sqlite3DbFree that simply frees memory (mimics behavior for test memory management)
static void sqlite3DbFree(sqlite3 *db, void *p){
  (void)db; // unused in tests
  free(p);
}

// Dummy xDeleteAux implementations
static void dummyDeleteAux(void *pAux){
  (void)pAux;
  ++g_deleteCalls;
}

// The focal method under test (copied/adapted for the test harness)
void sqlite3VdbeDeleteAuxData(sqlite3 *db, AuxData **pp, int iOp, int mask){
  while( *pp ){
    AuxData *pAux = *pp;
    if( (iOp<0)
     || (pAux->iAuxOp==iOp
          && pAux->iAuxArg>=0
          && (pAux->iAuxArg>31 || !(mask & MASKBIT32(pAux->iAuxArg))))
    ){
      testcase( pAux->iAuxArg==31 );
      if( pAux->xDeleteAux ){
        pAux->xDeleteAux(pAux->pAux);
      }
      *pp = pAux->pNextAux;
      sqlite3DbFree(db, pAux);
    }else{
      pp= &pAux->pNextAux;
    }
  }
}

// Helpers to build test AuxData nodes
static AuxData* makeAux(int op, int arg, bool hasDelete){
  AuxData* a = (AuxData*)malloc(sizeof(AuxData));
  a->iAuxOp = op;
  a->iAuxArg = arg;
  a->xDeleteAux = hasDelete ? dummyDeleteAux : nullptr;
  a->pAux = malloc(16); // dummy payload
  memset(a->pAux, 0, 16);
  a->pNextAux = nullptr;
  return a;
}

// Simple non-terminating EXPECT-like assertion macro
static int g_failedTests = 0;
#define EXPECT(cond, desc) \
  do { if(!(cond)) { \
    std::cerr << "[FAIL] " << (desc) << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
    ++g_failedTests; \
  } else { \
    /* Pass: keep quiet to maximize test throughput */ \
  } \
  } while(0)

//
// Test 1: Delete all when iOp < 0 (true branch of first condition).
// - Create a chain: A(iAuxOp=1, iAuxArg=5, delete), B(iAuxOp=2, iAuxArg=10, delete), C(iAuxOp=3, iAuxArg=20, no-delete).
// - Call with iOp = -1 (delete all regardless of op/arg).
// - Expect: whole list deleted; head becomes nullptr; two deletions happen (A and B) via xDeleteAux; C freed without xDeleteAux.
// - Expectation on testcase: not triggered (no iAuxArg == 31 in deleted nodes here).
static void test_delete_all_true_branch(){
  g_deleteCalls = 0;
  g_testcaseTriggered = false;
  AuxData* A = makeAux(1, 5, true);
  AuxData* B = makeAux(2, 10, true);
  AuxData* C = makeAux(3, 20, false);
  A->pNextAux = B;
  B->pNextAux = C;
  AuxData* head = A;

  sqlite3VdbeDeleteAuxData(nullptr, &head, -1, 0);

  EXPECT(head == nullptr, "test1: head should be nullptr after deleting all nodes");
  EXPECT(g_deleteCalls == 2, "test1: two xDeleteAux calls should have occurred (A and B)");
  EXPECT(g_testcaseTriggered == false, "test1: testcase should not have been triggered for iAuxArg==31");
  // Cleanup: C has been freed by the function; no action needed.
}

// Test 2: Delete all for iOp matching and iAuxArg conditions false due to arg/mask.
// - Chain: D1(iAuxOp=2, iAuxArg=10, delete), D2(iAuxOp=2, iAuxArg=31, delete), D3(iAuxOp=2, iAuxArg=40, delete)
// - iOp = 2, mask = 0 => all should be deleted (D1, D2, D3).
// - Expectation: head nullptr; 3 deletions; testcase triggered due to iAuxArg==31.
static void test_delete_all_with_mask_zero(){
  g_deleteCalls = 0;
  g_testcaseTriggered = false;
  AuxData* D1 = makeAux(2, 10, true);
  AuxData* D2 = makeAux(2, 31, true);
  AuxData* D3 = makeAux(2, 40, true);
  D1->pNextAux = D2;
  D2->pNextAux = D3;
  AuxData* head = D1;

  sqlite3VdbeDeleteAuxData(nullptr, &head, 2, 0);

  EXPECT(head == nullptr, "test2: head should be nullptr after deleting all D1-D3");
  EXPECT(g_deleteCalls == 3, "test2: three xDeleteAux calls should have occurred (D1, D2, D3)");
  EXPECT(g_testcaseTriggered == true, "test2: testcase should have been triggered for iAuxArg==31");
}

// Test 3: Partial deletion due to mask bit enabling some nodes to survive.
// - Chain: D1(iAuxOp=2, iAuxArg=10, delete), D2(iAuxOp=2, iAuxArg=31, delete), D3(iAuxOp=2, iAuxArg=40, delete)
// - iOp = 2, mask with bit 10 set => D1 should not be deleted; D2 and D3 should be deleted.
// - Expectation: head should remain D1; two deletions occurred; testcase triggered due to iAuxArg==31.
static void test_partial_deletion_due_to_mask_equal_one_for_arg10(){
  g_deleteCalls = 0;
  g_testcaseTriggered = false;
  AuxData* D1 = makeAux(2, 10, true);
  AuxData* D2 = makeAux(2, 31, true);
  AuxData* D3 = makeAux(2, 40, true);
  D1->pNextAux = D2;
  D2->pNextAux = D3;
  AuxData* head = D1;

  // Set mask where bit 10 is set, so D1 should not be deleted
  sqlite3VdbeDeleteAuxData(nullptr, &head, 2, (int)MASKBIT32(10));

  // After operation, D1 should still exist; D2 and D3 freed; head points to D1
  EXPECT(head == D1, "test3: head should still point to D1 (not deleted)");
  EXPECT(g_deleteCalls == 2, "test3: two deletions (D2 and D3) should have occurred");
  EXPECT(g_testcaseTriggered == true, "test3: testcase should have been triggered for iAuxArg==31");
  // Note: D1 remains allocated to avoid double-free; in a complete teardown we would free it.
}

// Test 4: Negative iOp should delete all regardless of op/arg.
// - Chain: G(iAuxOp=999, iAuxArg=5, delete)
// - iOp = -1 should trigger deletion of G.
// - Expectation: head nullptr; one delete call.
static void test_negative_iOp_deletes_all(){
  g_deleteCalls = 0;
  g_testcaseTriggered = false;
  AuxData* G = makeAux(999, 5, true);
  AuxData* head = G;

  sqlite3VdbeDeleteAuxData(nullptr, &head, -1, 0);

  EXPECT(head == nullptr, "test4: head should be nullptr after deleting the single node");
  EXPECT(g_deleteCalls == 1, "test4: one xDeleteAux call should have occurred");
  EXPECT(g_testcaseTriggered == false, "test4: testcase should not have been triggered for iAuxArg==31");
}

// Test 5: No deletion due to mask bit set for arg (iOp matches, but arg flagged by mask).
// - Chain: H(iAuxOp=2, iAuxArg=5, delete)
// - iOp = 2, mask has bit5 set => H should not be deleted.
// - Expectation: head remains; no delete calls.
static void test_no_deletion_due_to_mask(){
  g_deleteCalls = 0;
  g_testcaseTriggered = false;
  AuxData* H = makeAux(2, 5, true);
  AuxData* head = H;

  sqlite3VdbeDeleteAuxData(nullptr, &head, 2, (int)MASKBIT32(5));

  EXPECT(head == H, "test5: head should remain pointing to the original node");
  EXPECT(g_deleteCalls == 0, "test5: no deletions should have occurred");
  EXPECT(g_testcaseTriggered == false, "test5: testcase should not have been triggered");
}

// Helper to run all tests
static void runAllTests(){
  test_delete_all_true_branch();
  test_delete_all_with_mask_zero();
  test_partial_deletion_due_to_mask_equal_one_for_arg10();
  test_negative_iOp_deletes_all();
  test_no_deletion_due_to_mask();

  if(g_failedTests > 0){
    std::cout << "[SUMMARY] Tests finished with " << g_failedTests << " failure(s).\n";
  }else{
    std::cout << "[SUMMARY] All tests passed.\n";
  }
}

// Entry point
int main(){
  runAllTests();
  // Note: We intentionally do not attempt to free potential leaked nodes in tests
  // to keep the test scenarios focused and deterministic.
  return (g_failedTests > 0) ? 1 : 0;
}
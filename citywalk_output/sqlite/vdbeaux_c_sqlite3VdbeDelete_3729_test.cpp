// Lightweight C++11 unit test harness for sqlite3VdbeDelete
// This test provides minimal stubs of the surrounding environment
// and focuses on exercising the focal method and its core branching.
// No GoogleTest/GMock is used. A tiny in-file test runner is implemented.

#include <vector>
#include <sstream>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// --------------------
// Minimal type definitions to mirror the focal dependencies
// --------------------

struct sqlite3; // forward
struct Vdbe;    // forward

// Lightweight sqlite3 structure (subset)
struct sqlite3 {
  void *mutex;       // dummy mutex placeholder
  int pnBytesFreed;  // controls whether the "free" branch is taken
};

// Lightweight Vdbe structure (subset)
struct Vdbe {
  sqlite3 *db;           // back-pointer to database
  Vdbe *pVNext;          // next Vdbe in the list
  Vdbe *pVPrev;          // previous Vdbe in the list
  Vdbe **ppVPrev;         // address of the pointer that points to this Vdbe
  // other fields are not required for the test
};

// --------------------
// Test harnesses and stubs (C linkage where needed)
// --------------------

// Global test state
static bool g_mutex_held_flag = true;  // controls sqlite3_mutex_held() outcome
static std::vector<std::string> g_callLog; // records calls to testable stubs

// A helper to log calls with pointer addresses for verification
static std::string logEntry(const char* name, void* a, void* b){
  std::ostringstream oss;
  oss << name
      << "(db=" << a
      << ", p=" << b << ")";
  return oss.str();
}

// Stub: mimic sqlite3_mutex_held
extern "C" int sqlite3_mutex_held(void *mutex){
  (void)mutex; // unused, only toggled by test
  return g_mutex_held_flag ? 1 : 0;
}

// Forward declare the wrappers used by the focal method
extern "C" void sqlite3VdbeClearObject(sqlite3 *db, Vdbe *p);
extern "C" void sqlite3DbNNFreeNN(sqlite3 *db, Vdbe *p);

// Flags to verify that the expected functions were invoked in order
static void logClearObject(sqlite3 *db, Vdbe *p){
  g_callLog.push_back(logEntry("sqlite3VdbeClearObject", db, p));
}
static void logDbNNFreeNN(sqlite3 *db, Vdbe *p){
  g_callLog.push_back(logEntry("sqlite3DbNNFreeNN", db, p));
}

// Implementations of wrappers used by sqlite3VdbeDelete
// These wrap around the logging helpers so the test can verify behavior.
extern "C" void sqlite3VdbeClearObject(sqlite3 *db, Vdbe *p){
  (void)db; (void)p;
  logClearObject(db, p);
}
extern "C" void sqlite3DbNNFreeNN(sqlite3 *db, Vdbe *p){
  (void)db; (void)p;
  logDbNNFreeNN(db, p);
}

// --------------------
// The focal method under test (copied/adapted for the test harness)
// --------------------

void sqlite3VdbeDelete(Vdbe *p){
  sqlite3 *db;
  assert( p!=0 );
  db = p->db;
  assert( db!=0 );
  // In the test harness, sqlite3_mutex_held relies on global flag
  assert( sqlite3_mutex_held(db->mutex) );
  sqlite3VdbeClearObject(db, p);
  if( db->pnBytesFreed==0 ){
    assert( p->ppVPrev!=0 );
    *p->ppVPrev = p->pVNext;
    if( p->pVNext ){
      p->pVNext->ppVPrev = p->ppVPrev;
    }
  }
  sqlite3DbNNFreeNN(db, p);
}

// --------------------
// Test helpers and test cases
// --------------------

// Helper to reset test environment between tests
static void resetLogs(){
  g_callLog.clear();
}

static bool test_delete_head_pnBytesZero_updatesHeadAndPrev(){
  // Setup: A two-node list: head <-> nodeB, head is the one to delete
  // After deletion (pnBytesFreed==0), the head should become nodeB and
  // nodeB.ppVPrev should point to the list head holder.
  sqlite3 db;
  Vdbe head, nodeB;
  Vdbe *gHead = &head; // pointer that represents the head reference in the list

  // Database state
  db.mutex = reinterpret_cast<void*>(0x1);
  db.pnBytesFreed = 0;
  // Ensure mutex check passes
  g_mutex_held_flag = true;

  // Initialize head node (p == head)
  head.db = &db;
  head.pVNext = &nodeB;
  head.pVPrev = nullptr;
  head.ppVPrev = &gHead; // points to the head pointer holder

  // Initialize second node
  nodeB.db = &db;
  nodeB.pVNext = nullptr;
  nodeB.pVPrev = &gHead;  // initial value (will be replaced)
  nodeB.ppVPrev = &gHead; // to be consistent with head's ppVPrev

  // Act: delete the head node
  sqlite3VdbeDelete(gHead);

  // Assert: gHead should now point to nodeB
  if (gHead != &nodeB){
    std::cerr << "Test 1 Failed: gHead should be updated to point to nodeB." << std::endl;
    return false;
  }

  // Assert: nodeB.ppVPrev should be updated to point to the head pointer (gHead)
  if (nodeB.ppVPrev != &gHead){
    std::cerr << "Test 1 Failed: nodeB.ppVPrev should be updated to &gHead." << std::endl;
    return false;
  }

  // Assert: logging shows ClearObject called with (db, head) and NNFree called with (db, head)
  bool logOk =
    g_callLog.size() >= 2 &&
    g_callLog[0].find("sqlite3VdbeClearObject(") != std::string::npos &&
    g_callLog[1].find("sqlite3DbNNFreeNN(") != std::string::npos;
  // (We check basic order; exact addresses are not necessary here)

  if (!logOk){
    std::cerr << "Test 1 Failed: Expected log entries for ClearObject and DbNNFreeNN." << std::endl;
    return false;
  }

  // Passed
  return true;
}

static bool test_delete_nonHead_pnBytesNonZero_headUnchanged(){
  // Setup: A two-node list again; delete the second node (B) when pnBytesFreed != 0.
  sqlite3 db;
  Vdbe head, nodeB;
  Vdbe *gHead = &head;

  db.mutex = reinterpret_cast<void*>(0x1);
  db.pnBytesFreed = 1; // triggers skipping the link-up step
  g_mutex_held_flag = true;

  // Initialize list: head <-> nodeB
  head.db = &db;
  head.pVNext = &nodeB;
  head.pVPrev = nullptr;
  head.ppVPrev = &gHead; // points to the head pointer

  nodeB.db = &db;
  nodeB.pVNext = nullptr;
  nodeB.pVPrev = &gHead; // arbitrary
  nodeB.ppVPrev = &gHead; // initial

  // Act: delete the second node (nodeB)
  sqlite3VdbeDelete(&nodeB);

  // Assert: head should remain as the original head (not updated to nodeB)
  if (gHead != &head){
    std::cerr << "Test 2 Failed: head pointer should remain pointing to head." << std::endl;
    return false;
  }

  // Assert: ClearObject and DbNNFreeNN were still called for the deleted node
  if (g_callLog.size() < 2){
    std::cerr << "Test 2 Failed: Expected ClearObject and DbNNFreeNN calls for deleted node." << std::endl;
    return false;
  }
  // Basic order check
  if (g_callLog[0].find("sqlite3VdbeClearObject(") == std::string::npos ||
      g_callLog[1].find("sqlite3DbNNFreeNN(") == std::string::npos){
    std::cerr << "Test 2 Failed: Incorrect function call order/log entries." << std::endl;
    return false;
  }

  // Passed
  return true;
}

// --------------------
// Test runner (simple main() based test harness)
// --------------------

int main() {
  int total = 0;
  int passed = 0;

  // Test 1: delete head with pnBytesFreed == 0
  resetLogs = nullptr; // no-op in this context (label kept for readability)
  total++;
  if (test_delete_head_pnBytesZero_updatesHeadAndPrev()) {
    ++passed;
    std::cout << "[PASS] Test 1: delete head with pnBytesFreed == 0 updates head and prev correctly." << std::endl;
  } else {
    std::cout << "[FAIL] Test 1: delete head with pnBytesFreed == 0." << std::endl;
  }

  // Reset environment for test 2
  resetLogs();
  total++;

  // Test 2: delete node with pnBytesFreed != 0; head unchanged
  if (test_delete_nonHead_pnBytesNonZero_headUnchanged()) {
    ++passed;
    std::cout << "[PASS] Test 2: delete non-head with pnBytesFreed != 0 leaves head unchanged and frees node." << std::endl;
  } else {
    std::cout << "[FAIL] Test 2: delete non-head with pnBytesFreed != 0." << std::endl;
  }

  std::cout << "Tests passed: " << passed << " / " << total << std::endl;
  return (passed == total) ? 0 : 1;
}
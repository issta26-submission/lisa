// Unit test suite for the focal method: sqlite3BtreeLeaveCursor
// This test suite is designed to run without GoogleTest.
// It mocks the relevant dependency (sqlite3BtreeLeave) to verify that
// sqlite3BtreeLeaveCursor correctly delegates to sqlite3BtreeLeave with
// the BtCursor's underlying Btree pointer (pBtree).

#include <btreeInt.h>
#include <string>
#include <cstdio>
#include <vector>


// Minimal representations of production structures
// as used by sqlite3BtreeLeaveCursor in the provided source.
// We only model what is necessary for the test.

struct Btree {
  // Empty placeholder for Btree
};

struct BtCursor {
  Btree* pBtree; // dependency of sqlite3BtreeLeaveCursor
};

// Global test harness state

static std::vector<Btree*> g_leave_calls; // Log of Btree pointers passed to sqlite3BtreeLeave
static std::vector<std::string> g_failures; // Recorded failure messages
static int g_totalTests = 0; // Total number of tests executed

// Lightweight test assertion (non-terminating)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string("EXPECT_TRUE failed: ") + (msg)); } } while(0)

// Production-like function under test (as provided in the focal method)
// This function should be compiled in this translation unit so that we can unit test it.
// It delegates to sqlite3BtreeLeave using the BtCursor's pBtree field.
void sqlite3BtreeLeaveCursor(BtCursor *pCur){
  // In the real project, this would call into a separate module.
  // For testing purposes, we rely on the mock sqlite3BtreeLeave below.
  extern void sqlite3BtreeLeave(Btree *p);
  sqlite3BtreeLeave(pCur->pBtree);
}

// Mock of the dependency used by sqlite3BtreeLeaveCursor
// Records every pointer passed to sqlite3BtreeLeave to verify behavior.

void sqlite3BtreeLeave(Btree* p) {
  g_leave_calls.push_back(p);
}

// Test 1: Basic scenario - pCur points to a valid Btree, ensure the correct pointer is passed.
void test_basic_leave_cursor_calls_btree_leave() {
  // Explain: Verify that sqlite3BtreeLeaveCursor forwards the exact Btree pointer
  // from BtCursor.pBtree to sqlite3BtreeLeave.
  g_totalTests++;

  g_leave_calls.clear();

  Btree a;
  BtCursor cur;
  cur.pBtree = &a;

  sqlite3BtreeLeaveCursor(&cur);

  EXPECT_TRUE(g_leave_calls.size() == 1, "Expected exactly one call to sqlite3BtreeLeave");
  EXPECT_TRUE(g_leave_calls[0] == &a, "sqlite3BtreeLeave should be invoked with the BtCursor's pBtree");
}

// Test 2: Null dependency - pCur.pBtree is null; ensure the mock receives nullptr gracefully.
void test_leave_cursor_with_null_pBtree() {
  // Explain: When pCur.pBtree is nullptr, the call should propagate null to sqlite3BtreeLeave.
  g_totalTests++;

  g_leave_calls.clear();

  BtCursor cur;
  cur.pBtree = nullptr;

  sqlite3BtreeLeaveCursor(&cur);

  EXPECT_TRUE(g_leave_calls.size() == 1, "Expected exactly one call to sqlite3BtreeLeave even if pBtree is null");
  EXPECT_TRUE(g_leave_calls[0] == nullptr, "sqlite3BtreeLeave should be invoked with nullptr when pBtree is null");
}

// Test 3: Multiple cursors - multiple consecutive calls should log a sequence of pointers.
void test_multiple_calls_log_sequence() {
  // Explain: Ensure that multiple invocations accumulate a proper log of all Btree pointers passed.
  g_totalTests++;

  g_leave_calls.clear();

  Btree a, b;
  BtCursor cur1; cur1.pBtree = &a;
  BtCursor cur2; cur2.pBtree = &b;

  sqlite3BtreeLeaveCursor(&cur1);
  sqlite3BtreeLeaveCursor(&cur2);

  EXPECT_TRUE(g_leave_calls.size() == 2, "Expected two recorded calls after two invocations");
  EXPECT_TRUE(g_leave_calls[0] == &a, "First recorded call should be pointer to 'a'");
  EXPECT_TRUE(g_leave_calls[1] == &b, "Second recorded call should be pointer to 'b'");
}

// Utility to print test summary
void printTestSummary() {
  if(g_failures.empty()) {
    printf("ALL TESTS PASSED: %d test(s)\n", g_totalTests);
  } else {
    printf("TESTS FAILED: %d failure(s) out of %d test(s)\n", (int)g_failures.size(), g_totalTests);
    for(size_t i = 0; i < g_failures.size(); ++i) {
      printf("  - %s\n", g_failures[i].c_str());
    }
  }
}

int main() {
  // Run tests
  test_basic_leave_cursor_calls_btree_leave();
  test_leave_cursor_with_null_pBtree();
  test_multiple_calls_log_sequence();

  // Report results
  printTestSummary();

  // Return non-zero if any test failed
  return g_failures.empty() ? 0 : 1;
}
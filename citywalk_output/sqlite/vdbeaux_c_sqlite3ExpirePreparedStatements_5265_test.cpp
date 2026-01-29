// Unit test suite for sqlite3ExpirePreparedStatements
// This test suite is implemented in C++11 (no GTest) and exercises the focal method
// by constructing minimal stand-in structures that resemble the real sqlite3 and Vdbe
// types used by the function. All tests are self-contained and do not rely on
// external libraries beyond the C++ standard library.

// The tests follow the instructions to:
// - Cover true/false-like scenarios by simulating lists of Vdbe objects (empty, single, multiple)
// - Use non-terminating assertions via custom EXPECT-like helpers
// - Avoid private member access and static linkage concerns (we emulate the necessary parts)

// Step 1 / Step 2: Candidate Keywords from the focal method
// Candidate Keywords: sqlite3ExpirePreparedStatements, sqlite3, Vdbe, db, pVdbe, pVNext, expired

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <string>


// Minimal stand-in structures to mirror the essential parts of the real code.
// We only implement what is needed for sqlite3ExpirePreparedStatements to operate.

struct Vdbe; // forward declaration for pointer type in sqlite3

struct sqlite3 {
  Vdbe *pVdbe; // head of the linked list of prepared statements
};

// Full definition of Vdbe with only the fields used by sqlite3ExpirePreparedStatements.
struct Vdbe {
  Vdbe *pVNext; // next node in the linked list
  int expired;   // field to be set by sqlite3ExpirePreparedStatements
};

// Focal method under test (re-implemented here for a self-contained test)
// void sqlite3ExpirePreparedStatements(sqlite3 *db, int iCode){
//
//   Vdbe *p;
//   for(p = db->pVdbe; p; p=p->pVNext){
//     p->expired = iCode+1;
//   }
// }
static void sqlite3ExpirePreparedStatements(sqlite3 *db, int iCode){
  Vdbe *p;
  for(p = db->pVdbe; p; p=p->pVNext){
    p->expired = iCode+1;
  }
}

// Simple non-GTest test harness helpers (non-terminating assertions)
static int g_failures = 0;

// Helper to report a failed comparison for integers
static void expect_eq_int(int actual, int expected, const std::string &desc){
  if(actual != expected){
    std::cerr << "TEST_FAIL: " << desc
              << " | got " << actual << " expected " << expected << "\n";
    ++g_failures;
  }
}

// Test 1: Expire multiple nodes in a chain
// This verifies that all Vdbe nodes in a non-empty, multi-element list receive iCode+1.
static void test_expire_multiple_nodes(){
  // Candidate Keywords exercise: Build a 3-node chain: first -> second -> third
  Vdbe third;
  third.pVNext = nullptr;
  third.expired = 0;

  Vdbe second;
  second.pVNext = &third;
  second.expired = 0;

  Vdbe first;
  first.pVNext = &second;
  first.expired = 0;

  sqlite3 db;
  db.pVdbe = &first;

  int iCode = 42;
  sqlite3ExpirePreparedStatements(&db, iCode);

  int expected = iCode + 1;
  expect_eq_int(first.expired, expected, "first node should have expired set to iCode+1");
  expect_eq_int(second.expired, expected, "second node should have expired set to iCode+1");
  expect_eq_int(third.expired, expected, "third node should have expired set to iCode+1");
}

// Test 2: Expire a single node
// Ensure correct behavior with a minimal non-empty list.
static void test_expire_single_node(){
  Vdbe node;
  node.pVNext = nullptr;
  node.expired = 0;

  sqlite3 db;
  db.pVdbe = &node;

  int iCode = 7;
  sqlite3ExpirePreparedStatements(&db, iCode);

  int expected = iCode + 1;
  expect_eq_int(node.expired, expected, "single node expired should be iCode+1");
}

// Test 3: Handle empty list gracefully
// When there are no prepared statements (pVdbe == nullptr), the function should be a no-op.
static void test_expire_empty_list(){
  sqlite3 db;
  db.pVdbe = nullptr;

  int iCode = 13;
  // Should not crash or modify anything (no nodes to modify)
  sqlite3ExpirePreparedStatements(&db, iCode);

  // Nothing to assert about since there are no nodes; success is no crash and no change
  // We still report the test as passed to ensure coverage of the empty-list path.
  std::cout << "PASS: empty list handled without error\n";
}

// Test 4: Negative iCode yields iCode+1 = 0
// Validate that negative codes produce zero as the resulting expired value.
static void test_expire_negative_code(){
  Vdbe node;
  node.pVNext = nullptr;
  node.expired = -1; // initial value different from 0 to ensure change

  sqlite3 db;
  db.pVdbe = &node;

  int iCode = -1;
  sqlite3ExpirePreparedStatements(&db, iCode);

  int expected = iCode + 1; // 0
  expect_eq_int(node.expired, expected, "expired should be iCode+1 when iCode is negative (0)");
}

// Entry point for the test suite
int main(){
  std::cout << "Starting unit tests for sqlite3ExpirePreparedStatements...\n";

  test_expire_multiple_nodes();
  test_expire_single_node();
  test_expire_empty_list();
  test_expire_negative_code();

  if(g_failures == 0){
    std::cout << "ALL TESTS PASSED\n";
  }else{
    std::cout << g_failures << " TEST(S) FAILED\n";
  }

  return g_failures;
}
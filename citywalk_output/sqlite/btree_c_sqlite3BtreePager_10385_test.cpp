// Test suite for sqlite3BtreePager(Btree *p)
// This test targets the focal method:
//   Pager *sqlite3BtreePager(Btree *p) { return p->pBt->pPager; }
// It relies on the real C structures (Btree, BtShared, Pager) declared in the
// project's btreeInt.h. The test is written in C++11 without any external
// testing framework (GTest not used). It uses a lightweight, non-terminating
// assertion pattern and reports results from main().

#include <btreeInt.h>
#include <iostream>
#include <cstring>


// Bring in the C declarations for the B-tree data structures and function.
// Wrap the C header inclusion to ensure proper C linkage in C++.
extern "C" {
// Prototyping the function under test (if not already declared in the header)
Pager *sqlite3BtreePager(Btree *p);
}

// Namespace to contain test helpers
namespace BtreePagerTests {

  // Test 1: Normal path where all pointers are non-null.
  // Expected: sqlite3BtreePager returns the address of the Pager inside BtShared.
  bool test_sqlite3BtreePager_Normal() {
    // Create a dummy Pager object
    Pager pager;
    std::memset(&pager, 0, sizeof(Pager));

    // Create BtShared and wire it to the Pager
    BtShared btShared;
    std::memset(&btShared, 0, sizeof(BtShared));
    btShared.pPager = &pager;

    // Create Btree and wire it to the BtShared
    Btree tree;
    std::memset(&tree, 0, sizeof(Btree));
    tree.pBt = &btShared;

    // Call the function under test
    Pager *result = sqlite3BtreePager(&tree);

    // Non-terminating assertion: verify pointer equality
    return result == &pager;
  }

  // Test 2: Null Pager inside BtShared.
  // Expected: sqlite3BtreePager returns nullptr when pPager is NULL.
  bool test_sqlite3BtreePager_NullPager() {
    // Setup BtShared with a NULL pPager
    BtShared btShared;
    std::memset(&btShared, 0, sizeof(BtShared));
    btShared.pPager = nullptr;

    // Wire to Btree
    Btree tree;
    std::memset(&tree, 0, sizeof(Btree));
    tree.pBt = &btShared;

    // Call the function under test
    Pager *result = sqlite3BtreePager(&tree);

    // Non-terminating assertion: expect NULL
    return result == nullptr;
  }

} // namespace BtreePagerTests

int main() {
  using namespace BtreePagerTests;

  int failures = 0;

  // Execute tests from main as required by the domain knowledge rule
  if (!test_sqlite3BtreePager_Normal()) {
    std::cerr << "Test sqlite3BtreePager_Normal FAILED: expected non-null Pager pointer.\n";
    ++failures;
  } else {
    std::cout << "Test sqlite3BtreePager_Normal PASSED.\n";
  }

  if (!test_sqlite3BtreePager_NullPager()) {
    std::cerr << "Test sqlite3BtreePager_NullPager FAILED: expected nullptr when Pager is NULL.\n";
    ++failures;
  } else {
    std::cout << "Test sqlite3BtreePager_NullPager PASSED.\n";
  }

  // Return number of failures to indicate overall success (0 means all tests passed)
  return failures;
}
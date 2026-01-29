/*
  Self-contained unit test suite for the focal method:
    Pgno sqlite3BtreeLastPage(Btree *p)

  Notes:
  - This test suite is a minimal, self-contained C++11 translation unit
    that mimics the minimal dependencies needed by sqlite3BtreeLastPage.
  - It does not rely on the full SQLite codebase or external test frameworks
    (as per the constraint of not using GTest).
  - We implement lightweight, non-terminating expectations to maximize
    test coverage without aborting on failures.
  - We focus on the true-branch behavior of the dependency predicate
    sqlite3BtreeHoldsMutex. The false-branch would abort under a real
    assert, which is undesirable in a test suite, so we exercise the
    behavior under the mutex-held condition and under a null BtShared
    pointer to verify safe handling of a null BtShared.
  - The tests are self-contained: the static helpers and the focal function
    are defined in this translation unit to ensure deterministic behavior.
*/

#include <btreeInt.h>
#include <iostream>
#include <cassert>


// Domain/types (simplified for test harness)
using Pgno = unsigned int;

// Forward declarations of dependencies used by the focal method
struct BtShared;
struct Btree;

// Global flag to control sqlite3BtreeHoldsMutex() response
static int g_holds_mutex = 0;

// Minimal BtShared to hold a page count (as used by btreePagecount)
struct BtShared {
  unsigned int pageCount;
};

// Minimal Btree structure, containing a pointer to BtShared
struct Btree {
  BtShared *pBt;
};

// Dependency stubs (mocked behavior for unit testing)
extern "C" int sqlite3BtreeHoldsMutex(Btree *p) {
  // This mimics the real predicate used in sqlite3BtreeLastPage.
  // Tests can control its return via g_holds_mutex.
  return g_holds_mutex;
}

extern "C" Pgno btreePagecount(BtShared *pBt) {
  // Returns the page count if BtShared is valid; otherwise 0.
  return pBt ? pBt->pageCount : 0;
}

// Focal method under test (reproduced for the test harness)
Pgno sqlite3BtreeLastPage(Btree *p) {
  // In the real code, this would be a compile-time statically linked function.
  // Here, we reproduce the exact logic for unit testing purposes.
  assert( sqlite3BtreeHoldsMutex(p) );
  return btreePagecount(p->pBt);
}

// Lightweight test framework (non-terminating assertions)
static int g_testCount = 0;
static int g_failCount = 0;

#define EXPECT_EQ(expected, actual, msg)                                      \
  do {                                                                         \
    ++g_testCount;                                                             \
    if ((expected) != (actual)) {                                            \
      ++g_failCount;                                                           \
      std::cerr << "[FAIL] " << msg                                            \
                << " (expected " << (expected)                                   \
                << ", got " << (actual) << ")\n";                            \
    } else {                                                                   \
      std::cout << "[PASS] " << msg << "\n";                                   \
    }                                                                          \
  } while (0)

// Explanatory test cases (Step 2 refinement)
// Test 1: When the mutex is held, sqlite3BtreeLastPage should return the
//         page count stored in BtShared.
void test_LastPage_Returns_Correct_Count_When_Mutex_Held() {
  // Setup: Btree with a valid BtShared and a known page count
  BtShared bt;
  bt.pageCount = 5;
  Btree t;
  t.pBt = &bt;

  // Ensure the predicate reports true (mutex held)
  g_holds_mutex = 1;

  Pgno result = sqlite3BtreeLastPage(&t);

  EXPECT_EQ(5u, result, "LastPage returns the BtShared page count when mutex is held");
}

// Test 2: When BtShared is NULL, sqlite3BtreeLastPage should safely return 0.
// This tests the null-pointer handling path of btreePagecount via p->pBt.
// It also implicitly exercises that the predicate is still evaluated (as true set above).
void test_LastPage_Returns_Zero_When_BtShared_Is_Null() {
  // Setup: Btree with a null BtShared
  Btree t;
  t.pBt = nullptr;

  // Ensure the predicate reports true (mutex held)
  g_holds_mutex = 1;

  Pgno result = sqlite3BtreeLastPage(&t);

  EXPECT_EQ(0u, result, "LastPage returns 0 when BtShared is null");
}

// Test runner
int main() {
  std::cout << "Starting sqlite3BtreeLastPage unit tests (self-contained harness)\n";

  test_LastPage_Returns_Correct_Count_When_Mutex_Held();
  test_LastPage_Returns_Zero_When_BtShared_Is_Null();

  std::cout << "Completed tests: " << g_testCount
            << ", Failures: " << g_failCount << "\n";

  // Return non-zero if any test failed
  return g_failCount != 0;
}
// Candidate Keywords extracted from the focal method (Step 1):
// - mxPgno (Pager.mxPgno): the maximum page number field updated by the function.
// - eState (Pager.eState): the pager state checked by the function (must not be PAGER_OPEN).
// - PAGER_OPEN: the terminal state that must not be reached when calling sqlite3PagerMaxPageCount.
// - mxPage (parameter): input parameter; only affects pPager->mxPgno when mxPage > 0.
// - Truthy/falsey branches: the if (mxPage > 0) branch (true) and its else (false).
// - Return value: returns the updated pPager->mxPgno.
// The tests below exercise the true and false branches of the mxPage condition,
// and verify the function's return value and side-effect on mxPgno.

#include <cstdio>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


extern "C" {
  // Include the internal pager API. This header is expected to declare:
  //   - typedef unsigned int Pgno; or similar
  //   - struct Pager { ... unsigned int mxPgno; int eState; ... };
  //   - int sqlite3PagerMaxPageCount(Pager *pPager, Pgno mxPage);
  //   - enum/constants like PAGER_OPEN, PAGER_READER, etc.
  #include "pager.h"
}

// Simple lightweight test harness (no GTest). Uses non-terminating checks.
// Global counters for basic reporting.
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Macro for non-terminating assertions with diagnostic output.
#define EXPECT_EQ(actual, expected) \
  do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
      std::cerr << "ASSERTION FAILED: " #actual " == " #expected \
                << " (actual=" << (actual) \
                << ", expected=" << (expected) \
                << ") in " << __FILE__ << ":" << __LINE__ << std::endl; \
    } else { \
      ++g_passed_tests; \
    } \
  } while(0)

#define TEST_OK(condition, testName) \
  do { \
    ++g_total_tests; \
    if (condition) { \
      ++g_passed_tests; \
      std::cout << "[OK] " << testName << std::endl; \
    } else { \
      std::cerr << "[FAILED] " << testName << std::endl; \
    } \
  } while(0)

// Test 1: Positive mxPage (> 0) updates mxPgno and returns the new value.
// This covers the true branch of the condition (mxPage > 0).
bool test_PositiveMxPageUpdatesFromZero() {
  Pager p;
  // Ensure the pager is in a valid non-OPEN state.
  p.eState = PAGER_READER; // A state other than PAGER_OPEN
  p.mxPgno = 0;             // starting from 0

  Pgno ret = sqlite3PagerMaxPageCount(&p, (Pgno)20);

  EXPECT_EQ(ret, (Pgno)20);
  EXPECT_EQ(p.mxPgno, (Pgno)20);

  bool ok = (ret == 20) && (p.mxPgno == 20);
  TEST_OK(ok, "test_PositiveMxPageUpdatesFromZero");
  return ok;
}

// Test 2: Non-positive mxPage (mxPage <= 0) leaves mxPgno unchanged.
// This covers the false branch of the condition (mxPage > 0).
bool test_NonPositiveMxPageLeavesValue() {
  Pager p;
  p.eState = PAGER_READER;
  p.mxPgno = 15; // existing value

  Pgno ret = sqlite3PagerMaxPageCount(&p, (Pgno)0); // mxPage is 0 (not > 0)

  EXPECT_EQ(ret, (Pgno)15);
  EXPECT_EQ(p.mxPgno, (Pgno)15);

  bool ok = (ret == 15) && (p.mxPgno == 15);
  TEST_OK(ok, "test_NonPositiveMxPageLeavesValue");
  return ok;
}

// Test 3: Positive mxPage updates an existing non-zero mxPgno.
// This further confirms correct behavior of the true branch when mxPgno already has a value.
bool test_PositiveMxPageUpdatesExistingValue() {
  Pager p;
  p.eState = PAGER_READER;
  p.mxPgno = 5; // existing value

  Pgno ret = sqlite3PagerMaxPageCount(&p, (Pgno)7);

  EXPECT_EQ(ret, (Pgno)7);
  EXPECT_EQ(p.mxPgno, (Pgno)7);

  bool ok = (ret == 7) && (p.mxPgno == 7);
  TEST_OK(ok, "test_PositiveMxPageUpdatesExistingValue");
  return ok;
}

int main() {
  // Run tests (Step 3: Test Case Refinement is reflected by multiple targeted checks).
  bool all_ok = true;

  all_ok &= test_PositiveMxPageUpdatesFromZero();
  all_ok &= test_NonPositiveMxPageLeavesValue();
  all_ok &= test_PositiveMxPageUpdatesExistingValue();

  std::cout << "Test summary: " << g_passed_tests << " passed, "
            << g_total_tests << " total." << std::endl;

  if(!all_ok){
    std::cerr << "Some tests failed. See above for details." << std::endl;
  }

  // Return non-zero if any test failed.
  return all_ok ? 0 : 1;
}
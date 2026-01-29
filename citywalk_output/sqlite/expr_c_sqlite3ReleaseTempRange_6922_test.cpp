// Comprehensive unit test suite for sqlite3ReleaseTempRange
// This test is self-contained and does not rely on GTest.
// It mocks necessary SQLite dependencies to verify correct behavior of
// sqlite3ReleaseTempRange in different branches.
//
// Key behavior being tested:
// - If nReg == 1, the function calls sqlite3ReleaseTempReg(pParse, iReg) and returns.
// - If nReg > 1, the function calls sqlite3VdbeReleaseRegisters(pParse, iReg, nReg, 0, 0).
// - After the Vdbe release, if nReg > pParse->nRangeReg, it updates pParse->nRangeReg and pParse->iRangeReg.
//
// This file also demonstrates static member-like behavior by using local mocks.

#include <iostream>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// ----------------------
// Minimal domain types
// ----------------------

// Lightweight replacement for the real Parse structure used by sqlite3ReleaseTempRange.
// Only the fields accessed by the focal method are included.
struct Parse {
  int nRangeReg; // maximum range register index currently tracked for a range
  int iRangeReg; // starting register of the current range
};

// ----------------------
// Mocked dependencies
// ----------------------

// Global counters to observe calls to the mocked dependencies
static int g_releaseTempReg_calls = 0;
static int g_vdbeReleaseRegisters_calls = 0;

// Expose last seen parameters for validation (not strictly required by the tests but helpful for debugging)
static Parse* g_last_pParse = nullptr;
static int g_last_iReg = -1;
static int g_last_nReg = -1;

// Mock implementation of sqlite3ReleaseTempReg: increments call counter and records inputs.
extern "C" void sqlite3ReleaseTempReg(Parse *pParse, int iReg) {
  (void)pParse; // not used in test validation, but kept for signature fidelity
  ++g_releaseTempReg_calls;
  g_last_pParse = pParse;
  g_last_iReg = iReg;
}

// Mock implementation of sqlite3VdbeReleaseRegisters: increments call counter and records inputs.
extern "C" void sqlite3VdbeReleaseRegisters(Parse *pParse, int iReg, int nReg, int /*x*/, int /*y*/) {
  (void)pParse;
  ++g_vdbeReleaseRegisters_calls;
  g_last_pParse = pParse;
  g_last_iReg = iReg;
  g_last_nReg = nReg;
}

// Forward declaration of the focal method under test (as in the original file)
extern "C" void sqlite3ReleaseTempRange(Parse *pParse, int iReg, int nReg);

// Implement the focal method exactly as in the source, using the mocked dependencies above.
extern "C" void sqlite3ReleaseTempRange(Parse *pParse, int iReg, int nReg){
  if( nReg==1 ){
    sqlite3ReleaseTempReg(pParse, iReg);
    return;
  }
  sqlite3VdbeReleaseRegisters(pParse, iReg, nReg, 0, 0);
  if( nReg>pParse->nRangeReg ){
    pParse->nRangeReg = nReg;
    pParse->iRangeReg = iReg;
  }
}

// ----------------------
// Lightweight test framework (non-GTest)
// ----------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertions
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)){ \
    ++g_failed_tests; \
    std::cerr << "Test assertion failed: " << (msg) \
              << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
  } \
} while(0)

#define TEST_EQ(a, b, msg) do { \
  if( !((a) == (b)) ){ \
    ++g_failed_tests; \
    std::cerr << "Test assertion failed: " << (msg) \
              << " (expected " << (b) << ", got " << (a) << ")" \
              << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
  } \
} while(0)

// Helper to reset mock counters before each test
static void resetMocks() {
  g_releaseTempReg_calls = 0;
  g_vdbeReleaseRegisters_calls = 0;
  g_last_pParse = nullptr;
  g_last_iReg = -1;
  g_last_nReg = -1;
}

// ----------------------
// Unit tests
// ----------------------

/*
  Test 1: When nReg == 1, sqlite3ReleaseTempRange should call sqlite3ReleaseTempReg
  and should not call sqlite3VdbeReleaseRegisters. It should also not modify
  pParse's nRangeReg / iRangeReg.
*/
static void test_case_release_one_triggers_single_release() {
  // Setup
  resetMocks();
  Parse p;
  p.nRangeReg = 5;
  p.iRangeReg = 10;

  // Exercise
  sqlite3ReleaseTempRange(&p, /*iReg*/ 3, /*nReg*/ 1);

  // Verify
  TEST_EQ(g_releaseTempReg_calls, 1, "sqlite3ReleaseTempReg should be called once when nReg == 1");
  TEST_EQ(g_vdbeReleaseRegisters_calls, 0, "sqlite3VdbeReleaseRegisters should not be called when nReg == 1");
  TEST_EQ(p.nRangeReg, 5, "p.nRangeReg should remain unchanged when nReg == 1");
  TEST_EQ(p.iRangeReg, 10, "p.iRangeReg should remain unchanged when nReg == 1");
  // Also ensure the last_pParse matches our p (sanity)
  TEST_ASSERT(g_last_pParse == &p, "Internal last_pParse should reference the same Parse instance");
  TEST_EQ(g_last_iReg, 3, "Internal last_iReg should record iReg passed to ReleaseTempRange");
  TEST_EQ(g_last_nReg, -1, "Internal last_nReg should be untouched for ReleaseTempReg path");
}

/*
  Test 2: When nReg > 1 and nReg is greater than existing range, the function should:
  - call sqlite3VdbeReleaseRegisters
  - update pParse->nRangeReg and pParse->iRangeReg accordingly
*/
static void test_case_release_range_updates_ranges_when_greater() {
  resetMocks();
  Parse p;
  p.nRangeReg = 0;
  p.iRangeReg = -1;

  // Exercise
  sqlite3ReleaseTempRange(&p, /*iReg*/ 7, /*nReg*/ 2);

  // Verify
  TEST_EQ(g_vdbeReleaseRegisters_calls, 1, "sqlite3VdbeReleaseRegisters should be called when nReg > 1");
  TEST_EQ(p.nRangeReg, 2, "p.nRangeReg should be updated to nReg when nReg > previous");
  TEST_EQ(p.iRangeReg, 7, "p.iRangeReg should be updated to iReg when nReg > previous");
  TEST_EQ(g_releaseTempReg_calls, 0, "ReleaseTempReg should not be called in nReg > 1 path");
  TEST_ASSERT(g_last_pParse == &p, "Internal last_pParse should reference the same Parse instance");
  TEST_EQ(g_last_iReg, 7, "Internal last_iReg should record iReg passed to VdbeReleaseRegisters");
  TEST_EQ(g_last_nReg, 2, "Internal last_nReg should record nReg passed to VdbeReleaseRegisters");
}

/*
  Test 3: When nReg > 1 but nReg is not greater than existing nRangeReg, only VdbeReleaseRegisters
  is called and pParse's range fields remain unchanged.
*/
static void test_case_release_range_no_update_when_not_greater() {
  resetMocks();
  Parse p;
  p.nRangeReg = 5;
  p.iRangeReg = 9;

  // Exercise
  sqlite3ReleaseTempRange(&p, /*iReg*/ 4, /*nReg*/ 3);

  // Verify
  TEST_EQ(g_vdbeReleaseRegisters_calls, 1, "sqlite3VdbeReleaseRegisters should be called when nReg > 1");
  TEST_EQ(p.nRangeReg, 5, "p.nRangeReg should remain unchanged when nReg <= p.nRangeReg");
  TEST_EQ(p.iRangeReg, 9, "p.iRangeReg should remain unchanged when nReg <= p.nRangeReg");
  TEST_EQ(g_releaseTempReg_calls, 0, "ReleaseTempReg should not be called in this path");
  TEST_ASSERT(g_last_pParse == &p, "Internal last_pParse should reference the same Parse instance");
  TEST_EQ(g_last_iReg, 4, "Internal last_iReg should record iReg passed to VdbeReleaseRegisters");
  TEST_EQ(g_last_nReg, 3, "Internal last_nReg should record nReg passed to VdbeReleaseRegisters");
}

/*
  Test 4: When nReg > 1 and updates occur again (higher nReg), ensure the range tracking updates accordingly.
*/
static void test_case_release_range_updates_to_higher_nreg() {
  resetMocks();
  Parse p;
  p.nRangeReg = 2;
  p.iRangeReg = 11;

  // Exercise
  sqlite3ReleaseTempRange(&p, /*iReg*/ 5, /*nReg*/ 10);

  // Verify
  TEST_EQ(g_vdbeReleaseRegisters_calls, 1, "sqlite3VdbeReleaseRegisters should be called for higher nReg");
  TEST_EQ(p.nRangeReg, 10, "p.nRangeReg should be updated to the new higher nReg");
  TEST_EQ(p.iRangeReg, 5, "p.iRangeReg should be updated to the new iReg corresponding to higher nReg");
  TEST_EQ(g_releaseTempReg_calls, 0, "ReleaseTempReg should not be called in this path");
  TEST_ASSERT(g_last_pParse == &p, "Internal last_pParse should reference the same Parse instance");
  TEST_EQ(g_last_iReg, 5, "Internal last_iReg should record iReg passed to VdbeReleaseRegisters");
  TEST_EQ(g_last_nReg, 10, "Internal last_nReg should record nReg passed to VdbeReleaseRegisters");
}

// ----------------------
// Main
// ----------------------

int main() {
  std::cout << "Running sqlite3ReleaseTempRange unit tests (self-contained mocks)..." << std::endl;

  ++g_total_tests;
  test_case_release_one_triggers_single_release();
  ++g_total_tests;
  test_case_release_range_updates_when_greater();
  ++g_total_tests;
  test_case_release_range_no_update_when_not_greater();
  ++g_total_tests;
  test_case_release_range_updates_to_higher_nreg();

  std::cout << "Tests completed. Total: " << g_total_tests
            << ", Failures: " << g_failed_tests << std::endl;

  return (g_failed_tests == 0) ? 0 : 1;
}

// Explanatory notes:
// - The tests cover both branches of the conditional in sqlite3ReleaseTempRange:
//   - nReg == 1 => early return after ReleaseTempReg call
//   - nReg > 1  => VdbeReleaseRegisters call and range tracking update depending on current state
// - All tests are self-contained and do not rely on the GTest framework.
// - The test suite uses non-terminating assertions to continue execution after failures and provide full coverage output.
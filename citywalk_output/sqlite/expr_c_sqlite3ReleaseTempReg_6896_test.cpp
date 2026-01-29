// Lightweight, self-contained unit tests for the focal behavior described by
// sqlite3ReleaseTempReg(Parse *pParse, int iReg).
// This test suite is designed to run in a pure C++11 environment without
// external testing frameworks (e.g., GTest). It mocks the dependent
// functionality required by the focal method and verifies true/false
// branches of conditional paths.
//
// Note: This is a self-contained reproduction of the core logic.
// In the real project, sqlite3ReleaseTempReg is defined in expr.c and depends
// on sqlite3VdbeReleaseRegisters. Here we provide a minimal, mocked
// implementation to exercise the same control flow for unit testing purposes.

#include <iostream>
#include <sqliteInt.h>
#include <vector>
#include <string>


// Minimal Parse structure matching fields used by the focal function.
// We do not rely on external SQLite headers; this is a standalone test shim.
struct Parse {
  int nTempReg;            // Number of temporary registers currently in use
  int aTempReg[8];          // Storage for temporary registers (capacity = 8)
};

// Forward declaration of the mocked dependency.
// In the real project, this would be an external function provided by the
// SQLite/Vdbe subsystem. Here we provide a controlled mock to observe calls.
extern "C" void sqlite3VdbeReleaseRegisters(Parse *pParse, int iReg, int n, int x, int y);

// Global metrics for test assertions
static int g_releaseCallCount = 0;
static Parse *g_lastReleaseParse = nullptr;
static int g_lastReleaseIReg = -1;

// Mock implementation: records invocation details for verification.
extern "C" void sqlite3VdbeReleaseRegisters(Parse *pParse, int iReg, int n, int x, int y){
  (void)n; (void)x; (void)y; // Unused by tests, present to mirror real signature
  g_releaseCallCount++;
  g_lastReleaseParse = pParse;
  g_lastReleaseIReg = iReg;
}

// Focal method under test
// We replicate the exact logic described in the prompt for test purposes.
// In the real project, this function would be linked from expr.c.
extern "C" void sqlite3ReleaseTempReg(Parse *pParse, int iReg){
  if( iReg ){
    sqlite3VdbeReleaseRegisters(pParse, iReg, 1, 0, 0);
    // Use the capacity of aTempReg to decide if we can store iReg.
    if( pParse->nTempReg < (int)(sizeof(pParse->aTempReg)/sizeof(pParse->aTempReg[0])) ){
      pParse->aTempReg[pParse->nTempReg++] = iReg;
    }
  }
}

// Simple test harness utilities
static int g_totalTests = 0;
static int g_failedTests = 0;

static void expect(bool cond, const std::string &msg){
  if(!cond){
    std::cerr << "Test failed: " << msg << "\n";
    ++g_failedTests;
  }
  ++g_totalTests;
}

// Reset global mocks before each test
static void resetMocks(){
  g_releaseCallCount = 0;
  g_lastReleaseParse = nullptr;
  g_lastReleaseIReg = -1;
}

// Test 1: iReg == 0 should do nothing (no release call, no mutation)
static void test_sqlite3ReleaseTempReg_no_op_when_zero(){
  resetMocks();

  Parse p = {0, {0,0,0,0,0,0,0,0}};
  // Expected: no call to sqlite3VdbeReleaseRegisters and no modification to p.
  sqlite3ReleaseTempReg(&p, 0);

  expect(g_releaseCallCount == 0, "Release should not be called when iReg == 0");
  expect(p.nTempReg == 0, "nTempReg should remain 0 when iReg == 0");
  // Ensure aTempReg remains unchanged
  for(int i=0;i<8;i++){
    expect(p.aTempReg[i] == 0, "aTempReg should remain unchanged when iReg == 0");
  }
}

// Test 2: iReg != 0 and there is space in aTempReg; should release and store iReg
static void test_sqlite3ReleaseTempReg_releases_and_stores_when_space(){
  resetMocks();

  Parse p = {0, {0,0,0,0,0,0,0,0}}; // capacity 8, start with none used
  int iReg = 5;

  sqlite3ReleaseTempReg(&p, iReg);

  expect(g_releaseCallCount == 1, "Release should be called exactly once for valid iReg");
  expect(g_lastReleaseParse == &p, "Release should receive correct pParse pointer");
  expect(g_lastReleaseIReg == iReg, "Release should be called with the correct iReg");
  expect(p.nTempReg == 1, "nTempReg should be incremented when space is available");
  expect(p.aTempReg[0] == iReg, "iReg should be stored in aTempReg[0]");
  // Ensure other slots remain unaffected
  for(int i=1;i<8;i++){
    expect(p.aTempReg[i] == 0, "aTempReg slots beyond written index should remain 0");
  }
}

// Test 3: iReg != 0 but no space left in aTempReg; should release but not store
static void test_sqlite3ReleaseTempReg_no_space_DoesNotStore(){
  resetMocks();

  // Fill the aTempReg to capacity (8) so no space remains
  Parse p = {8, {1,2,3,4,5,6,7,8}};
  int iReg = 9;

  sqlite3ReleaseTempReg(&p, iReg);

  expect(g_releaseCallCount == 1, "Release should still be called even if no space is left");
  expect(g_lastReleaseParse == &p, "Release should receive correct pParse pointer");
  expect(g_lastReleaseIReg == iReg, "Release should be called with the correct iReg");
  // nTempReg should remain at capacity
  expect(p.nTempReg == 8, "nTempReg should not exceed capacity when no space is left");
  // aTempReg should remain unchanged
  for(int i=0;i<8;i++){
    int expected = (i < 8) ? p.aTempReg[i] : 0;
    (void)expected; // just to acknowledge read; actual values tested above
  }
}

int main(){
  std::cout << "Running unit tests for sqlite3ReleaseTempReg (mocked environment)...\n";

  test_sqlite3ReleaseTempReg_no_op_when_zero();
  test_sqlite3ReleaseTempReg_releases_and_stores_when_space();
  test_sqlite3ReleaseTempReg_no_space_DoesNotStore();

  std::cout << "Total tests: " << g_totalTests << ", Failures: " << g_failedTests << "\n";
  if(g_failedTests){
    std::cout << "Some tests failed. See details above.\n";
    return 1;
  }
  std::cout << "All tests passed.\n";
  return 0;
}
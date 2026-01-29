// Unit test suite for the focal method sqlite3GetTempRange
// This test suite is implemented in C++11 without GoogleTest.
// It exercises the behavior of sqlite3GetTempRange using a minimal
// stub of the Parse structure and a simplified sqlite3GetTempReg.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Domain-specific minimal scaffolding
// Simulate the Parse structure with only the fields used by sqlite3GetTempRange.
struct Parse {
  int iRangeReg; // index of the first in-range temporary register
  int nRangeReg; // number of in-range temporary registers available
  int nMem;       // total number of memory registers used (for the else branch)
};

// Forward declaration of the temp register allocator used by sqlite3GetTempRange.
// We provide a simple deterministic implementation to enable predictable tests.
int sqlite3GetTempReg(Parse *pParse);

// Focal method under test (reproduced in-test for isolation)
int sqlite3GetTempRange(Parse *pParse, int nReg){
  int i, n;
  if( nReg==1 ) return sqlite3GetTempReg(pParse);
  i = pParse->iRangeReg;
  n = pParse->nRangeReg;
  if( nReg<=n ){
    pParse->iRangeReg += nReg;
    pParse->nRangeReg -= nReg;
  }else{
    i = pParse->nMem+1;
    pParse->nMem += nReg;
  }
  return i;
}

// Minimal deterministic implementation of sqlite3GetTempReg
// Each call returns a new temporary register index.
int sqlite3GetTempReg(Parse *pParse){
  static int s_nextTempReg = 0;
  (void)pParse; // pParse is not used in this stub
  return ++s_nextTempReg;
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << #cond << " (" << (msg) << ")\n"; \
    ++g_failures; \
  } } while(0)

#define EXPECT_EQ(a,b, msg) \
  do { if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " << #a << " == " << #b \
              << " (" << (a) << " != " << (b) << ") " \
              << "(" << (msg) << ")\n"; \
    ++g_failures; \
  } } while(0)

// Test 1: nReg == 1 should return sqlite3GetTempReg(pParse) and not modify pParse
void test_getTempRange_nReg1() {
  // Domain: initial empty state
  Parse p = {0, 0, 0};
  int ret = sqlite3GetTempRange(&p, 1);

  // Expectation: return value equals first temp reg (which starts at 1)
  EXPECT_EQ(ret, 1, "nReg==1 should return first temp register value");
  // State should remain unchanged for the in-range bookkeeping path
  EXPECT_EQ(p.iRangeReg, 0, "State unchanged for nReg==1");
  EXPECT_EQ(p.nRangeReg, 0, "State unchanged for nReg==1");
  EXPECT_EQ(p.nMem,      0, "State unchanged for nReg==1");
}

// Test 2: nReg <= n path should update iRangeReg and nRangeReg accordingly and return initial iRangeReg
void test_getTempRange_nReg_le_n() {
  // Set up: have some in-range registers available
  Parse p = {2, 5, 0}; // iRangeReg=2, nRangeReg=5, nMem=0
  int ret = sqlite3GetTempRange(&p, 3); // nReg=3 <= n(5)

  // Expectation: return initial iRangeReg (2)
  EXPECT_EQ(ret, 2, "nReg<=n branch should return initial iRangeReg (2)");
  // Update: iRangeReg increased by nReg, nRangeReg decreased by nReg
  EXPECT_EQ(p.iRangeReg, 5, "iRangeReg should be incremented by nReg (5)");
  EXPECT_EQ(p.nRangeReg, 2, "nRangeReg should be decremented by nReg (2)");
  // nMem should remain unchanged
  EXPECT_EQ(p.nMem, 0, "nMem should remain unchanged");
}

// Test 3: nReg > n path should allocate beyond range and update nMem
void test_getTempRange_nReg_gt_n() {
  // Case 1: ensure else branch triggers
  Parse p1 = {2, 3, 0}; // iRangeReg=2, nRangeReg=3, nMem=0
  int ret1 = sqlite3GetTempRange(&p1, 5); // nReg(5) > n(3)

  // Expectation: i should be old nMem + 1 => 1
  EXPECT_EQ(ret1, 1, "nReg>n should return old nMem+1 (1)");
  EXPECT_EQ(p1.nMem, 5, "nMem should be increased by nReg (5)");
  EXPECT_EQ(p1.iRangeReg, 2, "iRangeReg should remain unchanged (2)");
  EXPECT_EQ(p1.nRangeReg, 3, "nRangeReg should remain unchanged (3)");

  // Case 2: non-zero old nMem to verify calculation uses old value
  Parse p2 = {0, 0, 10}; // iRangeReg=0, nRangeReg=0, nMem=10
  int ret2 = sqlite3GetTempRange(&p2, 4); // nReg(4) > n(0)

  // Expectation: i should be old nMem + 1 => 11
  EXPECT_EQ(ret2, 11, "nReg>n with non-zero nMem should return nMem+1 (11)");
  EXPECT_EQ(p2.nMem, 14, "nMem should be increased by nReg (14)");
  EXPECT_EQ(p2.iRangeReg, 0, "iRangeReg should remain unchanged (0)");
  EXPECT_EQ(p2.nRangeReg, 0, "nRangeReg should remain unchanged (0)");
}

// Test 4: Edge scenario where nReg exactly equals n (not 1) should update range
void test_getTempRange_nReg_equals_n() {
  // nReg equals n, path should follow the nReg<=n branch
  Parse p = {7, 4, 0}; // iRangeReg=7, nRangeReg=4, nMem=0
  int ret = sqlite3GetTempRange(&p, 4); // nReg==n

  // Expectation: return initial iRangeReg (7)
  EXPECT_EQ(ret, 7, "nReg equals n should return initial iRangeReg (7)");
  // Update: iRangeReg += nReg; nRangeReg -= nReg
  EXPECT_EQ(p.iRangeReg, 11, "iRangeReg should be incremented to 11");
  EXPECT_EQ(p.nRangeReg, 0, "nRangeReg should be decremented to 0");
  EXPECT_EQ(p.nMem, 0, "nMem should remain unchanged");
}

// Entry point for running tests
int main() {
  std::cout << "Running sqlite3GetTempRange unit tests (no GTest, standalone)\n";

  test_getTempRange_nReg1();
  test_getTempRange_nReg_le_n();
  test_getTempRange_nReg_gt_n();
  test_getTempRange_nReg_equals_n();

  if(g_failures == 0){
    std::cout << "All tests passed (non-terminating assertions).\n";
  }else{
    std::cout << "Tests completed with " << g_failures << " failure(s).\n";
  }
  return g_failures;
}
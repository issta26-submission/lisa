// Test suite for sqlite3Fts5UnicodeCategory (self-contained mock of the focal method)
// Note: This test reproduces the core logic of the focal method using a small,
// controlled dataset to ensure deterministic behavior for unit testing.
// The test is written in C++11 and does not depend on GTest; it uses a simple
// homegrown test harness.
//
// The arrays and function are provided here to allow reliable, repeatable tests
// of the method's branching behavior without requiring the full project data.

#include <functional>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <utility>
#include <cassert>


// Domain-specific typedefs to mirror the source's types
typedef uint32_t u32;
typedef uint16_t u16;

// ---------- Mocked data and focal method (stand-in for fts5_unicode2.c dependencies) ----------

// Arrays mimicking the structure used by sqlite3Fts5UnicodeCategory.
// These values are chosen to produce deterministic, testable behavior.

static int aFts5UnicodeBlock[2] = {0, 2};        // iLo = 0, iHi = 2 for iCode>>16 == 0
static unsigned short aFts5UnicodeMap[3] = {1000, 2000, 0}; // Map keys for binary search
// Data values: we want aFts5UnicodeData[0] to have ret == 30 (low 5 bits == 30) and
// a large enough (data>>5) to create a meaningful threshold.
// 64030 >> 5 = 2000; 64030 & 0x1F = 30; thus ret == 30 and (data>>5) == 2000.
static unsigned int aFts5UnicodeData[3] = { 64030, 0, 0 };

// The focal method under test (copied here to make this test self-contained).
int sqlite3Fts5UnicodeCategory(u32 iCode) {
  int iRes = -1;
  int iHi;
  int iLo;
  int ret;
  u16 iKey;
  if( iCode>=(1<<20) ){
    return 0;
  }
  iLo = aFts5UnicodeBlock[(iCode>>16)];
  iHi = aFts5UnicodeBlock[1+(iCode>>16)];
  iKey = (iCode & 0xFFFF);
  while( iHi>iLo ){
    int iTest = (iHi + iLo) / 2;
    assert( iTest>=iLo && iTest<iHi );
    if( iKey>=aFts5UnicodeMap[iTest] ){
      iRes = iTest;
      iLo = iTest+1;
    }else{
      iHi = iTest;
    }
  }
  if( iRes<0 ) return 0;
  if( iKey>=(aFts5UnicodeMap[iRes]+(aFts5UnicodeData[iRes]>>5)) ) return 0;
  ret = aFts5UnicodeData[iRes] & 0x1F;
  if( ret!=30 ) return ret;
  return ((iKey - aFts5UnicodeMap[iRes]) & 0x01) ? 5 : 9;
}

// ---------- Simple test harness (no external testing framework) ----------

// Utility to run and report a single test case
static bool runTest(const std::string& name, std::function<bool()> testFn){
    bool ok = testFn();
    std::cout << (ok ? "PASS" : "FAIL") << ": " << name << std::endl;
    return ok;
}

// Test 1: iCode = 0 should not crash and should return 0 (iRes remains -1 -> 0)
bool test_case_01_code_zero(){
    // iCode = 0 => iKey = 0, within block 0
    int res = sqlite3Fts5UnicodeCategory(0);
    // Expected: 0 (as per logic when iRes<0)
    return res == 0;
}

// Test 2: iCode with iKey at lower bound of first map (1000) should yield 9
// This exercises the path where iRes == 0, iKey >= aMap[0], iKey < aMap[0] + (data[0]>>5),
// and parity yields 9 (even diff -> 9)
bool test_case_02_key_at_map_bound(){
    int res = sqlite3Fts5UnicodeCategory(1000); // iKey = 1000
    return res == 9;
}

// Test 3: iCode with iKey just above lower bound (1001) should yield 5
// Odd diff yields 5 in parity branch
bool test_case_03_key_just_above_bound(){
    int res = sqlite3Fts5UnicodeCategory(1001); // iKey = 1001
    return res == 5;
}

// Test 4: iCode with iKey well into the range but parity yields 5 (e.g., 1999 -> diff 999 odd)
bool test_case_04_mid_range_parity_odd(){
    int res = sqlite3Fts5UnicodeCategory(1999); // iKey = 1999
    return res == 5;
}

// Test 5: iCode with iKey triggering the threshold check (>= aMap[iRes] + (data[iRes]>>5)) => 0
// Using iKey = 3000 (>= 1000 + 2000), and since data>>5 == 2000 for index 0, this should return 0.
bool test_case_05_threshold_returns_zero(){
    int res = sqlite3Fts5UnicodeCategory(3000); // iKey = 3000
    return res == 0;
}

// Test 6: iCode >= (1<<20) should immediately return 0 (upper bound check)
bool test_case_06_code_above_limit(){
    int res = sqlite3Fts5UnicodeCategory((1<<20)); // 1048576
    return res == 0;
}

// Test 7: iCode with iKey below first map boundary (e.g., 999) should yield 0 (iRes < 0)
bool test_case_07_key_below_bound(){
    int res = sqlite3Fts5UnicodeCategory(999); // iKey = 999 < 1000
    return res == 0;
}

// ---------- Main: run all tests ----------

int main(){
    // Collect and run tests
    int total = 7;
    int passed = 0;

    passed += runTest("Case 1: iCode = 0 -> expect 0", test_case_01_code_zero);
    if (test_case_01_code_zero()) ++passed;

    if (runTest("Case 2: iCode = 1000 -> expect 9", test_case_02_key_at_map_bound)) ++passed;
    if (test_case_02_key_at_map_bound()) ++passed;

    if (runTest("Case 3: iCode = 1001 -> expect 5", test_case_03_key_just_above_bound)) ++passed;
    if (test_case_03_key_just_above_bound()) ++passed;

    if (runTest("Case 4: iCode = 1999 -> expect 5", test_case_04_mid_range_parity_odd)) ++passed;
    if (test_case_04_mid_range_parity_odd()) ++passed;

    if (runTest("Case 5: iCode = 3000 -> expect 0", test_case_05_threshold_returns_zero)) ++passed;
    if (test_case_05_threshold_returns_zero()) ++passed;

    if (runTest("Case 6: iCode = 1<<20 -> expect 0", test_case_06_code_above_limit)) ++passed;
    if (test_case_06_code_above_limit()) ++passed;

    if (runTest("Case 7: iCode = 999 -> expect 0", test_case_07_key_below_bound)) ++passed;
    if (test_case_07_key_below_bound()) ++passed;

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
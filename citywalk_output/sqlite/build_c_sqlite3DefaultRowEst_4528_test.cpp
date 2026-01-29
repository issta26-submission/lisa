// Minimal C++11 test suite for sqlite3DefaultRowEst(Index*) without GoogleTest.
// The tests exercise different code paths of sqlite3DefaultRowEst as exercised
// by carefully configuring a matching Index and Table structure.
// The test uses a lightweight in-file test harness (no GTest) and reports
// non-terminating pass/fail results to stdout.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Forward-declare the symbol as C linkage to match the original function signature.
extern "C" void sqlite3DefaultRowEst(struct Index *pIdx);

// Include internal SQLite structures to construct valid test inputs.
// This header provides the Index and Table types and related fields used
// by sqlite3DefaultRowEst (aiRowLogEst, nKeyCol, hasStat1, pTable, etc.).
extern "C" {
}

// Lightweight test harness
static int g_failures = 0;
#define TEST_EQ(msg, a, b) \
  do { if ((a) != (b)) { std::cerr << "TEST_FAIL: " << (msg) \
      << " expected " << (b) << " got " << (a) << std::endl; ++g_failures; } } while(0)

static void test_basic_small_keycol() {
  // Candidate Keywords: small nKeyCol, pPartIdxWhere == nullptr, nRowLogEst < 99
  // Expected behavior:
  // - pTable->nRowLogEst is raised to 99 (since 50 < 99)
  // - a[0] becomes 99
  // - a[1..nKeyCol] are filled from aVal (33,32,30 for nKeyCol=3)
  // - No extra a[i] beyond nKeyCol (nKeyCol=3) since nKeyCol <= nCopy
  int nKeyCol = 3;

  // Prepare objects
  static Index testIdx;
  static Table testTbl;
  // Ensure static lifetime doesn't interfere between tests
  // Allocate aiRowLogEst with at least nKeyCol+1 elements
  LogEst *arr = new LogEst[nKeyCol + 1];
  testIdx.aiRowLogEst = arr;
  testIdx.nKeyCol = nKeyCol;
  testIdx.hasStat1 = 0;
  testIdx.pTable = &testTbl;
  testIdx.pPartIdxWhere = 0; // nullptr to take first branch (no partition)

  testTbl.nRowLogEst = 50;  // less than 99 to trigger the 99 floor
  // Call method under test
  sqlite3DefaultRowEst(&testIdx);

  // Validate outcomes
  // a[0] should be 99 due to the floor
  TEST_EQ("basic_sm_ck_a0", (int)arr[0], 99);
  // a[1..3] should be 33, 32, 30 respectively
  TEST_EQ("basic_sm_ck_a1", (int)arr[1], 33);
  TEST_EQ("basic_sm_ck_a2", (int)arr[2], 32);
  TEST_EQ("basic_sm_ck_a3", (int)arr[3], 30);

  // No extra keys beyond nKeyCol; nothing to test here for extras.

  // Cleanup
  delete[] arr;
}

static void test_large_keycol_with_part() {
  // Candidate Keywords: large nKeyCol (> 5), pPartIdxWhere != nullptr
  // Expected behavior:
  // - x starts at 1000, floor check passes, since 99==sqlite3LogEst(1000)
  // - since pPartIdxWhere != 0, x is decremented by 10 -> 990
  // - a[0] should be 990
  // - a[1..5] should be 33,32,30,28,26
  // - a[6..nKeyCol] (here nKeyCol=7) should be 23
  int nKeyCol = 7;

  static Index testIdx;
  static Table testTbl;
  LogEst *arr = new LogEst[nKeyCol + 1];
  testIdx.aiRowLogEst = arr;
  testIdx.nKeyCol = nKeyCol;
  testIdx.hasStat1 = 0;
  testIdx.pTable = &testTbl;
  testIdx.pPartIdxWhere = (void*)1; // non-null to trigger pPartIdxWhere branch

  testTbl.nRowLogEst = 1000; // triggers x<... condition not used; floor occurs
  sqlite3DefaultRowEst(&testIdx);

  TEST_EQ("large_kc_with_part_a0", (int)arr[0], 990);
  TEST_EQ("large_kc_with_part_a1", (int)arr[1], 33);
  TEST_EQ("large_kc_with_part_a2", (int)arr[2], 32);
  TEST_EQ("large_kc_with_part_a3", (int)arr[3], 30);
  TEST_EQ("large_kc_with_part_a4", (int)arr[4], 28);
  TEST_EQ("large_kc_with_part_a5", (int)arr[5], 26);
  // a[6] and a[7] should be 23 as per the default fill for extra keys
  TEST_EQ("large_kc_with_part_a6", (int)arr[6], 23);
  TEST_EQ("large_kc_with_part_a7", (int)arr[7], 23);

  delete[] arr;
  // Note: We could test a[pIdx->nKeyCol] == 0 if IsUniqueIndex(testIdx) is true,
  // but this depends on the internal criterion for uniqueness which may vary
  // with the actual implementation. We focus on deterministic parts here.
}

static void test_small_x_with_part() {
  // Candidate Keywords: x<99 with partition index present
  // Expected behavior:
  // - pTable->nRowLogEst set to 99 (x<99 branch)
  // - Since pPartIdxWhere != 0, a[0] is then 89
  int nKeyCol = 4;

  static Index testIdx;
  static Table testTbl;
  LogEst *arr = new LogEst[nKeyCol + 1];
  testIdx.aiRowLogEst = arr;
  testIdx.nKeyCol = nKeyCol;
  testIdx.hasStat1 = 0;
  testIdx.pTable = &testTbl;
  testIdx.pPartIdxWhere = (void*)1;

  testTbl.nRowLogEst = 50; // < 99 to trigger floor
  sqlite3DefaultRowEst(&testIdx);

  // a[0] should be 89 after subtracting 10 from 99
  TEST_EQ("small_x_with_part_a0", (int)arr[0], 89);
  // a[1..4] should be 33,32,30,28 respectively (nCopy = min(5, 4) = 4)
  TEST_EQ("small_x_with_part_a1", (int)arr[1], 33);
  TEST_EQ("small_x_with_part_a2", (int)arr[2], 32);
  TEST_EQ("small_x_with_part_a3", (int)arr[3], 30);
  TEST_EQ("small_x_with_part_a4", (int)arr[4], 28);

  delete[] arr;
  // Also verify internal pTable.nRowLogEst was set to 99 due to floor
  TEST_EQ("small_x_with_part_nRowLogEst", (int)testTbl.nRowLogEst, 99);
}

int main() {
  // Run tests
  test_basic_small_keycol();
  test_large_keycol_with_part();
  test_small_x_with_part();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}
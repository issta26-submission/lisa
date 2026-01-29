// C++11 Test Suite for focal function testFetchStr (from lsmtest_main.c)
// This file provides a small, self-contained test harness (no GTest) that
// exercises testFetchStr via the C API exposed in lsmtest.h.
// It exercises true and false branches by writing, fetching, and validating
// values in a TestDb instance.

// Step 1 (Candidate Keywords summary):
// - TestDb, testOpen, testClose
// - testWriteStr, testFetchStr
// - testFetch (indirectly via testFetchStr), tdb_fetch (through the C API)
// - zKey, zVal and their lengths, pRc (out error code)
// - Memcmp-based value comparisons (via C functions in lsmtest.c)
// - Basic test harness macros to record failures without terminating tests

#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <iostream>


extern "C" {
}

// Lightweight, non-terminating test harness (EXPECT-like macros)
static int g_nTests = 0;
static int g_nFails = 0;

#define TEST_EXPECT(cond, desc) \
  do { \
    ++g_nTests; \
    if(!(cond)) { \
      ++g_nFails; \
      std::cerr << "[TEST FAIL] " << desc \
                << " (condition: " #cond ", line: " << __LINE__ << ")" << std::endl; \
    } \
  } while(0)


// Helper to print a short summary header per test file
static void print_summary_header() {
  std::cout << "LSM testFetchStr suite starting..." << std::endl;
}


// Step 2 & 3: Test Suite for testFetchStr
// Scenarios:
//  - Write a key-value pair and fetch the exact value (expect success)
//  - Write an empty-value entry and fetch it (expect success)
//  - Attempt to fetch with an incorrect value (expect failure)
//  - Attempt to fetch a non-existent key (expect failure)
static void run_testFetchStr_suite() {
  print_summary_header();

  int rc = 0;
  // Open a fresh test DB (lsm system) and clear existing data
  TestDb *pDb = testOpen("lsm", 1, &rc);
  TEST_EXPECT(pDb != 0, "Open test database (lsm, clear=true)");
  if(!pDb) return;

  // 1) Write key1 -> val1
  int pRc = 0;
  testWriteStr(pDb, "key1", "val1", &pRc);
  TEST_EXPECT(pRc == 0, "Write: key1 -> val1");

  // 2) Fetch key1 with exact value "val1"
  pRc = 0;
  testFetchStr(pDb, "key1", "val1", &pRc);
  TEST_EXPECT(pRc == 0, "Fetch: key1 with correct value should succeed");

  // 3) Write key2 with empty value ""
  pRc = 0;
  testWriteStr(pDb, "emptyKey", "", &pRc);
  TEST_EXPECT(pRc == 0, "Write: emptyKey -> empty string");

  // 4) Fetch emptyKey with empty value ""
  pRc = 0;
  testFetchStr(pDb, "emptyKey", "", &pRc);
  TEST_EXPECT(pRc == 0, "Fetch: emptyKey with empty value should succeed");

  // 5) Fetch key1 with wrong value -> should fail (rc non-zero)
  pRc = 0;
  testFetchStr(pDb, "key1", "wrong", &pRc);
  TEST_EXPECT(pRc != 0, "Fetch: key1 with incorrect value should fail");

  // 6) Fetch non-existent key -> should fail (rc non-zero)
  pRc = 0;
  testFetchStr(pDb, "noKey", "val", &pRc);
  TEST_EXPECT(pRc != 0, "Fetch: non-existent key should fail");

  // Cleanup
  testClose(&pDb);
}


// Entry point
int main(int argc, char **argv) {
  // Run test suite
  run_testFetchStr_suite();

  // Summary
  std::cout << "Total tests executed: " << g_nTests
            << ", Failures: " << g_nFails << std::endl;

  // Non-zero return indicates test failures
  return g_nFails;
}
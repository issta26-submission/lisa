// Lightweight C++11 test suite for the focal method tdb_fetch
// This test harness uses a small, self-contained assertion mechanism
// (no GTest). It tests the behavior of tdb_fetch by providing a mock
// pMethods->xFetch implementation and verifying that tdb_fetch passes
// arguments correctly and propagates return values.
//
// Notes:
// - The tests rely on the project headers: lsmtest_tdb.h, lsm.h, lsmtest.h
//   as included in the original FOCAL_CLASS_DEP.
// - We implement a minimal mock for xFetch that records the parameters it
//   received and optionally simulates an error to test both branches.
// - Static members are accessed only through the provided interface, matching
//   the constraint about static visibility.
// - The tests are executed from main() and report pass/fail counts with
//   non-terminating assertions to maximize code coverage.

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <cstdio>
#include <cstddef>
#include <stdio.h>
#include <cstring>
#include <leveldb/c.h>
#include <lsm.h>


// Import all necessary dependencies with correct paths (as per the given project)
extern "C" {
}

// Lightweight testing framework (non-terminating, records failures)
static int g_fail_count = 0;

#define EXPECT_INT_EQ(a, b) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_INT_EQ failed: %d != %d (%s:%d)\n", (int)(a), (int)(b), __FILE__, __LINE__); \
    ++g_fail_count; \
  } \
} while (0)

#define EXPECT_PTR_EQ(a, b) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_PTR_EQ failed: %p != %p (%s:%d)\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
    ++g_fail_count; \
  } \
} while (0)

// Global state for the mock xFetch to verify call details
static int g_simulate_error = 0;
static int g_last_nKey = 0;
static void *g_last_pKey = nullptr;
static void *g_last_pDb = nullptr;
static void **g_last_ppVal = nullptr;
static int *g_last_pnVal = nullptr;

// Shared value whose address will be returned by the mock fetch
static int g_shared_fetch_value = 0x12345678;

// Forward declaration of the mock xFetch (signature must match actual type)
static int mock_xFetch(TestDb *pDb, void *pKey, int nKey, void **ppVal, int *pnVal);

// Helper to initialize the mock environment for each test
static void init_mock_environment()
{
  g_simulate_error = 0;
  g_last_nKey = 0;
  g_last_pKey = nullptr;
  g_last_pDb = nullptr;
  g_last_ppVal = nullptr;
  g_last_pnVal = nullptr;
}

// Test 1: Success path - ensure tdb_fetch forwards parameters and returns 0
// and that output pointers are set by the mock fetch.
static void test_fetch_success()
{
  // Prepare a TestDb with the mock xFetch
  static TestDb mock_db;
  static TestDbMethods mock_methods;

  mock_methods.xFetch = mock_xFetch;
  mock_db.pMethods = &mock_methods;

  // Input key
  const char keyData[] = "ab";   // nKey = 2
  void *valOut = nullptr;
  int valLen = 0;

  init_mock_environment();
  g_simulate_error = 0;
  // Call the focal method
  int rc = tdb_fetch(&mock_db, (void*)keyData, (int)strlen(keyData), &valOut, &valLen);

  // Expectations
  EXPECT_INT_EQ(rc, 0);
  // The mock should have captured the call details
  EXPECT_PTR_EQ(g_last_pDb, &mock_db);
  EXPECT_PTR_EQ(g_last_pKey, (void*)keyData);
  EXPECT_INT_EQ(g_last_nKey, 2);
  // Output pointers should be set to the shared value
  EXPECT_PTR_EQ(valOut, (void*)&g_shared_fetch_value);
  EXPECT_INT_EQ(valLen, 0xAB); // As set by mock_xFetch (0xAB)
}

// Test 2: Error path - simulate xFetch failure and verify that tdb_fetch propagates the error
static void test_fetch_error()
{
  // Use the same DB setup as Test 1
  static TestDb mock_db;
  static TestDbMethods mock_methods;

  mock_methods.xFetch = mock_xFetch;
  mock_db.pMethods = &mock_methods;

  const char keyData[] = "xyz"; // nKey = 3
  void *valOut = nullptr;
  int valLen = 0;

  init_mock_environment();
  g_simulate_error = 1; // Force error from xFetch

  int rc = tdb_fetch(&mock_db, (void*)keyData, (int)strlen(keyData), &valOut, &valLen);

  // Expect error code propagated
  EXPECT_INT_EQ(rc, -1);
  // Even on error, parameters up to the point of call should be captured
  EXPECT_PTR_EQ(g_last_pDb, &mock_db);
  EXPECT_PTR_EQ(g_last_pKey, (void*)keyData);
  EXPECT_INT_EQ(g_last_nKey, 3);
  // Output pointers should not have been modified by a failing xFetch
  // (Retain previous values is not guaranteed; we only check for non-crash and error code)
}

// Test 3: Null output pointers - ensure code handles null ppVal/pnVal gracefully
static void test_fetch_null_pointers()
{
  static TestDb mock_db;
  static TestDbMethods mock_methods;

  mock_methods.xFetch = mock_xFetch;
  mock_db.pMethods = &mock_methods;

  const char keyData[] = "k"; // nKey = 1

  init_mock_environment();
  g_simulate_error = 0;

  // Pass NULL for both ppVal and pnVal
  int rc = tdb_fetch(&mock_db, (void*)keyData, (int)strlen(keyData), nullptr, nullptr);

  // Expect success and no crash
  EXPECT_INT_EQ(rc, 0);
  // The mock should still have seen the correct call details
  EXPECT_PTR_EQ(g_last_pDb, &mock_db);
  EXPECT_PTR_EQ(g_last_pKey, (void*)keyData);
  EXPECT_INT_EQ(g_last_nKey, 1);
}

// Test 4: Different key length propagation - verify that varying nKey reaches xFetch
static void test_fetch_various_key_length()
{
  static TestDb mock_db;
  static TestDbMethods mock_methods;

  mock_methods.xFetch = mock_xFetch;
  mock_db.pMethods = &mock_methods;

  const char keyData[] = "abcdefgh"; // nKey = 8

  void *valOut = nullptr;
  int valLen = 0;

  init_mock_environment();
  g_simulate_error = 0;

  int rc = tdb_fetch(&mock_db, (void*)keyData, 8, &valOut, &valLen);

  // Expect success and that nKey was propagated as 8
  EXPECT_INT_EQ(rc, 0);
  EXPECT_PTR_EQ(g_last_pDb, &mock_db);
  EXPECT_PTR_EQ(g_last_pKey, (void*)keyData);
  EXPECT_INT_EQ(g_last_nKey, 8);
  EXPECT_PTR_EQ(valOut, (void*)&g_shared_fetch_value);
  EXPECT_INT_EQ(valLen, 0xAB);
}

// Implementation of the mock xFetch used by all tests
static int mock_xFetch(TestDb *pDb, void *pKey, int nKey, void **ppVal, int *pnVal)
{
  // Capture all parameters to verify proper forwarding
  g_last_pDb = pDb;
  g_last_pKey = pKey;
  g_last_nKey = nKey;
  g_last_ppVal = ppVal;
  g_last_pnVal = pnVal;

  if (g_simulate_error) {
    // Simulate an error condition
    return -1;
  }

  // On success, populate output pointers if provided
  if (ppVal != nullptr) {
    *ppVal = (void *)&g_shared_fetch_value;
  }
  if (pnVal != nullptr) {
    *pnVal = 0xAB;
  }
  return 0;
}

// Entry point for running all tests
int main()
{
  // Run tests, preserving a non-terminating assertion strategy
  test_fetch_success();
  test_fetch_error();
  test_fetch_null_pointers();
  test_fetch_various_key_length();

  if (g_fail_count == 0) {
    printf("All tests passed.\n");
  } else {
    fprintf(stderr, "Total failures: %d\n", g_fail_count);
  }

  // Return non-zero if any test failed
  return (g_fail_count == 0) ? 0 : 1;
}

// End of test suite
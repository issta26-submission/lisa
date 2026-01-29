// This test suite exercises the focal method tdb_write by validating its forwarding
// behavior to the underlying xWrite method of the TestDb's pMethods.
// The tests are written in C++11, do not rely on GoogleTest, and use a lightweight
// in-file test harness with non-terminating assertions (printed results).

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <vector>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <leveldb/c.h>
#include <lsm.h>


// Include dependencies and declarations for the focal method and related types.
// The tests rely on the existing C API/types defined in lsmtest_tdb.h.

extern "C" int tdb_write(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal);

// Lightweight capture of xWrite invocations to validate forwarding behavior.
struct CallRecord {
  TestDb* pDb;
  void* pKey;
  int nKey;
  void* pVal;
  int nVal;
};

// Global capture store for all synthetic xWrite calls during tests.
static std::vector<CallRecord> g_calls;

// First fake xWrite: returns 123 and records the invocation details.
static int fake_xWrite_123(TestDb* pDb, void* pKey, int nKey, void* pVal, int nVal) {
  g_calls.push_back({pDb, pKey, nKey, pVal, nVal});
  return 123;
}

// Second fake xWrite: returns 42 and records the invocation details.
static int fake_xWrite_42(TestDb* pDb, void* pKey, int nKey, void* pVal, int nVal) {
  g_calls.push_back({pDb, pKey, nKey, pVal, nVal});
  return 42;
}

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;
static void log_fail(const char* msg) {
  printf("  [FAIL] %s\n", msg);
  ++g_failed_tests;
}
static void log_pass(const char* name) {
  printf("  [PASS] %s\n", name);
  // total count updated by caller after a test; kept separate to allow per-test granularity.
}
static void report_test_result(const char* name, bool ok) {
  ++g_total_tests;
  if (ok) log_pass(name);
  else log_fail(name);
}

// Test 1: Verify that tdb_write forwards all parameters to xWrite and returns the same value.
// Explanatory: Ensures the wrapper delegates to the underlying transport and passes key/value data correctly.
static void test_tdb_write_forwards_basic() {
  g_calls.clear();

  // Arrange: construct a TestDb with its pMethods.xWrite pointing to fake_xWrite_123
  TestDb db;
  TestDbMethods methods;
  methods.xWrite = fake_xWrite_123;
  db.pMethods = &methods;

  const char key[] = "abc";
  const char val[] = "def";

  // Act
  int ret = tdb_write(&db, (void*)key, (int)strlen(key), (void*)val, (int)strlen(val));

  // Assert
  bool ok = true;
  if (ret != 123) {
    log_fail("tdb_write should propagate xWrite return value (123)");
    ok = false;
  }
  if (g_calls.size() != 1) {
    log_fail("tdb_write should call xWrite exactly once");
    ok = false;
  }
  if (ok) {
    CallRecord &rec = g_calls[0];
    if (rec.pDb != &db) log_fail("tdb_write should pass the correct TestDb pointer");
    if (rec.pKey != (void*)key) log_fail("tdb_write should pass the exact pKey pointer");
    if (rec.nKey != (int)strlen(key)) log_fail("tdb_write should pass the exact nKey length");
    if (rec.pVal != (void*)val) log_fail("tdb_write should pass the exact pVal pointer");
    if (rec.nVal != (int)strlen(val)) log_fail("tdb_write should pass the exact nVal length");
  }

  report_test_result("test_tdb_write_forwards_basic", ok);
}

// Test 2: Validate that tdb_write forwards and returns a different value when xWrite returns 42.
// Explanatory: Confirms that the wrapper does not alter the return semantics from xWrite.
static void test_tdb_write_return_value_forwarding() {
  g_calls.clear();

  // Arrange: switch to fake_xWrite_42
  TestDb db;
  TestDbMethods methods;
  methods.xWrite = fake_xWrite_42;
  db.pMethods = &methods;

  const char key[] = "k";
  const char val[] = "v";

  // Act
  int ret = tdb_write(&db, (void*)key, (int)strlen(key), (void*)val, (int)strlen(val));

  // Assert
  bool ok = true;
  if (ret != 42) {
    log_fail("tdb_write should propagate xWrite return value (42)");
    ok = false;
  }
  if (g_calls.size() != 1) {
    log_fail("tdb_write should call xWrite exactly once in return-value test");
    ok = false;
  }
  if (ok) {
    CallRecord &rec = g_calls[0];
    if (rec.pDb != &db) log_fail("tdb_write should pass the correct TestDb pointer (return-value test)");
    if (rec.pKey != (void*)key) log_fail("tdb_write should pass the exact pKey pointer (return-value test)");
    if (rec.nKey != (int)strlen(key)) log_fail("tdb_write should pass the exact nKey length (return-value test)");
    if (rec.pVal != (void*)val) log_fail("tdb_write should pass the exact pVal pointer (return-value test)");
    if (rec.nVal != (int)strlen(val)) log_fail("tdb_write should pass the exact nVal length (return-value test)");
  }

  report_test_result("test_tdb_write_return_value_forwarding", ok);
}

// Test 3: Check behavior when key/value lengths are zero.
// Explanatory: Ensures tdb_write correctly forwards zero-length metadata without errors.
static void test_tdb_write_zero_lengths() {
  g_calls.clear();

  // Arrange: xWrite returns 123 again
  TestDb db;
  TestDbMethods methods;
  methods.xWrite = fake_xWrite_123;
  db.pMethods = &methods;

  const char key[] = "empty";
  const char val[] = "nothing";

  // Act
  int ret = tdb_write(&db, (void*)key, 0, (void*)val, 0);

  // Assert
  bool ok = true;
  if (ret != 123) {
    log_fail("tdb_write with zero lengths should propagate xWrite return value (123)");
    ok = false;
  }
  if (g_calls.size() != 1) {
    log_fail("tdb_write with zero lengths should still call xWrite exactly once");
    ok = false;
  }
  if (ok) {
    CallRecord &rec = g_calls[0];
    if (rec.pDb != &db) log_fail("tdb_write with zero lengths should pass the correct TestDb pointer");
    if (rec.pKey != (void*)key) log_fail("tdb_write with zero lengths should pass the exact pKey pointer");
    if (rec.nKey != 0) log_fail("tdb_write with zero lengths should pass nKey == 0");
    if (rec.pVal != (void*)val) log_fail("tdb_write with zero lengths should pass the exact pVal pointer");
    if (rec.nVal != 0) log_fail("tdb_write with zero lengths should pass nVal == 0");
  }

  report_test_result("test_tdb_write_zero_lengths", ok);
}

// Main function: Run all tests and summarize results.
// Explanatory: Invoked directly from main to satisfy the constraint of not using a test framework.
int main() {
  printf("Starting tdb_write unit tests...\n");

  test_tdb_write_forwards_basic();
  test_tdb_write_return_value_forwarding();
  test_tdb_write_zero_lengths();

  printf("Tests completed. Total: %d, Failed: %d\n", g_total_tests, g_failed_tests);
  return (g_failed_tests != 0) ? 1 : 0;
}
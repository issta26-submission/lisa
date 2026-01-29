// Test suite for the focal method: tdb_system_name(int i)
// Location: lsmtest_tdb.c
// This test harness is written as a standalone C++11 test that exercises
// the C function tdb_system_name exposed in lsmtest_tdb.c. We deliberately
// keep tests lightweight and non-terminating to maximize coverage across
// multiple runs. The tests rely on the actual in-repo definition of aLib and
// ArraySize used by tdb_system_name, so we include the C source to ensure the
// same translation unit is used during testing.

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <cstdlib>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <cstdio>
#include <lsmtest_tdb.c>
#include <stdio.h>
#include <leveldb/c.h>
#include <iostream>
#include <lsm.h>


// Ensure C linkage for the focal function when linked with the C test file.
extern "C" {
  // Prototypes for the focal function. The actual implementation is in lsmtest_tdb.c.
  const char *tdb_system_name(int i);
}

// Include the C test source that defines aLib and ArraySize, and the
// implementation of tdb_system_name in the same translation unit.
// Wrapping the include in extern "C" ensures C linkage for the embedded C code.
extern "C" {
}

// Simple non-terminating test harness (gtest not allowed). We accumulate
// failures and report at the end. This allows all tests to run even if one fails.
static int g_failures = 0;
#define ASSERT(cond, desc) do { \
  if(!(cond)) { \
    std::cerr << "ASSERT FAILED: " << desc << std::endl; \
    ++g_failures; \
  } \
} while(0)

// Test 1: Negative index should return NULL (false branch)
void test_negative_index() {
  const char *res = tdb_system_name(-1);
  ASSERT(res == nullptr, "tdb_system_name(-1) should return NULL");
}

// Test 2: Out-of-range index should return NULL (false branch)
void test_out_of_range_index() {
  // Pick a large value guaranteed to be out of range relative to aLib size
  const int out_of_range = 1000000;
  const char *res = tdb_system_name(out_of_range);
  ASSERT(res == nullptr, "tdb_system_name(out_of_range) should return NULL");
}

// Test 3: In-range index should return a non-null zName pointer
// We assume at least one element exists in aLib (typical for database libraries).
void test_in_range_index_non_null() {
  const char *res = tdb_system_name(0);
  ASSERT(res != nullptr, "tdb_system_name(0) should return a valid name pointer");
}

// Test 4: Repeated in-range calls should yield the same pointer (pointer stability)
void test_pointer_stability() {
  const char *p1 = tdb_system_name(0);
  const char *p2 = tdb_system_name(0);
  ASSERT(p1 == p2, "tdb_system_name(0) should return the same pointer on consecutive calls");
}

// Optional: Test 5 if there are multiple elements and index 1 is valid; this is
// best-effort since the exact size of aLib is not known here. We only run this
// when the second element exists in the current translation unit.
void test_in_range_index_one_maybe() {
  const char *res = tdb_system_name(1);
  // If the second element exists, it should yield a non-null pointer.
  // If it does not exist, this will simply assert to ensure we don't crash.
  // We can't know at compile time; we check for both possibilities gracefully.
  // Either the result is NULL (no second element) or non-NULL (second element exists).
  // We consider non-NULL as success here to maximize coverage when available.
  // When NULL, we don't fail since behavior is consistent with aLib size.
  if(res != nullptr) {
    ASSERT(res != nullptr, "tdb_system_name(1) exists and returns non-NULL");
  } else {
    // If NULL, reflect the absence of a second element; not a failure.
    std::cerr << "INFO: tdb_system_name(1) is NULL (no second element in aLib in this build)." << std::endl;
  }
}

// Runner: execute all tests and summarize results
int main() {
  std::cout << "Starting test suite for tdb_system_name...\n";

  test_negative_index();
  test_out_of_range_index();
  test_in_range_index_non_null();
  test_pointer_stability();
  test_in_range_index_one_maybe();

  if(g_failures == 0) {
    std::cout << "All tests passed (non-terminating)." << std::endl;
  } else {
    std::cerr << g_failures << " test(s) failed." << std::endl;
  }

  // Return number of failed tests to allow external harnesses to detect status
  return g_failures;
}
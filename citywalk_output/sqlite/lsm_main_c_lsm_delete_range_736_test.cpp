// Unit test suite for the focal method: lsm_delete_range
// This test suite is written in C++11, without using GTest.
// It focuses on exercising the true/false branches of the condition
// inside lsm_delete_range by controlling the xCmp callback via a fake lsm_db.
// Explanatory comments are added to each test to describe intent and coverage.

// Candidate Keywords extracted from the focal method and its dependencies:
// - lsm_delete_range, lsm_db, xCmp, doWriteOp, LSM_OK
// - pKey1, nKey1, pKey2, nKey2, range key bounds
// - Conditional branch: if (db->xCmp(...) < 0) { rc = doWriteOp(...); }
// - Test goals: exercise true branch (xCmp < 0) and false branch (xCmp >= 0)
// - Interaction with static/internal helpers (not directly accessible from tests)
// - Access and verification of parameter passing (addresses vs. contents)

#include <cstddef>
#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Include the project's public/internal header that defines lsm_db and API.
// This header is assumed to exist in the project, as suggested by the provided
// class dependencies (<FOCAL_CLASS_DEP>).

// Forward declaration of the focal function (C linkage) to ensure linkage from C++ test code.
extern "C" int lsm_delete_range(
  lsm_db *db,
  const void *pKey1, int nKey1,
  const void *pKey2, int nKey2
);

// Global test state to observe comparator invocation and key values
static bool g_cmp_called = false;
static const void *g_last_p1 = nullptr;
static const void *g_last_p2 = nullptr;
static int g_last_n1 = 0;
static int g_last_n2 = 0;
static int g_last_value1 = 0;
static int g_last_value2 = 0;

// Sentinel comparator implementations to drive true/false branches
// These are declared with C linkage so the function pointer type in lsm_db
// can point to them from the test code.
extern "C" int lsm_xcmp_negative(void *p1, int n1, void *p2, int n2) {
  g_cmp_called = true;
  g_last_p1 = p1;
  g_last_p2 = p2;
  g_last_n1 = n1;
  g_last_n2 = n2;
  // Interpret the keys as integers to observe content-based comparisons.
  g_last_value1 = *(int*)p1;
  g_last_value2 = *(int*)p2;
  return -1; // force the true-branch: delete range
}

extern "C" int lsm_xcmp_positive(void *p1, int n1, void *p2, int n2) {
  g_cmp_called = true;
  g_last_p1 = p1;
  g_last_p2 = p2;
  g_last_n1 = n1;
  g_last_n2 = n2;
  g_last_value1 = *(int*)p1;
  g_last_value2 = *(int*)p2;
  return 1; // force the false-branch: do not delete range
}

// Simple non-terminating assertion macro (does not exit on failure)
static int g_failures = 0;
#define CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "CHECK FAILED: " << (msg) \
              << " (Line " << __LINE__ << ")" << std::endl; \
    ++g_failures; \
  } \
} while(0)

// Test fixture-like encapsulation for readability
class LsmDeleteRangeTests {
public:
  // Run all tests
  static void runAll() {
    test_branch_true_when_xcmp_negative();
    test_branch_false_when_xcmp_positive();
  }

private:
  // Test 1: Branch true when xCmp returns negative (-1)
  // This exercises the path that calls doWriteOp with a valid db handle.
  // We verify that:
  // - rc equals LSM_OK (assuming doWriteOp returns 0 in the simplified stub)
  // - xCmp is invoked
  // - pKey1 and pKey2 are passed as the exact addresses we provided
  // - nKey1 and nKey2 match the sizes we supplied (sizeof(int))
  static void test_branch_true_when_xcmp_negative() {
    // Reset state
    g_cmp_called = false;
    g_last_p1 = nullptr;
    g_last_p2 = nullptr;
    g_last_n1 = g_last_n2 = 0;
    g_last_value1 = g_last_value2 = 0;

    // Prepare a minimal key pair (integers treated as keys)
    const int key1 = 5;
    const int key2 = 10;

    // Prepare a db handle with a controlled comparator
    lsm_db db;
    db.xCmp = lsm_xcmp_negative; // comparator that forces negative result

    int rc = lsm_delete_range(&db, &key1, (int)sizeof(int), &key2, (int)sizeof(int));

    CHECK(rc == LSM_OK, "rc should be LSM_OK when comparator returns negative (branch taken)");
    CHECK(g_cmp_called, "xCmp should have been invoked for branch selection");
    CHECK(g_last_p1 == &key1, "pKey1 should be the address of key1 (content address should be passed)");
    CHECK(g_last_p2 == &key2, "pKey2 should be the address of key2 (content address should be passed)");
    CHECK(g_last_n1 == (int)sizeof(int) && g_last_n2 == (int)sizeof(int),
          "nKey1/nKey2 should equal the provided key sizes");
    // Also verify we observed the content values used by xCmp
    CHECK(g_last_value1 == key1 && g_last_value2 == key2,
          "xCmp received key contents (value1/value2) corresponding to key1/key2");
  }

  // Test 2: Branch false when xCmp returns non-negative (e.g., positive)
  // This exercises the path where doWriteOp is not invoked.
  // We verify that:
  // - rc equals LSM_OK (no write op performed; initializer)
  // - xCmp is invoked
  // - pKey1 and pKey2 are passed as the exact addresses we provided
  // - nKey1 and nKey2 match the sizes we supplied (sizeof(int))
  static void test_branch_false_when_xcmp_positive() {
    // Reset state
    g_cmp_called = false;
    g_last_p1 = nullptr;
    g_last_p2 = nullptr;
    g_last_n1 = g_last_n2 = 0;
    g_last_value1 = g_last_value2 = 0;

    const int key1 = 20;
    const int key2 = 25;

    lsm_db db;
    db.xCmp = lsm_xcmp_positive; // comparator that forces non-negative result

    int rc = lsm_delete_range(&db, &key1, (int)sizeof(int), &key2, (int)sizeof(int));

    CHECK(rc == LSM_OK, "rc should be LSM_OK when comparator returns non-negative (branch skipped)");
    CHECK(g_cmp_called, "xCmp should have been invoked for branch selection");
    CHECK(g_last_p1 == &key1, "pKey1 should be the address of key1 (content address should be passed)");
    CHECK(g_last_p2 == &key2, "pKey2 should be the address of key2 (content address should be passed)");
    CHECK(g_last_n1 == (int)sizeof(int) && g_last_n2 == (int)sizeof(int),
          "nKey1/nKey2 should equal the provided key sizes");
    CHECK(g_last_value1 == key1 && g_last_value2 == key2,
          "xCmp received key contents (value1/value2) corresponding to key1/key2");
  }

  // Helper wrappers to access the private tests
  // These wrappers call the static methods above.
  static void test_branch_true_when_xcmp_negative() {
    test_branch_true_when_xcmp_negative_impl();
  }
  static void test_branch_false_when_xcmp_positive() {
    test_branch_false_when_xcmp_positive_impl();
  }

  // Actual implementations invoked by wrappers
  static void test_branch_true_when_xcmp_negative_impl() {
    test_branch_true_when_xcmp_negative(); // call static method (will be redirected by the preprocessor)
  }
  static void test_branch_false_when_xcmp_positive_impl() {
    test_branch_false_when_xcmp_positive(); // call static method (will be redirected by the preprocessor)
  }

  // Note:
  // The above indirection (impl wrappers) is to keep the test layout explicit
  // while meeting the "static methods" constraint from the domain guidance.
};

// Entry point for tests
int main() {
  // Run all tests
  LsmDeleteRangeTests::runAll();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
  }

  return g_failures;
}
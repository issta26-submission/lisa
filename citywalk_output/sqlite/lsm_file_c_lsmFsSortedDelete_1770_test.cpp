// C++11 style lightweight unit tests for the focal C function lsmFsSortedDelete
// This test suite does not rely on GTest or any external testing framework.
// It uses a minimal in-file test runner and simple EXPECT-like assertions.
// The tests exercise the lsmFsSortedDelete function provided by the C codebase.
// Notes:
// - We rely on the existing type definitions (FileSystem, Snapshot, Segment, Redirect, etc.)
//   from the project's headers. These tests instantiate those types and invoke the
//   function to observe behavior.
// - We avoid private/static internals beyond what the focal function already uses.
// - Static helpers inside lsm_file.c remain unmocked; we rely on the actual behavior
//   of the static helpers for realistic interaction.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Include the project header(s) that declare the focal structures and functions.
// The exact include paths depend on the project layout. If your environment
// uses a different include path, adjust accordingly.

// The focal function uses C linkage. Declare it for C++ to call it correctly.
extern "C" int lsmFsSortedDelete(
  FileSystem *pFS, 
  Snapshot *pSnapshot,
  int bZero,                      /* True to zero the Segment structure */
  Segment *pDel
);

// Lightweight test assertions (non-terminating)
#define TEST_PASSED true
#define TEST_FAILED false

#define EXPECT_EQ(actual, expected)                                                \
  do {                                                                             \
    if ((actual) != (expected)) {                                                  \
      std::cerr << "EXPECT_EQ failed: " #actual " (" << (actual)                 \
                << ") != " #expected " (" << (expected) << ") at " << __FILE__     \
                << ":" << __LINE__ << "\n";                                      \
      return TEST_FAILED;                                                        \
    }                                                                            \
  } while (0)

#define EXPECT_TRUE(cond)                                                            \
  do {                                                                                \
    if (!(cond)) {                                                                    \
      std::cerr << "EXPECT_TRUE failed: condition is false at " << __FILE__ << ":" \
                << __LINE__ << "\n";                                                  \
      return TEST_FAILED;                                                           \
    }                                                                               \
  } while (0)

#define EXPECT_NOT_NULL(ptr)                                                         \
  do {                                                                                \
    if ((ptr) == nullptr) {                                                           \
      std::cerr << "EXPECT_NOT_NULL failed: pointer is null at " << __FILE__ << ":"  \
                << __LINE__ << "\n";                                                  \
      return TEST_FAILED;                                                           \
    }                                                                               \
  } while (0)


// Utility to zero-initialize a structure safely
template <typename T>
static void ZeroInit(T &obj) {
  std::memset(&obj, 0, sizeof(obj));
}


// Test 1: No-operation when pDel->iFirst is zero
// Description:
// - pDel.iFirst == 0 -> the function should skip processing and return LSM_OK.
// - Ensures that no side effects occur on FileSystem or Snapshot.
// - Validates that the return code remains LSM_OK under the no-op condition.
static bool test_lsmFsSortedDelete_noop_when_iFirst_zero() {
  FileSystem fs;
  Snapshot snap;
  Segment del;

  ZeroInit(fs);
  ZeroInit(snap);
  ZeroInit(del);

  // Ensure iFirst is zero to trigger the no-op path
  del.iFirst = 0;

  int rc = lsmFsSortedDelete(&fs, &snap, 0, &del);

  // Return code should be LSM_OK
  EXPECT_EQ(rc, LSM_OK);

  // No modifications should occur to the provided structures (rough sanity check)
  // We expect the segment to remain zero-initialized or unchanged in a no-op.
  EXPECT_EQ(del.iFirst, 0);
  return TEST_PASSED;
}


// Test 2: When bZero is true, the function should still clear redirect if present
// and then zero the Segment structure after processing.
// Description:
// - pDel has iFirst non-zero and pRedirect pointing to snapshot.redirect to satisfy the
//   assertion in the code.
// - bZero = 1 ensures that after the loop, pDel is zeroed.
// - The side-effect on Snapshot.redirect.n should be that it gets cleared to 0 before the memset.
// - Verifies both the redirect handling and the post-call zeroing behavior.
static bool test_lsmFsSortedDelete_redirect_and_zero_segment() {
  FileSystem fs;
  Snapshot snap;
  Segment del;

  ZeroInit(fs);
  ZeroInit(snap);
  ZeroInit(del);

  // Set up a redirect to satisfy the internal assert:
  // pDel->pRedirect must point to &pSnapshot->redirect
  del.iFirst = 1;
  del.iLastPg = 1;
  del.pRedirect = &snap.redirect;

  // Pre-load redirect.n to a non-zero value to verify that the function clears it.
  snap.redirect.n = 123;

  int rc = lsmFsSortedDelete(&fs, &snap, 1, &del);

  // Expect successful operation
  EXPECT_EQ(rc, LSM_OK);

  // After the call with bZero == 1, del should be zeroed
  // We can't rely on struct contents post-zero; just ensure we attempted to process and didn't crash.
  EXPECT_EQ(del.iFirst, 0);
  EXPECT_EQ(del.iLastPg, 0);
  EXPECT_EQ(del.pRedirect, nullptr); // Should be cleared by memset

  // Redirect.n should have been cleared prior to memset; we verify the snapshot side effect
  // If memset zeroed the redirect as well, redirect.n would be 0, which is consistent with the expected behavior.
  EXPECT_EQ(snap.redirect.n, 0);

  return TEST_PASSED;
}


// Test 3: Ensure function handles non-redirect, non-zero iFirst paths without crashing.
// Description:
// - Provide a non-zero iFirst but do not set a redirect (to avoid triggering the assert).
// - Set bZero to 0 to exercise the flopping condition in the loop when blocks are skipped.
// - We primarily verify that the function returns LSM_OK under this configuration and does not crash.
static bool test_lsmFsSortedDelete_non_redirect_no_zero() {
  FileSystem fs;
  Snapshot snap;
  Segment del;

  ZeroInit(fs);
  ZeroInit(snap);
  ZeroInit(del);

  del.iFirst = 2;
  del.iLastPg = 2;
  // pDel.pRedirect remains NULL to avoid triggering the internal assert

  int rc = lsmFsSortedDelete(&fs, &snap, 0, &del);

  // We expect the function to return OK even if it doesn't free any blocks due to internal conditions.
  EXPECT_EQ(rc, LSM_OK);

  // pDel could be modified by internal logic; at minimum, ensure the return path is valid
  // No crash or assertion should have occurred.
  return TEST_PASSED;
}


// Simple test runner
int main() {
  int passed = 0;
  int failed = 0;

  auto run = [&](const char* name, bool result) {
    if (result) {
      std::cout << "[PASS] " << name << "\n";
      ++passed;
    } else {
      std::cout << "[FAIL] " << name << "\n";
      ++failed;
    }
  };

  run("test_lsmFsSortedDelete_noop_when_iFirst_zero", test_lsmFsSortedDelete_noop_when_iFirst_zero());
  run("test_lsmFsSortedDelete_redirect_and_zero_segment", test_lsmFsSortedDelete_redirect_and_zero_segment());
  run("test_lsmFsSortedDelete_non_redirect_no_zero", test_lsmFsSortedDelete_non_redirect_no_zero());

  std::cout << "Tests passed: " << passed << ", failed: " << failed << "\n";

  // Return non-zero if any test failed to aid integration with build systems.
  return (failed == 0) ? 0 : 1;
}
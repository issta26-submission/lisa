// C++11 unit test suite for sqlite3PcacheSetSpillsize in pcache.c
// This test suite is designed to be compiled alongside pcache.c (and any
// necessary sqlite headers) without using Google Test.
// It exercises key branches of the function (positive mxPage, negative mxPage,
// and mxPage == 0) and validates expected state changes in the PCache object.

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Forward declaration of the focal function from the C source file.
// We re-declare PCache in a way that matches the layout used in pcache.c.
// Domain knowledge indicates PCache in the real code has at least:
//   void* pCache; int szPage; int szExtra; int szSpill;  (and other fields)
extern "C" {
  struct PCache {
    void* pCache;
    int szPage;
    int szExtra;
    int szSpill;
    // Other fields exist in the real structure, but are opaque to this test.
  };

  // The focal function under test. C linkage to match the C implementation.
  int sqlite3PcacheSetSpillsize(struct PCache* p, int mxPage);
}

// Simple non-terminating test assertion mechanism.
// Each failed assertion is logged, but execution continues to maximize coverage.
static int g_failures = 0;

#define T_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::printf("TEST FAILED: %s\n  Condition: %s\n  File: %s:%d\n", (msg), #cond, __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

// Test Case 1: Positive mxPage should set p->szSpill to mxPage and return a value
// that is at least as large as the spill size.
static void test_mxPage_positive() {
  // Prepare PCache with non-null pCache (as required by the function's assert)
  struct PCache p;
  p.pCache = (void*)0x1;  // non-null sentinel
  p.szPage  = 1024;
  p.szExtra = 0;
  p.szSpill = 0;

  // Call the focal function with a positive mxPage
  int mxPage = 7;
  int res = sqlite3PcacheSetSpillsize(&p, mxPage);

  // Expects: p.szSpill becomes mxPage
  T_ASSERT(p.szSpill == mxPage, "When mxPage > 0, p->szSpill must be set to mxPage");

  // Return value should be max(numberOfCachePages(p), p->szSpill).
  // We cannot know numberOfCachePages' exact value here, but we can assert
  // that it is at least as large as the spill size.
  T_ASSERT(res >= p.szSpill, "Return value should be >= spill size after setting spill");
}

// Test Case 2: Negative mxPage should trigger conversion and set p->szSpill accordingly.
static void test_mxPage_negative() {
  // Prepare PCache
  struct PCache p;
  p.pCache = (void*)0x2;  // non-null sentinel
  p.szPage  = 1024;
  p.szExtra = 0;
  p.szSpill = 0;

  // Use a negative mxPage to exercise the conversion formula:
  // mxPage = (int)((-1024*(i64)mxPage)/(p->szPage+p->szExtra))
  int mxPage = -3;
  long long numerator = -1024LL * (long long)mxPage; // -1024 * (-3) = 3072
  long long denom = (long long)p.szPage + (long long)p.szExtra; // 1024
  int expectedSpill = (int)(numerator / denom); // 3072 / 1024 = 3

  int res = sqlite3PcacheSetSpillsize(&p, mxPage);

  // Expects: p.szSpill equals computed expected spill
  T_ASSERT(p.szSpill == expectedSpill,
           "For negative mxPage, p->szSpill should be set to the converted spill size");

  // Return value should be at least as large as the spill size
  T_ASSERT(res >= p.szSpill, "Return value should be >= spilled size after negative mxPage");
}

// Test Case 3: mxPage == 0 should not modify p->szSpill and should return a value
// that is >= current spill size.
static void test_mxPage_zero() {
  // Prepare PCache with an existing spill value
  struct PCache p;
  p.pCache = (void*)0x3;  // non-null sentinel
  p.szPage  = 256;
  p.szExtra = 8;
  p.szSpill = 5;            // non-zero initial spill

  int mxPage = 0;
  int res = sqlite3PcacheSetSpillsize(&p, mxPage);

  // Expects: p.szSpill remains unchanged
  T_ASSERT(p.szSpill == 5, "When mxPage == 0, p->szSpill must remain unchanged");

  // Return value should be at least as big as the current spill size
  T_ASSERT(res >= p.szSpill, "Return value should be >= unchanged spill when mxPage == 0");
}

// Helper to run all tests
static void run_all_tests() {
  test_mxPage_positive();
  test_mxPage_negative();
  test_mxPage_zero();
}

// Entry point
int main() {
  std::printf("sqlite3PcacheSetSpillsize unit tests (C++11) starting...\n");

  run_all_tests();

  if(g_failures == 0) {
    std::printf("ALL TESTS PASSED\n");
  } else {
    std::printf("TOTAL FAILURES: %d\n", g_failures);
  }

  // Return non-zero if any test failed
  return (g_failures == 0) ? 0 : 1;
}
/*
  C++11 unit test suite for the focal method:
  sqlite3PcacheRelease in pcache.c

  Notes on design and approach (mapped from the provided instructions):
  - Step 1 (Program Understanding): We identify the core dependencies used by sqlite3PcacheRelease:
      PgHdr p; p->nRef; p->pCache->nRefSum; p->flags; PGHDR_CLEAN flag.
    The function decrements the per-page reference count and, when it hits zero,
    either unreferences the page (pcacheUnpin) if CLEAN, or moves the page into a
    dirty list and sanity-checks the page otherwise.

  - Step 2 (Unit Test Generation): We craft tests that exercise:
      a) The false branch of the inner if (i.e., (--p->nRef) != 0, so no further work beyond
         nRefSum decrement occurs) – test that nRefSum is decremented and p->nRef is decremented.
      b) The true branch with CLEAN flag set (p->nRef becomes 0 and p->flags & PGHDR_CLEAN) –
         test that nRefSum is decremented and p->nRef is 0.
      c) The true branch with CLEAN not set (p->nRef becomes 0 and p->flags & ~CLEAN) –
         test that nRefSum is decremented and the code path through the dirty-list is taken.
    Because pcacheUnpin and pcacheManageDirtyList are static/internal to pcache.c, we rely on the
    existing implementation to perform side effects. We observe their indirect effects through
    shared memory state (nRefSum and p->nRef).

  - Step 3 (Test Case Refinement): We implement a lightweight test harness (no GTest) that uses
    non-terminating assertions (custom CHECK macro) to collect failures and run all tests in main.
    We avoid terminating the process on a single failure to maximize code coverage.

  Important build note:
  - The test is intended to be compiled together with pcache.c (as a single translation unit or
    linked together). We declare compatible lightweight stubs for the data structures used by
    sqlite3PcacheRelease so we can construct PgHdr / PCache objects in the test.
  - We assume the PCache structure used by pcache.c exposes a field named nRefSum as the first
    field (as per the usage in sqlite3PcacheRelease). If actual layout differs, resize/match
    accordingly for real-world runs.

  Limitations:
  - Static/internal functions like pcacheUnpin, pcacheManageDirtyList, and sqlite3PcachePageSanity
    are invoked by sqlite3PcacheRelease. We cannot directly observe them; we observe their
    effects on observable fields (nRef, nRefSum) and ensure no aborts occur.
  - We implement three tests to cover true/false branches of the key condition predicates.

  Below is the test code (C++11) that should be compiled with pcache.c:
*/

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Step 1: Provide C linkage for the focal function
extern "C" {
  // Forward declarations matching the usage within pcache.c
  // The actual definitions live in pcache.c; we rely on compatible memory layout.
  struct PCache;
  struct PgHdr;

  // The focal method under test
  void sqlite3PcacheRelease(PgHdr *p);

  // Some constants used by the focal method
  // We define PGHDR_CLEAN to mimic the flag check in sqlite3PcacheRelease
  static const unsigned int PGHDR_CLEAN = 0x01;
}

// Step 2: Provide test-facing type definitions compatible with pcache.c usage.
// Note: We assume PCache.nRefSum is the first field in PCache used by sqlite3PcacheRelease.
struct PCache {
  int nRefSum;  // observed by sqlite3PcacheRelease
  // Other members exist in the real sqlite3 PCache, but are opaque to this test.
};

// PgHdr layout: we mirror only the fields accessed by sqlite3PcacheRelease.
struct PgHdr {
  int nRef;            // decremented by sqlite3PcacheRelease
  unsigned int flags;    // contains PGHDR_CLEAN flag
  PCache *pCache;       // pointer to PCache containing nRefSum
  // Additional fields may exist in the real structure; they are not used by the focal method.
};

// Step 3: Utility: minimal non-terminating test harness
static std::vector<std::string> g_failures;

#define CHECK(cond, msg) do { if(!(cond)) { g_failures.push_back(msg); } } while(0)

static void test_release_branch_not_zero() {
  // Test that when initial nRef > 1, (--p->nRef) != 0 and only the sum is decremented (and nRef decremented)
  PCache cache;
  cache.nRefSum = 5;

  PgHdr hdr;
  hdr.nRef = 2;                  // initial nRef > 1
  hdr.flags = 0;                   // not CLEAN
  hdr.pCache = &cache;

  // Call the focal method
  sqlite3PcacheRelease(&hdr);

  // Expected: hdr.nRef decreased to 1; cache.nRefSum decreased to 4
  CHECK(hdr.nRef == 1, "test_release_branch_not_zero: nRef should decrement from 2 to 1");
  CHECK(cache.nRefSum == 4, "test_release_branch_not_zero: nRefSum should decrement from 5 to 4");
}

static void test_release_branch_zero_clean() {
  // Test that when initial nRef == 1 and CLEAN is set, (--p->nRef) becomes 0
  // and nRefSum is decremented; branch should call pcacheUnpin (observable via no crash).
  PCache cache;
  cache.nRefSum = 3;

  PgHdr hdr;
  hdr.nRef = 1;                  // initial nRef = 1
  hdr.flags = PGHDR_CLEAN;       // CLEAN flag set
  hdr.pCache = &cache;

  sqlite3PcacheRelease(&hdr);

  // Expected: hdr.nRef == 0; cache.nRefSum == 2
  CHECK(hdr.nRef == 0, "test_release_branch_zero_clean: nRef should decrement from 1 to 0");
  CHECK(cache.nRefSum == 2, "test_release_branch_zero_clean: nRefSum should decrement from 3 to 2");
}

static void test_release_branch_zero_not_clean() {
  // Test that when initial nRef == 1 and CLEAN is NOT set, (--p->nRef) becomes 0
  // to exercise the non-clean path (dirty list) as a branch.
  PCache cache;
  cache.nRefSum = 7;

  PgHdr hdr;
  hdr.nRef = 1;                 // initial nRef = 1
  hdr.flags = 0;                  // CLEAN not set
  hdr.pCache = &cache;

  sqlite3PcacheRelease(&hdr);

  // Expected: hdr.nRef == 0; cache.nRefSum == 6
  // We cannot observe internal static calls, but we can verify observable state changes.
  CHECK(hdr.nRef == 0, "test_release_branch_zero_not_clean: nRef should decrement from 1 to 0");
  CHECK(cache.nRefSum == 6, "test_release_branch_zero_not_clean: nRefSum should decrement from 7 to 6");
}

static void run_all_tests() {
  test_release_branch_not_zero();
  test_release_branch_zero_clean();
  test_release_branch_zero_not_clean();
}

int main() {
  // Run tests
  run_all_tests();

  // Report results
  if(g_failures.empty()) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << g_failures.size() << " TEST(S) FAILED:\n";
    for(const auto &s : g_failures) {
      std::cout << "- " << s << "\n";
    }
    return 1;
  }
}

/*
  Explanatory notes about the tests:
  - test_release_branch_not_zero covers the false-branch of the inner conditional (the --p->nRef != 0 case).
    We verify that both p->nRef and pCache->nRefSum are updated as expected.

  - test_release_branch_zero_clean covers the true-branch with CLEAN flag set. This exercises the path
    where pcacheUnpin would be invoked as part of the release, and we still verify the observable
    state changes on p and pCache.

  - test_release_branch_zero_not_clean covers the true-branch without CLEAN. This exercises the path
    that would call pcacheManageDirtyList and the subsequent sanity check, again verifying observable
    state changes.

  The tests refrain from using a full-fledged testing framework (as GTest is not allowed per the prompt)
  and rely on a small non-terminating assertion macro to accumulate failures and continue execution.

  Candidate Keywords (Step 1): sqlite3PcacheRelease, PgHdr, nRef, PCache, nRefSum, PGHDR_CLEAN, pcacheUnpin,
  pcacheManageDirtyList, sqlite3PcachePageSanity, pCache, flags, and related predicates.
*/
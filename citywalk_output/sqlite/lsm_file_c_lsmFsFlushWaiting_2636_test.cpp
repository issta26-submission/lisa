/*
  Unit test suite for the focal method:
  void lsmFsFlushWaiting(FileSystem *pFS, int *pRc)

  Notes:
  - This test suite is written in C++11 and intended to be linked with the C sources
    that define lsmFsFlushWaiting and its dependencies (e.g., lsm_file.c).
  - Tests assume presence of the lsmInt.h header providing FileSystem, Page, LSM_OK, etc.
  - To avoid abrupt termination due to asserts inside the focal code, tests should be
    compiled with -DNDEBUG to disable runtime asserts.
  - We implement a small, lightweight test harness (no GTest) that runs from main().
  - Tests focus on the observable behavior around pFS->pWaiting and pRc, exercising:
    a) empty waiting queue
    b) single-page queue with rc == LSM_OK
    c) multiple-page queue with rc == LSM_OK
  - We rely on the actual lsmFsPagePersist and lsmFsPageRelease implementations in
    lsm_file.c. If those functions have complex side effects, this test still exercises
    the control flow and basic state transitions of lsmFsFlushWaiting.
  - The test prints PASS/FAIL results for each case.

  Build notes:
  - Compile with C sources (lsm_file.c, etc.) and this test, e.g.:
      g++ -std=c++11 -DNDEBUG test_lsmfsflushwaiting.cpp lsm_file.c -o test_lsmfsflushwaiting
  - The -DNDEBUG flag disables asserts to allow non-crashing execution.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>


extern "C" {
  // Include the C header that defines the types used by lsmFsFlushWaiting.
  // This ensures the FileSystem and Page structures match the implementation.
  #include "lsmInt.h"

  // Focal function under test (C linkage)
  void lsmFsFlushWaiting(FileSystem *pFS, int *pRc);
}

// Lightweight test harness (no external testing framework)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_PASS() do { ++g_tests_passed; ++g_tests_run; } while(0)
#define TEST_FAIL() do { ++g_tests_run; } while(0)

static void report_results() {
  printf("Tests run: %d, Passed: %d, Failed: %d\n",
         g_tests_run, g_tests_passed, g_tests_run - g_tests_passed);
}

/*
  Test 1: Empty waiting queue
  - Setup: pFS->pWaiting == NULL
  - Expectation: rc remains unchanged; pFS->pWaiting remains NULL
*/
static bool test_flush_waiting_empty_queue() {
  FileSystem fs;
  // Zero initialize to mimic clean state
  memset(&fs, 0, sizeof(fs));

  int rc_in = LSM_OK; // typical success value
  lsmFsFlushWaiting(&fs, &rc_in);

  // Post-conditions:
  // - pWaiting should stay NULL (no processing)
  // - rc should remain LSM_OK
  bool ok = (fs.pWaiting == 0) && (rc_in == LSM_OK);
  return ok;
}

/*
  Test 2: Single page in waiting list with rc == LSM_OK
  - Setup: pFS->pWaiting -> pPage (nRef == 1, pWaitingNext == NULL)
  - Expectation: pWaiting set to NULL after processing; rc may be updated by lsmFsPagePersist
*/
static bool test_flush_waiting_single_page_rc_ok() {
  FileSystem fs;
  Page page1;

  // Prepare a single page in the waiting queue
  memset(&fs, 0, sizeof(fs));
  memset(&page1, 0, sizeof(page1));

  page1.nRef = 1;
  page1.pWaitingNext = NULL;

  fs.pWaiting = &page1;

  int rc_in = LSM_OK;
  lsmFsFlushWaiting(&fs, &rc_in);

  // Post-conditions:
  // - pWaiting should be NULL
  // - rc should be LSM_OK (assuming lsmFsPagePersist returned LSM_OK)
  bool ok = (fs.pWaiting == NULL) && (rc_in == LSM_OK);
  return ok;
}

/*
  Test 3: Multiple pages in waiting list with rc == LSM_OK
  - Setup: pFS->pWaiting -> pPage1 -> pPage2 -> NULL
  - Expectation: All pages processed; pWaiting becomes NULL; rc remains LSM_OK
*/
static bool test_flush_waiting_multiple_pages_rc_ok() {
  FileSystem fs;
  Page page1, page2;

  memset(&fs, 0, sizeof(fs));
  memset(&page1, 0, sizeof(page1));
  memset(&page2, 0, sizeof(page2));

  // Chain: page1 -> page2
  page1.nRef = 1;
  page1.pWaitingNext = &page2;

  page2.nRef = 1;
  page2.pWaitingNext = NULL;

  fs.pWaiting = &page1;

  int rc_in = LSM_OK;
  lsmFsFlushWaiting(&fs, &rc_in);

  // Post-conditions:
  bool ok = (fs.pWaiting == NULL) && (rc_in == LSM_OK);
  return ok;
}

int main() {
  // Run tests. Each test prints a pass/fail indicator via harness.
  // We use a simple boolean approach and report at the end.
  if (test_flush_waiting_empty_queue()) {
    printf("PASS: test_flush_waiting_empty_queue\n");
    ++g_tests_passed;
  } else {
    printf("FAIL: test_flush_waiting_empty_queue\n");
  }
  ++g_tests_run;

  if (test_flush_waiting_single_page_rc_ok()) {
    printf("PASS: test_flush_waiting_single_page_rc_ok\n");
    ++g_tests_passed;
  } else {
    printf("FAIL: test_flush_waiting_single_page_rc_ok\n");
  }
  ++g_tests_run;

  if (test_flush_waiting_multiple_pages_rc_ok()) {
    printf("PASS: test_flush_waiting_multiple_pages_rc_ok\n");
    ++g_tests_passed;
  } else {
    printf("FAIL: test_flush_waiting_multiple_pages_rc_ok\n");
  }
  ++g_tests_run;

  report_results();
  // Return non-zero on overall failure to indicate test suite status
  return (g_tests_passed == g_tests_run) ? 0 : 1;
}
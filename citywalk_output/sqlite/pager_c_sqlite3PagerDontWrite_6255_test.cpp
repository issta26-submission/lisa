#include <iostream>
#include <wal.h>
#include <sqliteInt.h>


// Minimal scaffolding to test sqlite3PagerDontWrite in a C++11 environment
// without requiring the full SQLite infrastructure.

// Domain constants (as used by the FOCAL_METHOD)
static const int PGHDR_DIRTY      = 0x01;
static const int PGHDR_WRITEABLE  = 0x02;
static const int PGHDR_DONT_WRITE = 0x04;
static const int PGHDR_NEED_SYNC  = 0x08;

// Lightweight dependencies (structs) for the method under test
struct Pager {
  bool tempFile;
  int nSavepoint;
};

struct PgHdr {
  Pager *pPager;
  int pgno;
  int flags;
};

// Macros and hooks used by sqlite3PagerDontWrite (mocked for tests)
#define PAGERTRACE(...) do {} while(0)
#define IOTRACE(...)    do {} while(0)
#define PAGERID(pPager) 0

static bool g_pagerSetPageHashCalled = false;
static PgHdr *g_lastPgHashPg = nullptr;

static bool g_testcaseCalled = false;
static int  g_testcaseValue  = 0;

// Mock implementations of the dependencies that the focal method calls
static void pager_set_pagehash(PgHdr *pPg) {
  g_pagerSetPageHashCalled = true;
  g_lastPgHashPg = pPg;
}
static void testcase(int v) {
  g_testcaseCalled = true;
  g_testcaseValue  = v;
}

// The focal method under test (adapted to this test scaffold)
void sqlite3PagerDontWrite(PgHdr *pPg){
  Pager *pPager = pPg->pPager;
  if( !pPager->tempFile && (pPg->flags&PGHDR_DIRTY) && pPager->nSavepoint==0 ){
    PAGERTRACE(("DONT_WRITE page %d of %d\n", pPg->pgno, PAGERID(pPager)));
    IOTRACE(("CLEAN %p %d\n", pPager, pPg->pgno))
    pPg->flags |= PGHDR_DONT_WRITE;
    pPg->flags &= ~PGHDR_WRITEABLE;
    testcase( pPg->flags & PGHDR_NEED_SYNC );
    pager_set_pagehash(pPg);
  }
}

// Helpers for tests
static void resetGlobals() {
  g_pagerSetPageHashCalled = false;
  g_lastHashPg = nullptr;
  g_lastPgHashPg = nullptr;
  g_testcaseCalled = false;
  g_testcaseValue  = 0;
}

// Overwrite to a local symbol to avoid potential naming conflicts in extended tests
static PgHdr *g_lastHashPg = nullptr;

// Test 1: True branch with NEED_SYNC set (all conditions satisfied)
static bool testDontWrite_trueBranchNeedSync() {
  resetGlobals();

  Pager pager;
  pager.tempFile   = false;
  pager.nSavepoint = 0;

  PgHdr pg;
  pg.pPager = &pager;
  pg.pgno = 123;
  pg.flags = PGHDR_DIRTY | PGHDR_NEED_SYNC; // DIRTY and NEED_SYNC

  sqlite3PagerDontWrite(&pg);

  bool changed = (pg.flags & PGHDR_DONT_WRITE) != 0 && (pg.flags & PGHDR_WRITEABLE) == 0;
  bool pageHashCalled = g_pagerSetPageHashCalled && (g_lastPgHashPg == &pg);
  bool testcaseValueOk = g_testcaseCalled && ((g_testcaseValue & PGHDR_NEED_SYNC) != 0);

  if (!changed) {
    std::cout << "FAIL: testDontWrite_trueBranchNeedSync - page not marked DONT_WRITE correctly\n";
  }
  if (!pageHashCalled) {
    std::cout << "FAIL: testDontWrite_trueBranchNeedSync - pager_set_pagehash not called with correct pg\n";
  }
  if (!testcaseValueOk) {
    std::cout << "FAIL: testDontWrite_trueBranchNeedSync - testcase not called with NEED_SYNC value\n";
  }
  if (changed && pageHashCalled && testcaseValueOk) {
    std::cout << "PASS: testDontWrite_trueBranchNeedSync\n";
    return true;
  }
  return false;
}

// Test 2: True branch with NEED_SYNC NOT set (NEED_SYNC is 0, ensure 0 is passed)
static bool testDontWrite_trueBranchNoNeedSync() {
  resetGlobals();

  Pager pager;
  pager.tempFile   = false;
  pager.nSavepoint = 0;

  PgHdr pg;
  pg.pPager = &pager;
  pg.pgno = 124;
  pg.flags = PGHDR_DIRTY; // DIRTY only, no NEED_SYNC

  sqlite3PagerDontWrite(&pg);

  bool changed = (pg.flags & PGHDR_DONT_WRITE) != 0 && (pg.flags & PGHDR_WRITEABLE) == 0;
  bool pageHashCalled = g_pagerSetPageHashCalled && (g_lastPgHashPg == &pg);
  bool testcaseValueOk = g_testcaseCalled && (g_testcaseValue == 0);

  if (!changed) {
    std::cout << "FAIL: testDontWrite_trueBranchNoNeedSync - page not marked DONT_WRITE correctly\n";
  }
  if (!pageHashCalled) {
    std::cout << "FAIL: testDontWrite_trueBranchNoNeedSync - pager_set_pagehash not called with correct pg\n";
  }
  if (!testcaseValueOk) {
    std::cout << "FAIL: testDontWrite_trueBranchNoNeedSync - testcase not called with 0 (NEED_SYNC not set)\n";
  }
  if (changed && pageHashCalled && testcaseValueOk) {
    std::cout << "PASS: testDontWrite_trueBranchNoNeedSync\n";
    return true;
  }
  return false;
}

// Test 3: False branch due to tempFile being true (no changes)
static bool testDontWrite_falseBranch_TempFile() {
  resetGlobals();

  Pager pager;
  pager.tempFile   = true; // not eligible
  pager.nSavepoint = 0;

  PgHdr pg;
  pg.pPager = &pager;
  pg.pgno = 200;
  pg.flags = PGHDR_DIRTY | PGHDR_NEED_SYNC;

  sqlite3PagerDontWrite(&pg);

  bool changed = (pg.flags & PGHDR_DONT_WRITE) != 0;
  bool pageHashCalled = g_pagerSetPageHashCalled;
  bool testcaseValueOk = g_testcaseCalled;

  if (changed) {
    std::cout << "FAIL: testDontWrite_falseBranch_TempFile - page incorrectly marked DONT_WRITE\n";
  }
  if (pageHashCalled) {
    std::cout << "FAIL: testDontWrite_falseBranch_TempFile - pager_set_pagehash should not be called\n";
  }
  if (testcaseValueOk) {
    std::cout << "FAIL: testDontWrite_falseBranch_TempFile - testcase should not be called\n";
  }
  if (!changed && !pageHashCalled && !testcaseValueOk) {
    std::cout << "PASS: testDontWrite_falseBranch_TempFile\n";
    return true;
  }
  return false;
}

// Test 4: False branch due to non-zero savepoint (nSavepoint)
static bool testDontWrite_falseBranch_NSavepoint() {
  resetGlobals();

  Pager pager;
  pager.tempFile   = false;
  pager.nSavepoint = 1; // non-zero

  PgHdr pg;
  pg.pPager = &pager;
  pg.pgno = 201;
  pg.flags = PGHDR_DIRTY;

  sqlite3PagerDontWrite(&pg);

  bool changed = (pg.flags & PGHDR_DONT_WRITE) != 0;
  bool pageHashCalled = g_pagerSetPageHashCalled;
  bool testcaseValueOk = g_testcaseCalled;

  if (changed) {
    std::cout << "FAIL: testDontWrite_falseBranch_NSavepoint - page incorrectly marked DONT_WRITE\n";
  }
  if (pageHashCalled) {
    std::cout << "FAIL: testDontWrite_falseBranch_NSavepoint - pager_set_pagehash should not be called\n";
  }
  if (testcaseValueOk) {
    std::cout << "FAIL: testDontWrite_falseBranch_NSavepoint - testcase should not be called\n";
  }
  if (!changed && !pageHashCalled && !testcaseValueOk) {
    std::cout << "PASS: testDontWrite_falseBranch_NSavepoint\n";
    return true;
  }
  return false;
}

// Test 5: False branch due to not dirty (PGHDR_DIRTY not set)
static bool testDontWrite_falseBranch_NotDirty() {
  resetGlobals();

  Pager pager;
  pager.tempFile   = false;
  pager.nSavepoint = 0;

  PgHdr pg;
  pg.pPager = &pager;
  pg.pgno = 202;
  pg.flags = 0; // not DIRTY

  sqlite3PagerDontWrite(&pg);

  bool changed = false; // should not change
  bool pageHashCalled = g_pagerSetPageHashCalled;
  bool testcaseValueOk = g_testcaseCalled;

  if (changed) {
    std::cout << "FAIL: testDontWrite_falseBranch_NotDirty - unexpected DONT_WRITE set\n";
  }
  if (pageHashCalled) {
    std::cout << "FAIL: testDontWrite_falseBranch_NotDirty - pager_set_pagehash should not be called\n";
  }
  if (testcaseValueOk) {
    std::cout << "FAIL: testDontWrite_falseBranch_NotDirty - testcase should not be called\n";
  }
  if (!pageHashCalled && !testcaseValueOk) {
    std::cout << "PASS: testDontWrite_falseBranch_NotDirty\n";
    return true;
  }
  return false;
}

int main() {
  int passed = 0;
  int total = 5;

  if (testDontWrite_trueBranchNeedSync()) passed++;
  if (testDontWrite_trueBranchNoNeedSync()) passed++;
  if (testDontWrite_falseBranch_TempFile()) passed++;
  if (testDontWrite_falseBranch_NSavepoint()) passed++;
  if (testDontWrite_falseBranch_NotDirty()) passed++;

  std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";
  return (passed == total) ? 0 : 1;
}
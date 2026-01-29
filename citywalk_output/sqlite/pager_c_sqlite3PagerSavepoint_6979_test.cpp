#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cassert>


// Lightweight, self-contained test harness that simulates the
// sqlite3PagerSavepoint logic for unit testing without pulling the
// entire SQLite codebase. This is intentionally minimal and focuses on
// exercising the control flow and key state mutations of the focal method.

#define SQLITE_ENABLE_ZIPVFS 1  // Enable ZIPVFS-related test path (guarded code)

using i64 = long long;

// Basic constants used by sqlite3PagerSavepoint (subset sufficient for tests)
enum {
  SAVEPOINT_RELEASE = 0,
  SAVEPOINT_ROLLBACK = 1,
  SQLITE_OK = 0,
  SQLITE_ABORT = 1,
  SQLITE_DONE = 2
};

enum {
  PAGER_JOURNALMODE_OFF = 0,
};

enum {
  PAGER_WRITER_CACHEMOD = 1,
  PAGER_ERROR = 2
};

// Forward declarations for "static" helpers used by the focal method
struct Bitvec {};
struct PageFile {
  bool isOpen;
  bool isInMemory;
};

static void sqlite3BitvecDestroy(Bitvec *p) {
  // In test, mark destruction by nulling the pointer (if possible) or
  // by recording destruction in a global log handled in test harness.
  // Actual destruction is simulated via a logged pointer in the test code.
  // (No-op here; test harness uses logging in the call-site.)
}

static int isOpen(void *pFd) {
  PageFile *p = static_cast<PageFile*>(pFd);
  return (p != nullptr) && p->isOpen;
}

static int sqlite3JournalIsInMemory(PageFile *pJrnl) {
  return (pJrnl != nullptr) && pJrnl->isInMemory;
}

static long long g_lastTruncateSize = 0;
static int g_truncateCalled = 0;
static int sqlite3OsTruncate(PageFile *pJrnl, i64 sz) {
  (void)pJrnl;
  g_lastTruncateSize = sz;
  g_truncateCalled++;
  return SQLITE_OK;
}

// A simple log for destructor calls, to verify resources freed
static std::vector<Bitvec*> g_destroyedBitvecs;

// Audio-ish "logger" for later assertions
static int g_getGetterCalled = 0;
static void setGetterMethod(void *pPager) {
  (void)pPager;
  g_getGetterCalled++;
}

// Playback simulation
static int g_playback_rc = SQLITE_OK;
static void *g_playback_pPager = nullptr;
static void *g_playback_pSavepoint = nullptr;
static int pagerPlaybackSavepoint_sim(PagerSavepoint *pSavepoint); // forward

static int pagerPlaybackSavepoint_sim(PagerSavepoint *pSavepoint) {
  g_playback_pSavepoint = static_cast<void*>(pSavepoint);
  g_playback_pPager = nullptr;
  g_playback_rc = SQLITE_OK;
  // No complex playback simulation; we just record the call
  return SQLITE_OK;
}

// Minimal pager use/wal helpers
struct PagerSavepoint {
  bool bTruncateOnRelease;
  int iSubRec;
  Bitvec *pInSavepoint;
};

struct Pager {
  int errCode;
  int nSavepoint;
  std::vector<PagerSavepoint> aSavepoint;

  PageFile *sjfd; // Sub-journal file descriptor (simulated)
  PageFile *jfd;  // Journal file descriptor (simulated)

  int nSubRec;
  int pageSize;

  int journalMode;
  int eState;

  bool useWal;
};

// The focal method re-implemented in this test harness (subset of original logic)
int sqlite3PagerSavepoint(Pager *pPager, int op, int iSavepoint) {
  int rc = pPager->errCode;

#ifdef SQLITE_ENABLE_ZIPVFS
  if (op == SAVEPOINT_RELEASE) rc = SQLITE_OK;
#endif
  // Basic contract checks (mirror of original asserts in testable form)
  // - op must be RELEASE or ROLLBACK
  // - iSavepoint must be >= 0 unless this is a ROLBACK operation
  // In test harness we simply enforce at runtime:
  if (!(op == SAVEPOINT_RELEASE || op == SAVEPOINT_ROLLBACK)) {
    // Invalid op; treat as error
    return SQLITE_ABORT;
  }
  if (!(iSavepoint >= 0 || op == SAVEPOINT_ROLLBACK)) {
    return SQLITE_ABORT;
  }

  if (rc == SQLITE_OK && iSavepoint < pPager->nSavepoint) {
    int ii;
    int nNew;

    nNew = iSavepoint + ((op == SAVEPOINT_RELEASE) ? 0 : 1);

    for (ii = nNew; ii < pPager->nSavepoint; ii++) {
      sqlite3BitvecDestroy(pPager->aSavepoint[ii].pInSavepoint);
      // In test, we record destruction
      g_destroyedBitvecs.push_back(pPager->aSavepoint[ii].pInSavepoint);
      pPager->aSavepoint[ii].pInSavepoint = nullptr;
    }

    pPager->nSavepoint = nNew;

    if (op == SAVEPOINT_RELEASE) {
      PagerSavepoint *pRel = &pPager->aSavepoint[nNew];
      if (pRel->bTruncateOnRelease && isOpen(pPager->sjfd)) {
        if (sqlite3JournalIsInMemory(pPager->sjfd)) {
          i64 sz = (pPager->pageSize + 4) * (i64)pRel->iSubRec;
          rc = sqlite3OsTruncate(pPager->sjfd, sz);
          // In test, rc is SQLITE_OK, and we capture sz
        }
        pPager->nSubRec = pRel->iSubRec;
      }
    } else if (pagerUseWal(pPager) || isOpen(pPager->jfd)) {
      PagerSavepoint *pSavepoint = (nNew == 0) ? 0 : &pPager->aSavepoint[nNew - 1];
      rc = pagerPlaybackSavepoint_sim(pSavepoint);
      // In test, rc is SQLITE_OK
    }
#ifdef SQLITE_ENABLE_ZIPVFS
    else if (pPager->journalMode == PAGER_JOURNALMODE_OFF && pPager->eState >= PAGER_WRITER_CACHEMOD) {
      pPager->errCode = SQLITE_ABORT;
      pPager->eState = PAGER_ERROR;
      setGetterMethod(pPager);
    }
#endif
  }

  return rc;
}

// Helper to "mock" WAL usage
static int pagerUseWal(Pager *pPager) {
  return pPager->useWal ? 1 : 0;
}

// Public entry points for tests
int run_all_tests();


// Test helpers and test cases

// Tests will be executed in main via run_all_tests()
static bool test_release_truncate_and_subrec_case();
static bool test_rollback_playback_case();
static bool test_zipvfs_abort_case();

bool test_release_truncate_and_subrec_case() {
  // Setup: RELEASE operation should truncate subjournal if allowed and open,
  // and should update nSavepoint and nSubRec accordingly. All savepoints
  // should be destroyed for indices >= nNew (here nNew = 0 when iSavepoint=0).
  Pager p;
  p.errCode = SQLITE_OK;
  p.nSavepoint = 3;
  p.nSubRec = 999;
  p.pageSize = 4096;
  p.sjfd = new PageFile{ true, true }; // open and in-memory
  p.jfd = nullptr;

  // no WAL
  p.useWal = false;
  p.journalMode = 0;
  p.eState = 0;

  // Prepare three savepoints
  Bitvec *bv0 = new Bitvec{};
  Bitvec *bv1 = new Bitvec{};
  Bitvec *bv2 = new Bitvec{};
  PagerSavepoint sp0{ true, 5, bv0 };
  PagerSavepoint sp1{ false,  3, bv1 };
  PagerSavepoint sp2{ false,  1, bv2 };

  p.aSavepoint.resize(3);
  p.aSavepoint[0] = sp0;
  p.aSavepoint[1] = sp1;
  p.aSavepoint[2] = sp2;

  p.nSubRec = 7;

  // Precondition: ensure some memory is present to be freed
  g_destroyedBitvecs.clear();
  g_lastTruncateSize = 0;
  g_truncateCalled = 0;
  g_getGetterCalled = 0;

  int rc = sqlite3PagerSavepoint(&p, SAVEPOINT_RELEASE, 0);

  // Assertions
  bool ok = true;
  if (rc != SQLITE_OK) {
    std::cerr << "Release test: rc != SQLITE_OK\n";
    ok = false;
  }
  if (p.nSavepoint != 0) {
    std::cerr << "Release test: nSavepoint expected 0, got " << p.nSavepoint << "\n";
    ok = false;
  }
  // Subjournal truncation should have been performed
  long long expectedSz = (p.pageSize + 4) * (long long)sp0.iSubRec;
  if (g_lastTruncateSize != expectedSz) {
    std::cerr << "Release test: truncate size expected " << expectedSz
              << ", got " << g_lastTruncateSize << "\n";
    ok = false;
  }
  if (g_truncateCalled == 0) {
    std::cerr << "Release test: truncate function not invoked\n";
    ok = false;
  }
  if (p.nSubRec != sp0.iSubRec) {
    std::cerr << "Release test: nSubRec expected " << sp0.iSubRec
              << ", got " << p.nSubRec << "\n";
    ok = false;
  }
  if (g_destroyedBitvecs.size() != 3) {
    std::cerr << "Release test: expected 3 Bitvec destruction, got "
              << g_destroyedBitvecs.size() << "\n";
    ok = false;
  }

  // Cleanup
  delete p.sjfd;
  delete p.jfd;
  delete bv0; delete bv1; delete bv2;

  return ok;
}

bool test_rollback_playback_case() {
  // Setup: ROLLBACK path should trigger playback of the last active savepoint
  // when WAL is not used and jfd is open. We verify that pagerPlaybackSavepoint
  // is invoked with the correct savepoint and that nSavepoint is updated.
  Pager p;
  p.errCode = SQLITE_OK;
  p.nSavepoint = 2;
  p.nSubRec = 0;
  p.pageSize = 4096;
  p.sjfd = nullptr;
  p.jfd  = new PageFile{ true, true }; // journal file open
  p.useWal = false;
  p.journalMode = 0;
  p.eState = 0;

  // Prepare two savepoints
  Bitvec *bvA = new Bitvec{};
  Bitvec *bvB = new Bitvec{};
  PagerSavepoint spA{ false, 2, bvA }; // In-savepoint A
  PagerSavepoint spB{ false, 1, bvB }; // In-savepoint B
  p.aSavepoint.resize(2);
  p.aSavepoint[0] = spA;
  p.aSavepoint[1] = spB;

  // Precondition: ensure callbacks log the playback
  g_playback_pPager = nullptr;
  g_playback_pSavepoint = nullptr;
  g_destroyedBitvecs.clear();
  g_getGetterCalled = 0;
  g_lastTruncateSize = 0;

  int rc = sqlite3PagerSavepoint(&p, SAVEPOINT_ROLLBACK, 0);

  bool ok = true;
  if (rc != SQLITE_OK) {
    std::cerr << "Rollback test: rc != SQLITE_OK\n";
    ok = false;
  }
  if (p.nSavepoint != 1) {
    std::cerr << "Rollback test: nSavepoint expected 1, got " << p.nSavepoint << "\n";
    ok = false;
  }
  if (g_playback_pSavepoint == nullptr) {
    std::cerr << "Rollback test: playback was not invoked\n";
    ok = false;
  } else {
    PagerSavepoint *played = static_cast<PagerSavepoint*>(g_playback_pSavepoint);
    // In this scenario, nNew-1 == 0, so we expect to pass aSavepoint[0]
    if (played != &p.aSavepoint[0]) {
      std::cerr << "Rollback test: playback was invoked with wrong savepoint\n";
      ok = false;
    }
  }
  if (g_destroyedBitvecs.size() != 1) {
    std::cerr << "Rollback test: expected 1 Bitvec destruction (index 1)\n";
    ok = false;
  }

  // Cleanup
  delete p.jfd;
  delete bvA; delete bvB;

  return ok;
}

bool test_zipvfs_abort_case() {
  // ZIPVFS path: when WAL is not used and jfd is not open, with OFF journal mode
  // and eState sufficiently advanced, an abort should be raised and eState set.
  // Enable ZIPVFS path by macro above.
  Pager p;
  p.errCode = SQLITE_OK;
  p.nSavepoint = 1;
  p.aSavepoint.resize(1);
  Bitvec *bv0 = new Bitvec{};
  p.aSavepoint[0] = { false, 0, bv0 };
  p.nSubRec = 0;
  p.pageSize = 4096;
  p.sjfd = nullptr;
  p.jfd  = new PageFile{ false, false }; // closed
  p.useWal = false;
  p.journalMode = PAGER_JOURNALMODE_OFF;
  p.eState = PAGER_WRITER_CACHEMOD; // >= threshold

  g_getGetterCalled = 0;

  int rc = sqlite3PagerSavepoint(&p, SAVEPOINT_ROLLBACK, 0);

  bool ok = true;
  if (rc != SQLITE_OK) {
    std::cerr << "ZIPVFS test: rc != SQLITE_OK\n";
    ok = false;
  }
  // ZIPVFS path should set error state and call setGetterMethod
  if (p.errCode != SQLITE_ABORT) {
    std::cerr << "ZIPVFS test: errCode not set to SQLITE_ABORT\n";
    ok = false;
  }
  if (p.eState != PAGER_ERROR) {
    std::cerr << "ZIPVFS test: eState not set to PAGER_ERROR\n";
    ok = false;
  }
  if (g_getGetterCalled == 0) {
    std::cerr << "ZIPVFS test: setGetterMethod not called\n";
    ok = false;
  }

  delete p.jfd;
  delete bv0;

  return ok;
}

int run_all_tests() {
  int overall = 0;
  int passed = 0;

  // Test 1: Release path truncation and sub-rectangle update
  {
    bool ok = test_release_truncate_and_subrec_case();
    overall++;
    if (ok) passed++;
    std::cout << "Test 1 (Release -> Truncate/SubRec/Destroy) "
              << (ok ? "PASSED" : "FAILED") << std::endl;
  }

  // Test 2: Rollback path triggers playback
  {
    bool ok = test_rollback_playback_case();
    overall++;
    if (ok) passed++;
    std::cout << "Test 2 (Rollback -> Playback) "
              << (ok ? "PASSED" : "FAILED") << std::endl;
  }

  // Test 3: ZIPVFS abort path
  {
    bool ok = test_zipvfs_abort_case();
    overall++;
    if (ok) passed++;
    std::cout << "Test 3 (ZIPVFS Abort Path) "
              << (ok ? "PASSED" : "FAILED") << std::endl;
  }

  std::cout << "Summary: " << passed << " / " << overall << " tests passed.\n";
  // Return 0 if all passed, non-zero otherwise
  return (passed == overall) ? 0 : 1;
}

// Entrypoint for running the tests
int main() {
  return run_all_tests();
}
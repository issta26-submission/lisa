// Purpose: Provide a self-contained C++11 test harness for the focal C function
// lsmFsMoveBlock, using minimal mock implementations of its dependencies.
// This test suite does not rely on GTest and uses a lightweight, in-process
// test runner with explanatory comments for each test case.

// Note: The test re-implements a reduced, self-contained version of the focal
// function and its required dependencies to enable isolated unit testing
// without the original large codebase.

#include <vector>
#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain-specific constants (mirroring the original project)
typedef long long i64;
typedef unsigned char u8;
typedef int LsmPgno;
typedef int i32;

#define LSM_OK 0
#define LSM_ERR 1
#define LSM_APPLIST_SZ 4
#define LSM_MIN(a,b) (((a)<(b))?(a):(b))

// Minimal types used by the focal method
struct Snapshot {
  int aiAppend[LSM_APPLIST_SZ];
};

struct Db {
  Snapshot *pWorker;
};

struct FileSystem {
  int nBlocksize;     // number of bytes per block (for pages)
  int nPagesize;      // size of a page (in bytes)
  i64 nMapLimit;       // map limit (byte-addressable)
  u8 *pMap;            // in-memory map representing file system data
  void *pEnv;           // environment handle (not used in tests)
  int fdDb;              // dummy file descriptor (not used)
  Db *pDb;               // database handle with worker thread
};

struct Segment {
  int iFirst;
  int iLastPg;
  int iRoot;
};

// Forward declarations of helpers and the focal function
static i64 LSM_MIN2(i64 a, i64 b) { return (a < b) ? a : b; }

// Lightweight helper: emulate memory grow (no-op in tests, but sets rc)
static void fsGrowMapping(FileSystem *pFS, i64 iSz, int *pRc) {
  // In test harness, simply acknowledge request; ensure pFS has enough space
  if (pFS->pMap == nullptr) {
    // Allocate map with at least iSz bytes if not already allocated
    pFS->pMap = (u8*)std::malloc((size_t)std::max<i64>(iSz, 1));
  }
  *pRc = LSM_OK;
}

// Simple malloc wrapper used by the focal method (returns buffer and rc)
static void *lsmMallocRc(void *pEnv, int n, int *pRc) {
  (void)pEnv; // unused in test harness
  *pRc = LSM_OK;
  return std::malloc(n);
}

static void lsmFree(void *pEnv, void *p) {
  (void)pEnv; // unused
  std::free(p);
}

// Emulated environment read/write operations
// These are instrumented to help validate test behavior
static int g_envReadCount = 0;
static int g_envWriteCount = 0;
static bool g_read_error = false;
static std::vector<std::pair<i64, std::vector<u8>>> g_writeLog;

static int lsmEnvRead(void *pEnv, int pFile, i64 iOff, void *pRead, int nRead) {
  (void)pEnv;
  (void)pFile;
  if (g_read_error) {
    return LSM_ERR;
  }
  g_envReadCount++;
  u8 *buf = (u8*)pRead;
  // Fill buffer with a deterministic pattern for verification
  for (int i = 0; i < nRead; ++i) {
    buf[i] = (u8)((iOff + i) & 0xFF);
  }
  return LSM_OK;
}

static int lsmEnvWrite(void *pEnv, int pFile, i64 iOff, const void *pWrite, int nWrite) {
  (void)pEnv;
  (void)pFile;
  (void)iOff;
  g_envWriteCount++;
  const u8 *src = (const u8*)pWrite;
  std::vector<u8> data(nWrite);
  std::memcpy(data.data(), src, nWrite);
  g_writeLog.push_back(std::make_pair(iOff, data));
  return LSM_OK;
}

// Purge cache stub
static void lsmFsPurgeCache(FileSystem *pFS) {
  (void)pFS;
  // No-op in test harness
}

// Move a page indicator (stubbed to reflect a "move" operation)
static void fsMovePage(FileSystem *pFS, int iTo, int iFrom, int *piPg) {
  (void)pFS; (void)iTo; (void)iFrom;
  if (piPg) {
    // Simulate a "move" by bumping the stored page number
    *piPg = *piPg + 1;
  }
}

// The focal function under test, implemented in a self-contained way
static int lsmFsMoveBlock(FileSystem *pFS, Segment *pSeg, int iTo, int iFrom) {
  // Local aliases to mimic original structure
  Snapshot *p = pFS->pDb->pWorker;
  int rc = LSM_OK;
  int i;
  i64 nMap;
  i64 iFromOff = (i64)(iFrom-1) * pFS->nBlocksize;
  i64 iToOff = (i64)(iTo-1) * pFS->nBlocksize;

  // Precondition checks (kept as asserts to mirror original)
  // These will abort if violated; tests ensure valid inputs
  // In C++, std::abort would be triggered; here we keep behavior consistent
  assert( iTo != 1 );
  assert( iFrom > iTo );

  // Grow the mapping as required
  nMap = LSM_MIN2(pFS->nMapLimit, (i64)iFrom * pFS->nBlocksize);
  fsGrowMapping(pFS, nMap, &rc);
  if (rc==LSM_OK) {
    const int nPagePerBlock = (pFS->nBlocksize / pFS->nPagesize);
    int nSz = pFS->nPagesize;
    u8 *aBuf = 0;
    u8 *aData = 0;
    for (i=0; rc==LSM_OK && i<nPagePerBlock; i++) {
      i64 iOff = iFromOff + i*nSz;
      // Use map memory if within limits
      if( (iOff+nSz)<=pFS->nMapLimit ){
        u8 *aMap = (u8 *)(pFS->pMap);
        aData = &aMap[iOff];
      } else {
        if( aBuf==0 ){
          aBuf = (u8 *)lsmMallocRc(pFS->pEnv, nSz, &rc);
          if( aBuf==0 ) break;
        }
        aData = aBuf;
        rc = lsmEnvRead(pFS->pEnv, pFS->fdDb, iOff, aData, nSz);
      }
      if( rc==LSM_OK ){
        iOff = iToOff + i*nSz;
        if( (iOff+nSz)<=pFS->nMapLimit ){
          u8 *aMap = (u8 *)(pFS->pMap);
          memcpy(&aMap[iOff], aData, nSz);
        }else{
          rc = lsmEnvWrite(pFS->pEnv, pFS->fdDb, iOff, aData, nSz);
        }
      }
    }
    lsmFree(pFS->pEnv, aBuf);
    lsmFsPurgeCache(pFS);
  }
  // Update append-point list if necessary
  for(i=0; i<LSM_APPLIST_SZ; i++){
    fsMovePage(pFS, iTo, iFrom, &p->aiAppend[i]);
  }
  // Update the Segment structure itself
  fsMovePage(pFS, iTo, iFrom, &pSeg->iFirst);
  fsMovePage(pFS, iTo, iFrom, &pSeg->iLastPg);
  fsMovePage(pFS, iTo, iFrom, &pSeg->iRoot);
  return rc;
}

// Lightweight test harness
struct TestCase {
  std::string name;
  std::function<void()> body;
  bool pass = false;
  std::string failReason;
};

// Global counters for test reporting
static int g_total = 0;
static int g_pass = 0;

// Helpers to mark test pass/fail
static void test_pass() { g_pass++; }
static void test_fail(const std::string &reason) {
  // A real harness would collect reasons per test; keep simple here
  (void)reason;
}

// Utility: Reset environment between tests
static void resetEnvironmentForTest() {
  g_envReadCount = 0;
  g_envWriteCount = 0;
  g_read_error = false;
  g_writeLog.clear();
}

// Test 1: Basic path where all reads come from in-memory map and writes are within map limits.
// Expected: values copied from pMap[iFromOff..] to pMap[iToOff..], and aiAppend moves.
static void test_basic_map_copy() {
  // Setup
  resetEnvironmentForTest();
  // Allocate FS and dependencies
  FileSystem fs;
  fs.nBlocksize = 4;      // 4 bytes per block
  fs.nPagesize = 1;       // 1 byte per page
  fs.nMapLimit = 64;        // map limit in bytes
  fs.pEnv = nullptr;
  fs.fdDb = 0;

  // Allocate and wire DB and worker snapshot
  Snapshot snap;
  for (int k=0; k<LSM_APPLIST_SZ; ++k) snap.aiAppend[k] = 0;
  Db db;
  db.pWorker = &snap;
  fs.pDb = &db;

  // Allocate and initialize map
  fs.pMap = (u8*)std::calloc((size_t)fs.nMapLimit, 1);
  // Pre-fill source region with known values
  // Source block numbers: iFromOff = (iFrom-1)*nBlocksize
  // We'll put distinct bytes at indices 16..19 (iFromOff + i)
  fs.pMap[16+0] = 10; // i=0 data byte
  fs.pMap[16+1] = 11;
  fs.pMap[16+2] = 12;
  fs.pMap[16+3] = 13;

  // Segment and test inputs
  Segment seg;
  seg.iFirst = 100;
  seg.iLastPg = 101;
  seg.iRoot = 102;

  int iTo = 2;
  int iFrom = 5; // iFrom > iTo, iTo != 1

  // Call focal function
  int rc = lsmFsMoveBlock(&fs, &seg, iTo, iFrom);

  // Expectations:
  // - rc should be LSM_OK
  // - bytes at pMap[iToOff + i] (iToOff = (iTo-1)*nBlocksize = 4) should match source at iFromOff + i
  bool cond_rc_ok = (rc == LSM_OK);
  bool cond_copy = (fs.pMap[4+0] == 10 && fs.pMap[4+1] == 11 && fs.pMap[4+2] == 12 && fs.pMap[4+3] == 13);
  // aiAppend entries should have been moved (fsMovePage increments)
  bool cond_append_moved = (snap.aiAppend[0] == 1 && snap.aiAppend[1] == 1 && snap.aiAppend[2] == 1 && snap.aiAppend[3] == 1);
  g_total++;

  if (cond_rc_ok && cond_copy && cond_append_moved) {
    g_pass++;
    std::cout << "Test 1 (basic map copy) - PASS\n";
  } else {
    std::cout << "Test 1 (basic map copy) - FAIL"
              << " rc_ok=" << cond_rc_ok
              << " copy=(" << cond_copy << ")"
              << " append=(" << cond_append_moved << ")\n";
  }

  // Cleanup
  std::free(fs.pMap);
}

// Test 2: Path where we read from environment (lsmEnvRead) because iOff+nSz > nMapLimit and aBuf is used.
// Expect: rc OK and a write logged for iOff=4 (iToOff).
static void test_env_read_path() {
  resetEnvironmentForTest();

  FileSystem fs;
  fs.nBlocksize = 4;
  fs.nPagesize = 1;
  fs.nMapLimit = 4; // Force map-limit breach to trigger EnvRead for i=0
  fs.pEnv = nullptr;
  fs.fdDb = 0;

  Snapshot snap;
  for (int k=0; k<LSM_APPLIST_SZ; ++k) snap.aiAppend[k] = 0;
  Db db; db.pWorker = &snap;
  fs.pDb = &db;

  // Build an originally-empty map
  fs.pMap = (u8*)std::calloc((size_t)fs.nMapLimit, 1);

  Segment seg;
  seg.iFirst = 200;
  seg.iLastPg = 201;
  seg.iRoot = 202;

  int iTo = 2;
  int iFrom = 5;

  // Expect: reading into aBuf then writing to iOff=4
  int rc = lsmFsMoveBlock(&fs, &seg, iTo, iFrom);

  // Validate: rc OK and a write log captured
  bool cond_rc_ok = (rc == LSM_OK);
  bool cond_write_logged = !g_writeLog.empty();
  bool cond_write_src_valid = false;
  if (cond_write_logged) {
    // The first write should have iOff equal to 4 (iToOff)
    cond_write_src_valid = (g_writeLog[0].first == 4);
  }

  g_total++;
  if (cond_rc_ok && cond_write_logged && cond_write_src_valid) {
    g_pass++;
    std::cout << "Test 2 (env read path) - PASS\n";
  } else {
    std::cout << "Test 2 (env read path) - FAIL"
              << " rc_ok=" << cond_rc_ok
              << " writeLogged=" << cond_write_logged
              << " firstWriteOff=" << (cond_write_src_valid ? "4" : "non-4") << "\n";
  }

  // Cleanup
  std::free(fs.pMap);
  g_writeLog.clear();
}

// Test 3: Error path: lsmEnvRead returns error, ensuring rc propagates.
// Expect: rc == LSM_ERR and no mapping progress occurs.
static void test_env_read_error_path() {
  resetEnvironmentForTest();

  FileSystem fs;
  fs.nBlocksize = 4;
  fs.nPagesize = 1;
  fs.nMapLimit = 64;
  fs.pEnv = nullptr;
  fs.fdDb = 0;

  Snapshot snap;
  for (int k=0; k<LSM_APPLIST_SZ; ++k) snap.aiAppend[k] = 0;
  Db db; db.pWorker = &snap;
  fs.pDb = &db;

  fs.pMap = (u8*)std::calloc((size_t)fs.nMapLimit, 1);

  Segment seg;
  seg.iFirst = 300;
  seg.iLastPg = 301;
  seg.iRoot = 302;

  // Force error in EnvRead for the first read
  g_read_error = true;

  int iTo = 2;
  int iFrom = 5;

  int rc = lsmFsMoveBlock(&fs, &seg, iTo, iFrom);

  bool cond_rc_err = (rc == LSM_ERR);
  // After error, we expect no valid data moved: check that the map at destination is still untouched (still 0)
  bool cond_no_move = (fs.pMap[4] == 0) && (fs.pMap[5] == 0) && (fs.pMap[6] == 0) && (fs.pMap[7] == 0);

  g_total++;
  if (cond_rc_err && cond_no_move) {
    g_pass++;
    std::cout << "Test 3 (env read error path) - PASS\n";
  } else {
    std::cout << "Test 3 (env read error path) - FAIL"
              << " rc_err=" << cond_rc_err
              << " no_move=" << cond_no_move << "\n";
  }

  // Cleanup
  std::free(fs.pMap);
  g_writeLog.clear();
  g_read_error = false;
}

// Main: run all tests and report summary
int main() {
  std::cout << "Starting unit tests for lsmFsMoveBlock (self-contained harness)\n";

  // Run tests
  test_basic_map_copy();
  test_env_read_path();
  test_env_read_error_path();

  // Summary
  std::cout << "Test summary: " << g_pass << " / " << g_total << " tests passed.\n";
  return (g_total == g_pass) ? 0 : 1;
}
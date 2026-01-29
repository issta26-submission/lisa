// Lightweight C++11 test suite for sqlite3WalHeapMemory
// This test reproduces the minimal WAL structures required to exercise the focal
// function without pulling in the entire SQLite test framework.
// The tests are non-terminating and will report pass/fail per test case.
// Note: Link with the actual sqlite3 Wal implementation (e.g., wal.c) when compiling.

#include <Windows.h>
#include <cstddef>
#include <iostream>
#include <wal.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Reproduced minimal WAL types and constants to match the focal function's layout
// This mirrors the structure layout used by sqlite3WalHeapMemory in wal.c.
// -----------------------------------------------------------------------------

// Macros to mirror the dependent declarations in wal.c (simplified)
#define SQLITE_SHM_NLOCK 6
#define WAL_NREADER (SQLITE_SHM_NLOCK-3)
#define WAL_HEAPMEMORY_MODE 2

// Forward declarations to resemble sqlite3_vfs and sqlite3_file used in Wal
struct sqlite3_vfs;
struct sqlite3_file;

// Basic fixed-width types to match the original code
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

// Use standard 64-bit type alias to mirror i64 in the original code
typedef int64_t i64;

// WalIndexHdr as in the provided <FOCAL_CLASS_DEP>
struct WalIndexHdr {
  u32 iVersion;
  u32 unused;
  u32 iChange;
  u8 isInit;
  u8 bigEndCksum;
  u16 szPage;
  u32 mxFrame;
  u32 nPage;
  u32 aFrameCksum[2];
  u32 aSalt[2];
  u32 aCksum[2];
};

// WalCkptInfo as in the provided <FOCAL_CLASS_DEP>
struct WalCkptInfo {
  u32 nBackfill;
  u32 aReadMark[WAL_NREADER];
  u8 aLock[SQLITE_SHM_NLOCK];
  u32 nBackfillAttempted;
  u32 notUsed0;
};

// Optional macros used by the original code for layout constants
#define READMARK_NOT_USED  0xffffffff
#define WALINDEX_LOCK_OFFSET (sizeof(WalIndexHdr)*2+offsetof(WalCkptInfo,aLock))
#define WALINDEX_HDR_SIZE    (sizeof(WalIndexHdr)*2+sizeof(WalCkptInfo))
#define WAL_FRAME_HDRSIZE 24
#define WAL_HDRSIZE 32
#define WAL_MAGIC 0x377f0682

// Forward declare the Wal typedef to match the original code's usage
typedef struct Wal Wal;

// The full Wal structure as seen in the original wal.c excerpt
struct Wal {
  struct sqlite3_vfs *pVfs;
  struct sqlite3_file *pDbFd;
  struct sqlite3_file *pWalFd;
  u32 iCallback;
  i64 mxWalSize;
  int nWiData;
  int szFirstBlock;
  volatile u32 **apWiData;
  u32 szPage;
  int16_t readLock;
  u8 syncFlags;
  u8 exclusiveMode;
  u8 writeLock;
  u8 ckptLock;
  u8 readOnly;
  u8 truncateOnCommit;
  u8 syncHeader;
  u8 padToSectorBoundary;
  u8 bShmUnreliable;
  WalIndexHdr hdr;
  u32 minFrame;
  u32 iReCksum;
  const char *zWalName;
  u32 nCkpt;
#ifdef SQLITE_USE_SEH
  u32 lockMask;
  void *pFree;
  u32 *pWiValue;
  int iWiPg;
  int iSysErrno;
#endif
#ifdef SQLITE_DEBUG
  int nSehTry;
  u8 lockError;
#endif
#ifdef SQLITE_ENABLE_SNAPSHOT
  WalIndexHdr *pSnapshot;
#endif
#ifdef SQLITE_ENABLE_SETLK_TIMEOUT
  struct sqlite3 *db;
#endif
};

// Function prototype of the focal method (as in the real wal.c)
extern "C" int sqlite3WalHeapMemory(Wal *pWal);

// -----------------------------------------------------------------------------
// Minimal test harness (non-GTest, non-GMock)
// Provides non-terminating EXPECT-like macros and a small runner.
// -----------------------------------------------------------------------------

#define EXPECT_TRUE(cond)  do { tests_run++; if(!(cond)) { failures++; \
  std::cerr << "[FAIL] " #cond " is false at " __FILE__ ":" << __LINE__ << "\n"; } \
} while(0)

#define EXPECT_FALSE(cond) do { tests_run++; if((cond)) { failures++; \
  std::cerr << "[FAIL] " #cond " is true at " __FILE__ ":" << __LINE__ << "\n"; } \
} while(0)

static int tests_run = 0;
static int failures  = 0;

// Helper to summarize results
void summarize(const char* suiteName) {
  std::cout << "Suite: " << suiteName << " -> "
            << (tests_run - failures) << "/" << tests_run
            << " tests passed, " << failures << " failed.\n";
}

// -----------------------------------------------------------------------------
// Test cases for sqlite3WalHeapMemory
// We test true/false branches and nullptr handling as per the function logic.
// -----------------------------------------------------------------------------

// Test 1: Null pointer should yield false (0)
bool test_sqlite3WalHeapMemory_nullptr() {
  int r = sqlite3WalHeapMemory(nullptr);
  EXPECT_FALSE(r != 0);
  return (r == 0);
}

// Test 2: pWal with exclusiveMode == WAL_HEAPMEMORY_MODE should yield true (non-zero)
bool test_sqlite3WalHeapMemory_heapMemoryMode() {
  Wal w;
  w.exclusiveMode = WAL_HEAPMEMORY_MODE;
  int r = sqlite3WalHeapMemory(&w);
  EXPECT_TRUE(r != 0);
  return (r != 0);
}

// Test 3: pWal with exclusiveMode != WAL_HEAPMEMORY_MODE should yield false
bool test_sqlite3WalHeapMemory_otherModes() {
  Wal w;
  w.exclusiveMode = 0;
  int r0 = sqlite3WalHeapMemory(&w);
  EXPECT_FALSE(r0 != 0);

  w.exclusiveMode = 1;
  int r1 = sqlite3WalHeapMemory(&w);
  EXPECT_FALSE(r1 != 0);

  // Also test an unrelated non-heap memory value
  w.exclusiveMode = 3;
  int r3 = sqlite3WalHeapMemory(&w);
  EXPECT_FALSE(r3 != 0);

  return (r0 == 0 && r1 == 0 && r3 == 0);
}

// -----------------------------------------------------------------------------
// main(): run tests from here to ensure compatibility with environments
// where a dedicated test runner is not available.
// -----------------------------------------------------------------------------

int main() {
  std::cout << "Starting sqlite3WalHeapMemory unit tests (non-GTest)."
            << std::endl;

  // Run tests and collect results
  bool t1 = test_sqlite3WalHeapMemory_nullptr();
  bool t2 = test_sqlite3WalHeapMemory_heapMemoryMode();
  bool t3 = test_sqlite3WalHeapMemory_otherModes();

  summarize("sqlite3WalHeapMemory");

  // Return non-zero if any test failed
  return (failures > 0) ? 1 : 0;
}
// Minimal C++11 test harness for sqlite3VdbeSorterNext
// This test targets the focal method described in the prompt.
// It uses lightweight, self-contained scaffolding to drive the
// else-branch (non-PMA) of sqlite3VdbeSorterNext.
// Notes:
// - This test assumes CURTYPE_SORTER is the value used by the focal code.
// - We avoid GTest/GMock; a tiny in-file test framework is implemented.
// - We do not rely on any private APIs beyond the declared structural layout
//   that sqlite3VdbeSorterNext inspects (as defined below for test purposes).

#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Provide C linkage for the focal function
extern "C" {
  // Forward declare the function under test
  struct sqlite3;
  struct VdbeCursor;

  // CURTYPE_SORTER is defined in the production headers; for test purposes we declare a value.
  // If the actual value differs in your build, adjust accordingly.
  #ifndef CURTYPE_SORTER
  #define CURTYPE_SORTER 2
  #endif

  // sqlite3VdbeSorterNext signature used by the tests
  int sqlite3VdbeSorterNext(struct sqlite3 *db, const struct VdbeCursor *pCsr);
}

// Lightweight, test-scoped type definitions that mirror the fields accessed by sqlite3VdbeSorterNext.
// These do not aim to be complete SQLite structures; only the minimal layout is required for testing.

struct sqlite3 {
  int dummy; // placeholder
};

// PmaReader and MergeEngine are opaque in the focal function; we only need fields accessed there.
struct PmaReader {
  int pFd; // Used in the call path: if pFd == 0 then rc may change to SQLITE_DONE
};

struct SortSubtask { int dummy; };

// Forward-declare to satisfy dependencies in VdbeSorter
struct SorterRecord {
  struct { SorterRecord* pNext; } u;
};
struct SorterList {
  SorterRecord *pList;
  int aMemory;
};

struct MergeEngine {
  SortSubtask *pTask;
};

// VdbeSorter layout expected by sqlite3VdbeSorterNext
struct VdbeSorter {
  int bUsePMA;
  PmaReader *pReader;
  MergeEngine *pMerger;
  SortSubtask aTask[1];
  int bUseThreads;
  SorterList list;
};

// VdbeCursor layout used by the focal function
struct VdbeCursor {
  int eCurType;
  struct { VdbeSorter *pSorter; } uc;
};

// Test helpers
static int g_total = 0;
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_TRUE(cond, desc) \
  do { \
    ++g_total; \
    if (cond) { ++g_pass; } \
    else { ++g_fail; std::cerr << "Test fail: " << (desc) \
      << " at " << __FILE__ << ":" << __LINE__ << std::endl; } \
  } while(0)

#define EXPECT_EQ(a, b, desc) EXPECT_TRUE((a) == (b), (desc))

// Helper to run the tests
static void run_tests();

// Test 1: No PMA, multiple items in the sorter list.
// Expectation: We remove the first item (pList becomes next), and rc should be SQLITE_OK.
// After execution, the list head should point to the second element.
static void test_no_pma_multiple_items() {
  // Prepare sorter with two records: rec1 -> rec2 -> nullptr
  SorterRecord rec2;
  SorterRecord rec1;
  rec1.u.pNext = &rec2;
  rec2.u.pNext = nullptr;

  SorterList list;
  list.pList = &rec1;
  list.aMemory = 0; // trigger vdbeSorterRecordFree path on the removed head (static in production)

  VdbeSorter sorter;
  sorter.bUsePMA = 0;
  sorter.pReader = nullptr;
  sorter.pMerger = nullptr;
  sorter.bUseThreads = 0;
  sorter.list = list;
  // aTask[0] is referenced by the production code; initialize with a dummy value
  // (no actual usage beyond address comparison in assertions)
  // sorter.aTask[0] is part of sorter; in test, it's enough to exist
  // (we rely on its address for the production assert, but we don't modify it here)

  VdbeCursor csr;
  csr.eCurType = CURTYPE_SORTER;
  csr.uc.pSorter = &sorter;

  struct sqlite3 db;
  int rc = sqlite3VdbeSorterNext(&db, &csr);

  // Expected: rc == SQLITE_OK, head advanced to rec2
  EXPECT_EQ(rc, 0, "rc should be SQLITE_OK for non-empty list after one pop");
  EXPECT_TRUE(sorter.list.pList == &rec2, "Sorter list head should now be rec2 after removing rec1");
}

// Test 2: No PMA, single item in the sorter list.
// Expectation: We remove the only item, resulting in an empty list and rc == SQLITE_DONE.
static void test_no_pma_single_item() {
  // Prepare sorter with a single record: rec1 -> nullptr
  SorterRecord rec1;
  rec1.u.pNext = nullptr;

  SorterList list;
  list.pList = &rec1;
  list.aMemory = 0;

  VdbeSorter sorter;
  sorter.bUsePMA = 0;
  sorter.pReader = nullptr;
  sorter.pMerger = nullptr;
  sorter.bUseThreads = 0;
  sorter.list = list;

  VdbeCursor csr;
  csr.eCurType = CURTYPE_SORTER;
  csr.uc.pSorter = &sorter;

  struct sqlite3 db;
  int rc = sqlite3VdbeSorterNext(&db, &csr);

  // Expected: rc == SQLITE_DONE, list should now be empty
  EXPECT_EQ(rc, 101, "rc should be SQLITE_DONE when list becomes empty");
  EXPECT_TRUE(sorter.list.pList == nullptr, "Sorter list head should be nullptr after removing the only item");
}

static void run_tests() {
  test_no_pma_multiple_items();
  test_no_pma_single_item();

  std::cout << "Test summary: "
            << g_pass << " passed, "
            << g_fail << " failed, "
            << g_total << " total." << std::endl;
}

// The main function executes the tests.
// We avoid terminating on first failure to maximize coverage as requested.
int main() {
  run_tests();
  return (g_fail != 0) ? 1 : 0;
}
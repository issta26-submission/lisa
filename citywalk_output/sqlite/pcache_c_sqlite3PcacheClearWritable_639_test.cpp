// Unit tests for sqlite3PcacheClearWritable
// This test suite targets the focal method defined in pcache.c:
// void sqlite3PcacheClearWritable(PCache *pCache)

#include <functional>
#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Include sqlite internal declarations to obtain PCache, PgHdr, and flag constants.
// The tests run in a C++11 environment but call a C function from the sqlite3 codebase.
extern "C" {
}

// Forward declaration of the function under test to ensure linkage.
// It is already declared in sqliteInt.h, but this provides a direct symbol for C linkage in C++ tests.
extern "C" void sqlite3PcacheClearWritable(PCache *pCache);

// Simple non-terminating test harness
static int gTotalTests = 0;
static int gFailedTests = 0;

// Basic check macro that records failures but does not terminate execution.
#define CHECK(cond, msg)                                      \
  do {                                                         \
    ++gTotalTests;                                            \
    if (!(cond)) {                                            \
      std::cerr << "[FAIL] " << msg << std::endl;             \
      ++gFailedTests;                                        \
    } else {                                                  \
      /* Optional: uncomment to print successes for debugging */ \
      /* std::cout << "[OK] " << msg << std::endl; */        \
    }                                                         \
  } while (0)

static void test_sqlite3PcacheClearWritable_Normal()
{
  // Comment: Normal scenario with a non-empty dirty list where all pages have both
  // PGHDR_NEED_SYNC and PGHDR_WRITEABLE flags set. After clear, both flags should be cleared
  // and pSynced should point to the tail of the dirty list.

  // Create three PgHdr pages
  PgHdr *p1 = new PgHdr();
  PgHdr *p2 = new PgHdr();
  PgHdr *p3 = new PgHdr();

  // Build the dirty list: p1 -> p2 -> p3
  p1->pDirtyNext = p2;
  p2->pDirtyNext = p3;
  p3->pDirtyNext = NULL;

  // Set initial flags: both NEED_SYNC and WRITEABLE set
  p1->flags = PGHDR_NEED_SYNC | PGHDR_WRITEABLE;
  p2->flags = PGHDR_NEED_SYNC | PGHDR_WRITEABLE;
  p3->flags = PGHDR_NEED_SYNC | PGHDR_WRITEABLE;

  // Create PCache and connect the dirty list
  PCache *pCache = new PCache();
  pCache->pDirty = p1;
  pCache->pDirtyTail = p3;
  pCache->pSynced = NULL;

  // Execute the function under test
  sqlite3PcacheClearWritable(pCache);

  // All flags should be cleared
  bool a1 = (p1->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;
  bool a2 = (p2->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;
  bool a3 = (p3->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;

  CHECK(a1 && a2 && a3, "Normal: All PgHdr flags cleared after CLEAR-WRITEABLE");

  // pSynced should be set to the dirty tail
  CHECK(pCache->pSynced == p3, "Normal: pSynced points to pDirtyTail after CLEAR-WRITEABLE");

  // Cleanup
  delete pCache;
  delete p1;
  delete p2;
  delete p3;
}

static void test_sqlite3PcacheClearWritable_PartialBits()
{
  // Comment: Partial bits scenario where some pages do not have both bits set initially.
  // Ensure the function clears whatever bits are present.

  PgHdr *p1 = new PgHdr();
  PgHdr *p2 = new PgHdr();
  PgHdr *p3 = new PgHdr();

  p1->pDirtyNext = p2;
  p2->pDirtyNext = p3;
  p3->pDirtyNext = NULL;

  // p1 has both bits; p2 has only NEED_SYNC; p3 has none
  p1->flags = PGHDR_NEED_SYNC | PGHDR_WRITEABLE;
  p2->flags = PGHDR_NEED_SYNC; // missing WRITEABLE
  p3->flags = 0; // none

  PCache *pCache = new PCache();
  pCache->pDirty = p1;
  pCache->pDirtyTail = p3;
  pCache->pSynced = NULL;

  sqlite3PcacheClearWritable(pCache);

  // p1 should have both bits cleared
  bool b1 = (p1->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;
  // p2 should have NEED_SYNC cleared, leaving 0
  bool b2 = (p2->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;
  // p3 remains unchanged (still 0)
  bool b3 = (p3->flags & (PGHDR_NEED_SYNC | PGHDR_WRITEABLE)) == 0;

  CHECK(b1, "PartialBits: p1 flags cleared");
  CHECK(b2, "PartialBits: p2 flags cleared (even if already missing WRITEABLE)");
  CHECK(b3, "PartialBits: p3 flags unchanged (still 0)");

  CHECK(pCache->pSynced == p3, "PartialBits: pSynced points to pDirtyTail after CLEAR-WRITEABLE");

  delete pCache;
  delete p1;
  delete p2;
  delete p3;
}

static void test_sqlite3PcacheClearWritable_EmptyList()
{
  // Comment: Edge case where the dirty list is empty. pDirty == NULL and pDirtyTail may be NULL or non-NULL.
  // The function should simply set pSynced to pDirtyTail.

  PCache *pCache = new PCache();
  pCache->pDirty = NULL;
  pCache->pDirtyTail = NULL; // scenario A
  pCache->pSynced = (PgHdr*)0xDEADBEEF; // ensure non-null before

  sqlite3PcacheClearWritable(pCache);

  // pSynced should be set to NULL (the dirty tail)
  CHECK(pCache->pSynced == NULL, "EmptyList: pSynced equals NULL when pDirtyTail is NULL");

  delete pCache;

  // Now test with a non-NULL pDirtyTail while pDirty is NULL
  pCache = new PCache();
  pCache->pDirty = NULL;
  PgHdr dummyTail;
  pCache->pDirtyTail = &dummyTail;
  pCache->pSynced = NULL;

  sqlite3PcacheClearWritable(pCache);

  CHECK(pCache->pSynced == &dummyTail, "EmptyList: pSynced equals pDirtyTail when pDirty is NULL but pDirtyTail non-NULL");

  delete pCache;
}

// Entry point
int main()
{
  // Run tests
  test_sqlite3PcacheClearWritable_Normal();
  test_sqlite3PcacheClearWritable_PartialBits();
  test_sqlite3PcacheClearWritable_EmptyList();

  // Summary
  std::cout << "Total tests run: " << gTotalTests << std::endl;
  std::cout << "Total failures: " << gFailedTests << std::endl;

  return (gFailedTests == 0) ? 0 : 1;
}
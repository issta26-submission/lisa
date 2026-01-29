// Unit tests for lsmFsPageRelease in lsm_file.c
// This test suite is written for C++11 without GTest and uses a tiny
// handcrafted assertion framework to maximize code coverage across
// true/false branches of predicates inside lsmFsPageRelease.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Import necessary dependencies and type definitions from the project.
// These headers provide Page, FileSystem, and related constants/macros
// such as PAGE_FREE, PAGE_HASPREV, LSM_OK, etc.
extern "C" {
}

// If the project provides a header for lsmFsPageRelease, it will be declared here.
// Otherwise, we rely on the extern "C" declaration below to ensure proper linkage.
extern "C" int lsmFsPageRelease(Page *pPg);

// Basic tiny test framework (non-terminating assertions)
static int g_total_failures = 0;

#define T_EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << #cond \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_total_failures; \
  } \
} while(0)

#define T_EXPECT_FALSE(cond) T_EXPECT_TRUE(!(cond))

#define T_EXPECT_EQ(a,b) do { \
  if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " \
              << #b << " (" << (b) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_total_failures; \
  } \
} while(0)

// Helper function to reset a FileSystem and Page to a known initial state
static void initialize_basic_fs_and_page(FileSystem *fs, Page *pg) {
  // Reset file system structure to a clean state
  std::memset(fs, 0, sizeof(FileSystem));
  fs->nOut = 5;               // arbitrary starting value
  fs->pCompress = (void*)0x1;  // non-null to satisfy the assert path
  fs->pFree = 0;
  fs->pMapped = 0;

  // Initialize page structure
  std::memset(pg, 0, sizeof(Page));
  pg->nRef = 1;
  pg->pFS = fs;
  pg->iPg = 42;
  pg->flags = 0;              // no PAGE_HASPREV, no PAGE_FREE
  unsigned char dummy[8] = {0};
  pg->aData = dummy;
  pg->pMappedNext = 0;
  pg->pFreeNext = 0;
}

// Test 1: pPg == NULL should simply return LSM_OK
static void test_lsmFsPageRelease_null_pointer() {
  int rc = lsmFsPageRelease((Page*)0);
  T_EXPECT_EQ(rc, LSM_OK);
  // No further state changes should occur when input is NULL
}

// Test 2: Release with last reference and PAGE_FREE not set
// Expected behavior:
// - rc == LSM_OK
// - nOut decremented
// - PAGE_HASPREV cleared
// - aData unchanged (since PAGE_HASPREV is 0)
// - Page removed from pMapped and added to pFree
static void test_lsmFsPageRelease_lastref_remove_from_mapped() {
  // Allocate and initialize structures
  FileSystem fs;
  Page *pPg = new Page;

  initialize_basic_fs_and_page(&fs, pPg);

  // Set up initial mapped list with only pPg
  fs.pMapped = pPg;
  // Ensure a DATA pointer is accessible for validation
  unsigned char dataBuf[8];
  std::memcpy(dataBuf, pPg->aData, sizeof(dataBuf)); // keep a reference
  pPg->aData = dataBuf;

  // Call the function
  int rc = lsmFsPageRelease(pPg);

  // Validate results
  T_EXPECT_EQ(rc, LSM_OK);
  T_EXPECT_EQ(fs.nOut, 4);                         // decremented by 1
  T_EXPECT_EQ((pPg->flags & PAGE_HASPREV), 0);      // should be cleared
  T_EXPECT_EQ(pPg->aData, dataBuf);                 // unchanged
  T_EXPECT_EQ(pPg->pMappedNext, (Page*)0);          // removed from mapped list
  T_EXPECT_EQ(fs.pMapped, (Page*)0);                // mapped list should be empty
  T_EXPECT_EQ(fs.pFree, pPg);                       // added to free list
  T_EXPECT_EQ(pPg->pFreeNext, (Page*)0);            // no previous free item

  delete pPg;
}

// Test 3: Release with last reference and PAGE_FREE set
// Expected behavior:
// - rc == LSM_OK
// - nOut decremented
// - Page remains in mapped list (since it's not removed when PAGE_FREE is set)
// - The call path goes to fsPageAddToLru (static), which we can't observe directly,
//   but we can observe that pMapped is still pointing to the page.
static void test_lsmFsPageRelease_lastref_with_free_flag() {
  FileSystem fs;
  Page *pPg = new Page;

  // Initialize fs and page
  std::memset(&fs, 0, sizeof(FileSystem));
  fs.nOut = 1;
  fs.pCompress = (void*)0x1;
  fs.pFree = 0;
  fs.pMapped = 0;

  pPg->nRef = 1;
  pPg->pFS = &fs;
  pPg->iPg = 7;
  pPg->flags = PAGE_FREE; // mark as free to trigger else branch
  unsigned char dataBuf[8] = {0};
  pPg->aData = dataBuf;
  pPg->pMappedNext = 0;
  pPg->pFreeNext = 0;

  // Put page in mapped list
  fs.pMapped = pPg;

  int rc = lsmFsPageRelease(pPg);

  // Validate results
  T_EXPECT_EQ(rc, LSM_OK);
  T_EXPECT_EQ(pPg->nRef, 0);     // ref decremented to zero
  T_EXPECT_EQ(fs.nOut, 0);       // nOut decremented
  // Since PAGE_FREE is set, the page should remain in mapped list (not removed)
  T_EXPECT_EQ(fs.pMapped, pPg);
  // We cannot inspect internal LRU state; ensure no crash and state remains coherent
  delete pPg;
}

// Additional test: ensure inner path when nRef does not reach zero is safe
// Setup: nRef starts at 2; after release becomes 1; the inner block should be skipped.
static void test_lsmFsPageRelease_non_zero_after_release() {
  FileSystem fs;
  Page *pPg = new Page;

  std::memset(&fs, 0, sizeof(FileSystem));
  fs.nOut = 2;
  fs.pCompress = (void*)0x1;
  fs.pFree = 0;
  fs.pMapped = 0;

  pPg->nRef = 2;
  pPg->pFS = &fs;
  pPg->iPg = 9;
  pPg->flags = 0; // ensure simple path
  unsigned char dataBuf[8] = {0};
  pPg->aData = dataBuf;
  pPg->pMappedNext = 0;
  pPg->pFreeNext = 0;

  // Map it
  fs.pMapped = pPg;

  int rc = lsmFsPageRelease(pPg);

  // Validate: since nRef > 0 after decrement, inner logic is skipped
  T_EXPECT_EQ(rc, LSM_OK);
  // nOut should remain unchanged
  T_EXPECT_EQ(fs.nOut, 2);
  // pMapped should remain since we didn't remove
  T_EXPECT_EQ(fs.pMapped, pPg);

  delete pPg;
}

// Runner
static void run_all_tests() {
  std::cout << "Running unit tests for lsmFsPageRelease (C API) ..." << std::endl;

  test_lsmFsPageRelease_null_pointer();
  test_lsmFsPageRelease_lastref_remove_from_mapped();
  test_lsmFsPageRelease_lastref_with_free_flag();
  test_lsmFsPageRelease_non_zero_after_release();

  if(g_total_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cout << g_total_failures << " TEST(S) FAILED" << std::endl;
  }
}

int main() {
  run_all_tests();
  return (g_total_failures == 0) ? 0 : 1;
}
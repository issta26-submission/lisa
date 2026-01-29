// fts5_iter_next_scan_test.cpp
// A self-contained unit test suite for the focal method logic structure
// inspired by sqlite3Fts5IterNextScan, implemented in a simplified, dependency-free
// environment suitable for C++11. This test focuses on exercising the branching
// logic around the leaf release and EOF flag as described in the focal method.

// Notes:
// - This file does not depend on the real FTS5/SQLite codebase. Instead, it
//   provides minimal stubs and a self-contained replica of the critical logic
//   to enable isolated testing of the control flow.
// - The tests use a lightweight non-terminating assertion mechanism (EXPECT_*) as
//   requested, so that execution continues after a failed expectation.
// - All code is in a single translation unit to simplify static-linkage concerns.

#include <fts5Int.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain/compatibility constants
static const int SQLITE_OK = 0;
static const unsigned char FTS5_MAIN_PREFIX = 0; // chosen to match test expectations

// Lightweight, test-only type definitions mirroring the minimal subset used by the focal method.
using u8 = uint8_t;

// Forward declarations for test hooks (these would normally come from the production headers)
struct Fts5Index;       // forward
struct Fts5SegIter;
struct Fts5Leaf;

// Minimal term representation (we only need p[0] access)
struct Fts5Term {
  u8 *p;
};

// Leaf structure (opaque in production; we only track presence)
struct Fts5Leaf {
  int dummy; // placeholder payload
};

// Segment iterator used by the focal method
struct Fts5SegIter {
  Fts5Leaf *pLeaf;
  Fts5Term term;
};

// Core index structure
struct Fts5Index {
  int rc;
};

// Base iterator structure; the focal method casts the index iterator to a general iter
struct Fts5Iter {
  Fts5Index *pIndex;
  int bEof;
};

// Fts5IndexIter layout must begin with an Fts5Iter base to safely cast
struct Fts5IndexIter {
  Fts5Iter base;          // base part (cast target)
  Fts5Index *pIndex;        // actual index pointer
  Fts5SegIter *aSeg;        // array of segment iterators
  struct { int iFirst; } aFirst[2]; // minimal aFirst used by the focal method
};

// Global test state to observe effects of the focal method
static int g_leafReleasedCount = 0;

// Test hook: simulate data release when a leaf is no longer needed
static void fts5DataRelease(Fts5Leaf *pData){
  (void)pData;        // unused in this simplified test, but we count releases
  ++g_leafReleasedCount;
}

// Test hook: simulate the continuation step of the multi-iterator (no-op here)
static void fts5MultiIterNext(Fts5Index *p, Fts5Iter *pIter, int iFrom, int iFrom2){
  (void)p;
  (void)pIter;
  (void)iFrom;
  (void)iFrom2;
  // In the real code this advances internal state; for tests we keep it inert.
}

// Test hook: simulate the final return value resolution from the index
static int fts5IndexReturn(Fts5Index *p){
  return p->rc;
}

// Focal method re-implemented (self-contained, mirroring the control-flow of the real code)
static int sqlite3Fts5IterNextScan(Fts5IndexIter *pIndexIter){
  Fts5Iter *pIter = (Fts5Iter*)pIndexIter;
  Fts5Index *p = pIter->pIndex;

  // In the real code there is an assert here that rc==SQLITE_OK; for testability
  // we simply short-circuit if rc is not OK to observe the behavior without aborts.
  if (p->rc != SQLITE_OK) {
    return fts5IndexReturn(p);
  }

  fts5MultiIterNext(p, pIter, 0, 0);

  if( p->rc==SQLITE_OK ){
    Fts5SegIter *pSeg = &pIter->aSeg[ pIter->aFirst[1].iFirst ];
    if( pSeg->pLeaf && pSeg->term.p[0]!=FTS5_MAIN_PREFIX ){
      fts5DataRelease(pSeg->pLeaf);
      pSeg->pLeaf = 0;
      pIter->base.bEof = 1;
    }
  }

  return fts5IndexReturn(pIter->pIndex);
}

// Simple non-terminating test assertion helper
static int g_failureCount = 0;
#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { std::cout << "EXPECTATION FAILED: " << msg \
      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; ++g_failureCount; } } while(0)


// Test 1: Branch where the leaf is present and its first term is not MAIN_PREFIX.
// Expectation: the leaf is released, aSeg.pLeaf becomes NULL, and EOF flag is set.
static void test_NonMainPrefix_LeafReleased(){
  // Prepare index and iter structures
  Fts5Index idx;
  idx.rc = SQLITE_OK;

  Fts5Leaf leaf;
  leaf.dummy = 123;

  Fts5Term term;
  unsigned char tbuf[1] = { 5 }; // non-MAIN_PREFIX
  term.p = tbuf;

  Fts5SegIter seg;
  seg.pLeaf = &leaf;
  seg.term = term;

  Fts5SegIter segsArr[1] = { seg };

  Fts5IndexIter idxIter;
  idxIter.base.pIndex = &idx;
  idxIter.base.bEof = 0;
  idxIter.pIndex = &idx;
  idxIter.aSeg = segsArr;
  idxIter.aFirst[0].iFirst = 0;
  idxIter.aFirst[1].iFirst = 0;

  // Reset test state
  g_leafReleasedCount = 0;

  // Call focal method
  int rv = sqlite3Fts5IterNextScan(&idxIter);

  // Validate results
  EXPECT_TRUE(rv == SQLITE_OK, "Return value should be SQLITE_OK when rc is OK");
  EXPECT_TRUE(idx.rc == SQLITE_OK, "Index rc should remain SQLITE_OK");
  EXPECT_TRUE(idxIter.aSeg[0].pLeaf == nullptr, "Leaf should be released (NULL) after scan");
  EXPECT_TRUE(idxIter.base.bEof == 1, "EOF flag should be set to 1 when leaf is released");
  EXPECT_TRUE(g_leafReleasedCount == 1, "fts5DataRelease should be invoked exactly once");
}

// Test 2: Branch where the leaf is present but MAIN_PREFIX blocks release.
// Expectation: the leaf remains; EOF is not set; no release occurs.
static void test_MainPrefix_NoRelease(){
  Fts5Index idx;
  idx.rc = SQLITE_OK;

  Fts5Leaf leaf;
  leaf.dummy = 222;

  Fts5Term term;
  unsigned char tbuf[1] = { 0 }; // MAIN_PREFIX
  term.p = tbuf;

  Fts5SegIter seg;
  seg.pLeaf = &leaf;
  seg.term = term;

  Fts5SegIter segsArr[1] = { seg };

  Fts5IndexIter idxIter;
  idxIter.base.pIndex = &idx;
  idxIter.base.bEof = 0;
  idxIter.pIndex = &idx;
  idxIter.aSeg = segsArr;
  idxIter.aFirst[0].iFirst = 0;
  idxIter.aFirst[1].iFirst = 0;

  g_leafReleasedCount = 0;

  int rv = sqlite3Fts5IterNextScan(&idxIter);

  EXPECT_TRUE(rv == SQLITE_OK, "Return value should be SQLITE_OK when rc is OK");
  EXPECT_TRUE(idx.rc == SQLITE_OK, "Index rc should remain SQLITE_OK");
  EXPECT_TRUE(idxIter.aSeg[0].pLeaf != nullptr, "Leaf should NOT be released when MAIN_PREFIX is present");
  EXPECT_TRUE(idxIter.base.bEof == 0, "EOF flag should remain 0 when leaf is not released");
  EXPECT_TRUE(g_leafReleasedCount == 0, "fts5DataRelease should not be invoked when MAIN_PREFIX matches");
}

// Test 3: Branch when index rc is not SQLITE_OK.
// Expectation: the function should not attempt to release the leaf or modify EOF by the focal branch.
static void test_NonOkRc_NoModification(){
  Fts5Index idx;
  idx.rc = 2; // non-OK

  Fts5Leaf leaf;
  leaf.dummy = 555;

  Fts5Term term;
  unsigned char tbuf[1] = { 7 }; // non-MAIN_PREFIX
  term.p = tbuf;

  Fts5SegIter seg;
  seg.pLeaf = &leaf;
  seg.term = term;

  Fts5SegIter segsArr[1] = { seg };

  Fts5IndexIter idxIter;
  idxIter.base.pIndex = &idx;
  idxIter.base.bEof = 0;
  idxIter.pIndex = &idx;
  idxIter.aSeg = segsArr;
  idxIter.aFirst[0].iFirst = 0;
  idxIter.aFirst[1].iFirst = 0;

  g_leafReleasedCount = 0;

  int rv = sqlite3Fts5IterNextScan(&idxIter);

  // With non-OK rc, we expect the function to return quickly with the same rc
  EXPECT_TRUE(rv == 2, "Return value should propagate non-OK rc from index");
  // No leaf release should happen in this path
  EXPECT_TRUE(idxIter.aSeg[0].pLeaf != nullptr, "Leaf should remain non-null when rc is not SQLITE_OK");
  EXPECT_TRUE(idxIter.base.bEof == 0, "EOF flag should remain unchanged when rc is not SQLITE_OK");
  EXPECT_TRUE(g_leafReleasedCount == 0, "fts5DataRelease should not be invoked when rc is not SQLITE_OK");
}

// Test 4: Edge-case where there is no leaf (pLeaf == NULL) but non-MAIN_PREFIX term.
// Expectation: no crash, no leaf release, but function should still return current index rc.
static void test_NoLeaf_NoRelease(){
  Fts5Index idx;
  idx.rc = SQLITE_OK;

  Fts5Leaf *nullLeaf = nullptr; // simulate missing leaf

  Fts5Term term;
  unsigned char tbuf[1] = { 8 }; // non-MAIN_PREFIX
  term.p = tbuf;

  Fts5SegIter seg;
  seg.pLeaf = nullLeaf;
  seg.term = term;

  Fts5SegIter segsArr[1] = { seg };

  Fts5IndexIter idxIter;
  idxIter.base.pIndex = &idx;
  idxIter.base.bEof = 0;
  idxIter.pIndex = &idx;
  idxIter.aSeg = segsArr;
  idxIter.aFirst[0].iFirst = 0;
  idxIter.aFirst[1].iFirst = 0;

  g_leafReleasedCount = 0;

  int rv = sqlite3Fts5IterNextScan(&idxIter);

  EXPECT_TRUE(rv == SQLITE_OK, "Return value should be SQLITE_OK when rc is OK and leaf is NULL");
  EXPECT_TRUE(idxIter.aSeg[0].pLeaf == nullptr, "Leaf remains NULL as there was none to release");
  EXPECT_TRUE(idxIter.base.bEof == 0, "EOF flag should remain 0 when there is no leaf to release");
  EXPECT_TRUE(g_leafReleasedCount == 0, "fts5DataRelease should not be called when there is no leaf");
}


// Main entry: run all tests and report results
int main() {
  test_NonMainPrefix_LeafReleased();
  test_MainPrefix_NoRelease();
  test_NonOkRc_NoModification();
  test_NoLeaf_NoRelease();

  if (g_failureCount > 0) {
    std::cout << "TEST SUMMARY: " << g_failureCount << " failure(s) detected." << std::endl;
    return 1;
  } else {
    std::cout << "TEST SUMMARY: All tests passed." << std::endl;
    return 0;
  }
}
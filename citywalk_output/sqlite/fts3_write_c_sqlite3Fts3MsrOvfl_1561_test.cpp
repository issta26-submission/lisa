// Unit test suite for the focal method: sqlite3Fts3MsrOvfl
// This test harness is self-contained (no external testing framework) and
// compiles with C++11. It provides minimal stubs for dependencies and drives
// different paths through sqlite3Fts3MsrOvfl by configuring test data.
// The approach mirrors the provided function's behavior and validates true/false
// branches, as well as error propagation paths.

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Domain knowledge and testing approach notes (embedded for clarity):
// - True/false branches are driven by fts3SegReaderIsPending / fts3SegReaderIsRootOnly.
// - Static members in the real code are represented here by module-level helpers.
// - We avoid terminating assertions in tests; instead, we log failures.
// - We test both successful completion and error propagation paths.
// - Only standard library features are used; no GTest or mocking frameworks.

// ---------------------------------------------------------------------------
// Minimal type definitions to mimic the real code's interfaces

typedef long long sqlite3_int64;

const int SQLITE_OK = 0;
const int SQLITE_ERROR = 1;

// Fts3Table and related structures (minimal subset used by sqlite3Fts3MsrOvfl)
struct Fts3Table {
    int nPgsz;  // page size
    int bFts4;  // flag (assertion in focal method expects true)
};

// Fts3Cursor and its base.pVtab member
struct Fts3Cursor {
    struct {
        void* pVtab;
    } base;
};

// Fts3SegReader with fields used by the focal method
struct Fts3SegReader {
    sqlite3_int64 iStartBlock;
    sqlite3_int64 iLeafEndBlock;
    int pending;   // corresponds to fts3SegReaderIsPending
    int rootOnly;  // corresponds to fts3SegReaderIsRootOnly
};

// Fts3MultiSegReader holding an array of segment readers
struct Fts3MultiSegReader {
    int nSegment;
    Fts3SegReader **apSegment;
};

// ---------------------------------------------------------------------------
// Global scaffolding to drive test scenarios

static std::vector<int> g_blockSizes; // block blob sizes per block id
static int g_errorOnBlock = -1;      // if set to block id, sqlite3Fts3ReadBlock returns error

// Helper predicates matching the focal file's behavior (non-static in our harness)
static int fts3SegReaderIsPending(Fts3SegReader *pReader){
    return pReader ? pReader->pending : 0;
}
static int fts3SegReaderIsRootOnly(Fts3SegReader *pReader){
    return pReader ? pReader->rootOnly : 0;
}

// Mock of sqlite3Fts3ReadBlock used by the focal method
static int sqlite3Fts3ReadBlock(Fts3Table *p, sqlite3_int64 iBlock, int /*dummy*/, int *pnBlob, int * /*pnLoad*/){
    (void)p; // unused in mocks
    if (iBlock < 0) {
        *pnBlob = 0;
        return SQLITE_OK;
    }
    if ((size_t)iBlock < g_blockSizes.size()) {
        if (iBlock == g_errorOnBlock) {
            return SQLITE_ERROR; // simulate a read error on a specific block
        }
        *pnBlob = g_blockSizes[(size_t)iBlock];
        return SQLITE_OK;
    }
    *pnBlob = 0;
    return SQLITE_OK;
}

// ---------------------------------------------------------------------------
// The focal method under test (copied from the provided source, adapted to harness)

int sqlite3Fts3MsrOvfl(
  Fts3Cursor *pCsr, 
  Fts3MultiSegReader *pMsr,
  int *pnOvfl
){
{
  Fts3Table *p = (Fts3Table*)pCsr->base.pVtab;
  int nOvfl = 0;
  int ii;
  int rc = SQLITE_OK;
  int pgsz = p->nPgsz;
  assert( p->bFts4 );
  assert( pgsz>0 );
  for(ii=0; rc==SQLITE_OK && ii<pMsr->nSegment; ii++){
    Fts3SegReader *pReader = pMsr->apSegment[ii];
    if( !fts3SegReaderIsPending(pReader) 
     && !fts3SegReaderIsRootOnly(pReader) 
    ){
      sqlite3_int64 jj;
      for(jj=pReader->iStartBlock; jj<=pReader->iLeafEndBlock; jj++){
        int nBlob;
        rc = sqlite3Fts3ReadBlock(p, jj, 0, &nBlob, 0);
        if( rc!=SQLITE_OK ) break;
        if( (nBlob+35)>pgsz ){
          nOvfl += (nBlob + 34)/pgsz;
        }
      }
    }
  }
  *pnOvfl = nOvfl;
  return rc;
}
}

// ---------------------------------------------------------------------------
// Test harness and test cases

static int g_failures = 0;

// Simple non-terminating assertion helper for tests
#define TEST_EXPECT_EQ(a,b,desc) do { if((a)!=(b)) { \
    fprintf(stderr, "TEST FAIL: %s: expected %d, got %d (desc: %s)\n", __func__, (int)(b), (int)(a), (desc)); \
    g_failures++; } else { \
    printf("TEST PASS: %s: %s\n", __func__, (desc)); } \
} while(0)

#define TEST_EXPECT_RC_EQ(rc, expected, desc) TEST_EXPECT_EQ((rc), (expected), (desc))

// Helpers to construct test readers and the multi-seg reader
static Fts3SegReader* makeReader(sqlite3_int64 start, sqlite3_int64 end, int pending, int rootOnly){
    Fts3SegReader *r = new Fts3SegReader;
    r->iStartBlock = start;
    r->iLeafEndBlock = end;
    r->pending = pending;
    r->rootOnly = rootOnly;
    return r;
}
static Fts3MultiSegReader* makeMsr(int n, Fts3SegReader **arr){
    Fts3MultiSegReader *m = new Fts3MultiSegReader;
    m->nSegment = n;
    m->apSegment = arr;
    return m;
}
static void deleteMsr(Fts3MultiSegReader *m){
    if(!m) return;
    for(int i=0;i<m->nSegment;i++){
        delete m->apSegment[i];
    }
    delete[] m->apSegment;
    delete m;
}

// Test 1: Basic overflow accumulation across a single segment (no skips)
void test_overflow_basic(){
    // Prepare environment: page size 32, FTS4 enabled
    Fts3Table p;
    p.nPgsz = 32;
    p.bFts4 = 1;
    // Cursor referencing table
    Fts3Cursor csr;
    csr.base.pVtab = &p;

    // Segment 0: active (not pending, not rootOnly), blocks 0..2
    Fts3SegReader r0 = {0, 2, 0, 0};
    Fts3SegReader* ar[1] = { &r0 };
    Fts3MultiSegReader *Msr = makeMsr(1, ar);

    // Blob sizes for blocks 0,1,2
    g_blockSizes.clear();
    g_blockSizes.push_back(10); // block 0
    g_blockSizes.push_back(12); // block 1
    g_blockSizes.push_back(20); // block 2
    g_errorOnBlock = -1;

    int ovfl = 0;
    int rc = sqlite3Fts3MsrOvfl(&csr, Msr, &ovfl);

    TEST_EXPECT_RC_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK in basic path");
    // Each block yields overflow because blob + 35 > pgsz
    // block0: (10+34)/32 = 44/32 = 1
    // block1: (12+34)/32 = 46/32 = 1
    // block2: (20+34)/32 = 54/32 = 1
    TEST_EXPECT_EQ(ovfl, 3, "overflow count should be 3 for 3 overflowing blocks");
    deleteMsr(Msr);
}

// Test 2: Segments with pending/rootOnly should be skipped
void test_skip_pending_and_rootonly(){
    Fts3Table p;
    p.nPgsz = 32;
    p.bFts4 = 1;
    Fts3Cursor csr;
    csr.base.pVtab = &p;

    // Segment 0: active, blocks 0..1
    Fts3SegReader r0 = {0, 1, 0, 0};
    // Segment 1: pending and should be skipped
    Fts3SegReader r1 = {0, 1, 1, 0};
    Fts3SegReader* ar[2] = { &r0, &r1 };
    Fts3MultiSegReader *Msr = makeMsr(2, ar);

    g_blockSizes.clear();
    g_blockSizes.push_back(10); // block 0
    g_blockSizes.push_back(20); // block 1
    g_errorOnBlock = -1;

    int ovfl = 0;
    int rc = sqlite3Fts3MsrOvfl(&csr, Msr, &ovfl);

    // Expect only the first segment considered: 1 block (block0) overflow
    TEST_EXPECT_RC_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when only first segment processed");
    TEST_EXPECT_EQ(ovfl, 1, "overflow should reflect only first active segment (block0)");
    deleteMsr(Msr);
}

// Test 3: Error propagation halts processing but returns accumulated overflow so far
void test_error_propagation(){
    Fts3Table p;
    p.nPgsz = 32;
    p.bFts4 = 1;
    Fts3Cursor csr;
    csr.base.pVtab = &p;

    // Segment 0: two blocks; trigger error on block 1
    Fts3SegReader r0 = {0, 2, 0, 0};
    Fts3SegReader* ar[1] = { &r0 };
    Fts3MultiSegReader *Msr = makeMsr(1, ar);

    g_blockSizes.clear();
    g_blockSizes.push_back(10); // block 0 -> overflow 1
    g_blockSizes.push_back(1000); // block 1 -> will cause error
    g_errorOnBlock = 1; // cause error on block 1

    int ovfl = 0;
    int rc = sqlite3Fts3MsrOvfl(&csr, Msr, &ovfl);

    TEST_EXPECT_RC_EQ(rc, SQLITE_ERROR, "rc should be SQLITE_ERROR due to block read error");
    // Overflow from block 0 should have been counted before error
    TEST_EXPECT_EQ(ovfl, 1, "overflow should reflect blocks processed before error (block0)");
    deleteMsr(Msr);
}

// Test 4: Multiple segments with cumulative overflows
void test_multiple_segments_cumulative(){
    Fts3Table p;
    p.nPgsz = 32;
    p.bFts4 = 1;
    Fts3Cursor csr;
    csr.base.pVtab = &p;

    // Segment 0: blocks 0..1
    Fts3SegReader r0 = {0, 1, 0, 0};
    // Segment 1: blocks 2..2
    Fts3SegReader r1 = {2, 2, 0, 0};
    Fts3SegReader* ar[2] = { &r0, &r1 };
    Fts3MultiSegReader *Msr = makeMsr(2, ar);

    g_blockSizes.clear();
    // block 0: 10 -> overflow 1
    g_blockSizes.push_back(10);
    // block 1: 40 -> (40+34)/32 = 74/32 = 2
    g_blockSizes.push_back(40);
    // block 2: 5  -> (5+34)/32 = 39/32 = 1
    g_blockSizes.push_back(5);
    g_errorOnBlock = -1;

    int ovfl = 0;
    int rc = sqlite3Fts3MsrOvfl(&csr, Msr, &ovfl);

    TEST_EXPECT_RC_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for multi-segment overflow");
    // Total 1 + 2 + 1 = 4
    TEST_EXPECT_EQ(ovfl, 4, "overflow should accumulate across two segments (4 total)");
    deleteMsr(Msr);
}

// Test 5: No segments to process
void test_no_segments(){
    Fts3Table p;
    p.nPgsz = 32;
    p.bFts4 = 1;
    Fts3Cursor csr;
    csr.base.pVtab = &p;

    Fts3SegReader *ar[0] = { nullptr };
    Fts3MultiSegReader *Msr = makeMsr(0, ar);

    g_blockSizes.clear(); // not used
    g_errorOnBlock = -1;

    int ovfl = -1;
    int rc = sqlite3Fts3MsrOvfl(&csr, Msr, &ovfl);

    TEST_EXPECT_RC_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when no segments exist");
    TEST_EXPECT_EQ(ovfl, 0, "overflow should be 0 when there are no segments");
    deleteMsr(Msr);
}

// Runner
int main(){
    printf("Starting sqlite3Fts3MsrOvfl unit tests (standalone harness)\n");
    test_overflow_basic();
    test_skip_pending_and_rootonly();
    test_error_propagation();
    test_multiple_segments_cumulative();
    test_no_segments();

    if(g_failures==0){
        printf("ALL TESTS PASSED\n");
        return 0;
    }else{
        printf("TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }
}
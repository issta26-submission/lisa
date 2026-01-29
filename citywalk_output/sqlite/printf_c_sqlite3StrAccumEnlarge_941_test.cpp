// Unit tests for the focal method: sqlite3StrAccumEnlarge
// This test suite is written in C++11 without any external test framework.
// It re-implements the necessary pieces of the StrAccum machinery in a
// self-contained manner so the focal function can be exercised in isolation.
//
// Notes:
// - The tests cover true/false branches of predicates within sqlite3StrAccumEnlarge.
// - We simulate the memory allocator behavior to trigger success and failure paths.
// - All tests use non-terminating expectations (EXPECT_* style) implemented below.
// - Static/internal helpers are provided within this file to avoid linking to
//   the real SQLite sources.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Step 1: Candidate Keywords for the focal method
// - StrAccum: structure holding string accumulation state
// - nChar, nAlloc: current char count and allocation size
// - mxAlloc: maximum allowed allocation
// - zText: current text buffer
// - db: database handle pointer (used for sqlite3DbRealloc path)
// - printfFlags, accError: flags and error state
// - isMalloced(p): predicate to know if current buffer is malloced by StrAccum
// - sqlite3StrAccumSetError: set error code in StrAccum
// - sqlite3_str_reset: reset StrAccum state
// - sqlite3DbRealloc / sqlite3Realloc: reallocation helpers (mocked here)
// - SQLITE_TOOBIG, SQLITE_NOMEM, SQLITE_PRINTF_MALLOCED: essential constants

// Domain Knowledge: We implement a minimal, self-contained environment that mimics
// the necessary behavior for the focal function to operate.

using i64 = long long;

// Core constants used by the focal method
static const unsigned char SQLITE_TOOBIG = 1;
static const unsigned char SQLITE_NOMEM  = 2;
static const unsigned int  SQLITE_PRINTF_MALLOCED = 1;

// Minimal StrAccum structure used by the focal function
struct StrAccum {
  i64 nChar;          // number of characters currently in the buffer
  int nAlloc;         // allocated size of the buffer (bytes)
  i64 mxAlloc;        // maximum allowed allocation
  char *zText;          // text buffer
  void *db;             // database handle (mocked)
  unsigned int printfFlags; // flags (e.g., whether malloced)
  int accError;           // non-zero means error state
};

// Forward declarations of helpers (mocked in test environment)
static int isMalloced(StrAccum *p);
static void sqlite3StrAccumSetError(StrAccum *p, unsigned char eError);
static void sqlite3_str_reset(StrAccum *p);

// Minimal allocator/memory-tracking to drive test paths
static bool gFailNextAlloc = false; // when true, next real allocation fails
static std::unordered_map<void*, size_t> gAllocSizes; // track allocation sizes

static void* sqlite3Realloc(void *p, size_t n){
  if( gFailNextAlloc ) return NULL;
  void *q = realloc(p, n);
  if( q ) gAllocSizes[q] = n;
  return q;
}
static void* sqlite3DbRealloc(void *db, void *p, size_t n){
  (void)db; // db is not used in test harness
  return sqlite3Realloc(p, n);
}
static int sqlite3DbMallocSize(void *db, void *p){
  (void)db;
  if(p==nullptr) return 0;
  auto it = gAllocSizes.find(p);
  if(it != gAllocSizes.end()) return (int)it->second;
  return 0;
}
static int isMalloced(StrAccum *p){
  return (p->printfFlags & SQLITE_PRINTF_MALLOCED) != 0;
}
static void sqlite3StrAccumSetError(StrAccum *p, unsigned char eError){
  p->accError = (int)eError;
}
static void sqlite3_str_reset(StrAccum *p){
  // In real code this would free/release buffers; here we mimic minimal behavior.
  if( p->zText && isMalloced(p) ){
    free(p->zText);
  }
  p->zText = nullptr;
  p->nChar = 0;
  p->nAlloc = 0;
  p->printfFlags = 0;
  p->accError = 0;
  p->db = nullptr;
}

// The focal method under test (copied/adapted for the test harness)
int sqlite3StrAccumEnlarge(StrAccum *p, i64 N){
  char *zNew;
  assert( p->nChar+N >= p->nAlloc ); /* Only called if really needed */
  if( p->accError ){
    // In the real test harness, this would trigger testcases.
    // Here we simply return failure to exercise the early exit path.
    return 0;
  }
  if( p->mxAlloc==0 ){
    sqlite3StrAccumSetError(p, SQLITE_TOOBIG);
    return p->nAlloc - p->nChar - 1;
  }else{
    char *zOld = isMalloced(p) ? p->zText : 0;
    i64 szNew = p->nChar + N + 1;
    if( szNew+p->nChar<=p->mxAlloc ){
      /* Force exponential buffer size growth as long as it does not overflow,
      ** to avoid having to call this routine too often */
      szNew += p->nChar;
    }
    if( szNew > p->mxAlloc ){
      sqlite3_str_reset(p);
      sqlite3StrAccumSetError(p, SQLITE_TOOBIG);
      return 0;
    }else{
      p->nAlloc = (int)szNew;
    }
    if( p->db ){
      zNew = sqlite3DbRealloc(p->db, zOld, p->nAlloc);
    }else{
      zNew = sqlite3Realloc(zOld, p->nAlloc);
    }
    if( zNew ){
      assert( p->zText!=0 || p->nChar==0 );
      if( !isMalloced(p) && p->nChar>0 ) memcpy(zNew, p->zText, p->nChar);
      p->zText = zNew;
      p->nAlloc = sqlite3DbMallocSize(p->db, zNew);
      p->printfFlags |= SQLITE_PRINTF_MALLOCED;
    }else{
      sqlite3_str_reset(p);
      sqlite3StrAccumSetError(p, SQLITE_NOMEM);
      return 0;
    }
  }
  assert( N>=0 && N<=0x7fffffff );
  return (int)N;
}

// Helper: allocate a StrAccum instance for tests
static StrAccum makeP(i64 nChar, int nAlloc, i64 mxAlloc, const char* initialData, bool malloced){
  StrAccum p;
  p.nChar = nChar;
  p.nAlloc = nAlloc;
  p.mxAlloc = mxAlloc;
  p.zText = nullptr;
  p.db = nullptr;
  p.printfFlags = malloced ? SQLITE_PRINTF_MALLOCED : 0;
  p.accError = 0;
  if( initialData && nChar>0 ){
    p.zText = (char*)malloc((size_t)nAlloc);
    if(p.zText){
      memcpy(p.zText, initialData, (size_t)nChar);
      gAllocSizes[p.zText] = nAlloc;
    }
  }else if(nAlloc>0){
    p.zText = (char*)malloc((size_t)nAlloc);
    if(p.zText){
      p.zText[0] = '\0';
      gAllocSizes[p.zText] = nAlloc;
    }
  }
  return p;
}

// Helper: destroy StrAccum instance
static void destroyP(StrAccum &p){
  if(p.zText){
    free(p.zText);
    p.zText = nullptr;
  }
  p.nChar = 0;
  p.nAlloc = 0;
  p.mxAlloc = 0;
  p.db = nullptr;
  p.printfFlags = 0;
  p.accError = 0;
}

// Lightweight test framework (non-terminating)
static int gTestFailures = 0;
static void expectTrue(bool cond, const char* msg){
  if(!cond){
    std::cerr << "Test failure: " << msg << std::endl;
    ++gTestFailures;
  }
}
#define EXPECT_TRUE(cond, msg) expectTrue((cond), (msg))

// Test 1: accError is non-zero -> early return 0
static void test_accError_path(){
  StrAccum p = makeP(2, 4, 100, "ab", false);
  p.accError = (int)SQLITE_TOOBIG;
  int rc = sqlite3StrAccumEnlarge(&p, 3);
  // Should return 0 and not mutate allocations (early exit)
  EXPECT_TRUE(rc == 0, "accError path should return 0");
  EXPECT_TRUE(p.nChar == 2, "accError path should not modify nChar");
  destroyP(p);
}

// Test 2: mxAlloc == 0 -> TOOBIG and partial allocation calc
static void test_mxAlloc_zero_path(){
  StrAccum p = makeP(2, 4, 0, "ab", false);
  p.accError = 0;
  int rc = sqlite3StrAccumEnlarge(&p, 1);
  EXPECT_TRUE(rc == 0, "mxAlloc==0 path should return 0");
  EXPECT_TRUE(p.accError == (int)SQLITE_TOOBIG, "mxAlloc==0 should set TOOBIG error");
  // Difference: nAlloc - nChar - 1 is expected, but we only validate return value (0 here)
  destroyP(p);
}

// Test 3A: normal exponential growth path with malloced buffer expansion
static void test_normal_growth_path(){
  // Initial buffer: 5 chars, not malloced (to test copy)
  const char initData[6] = "ABCDE";
  StrAccum p = makeP(5, 5, 100, initData, false);
  p.db = nullptr;
  p.mxAlloc = 100;
  p.accError = 0;
  // Enlarge by N=3
  int rc = sqlite3StrAccumEnlarge(&p, 3);
  // Expect rc == 3 and buffer grown to at least 14
  EXPECT_TRUE(rc == 3, "normal growth should return N");
  EXPECT_TRUE(p.nAlloc >= 14, "nAlloc should be grown to at least 14");
  EXPECT_TRUE(p.zText != nullptr, "zText should be non-null after enlarge");
  // Old data should be copied to new buffer since !isMalloced
  if(p.zText){
    // First 5 bytes should equal initial data
    EXPECT_TRUE(std::strncmp(p.zText, initData, 5) == 0, "old data should be preserved/c copies");
  }
  // After growth, buffer should be marked as malloced
  EXPECT_TRUE(p.printfFlags & SQLITE_PRINTF_MALLOCED, "malloc flag should be set after enlarge");
  destroyP(p);
}

// Test 3B: overflow path triggers TOOBIG because szNew would exceed mxAlloc
static void test_overflow_path(){
  StrAccum p = makeP(40, 40, 50, "ABCDEFGHIJKLMNOPQRSTABCDEFGHIJKLMNOPQRST", false);
  p.accError = 0;
  int rc = sqlite3StrAccumEnlarge(&p, 20); // 40+20+1=61; 61+40=101 > mxAlloc(50)
  EXPECT_TRUE(rc == 0, "overflow path should return 0");
  EXPECT_TRUE(p.accError == (int)SQLITE_TOOBIG, "overflow should set TOOBIG error");
  destroyP(p);
}

// Test 4: allocation failure during realloc -> NOMEM
static void test_alloc_failure_path(){
  StrAccum p = makeP(4, 4, 100, "ABCD", false);
  p.accError = 0;
  // Force next allocation to fail
  gFailNextAlloc = true;
  int rc = sqlite3StrAccumEnlarge(&p, 2);
  EXPECT_TRUE(rc == 0, "alloc failure should return 0");
  EXPECT_TRUE(p.accError == (int)SQLITE_NOMEM, "alloc failure should set NOMEM error");
  gFailNextAlloc = false;
  destroyP(p);
}

// Test 5: non-malloced old buffer data should be copied into new buffer
static void test_copy_nonMalloced_path(){
  // Create initial data in malloced buffer, but do not mark as malloced (simulate external ownership)
  StrAccum p = makeP(5, 5, 100, "12345", false);
  // Ensure initial data is indeed present
  p.accError = 0;
  // Enlarge by N=4 to force a reallocation
  int rc = sqlite3StrAccumEnlarge(&p, 4);
  EXPECT_TRUE(rc == 4, "copy non-malloced path should return N");
  // Verify the first 5 bytes match original data
  if(p.zText){
    EXPECT_TRUE(std::strncmp(p.zText, "12345", 5) == 0, "data should be copied to new buffer");
  }
  destroyP(p);
}

// Test 6: path with db pointer non-null uses sqlite3DbRealloc
static void test_dbPath_growth_path(){
  // Initial buffer
  StrAccum p = makeP(5, 5, 100, "HELLO", false);
  p.db = (void*)0xDEADBEEF; // mock non-null db
  p.mxAlloc = 200;
  p.accError = 0;
  int rc = sqlite3StrAccumEnlarge(&p, 3);
  EXPECT_TRUE(rc == 3, "db path should behave like realloc and return N");
  EXPECT_TRUE(p.zText != nullptr, "db path should produce non-null zText");
  destroyP(p);
}

// Step 3 (execution): Run all tests
static void runAllTests(){
  test_accError_path();
  test_mxAlloc_zero_path();
  test_normal_growth_path();
  test_overflow_path();
  test_alloc_failure_path();
  test_copy_nonMalloced_path();
  test_dbPath_growth_path();
}

// Main: Entry point for the test executable
int main(){
  runAllTests();
  if(gTestFailures > 0){
    std::cerr << "Summary: " << gTestFailures << " test(s) failed." << std::endl;
    return EXIT_FAILURE;
  }else{
    std::cout << "All tests passed." << std::endl;
    return EXIT_SUCCESS;
  }
}
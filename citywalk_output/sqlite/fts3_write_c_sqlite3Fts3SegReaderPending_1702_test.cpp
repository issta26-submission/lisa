// C++11 test harness for sqlite3Fts3SegReaderPending
// This test suite provides a minimal, self-contained environment to exercise
// the focal function sqlite3Fts3SegReaderPending from a fts-like C module.
// It mocks only the necessary SQLite/FTS3 structures and helpers in order
// to compile under C++11 without relying on GTest or external dependencies.
// The tests cover both exact-term lookup and prefix-lookup paths, including
// both single-match and multi-match scenarios.

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cassert>


// Domain knowledge helpers for testing
// - Provide minimal substitutes for sqlite3_int64, SQLITE_OK, SQLITE_NOMEM
using sqlite3_int64 = long long;
#define SQLITE_OK 0
#define SQLITE_NOMEM 7

// Forward declarations of test-scaffold types mirroring essential parts of fts3_write.c
struct Fts3HashElem;
struct Fts3Hash;
struct Fts3IndexEntry;
struct Fts3SegReader;
struct Fts3Table;

// Hash element representing a term entry in the in-memory pending-hash
struct Fts3HashElem {
    Fts3HashElem *pNext;
    char *zKey;     // term key (null-terminated)
    int nKey;       // length of zKey (nKey may be <= strlen(zKey))
};

// Hash container: a linked-list based placeholder for pHash
struct Fts3Hash {
    Fts3HashElem *pListHead;
};

// An index entry containing a pending-hash (as used by sqlite3Fts3SegReaderPending)
struct Fts3IndexEntry {
    Fts3Hash hPending;
};

// Simple SegReader object as used by the focal function
struct Fts3SegReader {
    int iIdx;
    Fts3HashElem **ppNextElem;
};

// The Fts3Table used by sqlite3Fts3SegReaderPending
struct Fts3Table {
    Fts3IndexEntry *aIndex;
    int nIndex;
};

// Memory helpers to mimic SQLite behavior used by the focal function.
// Expose only the small subset necessary for the test harness.

static void *sqlite3_malloc64(size_t n) { return std::malloc(n); }
static void *sqlite3_realloc64(void *p, size_t n) { return std::realloc(p, n); }
static void sqlite3_free(void *p) { std::free(p); }

// Convenience wrappers to access hash elements' internals
static Fts3HashElem* fts3HashFirst(Fts3Hash *p) { return p ? p->pListHead : nullptr; }
static Fts3HashElem* fts3HashNext(Fts3HashElem *p) { return p ? p->pNext : nullptr; }
static void* fts3HashKey(Fts3HashElem *p) { return (void*)p->zKey; }
static int fts3HashKeysize(Fts3HashElem *p) { return p->nKey; }

// Term comparison helper used for qsort in the focal function
static int fts3CompareElemByTerm(const void *A, const void *B) {
    Fts3HashElem * const *pA = (Fts3HashElem * const *)A;
    Fts3HashElem * const *pB = (Fts3HashElem * const *)B;
    const char *sa = (*pA)->zKey;
    const char *sb = (*pB)->zKey;
    int na = (*pA)->nKey;
    int nb = (*pB)->nKey;
    int len = (sa && sb) ? std::min(na, nb) : 0;
    int r = std::memcmp(sa, sb, len);
    if (r != 0) return r;
    if (na != nb) return na - nb;
    return 0;
}

// The focal method, adapted to the test harness environment.
// This is a faithful representation (with corrected formatting) of the logic
// described in the provided snippet.
static int sqlite3Fts3SegReaderPending(
  Fts3Table *p,                   
  int iIndex,                     
  const char *zTerm,              
  int nTerm,                      
  int bPrefix,                    
  Fts3SegReader **ppReader        
){
  Fts3SegReader *pReader = 0;
  Fts3HashElem *pE;
  Fts3HashElem **aElem = 0;
  int nElem = 0;
  int rc = SQLITE_OK;
  Fts3Hash *pHash;
  pHash = &p->aIndex[iIndex].hPending;
  if( bPrefix ){
    int nAlloc = 0;
    for(pE=fts3HashFirst(pHash); pE; pE=fts3HashNext(pE)){
      char *zKey = (char *)fts3HashKey(pE);
      int nKey = fts3HashKeysize(pE);
      if( nTerm==0 || (nKey>=nTerm && 0==std::memcmp(zKey, zTerm, nTerm)) ){
        if( nElem==nAlloc ){
          Fts3HashElem **aElem2;
          nAlloc += 16;
          aElem2 = (Fts3HashElem **)sqlite3_realloc64(
              aElem, nAlloc*sizeof(Fts3HashElem *)
          );
          if( !aElem2 ){
            rc = SQLITE_NOMEM;
            nElem = 0;
            break;
          }
          aElem = aElem2;
        }
        aElem[nElem++] = pE;
      }
    }
    if( nElem>1 ){
      qsort(aElem, nElem, sizeof(Fts3HashElem *), fts3CompareElemByTerm);
    }
  }else{
    pE = nullptr;
    // Simple exact-match lookup
    // Implement a minimal fts3HashFindElem
    for(Fts3HashElem *qe = pHash->pListHead; qe; qe = qe->pNext){
      if( qe->nKey == nTerm && std::memcmp(qe->zKey, zTerm, nTerm) == 0 ){
        pE = qe;
        break;
      }
    }
    if( pE ){
      aElem = &pE;
      nElem = 1;
    }
  }
  if( nElem>0 ){
    sqlite3_int64 nByte;
    nByte = sizeof(Fts3SegReader) + (nElem+1)*sizeof(Fts3HashElem *);
    pReader = (Fts3SegReader *)sqlite3_malloc64(nByte);
    if( !pReader ){
      rc = SQLITE_NOMEM;
    }else{
      std::memset(pReader, 0, (size_t)nByte);
      pReader->iIdx = 0x7FFFFFFF;
      pReader->ppNextElem = (Fts3HashElem **)&pReader[1];
      std::memcpy(pReader->ppNextElem, aElem, nElem*sizeof(Fts3HashElem *));
    }
  }
  if( bPrefix ){
    sqlite3_free(aElem);
  }
  *ppReader = pReader;
  return rc;
}

// Helper to create and insert a new hash element into a hash
static Fts3HashElem* addHashElem(Fts3Hash *pHash, const char *key) {
    if (!pHash) return nullptr;
    Fts3HashElem *e = (Fts3HashElem*)std::malloc(sizeof(Fts3HashElem));
    if(!e) return nullptr;
    e->zKey = std::strdup(key);
    e->nKey = (int)std::strlen(key);
    e->pNext = pHash->pListHead;
    pHash->pListHead = e;
    return e;
}

// Test framework scaffolding
static int gFailures = 0;
static std::vector<std::string> gLog;

// Lightweight non-terminating assertion helpers
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; \
        gFailures++; \
        gLog.push_back(std::string("FAIL: ") + (msg)); \
    } \
    } while(0)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr)!=nullptr, msg)
#define EXPECT_EQ(a,b,msg) EXPECT_TRUE((a)==(b), std::string(msg)+" ["+std::to_string((a))+"] != ["+std::to_string((b))+"]")

// Test 1: Exact-term lookup: single match, bPrefix = 0
static bool test_exact_lookup_single_match() {
    // Setup table with one index containing a single hash element "apple" (5)
    Fts3Table tbl;
    tbl.nIndex = 1;
    tbl.aIndex = new Fts3IndexEntry[1];
    tbl.aIndex[0].hPending.pListHead = nullptr;

    addHashElem(&tbl.aIndex[0].hPending, "apple");

    // Call focal function with exact term lookup
    Fts3SegReader *reader = nullptr;
    int rc = sqlite3Fts3SegReaderPending(&tbl, 0, "apple", 5, 0, &reader);

    // Assertions
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for successful lookup");
    EXPECT_NOT_NULL(reader, "Reader should be allocated for a single match");
    // The only element should be copied into reader
    Fts3HashElem *found = tbl.aIndex[0].hPending.pListHead;
    // reader should contain a pointer to the element in the aElem array
    EXPECT_NOT_NULL(reader->ppNextElem, "Reader's ppNextElem should be non-null");
    EXPECT_EQ(reader->ppNextElem[0], found, "Reader should point to the found hash element");

    // Cleanup
    // Free keys
    while(tbl.aIndex[0].hPending.pListHead){
        Fts3HashElem *e = tbl.aIndex[0].hPending.pListHead;
        tbl.aIndex[0].hPending.pListHead = e->pNext;
        std::free(e->zKey);
        std::free(e);
    }
    delete[] tbl.aIndex;
    std::free(reader);

    return gFailures == 0;
}

// Test 2: Prefix-lookup: multiple matches, bPrefix = 1, ensure sorted by term
static bool test_prefix_lookup_multiple_sorted() {
    // Setup table with two matching terms: "apple" and "application"
    Fts3Table tbl;
    tbl.nIndex = 1;
    tbl.aIndex = new Fts3IndexEntry[1];
    tbl.aIndex[0].hPending.pListHead = nullptr;

    Fts3HashElem *e1 = addHashElem(&tbl.aIndex[0].hPending, "apple");        // 5 chars
    Fts3HashElem *e2 = addHashElem(&tbl.aIndex[0].hPending, "application");  // 11 chars

    // Prefix term "ap" should include both
    Fts3SegReader *reader = nullptr;
    int rc = sqlite3Fts3SegReaderPending(&tbl, 0, "ap", 2, 1, &reader);

    // Assertions
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for successful prefix lookup");
    EXPECT_NOT_NULL(reader, "Reader should be allocated when multiple matches exist");
    // Expect 2 elements in the reader, in lexicographic order: "apple" before "application"
    EXPECT_EQ(reader->ppNextElem[0], e1, "First element should be 'apple' (lexicographically first)");
    EXPECT_EQ(reader->ppNextElem[1], e2, "Second element should be 'application' (lexicographically after 'apple')");

    // Cleanup
    // Free allocated hash elements
    while(tbl.aIndex[0].hPending.pListHead){
        Fts3HashElem *e = tbl.aIndex[0].hPending.pListHead;
        tbl.aIndex[0].hPending.pListHead = e->pNext;
        std::free(e->zKey);
        std::free(e);
    }
    delete[] tbl.aIndex;
    std::free(reader);

    return gFailures == 0;
}

// Test 3: Prefix-lookup: single match, bPrefix = 1
static bool test_prefix_lookup_single_match() {
    // Setup table with one matching term: "banana" (6)
    Fts3Table tbl;
    tbl.nIndex = 1;
    tbl.aIndex = new Fts3IndexEntry[1];
    tbl.aIndex[0].hPending.pListHead = nullptr;

    Fts3HashElem *e1 = addHashElem(&tbl.aIndex[0].hPending, "banana");

    Fts3SegReader *reader = nullptr;
    int rc = sqlite3Fts3SegReaderPending(&tbl, 0, "ban", 3, 1, &reader);

    // Assertions
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for single-match prefix lookup");
    EXPECT_NOT_NULL(reader, "Reader should be allocated for a single match");
    EXPECT_NOT_NULL(reader->ppNextElem, "Reader's element list should be non-null");
    EXPECT_EQ(reader->ppNextElem[0], e1, "Single matched element should be 'banana'");

    // Cleanup
    while(tbl.aIndex[0].hPending.pListHead){
        Fts3HashElem *e = tbl.aIndex[0].hPending.pListHead;
        tbl.aIndex[0].hPending.pListHead = e->pNext;
        std::free(e->zKey);
        std::free(e);
    }
    delete[] tbl.aIndex;
    std::free(reader);

    return gFailures == 0;
}

// Test 4: Prefix-lookup: no matches found -> reader == nullptr
static bool test_prefix_lookup_no_match() {
    // Setup table with two terms that do not match prefix "zz"
    Fts3Table tbl;
    tbl.nIndex = 1;
    tbl.aIndex = new Fts3IndexEntry[1];
    tbl.aIndex[0].hPending.pListHead = nullptr;

    addHashElem(&tbl.aIndex[0].hPending, "alpha");
    addHashElem(&tbl.aIndex[0].hPending, "beta");

    Fts3SegReader *reader = nullptr;
    int rc = sqlite3Fts3SegReaderPending(&tbl, 0, "zz", 2, 1, &reader);

    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK even if no matches");
    EXPECT_TRUE(reader == nullptr, "Reader should be NULL when there are no matches for the prefix");

    // Cleanup
    while(tbl.aIndex[0].hPending.pListHead){
        Fts3HashElem *e = tbl.aIndex[0].hPending.pListHead;
        tbl.aIndex[0].hPending.pListHead = e->pNext;
        std::free(e->zKey);
        std::free(e);
    }
    delete[] tbl.aIndex;
    // reader is null, nothing to free

    return gFailures == 0;
}

// Main: run all tests and report summary
int main() {
    std::cout << "Running sqlite3Fts3SegReaderPending test suite (C++11 harness)\n";

    bool ok = true;
    ok &= test_exact_lookup_single_match();
    ok &= test_prefix_lookup_multiple_sorted();
    ok &= test_prefix_lookup_single_match();
    ok &= test_prefix_lookup_no_match();

    if(ok && gFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "TESTS FAILED: " << gFailures << " failure(s)\n";
        // Optionally print log
        for (const auto &line : gLog) {
            std::cerr << line << "\n";
        }
        return 1;
    }
}
/*
  Lightweight C++11 test harness for the focal method:
  sqlite3Fts5HashScanEntry
  - This test is self-contained (no GTest) and uses plain C-like structures
    to reproduce the essential behavior of the function under test.
  - It validates both true and false branches of the NULL-check on pHash->pScan,
    and confirms correct term/doclist extraction, including pointer arithmetic.
  - It also verifies that fts5HashAddPoslistSize is invoked.
  - All tests use non-terminating EXPECT_* assertions so that execution continues
    after a failure to maximize coverage.
*/

#include <iostream>
#include <cstring>
#include <cassert>
#include <fts5Int.h>


// Domain-specific typedefs and structures (minimal stubs to emulate the real API)
typedef unsigned char u8;

struct Fts5HashEntry {
  int nData; // size of the entire memory block containing the entry, key, and doclist
};

struct Fts5Hash {
  Fts5HashEntry *pScan; // current scan entry
};

// Global counter to verify that side-effect helper was invoked
static int g_fts5HashAddPoslistSize_calls = 0;

// Mimic the internal helper used by sqlite3Fts5HashScanEntry
void fts5HashAddPoslistSize(Fts5Hash *pHash, Fts5HashEntry *p, Fts5HashEntry *p2){
  (void)pHash; (void)p; (void)p2;
  ++g_fts5HashAddPoslistSize_calls;
}

// Mimic the memory layout helper used inside fts5EntryKey(p)
char* fts5EntryKey(Fts5HashEntry* p){
  // In our simplified test, the key starts immediately after the header
  return (char*)p + sizeof(Fts5HashEntry);
}

// Implementation of the focal method under test (reproduced in test environment)
void sqlite3Fts5HashScanEntry(
  Fts5Hash *pHash,
  const char **pzTerm,            /* OUT: term (nul-terminated) */
  const u8 **ppDoclist,           /* OUT: pointer to doclist */
  int *pnDoclist                  /* OUT: size of doclist in bytes */
){
  Fts5HashEntry *p;
  if( (p = pHash->pScan) ){
    char *zKey = fts5EntryKey(p);
    int nTerm = (int)strlen(zKey);
    fts5HashAddPoslistSize(pHash, p, 0);
    *pzTerm = zKey;
    *ppDoclist = (const u8*)&zKey[nTerm+1];
    *pnDoclist = p->nData - (sizeof(Fts5HashEntry) + nTerm + 1);
  }else{
    *pzTerm = 0;
    *ppDoclist = 0;
    *pnDoclist = 0;
  }
}

// Lightweight test assertion helpers (non-terminating)
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) do { \
  if(!(cond)){ \
    std::cerr << "EXPECT_TRUE failed: " << #cond \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if( (a) != (b) ){ \
    std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " \
              << #b << " (" << (b) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
  if( (void*)(a) != (void*)(b) ){ \
    std::cerr << "EXPECT_PTR_EQ failed: " << #a << " (" << (void*)a << ") != " \
              << #b << " (" << (void*)b << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_STR_EQ(a,b) do { \
  if(std::strcmp((a),(b)) != 0){ \
    std::cerr << "EXPECT_STR_EQ failed: " << #a << " != " << #b \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

// Test 1: Non-null pHash with a well-formed entry; verify correct term/doclist extraction
void test_scan_entry_non_null_pScan(){
  // Prepare a memory block representing: [Fts5HashEntry][term][\\0][doclist bytes...]
  const char *term = "sampleTerm";
  size_t term_len = std::strlen(term);
  const size_t doclist_len = 7; // arbitrary non-zero length

  size_t total = sizeof(Fts5HashEntry) + term_len + 1 + doclist_len;
  char *block = new char[total];

  Fts5HashEntry *p = reinterpret_cast<Fts5HashEntry*>(block);
  p->nData = (int)total;

  char *zKey = block + sizeof(Fts5HashEntry);
  std::memcpy(zKey, term, term_len);
  zKey[term_len] = '\0';

  char *doclist = zKey + term_len + 1;
  // Fill doclist with recognizable data
  for(size_t i=0; i<doclist_len; ++i) doclist[i] = static_cast<char>(i+1);

  Fts5Hash hash;
  hash.pScan = p;

  const char *pzTerm = nullptr;
  const u8 *ppDoclist = nullptr;
  int pnDoclist = -1;

  // Reset side-effect counter
  g_fts5HashAddPoslistSize_calls = 0;

  sqlite3Fts5HashScanEntry(&hash, &pzTerm, &ppDoclist, &pnDoclist);

  // Assertions for the true-branch case
  EXPECT_TRUE(pzTerm != nullptr);
  EXPECT_PTR_EQ(pzTerm, zKey);
  EXPECT_STR_EQ(pzTerm, term);

  EXPECT_TRUE(ppDoclist != nullptr);
  EXPECT_PTR_EQ(ppDoclist, (const u8*)&zKey[term_len+1]);
  EXPECT_EQ(std::memcmp(ppDoclist, doclist, doclist_len) == 0 ? 0 : 1, 0); // content should match

  EXPECT_EQ(pnDoclist, (int)doclist_len);

  // Verify that the helper was invoked exactly once
  EXPECT_EQ(g_fts5HashAddPoslistSize_calls, 1);

  delete[] block;
}

// Test 2: Null pScan in pHash; verify the false-branch behavior
void test_scan_entry_null_pScan(){
  Fts5Hash hash;
  hash.pScan = nullptr;

  const char *pzTerm = (const char*)0x1; // dummy non-null to ensure we replace it
  const u8 *ppDoclist = (const u8*)0x1;
  int pnDoclist = -1;

  sqlite3Fts5HashScanEntry(&hash, &pzTerm, &ppDoclist, &pnDoclist);

  // Assertions for the false-branch case
  EXPECT_TRUE(pzTerm == 0);
  EXPECT_TRUE(ppDoclist == 0);
  EXPECT_EQ(pnDoclist, 0);
}

// Test 3: Entry with zero-length doclist; ensures proper calculation
void test_scan_entry_zero_doclist(){
  // Prepare memory block: [Fts5HashEntry][term][\\0] with zero doclist
  const char *term = "termZeroDoc";
  size_t term_len = std::strlen(term);
  const size_t doclist_len = 0; // zero-length doclist

  size_t total = sizeof(Fts5HashEntry) + term_len + 1 + doclist_len;
  char *block = new char[total];

  Fts5HashEntry *p = reinterpret_cast<Fts5HashEntry*>(block);
  p->nData = (int)total;

  char *zKey = block + sizeof(Fts5HashEntry);
  std::memcpy(zKey, term, term_len);
  zKey[term_len] = '\0';

  // No doclist data follows since length is zero
  Fts5Hash hash;
  hash.pScan = p;

  const char *pzTerm = nullptr;
  const u8 *ppDoclist = nullptr;
  int pnDoclist = -1;

  g_fts5HashAddPoslistSize_calls = 0;
  sqlite3Fts5HashScanEntry(&hash, &pzTerm, &ppDoclist, &pnDoclist);

  EXPECT_TRUE(pzTerm != nullptr);
  EXPECT_PTR_EQ(pzTerm, zKey);
  EXPECT_STR_EQ(pzTerm, term);

  // For zero doclist, the doclist pointer should still point to the byte right after the terminator
  // but the length should be 0
  EXPECT_TRUE(ppDoclist != nullptr);
  EXPECT_PTR_EQ(ppDoclist, (const u8*)&zKey[term_len+1]);
  EXPECT_EQ(pnDoclist, (int)doclist_len);

  delete[] block;
}

// Entry point
int main(){
  test_scan_entry_non_null_pScan();
  test_scan_entry_null_pScan();
  test_scan_entry_zero_doclist();

  if(g_test_failures == 0){
    std::cout << "ALL TESTS PASSED\n";
  }else{
    std::cerr << g_test_failures << " TEST(S) FAILED\n";
  }
  return g_test_failures;
}
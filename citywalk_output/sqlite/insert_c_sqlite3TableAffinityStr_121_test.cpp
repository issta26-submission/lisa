// SQLite Table Affinity String Unit Tests (C++11, no GoogleTest)
// Purpose: Test the focal function sqlite3TableAffinityStr(sqlite3 *db, const Table *pTab)
// Notes:
// - The test re-implements minimal necessary types and symbols to compile and run independently.
// - We provide a lightweight test harness with non-terminating assertions.
// - Tests cover: allocation success, allocation failure, all-virtual columns, single non-virtual,
//   multiple non-virtual with varying affinities, and mixed affinities to exercise the cutoff logic.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// ----------------- Minimal Dependencies and Focal Function Re-implementation -----------------

// Mocked constants matching the focal code expectations
#define COLFLAG_VIRTUAL 0x01
#define SQLITE_AFF_TEXT 1
#define SQLITE_AFF_NUMERIC 2
#define SQLITE_AFF_INTEGER 3
#define SQLITE_AFF_REAL 4
#define SQLITE_AFF_BLOB 5

// Forward declarations (mocked sqlite3 type)
struct sqlite3 { int dummy; };

// Minimal table/column structures to support sqlite3TableAffinityStr
struct TableCol {
  int colFlags;
  unsigned char affinity;
};

struct Table {
  int nCol;
  TableCol *aCol;
};

// Global flag to simulate allocation failure in tests
static bool g_failMalloc = false;

// Mocked sqlite3DbMallocRaw used by sqlite3TableAffinityStr
extern "C" char* sqlite3DbMallocRaw(sqlite3 *db, int n) {
  (void)db; // unused in tests
  if (g_failMalloc) return nullptr;
  if (n <= 0) return nullptr;
  return (char*)std::malloc(n);
}

// The focal function under test (re-implemented here for isolated testing)
extern "C" char *sqlite3TableAffinityStr(sqlite3 *db, const Table *pTab){
  char *zColAff;
  zColAff = (char *)sqlite3DbMallocRaw(db, pTab->nCol+1);
  if( zColAff ){
    int i, j;
    for(i=j=0; i<pTab->nCol; i++){
      if( (pTab->aCol[i].colFlags & COLFLAG_VIRTUAL)==0 ){
        zColAff[j++] = pTab->aCol[i].affinity;
      }
    }
    do{
      zColAff[j--] = 0;
    }while( j>=0 && zColAff[j]<=SQLITE_AFF_BLOB );
  }
  return zColAff; 
}

// ----------------- Test Harness Definitions -----------------

static int g_totalTests = 0;
static int g_failedTests = 0;

// Lightweight assertion helper (non-terminating)
#define TEST_ASSERT(cond, msg) do { \
  ++g_totalTests; \
  if(!(cond)) { \
    ++g_failedTests; \
    std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")" << std::endl; \
  } \
} while(0)

// Helper to create a Table with given columns
static Table createTable(int nCol, const int* flags, const unsigned char* affinities) {
  Table t;
  t.nCol = nCol;
  t.aCol = (TableCol*)std::malloc(nCol * sizeof(TableCol));
  for(int i=0; i<nCol; ++i){
    t.aCol[i].colFlags = flags ? flags[i] : 0;
    t.aCol[i].affinity = affinities ? affinities[i] : 0;
  }
  return t;
}

// Helper to free a Table's memory
static void destroyTable(Table &t) {
  if (t.aCol) {
    std::free(t.aCol);
    t.aCol = nullptr;
  }
  t.nCol = 0;
}

// Helper to interpret resulting affinity string as a memory buffer
static void expectBytes(const char* buf, size_t len, const unsigned char* expected, size_t expectedLen) {
  TEST_ASSERT(len == expectedLen, "Output length mismatch");
  if(len != expectedLen) return;
  TEST_ASSERT(std::memcmp((const void*)buf, (const void*)expected, expectedLen) == 0, "Buffer bytes mismatch");
}

// ----------------- Individual Tests -----------------

// Test A: All columns are virtual; expect first byte to be 0 and allocation succeeds
static bool test_all_virtual_columns() {
  // Setup: 3 columns, all virtual
  const int nCol = 3;
  int flags[nCol] = { COLFLAG_VIRTUAL, COLFLAG_VIRTUAL, COLFLAG_VIRTUAL };
  unsigned char affs[nCol] = {  SQLITE_AFF_TEXT, SQLITE_AFF_BLOB, SQLITE_AFF_NUMERIC }; // values don't matter since all virtual
  Table tab = createTable(nCol, flags, affs);

  sqlite3 db; // dummy db
  char* res = sqlite3TableAffinityStr(&db, &tab);
  TEST_ASSERT(res != nullptr, "Expected non-null result when allocation succeeds (Test A)");
  if(res){
    TEST_ASSERT(res[0] == 0, "Test A: first byte should be 0 since no non-virtual columns");
    // Free allocated memory
    std::free(res);
  }

  destroyTable(tab);
  return true;
}

// Test B: Single non-virtual column with high affinity (e.g., 10) to exercise non-trailing-zero branch
static bool test_single_non_virtual_high_affinity() {
  // Setup: 1 non-virtual column with affinity 10
  const int nCol = 1;
  int flags[nCol] = { 0 }; // non-virtual
  unsigned char affs[nCol] = { 10 }; // greater than SQLITE_AFF_BLOB (5)
  Table tab = createTable(nCol, flags, affs);

  sqlite3 db;
  char* res = sqlite3TableAffinityStr(&db, &tab);
  TEST_ASSERT(res != nullptr, "Expected non-null result when allocation succeeds (Test B)");
  if(res){
    TEST_ASSERT(res[0] == 10, "Test B: first byte should be 10 (non-BLOB affinity)");
    TEST_ASSERT(res[1] == 0, "Test B: second byte should be 0 terminator");
    std::free(res);
  }

  destroyTable(tab);
  return true;
}

// Test C: Two non-virtual columns with affinities 1 and 2; expect both to be trimmed by do-while loop to zeros
static bool test_two_non_virtual_affinities_small_values() {
  const int nCol = 2;
  int flags[nCol] = {0, 0}; // both non-virtual
  unsigned char affs[nCol] = { 1, 2 }; // <= SQLITE_AFF_BLOB
  Table tab = createTable(nCol, flags, affs);

  sqlite3 db;
  char* res = sqlite3TableAffinityStr(&db, &tab);
  TEST_ASSERT(res != nullptr, "Expected non-null result (Test C)");
  if(res){
    // All should be zero due to trimming logic
    unsigned char expected[3] = { 0, 0, 0 };
    expectBytes(res, 3, expected, 3);
    std::free(res);
  }

  destroyTable(tab);
  return true;
}

// Test D: Mixed affinities where a higher-than-BLOB affinity stops trimming
static bool test_mixed_affinities_stop_trimming() {
  // Setup: two non-virtual columns affinities 10 and 1
  const int nCol = 2;
  int flags[nCol] = {0, 0};
  unsigned char affs[nCol] = { 10, 1 };
  Table tab = createTable(nCol, flags, affs);

  sqlite3 db;
  char* res = sqlite3TableAffinityStr(&db, &tab);
  TEST_ASSERT(res != nullptr, "Expected non-null result (Test D)");
  if(res){
    // Expected: [10, 0, 0]
    unsigned char expected[3] = { 10, 0, 0 };
    expectBytes(res, 3, expected, 3);
    std::free(res);
  }

  destroyTable(tab);
  return true;
}

// Test E: Allocation failure should yield NULL
static bool test_allocation_failure() {
  g_failMalloc = true;
  const int nCol = 2;
  int flags[nCol] = {0, 0};
  unsigned char affs[nCol] = { 1, 2 };
  Table tab = createTable(nCol, flags, affs);

  sqlite3 db;
  char* res = sqlite3TableAffinityStr(&db, &tab);
  TEST_ASSERT(res == nullptr, "Test E: expected NULL on allocation failure");
  if(res) std::free(res);

  destroyTable(tab);
  g_failMalloc = false;
  return true;
}

// ----------------- Main Test Runner -----------------

int main() {
  std::cout << "Running sqlite3TableAffinityStr unit tests (C++11, no GTest)..." << std::endl;

  test_all_virtual_columns();
  test_single_non_virtual_high_affinity();
  test_two_non_virtual_affinities_small_values();
  test_mixed_affinities_stop_trimming();
  test_allocation_failure();

  std::cout << "Tests completed. Total: " << g_totalTests << ", Failures: " << g_failedTests << std::endl;

  if (g_failedTests > 0) {
    std::cerr << "Some tests FAILED." << std::endl;
    return 1;
  }
  std::cout << "All tests PASSED." << std::endl;
  return 0;
}
// C++11 test suite for sqlite3SrcListEnlarge (no GTest; simple test harness)
// This test suite mocks minimal SQLite structures to exercise the focal function
// in a self-contained manner. It validates allocation/reallocation paths and
// in-place modifications of SrcList as performed by sqlite3SrcListEnlarge.
//
// Note: This test targets the logic within sqlite3SrcListEnlarge as provided
// and uses lightweight stubs for dependencies (Parse, sqlite3, etc.).

#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Begin C-linkage for the focal function and its dependent types
extern "C" {

// Forward declarations of minimal types expected by sqlite3SrcListEnlarge
typedef struct SrcList_item {
  int iCursor;
} SrcList_item;

typedef struct SrcList {
  int nSrc;
  int nAlloc;
  SrcList_item a[1]; // flexible array member (simulated with size-1)
} SrcList;

typedef struct sqlite3 {
  int mallocFailed;
} sqlite3;

typedef struct Parse {
  sqlite3 *db;
} Parse;

// Prototype of the focal function under test
SrcList *sqlite3SrcListEnlarge(
  Parse *pParse,
  SrcList *pSrc,
  int nExtra,
  int iStart
);

// Memory/realloc helper used by the focal function
void *sqlite3DbRealloc(sqlite3 *db, void *pOld, size_t newSize);

// Error reporting used by the focal function on boundary overflow
void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...);

} // extern "C"

// Domain-specific macros used by the focal code
#ifndef SQLITE_MAX_SRCLIST
#define SQLITE_MAX_SRCLIST 1000
#endif

// End of C-linkage for dependencies

// Global state to capture error messages from sqlite3ErrorMsg for tests
static bool g_errorMsgCalled = false;
static char g_lastErrorMsg[1024] = {0};

// Stub: emulate sqlite3DbRealloc using standard realloc (works for non-null pOld)
void *sqlite3DbRealloc(sqlite3 *db, void *pOld, size_t newSize) {
  (void)db; // not used in this minimal test harness
  return realloc(pOld, newSize);
}

// Stub: simple error reporter that formats and stores the last message
void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
  (void)pParse; // not used by tests
  va_list ap;
  va_start(ap, zFormat);
  vsnprintf(g_lastErrorMsg, sizeof(g_lastErrorMsg), zFormat, ap);
  va_end(ap);
  g_errorMsgCalled = true;
}

// Helpers to create and destroy a SrcList with a preallocated a[] buffer
static SrcList* createSrcList(int nSrc, int nAlloc) {
  // Allocate memory for SrcList plus (nAlloc-1) additional items
  SrcList *p = (SrcList*) malloc(sizeof(SrcList) + (nAlloc - 1) * sizeof(SrcList_item));
  if (!p) return nullptr;
  p->nSrc = nSrc;
  p->nAlloc = nAlloc;
  // Initialize iCursor for existing slots, if any
  for (int i = 0; i < nSrc; ++i) {
    p->a[i].iCursor = i + 1; // arbitrary non-negative sentinel
  }
  // Do not initialize newly allocated slots here; sqlite3SrcListEnlarge will do that
  return p;
}

static void freeSrcList(SrcList *p) {
  free(p);
}

// Simple test harness utilities
static int g_testCount = 0;
static int g_passCount = 0;

#define TEST_BEGIN() bool test_passed = true;
#define TEST_ASSERT(cond) do { if(!(cond)) { test_passed = false; } } while(0)
#define TEST_END(test_name) do { ++g_testCount; if(test_passed) { ++g_passCount; std::cout << "[PASS] " #test_name "\n"; } else { std::cout << "[FAIL] " #test_name "\n"; } } while(0)

// Focal tests

// Test 1: Enlarge empty SrcList by 2 slots at start index 0
static bool test_sqlite3SrcListEnlarge_EnlargeFromEmpty() {
  TEST_BEGIN();

  Parse pParse;
  sqlite3 db;
  db.mallocFailed = 0;
  pParse.db = &db;

  SrcList *pSrc = createSrcList(0, 2); // nSrc=0, nAlloc=2; a[] has 2 slots
  if (!pSrc) { return false; }

  SrcList *ret = sqlite3SrcListEnlarge(&pParse, pSrc, 2, 0);
  TEST_ASSERT(ret != nullptr);
  TEST_ASSERT(ret == pSrc); // in-place enlargement

  // After enlargement, nSrc should be 2
  TEST_ASSERT(pSrc->nSrc == 2);
  // The newly allocated slots should be zeroed and iCursor = -1
  TEST_ASSERT(pSrc->a[0].iCursor == -1);
  TEST_ASSERT(pSrc->a[1].iCursor == -1;

  // Cleanup
  freeSrcList(pSrc);
  TEST_END(test_sqlite3SrcListEnlarge_EnlargeFromEmpty);
  return test_passed;
}

// Test 2: Enlarge existing SrcList by inserting in the middle
// initial: nSrc=2 with a[0].iCursor=1, a[1].iCursor=2
// nExtra=1, iStart=1 -> expect a[1] moved to a[2], new a[1].iCursor=-1
static bool test_sqlite3SrcListEnlarge_MoveExistingSlot() {
  TEST_BEGIN();

  Parse pParse;
  sqlite3 db;
  db.mallocFailed = 0;
  pParse.db = &db;

  SrcList *pSrc = createSrcList(2, 4); // allocate space for 4 entries
  // Fill existing entries
  pSrc->a[0].iCursor = 101;
  pSrc->a[1].iCursor = 202;

  SrcList *ret = sqlite3SrcListEnlarge(&pParse, pSrc, 1, 1);
  TEST_ASSERT(ret != nullptr);
  TEST_ASSERT(ret == pSrc);

  // After enlargement:
  // - nSrc should be 3
  // - a[2] should contain the old a[1] (iCursor=202)
  // - a[1].iCursor should be -1
  TEST_ASSERT(pSrc->nSrc == 3);
  TEST_ASSERT(pSrc->a[2].iCursor == 202);
  TEST_ASSERT(pSrc->a[1].iCursor == -1);

  // The original a[0] should remain intact
  TEST_ASSERT(pSrc->a[0].iCursor == 101);

  freeSrcList(pSrc);
  TEST_END(test_sqlite3SrcListEnlarge_MoveExistingSlot);
  return test_passed;
}

// Test 3: Append at the end (iStart == nSrc), ensure proper reallocation and iCursor setup
static bool test_sqlite3SrcListEnlarge_AppendEnd() {
  TEST_BEGIN();

  Parse pParse;
  sqlite3 db;
  db.mallocFailed = 0;
  pParse.db = &db;

  SrcList *pSrc = createSrcList(1, 1); // nSrc=1, nAlloc=1 (will trigger realloc)
  pSrc->a[0].iCursor = 7;

  SrcList *ret = sqlite3SrcListEnlarge(&pParse, pSrc, 1, 1);
  TEST_ASSERT(ret != nullptr);
  TEST_ASSERT(ret == pSrc);

  // After enlargement: nSrc=2, a[1] should be iCursor = -1
  TEST_ASSERT(pSrc->nSrc == 2);
  TEST_ASSERT(pSrc->a[0].iCursor == 7);
  TEST_ASSERT(pSrc->a[1].iCursor == -1);

  freeSrcList(pSrc);
  TEST_END(test_sqlite3SrcListEnlarge_AppendEnd);
  return test_passed;
}

// Test 4: Error path when exceeding SQLITE_MAX_SRCLIST terms
// Set up nSrc = SQLITE_MAX_SRCLIST - 1 and nExtra = 1 to trigger error
static bool test_sqlite3SrcListEnlarge_MaxSrcListExceeded() {
  TEST_BEGIN();

  g_errorMsgCalled = false;
  g_lastErrorMsg[0] = '\0';

  Parse pParse;
  sqlite3 db;
  db.mallocFailed = 0;
  pParse.db = &db;

  SrcList *pSrc = createSrcList(SQLITE_MAX_SRCLIST - 1, SQLITE_MAX_SRCLIST - 1);
  // Fill not strictly required; the error happens before realloc
  SrcList *ret = sqlite3SrcListEnlarge(&pParse, pSrc, 1, SQLITE_MAX_SRCLIST - 1);
  TEST_ASSERT(ret == nullptr);
  TEST_ASSERT(g_errorMsgCalled);
  TEST_ASSERT(strstr(g_lastErrorMsg, "too many FROM clause terms") != nullptr);

  freeSrcList(pSrc);
  TEST_END(test_sqlite3SrcListEnlarge_MaxSrcListExceeded);
  return test_passed;
}

// Main function: run all tests
int main() {
  std::cout << "Running sqlite3SrcListEnlarge tests (C++11 harness)\n";

  // Run tests
  test_sqlite3SrcListEnlarge_EnlargeFromEmpty();
  test_sqlite3SrcListEnlarge_MoveExistingSlot();
  test_sqlite3SrcListEnlarge_AppendEnd();
  test_sqlite3SrcListEnlarge_MaxSrcListExceeded();

  std::cout << "Tests passed: " << g_passCount << " / " << g_testCount << "\n";

  return (g_testCount == g_passCount) ? 0 : 1;
}
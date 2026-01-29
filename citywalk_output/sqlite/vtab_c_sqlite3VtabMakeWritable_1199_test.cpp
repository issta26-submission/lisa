// C++11 Test Suite for sqlite3VtabMakeWritable
// This test suite provides a minimal, self-contained testing environment
// that exercises the focal function's behavior by stubbing the necessary
// sqlite internals. It does not depend on Google Test; instead, it uses a
// lightweight, non-terminating assertion mechanism and a small test harness.

// Notes:
// - We mock the relevant sqlite types (Parse, Table, sqlite3) and a minimal
//   runtime environment to drive sqlite3VtabMakeWritable.
// - We supply C-compatible stubs for the C function(s) the focal method relies on
//   (sqlite3ParseToplevel, sqlite3Realloc, sqlite3OomFault, and IsVirtual).
// - The tests verify: normal add, no-duplication behavior, and OOM path.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqliteInt.h>


// Lightweight, non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;
#define EXPECT(cond, msg) do { \
  if(!(cond)) { \
    fprintf(stderr, "Test failed: %s\n", msg); \
    g_tests_failed++; \
  } \
  g_tests_run++; \
} while(0)

extern "C" {

// Forward declare the focal function under test (to be linked with vtab.c)
void sqlite3VtabMakeWritable(struct Parse *pParse, struct Table *pTab);

// Minimal sqlite-like types to satisfy vtab.c references
typedef struct sqlite3 {
  int dummy;
} sqlite3;

typedef struct Table {
  // Inline flag to indicate virtual table status for IsVirtual()
  int isVirtual;
} Table;

typedef struct Parse {
  int nVtabLock;
  Table **apVtabLock;
  sqlite3 *db;
} Parse;

// Global stub state for tests
static Parse g_topParse;
static int g_oomFlag = 0;           // Forces sqlite3Realloc to fail when non-zero
static int g_oomCalledFlag = 0;     // Set when sqlite3OomFault is invoked
static int g_forceReset = 0;

// Stub: sqlite3ParseToplevel - always return a pointer to our top-level Parse
Parse* sqlite3ParseToplevel(Parse* pParse) {
  (void)pParse;
  return &g_topParse;
}

// Stub: sqlite3Realloc - simple wrapper around realloc; supports forced failure
void* sqlite3Realloc(void* pOld, size_t n) {
  (void)pOld;
  if (g_oomFlag) {
    // Simulate OOM
    return NULL;
  }
  // Reallocate normally
  return realloc(pOld, n);
}

// Stub: sqlite3OomFault - record that OOM fault occurred
void sqlite3OomFault(sqlite3* db) {
  (void)db;
  g_oomCalledFlag = 1;
}

// Stub: IsVirtual - determine if a Table is virtual
// In sqlite, IsVirtual is typically a macro; we provide a function to satisfy linkage.
// The macro in the actual code would forward to this function.
int IsVirtual(Table *pTab) {
  return pTab != NULL && pTab->isVirtual;
}

// Accessor for tests to verify OOM flag (non-terminating)
int sqlite3Test_getOOMFlag() {
  return g_oomCalledFlag;
}

// Reset helper for tests
void sqlite3Test_resetEnvironment() {
  g_oomFlag = 0;
  g_oomCalledFlag = 0;
  g_forceReset = 0;
  // Reset toplevel parse state
  g_topParse.nVtabLock = 0;
  g_topParse.apVtabLock = NULL;
  g_topParse.db = NULL;
}

} // extern "C"

//
// Test helpers and scenarios
//

static void test_makeWritable_adds_new_tab() {
  // Reset environment
  sqlite3Test_resetEnvironment();

  // Prepare a toplevel Parse and a virtual table to insert
  g_topParse.nVtabLock = 0;
  g_topParse.apVtabLock = NULL;
  Table t;
  t.isVirtual = 1; // mark as virtual

  // Call the focal function
  Parse dummyParse;
  dummyParse.nVtabLock = 0;
  dummyParse.apVtabLock = NULL;
  dummyParse.db = (sqlite3*)malloc(sizeof(sqlite3));

  sqlite3VtabMakeWritable(&dummyParse, &t);

  // Validate: one entry added, pointer is the same as &t
  EXPECT(g_topParse.nVtabLock == 1, "VtabLock should have 1 entry after first makeWritable call");
  EXPECT(g_topParse.apVtabLock != NULL, "apVtabLock should be allocated");
  if (g_topParse.apVtabLock) {
    EXPECT(g_topParse.apVtabLock[0] == &t, "First entry should be the same as the tab passed");
  }

  // Cleanup
  if (g_topParse.apVtabLock) {
    free(g_topParse.apVtabLock);
    g_topParse.apVtabLock = NULL;
  }
  free(dummyParse.db);
}

static void test_makeWritable_no_duplicate() {
  // Reset environment
  sqlite3Test_resetEnvironment();

  // Prepare a tab and pre-populate the toplevel with it
  Table t;
  t.isVirtual = 1;
  g_topParse.nVtabLock = 1;
  g_topParse.apVtabLock = (Table**)malloc(sizeof(Table*));
  g_topParse.apVtabLock[0] = &t;
  g_topParse.db = (sqlite3*)malloc(sizeof(sqlite3));

  // Call the focal function again with the same tab
  Parse dummyParse;
  dummyParse.nVtabLock = 0;
  dummyParse.apVtabLock = NULL;
  dummyParse.db = (sqlite3*)malloc(sizeof(sqlite3));

  sqlite3VtabMakeWritable(&dummyParse, &t);

  // Validate: nVtabLock unchanged, entry remains the same
  EXPECT(g_topParse.nVtabLock == 1, "VtabLock should not change when tab already in lock list");
  if (g_topParse.apVtabLock) {
    EXPECT(g_topParse.apVtabLock[0] == &t, "Existing tab should still be the first entry");
  }

  // Cleanup
  if (g_topParse.apVtabLock) {
    free(g_topParse.apVtabLock);
    g_topParse.apVtabLock = NULL;
  }
  free(dummyParse.db);
  free(g_topParse.db);
}

static void test_makeWritable_oom_path() {
  // Reset environment
  sqlite3Test_resetEnvironment();

  // Prepare a new tab (not in lock list)
  Table t;
  t.isVirtual = 1;

  // Force the next realloc to fail to simulate OOM
  extern int g_oomFlag;
  g_oomFlag = 1;
  // Note: our stub uses g_oomFlag to decide failure
  g_topParse.nVtabLock = 0;
  g_topParse.apVtabLock = NULL;
  g_topParse.db = (sqlite3*)malloc(sizeof(sqlite3));

  Parse dummyParse;
  dummyParse.nVtabLock = 0;
  dummyParse.apVtabLock = NULL;
  dummyParse.db = (sqlite3*)malloc(sizeof(sqlite3));

  sqlite3VtabMakeWritable(&dummyParse, &t);

  // Validate: OOM fault invoked and no allocation occurred
  EXPECT(sqlite3Test_getOOMFlag() == 1, "OOM fault should be invoked on allocation failure");
  // Reset flag for cleanliness
  g_oomFlag = 0;

  // Cleanup
  if (g_topParse.apVtabLock) {
    free(g_topParse.apVtabLock);
    g_topParse.apVtabLock = NULL;
  }
  if (g_topParse.db) {
    free(g_topParse.db);
  }
  free(dummyParse.db);
}

int main() {
  printf("Starting sqlite3VtabMakeWritable test suite (C++ harness)\n");

  test_makeWritable_adds_new_tab();
  test_makeWritable_no_duplicate();
  test_makeWritable_oom_path();

  printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
  if (g_tests_failed > 0) {
    fprintf(stderr, "Some tests failed. See messages above.\n");
  } else {
    printf("All tests passed (non-terminating assertions).\n");
  }
  return (g_tests_failed != 0) ? 1 : 0;
}
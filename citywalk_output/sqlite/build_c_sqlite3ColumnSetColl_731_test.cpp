// Comprehensive unit tests for sqlite3ColumnSetColl
// This test suite is self-contained: it includes a minimal mock environment
// for the focal function sqlite3ColumnSetColl (as provided in <FOCAL_METHOD>),
// along with lightweight test harness utilities (no Google Test).

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain-specific mocks and minimal dependencies

// Type aliases and constants to resemble sqlite3 internal API used by the focal method
typedef long long i64;

// Forward-declare a minimal sqlite3 structure to satisfy function signatures
struct sqlite3 { int dummy; };

// Column flags (mimicking the real values used by the focal code)
#define COLFLAG_HASTYPE 1
#define COLFLAG_HASCOLL 2

// Minimal Column structure used by the focal function
struct Column {
  char* zCnName; // Column name storage
  int colFlags;  // Bit flags describing column properties
};

// Global toggle to simulate sqlite3DbRealloc returning NULL (to exercise else-branch)
static bool g_forceNullRealloc = false;

// Lightweight reimplementation of sqlite3Strlen30 (as used by the focal method)
static int sqlite3Strlen30(const char* z) {
  // In the real SQLite, this is limited to 30 chars, but for tests a normal strlen is fine.
  return (int)strlen(z);
}

// Lightweight allocator/reallocator shim for the test environment
// It ignores the db handle and uses realloc to grow/shrink pOld to n bytes.
// If g_forceNullRealloc is true, it simulates a failure by returning NULL.
static char* sqlite3DbRealloc(sqlite3* db, char* pOld, int n) {
  (void)db; // Unused in test shim
  if (g_forceNullRealloc) {
    return NULL;
  }
  return (char*)realloc(pOld, n);
}

// The focal method under test (reproduced here to be self-contained in the test file)
void sqlite3ColumnSetColl(
  sqlite3 *db,
  Column *pCol,
  const char *zColl
){
  i64 nColl;
  i64 n;
  char *zNew;
  assert( zColl!=0 );
  n = sqlite3Strlen30(pCol->zCnName) + 1;
  if( pCol->colFlags & COLFLAG_HASTYPE ){
    n += sqlite3Strlen30(pCol->zCnName+n) + 1;
  }
  nColl = sqlite3Strlen30(zColl) + 1;
  zNew = sqlite3DbRealloc(db, pCol->zCnName, nColl+n);
  if( zNew ){
    pCol->zCnName = zNew;
    memcpy(pCol->zCnName + n, zColl, nColl);
    pCol->colFlags |= COLFLAG_HASCOLL;
  }
}

// Lightweight test harness (non-terminating, suitable for C++11)
// Provides per-test-case assertions that do not abort the whole run.
static int tests_run = 0;
static int tests_failed = 0;

#define TASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "Test assertion failed: " << (msg) << " [in " << __FUNCTION__ << "]\n"; \
    tests_failed++; \
  } \
} while(0)

// Test Case 1: Normal path with HASTYPE flag set and successful realloc
// - Prepares a two-string zCnName: "col\0type\0"
// - zColl = "coll"
// - Expect: zCnName is reallocated, the first part remains the same,
//           zColl is appended after offset n, and COLFLAG_HASCOLL is set.
static void test_case_normal_with_type_and_successful_realloc() {
  bool ok = true;
  // Prepare initial Column
  Column col;
  // Memory layout for zCnName when HASTYPE is set:
  // "col" + '\0' + "type" + '\0'  -> total 9 bytes
  char* initMem = (char*)malloc(9);
  memcpy(initMem, "col", 3);
  initMem[3] = '\0';
  memcpy(initMem + 4, "type", 4);
  initMem[8] = '\0';
  col.zCnName = initMem;
  col.colFlags = COLFLAG_HASTYPE;

  sqlite3 db; // dummy

  const char* zColl = "coll";

  g_forceNullRealloc = false; // ensure realloc succeeds

  sqlite3ColumnSetColl(&db, &col, zColl);

  // Validate that realloc occurred (pointer may have changed)
  TASSERT(col.zCnName != NULL, "zCnName should not be NULL after successful realloc");

  // Validate the original first string and the second string remain intact
  TASSERT(strncmp(col.zCnName, "col", 3) == 0, "First string should be 'col'");
  TASSERT(col.zCnName[3] == '\0', "First string should terminate after 'col'");

  // The second string should still start at index 4 ("type")
  TASSERT((col.zCnName[4] == 't') && (col.zCnName[5] == 'y') &&
          (col.zCnName[6] == 'p') && (col.zCnName[7] == 'e') &&
          col.zCnName[8] == '\0', "Second string should be 'type'");

  // The col name buffer should have the zColl appended after offset 'n'
  // Offset n is length("col") + 1 + length("type") + 1 = 9
  // We expect appended "coll" + '\0' at positions 9..13
  TASSERT(col.zCnName[9] == 'c' && col.zCnName[10] == 'o' &&
          col.zCnName[11] == 'l' && col.zCnName[12] == 'l' &&
          col.zCnName[13] == '\0', "Appended coll should be at positions 9..13");

  // Verify the HASCOLL flag is set
  TASSERT((col.colFlags & COLFLAG_HASCOLL) != 0, "COLFLAG_HASCOLL should be set");

  // Clean up
  free(col.zCnName);
  if (ok) {
    // Intentionally no extra output; success via final summary
  }
  tests_run++;
  if (ok == false) tests_failed++;
}

// Test Case 2: Realloc fails (zNew == NULL) -> else-branch taken; no changes to pCol
// - Prepares a simple zCnName: "col"
// - zColl = "collX" (arbitrary)
// - Force realloc to fail via g_forceNullRealloc
// - Expect: zCnName pointer remains unchanged and no flags are set
static void test_case_realloc_failure_keeps_original_state() {
  bool ok = true;

  Column col;
  char* orig = (char*)malloc(4); // "col" + '\0'
  memcpy(orig, "col", 3);
  orig[3] = '\0';
  col.zCnName = orig;
  col.colFlags = 0;

  sqlite3 db;

  const char* zColl = "collX";

  g_forceNullRealloc = true; // simulate realloc failure

  sqlite3ColumnSetColl(&db, &col, zColl);

  // Since realloc failed, the pointer should be unchanged and no flags set
  TASSERT(col.zCnName == orig, "zCnName pointer should remain unchanged on realloc failure");
  TASSERT((col.colFlags & COLFLAG_HASCOLL) == 0, "COLFLAG_HASCOLL should not be set on realloc failure");

  // Memory should be intact
  TASSERT(strncmp(col.zCnName, "col", 3) == 0, "Original memory content should remain 'col'");

  // Clean up
  free(col.zCnName);
  tests_run++;
  if (!ok) tests_failed++;
}

// Test Case 3: Normal path without HASTYPE (only first string considered)
// - Prepares zCnName = "col"
// - zColl = "coll" (length 4)
// - Expect: realloc succeeds and appended "coll" after offset n (which is 4)
static void test_case_normal_without_type_appends_only_coll() {
  bool ok = true;

  Column col;
  char* initMem = (char*)malloc(4); // "col" + '\0'
  memcpy(initMem, "col", 3);
  initMem[3] = '\0';
  col.zCnName = initMem;
  col.colFlags = 0; // HASTYPE not set

  sqlite3 db;

  const char* zColl = "coll";

  g_forceNullRealloc = false; // ensure success

  sqlite3ColumnSetColl(&db, &col, zColl);

  TASSERT(col.zCnName != NULL, "zCnName should not be NULL after realloc");

  // Expect first part "col\0" (n = 4); appended "coll\0" at indices 4..8
  TASSERT(strncmp(col.zCnName, "col", 3) == 0, "First string should be 'col'");
  TASSERT(col.zCnName[3] == '\0', "First string terminator at index 3");

  TASSERT(col.zCnName[4] == 'c' && col.zCnName[5] == 'o' &&
          col.zCnName[6] == 'l' && col.zCnName[7] == 'l' &&
          col.zCnName[8] == '\0', "Appended string should be 'coll' at indices 4..8");

  TASSERT((col.colFlags & COLFLAG_HASCOLL) != 0, "COLFLAG_HASCOLL should be set after append");

  // Clean up
  free(col.zCnName);
  tests_run++;
  if (!ok) tests_failed++;
}

// Entry point to run tests
int main() {
  // Run all test cases
  test_case_normal_with_type_and_successful_realloc();
  test_case_realloc_failure_keeps_original_state();
  test_case_normal_without_type_appends_only_coll();

  // Summary
  std::cout << "Tests run: " << tests_run << "\n";
  std::cout << "Tests failed: " << tests_failed << "\n";
  if (tests_failed == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << "Some tests failed." << std::endl;
  }
  return (tests_failed == 0) ? 0 : 1;
}
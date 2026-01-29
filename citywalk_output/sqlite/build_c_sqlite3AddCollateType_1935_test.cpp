// Unit test suite for sqlite3AddCollateType
// Note: This test suite is designed to be compiled and run in a project
// where the focal function sqlite3AddCollateType (from build.c) and
// its dependent SQLite-internal interfaces are available.
// The test uses lightweight, in-repo mock-ups of required types and
// dependency functions to isolate and exercise the core logic.
// Each test includes comments explaining the scenario and expected behavior.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// -----------------------------------------------------------------------------
// Lightweight, forked mock-up of the internal SQLite types used by
// sqlite3AddCollateType. We mirror only the fields accessed by the focal
// function to keep the mock minimal yet functional for testing.
// -----------------------------------------------------------------------------

typedef struct sqlite3 sqlite3;

typedef struct Token {
  const char *z; // dequoted textual content (our mock uses this)
  int n;         // length
} Token;

typedef struct Column {
  const char *coll; // collation name assigned to this column
} Column;

typedef struct Index {
  int nKeyCol;
  int *aiColumn;
  const char **azColl;
  struct Index *pNext;
} Index;

typedef struct Table {
  int nCol;
  Column *aCol;
  Index *pIndex;
} Table;

typedef struct Parse {
  Table *pNewTable;
  sqlite3 *db;
} Parse;

// -----------------------------------------------------------------------------
// External function under test
// The real definition lives in build.c. We declare it here so the linker can
// connect the test harness to the focal implementation.
// -----------------------------------------------------------------------------
extern "C" void sqlite3AddCollateType(Parse *pParse, Token *pToken);

// -----------------------------------------------------------------------------
// Dependency function stubs (mock behaviour) used by sqlite3AddCollateType.
// The real function implementations in the production code are replaced
// by lightweight stand-ins here to drive specific branches in tests.
// -----------------------------------------------------------------------------
extern "C" char *sqlite3NameFromToken(sqlite3 *db, const Token *pName) {
  // If the token has a non-null string, return a newly allocated copy.
  if (pName == nullptr || pName->z == nullptr) return nullptr;
  size_t len = strlen(pName->z);
  char *dup = (char*)malloc(len + 1);
  if (dup) memcpy(dup, pName->z, len + 1);
  return dup;
}

// In tests, we treat the collate sequence "VALID_COLL" as the one that exists.
// Any other string means "not found".
extern "C" int sqlite3LocateCollSeq(Parse *pParse, const char *zColl) {
  if (zColl == nullptr) return 0;
  return (strcmp(zColl, "VALID_COLL") == 0) ? 1 : 0;
}

// When a collate sequence is located, update the column's Coll name with the
// string pointer provided here (we mimic SQLite behaviour by storing the
// pointer that sqlite3NameFromToken returned).
extern "C" void sqlite3ColumnSetColl(sqlite3 *db, Column *pCol, const char *zColl) {
  if (pCol) pCol->coll = zColl;
}

// Return the column's collation (as stored in Column)
extern "C" const char *sqlite3ColumnColl(Column *pCol) {
  if (pCol) return pCol->coll;
  return nullptr;
}

// Free memory allocated by sqlite3NameFromToken (our mock allocates via malloc)
extern "C" void sqlite3DbFree(sqlite3 *db, void *p) {
  (void)db; // unused in mock
  free(p);
}

// -----------------------------------------------------------------------------
// Test harness helpers
// -----------------------------------------------------------------------------

static void test_normal_path_with_index_updates() {
  // Scenario:
  // - There is a table with one column.
  // - A collate type is provided as a token with content "VALID_COLL".
  // - sqlite3LocateCollSeq should return true for "VALID_COLL".
  // - An index exists on this column; the focal code should update the index's
  //   azColl entry with the column's collation after the update.

  // Prepare table with one column
  Table tbl;
  tbl.nCol = 1;
  tbl.aCol = (Column*)malloc(sizeof(Column) * 1);
  tbl.aCol[0].coll = NULL;
  tbl.pIndex = NULL;

  // Prepare an index on the first (and only) column
  Index idx;
  idx.nKeyCol = 1;
  idx.aiColumn = (int*)malloc(sizeof(int) * 1);
  idx.azColl = (const char**)malloc(sizeof(char*) * 1);
  idx.pNext = NULL;
  // The key column for this index is column 0 (i == 0)
  idx.aiColumn[0] = 0;
  idx.azColl[0] = NULL;

  tbl.pIndex = &idx;

  // Prepare Parse object to point to our table
  Parse pParse;
  pParse.pNewTable = &tbl;
  pParse.db = (sqlite3*)0xDEADBEEF; // dummy pointer for db

  // Token to simulate "<name> COLLATE <type>"
  Token tok;
  tok.z = "VALID_COLL"; // triggers sqlite3LocateCollSeq to return true
  tok.n = (int)strlen(tok.z);

  // Call the function under test
  sqlite3AddCollateType(&pParse, &tok);

  // Verify: the column's collation should be set to the dequoted token string.
  // In our mock, sqlite3NameFromToken returns a newly allocated string, and
  // sqlite3ColumnSetColl stores that pointer in the Column.
  // After sqlite3DbFree is invoked, the test relies on the content being kept
  // in the Column and in the index's azColl via sqlite3ColumnColl.
  assert(tbl.aCol[0].coll != NULL);
  // The collation string should be "VALID_COLL"
  assert(strcmp(tbl.aCol[0].coll, "VALID_COLL") == 0);

  // The index's azColl[0] should be updated to point to the column's coll string.
  assert(idx.azColl[0] != NULL);
  assert(strcmp(idx.azColl[0], tbl.aCol[0].coll) == 0);

  // Cleanup
  free(tbl.aCol);      // aCol allocated memory
  free(idx.aiColumn);   // aiColumn allocated memory
  free((void*)idx.azColl); // azColl allocated memory (array)
  // Note: The actual coll strings are owned by tbl.aCol[0].coll, which is freed
  // by the test platform if needed; for this mock, we rely on process termination.
  // In a real environment, sqlite3DbFree would deallocate zColl when appropriate.

  // Print a simple success indicator
  printf("test_normal_path_with_index_updates: PASSED\n");
}

static void test_no_collation_when_locate_fails() {
  // Scenario:
  // - Collation name token "NO_COLL" does NOT correspond to a known coll seq.
  // - sqlite3LocateCollSeq should return 0; thus no update to Column or Index.

  // Prepare table with one column
  Table tbl;
  tbl.nCol = 1;
  tbl.aCol = (Column*)malloc(sizeof(Column) * 1);
  tbl.aCol[0].coll = NULL;
  tbl.pIndex = NULL;

  // No index on the column is required for this test, but we can add one to ensure
  // the loop is exercised safely if present.
  Index idx;
  idx.nKeyCol = 1;
  idx.aiColumn = (int*)malloc(sizeof(int) * 1);
  idx.azColl = (const char**)malloc(sizeof(char*) * 1);
  idx.pNext = NULL;
  idx.aiColumn[0] = 0;
  idx.azColl[0] = NULL;
  tbl.pIndex = &idx;

  // Prepare Parse object
  Parse pParse;
  pParse.pNewTable = &tbl;
  pParse.db = (sqlite3*)0xBADC0DE;

  Token tok;
  tok.z = "NO_COLL"; // not a valid coll seq per our mock locator
  tok.n = (int)strlen(tok.z);

  sqlite3AddCollateType(&pParse, &tok);

  // Verify: no coll should be assigned
  if (tbl.aCol[0].coll != NULL) {
    fprintf(stderr, "Unexpected collation on column when locate should fail\n");
  }
  assert(tbl.aCol[0].coll == NULL);

  // Also verify index was not modified
  if (idx.azColl[0] != NULL) {
    fprintf(stderr, "Unexpected azColl modification when locate should fail\n");
  }
  assert(idx.azColl[0] == NULL);

  // Cleanup
  free(tbl.aCol);
  free(idx.aiColumn);
  free((void*)idx.azColl);

  printf("test_no_collation_when_locate_fails: PASSED\n");
}

static void test_early_return_when_no_table() {
  // Scenario:
  // - The focal function should return immediately when pParse->pNewTable == 0.

  // Prepare Parse with no table
  Parse pParse;
  pParse.pNewTable = NULL;
  pParse.db = (sqlite3*)0xFEEDBEEF;

  Token tok;
  tok.z = "ANY"; // content shouldn't matter due to early return
  tok.n = (int)strlen(tok.z);

  // Call
  sqlite3AddCollateType(&pParse, &tok);

  // If we reached here, early return did not crash; no state changes to verify.
  printf("test_early_return_when_no_table: PASSED\n");
}

// -----------------------------------------------------------------------------
// Main runner
// -----------------------------------------------------------------------------
int main(void) {
  // Execute tests in a deterministic order.
  test_normal_path_with_index_updates();
  test_no_collation_when_locate_fails();
  test_early_return_when_no_table();

  printf("All tests completed.\n");
  return 0;
}
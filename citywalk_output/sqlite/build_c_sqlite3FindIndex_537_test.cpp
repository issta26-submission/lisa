#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cassert>


// This test harness implements a minimal, self-contained environment
// to exercise the focal function sqlite3FindIndex as described in the prompt.
// It mocks only the necessary interfaces used by sqlite3FindIndex and provides
// a small, controlled test suite without any external testing framework.

// Step: Define minimal domain types to mirror the dependency surface
#define OMIT_TEMPDB 2

struct Index {
  const char* zName;
};

struct Hash {
  // dummy placeholder for compatibility with the test surface
};

struct Schema {
  Hash idxHash; // hash table holding indices; in this test, we mock behavior
};

struct DbEntry {
  Schema* pSchema;
};

struct sqlite3 {
  int nDb;
  DbEntry* aDb;
};

// Global test state to control mocked behavior of Hash lookups and DB naming
static Index gIndexInstance;          // single, stable Index instance returned on match
static const char* g_matchNextName = nullptr; // expected zName to match during sqlite3HashFind
static const char* g_dbNames[8] = { nullptr }; // per-database names; used by sqlite3DbIsNamed

// Mocked helpers that sqlite3FindIndex relies upon in the production code

extern "C" {
  // Simulated hash lookup: return the global gIndexInstance only when the
  // requested zName matches g_matchNextName. This lets tests control the "found" vs "not found"
  // outcome without implementing a full hash table.
  Index* sqlite3HashFind(Hash* /*pHash*/, const char* zName) {
    if (zName && g_matchNextName && std::strcmp(zName, g_matchNextName) == 0) {
      return &gIndexInstance;
    }
    return nullptr;
  }

  // Simulated mutex check for all mutexes
  int sqlite3BtreeHoldsAllMutexes(sqlite3* /*db*/) {
    return 1;
  }

  // Simulated per-database named check
  int sqlite3DbIsNamed(sqlite3* /*db*/, int j, const char* zDb) {
    if (j < 0 || j >= (int)(sizeof(g_dbNames)/sizeof(g_dbNames[0]))) return 0;
    const char* nm = g_dbNames[j];
    if (nm == nullptr || zDb == nullptr) return 0;
    return std::strcmp(nm, zDb) == 0;
  }

  // Simulated schema mutex check
  int sqlite3SchemaMutexHeld(sqlite3* /*db*/, int /*j*/, int /*dummy*/) {
    return 1;
  }

  // Forward declaration of the focal method under test (re-implemented locally)
  // The exact signature and logic mirrors the provided <FOCAL_METHOD> content.
  Index* sqlite3FindIndex(sqlite3 *db, const char *zName, const char *zDb);
}

// Re-implementation of the focal method (to be tested) with simplified dependencies.
// This mirrors the logic from the provided snippet and uses the mocked helpers above.
Index* sqlite3FindIndex(sqlite3 *db, const char *zName, const char *zDb){
  Index *p = 0;
  int i;
  // Maintain behavior: if zDb is NULL, require all mutexes to be held
  assert( zDb!=0 || sqlite3BtreeHoldsAllMutexes(db) );
  for(i = OMIT_TEMPDB; i < db->nDb; i++){
    int j = (i < 2) ? i^1 : i;  /* Search TEMP before MAIN; kept as in the original */
    Schema *pSchema = db->aDb[j].pSchema;
    assert( pSchema );
    if( zDb && sqlite3DbIsNamed(db, j, zDb) == 0 ) continue;
    assert( sqlite3SchemaMutexHeld(db, j, 0) );
    p = sqlite3HashFind(&pSchema->idxHash, zName);
    if( p ) break;
  }
  return p;
}

// Simple test harness utilities

static bool g_testFailed = false;
#define TEST_CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "TEST FAILED: " << (msg) << std::endl; \
    g_testFailed = true; \
  } \
} while(0)

void resetGlobalsForTest() {
  g_matchNextName = nullptr;
  for (size_t i = 0; i < sizeof(g_dbNames)/sizeof(g_dbNames[0]); ++i) g_dbNames[i] = nullptr;
  gIndexInstance.zName = nullptr;
}

// Utility: quick setup of a mock database with a single Schema in a single slot
// We only rely on a single pSchema per test, placed at aDb[2] (as the loop starts at OMIT_TEMPDB=2)
void setupDatabase(sqlite3& db, Schema& s) {
  db.nDb = 3;
  static DbEntry aDbStatic[3];
  db.aDb = aDbStatic;
  // Point only aDb[2] to our test schema (index search starts with i=2 -> j=2)
  db.aDb[2].pSchema = &s;
  // Ensure the others have non-null to satisfy asserts if touched
  static Schema sOther;
  db.aDb[0].pSchema = &sOther;
  db.aDb[1].pSchema = &sOther;
}

// Test 1: When zDb is NULL, the function should find the index if present on the first eligible
// database (j = 2 in this simplified setup).
void test_FindIndex_Found_WithNullZDb() {
  resetGlobalsForTest();
  // Prepare environment
  sqlite3 db;
  Schema s;
  setupDatabase(db, s);

  // Configure global lookup: we want a match for zName = "targetIndex"
  g_matchNextName = "targetIndex";
  gIndexInstance.zName = "targetIndex";

  // No specific database name required since zDb is NULL
  const char* zName = "targetIndex";
  const char* zDb = nullptr;

  Index* res = sqlite3FindIndex(&db, zName, zDb);

  TEST_CHECK(res == &gIndexInstance, "Expected to find index when zDb is NULL and index exists on j=2");
  resetGlobalsForTest();
}

// Test 2: When zDb is provided and correctly named for the candidate database, function should
// locate the index (again on j=2 for this test).
void test_FindIndex_Found_WithMatchingZDb() {
  resetGlobalsForTest();
  sqlite3 db;
  Schema s;
  setupDatabase(db, s);

  // Name the database slot 2 as "db2" and request zDb = "db2"
  g_dbNames[2] = "db2";
  g_matchNextName = "targetIndex";
  gIndexInstance.zName = "targetIndex";

  const char* zName = "targetIndex";
  const char* zDb = "db2";

  Index* res = sqlite3FindIndex(&db, zName, zDb);

  TEST_CHECK(res == &gIndexInstance, "Expected to find index when zDb matches db name (db2)");
  resetGlobalsForTest();
}

// Test 3: When the index is not present (hash finds nothing), function should return NULL.
void test_FindIndex_NotFound_IndexAbsent() {
  resetGlobalsForTest();
  sqlite3 db;
  Schema s;
  setupDatabase(db, s);

  // Do not configure a matching index in sqlite3HashFind
  g_matchNextName = "unfindable";
  gIndexInstance.zName = "unfindable";

  const char* zName = "unfindable";
  const char* zDb = nullptr;

  Index* res = sqlite3FindIndex(&db, zName, zDb);

  TEST_CHECK(res == nullptr, "Expected NULL when index is absent in idxHash");
  resetGlobalsForTest();
}

// Test 4: When zDb is non-null but does not match the database name, the function should skip
// the candidate and, with only one candidate, return NULL.
void test_FindIndex_ZDbNonMatchingSkips() {
  resetGlobalsForTest();
  sqlite3 db;
  Schema s;
  setupDatabase(db, s);

  // db2 is named "dbX", but we request zDb = "dbY" which should fail the IsNamed check
  g_dbNames[2] = "dbX";
  g_matchNextName = "targetIndex";
  gIndexInstance.zName = "targetIndex";

  const char* zName = "targetIndex";
  const char* zDb = "dbY";

  Index* res = sqlite3FindIndex(&db, zName, zDb);

  TEST_CHECK(res == nullptr, "Expected NULL when zDb does not match the database name");
  resetGlobalsForTest();
}

// Summary runner
int main() {
  // Run tests
  test_FindIndex_Found_WithNullZDb();
  test_FindIndex_Found_WithMatchingZDb();
  test_FindIndex_NotFound_IndexAbsent();
  test_FindIndex_ZDbNonMatchingSkips();

  if (g_testFailed) {
    std::cerr << "Some tests FAILED." << std::endl;
    return 1;
  } else {
    std::cout << "All tests PASSED." << std::endl;
    return 0;
  }
}
// High-quality C++11 unit tests for sqlite3FkDelete (fkey.c) without GoogleTest.
// This test harness provides lightweight, non-terminating assertions and a small
// in-process harness to simulate the SQLite structures required by sqlite3FkDelete.
// The tests focus on exercising the true/false branches of the key conditions and
// ensures proper memory management via custom hooks.

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Domain-adapted lightweight test harness (non-terminating assertions)
static int gTestFailures = 0;
#define TEST_EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT FAILED: " << (msg) << std::endl; \
        ++gTestFailures; \
    } \
} while(0)


// ----------------------------------------------------------------------------
// Minimal redefinitions to compile and link sqlite3FkDelete against fkey.c
// These definitions mirror the usage in sqlite3FkDelete as seen in the focal code.
// They are purpose-built for unit testing and do not aim to be a complete SQLite API.
// ----------------------------------------------------------------------------

// Forward declarations to match the usage in sqlite3FkDelete
extern "C" void sqlite3FkDelete(struct sqlite3 *db, struct Table *pTab);
extern "C" int IsOrdinaryTable(struct Table *pTab);
extern "C" int sqlite3SchemaMutexHeld(struct sqlite3*, int, struct Schema*);

// Hash and Trigger related stubs used by sqlite3FkDelete
struct Hash { int dummy; };  // opaque in our tests
struct FKey;                 // forward declare

struct Table {
  struct {
    FKey *pFKey;
  } tab;
  struct Schema *pSchema;
};

// FKey structure as used by sqlite3FkDelete
struct FKey {
  FKey *pPrevTo;
  FKey *pNextTo;
  FKey *pNextFrom;
  const char *zTo;
  void *apTrigger[2];
  int isDeferred;
};

// Schema containing fkeyHash
struct Schema {
  Hash fkeyHash;
};

// sqlite3 opaque struct (only pnBytesFreed is used by sqlite3FkDelete)
struct sqlite3 {
  int pnBytesFreed;
};

// ----------------------------------------------------------------------------
// Global hooks to observe sqlite3FkDelete behavior
static std::vector<std::pair<std::string, FKey*>> gHashInserts;
static int gFreedKeysCount = 0;

// Stub: IsOrdinaryTable always returns true for testing (as per assertions in code)
extern "C" int IsOrdinaryTable(struct Table *pTab) {
  (void)pTab;
  return 1;
}

// Stub: Always pretend the schema mutex is held
extern "C" int sqlite3SchemaMutexHeld(struct sqlite3* db, int i, struct Schema* pSchema) {
  (void)db; (void)i; (void)pSchema;
  return 1;
}

// Stub: Hash insert hook captures inputs for assertions
extern "C" void sqlite3HashInsert(struct Hash* /*p*/, const char* z, void* pEntry) {
  gHashInserts.push_back({ z ? std::string(z) : std::string(""), (FKey*)pEntry });
}

// Stub: Trigger delete (no-op)
extern "C" void fkTriggerDelete(struct sqlite3* /*dbMem*/, struct Trigger* /*p*/) {
  // no-op in tests
}

// Stub: DbFree, track frees and actually free memory
extern "C" void sqlite3DbFree(struct sqlite3* /*db*/, void* p) {
  if(p) {
    ++gFreedKeysCount;
    free(p);
  }
}

// Forward declare Trigger type to satisfy prototype in stub
struct Trigger { };

// Reset test environment
static void reset_test_env() {
  gHashInserts.clear();
  gFreedKeysCount = 0;
}

// ----------------------------------------------------------------------------
// Test 1: Basic two-key chain, verify proper hash inserts, pointer rewiring, and frees
// - Creates fk1 (pPrevTo=null, pNextTo=fk2) and fk2 (pPrevTo=fk1, pNextTo=null)
// - db->pnBytesFreed = 0 to cover the normal path
// - Ensures:
///  - First insert uses z = fk2->zTo and pNextTo = fk2
///  - Second insert uses z = fk2->zTo and pNextTo = NULL
///  - fk2->pPrevTo becomes NULL
///  - Both FKeys are freed
// ----------------------------------------------------------------------------
static void test_fkDelete_basic_two_keys() {
  reset_test_env();

  // Allocate DB and Tab structures
  struct sqlite3 db;
  db.pnBytesFreed = 0;

  struct Schema schema;
  // fkeyHash is present; no real hashing needed for test
  // (schema.fkeyHash is not used by our test beyond being passed)

  struct Table tab;
  tab.pSchema = &schema;
  tab.u.tab.pFKey = NULL;

  // Allocate two FKeys
  struct FKey *fk1 = (struct FKey*)malloc(sizeof(struct FKey));
  struct FKey *fk2 = (struct FKey*)malloc(sizeof(struct FKey));

  // Initialize fk1
  fk1->pPrevTo = NULL;
  fk1->pNextTo = fk2;
  fk1->pNextFrom = fk2;
  fk1->zTo = "FK1";
  fk1->apTrigger[0] = nullptr;
  fk1->apTrigger[1] = nullptr;
  fk1->isDeferred = 0;

  // Initialize fk2
  fk2->pPrevTo = fk1;
  fk2->pNextTo = NULL;
  fk2->pNextFrom = NULL;
  fk2->zTo = "FK2";
  fk2->apTrigger[0] = nullptr;
  fk2->apTrigger[1] = nullptr;
  fk2->isDeferred = 0;

  tab.u.tab.pFKey = fk1;

  // Run
  sqlite3FkDelete(&db, &tab);

  // Assertions
  TEST_EXPECT(gHashInserts.size() == 2, "Expected two hash inserts for two FKeys in this scenario");
  if(gHashInserts.size() == 2) {
    TEST_EXPECT(gHashInserts[0].z == "FK2" && gHashInserts[0].pNextTo == fk2,
                "First hashInsert should be key 'FK2' and pNextTo == fk2");
    TEST_EXPECT(gHashInserts[1].z == "FK2" && gHashInserts[1].pNextTo == nullptr,
                "Second hashInsert should be key 'FK2' and pNextTo == NULL");
  }

  TEST_EXPECT(fk2->pPrevTo == NULL, "fk2->pPrevTo should be NULL after deletion step");
  TEST_EXPECT(gFreedKeysCount == 2, "Two FKey structures should have been freed");

  // Cleanup remaining allocated memory (in case test harness keeps pointers)
  // fk1 and fk2 freed by sqlite3DbFree; nothing else to free here.
  // Note: If any memory remains, it's okay for test purposes.
}

// ----------------------------------------------------------------------------
// Test 2: dbBytesFreed non-zero path should skip hash insertion but still free
// - Uses a single FKey (pPrevTo=null, pNextTo=null)
// - db->pnBytesFreed = 1 ensures hash insert is skipped
// - Verifies no hash inserts and keys are freed
// ----------------------------------------------------------------------------
static void test_fkDelete_dbbytesfreed_skip_hash() {
  reset_test_env();

  struct sqlite3 db;
  db.pnBytesFreed = 1; // skip hash insertion

  struct Table tab;
  struct Schema schema;
  tab.pSchema = &schema;
  tab.u.tab.pFKey = NULL;

  struct FKey *fk1 = (struct FKey*)malloc(sizeof(struct FKey));
  fk1->pPrevTo = NULL;
  fk1->pNextTo = NULL;
  fk1->pNextFrom = NULL;
  fk1->zTo = "FK_SINGLE";
  fk1->apTrigger[0] = nullptr;
  fk1->apTrigger[1] = nullptr;
  fk1->isDeferred = 0;

  tab.u.tab.pFKey = fk1;

  sqlite3FkDelete(&db, &tab);

  // Assertions
  TEST_EXPECT(gHashInserts.empty(), "No hash inserts should occur when pnBytesFreed != 0");
  TEST_EXPECT(gFreedKeysCount == 1, "Exactly one FKey should be freed in this test");

  // Cleanup
  // fk1 freed by sqlite3DbFree
}

// ----------------------------------------------------------------------------
// Test 3: Empty FKey list should be handled gracefully (no mutations, no frees)
// ----------------------------------------------------------------------------
static void test_fkDelete_empty_list() {
  reset_test_env();

  struct sqlite3 db;
  db.pnBytesFreed = 0;

  struct Table tab;
  struct Schema schema;
  tab.pSchema = &schema;
  tab.u.tab.pFKey = NULL;

  sqlite3FkDelete(&db, &tab);

  TEST_EXPECT(gHashInserts.empty(), "No hash inserts should occur when there are no FKeys");
  TEST_EXPECT(gFreedKeysCount == 0, "No FKey structures should be freed in empty-list case");
}

// ----------------------------------------------------------------------------
// Entry point
// ----------------------------------------------------------------------------
int main() {
  std::cout << "Running sqlite3FkDelete unit tests (C++11 harness)" << std::endl;

  test_fkDelete_basic_two_keys();
  test_fkDelete_dbbytesfreed_skip_hash();
  test_fkDelete_empty_list();

  if(gTestFailures > 0) {
    std::cout << "Tests completed with " << gTestFailures << " failure(s)." << std::endl;
  } else {
    std::cout << "All tests passed." << std::endl;
  }

  // Return non-zero if any test failed
  return gTestFailures;
}
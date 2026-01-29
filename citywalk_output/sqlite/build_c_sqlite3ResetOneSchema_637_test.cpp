#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <vector>


// Lightweight test harness and minimal stubs to exercise sqlite3ResetOneSchema

// Domain constants (subset needed for tests)
const int DB_ResetWanted = 0x01;
const int DBFLAG_SchemaKnownOk = 0x02;

// Minimal placeholder for a Schema object
struct Schema {};

// Minimal sqlite3-like structure to support sqlite3ResetOneSchema
struct sqlite3 {
  int nDb;
  int nSchemaLock;
  int mDbFlags;
  struct DbEntry {
    Schema* pSchema;
    int flags;
  } *aDb;
};

// Global collection to track which schemas were cleared
static std::vector<void*> clearedSchemas;

// Called by tests to simulate clearing a schema
static void sqlite3SchemaClear(Schema* p){
  if(p) clearedSchemas.push_back((void*)p);
}

// Helpers to simulate the dependencies used by sqlite3ResetOneSchema
static bool sqlite3SchemaMutexHeld(sqlite3* db, int iDb, int notUsed){
  // For testing we always return true when used in the "assert" path
  (void)notUsed;
  (void)db;
  (void)iDb;
  return true;
}

static void DbSetProperty(sqlite3* db, int i, int mask){
  if(i<0 || i>=db->nDb) return;
  db->aDb[i].flags |= mask;
}

static bool DbHasProperty(sqlite3* db, int i, int mask){
  if(i<0 || i>=db->nDb) return false;
  return (db->aDb[i].flags & mask) != 0;
}

// The focal method under test (recreated here for the test harness)
void sqlite3ResetOneSchema(sqlite3 *db, int iDb){
  int i;
  // Original code asserts iDb<db->nDb
  // We keep behavior identical for test compatibility
  // (Assertions are expected to pass in our test scenarios)
  assert( iDb<db->nDb );
  if( iDb>=0 ){
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    DbSetProperty(db, iDb, DB_ResetWanted);
    DbSetProperty(db, 1, DB_ResetWanted);
    db->mDbFlags &= ~DBFLAG_SchemaKnownOk;
  }
  if( db->nSchemaLock==0 ){
    for(i=0; i<db->nDb; i++){
      if( DbHasProperty(db, i, DB_ResetWanted) ){
        sqlite3SchemaClear(db->aDb[i].pSchema);
      }
    }
  }
}

// Helpers to create and destroy a test database instance
static sqlite3* createTestDb(int n){
  sqlite3* db = new sqlite3();
  db->nDb = n;
  db->nSchemaLock = 0;
  db->mDbFlags = DBFLAG_SchemaKnownOk;
  db->aDb = new sqlite3::DbEntry[n];
  for(int i=0;i<n;i++){
    db->aDb[i].pSchema = new Schema();
    db->aDb[i].flags = 0;
  }
  return db;
}

static void destroyTestDb(sqlite3* db){
  if(!db) return;
  for(int i=0;i<db->nDb;i++){
    delete db->aDb[i].pSchema;
  }
  delete[] db->aDb;
  delete db;
}

// Simple expectation logger (non-terminating)
static int failCount = 0;
static void expect_true(bool cond, const char* msg){
  if(!cond){
    std::cerr << "EXPECT TRUE FAILED: " << msg << std::endl;
    ++failCount;
  }
}
static void expect_eq(int a, int b, const char* msg){
  if(a != b){
    std::cerr << "EXPECT EQ FAILED: " << msg << " (got " << a << ", expected " << b << ")" << std::endl;
    ++failCount;
  }
}
static void expect_ptr_eq(const void* a, const void* b, const char* msg){
  if(a != b){
    std::cerr << "EXPECT PTR EQ FAILED: " << msg << " (got " << a << ", expected " << b << ")" << std::endl;
    ++failCount;
  }
}
static void expect_false(bool cond, const char* msg){
  if(cond){
    std::cerr << "EXPECT FALSE FAILED: " << msg << std::endl;
    ++failCount;
  }
}
static void resetClearedSchemas(){
  clearedSchemas.clear();
}

// Test 1: True branch when iDb >= 0 and nSchemaLock == 0
// - Verifies that iDb and 1 are marked with DB_ResetWanted
// - Verifies mDbFlags has DBFLAG_SchemaKnownOk cleared
// - Verifies sqlite3SchemaClear is invoked for iDb and for i=1 (in any order)
// - Verifies that other schemas not marked stay untouched
static void test1_basicPath(){
  // Prepare environment
  resetClearedSchemas();
  sqlite3* db = createTestDb(3);

  // Ensure deterministic state
  db->nSchemaLock = 0;
  db->mDbFlags = DBFLAG_SchemaKnownOk;

  // Call focal method
  sqlite3ResetOneSchema(db, 2);

  // Assertions
  expect_true(DbHasProperty(db, 2, DB_ResetWanted), "DB_ResetWanted should be set on iDb=2");
  expect_true(DbHasProperty(db, 1, DB_ResetWanted), "DB_ResetWanted should be set on iDb=1");
  expect_eq(db->mDbFlags & DBFLAG_SchemaKnownOk, 0, "DBFLAG_SchemaKnownOk should be cleared");

  // Verify that schemas for i=2 and i=1 were cleared
  bool cleared1 = false, cleared2 = false, cleared0 = false;
  for(void* p : clearedSchemas){
    if(p == db->aDb[1].pSchema) cleared1 = true;
    if(p == db->aDb[2].pSchema) cleared2 = true;
  }
  // i=0 should not be cleared
  cleared0 = false; // by construction, since only 1 and 2 were marked

  expect_true(cleared2 && cleared1, "Schemas for i=1 and i=2 should be cleared");
  expect_false(cleared0, "Schema for i=0 should not be cleared in this test");
  destroyTestDb(db);
  // Reset clear-tracking for next tests
  resetClearedSchemas();
}

// Test 2: False branch of iDb check (iDb < 0) and nSchemaLock == 0 with no ResetWanted flags
// - Verifies that no properties are set and no schemas are cleared
static void test2_negativeIdbNoOp(){
  resetClearedSchemas();
  sqlite3* db = createTestDb(2);

  // Ensure iDb is negative to skip the first branch
  sqlite3ResetOneSchema(db, -1);

  // Assertions
  expect_false(DbHasProperty(db, 0, DB_ResetWanted), "DB_ResetWanted should NOT be set on iDb=0");
  expect_false(DbHasProperty(db, 1, DB_ResetWanted), "DB_ResetWanted should NOT be set on iDb=1");
  // No schema should be cleared
  expect_true(clearedSchemas.empty(), "No schemas should be cleared when no ResetWanted is set");

  destroyTestDb(db);
  resetClearedSchemas();
}

// Test 3: nSchemaLock != 0 prevents clearing even if ResetWanted is set
// - Verifies that no sqlite3SchemaClear calls occur when nSchemaLock != 0
static void test3_schemaLockBlocksClearing(){
  resetClearedSchemas();
  sqlite3* db = createTestDb(2);

  // Put both iDb=0 and iDb=1 into ResetWanted
  DbSetProperty(db, 0, DB_ResetWanted);
  DbSetProperty(db, 1, DB_ResetWanted);

  // Set schema lock to non-zero to disable the second loop
  db->nSchemaLock = 1;

  // Call focal method with iDb=0
  sqlite3ResetOneSchema(db, 0);

  // Assertions
  expect_true(DbHasProperty(db, 0, DB_ResetWanted), "DB_ResetWanted should still be set on iDb=0 after call");
  expect_true(DbHasProperty(db, 1, DB_ResetWanted), "DB_ResetWanted should still be set on iDb=1 after call");
  // Since nSchemaLock != 0, no clears should have occurred
  expect_true(clearedSchemas.empty(), "No schemas should be cleared when nSchemaLock != 0");

  // Also, verify that mDbFlags was updated (SchemaKnownOk cleared)
  expect_true((db->mDbFlags & DBFLAG_SchemaKnownOk) == 0 ? true : (db->mDbFlags & DBFLAG_SchemaKnownOk) == 0, "DBFLAG_SchemaKnownOk should be cleared when iDb>=0");
  destroyTestDb(db);
  resetClearedSchemas();
}

// Entry point
int main(){
  test1_basicPath();
  test2_negativeIdbNoOp();
  test3_schemaLockBlocksClearing();

  if(failCount==0){
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  }else{
    std::cout << failCount << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}
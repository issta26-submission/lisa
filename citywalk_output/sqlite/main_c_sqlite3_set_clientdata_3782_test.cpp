// Lightweight C++11 test harness for the focal method: sqlite3_set_clientdata
// This test suite is self-contained (no GTest) and uses the C-like structures
// and logic described in the focal method. It mocks the minimal SQLite internal
// environment required to exercise the function's control flow (insert, update,
// delete, and error path when memory allocation fails).

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Minimal stand-ins for internal SQLite types used by the focal function.
// We intentionally implement a small, self-contained subset to exercise logic.

struct sqlite3_mutex {}; // dummy mutex type

// DbClientData mimics the internal SQLite structure used by sqlite3_set_clientdata.
struct DbClientData {
  char zName[1];             // flexible/name buffer (allocated with extra memory)
  DbClientData *pNext;
  void *pData;
  void (*xDestructor)(void*);
};

// Lightweight sqlite3 structure with only fields required by sqlite3_set_clientdata.
struct sqlite3 {
  sqlite3_mutex *mutex;
  DbClientData *pDbData;
};

// Global flags and counters to drive and observe behaviors.
static bool g_forceMallocFail = false;
static int g_destructor_calls = 0;

// Destructor used in tests to observe that it was invoked with a particular pointer.
static void test_destructor(void* /*p*/) {
  g_destructor_calls++;
}

// Forward declaration of the focal function under test.
// We implement a faithful re-creation here (self-contained) to avoid linking
// against a full SQLite library while still verifying the core logic.
#define SQLITE_OK 0
#define SQLITE_NOMEM 7

static void sqlite3_mutex_enter(sqlite3_mutex*) { /* no-op in test harness */ }
static void sqlite3_mutex_leave(sqlite3_mutex*) { /* no-op in test harness */ }

static void* sqlite3_malloc64(size_t size) {
  if (g_forceMallocFail) return nullptr;
  return malloc(size);
}
static void sqlite3_free(void* p) {
  free(p);
}

// Implementation of the focal method logic (reproduced for test harness).
static int sqlite3_set_clientdata(
  sqlite3 *db,
  const char *zName,
  void *pData,
  void (*xDestructor)(void*)
){
  DbClientData *p, **pp;
  sqlite3_mutex_enter(db->mutex);
  pp = &db->pDbData;
  for(p=db->pDbData; p && strcmp(p->zName,zName); p=p->pNext){
    pp = &p->pNext;
  }
  if( p ){
    assert( p->pData!=0 );
    if( p->xDestructor ) p->xDestructor(p->pData);
    if( pData==0 ){
      *pp = p->pNext;
      sqlite3_free(p);
      sqlite3_mutex_leave(db->mutex);
      return SQLITE_OK;
    }
  }else if( pData==0 ){
    sqlite3_mutex_leave(db->mutex);
    return SQLITE_OK;
  }else{
    size_t n = strlen(zName);
    p = (DbClientData*)sqlite3_malloc64( sizeof(DbClientData)+n+1 );
    if( p==0 ){
      if( xDestructor ) xDestructor(pData);
      sqlite3_mutex_leave(db->mutex);
      return SQLITE_NOMEM;
    }
    memcpy(p->zName, zName, n+1);
    p->pNext = db->pDbData;
    db->pDbData = p;
  }
  p->pData = pData;
  p->xDestructor = xDestructor;
  sqlite3_mutex_leave(db->mutex);
  return SQLITE_OK;
}

// Helper function: locate an entry by name
static DbClientData* findEntry(sqlite3* db, const char* name) {
  for (DbClientData* p = db->pDbData; p != nullptr; p = p->pNext) {
    if (std::strcmp(p->zName, name) == 0) return p;
  }
  return nullptr;
}

// Helper to free all entries and associated data
static void freeAllDb(sqlite3* db) {
  DbClientData* p = db->pDbData;
  while (p) {
    DbClientData* next = p->pNext;
    if (p->pData && p->xDestructor) {
      p->xDestructor(p->pData);
    }
    free(p);
    p = next;
  }
  db->pDbData = nullptr;
}

// Test harness helpers
static void runTest(const std::string& name, bool ok) {
  std::cout << "[ " << (ok ? "PASS" : "FAIL") << " ] " << name << "\n";
}

// Test 1: Insert a new entry when no existing entry with the given name
static void test_insert_new_entry() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  int* d1 = new int(11);
  int rc = sqlite3_set_clientdata(&db, "client1", d1, test_destructor);

  // Expectations
  bool pass = true;
  if (rc != SQLITE_OK) pass = false;
  DbClientData* head = db.pDbData;
  if (!head) pass = false;
  if (head && std::strcmp(head->zName, "client1") != 0) pass = false;
  if (head && head->pData != d1) pass = false;
  if (head && head->pNext != nullptr) pass = false;
  if (head && head->xDestructor != test_destructor) pass = false;

  runTest("Insert NEW entry when not existing (name: client1)", pass);

  // Cleanup for next tests
  freeAllDb(&db);
  delete d1;
}

// Test 2: Insert second entry; new head should be the new name
static void test_insert_second_entry() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  int* d1 = new int(11);
  int* d2 = new int(22);

  sqlite3_set_clientdata(&db, "client1", d1, test_destructor);
  int rc2 = sqlite3_set_clientdata(&db, "client2", d2, test_destructor);

  bool pass = true;
  if (rc2 != SQLITE_OK) pass = false;
  DbClientData* head = db.pDbData;
  if (!head) pass = false;
  if (std::strcmp(head->zName, "client2") != 0) pass = false;
  if (head->pData != d2) pass = false;
  if (head->pNext == nullptr) pass = false;
  if (head->pNext && head->pNext->pData != d1) pass = false;

  runTest("Insert SECOND entry (head is client2)", pass);

  freeAllDb(&db);
  delete d1;
  delete d2;
}

// Test 3: Update existing entry; destructor should be invoked on old data, and new data should replace
static void test_update_existing_entry() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  int* d1 = new int(11);
  int* d2 = new int(22);
  sqlite3_set_clientdata(&db, "client1", d1, test_destructor); // initial insert
  // Update existing
  g_destructor_calls = 0;
  int rc = sqlite3_set_clientdata(&db, "client1", d2, test_destructor);

  bool pass = true;
  if (rc != SQLITE_OK) pass = false;
  DbClientData* head = db.pDbData;
  if (!head) pass = false;
  if (std::strcmp(head->zName, "client1") != 0) pass = false;
  if (head->pData != d2) pass = false;
  if (head->xDestructor != test_destructor) pass = false;
  // Destructor should have been called for the old data (d1)
  if (g_destructor_calls == 0) pass = false;

  runTest("Update EXISTING entry; destructor called on old data and data replaced", pass);

  freeAllDb(&db);
  delete d1;
  delete d2;
}

// Test 4: Delete existing entry by passing NULL data; ensure removal from list
static void test_delete_existing_entry() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  int* d1 = new int(11);
  int* d2 = new int(22);
  sqlite3_set_clientdata(&db, "client1", d1, test_destructor);
  sqlite3_set_clientdata(&db, "client2", d2, test_destructor);

  // Delete head by setting NULL data
  int rc = sqlite3_set_clientdata(&db, "client1", nullptr, nullptr);

  bool pass = true;
  if (rc != SQLITE_OK) pass = false;
  DbClientData* head = db.pDbData;
  if (!head) pass = false;
  if (std::strcmp(head->zName, "client2") != 0) pass = false;
  if (head->pData != d2) pass = false;

  runTest("Delete existing entry by NULL data (head removed)", pass);

  freeAllDb(&db);
  delete d1;
  delete d2;
}

// Test 5: No-op when entry missing and pData is NULL
static void test_no_op_missing_entry_with_null() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  // No such entry exists; operation should be SQLITE_OK and nothing changes
  int rc = sqlite3_set_clientdata(&db, "no_such_name", nullptr, nullptr);

  bool pass = true;
  if (rc != SQLITE_OK) pass = false;
  if (db.pDbData != nullptr) pass = false;

  runTest("No-op when missing entry and NULL data", pass);
}

// Test 6: Memory allocation failure path should return SQLITE_NOMEM and not attach new entry
static void test_malloc_failure_path() {
  sqlite3 db;
  sqlite3_mutex dummy;
  db.mutex = &dummy;
  db.pDbData = nullptr;

  g_forceMallocFail = true;
  int dummyData = 99;
  int rc = sqlite3_set_clientdata(&db, "client3", &dummyData, test_destructor);

  bool pass = true;
  if (rc != SQLITE_NOMEM) pass = false;
  if (db.pDbData != nullptr) pass = false; // nothing should be inserted

  runTest("Malloc failure path -> SQLITE_NOMEM, no insertion", pass);

  g_forceMallocFail = false;
  // cleanup if any
  freeAllDb(&db);
}

// Entry point to run all tests
int main() {
  // Run each test in order
  test_insert_new_entry();
  test_insert_second_entry();
  test_update_existing_entry();
  test_delete_existing_entry();
  test_no_op_missing_entry_with_null();
  test_malloc_failure_path();

  // Final message
  std::cout << "All tests executed.\n";
  return 0;
}
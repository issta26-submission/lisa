// Test suite for sqlite3_txn_state focusing on core logic and dependencies.
// This test is designed to be self-contained (no external test framework).
// It provides minimal stubs for SQLite internals and exercises multiple code paths.
// Build: g++ -std=c++11 -DSQLITE_ENABLE_API_ARMOR test_sqlite_txn_state.cpp -o test_sqlite_txn_state
//
// Notes:
// - The function under test is included here with identical logic to the focal method.
// - A lightweight in-process test harness is used with non-terminating assertions.
// - Armor path (SQLITE_ENABLE_API_ARMOR) is exercised by toggling a global safety flag.
// - The tests cover: multiple databases, null/non-null Btree pointers, and zSchema handling.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Forward declarations for C-linkage of the focal function.
extern "C" int sqlite3_txn_state(struct sqlite3 *db, const char *zSchema);

// Domain: minimal SQLite-like types to support sqlite3_txn_state without a full SQLite build.

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare internal Btree type used by sqlite3_txn_state
typedef struct Btree Btree;

// Minimal sqlite3_mutex type (no actual threading in tests)
typedef struct sqlite3_mutex sqlite3_mutex;

// Define a minimal sqlite3 structure compatible with the focal function usage
typedef struct sqlite3 {
  sqlite3_mutex *mutex;
  int nDb;
  struct DbEntry {
    Btree *pBt;
    const char *zDbName;
  } *aDb;
} sqlite3;

// Define a minimal Btree with a txn state
struct Btree {
  int txnState;
};

// Constants used by the focal function
enum {
  SQLITE_TXN_NONE = 0,
  SQLITE_TXN_READ = 1,
  SQLITE_TXN_WRITE = 2
};

// Stubbed mutex enter/leave (no real threading in unit tests)
void sqlite3_mutex_enter(sqlite3_mutex *m) {
  (void)m;
}
void sqlite3_mutex_leave(sqlite3_mutex *m) {
  (void)m;
}

// sqlite3FindDbName: simple in-test lookup by db name
int sqlite3FindDbName(sqlite3 *db, const char *zDbName) {
  if (!db || !db->aDb) return -1;
  for (int i = 0; i < db->nDb; ++i) {
    if (db->aDb[i].zDbName && zDbName && std::strcmp(db->aDb[i].zDbName, zDbName) == 0) {
      return i;
    }
  }
  return -1;
}

// sqlite3BtreeTxnState: return the txn state of a Btree or SQLITE_TXN_NONE
int sqlite3BtreeTxnState(Btree *pBt) {
  return pBt ? pBt->txnState : SQLITE_TXN_NONE;
}

// Armor protection toggle (only compiled when SQLITE_ENABLE_API_ARMOR is defined)
#ifdef SQLITE_ENABLE_API_ARMOR
static int safetyMode = 1; // 1 means OK, 0 means not OK

// sqlite3SafetyCheckOk: mimic safety check in API armor mode
int sqlite3SafetyCheckOk(sqlite3 *db) {
  (void)db;
  return safetyMode;
}
#endif

#ifdef __cplusplus
}
#endif

// Focal method copied (identical logic) for testing purposes.
// We rely on the minimal definitions above to exercise the logic.
int sqlite3_txn_state(sqlite3 *db, const char *zSchema){
  int iDb, nDb;
  int iTxn = -1;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return -1;
  }
#endif
  sqlite3_mutex_enter(db->mutex);
  if( zSchema ){
    nDb = iDb = sqlite3FindDbName(db, zSchema);
    if( iDb<0 ) nDb--;
  }else{
    iDb = 0;
    nDb = db->nDb-1;
  }
  for(; iDb<=nDb; iDb++){
    Btree *pBt = db->aDb[iDb].pBt;
    int x = pBt!=0 ? sqlite3BtreeTxnState(pBt) : SQLITE_TXN_NONE;
    if( x>iTxn ) iTxn = x;
  }
  sqlite3_mutex_leave(db->mutex);
  return iTxn;
}

// -------- TEST HARNESS --------

// Lightweight success/failure counters and non-terminating assertion macro
static int TestsRun = 0;
static int TestsFailed = 0;

#define CHECK(cond, msg) \
  do { \
    ++TestsRun; \
    if(!(cond)) { \
      ++TestsFailed; \
      std::cout << "Test " << TestsRun << " FAILED: " << (msg) << std::endl; \
    } else { \
      std::cout << "Test " << TestsRun << " PASSED: " << (msg) << std::endl; \
    } \
  } while(0)

// Helpers to create/destroy a test sqlite3 db instance
struct DbEnvironment {
  sqlite3 *db;
  // helper: allocate nDb entries with optional names
  DbEnvironment(int n, const char** names, int* pBtStates) : db(nullptr) {
    db = (sqlite3*)std::malloc(sizeof(sqlite3));
    db->mutex = (sqlite3_mutex*)std::malloc(sizeof(sqlite3_mutex)); // no-op
    db->nDb = n;
    db->aDb = (struct sqlite3::DbEntry*)std::calloc(n, sizeof(struct sqlite3::DbEntry));
    for(int i=0;i<n;i++){
      db->aDb[i].pBt = nullptr;
      if(pBtStates && pBtStates[i] != -1){
        db->aDb[i].pBt = (Btree*)std::malloc(sizeof(Btree));
        db->aDb[i].pBt->txnState = pBtStates[i];
      }
      if(names){
        db->aDb[i].zDbName = names[i];
      }else{
        db->aDb[i].zDbName = nullptr;
      }
    }
  }
  ~DbEnvironment(){
    if(db){
      if(db->aDb){
        for(int i=0;i<db->nDb;i++){
          if(db->aDb[i].pBt){
            std::free(db->aDb[i].pBt);
          }
        }
        std::free(db->aDb);
      }
      if(db->mutex){
        std::free(db->mutex);
      }
      std::free(db);
    }
  }
};

// Test 1: Basic behavior with all pBt = nullptr and zSchema = nullptr
// Expected: returns SQLITE_TXN_NONE (0) for a single database
void test_basic_null_bt_no_schema(){
  const int n = 1;
  const char* names[] = { nullptr };
  DbEnvironment env(n, names, nullptr);
  int res = sqlite3_txn_state(env.db, nullptr);
  CHECK(res == SQLITE_TXN_NONE, "basic: single db with no Btree should be SQLITE_TXN_NONE (0)");
}

// Test 2: Single Btree with txn state READ
void test_single_bt_read_state(){
  const int n = 1;
  const char* names[] = { nullptr };
  int states[] = { SQLITE_TXN_READ };
  DbEnvironment env(n, names, states);
  int res = sqlite3_txn_state(env.db, nullptr);
  CHECK(res == SQLITE_TXN_READ, "single db with txn state READ should return 1");
}

// Test 3: Multiple DBs with max txn state across Btrees
void test_multiple_dbs_max_txn(){
  const int n = 3;
  const char* names[] = { nullptr, nullptr, nullptr };
  // states: db0=0 (NONE), db1=null (NONE), db2=SQLITE_TXN_WRITE
  int states[] = { SQLITE_TXN_NONE, -1, SQLITE_TXN_WRITE }; // -1 means no Btree
  DbEnvironment env(n, names, states);
  int res = sqlite3_txn_state(env.db, nullptr);
  // Expect max of [NONE, NONE, WRITE] -> WRITE (2)
  CHECK(res == SQLITE_TXN_WRITE, "multiple dbs: max of (0,0,2) should be 2 (WRITE)");
}

// Test 4: zSchema path selects specific DB and uses its Btree state
void test_schema_path_selection(){
  const int n = 3;
  // Name the databases: "main", "schemaX", "schemaY"
  const char* names[] = { "main", "schemaX", "schemaY" };
  // Btrees: only index 1 (schemaX) has a Btree with txnState = READ (1)
  int states[] = { -1, SQLITE_TXN_READ, -1 };
  DbEnvironment env(n, names, states);
  // Request state for "schemaX" (index 1)
  int res = sqlite3_txn_state(env.db, "schemaX");
  CHECK(res == SQLITE_TXN_READ, "schema path should return the txn state of the specified DB (READ)");
}

// Test 5: Armor path: when safety check fails, returns -1; when it passes, behaves normally
void test_armor_path_behavior(){
  // Build a 1-db environment, with a Btree txn state = WRITE
  const int n = 1;
  const char* names[] = { nullptr };
  int states[] = { SQLITE_TXN_WRITE };
  DbEnvironment env(n, names, states);
  // First, simulate safety check failure
  // safetyMode global controls sqlite3SafetyCheckOk
#ifdef SQLITE_ENABLE_API_ARMOR
  // Set to fail
  extern int safetyMode;
  safetyMode = 0;
  int resFail = sqlite3_txn_state(env.db, nullptr);
  CHECK(resFail == -1, "armor path: safety check fail should return -1");
  // Now simulate success and ensure normal behavior
  safetyMode = 1;
  int resPass = sqlite3_txn_state(env.db, nullptr);
  CHECK(resPass == SQLITE_TXN_WRITE, "armor path: after passing safety check, should return max txn state (WRITE)");
#else
  // If armor is not compiled in, skip
  (void)env; (void)names;
  std::cout << "Armor path not compiled in (define SQLITE_ENABLE_API_ARMOR to enable)." << std::endl;
#endif
}

// Entry point
int main(){
  // Run tests. Each test prints its own result via CHECK macro.
  test_basic_null_bt_no_schema();
  test_single_bt_read_state();
  test_multiple_dbs_max_txn();
  test_schema_path_selection();
  test_armor_path_behavior();

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Tests run: " << TestsRun << ", Failures: " << TestsFailed << std::endl;
  return TestsFailed ? 1 : 0;
}

// Note on static/private members and naming:
// - All helper functions/types used by sqlite3_txn_state are provided in this test file.
// - There are no private members to access; tests interact with the exposed API surface.
// - Since the test is self-contained, static functions within this file are not required to mock externally.
// - The test harness uses a straightforward, non-terminating assertion approach (CHECK macro).
// - To exercise the armor path fully, compile with -DSQLITE_ENABLE_API_ARMOR and ensure safetyMode is controllable.
//
// Explanation of test intentions per case (summarized):
// - Test basic_null_bt_no_schema: ensures default behavior with a single DB and no Btree yields SQLITE_TXN_NONE.
// - Test_single_bt_read_state: verifies that a single Btree with READ state yields 1.
// - Test_multiple_dbs_max_txn: confirms that the function reports the maximum txn state across multiple databases.
// - Test_schema_path_selection: validates that providing a zSchema that exists uses the correct DB index's state.
// - Test_armor_path_behavior: checks behavior when the API armor guard is enabled; fails gracefully when safety check fails and continues when it passes.
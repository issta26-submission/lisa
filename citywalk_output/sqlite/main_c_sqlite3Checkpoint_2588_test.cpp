// C++11 Unit Test Suite for sqlite3Checkpoint (focal method)
 // This test harness re-creates the minimal, self-contained environment
 // to exercise the logic of sqlite3Checkpoint without requiring the full
 // SQLite project. It mocks necessary dependencies and validates
 // different execution paths (OK, BUSY, ERROR) as exercised by the
 // original implementation.

#include <vector>
#include <cstdio>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-specific constants (mocked for unit tests)
const int SQLITE_OK    = 0;
const int SQLITE_BUSY  = 1;
const int SQLITE_ERROR = 2;

const int SQLITE_MAX_DB      = 5;  // Distinct from SQLITE_MAX_ATTACHED
const int SQLITE_MAX_ATTACHED = 20;

// Forward declare the minimal structures used by sqlite3Checkpoint
struct Btree {
    int mode; // 0 = OK, 1 = BUSY, 2 = ERROR (simulated per-DB behavior)
};

struct DbEntry {
    Btree* pBt;
};

struct sqlite3_mutex {
    int dummy;
};

struct sqlite3 {
    int nDb;
    DbEntry* aDb;          // Array of databases (main + attached)
    sqlite3_mutex* mutex;    // Mutex (lock) owned by db
};

// Macro used in the focal method to mark test points. In this test harness,
// it's defined as a no-op to keep tests non-intrusive.
#define testcase(x) do { (void)(x); } while(0)

// Stub: simulate sqlite3_mutex_held(db->mutex) always returning true.
static int sqlite3_mutex_held(void* mutex) {
    (void)mutex;
    return 1;
}

// Forward declaration of the function under test (sqlite3Checkpoint)
int sqlite3Checkpoint(sqlite3 *db, int iDb, int eMode, int *pnLog, int *pnCkpt);

// Mock: simulate the B-Tree checkpoint operation for a single Btree.
// Behavior is controlled by Btree::mode:
//   0 -> SQLITE_OK
//   1 -> SQLITE_BUSY
//   2 -> SQLITE_ERROR
static int sqlite3BtreeCheckpoint(void* pBt, int /*eMode*/, int* /*pnLog*/, int* /*pnCkpt*/) {
    Btree* b = static_cast<Btree*>(pBt);
    switch (b->mode) {
        case 0: return SQLITE_OK;
        case 1: return SQLITE_BUSY;
        case 2: return SQLITE_ERROR;
        default: return SQLITE_ERROR;
    }
}

// Minimal implementation of sqlite3Checkpoint copied from the focal method
int sqlite3Checkpoint(sqlite3 *db, int iDb, int eMode, int *pnLog, int *pnCkpt) {
  int rc = SQLITE_OK;             /* Return code */
  int i;                          /* Used to iterate through attached dbs */
  int bBusy = 0;                  /* True if SQLITE_BUSY has been encountered */
  assert( sqlite3_mutex_held(db->mutex) );
  assert( !pnLog || *pnLog==-1 );
  assert( !pnCkpt || *pnCkpt==-1 );
  testcase( iDb==SQLITE_MAX_ATTACHED ); /* See forum post a006d86f72 */
  testcase( iDb==SQLITE_MAX_DB );
  for(i=0; i<db->nDb && rc==SQLITE_OK; i++){
    if( i==iDb || iDb==SQLITE_MAX_DB ){
      rc = sqlite3BtreeCheckpoint(db->aDb[i].pBt, eMode, pnLog, pnCkpt);
      pnLog = 0;
      pnCkpt = 0;
      if( rc==SQLITE_BUSY ){
        bBusy = 1;
        rc = SQLITE_OK;
      }
    }
  }
  return (rc==SQLITE_OK && bBusy) ? SQLITE_BUSY : rc;
}

// Helper utilities to construct and deallocate a mock sqlite3 database instance
struct TestDbHandle {
    sqlite3* db;
    ~TestDbHandle() { 
        // cleanup Btrees
        for (int i = 0; i < db->nDb; ++i) delete db->aDb[i].pBt;
        delete[] db->aDb;
        delete db->mutex; // the dummy mutex
        delete db;
    }
};

// Create a mock sqlite3 instance with nDb databases, each Btree configured with mode
static TestDbHandle makeDb(int nDb, const std::vector<int>& modes) {
    TestDbHandle handle;
    handle.db = new sqlite3;
    handle.db->nDb = nDb;
    handle.db->aDb = new DbEntry[nDb];
    for (int i = 0; i < nDb; ++i) {
        Btree* bt = new Btree{ (i < (int)modes.size()) ? modes[i] : 0 };
        handle.db->aDb[i].pBt = bt;
    }
    // Dummy mutex object (no real locking in tests)
    handle.db->mutex = new sqlite3_mutex{0};
    return handle;
}

// Simple assertion-like checker that reports non-terminating failures
static bool expect_eq(int a, int b, const std::string& msg) {
    if (a != b) {
        std::cerr << "EXPECT_EQ FAILED: " << msg << " | got " << a << ", expected " << b << std::endl;
        return false;
    }
    return true;
}

// Run a single test case and return true if it passes
static bool runTestCase(const std::string& testName,
                        int nDb,
                        const std::vector<int>& modes,
                        int iDb,
                        int expectedRc)
{
    TestDbHandle h = makeDb(nDb, modes);
    int logVal = -1;
    int ckptVal = -1;
    int rc = sqlite3Checkpoint(h.db, iDb, 0, &logVal, &ckptVal);

    bool pass = expect_eq(rc, expectedRc, testName);
    // Optional: print diagnostic data
    std::cout << "Test: " << testName
              << " | rc=" << rc
              << " | logVal=" << logVal
              << " | ckptVal=" << ckptVal
              << std::endl;
    // Cleanup happens via TestDbHandle destructor
    return pass;
}

int main() {
    using std::vector;
    using std::string;

    int total = 0;
    int passed = 0;

    // Test 1: Basic success path. One DB checkpointed (iDb = 0), mode 0 (OK)
    // Expected: SQLITE_OK
    {
        const string t = "Basic_OK_SingleDB";

        bool r = runTestCase(t,
                             3,               // nDb
                             {0, 0, 0},       // modes: all OK
                             0,                 // iDb selects first DB
                             SQLITE_OK);
        total++; if (r) passed++;
    }

    // Test 2: BUSY encountered in one DB; should return SQLITE_BUSY after processing
    // iDb = 1; aDb[1] -> BUSY
    {
        const string t = "Busy_ON_SingleDB";

        bool r = runTestCase(t,
                             3,
                             {0, 1, 0},    // middle DB BUSY
                             1,              // iDb selects second DB
                             SQLITE_BUSY);
        total++; if (r) passed++;
    }

    // Test 3: SQLITE_ERROR propagated when one DB reports ERROR
    // iDb = 1; aDb[1] -> ERROR
    {
        const string t = "Error_ON_SingleDB";

        bool r = runTestCase(t,
                             3,
                             {0, 2, 0},    // middle DB ERROR
                             1,              // iDb selects second DB
                             SQLITE_ERROR);
        total++; if (r) passed++;
    }

    // Test 4: iDb == SQLITE_MAX_DB condition triggers all DBs checkpointed.
    // Set one DB to BUSY to ensure final result is SQLITE_BUSY
    {
        const string t = "AllDBs_MaxDB_Busy";

        bool r = runTestCase(t,
                             3,
                             {0, 1, 0},    // one BUSY among many
                             SQLITE_MAX_DB,  // triggers all DBs due to iDb==MAX_DB
                             SQLITE_BUSY);
        total++; if (r) passed++;
    }

    // Test 5: iDb == SQLITE_MAX_ATTACHED should not trigger any checkpointing (no effect)
    // Expected: SQLITE_OK since no DB is iterated
    {
        const string t = "MaxAttached_NoCall";

        bool r = runTestCase(t,
                             3,
                             {0, 0, 0},      // modes unused since no call
                             SQLITE_MAX_ATTACHED, // no DB should be checked
                             SQLITE_OK);
        total++; if (r) passed++;
    }

    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
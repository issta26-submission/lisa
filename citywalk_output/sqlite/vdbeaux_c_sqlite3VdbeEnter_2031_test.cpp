// Minimal C++11 test harness for sqlite3VdbeEnter (focal method) with lightweight stubs.
// This test file provides a self-contained environment to validate the behavior of
// sqlite3VdbeEnter by simulating the minimal SQLite structures used by the function.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <vector>


// Domain: lightweight stubs to mirror the dependencies used by sqlite3VdbeEnter

// Forward declarations to avoid circular dependencies in the simplified test
struct Btree;
struct Db;
struct sqlite3;
struct Vdbe;

// Lightweight Btree structure with an id for identification
struct Btree {
    int id;
};

// Db wrapper with a pointer to a Btree (pBt) as in the real sqlite3 struct
struct Db {
    Btree *pBt;
};

// sqlite3 structure with an array of Db (aDb) and count (nDb)
struct sqlite3 {
    Db *aDb;
    int nDb;
};

// Vdbe (the focal parameter) containing a pointer to the database and a lockMask field
struct Vdbe {
    sqlite3 *db;
    unsigned int lockMask;
};

// Global log to capture which Btrees get entered (via sqlite3BtreeEnter)
static std::vector<int> g_entered_btree_ids;

// Logging function called by sqlite3VdbeEnter when a Btree is entered
static void sqlite3BtreeEnter(Btree *pBt) {
    if (pBt) {
        g_entered_btree_ids.push_back(pBt->id);
    }
}

// Macros and helpers to emulate the original code predicates
#define DbMaskAllZero(mask) ((mask) == 0)
#define DbMaskTest(mask,i) (((mask) & (1u << (i))) != 0)
#define ALWAYS(x) ((x) ? 1 : 0)

// The focal method under test (reproduced here for a self-contained TU)
void sqlite3VdbeEnter(Vdbe *p){
  int i;
  sqlite3 *db;
  Db *aDb;
  int nDb;
  if( DbMaskAllZero(p->lockMask) ) return;  /* The common case */
  db = p->db;
  aDb = db->aDb;
  nDb = db->nDb;
  for(i=0; i<nDb; i++){
    if( i!=1 && DbMaskTest(p->lockMask,i) && ALWAYS(aDb[i].pBt!=0) ){
      sqlite3BtreeEnter(aDb[i].pBt);
    }
  }
}

// Simple test harness
static int gTestsRun = 0;
static int gTestsFailed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b, msg) do { \
    if((int)(a) != (int)(b)) { \
        std::cerr << "EXPECT_EQ_INT failed: " << (msg) \
                  << " | " << (a) << " != " << (b) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)

static void reset_log() { g_entered_btree_ids.clear(); }

// Test 1: Common case - lockMask zero -> no BtreeEnter calls
static bool test_common_case_no_calls() {
    reset_log();

    // Prepare Btrees
    Btree b0{0}, b1{1}, b2{2};
    Db aDb[3];
    aDb[0].pBt = &b0;
    aDb[1].pBt = &b1;
    aDb[2].pBt = &b2;

    sqlite3 db;
    db.aDb = aDb;
    db.nDb = 3;

    Vdbe v;
    v.db = &db;
    v.lockMask = 0; // common/fast path

    sqlite3VdbeEnter(&v);

    // Expect no entries
    EXPECT_TRUE(g_entered_btree_ids.empty(), "no BtreeEnter should be called when lockMask is zero");

    return true;
}

// Test 2: Some indices should enter BTrees (except i == 1 and i with pBt == 0)
static bool test_enter_calls_excluding_one_and_null_btree() {
    reset_log();

    // Prepare Btrees
    Btree b0{0}, b1{1}, b2{2};
    Db aDb[3];
    aDb[0].pBt = &b0;  // i = 0 eligible (not 1)
    aDb[1].pBt = &b1;  // i = 1 should be skipped by the code
    aDb[2].pBt = &b2;  // i = 2 eligible (not 1)

    sqlite3 db;
    db.aDb = aDb;
    db.nDb = 3;

    Vdbe v;
    v.db = &db;
    // Set bits 0 and 2 (i=0 and i=2)
    v.lockMask = (1u<<0) | (1u<<2);

    sqlite3VdbeEnter(&v);

    // Expect two enters for i=0 and i=2
    EXPECT_EQ_INT((int)g_entered_btree_ids.size(), 2, "should enter 2 BTrees (i=0 and i=2)");
    // Validate the order/identity of BTrees entered
    EXPECT_EQ_INT(g_entered_btree_ids[0], b0.id, "first entered Btree should have id 0 (i=0)");
    EXPECT_EQ_INT(g_entered_btree_ids[1], b2.id, "second entered Btree should have id 2 (i=2)");

    return true;
}

// Test 3: i with pBt == 0 should not cause entry even if mask bit is set
static bool test_skip_null_btree_entries() {
    reset_log();

    // Prepare Btrees, but set i=0 to have null pBt
    Btree b1{1}, b2{2};
    Db aDb[3];
    aDb[0].pBt = 0;       // i = 0 has no BT tree
    aDb[1].pBt = &b1;     // i = 1 (skipped regardless of mask)
    aDb[2].pBt = &b2;     // i = 2

    sqlite3 db;
    db.aDb = aDb;
    db.nDb = 3;

    Vdbe v;
    v.db = &db;
    // Set bits 0 and 2; i=0 should not trigger because pBt == 0
    v.lockMask = (1u<<0) | (1u<<2);

    sqlite3VdbeEnter(&v);

    // Expect only 1 enter for i=2
    EXPECT_EQ_INT((int)g_entered_btree_ids.size(), 1, "should enter only 1 Btree (i=2)");
    EXPECT_EQ_INT(g_entered_btree_ids[0], b2.id, "entered Btree should have id 2 (i=2)");

    return true;
}

// Test 4: No iterations when nDb = 0
static bool test_zero_nDb_no_iterations() {
    reset_log();

    Btree b0{0};
    Db aDb[1];
    aDb[0].pBt = &b0;

    sqlite3 db;
    db.aDb = aDb;
    db.nDb = 0; // no iterations

    Vdbe v;
    v.db = &db;
    v.lockMask = (1u<<0); // non-zero but no iterations

    sqlite3VdbeEnter(&v);

    EXPECT_TRUE(g_entered_btree_ids.empty(), "no BTrees should be entered when nDb == 0");

    return true;
}

// Main to run tests
int main() {
    std::cout << "Starting sqlite3VdbeEnter unit tests (self-contained)..." << std::endl;

    gTestsRun = 0;
    gTestsFailed = 0;

    // Run tests
    gTestsRun++;
    if (test_common_case_no_calls()) {
        std::cout << "[PASS] test_common_case_no_calls" << std::endl;
    } else {
        std::cout << "[FAIL] test_common_case_no_calls" << std::endl;
        gTestsFailed++;
    }

    gTestsRun++;
    if (test_enter_calls_excluding_one_and_null_btree()) {
        std::cout << "[PASS] test_enter_calls_excluding_one_and_null_btree" << std::endl;
    } else {
        std::cout << "[FAIL] test_enter_calls_excluding_one_and_null_btree" << std::endl;
        gTestsFailed++;
    }

    gTestsRun++;
    if (test_skip_null_btree_entries()) {
        std::cout << "[PASS] test_skip_null_btree_entries" << std::endl;
    } else {
        std::cout << "[FAIL] test_skip_null_btree_entries" << std::endl;
        gTestsFailed++;
    }

    gTestsRun++;
    if (test_zero_nDb_no_iterations()) {
        std::cout << "[PASS] test_zero_nDb_no_iterations" << std::endl;
    } else {
        std::cout << "[FAIL] test_zero_nDb_no_iterations" << std::endl;
        gTestsFailed++;
    }

    std::cout << "Tests run: " << gTestsRun << ", Passed: " << (gTestsRun - gTestsFailed)
              << ", Failed: " << gTestsFailed << std::endl;

    return gTestsFailed ? 1 : 0;
}
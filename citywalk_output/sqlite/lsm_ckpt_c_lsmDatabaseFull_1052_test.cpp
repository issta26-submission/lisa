// C++11 test suite for the focal method lsmDatabaseFull in lsm_ckpt.c
// This test harness avoids GoogleTest and uses lightweight
// EXPECT-style macros that do not terminate on failure.
// It mocks out the external dependencies (lsmShmAssertLock) and
// constructs minimal in-process objects that mirror the project's
// lsm_db/Worker/Level structures to exercise lsmDatabaseFull.

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <vector>


// Bring in C declarations from the project's headers to ensure
// type compatibility with lsmDatabaseFull.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) \
  do { if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } } while(0)

#define EXPECT_EQ(a,b) \
  do { if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " << (a) << " != " << (b) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } } while(0)

#define TEST_PASSED() \
  do { if(g_test_failures == 0) std::cout << "PASS: " __FILE__ "\n"; } while(0)
#define TEST_FAILED() \
  do { if(g_test_failures != 0) std::cout << "FAIL: " __FILE__ " with " << g_test_failures << " failure(s)\n"; } while(0)

// ---- Mocking external dependencies ----

// The focal function uses: assert( lsmShmAssertLock(pDb, LSM_LOCK_WORKER, LSM_LOCK_EXCL) );
// We'll provide a controllable mock for lsmShmAssertLock.
// The real implementation is in the production code; this mock will
// override it in the test translation unit (C linkage).

static int g_lockReturn = 1; // default: true

extern "C" int lsmShmAssertLock(const lsm_db *pDb, int lock, int ex){
    // Simple mock: return value controlled by test logic.
    (void)pDb; (void)lock; (void)ex;
    return g_lockReturn;
}

// Helper to adjust mock behavior from tests
static void set_lock_return(int v){
    g_lockReturn = v;
}

// ---- Test Helpers: object construction/destruction ----

// The project defines the following shape (as seen in lsm_ckpt.c and FOCAL_CLASS_DEP):
// typedef struct Level Level;
// struct Level { int nRight; Level *pNext; };
// typedef struct Worker { Level *pLevel; } Worker;
// typedef struct lsm_db { Worker *pWorker; /* ... other members ... */ } lsm_db;

// The header lsmInt.h already provides compatible definitions. We will
// construct objects using those types to ensure correctness.

static void free_level_chain(Level *head){
    Level *p = head;
    while(p){
        Level *next = p->pNext;
        delete p;
        p = next;
    }
}

static Level* build_levels(int count, int default_nRight){
    Level *head = NULL;
    Level *tail = NULL;
    for(int i=0; i<count; ++i){
        Level *node = new Level();
        // Use provided nRight value; 0 should be interpreted as 1 by the production logic.
        node->nRight = default_nRight;
        node->pNext = NULL;
        if(!head){
            head = node;
            tail = node;
        }else{
            tail->pNext = node;
            tail = node;
        }
    }
    return head;
}

// ---- Tests ----

// Test 1: No levels (pWorker exists but pLevel is NULL)
// Expect lsmDatabaseFull to return false (0) since nRhs == 0.
static void test_lsmDatabaseFull_no_levels(){
    // Prepare db with a worker that has no levels
    lsm_db db;
    std::memset(&db, 0, sizeof(db)); // zero-initialize
    static Worker w;               // use a static to ensure address stays valid during the call
    w.pLevel = NULL;
    db.pWorker = &w;

    set_lock_return(1);              // ensure the lock check passes
    int res = lsmDatabaseFull(&db);

    // Verify: 0 RHS -> false
    EXPECT_EQ(res, 0);

    // No dynamic memory to free in this test
}

// Test 2: Threshold boundary behavior
// Build a chain with N = LSM_MAX_RHS_SEGMENTS - 1 levels, each contributing 1 RHS.
// Then append one more level to reach exactly LSM_MAX_RHS_SEGMENTS RHS.
// We verify false then true across the two calls.
static void test_lsmDatabaseFull_threshold_boundary(){
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    // Create a worker and a chain of levels
    static Worker w;
    db.pWorker = &w;
    Level *head = NULL;
    Level *tail = NULL;

    int threshold = LSM_MAX_RHS_SEGMENTS;

    // Create threshold-1 levels with default nRight = 0 (which equals 1 contributed RHS)
    int initial_levels = threshold - 1;
    for(int i=0; i<initial_levels; ++i){
        Level *node = new Level();
        node->nRight = 0; // contributes 1
        node->pNext = NULL;
        if(!head){
            head = node;
            tail = node;
        }else{
            tail->pNext = node;
            tail = node;
        }
    }
    w.pLevel = head;

    set_lock_return(1);
    int res_false = lsmDatabaseFull(&db);
    // Expect false because sum == threshold-1
    EXPECT_EQ(res_false, 0);

    // Append one more level to reach threshold
    Level *node = new Level();
    node->nRight = 0; // contributes 1
    node->pNext = NULL;
    tail->pNext = node;
    tail = node;

    int res_true = lsmDatabaseFull(&db);
    // Expect true because sum == threshold
    EXPECT_EQ(res_true, 1);

    // Cleanup
    // Free the chain
    Level *p = head;
    while(p){
        Level *next = p->pNext;
        delete p;
        p = next;
    }

    // Update db.pWorker for completeness (not strictly necessary after cleanup)
    w.pLevel = NULL;
}

// Test 3: Sanity with multiple levels including a mix of zero/non-zero nRight
// This test confirms that a non-trivial chain is evaluated without crashing.
// It does not rely on exact threshold value, using a small chain that should be well below most thresholds.
static void test_lsmDatabaseFull_mixed_values_sanity(){
    lsm_db db;
    std::memset(&db, 0, sizeof(db));

    static Worker w;
    db.pWorker = &w;
    // Build a small chain: [1, 0, 3]  -> contributes [1,1,3] => sum = 5
    Level *l1 = new Level(); l1->nRight = 1; l1->pNext = NULL;
    Level *l2 = new Level(); l2->nRight = 0; l2->pNext = NULL;
    Level *l3 = new Level(); l3->nRight = 3; l3->pNext = NULL;
    l1->pNext = l2; l2->pNext = l3;

    w.pLevel = l1;

    set_lock_return(1);
    int res = lsmDatabaseFull(&db);

    // Since threshold is unknown but likely > 3, and sum is 1+1+3 = 5, we check boolean type only
    // If threshold <= 5 this could be true; we only validate that the function executes and returns 0/1.
    EXPECT_TRUE(res == 0 || res == 1);

    // Cleanup
    delete l1;
    delete l2;
    delete l3;
}

// ---- Main runner ----

int main(){
    std::cout << "Running tests for lsmDatabaseFull (lsm_ckpt.c) under C++11 test harness\n";

    test_lsmDatabaseFull_no_levels();
    test_lsmDatabaseFull_threshold_boundary();
    test_lsmDatabaseFull_mixed_values_sanity();

    // Summary
    if(g_test_failures == 0){
        std::cout << "All tests PASSED.\n";
    }else{
        std::cout << g_test_failures << " test failure(s) detected.\n";
    }

    // Expose exit code: 0 for success, non-zero for failure
    return g_test_failures == 0 ? 0 : 1;
}
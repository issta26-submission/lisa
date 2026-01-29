/*
  Unit test suite for sqlite3BtreeEnter(Btree* p)
  - This test harness is written in C++11 without GoogleTest.
  - It relies on the real sqlite3BtreeEnter implementation available in btmutex.c.
  - We provide minimal stub definitions for the dependencies (Btree, sqlite3, sqlite3_mutex)
    that match the fields accessed by sqlite3BtreeEnter so the function can be exercised.
  - Tests cover:
    1) Early return when p->sharable is false (true asserts must pass under this path).
    2) Path where p->sharable is true and p->locked is false; ensures p->wantToLock increments.
    3) Path where p->sharable is true and p->locked is true; ensures p->wantToLock increments then short-circuits.
  - We use non-terminating expectations (EXPECT_*) to maximize coverage without aborting tests.
*/

#include <btreeInt.h>
#include <iostream>
#include <string>


// Lightweight test harness (non-terminating expectations)
static int g_total_expect = 0;
static int g_failed_expect = 0;

static void expect_true(bool cond, const std::string &msg) {
    ++g_total_expect;
    if (!cond) {
        ++g_failed_expect;
        std::cout << "[EXPECT_TRUE FAILED] " << msg << std::endl;
    }
}

static void expect_eq(int a, int b, const std::string &msg) {
    expect_true(a == b, msg + " (actual: " + std::to_string(a) + ", expected: " + std::to_string(b) + ")");
}

// Forward declaration of sqlite3BtreeEnter from the C library.
// We provide a C linkage since the library is C.
extern "C" void sqlite3BtreeEnter(struct Btree *p);

// Minimal type definitions matching usage in sqlite3BtreeEnter.
// These definitions are crafted to mirror the fields accessed by the focal method.
struct sqlite3_mutex {
    bool held;
};

struct sqlite3 {
    sqlite3_mutex *mutex;
};

struct Btree {
    Btree *pNext;
    Btree *pPrev;
    Btree *pBt;
    sqlite3 *db;
    int locked;
    int sharable;
    int wantToLock;
};

// Test helpers to construct a small in-test SQLite-like environment
namespace TestEnv {

static sqlite3* make_db(bool held) {
    sqlite3 *db = new sqlite3();
    db->mutex = new sqlite3_mutex();
    db->mutex->held = held;
    return db;
}

static void destroy_db(sqlite3* db) {
    if (db) {
        delete db->mutex;
        delete db;
    }
}

// Helper to reset a Btree pointer safely
static void reset_btree(Btree *p) {
    if (p) {
        p->pNext = nullptr;
        p->pPrev = nullptr;
        p->pBt = nullptr;
        p->db = nullptr;
        p->locked = 0;
        p->sharable = 0;
        p->wantToLock = 0;
    }
}

} // namespace TestEnv

// Test Case A: p->sharable == 0, with all related asserts satisfied. Expect early return with no change to p->wantToLock.
static void test_case_A_early_return_no_lock() {
    std::cout << "Running test_case_A_early_return_no_lock (sharable == false path)" << std::endl;

    using namespace TestEnv;

    // Setup database and mutex: mutex must be held to satisfy assert 8
    sqlite3 *db = make_db(true);

    // Setup Btree p such that asserts pass when sharable is false
    Btree p;
    reset_btree(&p);
    p.db = db;
    p.sharable = 0;     // false
    p.locked = 0;       // not locked
    p.wantToLock = 0;     // initial
    p.pNext = nullptr;
    p.pPrev = nullptr;

    // pBt must be non-null with db == p->db to satisfy assertion 9
    Btree pBt;
    pBt.db = db;
    p.pBt = &pBt;

    // Call function under test (extern "C" linkage)
    sqlite3BtreeEnter(&p);

    // Expectations
    expect_true(p.wantToLock == 0, "Case A: wantToLock should remain 0 on early return");
    // cleanup
    destroy_db(db);
    std::cout << "Finished test_case_A_early_return_no_lock" << std::endl;
}

// Test Case B: p->sharable == true, p->locked == false; ensure p->wantToLock increments to 1.
static void test_case_B_sharable_unlocked_increments_lock_want() {
    std::cout << "Running test_case_B_sharable_unlocked_increments_lock_want" << std::endl;

    using namespace TestEnv;

    // Setup database and mutex: must be held
    sqlite3 *db = make_db(true);

    // pBt helper with same db (assert 9 will be satisfied because p->sharable is true)
    Btree pBt;
    pBt.db = db;

    // Setup p
    Btree p;
    reset_btree(&p);
    p.db = db;
    p.pBt = &pBt;
    pNext_dummy:
    p.pNext = nullptr;
    p.pPrev = nullptr;

    p.sharable = 1; // true
    p.locked = 0;   // not locked
    p.wantToLock = 0;

    // Call function
    sqlite3BtreeEnter(&p);

    // Expect that we incremented wantToLock
    expect_true(p.wantToLock == 1, "Case B: after enter, wantToLock should be 1 when unlocked and sharable");
    destroy_db(db);
    std::cout << "Finished test_case_B_sharable_unlocked_increments_lock_want" << std::endl;
}

// Test Case C: p->sharable == true, p->locked == true; ensure p->wantToLock increments to 1 and returns early afterwards.
static void test_case_C_sharable_locked_path() {
    std::cout << "Running test_case_C_sharable_locked_path" << std::endl;

    using namespace TestEnv;

    // Setup database and mutex: must be held
    sqlite3 *db = make_db(true);

    Btree pBt;
    pBt.db = db;

    Btree p;
    reset_btree(&p);
    p.db = db;
    p.pBt = &pBt;
    p.pNext = nullptr;
    p.pPrev = nullptr;

    p.sharable = 1; // true
    p.locked = 1;   // locked -> should early return after increment
    p.wantToLock = 0;

    sqlite3BtreeEnter(&p);

    // expect that even though locked, we still increment wantToLock then return
    expect_true(p.wantToLock == 1, "Case C: when locked, still increment wantToLock before early return");
    destroy_db(db);
    std::cout << "Finished test_case_C_sharable_locked_path" << std::endl;
}

// Simple test runner
static void run_all_tests() {
    test_case_A_early_return_no_lock();
    test_case_B_sharable_unlocked_increments_lock_want();
    test_case_C_sharable_locked_path();

    std::cout << "Test suite finished. Total expectations: " << g_total_expect
              << ", Failures: " << g_failed_expect << std::endl;
}

// Main entry: run tests from program's main function (as required when gtest isn't allowed)
int main() {
    run_all_tests();
    return g_failed_expect != 0 ? 1 : 0;
}
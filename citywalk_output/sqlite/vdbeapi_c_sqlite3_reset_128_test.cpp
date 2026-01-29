// Minimal C++11 test harness for sqlite3_reset focusing on the focal logic.
// This file provides lightweight stubs for internal SQLite types and APIs
// to enable unit testing without the full SQLite build system.
// The tests avoid terminating assertions and report failures via custom macros.

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Lightweight test framework (non-terminating expectations).
static int g_failures = 0;
static void testAbort(const char* file, int line, const char* msg){
    std::cerr << "TEST ABORT: " << file << ":" << line << " - " << msg << std::endl;
    ++g_failures;
}
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { testAbort(__FILE__, __LINE__, "EXPECT_TRUE failed: " #cond); } } while(0)
#define EXPECT_EQ(a,b) \
    do { if( (a) != (b) ) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } } while(0)
#define EXPECT_NEQ(a,b) \
    do { if( (a) == (b) ) { \
        std::cerr << "EXPECT_NEQ failed: " #a " (" << (a) << ") == " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } } while(0)

// Domain-specific minimal stubs/types to support sqlite3_reset
#define SQLITE_OK 0

// Forward declarations to mimic the internal API surface used by sqlite3_reset
struct sqlite3_mutex;
struct sqlite3;
struct Vdbe;
struct sqlite3_stmt;

// Public-ish function we are testing (extern "C" to mimic C linkage)
extern "C" int sqlite3_reset(sqlite3_stmt *pStmt);

// Minimal internal state to drive test scenarios
static int g_next_vdbe_reset_rc = SQLITE_OK;
static int g_mutex_enters = 0;
static int g_mutex_leaves = 0;
static int g_profile_calls = 0;
static int g_vdbe_resets = 0;
static int g_vdbe_rewinds = 0;
static int g_api_exits = 0;

// Lightweight structures to simulate SQLite internal objects
struct sqlite3_mutex {
    int dummy;
};

struct sqlite3 {
    sqlite3_mutex* mutex;
    int errMask;
};

struct Vdbe {
    sqlite3* db; // back-reference to database object
};

struct sqlite3_stmt {
    int dummy; // opaque from the outside
};

// Global per-test control of behavior
static void setNextVdbeResetRc(int rc){ g_next_vdbe_reset_rc = rc; }

// Function hooks used by sqlite3_reset (implemented as simple stubs)
static void checkProfileCallback(sqlite3* db, Vdbe* /*p*/){
    // Track that the profiling callback would have been invoked
    ++g_profile_calls;
}

static int sqlite3VdbeReset(Vdbe* /*p*/){
    ++g_vdbe_resets;
    return g_next_vdbe_reset_rc;
}

static void sqlite3VdbeRewind(Vdbe* /*p*/){
    ++g_vdbe_rewinds;
}

static int sqlite3ApiExit(sqlite3* /*db*/, int rc){
    ++g_api_exits;
    return rc;
}

// Mutex handling stubs
static void sqlite3_mutex_enter(sqlite3_mutex* /*mutex*/){
    ++g_mutex_enters;
}
static void sqlite3_mutex_leave(sqlite3_mutex* /*mutex*/){
    ++g_mutex_leaves;
}

// The focal method under test, adapted to link with our lightweight stubs.
// We provide C linkage to resemble the original C function.
extern "C" int sqlite3_reset(sqlite3_stmt *pStmt){
  int rc;
  if( pStmt==0 ){
    rc = SQLITE_OK;
  }else{
    Vdbe *v = (Vdbe*)pStmt;
    sqlite3 *db = v->db;
    sqlite3_mutex_enter(db->mutex);
    checkProfileCallback(db, v);
    rc = sqlite3VdbeReset(v);
    sqlite3VdbeRewind(v);
    assert( (rc & (db->errMask))==rc );
    rc = sqlite3ApiExit(db, rc);
    sqlite3_mutex_leave(db->mutex);
  }
  return rc;
}

// Tests for sqlite3_reset
// Test 1: pStmt is null -> rc should be SQLITE_OK and no other actions performed
static void test_sqlite3_reset_null_stmt(){
    // Reset global counters
    g_mutex_enters = g_mutex_leaves = 0;
    g_profile_calls = 0;
    g_vdbe_resets = 0;
    g_vdbe_rewinds = 0;
    g_api_exits = 0;

    int rc = sqlite3_reset(nullptr);

    // Expect the fast-path to return SQLITE_OK
    EXPECT_EQ(rc, SQLITE_OK);

    // Ensure no side-effect path was triggered
    EXPECT_EQ(g_mutex_enters, 0);
    EXPECT_EQ(g_mutex_leaves, 0);
    EXPECT_EQ(g_profile_calls, 0);
    EXPECT_EQ(g_vdbe_resets, 0);
    EXPECT_EQ(g_vdbe_rewinds, 0);
    EXPECT_EQ(g_api_exits, 0);
}

// Test 2: Non-null pStmt path executes in correct order and returns rc from ApiExit
static void test_sqlite3_reset_non_null_path_basic(){
    // Prepare a minimal database and Vdbe structures
    sqlite3_mutex mutexObj;
    sqlite3 db;
    db.mutex = &mutexObj;
    db.errMask = 0xFF; // allow any rc in range 0..255

    Vdbe v;
    v.db = &db;

    sqlite3_stmt st;
    st.dummy = 0; // make sure the object is non-empty

    // Point pStmt to our Vdbe to simulate a real call
    sqlite3_stmt* pStmt = (sqlite3_stmt*)&v;

    // Case A: rc returned by sqlite3VdbeReset is SQLITE_OK
    setNextVdbeResetRc(SQLITE_OK);
    // Reset counters
    g_mutex_enters = g_mutex_leaves = 0;
    g_profile_calls = 0;
    g_vdbe_resets = 0;
    g_vdbe_rewinds = 0;
    g_api_exits = 0;

    int rc = sqlite3_reset(pStmt);
    EXPECT_EQ(rc, SQLITE_OK);
    // Validate call sequence counters
    EXPECT_EQ(g_mutex_enters, 1);
    EXPECT_EQ(g_mutex_leaves, 1);
    EXPECT_EQ(g_profile_calls, 1);
    EXPECT_EQ(g_vdbe_resets, 1);
    EXPECT_EQ(g_vdbe_rewinds, 1);
    EXPECT_EQ(g_api_exits, 1);

    // Case B: rc returned by sqlite3VdbeReset is non-zero and different
    setNextVdbeResetRc(5); // arbitrary non-zero rc
    // Reset counters again
    g_mutex_enters = g_mutex_leaves = 0;
    g_profile_calls = 0;
    g_vdbe_resets = 0;
    g_vdbe_rewinds = 0;
    g_api_exits = 0;

    rc = sqlite3_reset(pStmt);
    EXPECT_EQ(rc, 5);
    EXPECT_EQ(g_mutex_enters, 1);
    EXPECT_EQ(g_mutex_leaves, 1);
    EXPECT_EQ(g_profile_calls, 1);
    EXPECT_EQ(g_vdbe_resets, 1);
    EXPECT_EQ(g_vdbe_rewinds, 1);
    EXPECT_EQ(g_api_exits, 1);
}

// Test 3: Ensure true/false branches of each predicate are exercised at least once
// (Covered implicitly by tests 1 and 2: pStmt==0 vs non-null; rc path does not trigger assertion failure
// due to errMask choice.)
static void test_sqlite3_reset_branch_coverage(){
    // This test intentionally performs a narrow check that both branches are viable.
    // We re-run a null-path to ensure no crash and a non-null path to ensure the else branch executes.
    test_sqlite3_reset_null_stmt();
    test_sqlite3_reset_non_null_path_basic();
}

// Driver
int main(){
    test_sqlite3_reset_null_stmt();
    test_sqlite3_reset_non_null_path_basic();
    test_sqlite3_reset_branch_coverage();

    if(g_failures != 0){
        std::cerr << "Some tests FAILED. Failures: " << g_failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
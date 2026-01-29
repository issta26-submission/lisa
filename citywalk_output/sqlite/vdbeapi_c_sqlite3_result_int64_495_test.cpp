// Test suite for the focal method sqlite3_result_int64
// This test tree is designed to be self-contained (no external test frameworks).
// It mocks the minimal environment required by the focal function and verifies
// true/false branches of the API-armor path and the normal path.

// DOMAIN: Create a minimal SQLite-like environment sufficient to exercise sqlite3_result_int64.

#include <opcodes.h>
#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Enable API armor to exercise the guarded path
#define SQLITE_ENABLE_API_ARMOR 1

// The core data types and functions (minimal stubs) required by sqlite3_result_int64

using i64 = long long;

// Simulated mutex primitive (only a flag to indicate "held" state)
struct sqlite3_mutex {
    bool isHeld;
};

// Minimal sqlite3 object containing a mutex (as in real SQLite)
struct sqlite3 {
    sqlite3_mutex mutex;
};

// Mem-like structure used by sqlite3_result_int64 to hold the result
struct Mem {
    i64 i;          // 64-bit integer value storage
    unsigned int flags;
    sqlite3* db;     // back-reference to database (to access mutex)
    Mem() : i(0), flags(0), db(nullptr) {}
};

// SQLite context object passed to sqlite3_result_int64
struct sqlite3_context {
    Mem* pOut;
};

// Flag indicating 64-bit integer value in Mem
static const unsigned MEM_Int64 = 0x01;

// Minimal implementation of sqlite3_mutex_held used by the focal function
static int sqlite3_mutex_held(sqlite3_mutex *p) {
    return p != nullptr && p->isHeld;
}

// Minimal implementation of sqlite3VdbeMemSetInt64 used by the focal function
static void sqlite3VdbeMemSetInt64(Mem *p, i64 i) {
    if(p) {
        p->i = i;
        p->flags = MEM_Int64;
    }
}

// The focal method under test (copied/adapted into this test file)
void sqlite3_result_int64(sqlite3_context *pCtx, i64 iVal){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( pCtx==0 ) return;
#endif
  // In production, this would assert if the mutex isn't held.
  // We replicate that check with a direct assertion against our stub.
  assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
  sqlite3VdbeMemSetInt64(pCtx->pOut, iVal);
}

// Candidate Keywords to guide test coverage (extracted from the focal function behavior)
// - API Armor path: pCtx==nullptr guard (only when SQLITE_ENABLE_API_ARMOR is defined)
// - Mutex ownership assertion: sqlite3_mutex_held(pCtx->pOut->db->mutex)
// - Memory write operation: sqlite3VdbeMemSetInt64(pCtx->pOut, iVal)


// Simple test harness (non-terminating assertions to maximize coverage)
// It reports failures but continues executing other tests.

static int g_failures = 0;
static void test_printf(const char* s) { std::cout << s; }

static void expect_true(bool cond, const char* msg){
    if(!cond){
        std::cerr << "FAILED: " << msg << " (condition evaluated to false)\n";
        ++g_failures;
    }
}
static void expect_eq_i64(i64 a, i64 b, const char* msg){
    if(a != b){
        std::cerr << "FAILED: " << msg << " expected " << b << " but got " << a << "\n";
        ++g_failures;
    }
}
static void expect_ptr_eq(void* a, void* b, const char* msg){
    if(a != b){
        std::cerr << "FAILED: " << msg << " pointer mismatch.\n";
        ++g_failures;
    }
}


// Test 1: Basic functionality - ensures that when pCtx and environment are correct,
// the value iVal is written into pCtx->pOut with MEM_Int64 flag.
void test_sqlite3_result_int64_basic() {
    // Prepare fake database and memory
    sqlite3 db;
    // Simulate mutex being held
    db.mutex.isHeld = true;

    Mem m;
    m.db = &db; // Mem points to its database (as in real SQLite: pOut->db)
    // Prepare context
    sqlite3_context ctx;
    ctx.pOut = &m;

    // Preconditions
    i64 testVal = 123456789012345LL;
    // Call the function under test
    sqlite3_result_int64(&ctx, testVal);

    // Postconditions
    // The Mem should contain the 64-bit value and MEM_Int64 flag should be set
    expect_eq_i64(m.i, testVal, "sqlite3_result_int64 should store the provided 64-bit value in Mem");
    expect_true((m.flags & MEM_Int64) != 0, "Mem flags should indicate MEM_Int64 after sqlite3_result_int64");
}

// Test 2: API Armor path - with pCtx==nullptr, the function should return immediately
// without modifying any memory. This exercises the true branch of the API-armor check.
void test_sqlite3_result_int64_api_armor_null_ctx() {
    // Setup a memory region to observe no side-effects when function is called with null context
    sqlite3 db;
    db.mutex.isHeld = true;
    Mem m;
    m.db = &db;
    sqlite3_context ctx;
    ctx.pOut = &m;

    i64 pre = m.i;
    // Call with null pCtx; should return immediately due to API armor
    sqlite3_result_int64(nullptr, 987654321);

    // Postconditions: memory should be unchanged
    expect_eq_i64(m.i, pre, "With NULL pCtx under API_ARMOR, Mem should remain unchanged");
    // Also ensure no unintended mutation occurred
}


// Test runner
int main() {
    test_printf("Starting tests for sqlite3_result_int64...\n");

    test_sqlite3_result_int64_basic();
    test_sqlite3_result_int64_api_armor_null_ctx();

    if(g_failures == 0){
        test_printf("All tests passed.\n");
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}
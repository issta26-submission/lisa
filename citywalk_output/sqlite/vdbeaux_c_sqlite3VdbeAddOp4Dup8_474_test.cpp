// High-quality C++11 unit test suite for sqlite3VdbeAddOp4Dup8
// This test suite provides a minimal, self-contained environment to exercise
// the focal function sqlite3VdbeAddOp4Dup8, along with its dependent helpers.
// It avoids external testing frameworks (no GTest) and uses a lightweight
// in-file test harness with non-terminating assertions.
//
// The tests cover:
// - True/false branches of memory allocation: when allocation succeeds vs fails.
// - Correct copying of 8-byte P4 data when allocation succeeds.
// - Correct forwarding of parameters to sqlite3VdbeAddOp4, including p4 pointer and p4type.
// - Return value propagation from sqlite3VdbeAddOp4 through sqlite3VdbeAddOp4Dup8.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain-specific: provide a minimal, self-contained environment that mimics
// the SQLite-like API used by sqlite3VdbeAddOp4Dup8.

extern "C" {

// Typedef for 8-bit unsigned data used for P4 operand
typedef unsigned char u8;

// Forward declarations of minimal structures
struct Db {};
struct Vdbe { Db* db; };

// Global toggle to simulate allocation failure in tests
static bool g_failMalloc = false;

// Global last-call capture for sqlite3VdbeAddOp4
static Vdbe* g_lastVdbe = nullptr;
static int   g_lastOp = 0;
static int   g_lastP1 = 0;
static int   g_lastP2 = 0;
static int   g_lastP3 = 0;
static void* g_lastP4 = nullptr;
static int   g_lastP4type = 0;

// Minimal allocator that respects fail flag
static void* sqlite3DbMallocRawNN(Db* db, int n){
    (void)db;      // db is not used in this minimal test harness
    (void)n;
    if (g_failMalloc) return nullptr;
    return std::malloc(n);
}

// Accessor to retrieve the Vdbe's Db* (mimic sqlite3VdbeDb)
static Db* sqlite3VdbeDb(Vdbe* p){
    return p ? p->db : nullptr;
}

// Minimal implementation of sqlite3VdbeAddOp4 used by the focal function
static int sqlite3VdbeAddOp4(Vdbe* p, int op, int p1, int p2, int p3, void* p4, int p4type){
    // Record the call for test assertions
    g_lastVdbe  = p;
    g_lastOp    = op;
    g_lastP1    = p1;
    g_lastP2    = p2;
    g_lastP3    = p3;
    g_lastP4     = p4;
    g_lastP4type = p4type;
    // Return a sentinel value to verify propagation
    return 999;
}

// The focal function under test
// Copy 8 bytes from zP4 into a newly allocated buffer (if allocation succeeds),
// then forward to sqlite3VdbeAddOp4
// extern "C" to ensure C linkage like the real project
int sqlite3VdbeAddOp4Dup8(
  Vdbe *p,            /* Add the opcode to this VM */
  int op,             /* The new opcode */
  int p1,             /* The P1 operand */
  int p2,             /* The P2 operand */
  int p3,             /* The P3 operand */
  const u8 *zP4,      /* The P4 operand */
  int p4type          /* P4 operand type */
){
  void *p4copy = sqlite3DbMallocRawNN(sqlite3VdbeDb(p), 8);
  if( p4copy ) memcpy(p4copy, zP4, 8);
  return sqlite3VdbeAddOp4(p, op, p1, p2, p3, p4copy, p4type);
}

} // extern "C"

// Lightweight test harness (non-terminating assertions; prints results)
static int g_testFailures = 0;

// Helper macros for non-terminating assertions
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { \
    std::cerr << "[FAIL] " << (msg) << "\n"; ++g_testFailures; } \
  } while(0)

static void test_alloc_and_copy_success() {
    // Purpose:
    // - Ensure when allocation succeeds, a proper 8-byte copy is made from zP4
    // - The copied memory is passed to sqlite3VdbeAddOp4 as p4
    // - Other parameters are forwarded unchanged
    // - The function returns the value from sqlite3VdbeAddOp4 (here 999)

    std::cout << "Test 1: Allocation success -> p4 copied and forwarded\n";

    // Reset global state
    g_failMalloc = false;
    g_lastVdbe = nullptr;
    g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
    g_lastP4 = nullptr;
    g_lastP4type = 0;

    Db db;
    Vdbe vm;
    vm.db = &db;

    const u8 zP4[8] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 }; // known data
    int op  = 77;
    int p1  = 1;
    int p2  = 2;
    int p3  = 3;
    int p4t = 99;

    int ret = sqlite3VdbeAddOp4Dup8(&vm, op, p1, p2, p3, zP4, p4t);

    // Return value propagation
    TEST_ASSERT(ret == 999, "sqlite3VdbeAddOp4Dup8 should return value from sqlite3VdbeAddOp4");

    // Correct VM reference passed through
    TEST_ASSERT(g_lastVdbe == &vm, "sqlite3VdbeAddOp4 should receive the same Vdbe instance");

    // Correct parameters forwarded
    TEST_ASSERT(g_lastOp == op, "Op code should be forwarded");
    TEST_ASSERT(g_lastP1 == p1, "P1 should be forwarded");
    TEST_ASSERT(g_lastP2 == p2, "P2 should be forwarded");
    TEST_ASSERT(g_lastP3 == p3, "P3 should be forwarded");
    TEST_ASSERT(g_lastP4 != nullptr, "P4 pointer should not be null when allocation succeeds");
    TEST_ASSERT(g_lastP4type == p4t, "P4 type should be forwarded");

    // Memory copy check: copy should be 8 bytes equal to zP4
    unsigned char copied[8];
    std::memcpy(copied, g_lastP4, 8);
    TEST_ASSERT(std::memcmp(copied, zP4, 8) == 0, "Copied 8-byte P4 data should match source zP4");

    // Cleanup: free allocated copy
    std::free(g_lastP4);
    std::cout << "Test 1 completed.\n\n";
}

static void test_alloc_failure_pass_null_p4() {
    // Purpose:
    // - Simulate allocation failure by toggling g_failMalloc
    // - Ensure that null is passed as P4 to sqlite3VdbeAddOp4
    // - The rest of the parameters are forwarded unchanged
    std::cout << "Test 2: Allocation failure -> p4 should be NULL and forwarded\n";

    // Enable allocation failure
    g_failMalloc = true;
    g_lastVdbe = nullptr;
    g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
    g_lastP4 = nullptr;
    g_lastP4type = 0;

    Db db;
    Vdbe vm;
    vm.db = &db;

    const u8 zP4[8] = { 0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x11,0x22 };
    int op  = 7;
    int p1  = 10;
    int p2  = 20;
    int p3  = 30;
    int p4t = 5;

    int ret = sqlite3VdbeAddOp4Dup8(&vm, op, p1, p2, p3, zP4, p4t);

    // Return value propagation
    TEST_ASSERT(ret == 999, "sqlite3VdbeAddOp4Dup8 should return value from sqlite3VdbeAddOp4");

    // When allocation fails, p4 should be NULL
    TEST_ASSERT(g_lastP4 == nullptr, "P4 passed to sqlite3VdbeAddOp4 should be NULL on allocation failure");

    // Forwarded parameters (except p4)
    TEST_ASSERT(g_lastOp == op, "Op code should be forwarded (even on allocation failure)");
    TEST_ASSERT(g_lastP1 == p1, "P1 should be forwarded");
    TEST_ASSERT(g_lastP2 == p2, "P2 should be forwarded");
    TEST_ASSERT(g_lastP3 == p3, "P3 should be forwarded");
    TEST_ASSERT(g_lastP4type == p4t, "P4 type should be forwarded on failure");

    // Cleanup: reset allocation flag
    g_failMalloc = false;
    std::cout << "Test 2 completed.\n\n";
}

static void test_multiple_variations() {
    // Purpose:
    // - Basic variation test to ensure multiple parameter combinations are handled
    // - Re-run a successful allocation path with a different op/p1/p2/p3/p4type
    std::cout << "Test 3: Multiple variations -> ensure forward logic remains correct across runs\n";

    // Reset global state
    g_failMalloc = false;
    g_lastVdbe = nullptr;
    g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
    g_lastP4 = nullptr;
    g_lastP4type = 0;

    Db db;
    Vdbe vm;
    vm.db = &db;

    const u8 zP4[8] = { 0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80 };
    int op  = 0x1A;
    int p1  = -1;
    int p2  = 0;
    int p3  = 999;
    int p4t = 7;

    int ret = sqlite3VdbeAddOp4Dup8(&vm, op, p1, p2, p3, zP4, p4t);

    TEST_ASSERT(ret == 999, "Return value should originate from sqlite3VdbeAddOp4");

    TEST_ASSERT(g_lastVdbe == &vm, "Vdbe instance should be preserved in last call");
    TEST_ASSERT(g_lastOp == op, "Op forwarded correctly");
    TEST_ASSERT(g_lastP1 == p1, "P1 forwarded (including negative values)");
    TEST_ASSERT(g_lastP2 == p2, "P2 forwarded");
    TEST_ASSERT(g_lastP3 == p3, "P3 forwarded");
    TEST_ASSERT(g_lastP4 != nullptr, "P4 should be allocated and passed in this variation");
    TEST_ASSERT(g_lastP4type == p4t, "P4 type forwarded in variation");

    unsigned char copied[8];
    std::memcpy(copied, g_lastP4, 8);
    TEST_ASSERT(std::memcmp(copied, zP4, 8) == 0, "Copied P4 bytes should match zP4 in variation");

    std::free(g_lastP4);
    std::cout << "Test 3 completed.\n\n";
}

int main() {
    std::cout << "Starting sqlite3VdbeAddOp4Dup8 unit tests (C++11, no GTest)\n\n";

    test_alloc_and_copy_success();
    test_alloc_failure_pass_null_p4();
    test_multiple_variations();

    if (g_testFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) FAILED\n";
        return 1;
    }
}
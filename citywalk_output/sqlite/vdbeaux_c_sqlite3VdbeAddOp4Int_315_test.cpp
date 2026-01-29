// Unit test suite for sqlite3VdbeAddOp4Int
// Target: validate fast-path (pre-allocated Op array) behavior of sqlite3VdbeAddOp4Int
// Notes:
// - This test uses the actual sqlite3 internal types Vdbe and VdbeOp via the library headers.
// - Test 1 covers the fast path where p->nOpAlloc > i (i = p->nOp). It verifies that opcode, operands
//   and p4 values are written correctly.
// - Test 2 exercises the slow path by forcing the condition p->nOpAlloc <= i. This path is implemented
//   by a static function addOp4IntSlow in the same translation unit and is not directly observable here;
//   the test ensures the function returns a valid int in this scenario (basic sanity check).
// - The tests are executed from main() without any external test framework (non-terminating assertions style).
// - Make sure to build/link against the sqlite source so that Vdbe and VdbeOp structures and
//   sqlite3VdbeAddOp4Int are available.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Import sqlite internal headers (C linkage)
extern "C" {
}

// Lightweight test assertion helpers (non-terminating)
static int g_test_failures = 0;

#define TEST_FAIL(msg) do { \
    std::cerr << "TEST FAILURE: " msg " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    g_test_failures++; \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { TEST_FAIL("EXPECTED TRUE: " #cond); } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if(((a) != (b))) { TEST_FAIL("EXPECTED EQUAL: " #a " == " #b); } \
} while(0)

#define EXPECT_GE(a,b) do { \
    if(((a) < (b))) { TEST_FAIL("EXPECTED GREATER OR EQUAL: " #a " >= " #b); } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
    if((p) == nullptr) { TEST_FAIL("EXPECTED NON-NULL POINTER"); } \
} while(0)

// Test 1: Fast-path behavior (pre-allocated Op array)
bool test_sqlite3VdbeAddOp4Int_fastpath()
{
    // Prepare a Vdbe with a pre-allocated Op array (nOpAlloc > nOp)
    Vdbe v;
    VdbeOp *ops = new VdbeOp[1];
    // Zero-initialize to be safe; real code will populate fields
    std::memset(&v, 0, sizeof(v));
    std::memset(ops, 0, sizeof(VdbeOp) * 1);

    v.nOp = 0;
    v.nOpAlloc = 1; // ensures fast path: nOpAlloc <= i is false when i == 0
    v.aOp = ops;

    // Parameters for the new op
    int op  = 0x12;
    int p1  = 10;
    int p2  = 20;
    int p3  = 30;
    int p4  = 12345;

    // Call the function under test
    int ret = sqlite3VdbeAddOp4Int(&v, op, p1, p2, p3, p4);

    // Validate return value and affected state
    EXPECT_EQ(ret, 0);          // i should be the original nOp (0)
    EXPECT_EQ(v.nOp, 1);          // nOp should have been incremented
    VdbeOp &o = ops[0];
    EXPECT_EQ((int)o.opcode, op); // opcode should be set
    EXPECT_EQ((int)o.p5, 0);      // p5 should be 0
    EXPECT_EQ((int)o.p1, p1);
    EXPECT_EQ((int)o.p2, p2);
    EXPECT_EQ((int)o.p3, p3);
    // p4 is a union; access .i for integer literals
    EXPECT_EQ((int)o.p4.i, p4);
    EXPECT_EQ((int)o.p4type, P4_INT32);

    // Cleanup
    delete[] ops;
    return g_test_failures == 0;
}

// Test 2: Slow-path invocation (force the static addOp4IntSlow path)
// Note: This path is internal (static) to the vdbeaux.c translation unit. We cannot observe
// internal mutations directly, but we can exercise the path and perform a basic sanity check
// that a return value is produced (non-crashing behavior and non-negative index value).
bool test_sqlite3VdbeAddOp4Int_slowpath()
{
    // Prepare a Vdbe without pre-allocated Op array to force slow path
    Vdbe v;
    VdbeOp *ops = nullptr; // not allocated

    std::memset(&v, 0, sizeof(v));
    v.nOp = 0;
    v.nOpAlloc = 0; // triggers slow path: p->nOpAlloc <= i (0 <= 0)
    v.aOp = ops;

    int op  = 1;
    int p1  = 2;
    int p2  = 3;
    int p3  = 4;
    int p4  = 5;

    int ret = sqlite3VdbeAddOp4Int(&v, op, p1, p2, p3, p4);

    // Sanity check: function should return a valid non-negative address/index
    EXPECT_TRUE(ret >= 0);

    // Note: We cannot reliably inspect internals due to static function, but we can ensure
    // that the VM remains in a sane state (nOp may or may not increase depending on slow path implementation).
    // A conservative check is that code executed and returned an int.
    return g_test_failures == 0;
}

// Entry point
int main()
{
    std::cout << "Starting sqlite3VdbeAddOp4Int unit tests (C++11 harness, no GTest).\n";

    bool ok = true;

    if(!test_sqlite3VdbeAddOp4Int_fastpath()) {
        std::cerr << "Test 1 (fast-path) failed.\n";
        ok = false;
    } else {
        std::cout << "Test 1 (fast-path) passed.\n";
    }

    if(!test_sqlite3VdbeAddOp4Int_slowpath()) {
        std::cerr << "Test 2 (slow-path) failed.\n";
        ok = false;
    } else {
        std::cout << "Test 2 (slow-path) passed (sanity check).\n";
    }

    if(!ok) {
        std::cerr << "One or more tests FAILED. Failures: " << g_test_failures << "\n";
        return 1;
    }

    std::cout << "All tests PASSED.\n";
    return 0;
}
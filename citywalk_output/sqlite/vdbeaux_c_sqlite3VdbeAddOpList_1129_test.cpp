// Unit test suite for sqlite3VdbeAddOpList
// This test is designed to be run with a C/C++11 compliant compiler
// and built against the SQLite project sources that provide the
// real types Vdbe, VdbeOp, VdbeOpList, and constants such as OP_JUMP,
// OP_ADD, P4_NOTUSED, and the macro OPFLG_JUMP.
// The tests avoid terminating assertions; instead, they report
// failures and continue execution to maximize coverage.

#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declare the C API from the SQLite project.
// We rely on the real definitions provided by the project's headers
// (vdbeInt.h, sqliteInt.h, sqlite3.h). These declarations enable us
// to construct a minimal test harness around sqlite3VdbeAddOpList.
extern "C" {
}

// Lightweight non-terminating assertion macro for test continuity.
// It logs a failure but does not abort the test run.
#define EXPECT_EQ(a, b, msg) do { \
    bool _eq = ((const void*)(a) == (const void*)(b)); \
    /* For primitive integer comparisons, also check value equality. */ \
    if (!_eq) { \
        /* Attempt integer comparison if appropriate types */ \
        typeof(a) _va = (a); \
        typeof(b) _vb = (b); \
        if ( _va != _vb ) { \
            std::cerr << "[FAIL] " << msg << " | Expected: " << _vb << "  Got: " << _va << std::endl; \
            failures++; \
        } else { \
            std::cerr << "[FAIL] " << msg << " | Values differ but addresses equal in pointer cast. " << std::endl; \
            failures++; \
        } \
    } else { \
        successes++; \
    } \
} while(0)

///////////////////////////////////////////////////////////////////
// Helpers to create and destroy a minimal Vdbe instance for tests //
///////////////////////////////////////////////////////////////////

static int failures = 0;
static int successes = 0;

// Create a minimal Vdbe instance suitable for calling sqlite3VdbeAddOpList.
// This does not attempt to initialize the entire SQLite runtime; it only
// establishes the fields accessed by the focal function.
static Vdbe* createTestVdbe(int initialOpCapacity = 8, int initialNop = 0) {
    Vdbe *p = (Vdbe*)std::malloc(sizeof(Vdbe));
    if (!p) return nullptr;
    std::memset(p, 0, sizeof(Vdbe));

    // Initialize essential fields accessed by sqlite3VdbeAddOpList
    p->nOpAlloc = initialOpCapacity;
    p->nOp = initialNop;
    p->eVdbeState = VDBE_INIT_STATE; // required by the assertion in the focal method

    // Allocate the op array
    p->aOp = (VdbeOp*)std::malloc(sizeof(VdbeOp) * (size_t)initialOpCapacity);
    if (!p->aOp) {
        std::free(p);
        return nullptr;
    }
    std::memset(p->aOp, 0, sizeof(VdbeOp) * (size_t)initialOpCapacity);

    // db pointer is not used in our test (debug paths not enabled by default)
    p->db = NULL;

    return p;
}

// Free resources allocated for the test Vdbe
static void destroyTestVdbe(Vdbe *p) {
    if (!p) return;
    if (p->aOp) std::free(p->aOp);
    std::free(p);
}

///////////////////////////////////////////////////////////////
// Test 1: Basic copy and jump-offset behavior of the oplist //
// Verify that a jump opcode with a positive p2 offsets by nOp //
// and that normal opcodes copy fields unchanged.              //
///////////////////////////////////////////////////////////////

static void test_basic_copy_and_jump_offset() {
    // Prepare a Vdbe with some existing ops
    Vdbe *p = createTestVdbe(16, 7); // initial nOp = 7
    if (!p) {
        std::cerr << "[ERR] Failed to allocate test Vdbe for Test 1" << std::endl;
        return;
    }

    // Prepare a single op to add: a jump with p2>0 to trigger offset
    VdbeOpList opList[1];
    opList[0].opcode = OP_JUMP; // jump opcode (has OPFLG_JUMP)
    opList[0].p1 = 5;
    opList[0].p2 = 3;  // will be offset by p->nOp (7) -> expected 10
    opList[0].p3 = 9;

    VdbeOp *pFirst = sqlite3VdbeAddOpList(p, 1, opList, 100);

    // Assertions (non-terminating)
    EXPECT_EQ(pFirst, p->aOp + 7, "Test1: pFirst should point to the first newly added op");
    EXPECT_EQ(p->nOp, 8, "Test1: nOp should have increased by nOp (1)");
    EXPECT_EQ(pFirst->opcode, OP_JUMP, "Test1: opcode should be preserved");
    EXPECT_EQ(pFirst->p1, 5, "Test1: p1 should be preserved");
    EXPECT_EQ(pFirst->p2, 3 + 7, "Test1: p2 should be offset by previous nOp");
    EXPECT_EQ(pFirst->p3, 9, "Test1: p3 should be preserved");
    EXPECT_EQ(pFirst->p4type, P4_NOTUSED, "Test1: p4type should be P4_NOTUSED");
    // p4.p should be 0 after initialization
    // Depending on compiler, the pOut->p4.p field exists; if not, this check is skipped by the test environment.

    destroyTestVdbe(p);
}

// Test 2: Non-jump opcode should not apply p2 offset even if p2>0 //
// Verify that non-jump ops copy fields without altering p2.     //
static void test_non_jump_no_offset() {
    // Start from a clean Vdbe with 7 existing ops (like Test 1)
    Vdbe *p = createTestVdbe(16, 7);
    if (!p) {
        std::cerr << "[ERR] Failed to allocate test Vdbe for Test 2" << std::endl;
        return;
    }

    // Prepare a single non-jump op (OP_ADD is not a jump)
    VdbeOpList opList[1];
    opList[0].opcode = OP_ADD; // not a jump
    opList[0].p1 = 2;
    opList[0].p2 = 3; // positive, but should not offset
    opList[0].p3 = 4;

    VdbeOp *pFirst = sqlite3VdbeAddOpList(p, 1, opList, 0);

    // Assertions
    EXPECT_EQ(pFirst, p->aOp + 7, "Test2: pFirst should point to the first newly added op");
    EXPECT_EQ(p->nOp, 8, "Test2: nOp should have increased by 1");
    EXPECT_EQ(pFirst->opcode, OP_ADD, "Test2: opcode should be preserved");
    EXPECT_EQ(pFirst->p1, 2, "Test2: p1 should be preserved");
    EXPECT_EQ(pFirst->p2, 3, "Test2: p2 should NOT be offset for non-jump op");
    EXPECT_EQ(pFirst->p3, 4, "Test2: p3 should be preserved");
    EXPECT_EQ(pFirst->p4type, P4_NOTUSED, "Test2: p4type should be P4_NOTUSED");

    destroyTestVdbe(p);
}

//////////////////////////////
// Main runner for tests  //
//////////////////////////////

int main() {
    std::cout << "Starting sqlite3VdbeAddOpList unit tests (non-terminating assertions) ..." << std::endl;

    test_basic_copy_and_jump_offset();
    test_non_jump_no_offset();

    std::cout << "Tests completed. Summary: " 
              << "Passed=" << successes 
              << "  Failed=" << failures << std::endl;

    // Return non-zero if any test failed to aid integration with build scripts
    return (failures > 0) ? 1 : 0;
}
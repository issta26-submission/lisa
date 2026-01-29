#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain knowledge: We'll build a lightweight C++ test harness (no GTest) that
// exercises the focal function sqlite3VdbeTypeofColumn by constructing
// minimal Vdbe and VdbeOp instances and verifying the bitwise flag behavior.
//
// We rely on the project-provided headers for correct structure definitions and
// constants (e.g., OP_Column, OPFLAG_TYPEOFARG). These headers are C headers
// and we include them with C linkage to avoid name mangling issues.

extern "C" {
}

// Simple portable test assertion (non-terminating)
static bool expect(bool cond, const char* message, std::ostream &out = std::cerr) {
    if (!cond) {
        out << "[FAIL] " << message << std::endl;
        return false;
    } else {
        // Debug: uncomment to print successes
        // out << "[OK] " << message << std::endl;
        return true;
    }
}

// Helper to create a Vdbe with a single operation
static Vdbe* makeVdbeWithSingleOp(unsigned int opcode, int p3, uint16_t p5Init) {
    // Allocate a Vdbe object and a single VdbeOp
    Vdbe* p = new Vdbe();
    p->nOp = 1;
    p->aOp = new VdbeOp[1];
    // Initialize the last (and only) op
    p->aOp[0].opcode = (unsigned char)opcode;
    p->aOp[0].p3 = p3;
    p->aOp[0].p5 = p5Init;
    // Note: other fields are not used by sqlite3VdbeTypeofColumn
    return p;
}

// Helper to clean up a Vdbe object created for tests
static void destroyVdbe(Vdbe* p) {
    if (p) {
        if (p->aOp) delete[] p->aOp;
        delete p;
    }
}

// Test 1: When last op has p3 == iDest and opcode == OP_Column, the function
// should set the OPFLAG_TYPEOFARG bit in p5.
static bool test_lastop_matches_sets_flag() {
    const int iDest = 5;
    Vdbe* p = makeVdbeWithSingleOp(OP_Column, iDest, 0);
    // Ensure initial state: p5 has no OFARG flag
    uint16_t before = p->aOp[0].p5;
    bool ok = true;
    sqlite3VdbeTypeofColumn(p, iDest);
    uint16_t after = p->aOp[0].p5;
    ok &= expect((before & OPFLAG_TYPEOFARG) == 0, "Initial p5 should not have OFARG");
    ok &= expect((after & OPFLAG_TYPEOFARG) != 0, "p5 should have OFARG set after call");
    destroyVdbe(p);
    return ok;
}

// Test 2: When last op has p3 != iDest, the function should not modify p5.
static bool test_lastop_p3_diff_no_change() {
    const int iDest = 7;
    Vdbe* p = makeVdbeWithSingleOp(OP_Column, /*p3*/ 3, 0);
    // p3 does not match iDest
    sqlite3VdbeTypeofColumn(p, iDest);
    bool ok = true;
    ok &= expect((p->aOp[0].p5 & OPFLAG_TYPEOFARG) == 0, "p5 should remain unchanged when p3 != iDest");
    destroyVdbe(p);
    return ok;
}

// Test 3: When last op has opcode != OP_Column, the function should not modify p5.
static bool test_lastop_wrong_opcode_no_change() {
    const int iDest = 5;
    Vdbe* p = makeVdbeWithSingleOp(/*opcode*/ OP_Assign, iDest, 0); // OP_Assign chosen to differ from OP_Column
    // In SQLite, the opcode constant OP_Assign may not exist; if not, fallback to a known non-Column opcode
    // The test relies on the presence of a non-OP_Column opcode. If OP_Assign is unavailable, use a large value.
#ifdef OP_Assign
    // ok
#else
    p->aOp[0].opcode = 255; // ensure not OP_Column
#endif
    sqlite3VdbeTypeofColumn(p, iDest);
    bool ok = true;
    ok &= expect((p->aOp[0].p5 & OPFLAG_TYPEOFARG) == 0, "p5 should remain unchanged when opcode != OP_Column");
    destroyVdbe(p);
    return ok;
}

// Test 4: If p5 already has OFARG bit set, the function should keep it (idempotent behavior).
static bool test_p5_already_set_kept() {
    const int iDest = 5;
    Vdbe* p = makeVdbeWithSingleOp(OP_Column, iDest, OPFLAG_TYPEOFARG); // already set
    sqlite3VdbeTypeofColumn(p, iDest);
    bool ok = true;
    ok &= expect((p->aOp[0].p5 & OPFLAG_TYPEOFARG) != 0, "p5 OFARG bit should remain set after call");
    destroyVdbe(p);
    return ok;
}

int main() {
    int overall_ok = 1;

    std::cout << "Starting tests for sqlite3VdbeTypeofColumn" << std::endl;

    bool r1 = test_lastop_matches_sets_flag();
    overall_ok &= r1;
    std::cout << "Test 1 " << (r1 ? "PASSED" : "FAILED") << std::endl;

    bool r2 = test_lastop_p3_diff_no_change();
    overall_ok &= r2;
    std::cout << "Test 2 " << (r2 ? "PASSED" : "FAILED") << std::endl;

    bool r3 = test_lastop_wrong_opcode_no_change();
    overall_ok &= r3;
    std::cout << "Test 3 " << (r3 ? "PASSED" : "FAILED") << std::endl;

    bool r4 = test_p5_already_set_kept();
    overall_ok &= r4;
    std::cout << "Test 4 " << (r4 ? "PASSED" : "FAILED") << std::endl;

    if (overall_ok) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
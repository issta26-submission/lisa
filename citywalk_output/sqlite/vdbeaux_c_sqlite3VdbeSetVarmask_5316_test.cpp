// This is a self-contained C++11 test harness for the focal C function:
// sqlite3VdbeSetVarmask(Vdbe *v, int iVar)
// The real project would include sqlite3/vdbe headers; here we provide minimal
// stubs to exercise the function logic in a single compilation unit.

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain-specific constants (mimic the real SQLite flag bit)
#define SQLITE_EnableQPSG 0x01

// Lightweight stand-ins for the production structs used by sqlite3VdbeSetVarmask
typedef uint32_t u32;

struct Db {
    int flags;
};

struct Vdbe {
    Db *db;          // back-pointer to database object
    uint32_t expmask; // mask of enabled VARM (expmask)
};

// FOCAL_METHOD replicated here to allow isolated unit testing without the
// full SQLite source tree. The logic is identical to the production code.
void sqlite3VdbeSetVarmask(Vdbe *v, int iVar){
    // Precondition checks (kept exact as in production)
    assert( iVar>0 );
    assert( (v->db->flags & SQLITE_EnableQPSG)==0 );
    if( iVar>=32 ){
        v->expmask |= 0x80000000;
    }else{
        v->expmask |= ((u32)1 << (iVar-1));
    }
}

/*
  Simple, self-contained unit test framework (non-terminating assertions).
  The tests print PASS/FAIL messages to stdout so they can be run without a
  testing framework like Google Test (as per the project constraints).
  Each test is annotated with comments explaining what is being validated.
*/

// Minimal helper to report test results without terminating the process
bool checkEq(uint32_t actual, uint32_t expected, const char* testName){
    if(actual == expected){
        printf("[PASS] %s\n", testName);
        return true;
    }else{
        printf("[FAIL] %s: got %u, expected %u\n", testName, actual, expected);
        return false;
    }
}

// Candidate Keywords derived from the focal method (for test design alignment)
// - Vdbe, Db, expmask, iVar, SQLITE_EnableQPSG, 32-bit mask behavior
// - Branching on iVar < 32 vs iVar >= 32
// - Preconditions: iVar > 0 and (v->db->flags & SQLITE_EnableQPSG) == 0
// - Bit operations: 1ULL << (iVar-1) for iVar in [1..31], 0x80000000 for iVar >= 32

// Test 1: iVar = 1 should set the least-significant bit (bit 0) in expmask
bool test_case_iVar_1_sets_bit0(){
    Db db; db.flags = 0;
    Vdbe v; v.db = &db; v.expmask = 0;

    sqlite3VdbeSetVarmask(&v, 1);

    // Expect expmask to have bit 0 set -> value 1
    return checkEq(v.expmask, 1, "iVar=1 -> expmask should be 1");
}

// Test 2: iVar = 5 should set bit corresponding to (iVar-1) i.e., bit 4
bool test_case_iVar_5_sets_bit4(){
    Db db; db.flags = 0;
    Vdbe v; v.db = &db; v.expmask = 0;

    sqlite3VdbeSetVarmask(&v, 5);

    // Expect expmask to have bit 4 set -> value 16
    return checkEq(v.expmask, 0x10, "iVar=5 -> expmask should be 0x10");
}

// Test 3: iVar = 32 should set the high bit (0x80000000)
bool test_case_iVar_32_sets_high_bit(){
    Db db; db.flags = 0;
    Vdbe v; v.db = &db; v.expmask = 0;

    sqlite3VdbeSetVarmask(&v, 32);

    // Expect expmask to have high bit set -> value 0x80000000
    return checkEq(v.expmask, 0x80000000, "iVar=32 -> expmask should be 0x80000000");
}

// Test 4: Accumulation across multiple calls (bitwise OR behavior)
// Start fresh, then set iVar=1 and iVar=32, ensuring both bits are set
bool test_case_accumulate_bits_var38(){
    Db db; db.flags = 0;
    Vdbe v; v.db = &db; v.expmask = 0;

    sqlite3VdbeSetVarmask(&v, 1);   // set bit 0
    sqlite3VdbeSetVarmask(&v, 32);  // set high bit

    // Expect expmask to have bits 0 and 31 set (0x80000001)
    return checkEq(v.expmask, 0x80000001, "accumulate bits: iVar=1 then iVar=32 -> expmask should be 0x80000001");
}

// Test 5: Accumulate with iVar=31 to verify (iVar-1) = 30 -> 0x40000000
bool test_case_iVar_31_sets_bit30(){
    Db db; db.flags = 0;
    Vdbe v; v.db = &db; v.expmask = 0;

    sqlite3VdbeSetVarmask(&v, 31);

    // Expect expmask to have bit 30 set -> value 0x40000000
    return checkEq(v.expmask, 0x40000000, "iVar=31 -> expmask should be 0x40000000");
}

// Entry point: run all tests and report overall status
int main(){
    // Note: We deliberately avoid triggering precondition asserts by ensuring
    // iVar > 0 and db->flags does not have SQLITE_EnableQPSG set (0).
    // This mirrors typical unit-test usage where inputs are controlled.

    bool all_passed =
        test_case_iVar_1_sets_bit0() &
        test_case_iVar_5_sets_bit4() &
        test_case_iVar_32_sets_high_bit() &
        test_case_accumulate_bits_var38() &
        test_case_iVar_31_sets_bit30();

    if(all_passed){
        printf("[SUMMARY] All tests PASSED\n");
        return 0;
    }else{
        printf("[SUMMARY] SOME TESTS FAILED\n");
        return 1;
    }
}
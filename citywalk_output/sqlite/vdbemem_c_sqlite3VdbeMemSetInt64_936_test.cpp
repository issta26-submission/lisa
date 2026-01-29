// Unit tests for the focal method: sqlite3VdbeMemSetInt64
// This test suite follows the provided step-by-step guidance (Step 1..3) in a
// standalone C++11 test harness without using GTest.
// The tests exercise both branches of the conditional:
//  - true: VdbeMemDynamic(pMem) is true
//  - false: VdbeMemDynamic(pMem) is false
// We rely on the actual internal SQLite types and function prototype to ensure
// realistic behavior when linking against the project under test.
//
// Notes:
// - We do not rely on private/private-static behavior beyond what sqlite3VdbeMemSetInt64
//   exposes through the Mem structure. Static helpers inside vdbemem.c are out of scope.
// - Static/static-global state is not mocked; we exercise via real function calls.
// - The tests use simple, non-terminating checks (no aborts) and report a summary.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>


// Include internal SQLite type definitions.
// These headers define Mem, MEM_Int, MEM_Dyn, sqlite3_int64, etc.
// This assumes the build environment provides the internal headers (as in a full SQLite source tree).
extern "C" {
  #include "sqliteInt.h"   // Defines Mem, MEM_Int, MEM_Dyn, sqlite3_int64, etc.
  // Prototype for the focal function under test.
  void sqlite3VdbeMemSetInt64(Mem *pMem, sqlite3_int64 val);
}

// Simple non-terminating assertion helper
static bool expect_true(bool cond, const char* msg) {
    if(!cond) {
        std::cerr << "EXPECTATION FAILED: " << msg << "\n";
        return false;
    }
    return true;
}

// Test 1: Non-dynamic branch (VdbeMemDynamic(pMem) == false)
static bool test_VdbeMemSetInt64_nonDynamic() {
    // Arrange
    Mem m;
    std::memset(&m, 0, sizeof(Mem));  // Start from a clean slate
    sqlite3_int64 val = 123456789012345LL;

    // Act
    sqlite3VdbeMemSetInt64(&m, val);

    // Assert
    // In the non-dynamic branch, the code path sets:
    //   m.u.i = val;
    //   m.flags = MEM_Int;
    bool ok_i = (m.u.i == val);
    bool ok_flags = (m.flags == MEM_Int);

    if(!expect_true(ok_i, "Non-dynamic: m.u.i should equal the input value")) return false;
    if(!expect_true(ok_flags, "Non-dynamic: m.flags should be MEM_Int after set")) return false;

    return true;
}

// Test 2: Dynamic branch (VdbeMemDynamic(pMem) == true)
static bool test_VdbeMemSetInt64_dynamic() {
    // Arrange
    Mem m;
    std::memset(&m, 0, sizeof(Mem));  // Start from a clean slate
    // Force the dynamic path. MEM_Dyn is the flag that makes VdbeMemDynamic(pMem) true.
    m.flags = MEM_Dyn;
    sqlite3_int64 val = -9876543210987LL;

    // Act
    sqlite3VdbeMemSetInt64(&m, val);

    // Assert
    // In the dynamic branch, it calls vdbeReleaseAndSetInt64(pMem, val).
    // The expected end state for correctness is that the value is stored as an int64
    // and the MEM_Int flag is set (final representation should be integer).
    bool ok_i = (m.u.i == val);
    bool ok_flags = (m.flags == MEM_Int);

    if(!expect_true(ok_i, "Dynamic: m.u.i should equal the input value after set")) return false;
    if(!expect_true(ok_flags, "Dynamic: m.flags should be MEM_Int after set")) return false;

    return true;
}

int main() {
    int tests_run = 0;
    int tests_passed = 0;

    std::cout << "Running sqlite3VdbeMemSetInt64 tests (C++11, no GTest)..." << std::endl;

    // Test 1: Non-dynamic path
    {
        tests_run++;
        bool ok = test_VdbeMemSetInt64_nonDynamic();
        if(ok) tests_passed++;
        std::cout << "Test 1 (non-dynamic) " << (ok ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 2: Dynamic path
    {
        tests_run++;
        bool ok = test_VdbeMemSetInt64_dynamic();
        if(ok) tests_passed++;
        std::cout << "Test 2 (dynamic) " << (ok ? "PASSED" : "FAILED") << std::endl;
    }

    // Summary
    std::cout << "Summary: " << tests_passed << " / " << tests_run << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (tests_passed == tests_run) ? 0 : 1;
}
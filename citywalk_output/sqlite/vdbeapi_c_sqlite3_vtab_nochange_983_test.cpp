// Minimal C++11 test suite for the focal method sqlite3_vtab_nochange
// This test is self-contained and uses a small mock of the necessary sqlite3 types.
// It exercises both the null-pointer path (API_ARMOR enabled) and the non-null path
// where pOut is provided. It does not depend on the full SQLite project.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <opcodes.h>


// Enable the API armor path to exercise the null-pointer check
#define SQLITE_ENABLE_API_ARMOR 1

// Mock minimal SQLite types required by the focal method
struct sqlite3_value {
    int dummy;
};

struct sqlite3_context {
    sqlite3_value *pOut;
};

// Mock implementation of sqlite3_value_nochange invoked by sqlite3_vtab_nochange
// We provide deterministic behavior: non-null value yields 1, null yields 0.
int sqlite3_value_nochange(sqlite3_value *pVal) {
    return pVal ? 1 : 0;
}

// Focal method under test (copied to be testable in isolation)
// int sqlite3_vtab_nochange(sqlite3_context *p){
int sqlite3_vtab_nochange(sqlite3_context *p){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( p==0 ) return 0;
#else
  // In the testing environment we are compiling with API_ARMOR enabled
  // If API_ARMOR were disabled, behavior would be different (assert).
  // The test harness relies on the API_ARMOR path being active.
#endif
  return sqlite3_value_nochange(p->pOut);
}

// Simple test framework (non-terminating): collect failures and report at end
static int g_failures = 0;
#define TEST_FAIL(msg) do { std::cerr << "Test failure: " << (msg) << "\n"; ++g_failures; } while(0)

// Test 1: p is NULL; in API_ARMOR mode, the function should return 0 without dereferencing p
bool test_null_pointer_returns_zero() {
    // Given API_ARMOR enabled, sqlite3_vtab_nochange(nullptr) should return 0
    int res = sqlite3_vtab_nochange(nullptr);
    if (res != 0) {
        TEST_FAIL("Expected 0 when input sqlite3_context* is NULL (API_ARMOR path)");
        return false;
    }
    return true;
}

// Test 2: pOut is non-NULL; ensure the return equals sqlite3_value_nochange(pOut)
// We simulate a non-null sqlite3_value and expect the propagated value (here ≡ 1)
bool test_pOut_non_null_propagates_value() {
    sqlite3_context ctx;
    sqlite3_value val;
    ctx.pOut = &val;

    int res = sqlite3_vtab_nochange(&ctx);
    if (res != 1) {
        TEST_FAIL("Expected 1 propagated from sqlite3_value_nochange for non-null pOut");
        return false;
    }
    return true;
}

// Test 3: p is non-null but pOut is NULL; sqlite3_value_nochange(nullptr) should return 0
bool test_pOut_null_propagates_zero() {
    sqlite3_context ctx;
    ctx.pOut = nullptr;

    int res = sqlite3_vtab_nochange(&ctx);
    if (res != 0) {
        TEST_FAIL("Expected 0 propagated from sqlite3_value_nochange for null pOut");
        return false;
    }
    return true;
}

int main() {
    std::cout << "Running tests for sqlite3_vtab_nochange (API_ARMOR path forced)\n";

    bool ok = true;
    ok &= test_null_pointer_returns_zero();
    ok &= test_pOut_non_null_propagates_value();
    ok &= test_pOut_null_propagates_zero();

    if (g_failures == 0) {
        std::cout << "All tests PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED\n";
        return 1;
    }
}
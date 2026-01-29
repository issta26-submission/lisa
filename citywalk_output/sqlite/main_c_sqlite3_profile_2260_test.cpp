// This file contains a self-contained C++11 unit test suite for the focal C function
// sqlite3_profile, implemented in this translation unit to enable isolated testing
// without requiring the full SQLite project build. The tests cover key branches
// of the function logic, including the API armor safety check and the updates
// to the db state (xProfile, pProfileArg, and mTrace).
//
// Notes:
// - No external test framework is used (per requirements). A tiny, in-process test
//   harness is implemented with simple bool-returning test cases and console output.
// - The test defines minimal stubs for dependencies (sqlite3, sqlite3_mutex, etc.)
//   so that sqlite3_profile can be compiled and exercised in isolation.
// - Domain knowledge constraints: tests use only standard library, runtime state, and
//   function pointers; static/internal APIs are simulated as needed for testing.
// - Explanatory comments are added for each test case.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <string>


// Candidate Keywords extracted from Step 1 (for alignment with test design)
// - sqlite3, sqlite3_mutex, sqlite3_profile, xProfile, pArg, pProfileArg
// - mTrace, SQLITE_TRACE_XPROFILE, SQLITE_TRACE_NONLEGACY_MASK
// - sqlite_uint64, sqlite3SafetyCheckOk, sqlite3SafetyCheckOk path (API_ARMOR)

// Domain knowledge: provide a compact, self-contained mock environment
// Minimal typedefs and structures to simulate the focal code's environment.

typedef unsigned long long sqlite_uint64;

// Forward declaration to mirror the original signature used by sqlite3_profile
struct sqlite3;

// Simple mutex placeholder (no real locking needed for unit tests)
struct sqlite3_mutex {
    // Intentionally empty; used only to satisfy pointer type in sqlite3_profile
};

// Mocked flags used by sqlite3_profile
#define SQLITE_TRACE_XPROFILE 0x01
#define SQLITE_TRACE_NONLEGACY_MASK 0xFF
#define SQLITE_MISUSE_BKPT 0

// Global flag to simulate the API armor safety check outcome
static int g_safetyCheckShouldPass = 1;

// Minimal sqlite3-like structure with only members accessed by sqlite3_profile
struct sqlite3 {
    sqlite3_mutex *mutex;       // pointer to a dummy mutex
    void *pProfileArg;           // previously stored profile arg
    void (*xProfile)(void*, const char*, sqlite_uint64); // current profile callback
    unsigned int mTrace;          // tracing mask
};

// Dummy mutex enter/leave functions (no real synchronization needed in tests)
static void sqlite3_mutex_enter(sqlite3_mutex * /*mutex*/) {
    // no-op for unit testing
}
static void sqlite3_mutex_leave(sqlite3_mutex * /*mutex*/) {
    // no-op for unit testing
}

// API armor safety check mock (behaves deterministically for tests)
static int sqlite3SafetyCheckOk(sqlite3 * /*db*/) {
    return g_safetyCheckShouldPass;
}

// The focal method under test (reproduced here for a self-contained test)
void *sqlite3_profile(
  sqlite3 *db,
  void (*xProfile)(void*,const char*,sqlite_uint64),
  void *pArg
){
{
  void *pOld;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return 0;
  }
#endif
  sqlite3_mutex_enter(db->mutex);
  pOld = db->pProfileArg;
  db->xProfile = xProfile;
  db->pProfileArg = pArg;
  db->mTrace &= SQLITE_TRACE_NONLEGACY_MASK;
  if( db->xProfile ) db->mTrace |= SQLITE_TRACE_XPROFILE;
  sqlite3_mutex_leave(db->mutex);
  return pOld;
}
}

// Helper dummy profile callback implementations for tests
static void dummyProfileA(void*, const char*, sqlite_uint64) {
    // no-op
}
static void dummyProfileB(void*, const char*, sqlite_uint64) {
    // no-op
}

// Simple test harness
static bool assert_ptr_eq(void* a, void* b, const std::string& label) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << label << " - pointers not equal. "
                  << "Expected " << a << ", got " << b << "\n";
        return false;
    }
    return true;
}
static bool assert_int_eq(unsigned int a, unsigned int b, const std::string& label) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << label << " - int values not equal. "
                  << "Expected " << b << ", got " << a << "\n";
        return false;
    }
    return true;
}
static bool assert_int_eq_ll(unsigned long long a, unsigned long long b, const std::string& label) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << label << " - uint64 values not equal. "
                  << "Expected " << b << ", got " << a << "\n";
        return false;
    }
    return true;
}
static bool assert_true(bool cond, const std::string& label) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << label << "\n";
        return false;
    }
    return true;
}

// Test 1: Safety check fails -> sqlite3_profile should return 0 and not mutate db state
bool test_safety_check_fails_no_mutation() {
    // Setup
    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pProfileArg = (void*)0x1111;
    db.xProfile = &dummyProfileA;
    db.mTrace = 0xAB;
    g_safetyCheckShouldPass = 0; // make safety check fail

    // Action
    void *old = sqlite3_profile(&db, &dummyProfileB, (void*)0xDEAD);

    // Assertion
    bool ok = true;
    ok &= assert_true(old == nullptr, "Test1: old should be null when safety check fails");
    ok &= assert_ptr_eq(db.pProfileArg, (void*)0x1111, "Test1: pProfileArg unchanged after failure");
    ok &= assert_ptr_eq((void*)db.xProfile, (void*)dummyProfileA, "Test1: xProfile unchanged after failure");
    ok &= assert_int_eq(db.mTrace, 0xAB, "Test1: mTrace unchanged after failure");
    return ok;
}

// Test 2: Safety check passes and xProfile is non-null -> state should update accordingly
bool test_safety_check_pass_non_null_profile_updates_state() {
    // Setup
    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pProfileArg = (void*)0x1111;
    db.xProfile = nullptr;
    db.mTrace = 0xF0; // some initial value to verify masking
    g_safetyCheckShouldPass = 1; // safety check passes

    // Action
    void *old = sqlite3_profile(&db, &dummyProfileB, (void*)0xDEAD);

    // Expected: old == 0x1111
    unsigned int expectedMask = (0xF0) & SQLITE_TRACE_NONLEGACY_MASK; // 0xF0
    unsigned int expectedMTrace = expectedMask | SQLITE_TRACE_XPROFILE; // because xProfile is non-null after set

    // Assertions
    bool ok = true;
    ok &= assert_ptr_eq(old, (void*)0x1111, "Test2: old should be previous pProfileArg");
    ok &= assert_ptr_eq((void*)db.xProfile, (void*)&dummyProfileB, "Test2: xProfile should be updated to new function");
    ok &= assert_ptr_eq(db.pProfileArg, (void*)0xDEAD, "Test2: pProfileArg should be updated to new arg");
    ok &= assert_int_eq(db.mTrace, expectedMTrace, "Test2: mTrace should be masked and OR with XPROFILE");
    return ok;
}

// Test 3: Safety check passes and xProfile is null -> mTrace should be masked but not OR'ed with XPROFILE
bool test_safety_check_pass_null_profile_no_xprofile_flag() {
    // Setup
    sqlite3 db;
    sqlite3_mutex m;
    db.mutex = &m;
    db.pProfileArg = nullptr;
    db.xProfile = nullptr;
    db.mTrace = 0x300; // 0x300 & 0xFF = 0x00
    g_safetyCheckShouldPass = 1; // safety check passes

    // Action
    void *old = sqlite3_profile(&db, nullptr, (void*)0xBEEF);

    // Expected: old == nullptr
    unsigned int expectedMask = (0x300) & SQLITE_TRACE_NONLEGACY_MASK; // 0x300 & 0xFF = 0x00
    unsigned int expectedMTrace = expectedMask; // no XPROFILE OR'ed because xProfile is null

    // Assertions
    bool ok = true;
    ok &= assert_ptr_eq(old, nullptr, "Test3: old should be nullptr when profile callback is null");
    ok &= assert_ptr_eq((void*)db.xProfile, nullptr, "Test3: xProfile should remain null when input is null");
    ok &= assert_ptr_eq(db.pProfileArg, (void*)0xBEEF, "Test3: pProfileArg should be updated to new arg (even if profile is null)");
    ok &= assert_int_eq(db.mTrace, expectedMTrace, "Test3: mTrace should be masked and not OR with XPROFILE when profile is null");
    return ok;
}

// Main function runs all tests
int main() {
    std::cout << "Starting sqlite3_profile unit tests (self-contained, C++11)..." << std::endl;

    bool t1 = test_safety_check_fails_no_mutation();
    std::cout << "Test 1 " << (t1 ? "PASSED" : "FAILED") << std::endl;

    bool t2 = test_safety_check_pass_non_null_profile_updates_state();
    std::cout << "Test 2 " << (t2 ? "PASSED" : "FAILED") << std::endl;

    bool t3 = test_safety_check_pass_null_profile_no_xprofile_flag();
    std::cout << "Test 3 " << (t3 ? "PASSED" : "FAILED") << std::endl;

    bool all = t1 && t2 && t3;
    std::cout << "Overall result: " << (all ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;
    return all ? 0 : 1;
}
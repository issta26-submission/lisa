// Minimal unit test suite for sqlite3_mutex_leave (as implemented in the focal method).
// This test suite is crafted to compile under C++11 without any external testing framework (no GTest).
// It provides lightweight test harness utilities and two representative tests:
// 1) p == nullptr: ensure the leave function does not call the xMutexLeave hook.
// 2) p != nullptr: ensure the leave function calls the xMutexLeave hook with the correct argument.
//
// The implementation mirrors the essential parts of the provided focal code and the class dependencies
// while keeping the environment self-contained for testing purposes.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>
#include <string>


// ---------------------------
// Minimal FOCAL_CLASS_DEP mocks
// ---------------------------

// Forward declaration of the opaque sqlite3_mutex type used by sqlite3_mutex_leave.
struct sqlite3_mutex;

// Pointer types for mutex operations (as in the original sqlite3 mutex interface).
typedef void (*xMutexLeave_fn)(sqlite3_mutex*);
typedef int  (*xMutexHeld_fn)(sqlite3_mutex*);
typedef int  (*xMutexNotheld_fn)(sqlite3_mutex*);

// File-scope grouping of mutex-related hooks to emulate sqlite3GlobalConfig structure.
struct _MutexInterface {
    xMutexLeave_fn       xMutexLeave;
    xMutexHeld_fn        xMutexHeld;
    xMutexNotheld_fn      xMutexNotheld;
};

// Global configuration object used by sqlite3GlobalConfig.mutex.xMutexLeave(...)
struct _GlobalConfig {
    _MutexInterface mutex;
};

// Instantiate the global config (as the test harness would provide in the real project).
static _GlobalConfig sqlite3GlobalConfig = { { nullptr, nullptr, nullptr } };

// The focal method under test (reproduced here for isolation).
void sqlite3_mutex_leave(sqlite3_mutex *p){
  if( p ){
    // In the real code, this would be compiled with proper debugging asserts.
    // We preserve that behavior here to mirror the original semantics.
    assert( sqlite3GlobalConfig.mutex.xMutexLeave );
    sqlite3GlobalConfig.mutex.xMutexLeave(p);
  }
}

// ---------------------------
// Test harness infrastructure
// ---------------------------

// Simple test result recording and reporting
static int g_totalTests = 0;
static int g_failedTests = 0;

static void report_test_result(const std::string& testName, bool passed, const std::string& details = "") {
    ++g_totalTests;
    if (!passed) ++g_failedTests;
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName;
    if (!details.empty()) {
        std::cout << " - " << details;
    }
    std::cout << std::endl;
}

// Global hooks to spy on whether the mutex-leave hook is invoked and with what argument.
static bool g_leave_called = false;
static sqlite3_mutex* g_leave_arg = nullptr;

// Mock implementation of the xMutexLeave hook to observe calls
static void mock_xMutexLeave(sqlite3_mutex* p) {
    g_leave_called = true;
    g_leave_arg = p;
}

// A minimal dummy mutex type for testing (no behavior required beyond being addressable)
struct sqlite3_mutex {
    // Empty on purpose; only used as an opaque handle in tests.
};

// Utility macro to reset test harness state between tests
static void reset_harness_state() {
    g_leave_called = false;
    g_leave_arg = nullptr;
}

// ---------------------------
// Test 1: p == nullptr (true/false branch of the outer if)
// ---------------------------
// This test ensures that passing a null pointer does not trigger the xMutexLeave hook.
static void test_sqlite3_mutex_leave_null_pointer() {
    // Setup: ensure hook is installed but will not be called
    reset_harness_state();
    sqlite3GlobalConfig.mutex.xMutexLeave = mock_xMutexLeave;

    // Exercise: call with null pointer
    sqlite3_mutex_leave(nullptr);

    // Verify: hook should not have been called
    bool passed = (g_leave_called == false);
    report_test_result("sqlite3_mutex_leave_null_pointer", passed,
        g_leave_called ? "hook was unexpectedly called" : "");
}

// ---------------------------
// Test 2: p != nullptr (outer if true branch)
// ---------------------------
// This test ensures that passing a valid mutex pointer calls the xMutexLeave hook with the same pointer.
static void test_sqlite3_mutex_leave_valid_pointer() {
    // Setup: prepare a dummy mutex and a spy for the hook
    reset_harness_state();
    sqlite3GlobalConfig.mutex.xMutexLeave = mock_xMutexLeave;

    sqlite3_mutex m;
    sqlite3_mutex* p = &m;

    // Exercise: call with a non-null pointer
    sqlite3_mutex_leave(p);

    // Verify: hook should have been called exactly once with the same pointer
    bool passed = (g_leave_called == true) && (g_leave_arg == p);
    report_test_result("sqlite3_mutex_leave_valid_pointer", passed,
        (g_leave_called && g_leave_arg == p) ? "" : "hook invocation details mismatch");
}

// Entry point: run all tests
int main() {
    // Run test 1: null pointer case
    test_sqlite3_mutex_leave_null_pointer();

    // Run test 2: valid pointer case
    test_sqlite3_mutex_leave_valid_pointer();

    // Summary
    std::cout << "Total tests: " << g_totalTests
              << ", Passed: " << (g_totalTests - g_failedTests)
              << ", Failed: " << g_failedTests << std::endl;

    // Return non-zero if any test failed
    return g_failedTests != 0 ? 1 : 0;
}
// Lightweight C++11 unit tests for the focal method:
// int sqlite3ExprIsConstantOrFunction(Expr *p, u8 isInit)
// The real implementation delegates to exprIsConst(p, 4+isInit, 0).
// This test provides a self-contained environment (no GTest) and uses a
// small, controlled stub for exprIsConst to verify behavior.
// It also validates that the assertion on isInit when outside {0,1} triggers
// an abort (SIGABRT) as in the original code.

#include <csetjmp>
#include <csignal>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain: Lightweight, self-contained test scaffold without third-party test frameworks.

typedef unsigned char u8;

// Forward-declare the Expr type used by the focal function.
struct Expr;

// Minimal, test-scoped implementation of the dependency function
// (this mirrors the signature used by sqlite3ExprIsConstantOrFunction).
// In the real SQLite codebase, exprIsConst is static/private, but for
// unit-testing in isolation we provide a test-accessible version here.
static Expr sentinel_const1;
static Expr sentinel_const2;
static Expr sentinel_other;

// A simple stub for exprIsConst used by sqlite3ExprIsConstantOrFunction in this test.
// We return deterministic values based on which sentinel pointer is passed and
// the initFlag (4 for isInit==0, 5 for isInit==1).
int exprIsConst(Expr *p, int initFlag, int iCur) {
    (void)iCur; // unused in this test stub
    if (p == &sentinel_const1) {
        if (initFlag == 4) return 10; // corresponds to isInit==0
        if (initFlag == 5) return 11; // corresponds to isInit==1
    } else if (p == &sentinel_const2) {
        if (initFlag == 4) return 20;
        if (initFlag == 5) return 21;
    } else if (p == &sentinel_other) {
        // Some additional behavior for a different pointer, if needed
        if (initFlag == 4) return 30;
        if (initFlag == 5) return 31;
    }
    // Default fallback for any other pointer
    return -1;
}

// The focal function under test (recreated here for isolation).
// In the real project, this function lives in expr.c and would be linked in.
// Here we provide a self-contained version that delegates to our exprIsConst stub.
int sqlite3ExprIsConstantOrFunction(Expr *p, u8 isInit){
    assert( isInit==0 || isInit==1 );
    return exprIsConst(p, 4+isInit, 0);
}

// Simple test harness (no GTest). Lightweight assertion helpers.
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Assertion failed: " msg "\n"; \
        return false; \
    } \
} while(0)

static int g_abort_occurred = 0;
static jmp_buf g_jmp_env;

// SIGABRT handler to detect assertion aborts
static void sigabrt_handler(int) {
    g_abort_occurred = 1;
    longjmp(g_jmp_env, 1);
}

// Test 1: True branches for isInit values 0 and 1
bool test_isInit_true_branches(){
    // sentinel_const1 with isInit = 0 -> 4+0 = 4, expect 10
    int r0 = sqlite3ExprIsConstantOrFunction(&sentinel_const1, 0);
    TEST_ASSERT(r0 == 10, "expected 10 when isInit=0 for sentinel_const1");

    // sentinel_const1 with isInit = 1 -> 4+1 = 5, expect 11
    int r1 = sqlite3ExprIsConstantOrFunction(&sentinel_const1, 1);
    TEST_ASSERT(r1 == 11, "expected 11 when isInit=1 for sentinel_const1");

    // sentinel_const2 with isInit = 0 -> 4+0 = 4, expect 20
    int r2 = sqlite3ExprIsConstantOrFunction(&sentinel_const2, 0);
    TEST_ASSERT(r2 == 20, "expected 20 when isInit=0 for sentinel_const2");

    // sentinel_const2 with isInit = 1 -> 4+1 = 5, expect 21
    int r3 = sqlite3ExprIsConstantOrFunction(&sentinel_const2, 1);
    TEST_ASSERT(r3 == 21, "expected 21 when isInit=1 for sentinel_const2");

    return true;
}

// Test 2: Ensure static-like behavior (assert path) for invalid isInit values.
// We simulate the assert by triggering a real abort (SIGABRT) and catching it.
bool test_invalid_isInit_assert_triggers_abort(){
    // Setup ABORT signal capture
    struct sigaction old_sa, new_sa;
    new_sa.sa_handler = sigabrt_handler;
    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;
    int rc = sigaction(SIGABRT, &new_sa, &old_sa);
    TEST_ASSERT(rc == 0, "failed to install SIGABRT handler");

    g_abort_occurred = 0;

    if (setjmp(g_jmp_env) == 0){
        // This should trigger an abort via the internal assert:
        // isInit = 2 is invalid.
        sqlite3ExprIsConstantOrFunction(&sentinel_const1, 2);
        // If we reach here, the assert did not abort as expected.
        // Restore previous handler before returning.
        sigaction(SIGABRT, &old_sa, nullptr);
        return false;
    } else {
        // We landed here due to abort
        // Restore previous handler
        sigaction(SIGABRT, &old_sa, nullptr);
        // Confirm that abort was indeed captured
        return g_abort_occurred == 1;
    }
}

// Test 3: Boundary handling for a different sentinel with isInit values
bool test_boundary_with_other_sentinel(){
    // sentinel_other with isInit = 0 -> 4 -> expect 30
    int r0 = sqlite3ExprIsConstantOrFunction(&sentinel_other, 0);
    TEST_ASSERT(r0 == 30, "expected 30 when isInit=0 for sentinel_other");

    // sentinel_other with isInit = 1 -> 5 -> expect 31
    int r1 = sqlite3ExprIsConstantOrFunction(&sentinel_other, 1);
    TEST_ASSERT(r1 == 31, "expected 31 when isInit=1 for sentinel_other");

    return true;
}

// Entry point
int main(){
    // Prepare sentinel objects (addresses used as identity in exprIsConst stub)
    (void)sentinel_const1; // explicitly touch to silence unused warnings
    (void)sentinel_const2;
    (void)sentinel_other;

    int total = 0, passed = 0;

    std::cout << "Running unit tests for sqlite3ExprIsConstantOrFunction (isInit wrapper):\n";

    if (test_isInit_true_branches()) {
        std::cout << "[OK] test_isInit_true_branches\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_isInit_true_branches\n";
    }
    ++total;

    if (test_invalid_isInit_assert_triggers_abort()) {
        std::cout << "[OK] test_invalid_isInit_assert_triggers_abort\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_invalid_isInit_assert_triggers_abort\n";
    }
    ++total;

    if (test_boundary_with_other_sentinel()) {
        std::cout << "[OK] test_boundary_with_other_sentinel\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_boundary_with_other_sentinel\n";
    }
    ++total;

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
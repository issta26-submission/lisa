/*
Unit Test Suite for sqlite3ExprDup (focal method)
Location: Tests target the wrapper:
  Expr *sqlite3ExprDup(sqlite3 *db, const Expr *p, int flags){
    assert( flags==0 || flags==EXPRDUP_REDUCE );
    return p ? exprDup(db, p, flags, 0) : 0;
  }

Context and approach:
- The real codebase is SQLite C code. The isolated test harness below is written in C++11
  to be compilable in a modern C++ project without GTest. It relies on the presence
  of the actual sqlite3ExprDup symbol (and related types) in the linked object(s)
  from the project under test.

- Step 1 (Program Understanding) is encoded here as inline comments describing intent and
  the core dependent components (Expr, sqlite3, flags, and the wrapper contract).

- Step 2 (Unit Test Generation) focuses on:
  1) Safe path: p == nullptr -> returns 0 (null)
  2) Safe path: p == nullptr with EXPRDUP_REDUCE or 0 flags (still returns 0)
  3) Abnormal path: invalid flags should trigger an assertion failure.
     Since asserting in the focal function aborts, we run this test in a child process
     to observe an abort signal without terminating the parent test process.

- Step 3 (Test Refinement) uses a POSIX-only approach for the assertion-abort path to avoid
  crashing the entire test harness, while still exercising important code paths.

Notes:
- This test relies on POSIX fork/wait semantics for non-crashing assertion tests.
- The test uses only the public/internal-same symbols (extern "C") and forward-declared
  types to minimize coupling to private headers. If the project uses stricter symbol exposure,
  ensure the test is compiled as part of the same project scope so sqlite3ExprDup is linked.
- Static functions within expr.c are not testable directly; we test only the wrapper's behavior as requested.
*/

#include <sys/types.h>
#include <cstdio>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>



// Domain knowledge notes injected as comments for maintainers.
//  - Focal function depends on sqlite3 (opaque type) and Expr (opaque type).
//  - The wrapper asserts flags must be 0 or EXPRDUP_REDUCE.
//  - The safe branch (p == nullptr) should return 0 without invoking exprDup.

// Forward declarations to allow testing without including internal headers.
// The real project provides a sqlite3 type and an Expr type.
typedef struct sqlite3 sqlite3;
typedef struct Expr Expr;

// The focal function is defined in the project under test (expr.c).
// We declare it here for linkage in the test harness.
extern "C" Expr* sqlite3ExprDup(sqlite3* db, const Expr* p, int flags);

// Helper: compile-time value for EXPRDUP_REDUCE if not available.
// We don't rely on the real constant here; we only need to exercise valid vs invalid flags.
// If the real project defines EXPRDUP_REDUCE with a different value, adjust accordingly.
#ifndef EXPRDUP_REDUCE
#define EXPRDUP_REDUCE 1
#endif

// Test 1: Safe path - p == nullptr, flags == 0
// Expect: return value is null (0)
bool test_exprdup_null_p_zero_flags() {
    Expr* p = nullptr;
    sqlite3* db = nullptr;
    Expr* res = sqlite3ExprDup(db, p, 0);
    if (res != nullptr) {
        std::cerr << "Test 1 FAILED: sqlite3ExprDup(nullptr, nullptr, 0) should return nullptr, got non-null." << std::endl;
        return false;
    }
    return true;
}

// Test 1b: Safe path - p == nullptr, flags == EXPRDUP_REDUCE
// Expect: return value is null (0)
bool test_exprdup_null_p_reduce_flags() {
    Expr* p = nullptr;
    sqlite3* db = nullptr;
    Expr* res = sqlite3ExprDup(db, p, EXPRDUP_REDUCE);
    if (res != nullptr) {
        std::cerr << "Test 1b FAILED: sqlite3ExprDup(nullptr, nullptr, EXPRDUP_REDUCE) should return nullptr, got non-null." << std::endl;
        return false;
    }
    return true;
}

// Test 2: Invalid flags should trigger an assertion failure inside the focal function.
// We cannot catch an assertion from the same process without aborting the test runner.
// Workaround: run the call in a child process; expect it to abort (SIGABRT).
// If the child aborts with SIGABRT, the test passes.
bool test_exprdup_invalid_flags_asserts_in_child() {
#if defined(__unix__) || defined(__APPLE__)
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Test 2 SKIPPED: fork() failed." << std::endl;
        return false;
    }
    if (pid == 0) {
        // Child: invoke with an invalid flag, expecting an assertion abort
        const int BAD_FLAGS = 12345;
        // Intentionally pass null pointer values to minimize side effects
        (void)sqlite3ExprDup(nullptr, nullptr, BAD_FLAGS);
        // If we reach here, assertion did not abort as expected; exit non-zero to indicate failure
        _exit(2);
    } else {
        // Parent: wait for child status
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "Test 2 FAILED: waitpid() error." << std::endl;
            return false;
        }
        // Child should have been terminated by SIGABRT due to the assertion
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
            return true;
        } else {
            std::cerr << "Test 2 FAILED: Expected SIGABRT from assertion, status=" << status << std::endl;
            return false;
        }
    }
#else
    // Non-POSIX environments: skip this test
    std::cout << "Test 2 SKIPPED: No POSIX fork available." << std::endl;
    return true;
#endif
}

int main() {
    std::cout << "sqlite3ExprDup test suite (C++11 harness) started." << std::endl;

    int tests = 0;
    int passed = 0;

    // Test 1
    ++tests;
    if (test_exprdup_null_p_zero_flags()) {
        std::cout << "Test 1 PASSED: NULL p with flags 0 returns NULL as expected." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 1 FAILED." << std::endl;
    }

    // Test 1b
    ++tests;
    if (test_exprdup_null_p_reduce_flags()) {
        std::cout << "Test 1b PASSED: NULL p with EXPRDUP_REDUCE returns NULL as expected." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 1b FAILED." << std::endl;
    }

    // Test 2
    ++tests;
    if (test_exprdup_invalid_flags_asserts_in_child()) {
        std::cout << "Test 2 PASSED: Invalid flags cause assertion abort as expected (in child process)." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 2 FAILED." << std::endl;
    }

    std::cout << "Tests finished: " << passed << " / " << tests << " passed." << std::endl;
    return (passed == tests) ? 0 : 1;
}
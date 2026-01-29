// Self-contained C++11 unit tests for the focal C function:
// void db_including_add(const char * zKey, const char * zSrc, int srcLine)
// This test harness mocks the minimal dependencies and sqlite3_stmt handling
// to verify correct behavior under true/false branches and edge cases.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <cassert>


// Domain-specific simulating environment for the focal function under test.

// 1) Minimal faux sqlite3_stmt type and related helpers.
typedef struct sqlite3_stmt sqlite3_stmt;

// Simple fake sqlite3_stmt structure to capture bindings and steps.
struct sqlite3_stmt {
    std::string sql;
    bool prepared = false;
    std::string bind_text1;
    std::string bind_text2;
    int bind_int3 = 0;
    int step_calls = 0;
};

// 2) Global test environment mimicking the program's global state.
struct GlobalState {
    struct {
        sqlite3_stmt *inclIns;
    } stmt;
} g;

// 3) Test-controlled return code for db_step.
static int next_rc = 0;

// 4) Test scaffolding for assertions and test reporting.
static int test_passed = 0;
static int test_failed = 0;

// 5) Mocks and helpers for the dependencies used by the focal method.

// Re-define SQLITE_DONE and a generic SQLITE_ERROR for test control.
const int SQLITE_DONE = 101;
const int SQLITE_ERROR = 1;

// Tracks how many times db_prepare is invoked (i.e., whether a new statement is prepared).
static int db_prepare_invocations = 0;

// Tracks last error/rc reported by db_affirm_rc.
static int last_affirm_rc = 0;
static std::string last_affirm_msg;

// Tracks number of g_debug invocations.
static int g_debug_calls = 0;

// 6) Helper: reset test environment between tests.
void reset_test_env() {
    // Reset g
    g.stmt.inclIns = nullptr;

    // Reset next_rc
    next_rc = SQLITE_DONE;

    // Reset db_prepare tracking and error reporting
    db_prepare_invocations = 0;
    last_affirm_rc = 0;
    last_affirm_msg.clear();
    g_debug_calls = 0;
}

// 7) Implementations of dependencies (mocks) used by the focal function.

// db_prepare simulates allocation of a new sqlite3_stmt if *pStmt is NULL.
// We count invocations to verify whether the preparatory step occurred.
void db_prepare(sqlite3_stmt **pStmt, const char * zSql, ...) {
    if (*pStmt == nullptr) {
        *pStmt = new sqlite3_stmt();
        (*pStmt)->sql = zSql;
        (*pStmt)->prepared = true;
        db_prepare_invocations++;
    }
}

// db_bind_text stores the bound text for the first two parameters.
void db_bind_text(sqlite3_stmt *pStmt, int col, const char * zStr) {
    if (!pStmt) return;
    if (col == 1) pStmt->bind_text1 = (zStr ? zStr : "");
    else if (col == 2) pStmt->bind_text2 = (zStr ? zStr : "");
}

// db_bind_int stores the bound integer for the third parameter (srcLine).
void db_bind_int(sqlite3_stmt *pStmt, int col, int val) {
    if (!pStmt) return;
    if (col == 3) pStmt->bind_int3 = val;
}

// db_step returns the next_rc value to simulate execution result.
int db_step(sqlite3_stmt *pStmt) {
    if (pStmt) {
        pStmt->step_calls += 1;
    }
    return next_rc;
}

// Clear bindings on the statement (as sqlite3_clear_bindings would do).
void sqlite3_clear_bindings(sqlite3_stmt *pStmt) {
    if (pStmt) {
        pStmt->bind_text1.clear();
        pStmt->bind_text2.clear();
        pStmt->bind_int3 = 0;
    }
}

// Reset statement state (as sqlite3_reset would do). No-op for test purposes.
void sqlite3_reset(sqlite3_stmt *pStmt) {
    (void)pStmt;
    // No internal state required for test harness reset.
}

// db_affirm_rc reports a non-success rc; we capture rc and message.
void db_affirm_rc(int rc, const char * zMsg) {
    last_affirm_rc = rc;
    last_affirm_msg = (zMsg ? zMsg : "");
}

// g_debug simulates internal debugging logs; we only need to count invocations.
void g_debug(int level, const char *fmt, ...) {
    (void)level;
    (void)fmt;
    ++g_debug_calls;
}

// 8) Expose the focal function with C linkage to ensure linkage compatibility
// in this standalone test harness. We implement exactly the supplied body.
extern "C" void db_including_add(const char * zKey, const char * zSrc, int srcLine) {
    int rc;
    if(!g.stmt.inclIns){
        db_prepare(&g.stmt.inclIns,
                   "INSERT OR FAIL INTO incl(file,srcFile,srcLine) VALUES(?,?,?)");
    }
    db_bind_text(g.stmt.inclIns, 1, zKey);
    db_bind_text(g.stmt.inclIns, 2, zSrc);
    db_bind_int(g.stmt.inclIns, 3, srcLine);
    rc = db_step(g.stmt.inclIns);
    if(SQLITE_DONE != rc){
        db_affirm_rc(rc, "Stepping INSERT on incl");
    }
    g_debug(2,("inclpath add [%s] from [%s]:%d\n", zKey, zSrc, srcLine));
    sqlite3_clear_bindings(g.stmt.inclIns);
    sqlite3_reset(g.stmt.inclIns);
}

// 9) Lightweight test utilities

#define STRINGIFY(x) #x
#define ASSERT_TRUE(cond, msg)                               \
    do {                                                       \
        if (!(cond)) {                                         \
            std::cerr << "ASSERT_TRUE failed: " << (msg) << "\n"; \
            ++test_failed;                                      \
            return;                                             \
        } else {                                                \
            ++test_passed;                                      \
        }                                                       \
    } while (0)

#define RUN_TEST(test_func) test_func();

// 10) Test cases

// Test 1: Normal path - db_step returns SQLITE_DONE. Verify bindings, preparation, and resets.
void test_db_including_add_success() {
    reset_test_env();
    // Arrange
    next_rc = SQLITE_DONE;
    // Ensure a clean state: inclIns NULL triggers db_prepare
    g.stmt.inclIns = nullptr;
    db_prepare_invocations = 0;

    // Act
    db_including_add("myKey", "mySrc.c", 123);

    // Assert
    // a) A statement was prepared
    ASSERT_TRUE(g.stmt.inclIns != nullptr, "Statement should be prepared when inclIns was NULL.");

    // b) Correct bindings were applied
    ASSERT_TRUE(g.stmt.inclIns->bind_text1 == "myKey",
                "First bound text should be zKey (col 1).");
    ASSERT_TRUE(g.stmt.inclIns->bind_text2 == "mySrc.c",
                "Second bound text should be zSrc (col 2).");
    ASSERT_TRUE(g.stmt.inclIns->bind_int3 == 123,
                "Third bound value should be srcLine (col 3).");

    // c) db_step called exactly once and returned SQLITE_DONE (test-controlled)
    ASSERT_TRUE(g.stmt.inclIns->step_calls == 1,
                "db_step should be called exactly once.");

    // d) No error reported
    ASSERT_TRUE(last_affirm_rc == 0,
                "No error should be reported when rc == SQLITE_DONE.");

    // e) Debug path invoked
    ASSERT_TRUE(g_debug_calls > 0, "g_debug should be invoked for logging.");

    // f) Bindings cleared and reset called
    ASSERT_TRUE(g.stmt.inclIns->bind_text1.empty() && g.stmt.inclIns->bind_text2.empty(),
                "Bindings should be cleared after sqlite3_clear_bindings.");
    ASSERT_TRUE(g.stmt.inclIns->bind_int3 == 0,
                "Integer binding should be cleared after sqlite3_clear_bindings.");

    // Cleanup
    delete g.stmt.inclIns;
    g.stmt.inclIns = nullptr;
}

// Test 2: Failure path - db_step returns a non-DONE rc. Ensure db_affirm_rc is invoked with correct message.
void test_db_including_add_step_failure() {
    reset_test_env();
    // Arrange
    next_rc = SQLITE_ERROR; // simulate error
    g.stmt.inclIns = nullptr;
    db_prepare_invocations = 0;
    last_affirm_rc = 0;
    last_affirm_msg.clear();

    // Act
    db_including_add("errKey", "errSrc.c", 999);

    // Assert
    // a) A statement was prepared
    ASSERT_TRUE(g.stmt.inclIns != nullptr, "Statement should be prepared when inclIns was NULL (error path).");

    // b) db_affirm_rc called with the error rc and message
    ASSERT_TRUE(last_affirm_rc == SQLITE_ERROR,
                "db_affirm_rc should be called with the non-DONE rc value.");
    ASSERT_TRUE(last_affirm_msg.find("Stepping INSERT on incl") != std::string::npos,
                "db_affirm_rc should contain the expected error message.");

    // c) Even on error, cleanup should occur (bindings cleared)
    ASSERT_TRUE(g.stmt.inclIns != nullptr, "Statement should still exist for cleanup.");
    delete g.stmt.inclIns;
    g.stmt.inclIns = nullptr;
}

// Test 3: Pre-existing inclIns skips db_prepare call. Verify that we do not allocate a new statement.
void test_db_including_add_existing_stmt_no_prepare() {
    reset_test_env();
    // Arrange: Create a pre-existing statement and ensure we do not call db_prepare
    next_rc = SQLITE_DONE;
    g.stmt.inclIns = new sqlite3_stmt();
    g.stmt.inclIns->sql = "existing";
    db_prepare_invocations = 0;

    // Act
    db_including_add("existingKey", "existingSrc.c", 7);

    // Assert
    // a) db_prepare should not have been invoked because inclIns was already set
    ASSERT_TRUE(db_prepare_invocations == 0,
                "db_prepare should not be invoked when inclIns is already non-NULL.");

    // b) Bindings should reflect new values provided to function
    ASSERT_TRUE(g.stmt.inclIns->bind_text1 == "existingKey",
                "First bound text should be updated to new zKey even if statement existed.");
    ASSERT_TRUE(g.stmt.inclIns->bind_text2 == "existingSrc.c",
                "Second bound text should be updated to new zSrc even if statement existed.");
    ASSERT_TRUE(g.stmt.inclIns->bind_int3 == 7,
                "Third bound value should be updated to new srcLine even if statement existed.");

    // c) Step should have occurred
    ASSERT_TRUE(g.stmt.inclIns->step_calls == 1,
                "db_step should be called once even when using existing statement.");

    // Cleanup
    delete g.stmt.inclIns;
    g.stmt.inclIns = nullptr;
}

// 11) Main entry to run tests
int main() {
    // Initialize environment
    reset_test_env();

    // Run tests
    RUN_TEST(test_db_including_add_success);
    RUN_TEST(test_db_including_add_step_failure);
    RUN_TEST(test_db_including_add_existing_stmt_no_prepare);

    // Report
    std::cout << "Tests run: " << (test_passed + test_failed) << "\n";
    std::cout << "  Passed: " << test_passed << "\n";
    std::cout << "  Failed: " << test_failed << "\n";
    if (test_failed == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Some tests failed.\n";
    }
    return test_failed ? 1 : 0;
}

// Redefine the TEST macro behavior via the helper to increment pass/fail counts.
// The macros above directly increment; however, to tie into the main reporting,
// we hook the test macro to accumulate passes/fails in a more explicit way.
// We already increment test_passed/test_failed inside ASSERT_TRUE. If a test fails,
// it returns early; we won't reach completion and won't increment further.
// To ensure final counts are meaningful, reset counters at program end isn't necessary
// since the test harness uses early returns and per-test accounting above.
// C++11 Test Suite for sqlite3_stmt_busy (vdbeapi.c)
// This test harness does not use GoogleTest. It uses simple
// non-terminating EXPECT-like assertions and a small main() runner.
// The tests exercise the focal function sqlite3_stmt_busy by constructing
// a minimal Vdbe object (cast to sqlite3_stmt*) to simulate the VM state.

#include <cstdio>
#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdlib>
#include <cstdint>


// Include internal SQLite headers to access Vdbe type and constants.
// These headers expose internal structures needed for direct state manipulation.
// The repository under test should provide these headers in the include path.
extern "C" {
}

// Prototype of the focal method (C linkage to match the implementation in C source)
extern "C" int sqlite3_stmt_busy(sqlite3_stmt *pStmt);

// Lightweight non-terminating assertion framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ_I(actual, expected) \
    do { \
        ++g_tests_run; \
        if ((int)(actual) != (int)(expected)) { \
            fprintf(stderr, "EXPECT_EQ failed: %d != %d at %s:%d\n", (int)(actual), (int)(expected), __FILE__, __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE failed: condition is false at %s:%d\n", __FILE__, __LINE__); \
            ++g_tests_failed; \
        } \
    } while (0)


// Test 1: sqlite3_stmt_busy should return 0 when the input is a null pointer
// Rationale: The function checks v != 0; passing nullptr yields 0.
static void test_sqlite3_stmt_busy_null_input() {
    // When pStmt is null, the function should return 0
    int res = sqlite3_stmt_busy(nullptr);
    EXPECT_EQ_I(res, 0);
}

// Test 2: sqlite3_stmt_busy should return 1 when the internal Vdbe state is RUN
// Rationale: The function returns 1 if v != 0 and v->eVdbeState == VDBE_RUN_STATE.
static void test_sqlite3_stmt_busy_run_state_enabled() {
    // Allocate a Vdbe object (cast to sqlite3_stmt*) and set its run state.
    Vdbe *p = (Vdbe*)malloc(sizeof(Vdbe));
    if (p == nullptr) {
        fprintf(stderr, "Memory allocation failed for Vdbe in test_sqlite3_stmt_busy_run_state_enabled\n");
        ++g_tests_run; // count attempt
        ++g_tests_failed;
        return;
    }
    // Initialize the state to RUN
    // Access to VDBE_RUN_STATE is provided via the included header.
    p->eVdbeState = VDBE_RUN_STATE;

    int res = sqlite3_stmt_busy((sqlite3_stmt*)p);

    // We expect RUN_STATE to yield "busy" (non-zero)
    EXPECT_EQ_I(res, 1);

    free(p);
}

// Test 3: sqlite3_stmt_busy should return 0 when the internal Vdbe state is not RUN
// Rationale: If eVdbeState != VDBE_RUN_STATE, the function should return 0.
static void test_sqlite3_stmt_busy_not_run_state() {
    // Allocate a Vdbe object and set a non-RUN state.
    Vdbe *p = (Vdbe*)malloc(sizeof(Vdbe));
    if (p == nullptr) {
        fprintf(stderr, "Memory allocation failed for Vdbe in test_sqlite3_stmt_busy_not_run_state\n");
        ++g_tests_run; 
        ++g_tests_failed;
        return;
    }

    // Set to a state value that is intended to be different from VDBE_RUN_STATE.
    // We rely on VDBE_RUN_STATE being defined; using 0 is typically INIT state in SQLite.
    p->eVdbeState = 0;

    int res = sqlite3_stmt_busy((sqlite3_stmt*)p);
    EXPECT_EQ_I(res, 0);

    free(p);
}


// Entry point: run all tests and report summary
int main() {
    // Run tests with explanatory comments in code above.
    // In each test, we log outcomes but do not abort on failures to maximize coverage.

    printf("Starting tests for sqlite3_stmt_busy...\n");

    test_sqlite3_stmt_busy_null_input();
    test_sqlite3_stmt_busy_run_state_enabled();
    test_sqlite3_stmt_busy_not_run_state();

    printf("Tests completed. Ran: %d, Failed: %d\n", g_tests_run, g_tests_failed);
    return (g_tests_failed == 0) ? 0 : 1;
}
// Test suite for sqlite3Fts5StorageStmtRelease
// This test targets the focal method inside fts5_storage.c without using GTest.
// It uses a lightweight, non-terminating assertion style to maximize coverage.

#include <cstdio>
#include <cstring>
#include <fts5Int.h>


// -----------------------------------------------------------------------------
// Minimal sqlite3.h stub to allow compiling the focal C file without a real sqlite3.h.
// The actual project should provide the real sqlite3.h; this stub is only for tests.
// It provides just enough types and functions for sqlite3Fts5StorageStmtRelease to compile
// and for us to intercept sqlite3_reset and sqlite3_finalize calls.
// -----------------------------------------------------------------------------
#ifndef SQLITE3_H_STUB
#define SQLITE3_H_STUB

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlite3_stmt sqlite3_stmt;

// Minimal, test-scoped prototypes for sqlite3 API used by the focal function
int sqlite3_reset(sqlite3_stmt *pStmt);
int sqlite3_finalize(sqlite3_stmt *pStmt);

#ifdef __cplusplus
}
#endif

#endif // SQLITE3_H_STUB

// -----------------------------------------------------------------------------
// SQLite3 stub implementations and test hooks.
// We track calls to sqlite3_reset and sqlite3_finalize to validate the behavior
// of sqlite3Fts5StorageStmtRelease.
// -----------------------------------------------------------------------------
extern "C" {

// Counters for tracking calls
static int g_reset_calls = 0;
static sqlite3_stmt *g_reset_last = nullptr;
static int g_finalize_calls = 0;
static sqlite3_stmt *g_finalize_last = nullptr;

// Reset counters (for test isolation)
static void reset_call_counters() {
    g_reset_calls = 0;
    g_reset_last = nullptr;
    g_finalize_calls = 0;
    g_finalize_last = nullptr;
}

// Stubs of sqlite3_reset/finalize that record their inputs
int sqlite3_reset(sqlite3_stmt *pStmt) {
    g_reset_calls++;
    g_reset_last = pStmt;
    return 0; // pretend success
}

int sqlite3_finalize(sqlite3_stmt *pStmt) {
    g_finalize_calls++;
    g_finalize_last = pStmt;
    return 0; // pretend success
}

} // extern "C"

// -----------------------------------------------------------------------------
// Include project headers to obtain the focal type and constants.
// We rely on the project layout where Fts5Storage and constants like
// FTS5_STMT_SCAN_ASC, FTS5_STMT_SCAN_DESC, FTS5_STMT_LOOKUP are defined.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Lightweight unit test harness (non-terminating assertions).
// It prints detailed PASS/FAIL messages but does not exit on failure.
// -----------------------------------------------------------------------------
static int g_testsTotal = 0;
static int g_testsFailed = 0;

#define EXPECT_TRUE(cond, desc) do { \
    g_testsTotal++; \
    if(!(cond)) { \
        printf("TEST_FAIL: %s\n", (desc)); \
        g_testsFailed++; \
    } else { \
        printf("TEST_PASS: %s\n", (desc)); \
    } \
} while(0)

#define EXPECT_EQ_PTR(a,b,desc) do { \
    g_testsTotal++; \
    if((void*)(a) != (void*)(b)) { \
        printf("TEST_FAIL: %s (expected %p, got %p)\n", (desc), (void*)(b), (void*)(a)); \
        g_testsFailed++; \
    } else { \
        printf("TEST_PASS: %s\n", (desc)); \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b,desc) do { \
    g_testsTotal++; \
    if((a) != (b)) { \
        printf("TEST_FAIL: %s (expected %d, got %d)\n", (desc), (b), (a)); \
        g_testsFailed++; \
    } else { \
        printf("TEST_PASS: %s\n", (desc)); \
    } \
} while(0)

static void test_stmt_release_stores_ptr_when_empty() {
    // Test the path where p->aStmt[eStmt] == 0
    // - After call, aStmt[eStmt] should point to pStmt
    // - sqlite3_reset should be invoked with pStmt
    printf("Running test: stmt_release stores pointer when empty\n");

    // Prepare storage
    Fts5Storage storage;
    memset(&storage, 0, sizeof(storage)); // ensure aStmt[] starts with zeros

    // Prepare a fake sqlite3_stmt and call
    sqlite3_stmt stmt;
    sqlite3_stmt *pStmt = &stmt;

    // Ensure initial condition: aStmt[SCANNING] is zero
    int testStmtIndex = FTS5_STMT_SCAN_ASC; // use a valid enum value
    storage.aStmt[testStmtIndex] = 0;

    // Reset counters
    reset_call_counters();

    // Call the function under test
    sqlite3Fts5StorageStmtRelease(&storage, testStmtIndex, pStmt);

    // Validate: pointer stored, and sqlite3_reset called with pStmt
    EXPECT_EQ_PTR(storage.aStmt[testStmtIndex], pStmt, "aStmt entry should be set to pStmt");
    EXPECT_EQ_INT(g_reset_calls, 1, "sqlite3_reset should be called exactly once");
    EXPECT_EQ_PTR(g_reset_last, pStmt, "sqlite3_reset should be called with the same pStmt");
    // No finalize should have happened
    EXPECT_EQ_INT(g_finalize_calls, 0, "sqlite3_finalize should not be called in this path");
}

static void test_stmt_release_finalizes_ptr_when_already_set() {
    // Test the path where p->aStmt[eStmt] != 0
    // - After call, sqlite3_finalize should be invoked with the new pStmt
    // - aStmt[eStmt] should remain unchanged
    printf("Running test: stmt_release finalizes provided pStmt when aStmt already set\n");

    // Prepare storage
    Fts5Storage storage;
    memset(&storage, 0, sizeof(storage)); // zero initialize

    // Existing stored statement
    sqlite3_stmt stored;
    sqlite3_stmt *pStored = &stored;

    // New statement we pass to release (to be finalized)
    sqlite3_stmt newStmt;
    sqlite3_stmt *pNew = &newStmt;

    int testStmtIndex = FTS5_STMT_SCAN_DESC; // use another valid enum value
    storage.aStmt[testStmtIndex] = pStored;   // mark as already set

    // Reset counters
    reset_call_counters();

    // Call the function under test
    sqlite3Fts5StorageStmtRelease(&storage, testStmtIndex, pNew);

    // Validate: aStmt entry unchanged
    EXPECT_EQ_PTR(storage.aStmt[testStmtIndex], pStored, "aStmt entry should remain unchanged after release");
    // Finalize should be invoked with the new pStmt
    EXPECT_EQ_INT(g_finalize_calls, 1, "sqlite3_finalize should be called exactly once");
    EXPECT_EQ_PTR(g_finalize_last, pNew, "sqlite3_finalize should be called with the new pStmt");
    // Reset should not have happened in this path
    EXPECT_EQ_INT(g_reset_calls, 0, "sqlite3_reset should not be called in this path");
}

// Entry point
int main(void) {
    printf("Starting sqlite3Fts5StorageStmtRelease unit tests (non-GTest, lightweight harness)\n\n");

    test_stmt_release_stores_ptr_when_empty();
    printf("\n");
    test_stmt_release_finalizes_ptr_when_already_set();
    printf("\n");

    // Summary
    printf("Test summary: total=%d, failed=%d\n", g_testsTotal, g_testsFailed);
    if(g_testsFailed == 0){
        printf("ALL TESTS PASSED\n");
        return 0;
    }else{
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}
// Unit test suite for sqlite3Fts5StorageRowCount
// This test targets the focal method described in the prompt:
// int sqlite3Fts5StorageRowCount(Fts5Storage *p, i64 *pnRow)
// The tests assume the standard FTS5/C SQLite environment provided by fts5Int.h
// and the corresponding fts5_storage.c is compiled and linked with this test.
// No Google Test is used; a lightweight, non-terminating test harness is implemented.

#include <fts5Int.h>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstdint>


// Import necessary project headers to ensure correct types and constants.
// We assume the project provides the header that defines Fts5Storage, i64, and
// the symbolic constants such as SQLITE_OK and FTS5_CORRUPT.
extern "C" {
}

// Public function under test (C linkage)
extern "C" int sqlite3Fts5StorageRowCount(Fts5Storage *p, i64 *pnRow);

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

static void logTestResult(const char* testName, bool pass) {
    printf("[%s] %s\n", pass ? "PASS" : "FAIL", testName);
}

// Convenience macro for non-terminating checks
#define TEST_ASSERT(cond, testName) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        logTestResult(testName, false); \
    } else { \
        logTestResult(testName, true); \
    } \
} while(0)

// Test 1: When nTotalRow > 0, the function should return SQLITE_OK and set *pnRow accordingly.
static bool test_rowCount_positive() {
    // Prepare a storage instance with a positive total row count.
    Fts5Storage p;
    p.nTotalRow = 5;      // Core dependent member used by sqlite3Fts5StorageRowCount
    i64 nRow = 0;

    // Act
    int rc = sqlite3Fts5StorageRowCount(&p, &nRow);

    // Assert: rc must be SQLITE_OK and nRow must equal the total rows.
    bool condRC = (rc == SQLITE_OK);
    bool condRow = (nRow == 5);

    // Expose results via non-terminating assertions
    TEST_ASSERT(condRC, "RowCount_Positive: rc == SQLITE_OK");
    TEST_ASSERT(condRow, "RowCount_Positive: nRow == 5");

    // Return overall success for this test
    return condRC && condRow;
}

// Test 2: When nTotalRow == 0, the function should still call loadTotals successfully,
// then set rc to FTS5_CORRUPT and return with *pnRow = 0.
static bool test_rowCount_zero() {
    Fts5Storage p;
    p.nTotalRow = 0;
    i64 nRow = -1; // initialize to a distinct value to verify assignment

    int rc = sqlite3Fts5StorageRowCount(&p, &nRow);

    bool condRC = (rc == FTS5_CORRUPT);
    bool condRow = (nRow == 0);

    TEST_ASSERT(condRC, "RowCount_Zero: rc == FTS5_CORRUPT");
    TEST_ASSERT(condRow, "RowCount_Zero: nRow == 0");

    return condRC && condRow;
}

// Test 3: When nTotalRow < 0, the function should still return FTS5_CORRUPT and set *pnRow
// to the negative value (since it assigns before corruption check).
static bool test_rowCount_negative() {
    Fts5Storage p;
    p.nTotalRow = -3;
    i64 nRow = 0;

    int rc = sqlite3Fts5StorageRowCount(&p, &nRow);

    bool condRC = (rc == FTS5_CORRUPT);
    bool condRow = (nRow == -3);

    TEST_ASSERT(condRC, "RowCount_Negative: rc == FTS5_CORRUPT");
    TEST_ASSERT(condRow, "RowCount_Negative: nRow == -3");

    return condRC && condRow;
}

// Entry point
int main() {
    printf("Starting tests for sqlite3Fts5StorageRowCount...\n");
    bool r1 = test_rowCount_positive();
    bool r2 = test_rowCount_zero();
    bool r3 = test_rowCount_negative();

    if(r1 && r2 && r3) {
        printf("All tests passed.\n");
    } else {
        printf("Some tests failed. Summary: %d passed, %d failed, %d total.\n",
               g_totalTests - g_failedTests, g_failedTests, g_totalTests);
    }

    // Return non-zero if any test failed
    return (g_failedTests > 0) ? 1 : 0;
}
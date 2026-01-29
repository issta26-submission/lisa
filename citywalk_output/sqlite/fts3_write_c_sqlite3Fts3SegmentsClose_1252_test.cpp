// Minimal C++11 test harness for sqlite3Fts3SegmentsClose
// This test is self-contained and provides a small mock of the C interfaces
// to validate the focal method behavior: closing the blob and nullifying the pointer.
//
// Note: In the real project, sqlite3Fts3SegmentsClose is defined in fts3_write.c
// and operates on a real Fts3Table with a SQLite blob. For unit testing in this
// environment, we provide a lightweight, self-contained replica of the needed
// pieces to exercise the logic without requiring the full SQLite build.
//
// The tests intentionally use non-terminating assertions (they log failures but
// continue executing) to maximize code coverage within a single run.

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <iostream>


// -------------------------------------------------------------------------------------
// Mocked/types to support the focal function in a standalone test environment
// -------------------------------------------------------------------------------------

// Forward declare the blob type (opaque in the real library)
typedef struct sqlite3_blob sqlite3_blob;

// Lightweight Fts3Table structure with the first-level dependency used by the focal method.
// In the real code, Fts3Table is a larger struct defined in the FTS3 module.
// For the unit test, we only need the pSegments member to exercise sqlite3Fts3SegmentsClose.
struct Fts3Table {
    sqlite3_blob* pSegments;
};

// Global counters to observe calls to the mocked sqlite3_blob_close
static int g_blob_close_calls = 0;
static sqlite3_blob* g_last_blob_closed = nullptr;

// Mock implementation of sqlite3_blob_close to observe interactions.
// The real function returns void; this mock increments a counter and records the argument.
extern "C" void sqlite3_blob_close(sqlite3_blob* p) {
    ++g_blob_close_calls;
    g_last_blob_closed = p;
}

// The focal function under test (replicated here for a self-contained test).
// In the real project, this is defined in fts3_write.c and linked from the library.
// We include a faithful replica to ensure the unit tests exercise the intended logic.
extern "C" void sqlite3Fts3SegmentsClose(Fts3Table *p) {
    sqlite3_blob_close(p->pSegments);
    p->pSegments = 0;
}

// -------------------------------------------------------------------------------------
// Simple test framework (non-terminating assertions)
// -------------------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Expectation failed: " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "Expectation failed: " << (msg) \
                  << " - Expected: " << (a) << "  Got: " << (b) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

static void test_Sqlite3SegmentsClose_WithNonNullSegments() {
    // This test corresponds to validating the true/normal branch when pSegments is non-null.
    // Setup: a dummy blob object is assigned to pSegments.
    // Action: sqlite3Fts3SegmentsClose should call sqlite3_blob_close exactly once
    //         and then set pSegments to null.
    std::cout << "Test 1: sqlite3Fts3SegmentsClose with non-null pSegments" << std::endl;

    // Reset global trackers
    g_blob_close_calls = 0;
    g_last_blob_closed = nullptr;

    // Prepare test objects
    sqlite3_blob dummyBlob;        // fake blob instance
    Fts3Table table;
    table.pSegments = &dummyBlob;

    // Invoke the focal method
    sqlite3Fts3SegmentsClose(&table);

    // Assertions
    EXPECT_EQ(g_blob_close_calls, 1, "sqlite3_blob_close should be called exactly once for non-null segments");
    EXPECT_EQ(g_last_blob_closed, &dummyBlob, "sqlite3_blob_close should be called with the original blob pointer");
    EXPECT_TRUE(table.pSegments == nullptr, "pSegments should be set to null after sqlite3Fts3SegmentsClose");
}

static void test_Sqlite3SegmentsClose_WithNullSegments() {
    // This test corresponds to validating behavior when pSegments is null.
    // The code path should still call sqlite3_blob_close (with a null pointer in this mock)
    // and then set pSegments to null (remain null).
    std::cout << "Test 2: sqlite3Fts3SegmentsClose with null pSegments" << std::endl;

    // Reset global trackers
    g_blob_close_calls = 0;
    g_last_blob_closed = nullptr;

    Fts3Table table;
    table.pSegments = nullptr;

    // Invoke the focal method
    sqlite3Fts3SegmentsClose(&table);

    // Assertions
    EXPECT_EQ(g_blob_close_calls, 1, "sqlite3_blob_close should be called even when segments is null (in mock)");
    EXPECT_EQ(g_last_blob_closed, nullptr, "sqlite3_blob_close should be called with nullptr when segments is null");
    EXPECT_TRUE(table.pSegments == nullptr, "pSegments should remain null after sqlite3Fts3SegmentsClose");
}

static void test_Sqlite3SegmentsClose_MultipleCalls() {
    // This test ensures repeated calls behave consistently:
    // - First call closes the existing blob and nullifies pSegments
    // - Second call, with pSegments already null, should still call sqlite3_blob_close
    //   (as per the implementation that directly calls the close function with current pSegments)
    std::cout << "Test 3: sqlite3Fts3SegmentsClose with multiple successive calls" << std::endl;

    // Reset global trackers
    g_blob_close_calls = 0;
    g_last_blob_closed = nullptr;

    sqlite3_blob blobA;
    Fts3Table table;
    table.pSegments = &blobA;

    // First call: should close blobA
    sqlite3Fts3SegmentsClose(&table);
    EXPECT_EQ(g_blob_close_calls, 1, "First call should close the initial blob once");
    EXPECT_EQ(g_last_blob_closed, &blobA, "First call should pass blobA to sqlite3_blob_close");
    EXPECT_TRUE(table.pSegments == nullptr, "pSegments should be null after first close");

    // Second call: pSegments is null now; should still invoke sqlite3_blob_close once more with nullptr
    sqlite3Fts3SegmentsClose(&table);
    EXPECT_EQ(g_blob_close_calls, 2, "Second call should still invoke sqlite3_blob_close (with nullptr)");
    EXPECT_EQ(g_last_blob_closed, nullptr, "Second call should pass nullptr to sqlite3_blob_close");
    EXPECT_TRUE(table.pSegments == nullptr, "pSegments should remain null after second close");
}

// -------------------------------------------------------------------------------------
// Main entry point
// -------------------------------------------------------------------------------------

int main() {
    std::cout << "Starting unit tests for sqlite3Fts3SegmentsClose (self-contained mock)" << std::endl;

    test_Sqlite3SegmentsClose_WithNonNullSegments();
    test_Sqlite3SegmentsClose_WithNullSegments();
    test_Sqlite3SegmentsClose_MultipleCalls();

    std::cout << "Unit tests completed. Total: " << g_total_tests
              << "  Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests != 0) ? 1 : 0;
}
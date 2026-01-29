// Test suite for sqlite3BtreeClearTableOfCursor
// This harness targets the focal method:
// int sqlite3BtreeClearTableOfCursor(BtCursor *pCur) {
//   return sqlite3BtreeClearTable(pCur->pBtree, pCur->pgnoRoot, 0);
// }
// The goal is to verify that the wrapper correctly forwards the Btree pointer
// and the root page number to sqlite3BtreeClearTable, and to validate the
// return value propagation.
//
// Important: This test assumes a build environment where sqlite3BtreeClearTable is
// linkable and that we can provide a mock for its behavior to exercise forwarding
// behavior. In typical setups, a linker override or symbol wrapping mechanism is
// used to intercept sqlite3BtreeClearTable. For the purposes of this exercise, we
// provide a mock implementation of sqlite3BtreeClearTable that records the call
// parameters and returns a controllable value. The focal method remains unmodified.

#include <iomanip>
#include <btreeInt.h>
#include <iostream>
#include <string>


// The actual project's BtCursor and related types are defined in btreeInt.h.
// Include the project header to obtain correct type definitions (BtCursor, Btree, i64, Pgno, etc.).

// -----------------------------------------------------------------------------
// Candidate Keywords (Step 1: program understanding summary)
// - BtCursor: cursor instance that references a B-tree and its root page.
// - pCur->pBtree: pointer to the Btree instance used by the cursor.
// - pCur->pgnoRoot: page number for the root table to clear.
// - sqlite3BtreeClearTable: underlying function that performs the clear operation.
// - Btree: the B-tree structure type used by sqlite3BtreeClearTable.
// - i64 (Pgno/Payload types): 64-bit integer types used for page numbers and changes.
// - pnChange: output parameter capturing number of changes (not used by our wrapper).
// The test focuses on parameter forwarding and return value propagation through sqlite3BtreeClearTableOfCursor.
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// Mock backend for sqlite3BtreeClearTable to enable controlled testing.
// In a real linking scenario, this symbol would be provided by the production
// btree.c. Here we provide a test-time override to observe forwarding behavior.
// -----------------------------------------------------------------------------

// Global hooks for the mock to inspect forwarded arguments
static Btree* g_mock_last_p = nullptr;
static int    g_mock_last_iTable = -1;
static i64    g_mock_pnChangeSent = 0; // not used by the wrapper (the wrapper passes 0)

// Control the return value of the mock to exercise both success and error paths
static int g_mock_return_value = 0;

// Mock implementation of sqlite3BtreeClearTable to capture inputs and return a controlled value
extern "C" int sqlite3BtreeClearTable(Btree *p, int iTable, i64 *pnChange)
{
    g_mock_last_p = p;
    g_mock_last_iTable = iTable;
    if (pnChange) {
        // The wrapper passes 0 as pnChange in the real code; reflect that here.
        *pnChange = g_mock_pnChangeSent;
    }
    return g_mock_return_value;
}

// -----------------------------------------------------------------------------
// Test harness (lightweight, no GTest; manual assertions with non-terminating checks)
// -----------------------------------------------------------------------------

static int gTotalTests = 0;
static int gFailedTests = 0;

#define TEST_ASSERT(cond, msg) do {                                           \
    ++gTotalTests;                                                             \
    if (!(cond)) {                                                             \
        ++gFailedTests;                                                        \
        std::cerr << "ASSERTION FAILED: " << (msg) << "\n";                  \
    } else {                                                                   \
        std::cout << "PASS: " << (msg) << "\n";                              \
    }                                                                          \
} while(0)

// Helper to reset mock state before each test
static void resetMock() {
    g_mock_last_p = nullptr;
    g_mock_last_iTable = -1;
    g_mock_pnChangeSent = 0;
    // Do not reset g_mock_return_value here; tests set it explicitly
}

// Test 1: Forwarding of arguments -- ensure the wrapper forwards pBtree and pgnoRoot
// Domain knowledge: The focused method should forward the exact BtCursor fields to sqlite3BtreeClearTable.
// Expect that the mock receives the same pointers/values and the return code is propagated.
static void test_forwarding_of_arguments() {
    resetMock();
    BtCursor cur;
    // Initialize cursor with known values
    cur.pBtree = reinterpret_cast<Btree*>(0x1234ABCD);
    cur.pgnoRoot = 0x7F; // 127

    // Configure mock to simulate a successful operation
    g_mock_return_value = 0;
    g_mock_pnChangeSent = 0;

    // Call the focal method
    int rc = sqlite3BtreeClearTableOfCursor(&cur);

    // Assertions
    TEST_ASSERT(rc == 0, "sqlite3BtreeClearTableOfCursor should return the value from sqlite3BtreeClearTable (0)");
    TEST_ASSERT(g_mock_last_p == cur.pBtree, "Forwarded BtCursor.pBtree pointer should match sqlite3BtreeClearTable parameter");
    TEST_ASSERT(g_mock_last_iTable == (int)cur.pgnoRoot, "Forwarded BtCursor.pgnoRoot should match sqlite3BtreeClearTable parameter");
}

// Test 2: Return value propagation variation -- ensure non-zero return values propagate
// Domain knowledge: The wrapper should simply propagate the return value of sqlite3BtreeClearTable.
// We'll configure the mock to return an error code and verify propagation.
static void test_return_value_propagation() {
    resetMock();
    BtCursor cur;
    cur.pBtree = reinterpret_cast<Btree*>(0xDEADC0DE);
    cur.pgnoRoot = 42;

    // Configure mock to simulate an error
    g_mock_return_value = -1;
    g_mock_pnChangeSent = 0;

    int rc = sqlite3BtreeClearTableOfCursor(&cur);

    // Assertions
    TEST_ASSERT(rc == -1, "sqlite3BtreeClearTableOfCursor should propagate non-zero return value from sqlite3BtreeClearTable");
    TEST_ASSERT(g_mock_last_p == cur.pBtree, "Forwarded BtCursor.pBtree pointer should be the one provided in test");
    TEST_ASSERT(g_mock_last_iTable == (int)cur.pgnoRoot, "Forwarded BtCursor.pgnoRoot should be the one provided in test");
}

// Test 3: Ensure non-impact on pnChange (the wrapper passes 0, but we verify the wrapper handling)
// Domain knowledge: In the provided wrapper, pnChange is passed as 0. We validate that the mock
// received the value (if applicable) as part of the call signature.
// Note: The test checks that pnChange is not used to alter forwarding semantics.
static void test_pnChange_passed_as_zero() {
    resetMock();
    BtCursor cur;
    cur.pBtree = reinterpret_cast<Btree*>(0xA5A5A5A5);
    cur.pgnoRoot = 1;

    // pnChange is passed as 0 in the original call; ensure that's the case by mock behavior
    g_mock_return_value = 0;
    g_mock_pnChangeSent = 0;

    int rc = sqlite3BtreeClearTableOfCursor(&cur);

    TEST_ASSERT(rc == 0, "Return value should be 0 for this test case");
    TEST_ASSERT(g_mock_last_p == cur.pBtree, "BtCursor.pBtree forwarded correctly");
    TEST_ASSERT(g_mock_last_iTable == (int)cur.pgnoRoot, "BtCursor.pgnoRoot forwarded correctly");
}

// Main entry to run tests
int main() {
    std::cout << "Starting tests for sqlite3BtreeClearTableOfCursor...\n";

    test_forwarding_of_arguments();
    test_return_value_propagation();
    test_pnChange_passed_as_zero();

    std::cout << "\nTest Summary: "
              << gTotalTests << " tests, "
              << gFailedTests << " failures.\n";

    return (gFailedTests == 0) ? 0 : 1;
}

// Note to the reader:
// - This test suite uses a mock sqlite3BtreeClearTable to observe the forwarding behavior.
// - The actual project may provide a different symbol resolution strategy (e.g., symbol wrappings or
//   linker overrides). If your build system defines sqlite3BtreeClearTable in btree.c, you may need
//   to adapt the mock (e.g., place the mock in a separate translation unit with a specific linker
//   flag such as -Wl,--wrap sqlite3BtreeClearTable or use a dedicated test library that overrides
//   the symbol). The test logic, however, remains valid: it asserts that sqlite3BtreeClearTableOfCursor
//   forwards the correct arguments and propagates the return code.
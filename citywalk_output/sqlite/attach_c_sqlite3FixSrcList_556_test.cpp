// Unit test suite for sqlite3FixSrcList in attach.c
// Target: C++11 compatible tests without GoogleTest.
// Approach: Provide a lightweight, self-contained test harness that mocks
// sqlite3WalkSelect to observe how sqlite3FixSrcList interacts with its inputs.
// Assumes the project environment provides sqliteInt.h (internal SQLite types)
// so that DbFixer, SrcList, Select, and Walker are visible.
// The tests use extern "C" linkage for the sqlite3FixSrcList and the mocked
// sqlite3WalkSelect to reflect the exact C linkage of the production code.

#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Include SQLite internal definitions. Path may vary by project layout.
// Ensure this path is correct for your test environment.

// Mock control variables for the sqlite3WalkSelect mock.
static int g_mockWalkReturnValue = 0;
static int g_walkCallCount = 0;
static SrcList *g_lastPassedSrcList = nullptr;

// Forward declaration of the production function under test.
// Use C linkage to match the production code.
// The function signature is exactly as in the focal snippet.
extern "C" int sqlite3FixSrcList(DbFixer *pFix, SrcList *pList);

// Mock implementation of sqlite3WalkSelect to observe interactions.
// This overrides the actual implementation (in a test-only build) by
// providing a symbol with the same name. Ensure your build does not link
// against the real sqlite3WalkSelect to avoid multiple definitions.
extern "C" int sqlite3WalkSelect(Walker *pWalker, Select *pSelect) {
    // Record that the function was called and capture the input src list.
    ++g_walkCallCount;
    g_lastPassedSrcList = (pSelect != nullptr) ? pSelect->pSrc : nullptr;
    return g_mockWalkReturnValue;
}

// Lightweight test harness helpers
static int tests_run = 0;
static int tests_failed = 0;

// Simple non-terminating assertion for tests: logs on failure but continues.
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "TEST FAILURE: %s\n", msg); \
        ++tests_failed; \
    } \
    ++tests_run; \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::fprintf(stderr, "TEST FAILURE: %s (expected %ld, got %ld)\n", msg, (long)(b), (long)(a)); \
        ++tests_failed; \
    } \
    ++tests_run; \
} while(0)

// Test 1: When pList is null, sqlite3FixSrcList should return 0 and not call sqlite3WalkSelect.
void test_sqlite3FixSrcList_nullList() {
    // Arrange
    DbFixer fixer;
    std::memset(&fixer, 0, sizeof(DbFixer)); // zero-initialize to be safe
    SrcList *pList = nullptr;

    // Reset mock state
    g_walkCallCount = 0;
    g_lastPassedSrcList = nullptr;
    g_mockWalkReturnValue = 0;

    // Act
    int res = sqlite3FixSrcList(&fixer, pList);

    // Assert
    EXPECT_EQ(res, 0, "sqlite3FixSrcList should return 0 when pList is null");
    EXPECT_EQ(g_walkCallCount, 0, "sqlite3WalkSelect should not be called when pList is null");
    EXPECT_TRUE(g_lastPassedSrcList == nullptr, "No SrcList should be passed to sqlite3WalkSelect when pList is null");
}

// Test 2: When pList is non-null, sqlite3FixSrcList should prepare a Select with pSrc set to pList
// and call sqlite3WalkSelect, returning the mocked value.
void test_sqlite3FixSrcList_nonNullList_callsWalk() {
    // Arrange
    DbFixer fixer;
    std::memset(&fixer, 0, sizeof(DbFixer)); // zero-initialize
    SrcList listInstance;
    SrcList *pList = &listInstance;

    // Prepare mock behavior
    g_walkCallCount = 0;
    g_lastPassedSrcList = nullptr;
    g_mockWalkReturnValue = 123; // arbitrary non-zero return value to verify propagation

    // Act
    int res = sqlite3FixSrcList(&fixer, pList);

    // Assert
    EXPECT_TRUE(g_walkCallCount == 1, "sqlite3WalkSelect should be called exactly once when pList is non-null");
    EXPECT_EQ((long)g_lastPassedSrcList, (long)pList, "sqlite3WalkSelect should receive a Select with pSrc == pList");
    EXPECT_EQ(res, 123, "sqlite3FixSrcList should propagate sqlite3WalkSelect's return value");
}

// Test 3: Ensure that changing the mock return value affects the final result accordingly.
void test_sqlite3FixSrcList_returnValuePropagation() {
    // Arrange
    DbFixer fixer;
    std::memset(&fixer, 0, sizeof(DbFixer)); // zero-initialize
    SrcList someList;
    SrcList *pList = &someList;

    g_walkCallCount = 0;
    g_lastPassedSrcList = nullptr;

    // First, return value = 7
    g_mockWalkReturnValue = 7;
    int res1 = sqlite3FixSrcList(&fixer, pList);
    EXPECT_EQ(res1, 7, "Return value should propagate when mock returns 7");

    // Then, return value = -3
    g_mockWalkReturnValue = -3;
    int res2 = sqlite3FixSrcList(&fixer, pList);
    EXPECT_EQ(res2, -3, "Return value should propagate when mock returns -3");
}

// Entry point to run tests
int main() {
    // Informative header
    std::puts("Running sqlite3FixSrcList unit tests (mocked sqlite3WalkSelect)...");
    test_sqlite3FixSrcList_nullList();
    test_sqlite3FixSrcList_nonNullList_callsWalk();
    test_sqlite3FixSrcList_returnValuePropagation();

    // Summary
    std::fprintf(stdout, "\nTests run: %d, Failures: %d\n", tests_run, tests_failed);
    if(tests_failed > 0) {
        std::fprintf(stderr, "One or more tests failed.\n");
        return 2;
    }
    return 0;
}
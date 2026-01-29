/*
Unit Test Suite for sqlite3BtreePrevious (C/C++11 compatible)

Overview:
- This test harness re-implements a minimal, self-contained version of the focal method
  sqlite3BtreePrevious and its minimal dependencies, to enable isolated unit testing
  without bringing in the entire SQLite btree source tree.
- The tests verify both branches of the conditional in sqlite3BtreePrevious:
  1) The "btreePrevious" path is taken when eState != CURSOR_VALID, ix == 0, or pPage->leaf == 0.
  2) The "else" path is taken when the above conditions are not met (i.e., CURSOR_VALID, ix > 0, leaf != 0).
- To avoid aborts from assertions in the focal method, the test harness mocks:
  - cursorOwnsBtShared(...) -> always true
  - btreePrevious(...) -> records invocation and returns a configurable value
- The test suite uses a lightweight, non-terminating assertion mechanism to maximize code coverage.

Candidate Keywords (Step 1 rationale):
- BtCursor pCur, MemPage pPage
- pCur->curFlags, BTCF_AtLast, BTCF_ValidOvfl, BTCF_ValidNKey
- pCur->info.nSize
- pCur->eState, CURSOR_VALID
- pCur->ix, pCur->pPage->leaf
- sqlite3BtreePrevious, btreePrevious
- cursorOwnsBtShared(...) and UNUSED_PARAMETER(...)
- SQLITE_OK (0)
- Flags domain: 0 or 1
- Branch coverage: eState check, ix check, leaf check

Notes about static members/dependencies:
- The test is self-contained and does not rely on external static functions from the original SQLite sources. It provides minimal placeholders for static/internal pieces used by sqlite3BtreePrevious (btreePrevious and cursorOwnsBtShared).

Usage:
- Compile with a C++11 compliant compiler (e.g., g++, clang++).
- Run the resulting executable. It will print test results and a summary.

Below is the single-file test harness:

*/

#include <btreeInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-like constants and helpers, mimicking the minimal subset used by the focal method.

#define CURSOR_VALID 1

// Bitmasks for cursor flags (BTCF_*)
const int BTCF_AtLast   = 0x01;
const int BTCF_ValidOvfl  = 0x02;
const int BTCF_ValidNKey  = 0x04;

const int SQLITE_OK = 0;

#define UNUSED_PARAMETER(x) (void)(x)

// Minimal in-test data structures to simulate the environment of sqlite3BtreePrevious

struct MemPage {
    int leaf; // 1 if leaf page, 0 otherwise
};

struct BtCursor {
    int curFlags;              // flag bits
    struct {
        int nSize;               // payload size indicator
    } info;
    int eState;                  // cursor state
    int ix;                      // index into current page cells
    MemPage *pPage;              // current page
};

// Globals to simulate behavior of dependent routines

static int g_btreePrevCalled = 0;
static int g_btreePrevReturnValue = SQLITE_OK;

// Mocked dependency: cursorOwnsBtShared
extern "C" int cursorOwnsBtShared(BtCursor *pCur){
    // In tests, always report ownership to satisfy asserts
    (void)pCur;
    return 1;
}

// Mocked dependency: btreePrevious (part of the "true" branch of sqlite3BtreePrevious)
static int btreePrevious(BtCursor *pCur){
    (void)pCur;
    g_btreePrevCalled = 1;
    return g_btreePrevReturnValue;
}

// The focal method under test (reproduced in-test for isolated unit testing)
extern "C" int sqlite3BtreePrevious(BtCursor *pCur, int flags){
    assert( cursorOwnsBtShared(pCur) );
    assert( flags==0 || flags==1 );
    UNUSED_PARAMETER( flags );  /* Used in COMDB2 but not native SQLite */
    pCur->curFlags &= ~(BTCF_AtLast|BTCF_ValidOvfl|BTCF_ValidNKey);
    pCur->info.nSize = 0;
    if( pCur->eState!=CURSOR_VALID
     || pCur->ix==0
     || pCur->pPage->leaf==0
    ){
      return btreePrevious(pCur);
    }
    pCur->ix--;
    return SQLITE_OK;
}

// Lightweight non-terminating test framework

static int testCount = 0;
static int failCount = 0;

#define TEST_BEGIN(name) void test_##name()
#define TEST_END

#define ASSERT_TRUE(cond) \
    do { \
        ++testCount; \
        if(!(cond)) { \
            ++failCount; \
            std::cerr << "Test " << testCount << " FAILED: " << #cond << "\n"; \
        } \
    } while(0)

#define ASSERT_INT_EQ(a, b) \
    do { \
        ++testCount; \
        if( (int)(a) != (int)(b) ) { \
            ++failCount; \
            std::cerr << "Test " << testCount << " FAILED: " \
                      << #a << " == " << #b << " -> " \
                      << (int)(a) << " != " << (int)(b) << "\n"; \
        } \
    } while(0)

#define ASSERT_PTR_EQ(a,b) \
    do { \
        ++testCount; \
        if((void*)(a) != (void*)(b)) { \
            ++failCount; \
            std::cerr << "Test " << testCount << " FAILED: Pointer mismatch: " \
                      << #a << " != " << #b << "\n"; \
        } \
    } while(0)


// Test cases

// Test A: Branch taken when eState != CURSOR_VALID (true for first predicate)
TEST_BEGIN(A_branch_on_invalid_state)
{
    BtCursor cur;
    MemPage page;
    int result;

    // Initialize page and cursor
    page.leaf = 1;
    cur.pPage = &page;
    cur.curFlags = BTCF_AtLast | BTCF_ValidOvfl | BTCF_ValidNKey;
    cur.info.nSize = 10;
    cur.eState = 0; // not CURSOR_VALID
    cur.ix = 5;

    g_btreePrevCalled = 0;
    g_btreePrevReturnValue = 123;

    result = sqlite3BtreePrevious(&cur, 0);

    // Assertions
    ASSERT_INT_EQ(result, 123); // Should call btreePrevious and return its value
    ASSERT_TRUE(g_btreePrevCalled == 1); // btreePrevious must have been invoked
    int clearedFlags = BTCF_AtLast | BTCF_ValidOvfl | BTCF_ValidNKey;
    ASSERT_TRUE((cur.curFlags & clearedFlags) == 0); // Bits cleared
    ASSERT_INT_EQ(cur.info.nSize, 0);                 // nSize reset to 0
}
TEST_END

// Test B: Branch taken when ix == 0 (second predicate)
TEST_BEGIN(B_branch_on_zero_ix)
{
    BtCursor cur;
    MemPage page;
    int result;

    page.leaf = 1;
    cur.pPage = &page;
    cur.curFlags = 0;
    cur.info.nSize = 55;
    cur.eState = CURSOR_VALID;
    cur.ix = 0; // triggers ix==0

    g_btreePrevCalled = 0;
    g_btreePrevReturnValue = 999;

    result = sqlite3BtreePrevious(&cur, 0);

    // Assertions
    ASSERT_INT_EQ(result, 999);     // Should call btreePrevious
    ASSERT_TRUE(g_btreePrevCalled == 1);
    ASSERT_INT_EQ(cur.ix, 0);       // ix should remain unchanged
    ASSERT_INT_EQ(cur.info.nSize, 0); // nSize reset to 0
}
TEST_END

// Test C: Branch taken when leaf == 0 (third predicate)
TEST_BEGIN(C_branch_on_leaf_zero)
{
    BtCursor cur;
    MemPage page;
    int result;

    page.leaf = 0; // leaf == 0 triggers branch
    cur.pPage = &page;
    cur.curFlags = 0;
    cur.info.nSize = 77;
    cur.eState = CURSOR_VALID;
    cur.ix = 3;

    g_btreePrevCalled = 0;
    g_btreePrevReturnValue = 888;

    result = sqlite3BtreePrevious(&cur, 0);

    // Assertions
    ASSERT_INT_EQ(result, 888);     // Should call btreePrevious
    ASSERT_TRUE(g_btreePrevCalled == 1);
    ASSERT_INT_EQ(cur.ix, 3);       // ix unchanged
    ASSERT_INT_EQ(cur.info.nSize, 0); // nSize reset to 0
}
TEST_END

// Test D: Else branch (CURSOR_VALID, ix > 0, leaf != 0)
TEST_BEGIN(D_else_branch_no_btreenext)
{
    BtCursor cur;
    MemPage page;
    int result;

    page.leaf = 1;
    cur.pPage = &page;
    cur.curFlags = 0;
    cur.info.nSize = 42;
    cur.eState = CURSOR_VALID;
    cur.ix = 3; // > 0 and leaf != 0

    g_btreePrevCalled = 0;           // should NOT be called in this path
    g_btreePrevReturnValue = 777;    // value should be ignored

    result = sqlite3BtreePrevious(&cur, 0);

    // Assertions
    ASSERT_INT_EQ(result, SQLITE_OK); // Should return OK without calling btreePrevious
    ASSERT_TRUE(g_btreePrevCalled == 0); // btreePrevious must not be invoked
    ASSERT_INT_EQ(cur.ix, 2);           // ix decremented
    ASSERT_INT_EQ(cur.info.nSize, 0);   // nSize reset to 0
}
TEST_END

// Driver: Run all tests and report summary
int main() {
    std::cout << "Starting sqlite3BtreePrevious unit tests (self-contained mock)\n";

    test_A_branch_on_invalid_state();
    test_B_branch_on_zero_ix();
    test_C_branch_on_leaf_zero();
    test_D_else_branch_no_btreenext();

    std::cout << "Tests run: " << testCount << ", Failures: " << failCount << "\n";
    if(failCount > 0){
        std::cout << "Some tests failed. See details above.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
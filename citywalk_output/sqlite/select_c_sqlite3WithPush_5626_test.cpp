// Lightweight C++11 unit tests for sqlite3WithPush (mocked environment)
// This file provides a self-contained test harness without GoogleTest.
// It exercises the focal function sqlite3WithPush and its dependent behaviors
// by mocking minimal necessary SQLite-like structures.

#include <cstdio>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-specific: minimal stand-ins for SQLite types used by sqlite3WithPush
using u8 = uint8_t;

// Forward-declare a dummy sqlite3 type to satisfy the function signatures
struct sqlite3 {};

// Minimal dependency structures to mirror the focal function's usage
struct With {
    With* pOuter;
};

struct Parse {
    int nErr;
    With* pWith;
};

// Global switch to simulate different cleanup behaviors in tests
static bool gSimulateCleanupFailure = false;

// Mocked external cleanup function type used by sqlite3ParserAddCleanup
static void sqlite3WithDelete(sqlite3* db, void* p){ (void)db; (void)p; /* no-op in tests */ }

// Mocked function: simulates sqlite3ParserAddCleanup
// Returns the same pointer 'p' by default, or NULL if we want to simulate a cleanup failure.
static void* sqlite3ParserAddCleanup(Parse* pParse, void(*xCleanup)(sqlite3*, void*), void* p) {
    (void)xCleanup; // unused in tests beyond signature
    (void)pParse;  // pParse is unused for our simplified mock
    if (gSimulateCleanupFailure) {
        return nullptr;
    }
    return p;
}

// The focal method under test (reproduced here in a self-contained form)
static With* sqlite3WithPush(Parse *pParse, With *pWith, u8 bFree){
  if( pWith ){
    if( bFree ){
      pWith = (With*)sqlite3ParserAddCleanup(pParse,
                      (void(*)(sqlite3*,void*))sqlite3WithDelete,
                      pWith);
      if( pWith==0 ) return 0;
    }
    if( pParse->nErr==0 ){
      assert( pParse->pWith!=pWith );
      pWith->pOuter = pParse->pWith;
      pParse->pWith = pWith;
    }
  }
  return pWith;
}

// Lightweight test harness (non-terminating assertions)
static int gTestFailures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; \
        ++gTestFailures; \
    } } while(0)

#define EXPECT_EQ_PTR(a,b) \
    do { if((void*)(a) != (void*)(b)) { \
        std::fprintf(stderr, "EXPECT_PTR_EQ failed: %p != %p\n", (void*)(a), (void*)(b)); \
        ++gTestFailures; \
    } } while(0)

#define EXPECT_EQ_INT(a,b) \
    do { if((int)(a) != (int)(b)) { \
        std::fprintf(stderr, "EXPECT_EQ_INT failed: %d != %d\n", (int)(a), (int)(b)); \
        ++gTestFailures; \
    } } while(0)

#define TEST_START(name) static void name()
#define TEST_END(name) // no-op

// Helper to create and delete With nodes
static With* makeWith() {
    With* w = new With();
    w->pOuter = nullptr;
    return w;
}

// Test 1: When pWith is null, sqlite3WithPush should return null and not touch pParse.
TEST_START(test_WithPush_null_pWith_returns_null) {
    // Arrange
    Parse pParse;
    pParse.nErr = 0;
    pParse.pWith = nullptr;

    With* pWith = nullptr;
    gSimulateCleanupFailure = false;

    // Act
    With* res = sqlite3WithPush(&pParse, pWith, 0);

    // Assert
    EXPECT_EQ_PTR(res, nullptr, "Result should be nullptr when input pWith is nullptr");
    EXPECT_EQ_PTR(pParse.pWith, nullptr, "pParse.pWith should remain nullptr");
}
TEST_END(test_WithPush_null_pWith_returns_null)

// Test 2: nErr == 0 and cleanup succeeds; ensure pWith is pushed and pWith->pOuter set correctly.
TEST_START(test_WithPush_nErr0_cleanupSucceeds_setsOuterAndPWith) {
    // Arrange
    Parse pParse;
    pParse.nErr = 0;
    pParse.pWith = makeWith(); // existing outer
    With* oldPWith = pParse.pWith;

    With* newPWith = makeWith();

    gSimulateCleanupFailure = false;

    // Act
    With* res = sqlite3WithPush(&pParse, newPWith, 1); // bFree == 1 -> cleanup path active

    // Assert
    EXPECT_EQ_PTR(res, newPWith);
    EXPECT_EQ_PTR(pParse.pWith, newPWith);
    EXPECT_TRUE(newPWith->pOuter == oldPWith, "New pWith.pOuter should be set to old pParse.pWith");
    
    // Cleanup
    delete newPWith;
    delete oldPWith;
}
TEST_END(test_WithPush_nErr0_cleanupSucceeds_setsOuterAndPWith)

// Test 3: nErr == 0 but cleanup fails (returns NULL); sqlite3WithPush should return 0 immediately.
TEST_START(test_WithPush_nErr0_cleanupFails_returnsNull) {
    // Arrange
    Parse pParse;
    pParse.nErr = 0;
    pParse.pWith = makeWith();
    With* oldPWith = pParse.pWith;

    With* newPWith = makeWith();

    gSimulateCleanupFailure = true; // simulate cleanup failure leading to NULL

    // Act
    With* res = sqlite3WithPush(&pParse, newPWith, 1);

    // Assert
    EXPECT_TRUE(res == nullptr, "Result should be nullptr when cleanup returns NULL");
    // Since function returns 0 early, pParse should remain unchanged (except side-effects before return).
    EXPECT_EQ_PTR(pParse.pWith, oldPWith);
    // Cleanup
    delete newPWith; // may have been skipped for assignment due to early return
    delete oldPWith;
}
TEST_END(test_WithPush_nErr0_cleanupFails_returnsNull)

// Test 4: nErr != 0; ensure that no assignment happens to pWith and cleanup side effects may occur only if bFree==1.
TEST_START(test_WithPush_nErrNonZero_noAssignment) {
    // Arrange
    Parse pParse;
    pParse.nErr = 5; // non-zero
    pParse.pWith = makeWith(); // current outer

    With* oldPWith = pParse.pWith;
    With* newPWith = makeWith();

    // Case A: bFree = 0 (no cleanup)
    gSimulateCleanupFailure = false;
    With* resA = sqlite3WithPush(&pParse, newPWith, 0);

    // Assert A
    EXPECT_EQ_PTR(resA, newPWith);
    EXPECT_EQ_PTR(pParse.pWith, oldPWith); // should remain unchanged
    // pWith should not have had its pOuter changed
    EXPECT_TRUE(newPWith->pOuter == nullptr || newPWith->pOuter == nullptr);

    // Cleanup for case A
    delete newPWith;
    delete oldPWith;

    // Case B: bFree = 1 (would cleanup, but nErr != 0 prevents re-linking)
    pParse.nErr = 5;
    pParse.pWith = makeWith();
    oldPWith = pParse.pWith;
    newPWith = makeWith();

    gSimulateCleanupFailure = false;
    With* resB = sqlite3WithPush(&pParse, newPWith, 1);

    // Assert B
    EXPECT_EQ_PTR(resB, newPWith);
    EXPECT_EQ_PTR(pParse.pWith, oldPWith); // still unchanged due to nErr != 0

    // Cleanup
    delete newPWith;
    delete oldPWith;
}
TEST_END(test_WithPush_nErrNonZero_noAssignment)

// Test 5: pWith is non-null but bFree=0 and nErr==0; ensure correct outer linkage without cleanup.
TEST_START(test_WithPush_bFree0_nErr0_setsOuter) {
    // Arrange
    Parse pParse;
    pParse.nErr = 0;
    With* outer = makeWith();
    pParse.pWith = outer;

    With* inner = makeWith();

    gSimulateCleanupFailure = false;

    // Act
    With* res = sqlite3WithPush(&pParse, inner, 0);

    // Assert
    EXPECT_EQ_PTR(res, inner);
    EXPECT_EQ_PTR(pParse.pWith, inner);
    EXPECT_TRUE(inner->pOuter == outer, "inner.pOuter should be set to previous pParse.pWith (outer)");
    
    // Cleanup
    delete inner;
    delete outer;
}
TEST_END(test_WithPush_bFree0_nErr0_setsOuter)


// Main test runner
int main() {
    std::cout << "Running sqlite3WithPush unit tests (mocked environment)..." << std::endl;

    test_WithPush_null_pWith_returns_null();
    test_WithPush_nErr0_cleanupSucceeds_setsOuterAndPWith();
    test_WithPush_nErr0_cleanupFails_returnsNull();
    test_WithPush_nErrNonZero_noAssignment();
    test_WithPush_bFree0_nErr0_setsOuter();

    if (gTestFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << gTestFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
// Test suite for sqlite3FreeIndex (translated/adapted for C++11 unit testing without GTest)
// - Focuses on verifying correct cleanup behavior of Index object members.
// - Includes tests for both with and without ANALYZE branch (via SQLITE_OMIT_ANALYZE).
// - Uses custom lightweight assertions to avoid external dependencies.
// - Provides explanatory comments for each unit test.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstdint>


// Lightweight stubs and types to emulate the SQLite-like environment.

typedef struct sqlite3 sqlite3;

// Minimal Index structure matching what sqlite3FreeIndex expects.
typedef struct Index {
  void *pPartIdxWhere; // pointer to where clause (Expr)
  void *aColExpr;      // pointer to column expressions (ExprList)
  char *zColAff;        // allocated string (column affinity)
  int isResized;         // flag indicating whether azColl should be freed
  void *azColl;          // array of collations (placeholder)
  int *aiRowEst;          // optional row estimate array (STAT4)
} Index;

// Global dummy sqlite3 object
struct sqlite3 {};

// Forward declaration of the focal function under test (re-implemented here).
void sqlite3FreeIndex(sqlite3 *db, Index *p);

// Global test state to verify free/destroy calls
static int g_deleteIndexSamplesCalled = 0;
static int g_exprDeleteCalled = 0;
static int g_exprListDeleteCalled = 0;
static int g_dbFreeCalled = 0;
static int g_azCollFreeCalled = 0;
static int g_pWhereFreed = 0;
static int g_pColExprFreed = 0;
static int g_zColAffFreed = 0;
static int g_pEndFreeCalled = 0;
static int g_aiRowEstFreed = 0;

// Pointers used during test to identify which allocated objects correspond to which Free call.
static void *g_ptrWhereTracker = nullptr;
static void *g_ptrColExprTracker = nullptr;
static void *g_ptrZColAffTracker = nullptr;
static void *g_ptrAzCollTracker = nullptr;
static int *g_ptrAiRowEstTracker = nullptr;

// Pointer to the current Index being freed, used to verify final free() call on the object itself.
static Index *g_currentIndexPtr = nullptr;

// Test memory trackable objects
static int *g_pWhereMem = nullptr;     // memory backing pPartIdxWhere
static int *g_pColExprMem = nullptr;   // memory backing aColExpr
static int *g_pZColAffMem = nullptr;   // memory backing zColAff
static int *g_pAzCollMem = nullptr;    // memory backing azColl
static int *g_pAiRowEstMem = nullptr;  // memory backing aiRowEst (STAT4)

// Forward declarations of stubs that sqlite3FreeIndex calls
extern "C" {
  void sqlite3DeleteIndexSamples(sqlite3*, Index*); // optional (depends on ANALYZE)
  void sqlite3ExprDelete(sqlite3*, void*);          // deletes a single Expr
  void sqlite3ExprListDelete(sqlite3*, void*);      // deletes an ExprList
  void sqlite3DbFree(sqlite3*, void*);              // frees memory
  void sqlite3_free(void*);                         // free for STAT4 aiRowEst
}

// Flags to check whether ANALYZE-related code is included in the compilation.
// The test suite can be compiled both with and without SQLITE_OMIT_ANALYZE to exercise both branches.
#define SQLITE_ENABLE_STAT4  // define to enable STAT4-related free (aiRowEst)

// Global expectations for unit test
static bool EXPECTED_all_tests_passed = true;

// Simple non-terminal assertion helpers
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

#define EXPECT_TRUE(cond) do { \
  ++g_totalAssertions; \
  if(!(cond)) { \
    ++g_failedAssertions; \
    std::cerr << "EXPECT_TRUE failed at line " << __LINE__ << std::endl; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  ++g_totalAssertions; \
  if((a) != (b)) { \
    ++g_failedAssertions; \
    std::cerr << "EXPECT_EQ failed: " << (a) << " != " << (b) \
              << " at line " << __LINE__ << std::endl; \
  } \
} while(0)

#define TEST_SUMMARY() do { \
  std::cerr << "Tests: " << g_totalAssertions << " assertions, " \
            << g_failedAssertions << " failures." << std::endl; \
  if(g_failedAssertions > 0) EXPECTED_all_tests_passed = false; \
} while(0)


// Implementations of stubs to monitor calls

void sqlite3DeleteIndexSamples(sqlite3*, Index*){
  ++g_deleteIndexSamplesCalled;
}
void sqlite3ExprDelete(sqlite3*, void* p){
  if (p == g_ptrWhereTracker) g_pWhereFreed = 1;
}
void sqlite3ExprListDelete(sqlite3*, void* p){
  if (p == g_ptrColExprTracker) g_pColExprFreed = 1;
}
void sqlite3DbFree(sqlite3*, void* p){
  // Free of the Index object itself
  if (p == g_currentIndexPtr) g_pEndFreeCalled = 1;
  // Free of components
  if (p == g_pZColAffMem) g_zColAffFreed = 1;
  if (p == g_pAzCollMem) g_azCollFreeCalled = 1;
  if (p == g_pWhereMem) g_pWhereFreed = 1;
  if (p == g_pColExprMem) g_pColExprFreed = 1;
  // For the "zColAff" and "azColl" free calls we also track via separate pointers
  ++g_dbFreeCalled;
}
void sqlite3_free(void*){
#ifdef SQLITE_ENABLE_STAT4
  if (p == (void*)g_ptrAiRowEstTracker) g_aiRowEstFreed = 1;
#endif
}


// The focal function under test, implemented here to allow isolated unit testing.
void sqlite3FreeIndex(sqlite3 *db, Index *p){
#ifndef SQLITE_OMIT_ANALYZE
  sqlite3DeleteIndexSamples(db, p);
#endif
  sqlite3ExprDelete(db, p->pPartIdxWhere);
  sqlite3ExprListDelete(db, p->aColExpr);
  sqlite3DbFree(db, p->zColAff);
  if( p->isResized ) sqlite3DbFree(db, (void *)p->azColl);
#ifdef SQLITE_ENABLE_STAT4
  sqlite3_free(p->aiRowEst);
#endif
  sqlite3DbFree(db, p);
}

/* Helper function to reset all test state before each test. */
static void reset_test_state() {
  g_deleteIndexSamplesCalled = 0;
  g_exprDeleteCalled = 0;
  g_exprListDeleteCalled = 0;
  g_dbFreeCalled = 0;
  g_azCollFreeCalled = 0;
  g_pWhereFreed = 0;
  g_pColExprFreed = 0;
  g_zColAffFreed = 0;
  g_pEndFreeCalled = 0;
  g_aiRowEstFreed = 0;

  g_ptrWhereTracker = nullptr;
  g_ptrColExprTracker = nullptr;
  g_ptrZColAffTracker = nullptr;
  g_ptrAzCollTracker = nullptr;
  g_ptrAiRowEstTracker = nullptr;

  g_currentIndexPtr = nullptr;

  g_pWhereMem = nullptr;
  g_pColExprMem = nullptr;
  g_pZColAffMem = nullptr;
  g_pAzCollMem = nullptr;
  g_pAiRowEstMem = nullptr;
}

// Test 1: Verify full cleanup when ANALYZE is enabled (default path)
void test_freeindex_cleanup_with_analyze()
{
  reset_test_state();

  // Prepare a dummy Index with owned resources
  Index idx;
  g_currentIndexPtr = &idx;

  // Allocate and assign tracker-backed resources
  g_pWhereMem = new int(1);
  g_ptrWhereTracker = g_pWhereMem;
  idx.pPartIdxWhere = g_ptrWhereTracker;
  g_pWhereMem; // dummy

  g_pColExprMem = new int(2);
  g_ptrColExprTracker = g_pColExprMem;
  idx.aColExpr = g_ptrColExprTracker;

  g_pZColAffMem = new int(3);
  g_ptrZColAffTracker = g_pZColAffMem;
  idx.zColAff = (char*)g_pZColAffTracker;

  g_pAzCollMem = new int[1];
  g_ptrAzCollTracker = g_pAzCollMem;
  idx.azColl = g_ptrAzCollTracker;

#ifdef SQLITE_ENABLE_STAT4
  g_pAiRowEstMem = new int[4];
  g_ptrAiRowEstTracker = g_pAiRowEstMem;
  idx.aiRowEst = g_ptrAiRowEstTracker;
#endif

  idx.isResized = 1;

  sqlite3 db; // dummy

  // Call the function under test
  sqlite3FreeIndex(&db, &idx);

  // Assertions
  EXPECT_TRUE(g_deleteIndexSamplesCalled == 1);       // ANALYZE path should be executed
  EXPECT_TRUE(g_pWhereFreed == 1);
  EXPECT_TRUE(g_pColExprFreed == 1);
  EXPECT_TRUE(g_zColAffFreed == 1);
  EXPECT_TRUE(g_azCollFreeCalled == 1);
  EXPECT_TRUE(g_pEndFreeCalled == 1);

#ifdef SQLITE_ENABLE_STAT4
  EXPECT_TRUE(g_aiRowEstFreed == 1);
#endif

  // Cleanup
  delete g_pWhereMem;
  delete g_pColExprMem;
  delete g_pZColAffMem;
  delete g_pAzCollMem;
#ifdef SQLITE_ENABLE_STAT4
  delete g_pAiRowEstMem;
#endif
  g_currentIndexPtr = nullptr;
  g_ptrWhereTracker = nullptr;
  g_ptrColExprTracker = nullptr;
  g_ptrZColAffTracker = nullptr;
  g_ptrAzCollTracker = nullptr;
  g_ptrAiRowEstTracker = nullptr;

  // Summarize test
  TEST_SUMMARY();
}

// Test 2: Verify ANALYZE branch is omitted when SQLITE_OMIT_ANALYZE is defined
// This test is compiled with -DSQLITE_OMIT_ANALYZE externally; ensure the code
// compiles and that sqlite3DeleteIndexSamples is NOT called.
#ifdef SQLITE_OMIT_ANALYZE
void test_freeindex_no_analyze_branch()
{
  reset_test_state();

  // Prepare a dummy Index with owned resources (same as above)
  Index idx;
  g_currentIndexPtr = &idx;

  g_pWhereMem = new int(1);
  g_ptrWhereTracker = g_pWhereMem;
  idx.pPartIdxWhere = g_ptrWhereTracker;

  g_pColExprMem = new int(2);
  g_ptrColExprTracker = g_pColExprMem;
  idx.aColExpr = g_ptrColExprTracker;

  g_pZColAffMem = new int(3);
  g_ptrZColAffTracker = g_pZColAffMem;
  idx.zColAff = (char*)g_ptrZColAffTracker;

  g_pAzCollMem = new int[1];
  g_ptrAzCollTracker = g_pAzCollMem;
  idx.azColl = g_ptrAzCollTracker;

#ifdef SQLITE_ENABLE_STAT4
  g_pAiRowEstMem = new int[4];
  g_ptrAiRowEstTracker = g_pAiRowEstMem;
  idx.aiRowEst = g_ptrAiRowEstTracker;
#endif

  idx.isResized = 1;

  sqlite3 db;

  sqlite3FreeIndex(&db, &idx);

  // ANALYZE path should NOT be invoked
  EXPECT_TRUE(g_deleteIndexSamplesCalled == 0);

  // Other frees should still occur
  EXPECT_TRUE(g_pWhereFreed == 1);
  EXPECT_TRUE(g_pColExprFreed == 1);
  EXPECT_TRUE(g_zColAffFreed == 1);
  EXPECT_TRUE(g_azCollFreeCalled == 1);
  EXPECT_TRUE(g_pEndFreeCalled == 1);

#ifdef SQLITE_ENABLE_STAT4
  EXPECT_TRUE(g_aiRowEstFreed == 1);
#endif

  // Cleanup
  delete g_pWhereMem;
  delete g_pColExprMem;
  delete g_pZColAffMem;
  delete g_pAzCollMem;
#ifdef SQLITE_ENABLE_STAT4
  delete g_pAiRowEstMem;
#endif
  g_currentIndexPtr = nullptr;
  g_ptrWhereTracker = nullptr;
  g_ptrColExprTracker = nullptr;
  g_ptrZColAffTracker = nullptr;
  g_ptrAzCollTracker = nullptr;
  g_ptrAiRowEstTracker = nullptr;

  TEST_SUMMARY();
}
#endif // SQLITE_OMIT_ANALYZE

// Entry point and test runner
int main() {
  // Run tests that rely on full ANALYZE path
  test_freeindex_cleanup_with_analyze();

#ifdef SQLITE_OMIT_ANALYZE
  // If compiled with ANALYZE omitted, run the corresponding test as well
  test_freeindex_no_analyze_branch();
#else
  // If not omitting ANALYZE, optionally you could still run a synthetic test
  // that ensures the path compiles, but the actual ANALYZE-omitted path is excluded.
  // For this harness, we simply report results of the first test.
  TEST_SUMMARY();
#endif

  // Final result
  if (g_failedAssertions == 0) {
    std::cerr << "All unit tests PASSED." << std::endl;
    return 0;
  } else {
    std::cerr << "Some unit tests FAILED." << std::endl;
    return 1;
  }
}
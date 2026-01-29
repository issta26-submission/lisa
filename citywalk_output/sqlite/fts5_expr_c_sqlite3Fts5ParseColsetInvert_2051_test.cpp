// C++11 test harness for the C function sqlite3Fts5ParseColsetInvert
// This test suite is designed to run without GoogleTest (GTest).
// It mocks the minimal dependencies required by the focal method
// and validates both true/false branches of the inversion logic.
//
// Domain knowledge applied:
// - We create a minimal Fts5Parse/Fts5Config/Fts5Colset layout matching the
//   expectations of sqlite3Fts5ParseColsetInvert.
// - We simulate memory allocation via sqlite3Fts5MallocZero and sqlite3_free.
// - We verify the returned inverted column set and that the input is freed.
// - We use non-terminating assertions (custom EXPECT_EQ/EXPECT_TRUE) to maximize
//   code execution paths and coverage.

#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <cstddef>
#include <stdio.h>
#include <cstring>
#include <cstdlib>


// Forward declare C linkage for the focal function and its dependencies
extern "C" {

// Minimal type definitions matching the focal function's usage
typedef struct Fts5Config {
  int nCol;
} Fts5Config;

typedef struct Fts5Parse {
  int rc;
  Fts5Config *pConfig;
} Fts5Parse;

// Fts5Colset uses a flexible-like array (aiCol[]) at the end.
// We implement aiCol as a 1-element array for compatibility; allocations will be larger.
typedef struct Fts5Colset {
  int nCol;
  int aiCol[1];
} Fts5Colset;

// Focal function under test (present in the production C source)
Fts5Colset *sqlite3Fts5ParseColsetInvert(Fts5Parse *pParse, Fts5Colset *p);

// Memory management stubs (to be provided by test harness)
void *sqlite3Fts5MallocZero(int *pRc, size_t sz);
void sqlite3_free(void* p);

} // extern "C"

// Global state to observe allocations/frees in tests
static void *g_lastFreed = NULL;

// Simple, non-terminating assertion helpers
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    printf("Expectation failed: %s (%s:%d)\n", #cond, __FILE__, __LINE__); \
    g_test_failures++; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  auto _a = (a); auto _b = (b); \
  if(!(_a == _b)) { \
    printf("EXPECT_EQ failed: %s == %s, actual %lld vs expected %lld (%s:%d)\n", #a, #b, (long long)_a, (long long)_b, __FILE__, __LINE__); \
    g_test_failures++; \
  } \
} while(0)


// Implementation of the memory management stubs
static void* sqlite3Fts5MallocZero(int* pRc, size_t sz) {
  // Simple zero-initialized allocator; records last freed pointer if ever freed later
  void *p = std::calloc(1, sz);
  if(!p) {
    if(pRc) *pRc = 1;
    return NULL;
  }
  if(pRc) *pRc = 0;
  g_lastFreed = NULL;
  return p;
}

static void sqlite3_free(void* p) {
  // Track the last freed pointer for verification that input memory is released
  g_lastFreed = p;
  std::free(p);
}

// Helper to build a test input Fts5Colset with given nCol and aiCol data
static Fts5Colset* make_input_p(int nCol, const int* aiColData) {
  // Allocate memory for Fts5Colset with nCol elements in aiCol[]
  size_t size = sizeof(Fts5Colset) + (size_t)(nCol - 1) * sizeof(int);
  int rc = 0;
  Fts5Colset* p = (Fts5Colset*)sqlite3Fts5MallocZero(&rc, size);
  if(!p) return NULL;
  p->nCol = nCol;
  for(int i = 0; i < nCol; ++i) {
    p->aiCol[i] = aiColData[i];
  }
  return p;
}

// Test Case 1: Invert {0,2,4} within 0..4 columns -> expect {1,3}
static void test_case_invert_basic_case1() {
  // Setup parse context
  Fts5Parse* pParse = (Fts5Parse*)std::malloc(sizeof(Fts5Parse));
  pParse->rc = 0;
  Fts5Config* cfg = (Fts5Config*)std::malloc(sizeof(Fts5Config));
  cfg->nCol = 5;
  pParse->pConfig = cfg;

  // Input p with nCol=3 and aiCol = {0,2,4}
  int inputAi[3] = {0, 2, 4};
  Fts5Colset* p = make_input_p(3, inputAi);
  // Call function under test
  Fts5Colset* pRet = sqlite3Fts5ParseColsetInvert(pParse, p);

  // Assertions
  EXPECT_TRUE(pRet != NULL);
  EXPECT_EQ(pRet->nCol, 2);
  EXPECT_EQ(pRet->aiCol[0], 1);
  EXPECT_EQ(pRet->aiCol[1], 3);
  // The input p should be freed by the function
  EXPECT_TRUE(g_lastFreed == p);

  // Cleanup
  if (pRet) sqlite3_free(pRet);
  std::free(pParse);
  std::free(cfg);
}

// Test Case 2: Empty input set -> invert to all columns {0,1,2,3}
static void test_case_invert_empty_input() {
  Fts5Parse* pParse = (Fts5Parse*)std::malloc(sizeof(Fts5Parse));
  pParse->rc = 0;
  Fts5Config* cfg = (Fts5Config*)std::malloc(sizeof(Fts5Config));
  cfg->nCol = 4; // 0..3
  pParse->pConfig = cfg;

  int inputAi[0] = {};
  Fts5Colset* p = make_input_p(0, inputAi); // empty set

  Fts5Colset* pRet = sqlite3Fts5ParseColsetInvert(pParse, p);

  EXPECT_TRUE(pRet != NULL);
  EXPECT_EQ(pRet->nCol, 4);
  EXPECT_EQ(pRet->aiCol[0], 0);
  EXPECT_EQ(pRet->aiCol[1], 1);
  EXPECT_EQ(pRet->aiCol[2], 2);
  EXPECT_EQ(pRet->aiCol[3], 3);
  EXPECT_TRUE(g_lastFreed == p);

  if (pRet) sqlite3_free(pRet);
  std::free(pParse);
  std::free(cfg);
}

// Test Case 3: All columns present in input -> invert to empty set
static void test_case_invert_all_present() {
  Fts5Parse* pParse = (Fts5Parse*)std::malloc(sizeof(Fts5Parse));
  pParse->rc = 0;
  Fts5Config* cfg = (Fts5Config*)std::malloc(sizeof(Fts5Config));
  cfg->nCol = 4; // 0..3
  pParse->pConfig = cfg;

  int inputAi[4] = {0, 1, 2, 3};
  Fts5Colset* p = make_input_p(4, inputAi);

  Fts5Colset* pRet = sqlite3Fts5ParseColsetInvert(pParse, p);

  EXPECT_TRUE(pRet != NULL);
  EXPECT_EQ(pRet->nCol, 0);
  EXPECT_TRUE(g_lastFreed == p);

  if (pRet) sqlite3_free(pRet);
  std::free(pParse);
  std::free(cfg);
}

// Test Case 4: Invert with a non-contiguous input set across 0..5 -> expect {0,2,4}
static void test_case_invert_non_contiguous() {
  Fts5Parse* pParse = (Fts5Parse*)std::malloc(sizeof(Fts5Parse));
  pParse->rc = 0;
  Fts5Config* cfg = (Fts5Config*)std::malloc(sizeof(Fts5Config));
  cfg->nCol = 6; // 0..5
  pParse->pConfig = cfg;

  int inputAi[3] = {1, 3, 5};
  Fts5Colset* p = make_input_p(3, inputAi);

  Fts5Colset* pRet = sqlite3Fts5ParseColsetInvert(pParse, p);

  EXPECT_TRUE(pRet != NULL);
  EXPECT_EQ(pRet->nCol, 3);
  EXPECT_EQ(pRet->aiCol[0], 0);
  EXPECT_EQ(pRet->aiCol[1], 2);
  EXPECT_EQ(pRet->aiCol[2], 4);
  EXPECT_TRUE(g_lastFreed == p);

  if (pRet) sqlite3_free(pRet);
  std::free(pParse);
  std::free(cfg);
}

// Entry point
int main() {
  // Run all unit tests
  test_case_invert_basic_case1();
  test_case_invert_empty_input();
  test_case_invert_all_present();
  test_case_invert_non_contiguous();

  if (g_test_failures == 0) {
    printf("All tests passed.\n");
  } else {
    printf("Total failures: %d\n", g_test_failures);
  }

  // Return non-zero if any test failed to indicate issues during CI.
  return g_test_failures;
}
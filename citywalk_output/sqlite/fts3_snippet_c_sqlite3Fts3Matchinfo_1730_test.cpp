/*
  C++11 test suite for the focal method:
  void sqlite3Fts3Matchinfo(sqlite3_context *pContext,
                            Fts3Cursor *pCsr,
                            const char *zArg)

  This test harness uses lightweight, self-contained stubs to observe
  the control flow without requiring a full SQLite/FTS3 build. It
  follows the constraints:
  - No GoogleTest; a simple custom assertion framework is used.
  - Covers both branches of the key conditional predicates.
  - Demonstrates how static/global interactions would be exercised from tests.
  - Includes explanatory comments for each test case.

  Note: This file is intended to be linked with the production
  implementation of sqlite3Fts3Matchinfo (as provided in fts3_snippet.c)
  along with the needed dependencies. The mock functions below provide
  observable side effects so tests can verify behavior without needing the
  full database environment.
*/

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>


// Lightweight test harness
static int g_passCount = 0;
static int g_failCount = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if (cond) { \
    ++g_passCount; \
  } else { \
    ++g_failCount; \
    std::fprintf(stderr, "EXPECT_TRUE failed: %s (at %s:%d)\n", (msg), __FILE__, __LINE__); \
  } \
} while(0)

#define RUN_TEST(func) do { \
  func(); \
} while(0)

// Forward-declare types and the function under test (as seen by the production code)
#ifdef __cplusplus
extern "C" {
#endif

// Production code's external API surface (as used by sqlite3Fts3Matchinfo)
struct sqlite3_context { int dummy; };

// Destructor type for sqlite3_result_blob
typedef void (*sqlite3_destructor_type)(void*);

// Minimal stubs for FTS3-related types used by the focal function
struct Fts3Table { int dummy; };
struct Fts3Cursor { struct { void* pVtab; } base; void* pExpr; };

// Function prototype of the focal method (expected to be linked from production code)
void sqlite3Fts3Matchinfo(
  sqlite3_context *pContext,
  Fts3Cursor *pCsr,
  const char *zArg
);

// Mockable dependencies used by the focal method's branch actions
void sqlite3_result_blob(sqlite3_context* pContext, const void* pBlob, int nBlob, sqlite3_destructor_type xDel);
void sqlite3Fts3SegmentsClose(Fts3Table* pTab);
void fts3GetMatchinfo(sqlite3_context* pCtx, Fts3Cursor* pCsr, const char* zFormat);

#ifdef __cplusplus
}
#endif

// -------------------------------------------------------------------------------------
// Mock state to observe focal method behavior
// -------------------------------------------------------------------------------------

// Blob result observation
static const char* g_blobPtr = nullptr;
static int g_blobLen = -1;
static bool g_blobCalled = false;

// Segments close observation
static Fts3Table* g_segmentsClosedTab = nullptr;
static bool g_segmentsClosedCalled = false;

// Matchinfo retrieval observation
static const char* g_lastFormatUsed = nullptr;
static bool g_matchinfoCalled = false;

// Context placeholder (opaque to focal method; mocks do not rely on its internals)
static sqlite3_context g_ctx;

// -------------------------------------------------------------------------------------
// Mock implementations for dependencies (symbol names chosen to match production's usage)
// -------------------------------------------------------------------------------------

extern "C" {

// Mock of sqlite3_result_blob: records the blob result passed to the function under test.
void sqlite3_result_blob(sqlite3_context* pContext, const void* pBlob, int nBlob, sqlite3_destructor_type xDel) {
  (void)pContext;      // silence unused warnings for the test harness
  (void)xDel;
  g_blobCalled = true;
  g_blobPtr = (const char*)pBlob;
  g_blobLen = nBlob;
}

// Mock of sqlite3Fts3SegmentsClose: records the tab object passed for closing segments
void sqlite3Fts3SegmentsClose(Fts3Table* pTab) {
  (void)pTab;
  g_segmentsClosedCalled = true;
  g_segmentsClosedTab = pTab;
}

// Mock of fts3GetMatchinfo: records the format string argument used
void fts3GetMatchinfo(sqlite3_context* pCtx, Fts3Cursor* pCsr, const char* zFormat) {
  (void)pCtx;
  (void)pCsr;
  g_matchinfoCalled = true;
  g_lastFormatUsed = zFormat;
}
}

// -------------------------------------------------------------------------------------
// Test helpers
// -------------------------------------------------------------------------------------

static void reset_mock_state() {
  g_blobPtr = nullptr;
  g_blobLen = -1;
  g_blobCalled = false;

  g_segmentsClosedTab = nullptr;
  g_segmentsClosedCalled = false;

  g_lastFormatUsed = nullptr;
  g_matchinfoCalled = false;
}

// -------------------------------------------------------------------------------------
// Test Case 1: When pCsr->pExpr is NULL, the function should emit an empty blob
// and should not attempt to retrieve matchinfo data or close segments.
// -------------------------------------------------------------------------------------
static void test_sqlite3Fts3Matchinfo_pCsrExprNull_emitsEmptyBlob() {
  reset_mock_state();

  // Build a minimal CSR with a NULL pExpr
  Fts3Table tab;
  Fts3Cursor csr;
  csr.base.pVtab = &tab;
  csr.pExpr = nullptr; // triggers the "empty blob" branch

  // Call the focal method with NULL zArg to test defaulting path K (zArg ignored when pExpr is NULL)
  sqlite3Fts3Matchinfo(&g_ctx, &csr, nullptr);

  // Observations
  EXPECT_TRUE(g_blobCalled, "sqlite3_result_blob should be called with empty blob");
  EXPECT_TRUE(g_blobLen == 0, "Length of emitted blob should be 0 for empty blob");
  EXPECT_TRUE(g_segmentsClosedCalled == false, "Segments should not be closed when pExpr is NULL");
  EXPECT_TRUE(g_matchinfoCalled == false, "fts3GetMatchinfo should not be called when pExpr is NULL");
}

// -------------------------------------------------------------------------------------
// Test Case 2: When pCsr->pExpr is non-NULL, the function should retrieve matchinfo data
// and then close segments. It should not emit the empty blob in this path.
// Also verify that the provided zArg is passed through to the matchinfo routine.
// -------------------------------------------------------------------------------------
static void test_sqlite3Fts3Matchinfo_pCsrExprNonNull_triggersMatchinfoAndCloseSegments() {
  reset_mock_state();

  // Build a minimal CSR with a non-NULL pExpr
  Fts3Table tab;
  Fts3Cursor csr;
  csr.base.pVtab = &tab;
  csr.pExpr = (void*)0x1; // arbitrary non-NULL to trigger non-empty path

  // Use a non-NULL zArg to ensure it's passed through to fts3GetMatchinfo
  const char* testFormat = "TEST_FORMAT";
  sqlite3Fts3Matchinfo(&g_ctx, &csr, testFormat);

  // Observations
  EXPECT_TRUE(g_matchinfoCalled, "fts3GetMatchinfo should be called when pExpr is non-NULL");
  EXPECT_TRUE(g_lastFormatUsed != nullptr, "Format argument should be passed to fts3GetMatchinfo");
  EXPECT_TRUE(std::strcmp(g_lastFormatUsed, testFormat) == 0,
              "Format argument passed to fts3GetMatchinfo should match the provided zArg");
  EXPECT_TRUE(g_segmentsClosedCalled, "sqlite3Fts3SegmentsClose should be called after retrieving matchinfo");
  // When pExpr is non-NULL, the empty blob path should not be taken
  EXPECT_TRUE(g_blobCalled == false, "sqlite3_result_blob should not be called in non-empty path");
}

// -------------------------------------------------------------------------------------
// Test Case 3: When zArg is NULL but pExpr is non-NULL, ensure default format path is used
// This test validates that the default macro would be used in the absence of an explicit zArg.
// -------------------------------------------------------------------------------------
static void test_sqlite3Fts3Matchinfo_pCsrExprNonNull_nullZArg_usesDefaultFormat() {
  reset_mock_state();

  Fts3Table tab;
  Fts3Cursor csr;
  csr.base.pVtab = &tab;
  csr.pExpr = (void*)0x2; // non-NULL

  // Pass NULL for zArg to exercise default handling inside the focal method
  sqlite3Fts3Matchinfo(&g_ctx, &csr, nullptr);

  // Observations
  EXPECT_TRUE(g_matchinfoCalled, "fts3GetMatchinfo should be invoked when pExpr is non-NULL");
  // We can't know the exact string of FTS3_MATCHINFO_DEFAULT here, but we can assert
  // that a format was indeed used (non-NULL).
  EXPECT_TRUE(g_lastFormatUsed != nullptr, "Default format should be used when zArg is NULL");
  // Additionally, close segments should occur in this path
  EXPECT_TRUE(g_segmentsClosedCalled, "Segments should be closed after matchinfo retrieval");
}

// -------------------------------------------------------------------------------------
// Main: run tests and report summary
// -------------------------------------------------------------------------------------
int main() {
  RUN_TEST(test_sqlite3Fts3Matchinfo_pCsrExprNull_emitsEmptyBlob);
  RUN_TEST(test_sqlite3Fts3Matchinfo_pCsrExprNonNull_triggersMatchinfoAndCloseSegments);
  RUN_TEST(test_sqlite3Fts3Matchinfo_pCsrExprNonNull_nullZArg_usesDefaultFormat);

  std::fprintf(stdout, "\nTest Summary: Passed %d, Failed %d\n", g_passCount, g_failCount);
  return (g_failCount == 0) ? 0 : 1;
}
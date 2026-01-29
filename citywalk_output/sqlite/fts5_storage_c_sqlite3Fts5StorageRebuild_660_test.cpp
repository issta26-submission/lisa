/*
  Lightweight C++11 unit test suite for the focal function:
  sqlite3Fts5StorageRebuild

  Approach:
  - Reproduce a minimal in-file environment that mimics the necessary
    SQLite/FTS5 plumbing used by sqlite3Fts5StorageRebuild.
  - Provide stubbed implementations for all external dependencies used by the
    focal function, with controllable behavior to exercise both true/false
    branches of predicates.
  - Implement a small, non-terminating assertion framework (no exceptions, no GTest).
  - Write several test cases to cover normal path, edge cases, and error paths.
  - All tests live in a single translation unit for simplicity and portability.

  Notes:
  - This is a self-contained test harness; it defines the minimum types and
    functions required by the focal function and its usage.
  - Unit tests call sqlite3Fts5StorageRebuild through the same interface as the
    production code, but with stubs that you can drive to simulate scenarios.
  - Static helpers are kept file-local; test code accesses simulated static-like
    behavior via globals (as allowed by the domain knowledge).
*/

#include <vector>
#include <fts5Int.h>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight, non-terminating test framework (no GTest)
static int g_test_failures = 0;
static int g_test_total = 0;

// Assertions (non-terminating)
#define EXPECT_EQ(a,b) do { auto _a=(a); auto _b=(b); if(!(_a==_b)){ \
  std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " << #b << " (" << _b << ")\n"; \
  ++g_test_failures; } ++g_test_total; } while(0)

#define EXPECT_NE(a,b) do { auto _a=(a); auto _b=(b); if(_a==_b)){ \
  std::cerr << "EXPECT_NE failed: " << #a << " (" << _a << ") == " << #b << " (" << _b << ")\n"; \
  ++g_test_failures; } ++g_test_total; } while(0)

#define EXPECT_TRUE(x) do { if(!(x)){ \
  std::cerr << "EXPECT_TRUE failed: " << #x << "\n"; ++g_test_failures; } ++g_test_total; } while(0)
#define EXPECT_FALSE(x) do { if(x){ \
  std::cerr << "EXPECT_FALSE failed: " << #x << "\n"; ++g_test_failures; } ++g_test_total; } while(0)

// Forward declarations to mirror production types/signatures (minimal subset)
using i64 = long long;
typedef unsigned char u8;
typedef unsigned int u32;

struct Fts5Buffer {
  unsigned char *p; // pointer data
  int n;              // capacity/size (used in tests simply as a count)
  int i;              // unused in test
};

// minimal Fts5Config
struct Fts5Config {
  int nCol;
  unsigned char abUnindexed[8];
};

// opaque storage/index types
struct Fts5Index {};

// storage struct used by the focal function
struct Fts5Storage {
  Fts5Config *pConfig;
  Fts5Index *pIndex;
  long long aTotalSize[8];
  int nTotalRow;
};

// context for tokenization callback
struct Fts5InsertCtx {
  int iCol;
  int szCol;
  void *pStorage; // points to Fts5Storage
};

// fake sqlite3_stmt (opaque in production)
typedef struct sqlite3_stmt sqlite3_stmt;

// ---------------------- Simulation knobs (test control) ----------------------

static int g_tokenizeFail = 0;     // if set, sqlite3Fts5Tokenize simulates error
static int g_saveTotalsError = 0;  // if set, fts5StorageSaveTotals simulates error
static int g_fakeMode = 0;          // dataset mode for the fake scanner

enum {
  SQLITE_OK = 0,
  SQLITE_ROW = 100,
  SQLITE_DONE = 101,
  SQLITE_ERROR = 1
};

// fake scan dataset modes
enum {
  TEST_MODE_NORMAL = 0,     // 2 rows, 2 cols
  TEST_MODE_SINGLE_ROW = 1, // 1 row
  TEST_MODE_EMPTY = 2         // 0 rows
};

// --------------- Fake statement dataset used by sqlite3_step/column* ---------------
struct FakeStmt {
  int currentRow;
  int totalRows;
  long long rowids[4];
  const char* texts[4][2]; // [row][colIndex]
  int nCol; // number of data columns per row
};

// Global fake statement instance (used by the stubbed sqlite3_* functions)
static FakeStmt g_fakeStmt;

// Helpers for dataset setup (called by GetStmt)
static void setup_fake_stmt_for_mode(int mode) {
  // Reset
  g_fakeStmt.currentRow = 0;
  g_fakeStmt.totalRows = 0;
  g_fakeStmt.nCol = 2; // default 2 data columns per row
  for (int r=0; r<4; ++r) {
    g_fakeStmt.rowids[r] = 0;
    g_fakeStmt.texts[r][0] = nullptr;
    g_fakeStmt.texts[r][1] = nullptr;
  }

  if (mode == TEST_MODE_NORMAL) {
    // Two rows with two text columns
    g_fakeStmt.totalRows = 2;
    g_fakeStmt.rowids[0] = 1;
    g_fakeStmt.rowids[1] = 2;
    g_fakeStmt.texts[0][0] = "alpha";
    g_fakeStmt.texts[0][1] = "beta";
    g_fakeStmt.texts[1][0] = "gamma";
    g_fakeStmt.texts[1][1] = "delta";
  } else if (mode == TEST_MODE_SINGLE_ROW) {
    g_fakeStmt.totalRows = 1;
    g_fakeStmt.rowids[0] = 10;
    g_fakeStmt.texts[0][0] = "one";
    g_fakeStmt.texts[0][1] = "two";
  } else if (mode == TEST_MODE_EMPTY) {
    g_fakeStmt.totalRows = 0;
  }
}

// ---------------------- Stubbed SQLite/FTS5 related functions ----------------------

// sqlite3_step: advances fake cursor
static int sqlite3_step(sqlite3_stmt *pScan) {
  (void)pScan;
  if (g_fakeStmt.currentRow < g_fakeStmt.totalRows) {
    g_fakeStmt.currentRow++;
    return SQLITE_ROW;
  }
  return SQLITE_DONE;
}

// Accessors for row data
static long long sqlite3_column_int64(sqlite3_stmt *pScan, int iCol) {
  (void)pScan;
  if (iCol == 0) {
    // rowid is stored as the current row index's id
    int idx = g_fakeStmt.currentRow - 1;
    if (idx >= 0 && idx < g_fakeStmt.totalRows) {
      return g_fakeStmt.rowids[idx];
    }
  }
  return 0;
}

static const unsigned char* sqlite3_column_text(sqlite3_stmt *pScan, int iCol) {
  (void)pScan;
  if (iCol <= 0) return nullptr;
  int idx = g_fakeStmt.currentRow - 1;
  int colIndex = iCol - 1;
  if (idx >= 0 && idx < g_fakeStmt.totalRows && colIndex < g_fakeStmt.nCol) {
    return (const unsigned char*)g_fakeStmt.texts[idx][colIndex];
  }
  return nullptr;
}

static int sqlite3_column_bytes(sqlite3_stmt *pScan, int iCol) {
  const unsigned char* s = sqlite3_column_text(pScan, iCol);
  if (!s) return 0;
  return (int)strlen((const char*)s);
}

// blob free (no-op for test)
static void sqlite3_free(void* p) {
  if (p) free(p);
}

// AppendVarint: in test, do not allocate; simply accumulate into buffer size
static int sqlite3Fts5BufferAppendVarint(int *rc, Fts5Buffer *p, int v) {
  if (*rc != SQLITE_OK) return *rc;
  // simulate a write by increasing n (no actual allocation)
  p->n += v;
  return SQLITE_OK;
}

// Buffer zero
static void sqlite3Fts5BufferZero(Fts5Buffer *p) {
  p->p = nullptr;
  p->n = 0;
  p->i = 0;
}

// Begin write (no-op in test)
static int sqlite3Fts5IndexBeginWrite(Fts5Index *pIndex, int a, i64 rowid) {
  (void)pIndex; (void)a; (void)rowid;
  return SQLITE_OK;
}

// Tokenize callback (our test callback will accumulate szCol)
static int fts5StorageInsertCallback(void *pContext, int tflags, const char *pToken, int nToken, int iUnused1, int iUnused2);

// Tokenize function (stubbed): calls the callback once with a synthesized token length
static int sqlite3Fts5Tokenize(Fts5Config *pConfig, int eMode, const char *zText, int nText, void *pCtx, int (*xTokenize)(void*, int, const char*, int, int, int)) {
  (void)pConfig; (void)eMode; (void)zText;
  if (g_tokenizeFail) {
    return SQLITE_ERROR;
  }
  // Simulate a tokenization pass: invoke the callback once with a token length derived from text
  int tokenLen = (nText > 0) ? (nText / 2) : 0;
  int rc = xTokenize(pCtx, 0, zText, tokenLen, 0, 0);
  if (rc != SQLITE_OK) return rc;
  return SQLITE_OK;
}

// Insert docsize (no-op in tests)
static int fts5StorageInsertDocsize(Fts5Storage *p, i64 iRowid, Fts5Buffer *pBuf) {
  (void)p; (void)iRowid; (void)pBuf;
  return SQLITE_OK;
}

// Load/save totals (save can fail via g_saveTotalsError)
static int fts5StorageLoadTotals(Fts5Storage *p, int bCache) {
  (void)p; (void)bCache;
  return SQLITE_OK;
}
static int fts5StorageSaveTotals(Fts5Storage *p) {
  (void)p;
  if (g_saveTotalsError) return SQLITE_ERROR;
  return SQLITE_OK;
}

// Delete all entries (no-op in test)
static int sqlite3Fts5StorageDeleteAll(Fts5Storage *p) {
  (void)p;
  return SQLITE_OK;
}

// Get statement: return pointer to our fake statement container
static int FTS5_STMT_SCAN = 0; // compatibility alias (not used in test)
static int fts5StorageGetStmt(Fts5Storage *p, int eStmt, sqlite3_stmt **ppStmt, char **pzErrMsg) {
  (void)p; (void)eStmt; (void)ppStmt; (void)pzErrMsg;
  // Provide the address of our static fake statement
  *ppStmt = (sqlite3_stmt*)&g_fakeStmt;
  return SQLITE_OK;
}

// Reset statement (no-op)
static int sqlite3_reset(sqlite3_stmt *pStmt) {
  (void)pStmt;
  return SQLITE_OK;
}

// Static method used in original file; not used directly in test, but declared for compatibility
static int fts5StorageDecodeSizeArray(int *aCol, int nCol, const unsigned char *aBlob, int nBlob) {
  (void)aCol; (void)nCol; (void)aBlob; (void)nBlob;
  return 0;
}

// ---------------------- FOCAL FUNCTION: sqlite3Fts5StorageRebuild ----------------------
// We re-implement the focal function body here to be self-contained with the above stubs.
int sqlite3Fts5StorageRebuild(Fts5Storage *p){
  Fts5Buffer buf = {0,0,0};
  Fts5Config *pConfig = p->pConfig;
  sqlite3_stmt *pScan = 0;
  Fts5InsertCtx ctx;
  int rc, rc2;
  memset(&ctx, 0, sizeof(Fts5InsertCtx));
  ctx.pStorage = p;
  rc = sqlite3Fts5StorageDeleteAll(p);
  if( rc==SQLITE_OK ){
    rc = fts5StorageLoadTotals(p, 1);
  }
  if( rc==SQLITE_OK ){
    rc = fts5StorageGetStmt(p, /*FTS5_STMT_SCAN*/0, &pScan, 0);
  }
  while( rc==SQLITE_OK && SQLITE_ROW==sqlite3_step(pScan) ){
    i64 iRowid = sqlite3_column_int64(pScan, 0);
    sqlite3Fts5BufferZero(&buf);
    rc = sqlite3Fts5IndexBeginWrite(p->pIndex, 0, iRowid);
    for(ctx.iCol=0; rc==SQLITE_OK && ctx.iCol<pConfig->nCol; ctx.iCol++){
      ctx.szCol = 0;
      if( pConfig->abUnindexed[ctx.iCol]==0 ){
        const char *zText = (const char*)sqlite3_column_text(pScan, ctx.iCol+1);
        int nText = sqlite3_column_bytes(pScan, ctx.iCol+1);
        rc = sqlite3Fts5Tokenize(pConfig, 
            /*FTS5_TOKENIZE_DOCUMENT*/0,
            zText, nText,
            (void*)&ctx,
            fts5StorageInsertCallback
        );
      }
      sqlite3Fts5BufferAppendVarint(&rc, &buf, ctx.szCol);
      p->aTotalSize[ctx.iCol] += (i64)ctx.szCol;
    }
    p->nTotalRow++;
    if( rc==SQLITE_OK ){
      rc = fts5StorageInsertDocsize(p, iRowid, &buf);
    }
  }
  sqlite3_free(buf.p);
  rc2 = sqlite3_reset(pScan);
  if( rc==SQLITE_OK ) rc = rc2;
  /* Write the averages record */
  if( rc==SQLITE_OK ){
    rc = fts5StorageSaveTotals(p);
  }
  return rc;
}

// callback for tokenization
static int fts5StorageInsertCallback(
  void *pContext,
  int tflags,
  const char *pToken,
  int nToken,
  int iUnused1,
  int iUnused2
){
  Fts5InsertCtx *ctx = (Fts5InsertCtx*)pContext;
  (void)tflags; (void)pToken; (void)iUnused1; (void)iUnused2;
  // accumulate token length into szCol
  ctx->szCol += nToken;
  return SQLITE_OK;
}

// ---------------------- Test harness: test cases ----------------------

static void run_all_tests();

static void test_rebuild_normal();          // normal path: 2 rows, 2 cols; first col unindexed=0, second unindexed=1
static void test_rebuild_tokenize_failure(); // tokenization failure path
static void test_rebuild_save_totals_failure(); // failure on totals save
static void test_rebuild_empty_dataset();       // empty dataset path
static void test_rebuild_skip_first_column();   // first column unindexed (skip tokenize)

static void reset_globals_for_test() {
  g_tokenizeFail = 0;
  g_saveTotalsError = 0;
  g_fakeMode = TEST_MODE_NORMAL;
  setup_fake_stmt_for_mode(g_fakeMode);
}

// Implementation of test cases

// Test 1: Normal path with 2 rows, 2 data columns; first column tokenized, second column skipped
static void test_rebuild_normal() {
  reset_globals_for_test();

  // Setup configuration
  Fts5Config cfg;
  cfg.nCol = 2;
  // first column unindexed (0) -> tokenize; second column unindexed (1) -> skip
  cfg.abUnindexed[0] = 0;
  cfg.abUnindexed[1] = 1;

  // Prepare storage
  Fts5Storage storage;
  storage.pConfig = &cfg;
  storage.pIndex = nullptr; // not used by test
  for (int i=0;i<8;i++) storage.aTotalSize[i] = 0;
  storage.nTotalRow = 0;

  // Run
  int rc = sqlite3Fts5StorageRebuild(&storage);
  // Verify
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(storage.nTotalRow, 2);
  // First column should have non-zero total size due to tokenization
  EXPECT_TRUE(storage.aTotalSize[0] > 0);
  // Second column is unindexed, so tokenization not performed; size remains 0 or unchanged above
  EXPECT_EQ(storage.aTotalSize[1], 0);
  std::cout << "test_rebuild_normal completed.\n";
}

// Test 2: Tokenization fails -> rebuild should return error
static void test_rebuild_tokenize_failure() {
  reset_globals_for_test();
  g_tokenizeFail = 1;
  g_fakeMode = TEST_MODE_SINGLE_ROW;
  setup_fake_stmt_for_mode(g_fakeMode); // ensure we have at least one row

  Fts5Config cfg;
  cfg.nCol = 2;
  cfg.abUnindexed[0] = 0;
  cfg.abUnindexed[1] = 0; // both columns tokenized

  Fts5Storage storage;
  storage.pConfig = &cfg;
  storage.pIndex = nullptr;
  for (int i=0;i<8;i++) storage.aTotalSize[i] = 0;
  storage.nTotalRow = 0;

  int rc = sqlite3Fts5StorageRebuild(&storage);
  EXPECT_NE(rc, SQLITE_OK);
  std::cout << "test_rebuild_tokenize_failure completed.\n";
}

// Test 3: Saving totals fails (rc should propagate)
static void test_rebuild_save_totals_failure() {
  reset_globals_for_test();
  g_saveTotalsError = 1;
  g_fakeMode = TEST_MODE_SINGLE_ROW;
  setup_fake_stmt_for_mode(g_fakeMode);

  Fts5Config cfg;
  cfg.nCol = 2;
  cfg.abUnindexed[0] = 0;
  cfg.abUnindexed[1] = 0;

  Fts5Storage storage;
  storage.pConfig = &cfg;
  storage.pIndex = nullptr;
  for (int i=0;i<8;i++) storage.aTotalSize[i] = 0;
  storage.nTotalRow = 0;

  int rc = sqlite3Fts5StorageRebuild(&storage);
  EXPECT_NE(rc, SQLITE_OK);
  std::cout << "test_rebuild_save_totals_failure completed.\n";
}

// Test 4: Empty dataset path
static void test_rebuild_empty_dataset() {
  reset_globals_for_test();
  g_fakeMode = TEST_MODE_EMPTY;
  setup_fake_stmt_for_mode(g_fakeMode);

  Fts5Config cfg;
  cfg.nCol = 2;
  cfg.abUnindexed[0] = 0;
  cfg.abUnindexed[1] = 0;

  Fts5Storage storage;
  storage.pConfig = &cfg;
  storage.pIndex = nullptr;
  for (int i=0;i<8;i++) storage.aTotalSize[i] = 0;
  storage.nTotalRow = 0;

  int rc = sqlite3Fts5StorageRebuild(&storage);
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(storage.nTotalRow, 0);
  EXPECT_EQ(storage.aTotalSize[0], 0);
  std::cout << "test_rebuild_empty_dataset completed.\n";
}

// Test 5: First column unindexed (skip tokenize) and ensure second column tokenization only
static void test_rebuild_skip_first_column() {
  reset_globals_for_test();
  g_fakeMode = TEST_MODE_NORMAL;
  setup_fake_stmt_for_mode(g_fakeMode);

  Fts5Config cfg;
  cfg.nCol = 2;
  cfg.abUnindexed[0] = 1; // skip first column
  cfg.abUnindexed[1] = 0; // tokenize second column

  Fts5Storage storage;
  storage.pConfig = &cfg;
  storage.pIndex = nullptr;
  for (int i=0;i<8;i++) storage.aTotalSize[i] = 0;
  storage.nTotalRow = 0;

  int rc = sqlite3Fts5StorageRebuild(&storage);
  EXPECT_EQ(rc, SQLITE_OK);
  // First column should remain 0
  EXPECT_EQ(storage.aTotalSize[0], 0);
  // Second column should have been tokenized for both rows
  EXPECT_GT(storage.aTotalSize[1], 0);
  std::cout << "test_rebuild_skip_first_column completed.\n";
}

// ---------------------- Test runner ----------------------

static void run_all_tests() {
  test_rebuild_normal();
  test_rebuild_tokenize_failure();
  test_rebuild_save_totals_failure();
  test_rebuild_empty_dataset();
  test_rebuild_skip_first_column();

  std::cout << "Tests run: " << g_test_total << ", Failures: " << g_test_failures << "\n";
  if (g_test_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Some tests failed.\n";
  }
}

int main() {
  run_all_tests();
  return (g_test_failures == 0) ? 0 : 1;
}

// Dummy main body ends here.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic pop

// Note: The actual production code's static helpers and more precise type layout are not
// required for these unit tests. The above implementation focuses on exercising the
// control flow of sqlite3Fts5StorageRebuild using a self-contained, test-friendly
// simulation of the dependent components (tokenization, scanning, and totals handling).
// Each test case is annotated with comments explaining its goal and coverage intent.
/*
Unit test suite for sqlite3Fts5StorageIndexInsert

Notes for maintainers:
- This test suite is designed as a lightweight C++11 test harness without Google Test.
- It relies on user-provided stubs/mocks for the SQLite FTS5 related types and functions to exercise the focal method.
- The test aims to cover true/false branches of key predicates in sqlite3Fts5StorageIndexInsert, including:
  - Path where rc from fts5StorageLoadTotals is SQLITE_OK and index write begins.
  - Loop behavior for indexed vs unindexed columns.
  - Docsize insertion path and buffer handling.
  - Increment of total row counter.
- Static helpers and domain-knowledge constraints are respected:
  - Access to static-like helpers via the test environment (simulated).
  - No dependency on GTest; uses simple EXPECT_* macros that do not terminate on failure.
- If you integrate with the real SQLite/FTS5 sources, ensure the included headers (fts5Int.h, sqlite3.h) are on the include path and linked with the test binary.

This file intentionally provides explanatory comments on each test case. The actual compilation requires the project’s sqlite3/fts5 headers and appropriate linker setup to resolve sqlite3Fts5StorageIndexInsert and its dependencies. Where necessary, replace stubs with real implementations or provide proper mocks as per your environment.

To use:
- Compile with a C++11-capable compiler.
- Link against sqlite3 (with FTS5) and include fts5Int.h from your source tree, or adapt the test to work with your build system.
- Run the produced executable to observe test outputs.

Compilation and linking hints:
- If your environment uses a different calling convention or symbol visibility, adjust the mock/spy definitions accordingly.
- Ensure static/dynamic linkage does not exclude the focal function during linking.

*/

#include <vector>
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Define project-typical typedefs and constants for test harness.
// In a real environment you should include sqlite3.h and fts5Int.h instead of re-defining.
typedef long long i64;
#define SQLITE_OK 0

// Forward declarations of the focal function (to be resolved by linker in your environment)
extern "C" int sqlite3Fts5StorageIndexInsert(
  struct Fts5Storage *p, 
  struct sqlite3_value **apVal, 
  i64 iRowid
);

// Minimal mock/types to compile this test harness standalone.
// In the real test, rely on the actual project headers for full fidelity.
struct Fts5Config {
  int nCol;
  unsigned char *abUnindexed; // 0 => indexed, non-zero => unindexed
};

struct Fts5Storage {
  Fts5Config *pConfig;
  void *pIndex;
  long long *aTotalSize;
  long long nTotalRow;
};

struct Fts5Buffer {
  unsigned char *p;
  int n;
};

struct Fts5InsertCtx {
  Fts5Storage *pStorage;
  int iCol;
  int szCol;
};

// A very lightweight mimic of sqlite3_value for test purposes.
// The focal function uses sqlite3_value_text() and sqlite3_value_bytes().
// We provide overrides (heavyweight mocks) in this test to simulate behavior.
struct sqlite3_value {
  const char *text;
  int bytes;
};

// Backward-compatible mock implementations of sqlite3_value_text and sqlite3_value_bytes.
// We provide them here so the focal function can compile against this test harness without pulling in the real SQLite sources.
extern "C" const unsigned char* sqlite3_value_text(struct sqlite3_value *pVal){
  if(pVal == nullptr) return nullptr;
  return reinterpret_cast<const unsigned char*>(pVal->text);
}
extern "C" int sqlite3_value_bytes(struct sqlite3_value *pVal){
  if(pVal == nullptr) return 0;
  return pVal->bytes;
}

// Mocked dependencies to drive sqlite3Fts5StorageIndexInsert through test scenarios.

// fts5StorageLoadTotals mock: always return SQLITE_OK in tests unless explicitly set.
static int g_bypassTotalsOk = 1;
extern "C" int fts5StorageLoadTotals(Fts5Storage *p, int bCache){
  (void)p; (void)bCache;
  return g_bypassTotalsOk ? SQLITE_OK : 1; // non-zero simulates failure path
}

// sqlite3Fts5IndexBeginWrite mock: pretend to begin a write, always return OK
extern "C" int sqlite3Fts5IndexBeginWrite(void *pIndex, int a, i64 rowid){
  (void)pIndex; (void)a; (void)rowid;
  return SQLITE_OK;
}

// sqlite3Fts5Tokenize mock: do not perform real tokenization; simply simulate a column size calc.
// We'll set ctx->szCol to a deterministic value derived from the input text length to drive buffer behavior.
typedef int (*TokenizeCallback)(void*, int, const char*, int, int, int);

extern "C" int sqlite3Fts5Tokenize(Fts5Config*, int, const char *zText, int nText,
                                   void *pCtx, TokenizeCallback){
  // If pCtx is valid, set szCol to a function of the text length to drive varint writing.
  if(pCtx){
    Fts5InsertCtx *ctx = reinterpret_cast<Fts5InsertCtx*>(pCtx);
    // A simple deterministic value based on text length, ensuring non-zero for realistic flow.
    ctx->szCol = (nText > 0 ? nText : 1);
  }
  // Do not invoke the callback in this mocked environment.
  return SQLITE_OK;
}

// sqlite3Fts5BufferAppendVarint mock: append a single byte equal to the provided value
extern "C" void sqlite3Fts5BufferAppendVarint(int *rc, Fts5Buffer *pBuf, int v){
  if(*rc != SQLITE_OK) return;
  // allocate or extend buffer
  int newN = pBuf->n + 1;
  unsigned char *newP = (unsigned char*)realloc(pBuf->p, newN);
  if(!newP){
    *rc = 1; // non-OK
    return;
  }
  newP[pBuf->n] = (unsigned char)v;
  pBuf->p = newP;
  pBuf->n = newN;
}

// fts5StorageInsertDocsize mock: pretend to write the docsize record
extern "C" int fts5StorageInsertDocsize(Fts5Storage*, i64, Fts5Buffer*){
  // no-op for test
  return SQLITE_OK;
}

// sqlite3_free mock: free memory (wrapper)
extern "C" void sqlite3_free(void* p){
  free(p);
}

// fts5StorageNewRowid and related internals are not needed for these tests (we rely on the focal call).

// Helper assertion macros: non-terminating, print only on failure
#define EXPECT_TRUE(expr) do { \
  if(!(expr)){ \
    std::cerr << "EXPECT_TRUE failed: " #expr " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if((a)!=(b)){ \
    std::cerr << "EXPECT_EQ failed: " #a " != " #b " actual=" << (a) << " expected=" << (b) \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

#define EXPECT_NEQ(a,b) do { \
  if((a)==(b)){ \
    std::cerr << "EXPECT_NEQ failed: " #a " == " #b " actual=" << (a) \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

// Test scaffolding helpers
static void reset_storage(Fts5Storage &p, int nCol){
  p.pConfig = new Fts5Config();
  p.pConfig->nCol = nCol;
  p.pConfig->abUnindexed = new unsigned char[nCol];
  // by default, index everything (abUnindexed[i] == 0)
  for(int i = 0; i < nCol; ++i) p.pConfig->abUnindexed[i] = 0;
  p.pIndex = nullptr;
  p.aTotalSize = new long long[nCol];
  std::memset(p.aTotalSize, 0, sizeof(long long)*nCol);
  p.nTotalRow = 0;
}

static void cleanup_storage(Fts5Storage &p){
  delete[] p.pConfig->abUnindexed;
  delete p.pConfig;
  delete[] p.aTotalSize;
  p.pConfig = nullptr;
  p.aTotalSize = nullptr;
  p.pIndex = nullptr;
}

// Utility to create apVal array with 4 entries (to access apVal[2] and apVal[3] in tests)
static void setup_apVal(sqlite3_value *apVal[4], const char* t0, int b0,
                        const char* t1, int b1)
{
  // apVal[0], apVal[1] are unused by the focal code for this test
  apVal[0] = new sqlite3_value{nullptr, 0};
  apVal[1] = new sqlite3_value{nullptr, 0};
  apVal[2] = new sqlite3_value{t0, b0};
  apVal[3] = new sqlite3_value{t1, b1};
}

// Cleanup apVal
static void cleanup_apVal(sqlite3_value *apVal[4]){
  for(int i=0;i<4;i++){
    delete apVal[i];
  }
}

// Test 1: All columns indexed (abUnindexed all zeros). Expect normal flow and rc SQLITE_OK.
static void test_Insert_AllColumnsIndexed(){
  std::cout << "[Test] sqlite3Fts5StorageIndexInsert: all columns indexed" << std::endl;

  Fts5Storage p;
  reset_storage(p, 2); // two columns
  // ensure all columns are indexed (abUnindexed == 0)
  p.pConfig->abUnindexed[0] = 0;
  p.pConfig->abUnindexed[1] = 0;
  // apVal containing column texts
  sqlite3_value *apVal[4];
  setup_apVal(apVal, "col0_text", (int)strlen("col0_text"),
                     "col1_text", (int)strlen("col1_text"));

  // Call the focal method
  int rc = sqlite3Fts5StorageIndexInsert(&p, apVal, 12345LL);

  // Assertions: rc ok; nTotalRow incremented; aTotalSize updated per ctx.szCol (simulated as text lengths)
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(p.nTotalRow == 1);
  // Each col's total size should have been incremented by a value derived from text length;
  // In our mock, ctx.szCol is set to text length, so expect 8 and 8 for "col0_text" and "col1_text" respectively.
  // But our mock adds these sizes to aTotalSize[i] after each column processed.
  EXPECT_EQ(p.aTotalSize[0], (long long)strlen("col0_text"));
  EXPECT_EQ(p.aTotalSize[1], (long long)strlen("col1_text"));

  cleanup_apVal(apVal);
  cleanup_storage(p);
  std::cout << "[Test Completed] AllColumnsIndexed" << std::endl;
}

// Test 2: Some columns unindexed. abUnindexed[0] = 1 forces skip of tokenization for first column.
// Expect the function to still complete with SQLITE_OK and update totals accordingly.
static void test_Insert_WithUnindexedColumn(){
  std::cout << "[Test] sqlite3Fts5StorageIndexInsert: one unindexed column" << std::endl;

  Fts5Storage p;
  reset_storage(p, 2); // two columns
  // Make the first column unindexed
  p.pConfig->abUnindexed[0] = 1;
  p.pConfig->abUnindexed[1] = 0; // second column remains indexed

  sqlite3_value *apVal[4];
  setup_apVal(apVal, "col0_text_unindexed", (int)strlen("col0_text_unindexed"),
                     "col1_text_indexed", (int)strlen("col1_text_indexed"));

  int rc = sqlite3Fts5StorageIndexInsert(&p, apVal, 22222LL);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(p.nTotalRow == 1);
  // For unindexed first column, ctx.szCol would be derived from text length or default; ensure sizes updated for both columns
  EXPECT_EQ(p.aTotalSize[0], (long long)strlen("col0_text_unindexed"));
  EXPECT_EQ(p.aTotalSize[1], (long long)strlen("col1_text_indexed"));

  cleanup_apVal(apVal);
  cleanup_storage(p);
  std::cout << "[Test Completed] WithUnindexedColumn" << std::endl;
}

// Test 3: Edge case - zero-length texts for all indexed columns. Ensure graceful handling and docsize write.
static void test_Insert_ZeroLengthTexts(){
  std::cout << "[Test] sqlite3Fts5StorageIndexInsert: zero-length texts" << std::endl;

  Fts5Storage p;
  reset_storage(p, 2);
  p.pConfig->abUnindexed[0] = 0;
  p.pConfig->abUnindexed[1] = 0;

  sqlite3_value *apVal[4];
  setup_apVal(apVal, "", 0, "", 0);

  int rc = sqlite3Fts5StorageIndexInsert(&p, apVal, 54321LL);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(p.nTotalRow == 1);
  // Zero-length text may result in szCol being 0 in some implementations; our mock uses nText, which is 0 here.
  // The test ensures function completes and docsize insertion path is exercised without crash.
  // We expect aTotalSize to have been increased by 0 for both columns in this simplified mock.
  EXPECT_EQ(p.aTotalSize[0], 0LL);
  EXPECT_EQ(p.aTotalSize[1], 0LL);

  cleanup_apVal(apVal);
  cleanup_storage(p);
  std::cout << "[Test Completed] ZeroLengthTexts" << std::endl;
}

// Simple test runner
int main(){
  // Informational header
  std::cout << "Running sqlite3Fts5StorageIndexInsert unit tests (mock environment)" << std::endl;

  test_Insert_AllColumnsIndexed();
  test_Insert_WithUnindexedColumn();
  test_Insert_ZeroLengthTexts();

  std::cout << "All tests executed. Review any failures above." << std::endl;
  return 0;
}

/*
Explanation of the key elements (Mapping to the focal method and its class dependencies):

- Fts5Storage, Fts5Config, Fts5Buffer, Fts5InsertCtx:
  - Represent the core dependencies the focal function relies on.
  - The tests instantiate a minimal, realistic layout and simulate interactions.

- Mocked dependencies:
  - fts5StorageLoadTotals, sqlite3Fts5IndexBeginWrite, sqlite3Fts5Tokenize,
    sqlite3Fts5BufferAppendVarint, fts5StorageInsertDocsize, and sqlite3_free
    are overridden/mocked to drive deterministic behavior and focus coverage on the focal method's control flow.

- sqlite3_value, sqlite3_value_text, sqlite3_value_bytes:
  - The focal function uses sqlite3_value_text and sqlite3_value_bytes to obtain per-column text and length.
  - The test provides lightweight sqlite3_value mocks and corresponding overrides for sqlite3_value_text and sqlite3_value_bytes to simulate this behavior without requiring a full SQLite build.

- Candidate Keywords (from Step 1):
  - Fts5Storage, Fts5Config, Fts5Buffer, Fts5InsertCtx
  - abUnindexed predicate, nCol
  - sqlite3Fts5IndexBeginWrite
  - fts5StorageLoadTotals
  - sqlite3Fts5Tokenize
  - fts5StorageInsertCallback (represented via the mock tokenize)
  - sqlite3Fts5BufferAppendVarint
  - fts5StorageInsertDocsize
  - sqlite3_free
  - sqlite3_value, sqlite3_value_text, sqlite3_value_bytes

Notes and caveats:
- The test harness relies on the ability to override or mock the dependencies (ft5sStorageLoadTotals, sqlite3Fts5Tokenize, etc.). In a real environment, you may need to adapt the mocks to your build system and provide stable symbol resolution.
- The tests are designed to be non-terminating (they do not call exit on failure) and to print failure messages via the EXPECT_* macros.
- The code uses minimal memory management to avoid leaks during tests; cleanup routines are invoked after each test.
- If you want to expand coverage further (e.g., test rc != SQLITE_OK paths, test maximum nCol, test mixed static/unindexed configurations, etc.), you can extend the test suite with additional test cases following the same pattern.
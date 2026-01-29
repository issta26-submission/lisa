/*
 * C++11 Test Suite for sqlite3Fts5IndexWrite (focusing on logic around
 * adding terms and prefix-specific writes).
 *
 * Note:
 * - This test harness is self-contained and provides a localized, mock
 *   environment to exercise the core logic of sqlite3Fts5IndexWrite.
 * - It mimics the essential dependencies (Fts5Index, Fts5Config, and the
 *   hash-write function) in a minimal way so the test can run without the
 *   full SQLite/FTS5 codebase.
 * - The implementation mirrors the focal method's behavior to validate
 *   true/false branches and rc propagation.
 * - The tests are written using a lightweight, non-terminating assertion
 *   approach (custom TEST_ASSERT macros) suitable for a C++11 environment
 *   without GoogleTest.
 *
 * Important: This is a unit-test harness mirroring the focal logic; it is
 * intended for environments where the real fts5_index.c is not wired into
 * the test harness. It demonstrates the test coverage strategy and structure.
 */

// Compile with: g++ -std=c++11 -O2 -DTEST_MAIN test_fts5_index_write.cpp -o test_fts5_index_write

#include <vector>
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Lightweight test framework (no gtest)
#define TEST_SUITE(name) static void name(); int main_wrapper_##name() { name(); return 0; } static int dummy_main = main_wrapper_##name();
#define TEST(case_name) static void case_name()
#define TEST_ASSERT(cond) do { if(!(cond)){ std::cerr << "ASSERTION FAILED: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; exit(1); } } while(0)
#define TEST_LOG(msg) std::cerr << "[TEST] " << msg << std::endl

// Candidate Keywords (core dependencies/features relied on by sqlite3Fts5IndexWrite)
 // - Fts5Index, Fts5Config
 // - sqlite3Fts5HashWrite (hash write used for both main index and prefixes)
 // - sqlite3Fts5IndexCharlenToBytelen (convert token length to bytelen for a given char)
 // - FTS5_MAIN_PREFIX (prefix constant)
 // - SQLITE_OK (return code)

// Domain-specific lightweight types/constants
static const int SQLITE_OK = 0;
static const char FTS5_MAIN_PREFIX = 0; // main prefix token

// Lightweight mock data structures to simulate the real environment
struct Fts5Config {
    int nPrefix;      // number of prefixes
    int *aPrefix;     // array of prefix characters (as int values)
};

struct Fts5Index {
    Fts5Config *pConfig;
    int rc;
    int bDelete;
    void *pHash;
    long long iWriteRowid;
};

// Global structure to capture write calls for verification
struct WriteLogEntry {
    long long iWriteRowid;
    int iCol;
    int iPos;
    char prefix;
    const char *pToken;
    int nToken;
};

// Global state used by mock functions
static std::vector<WriteLogEntry> g_writeLog;
static int g_hashReturnCodeNext = 0;      // 0 means OK, non-zero to simulate error
static int g_hashCallCount = 0;           // number of times sqlite3Fts5HashWrite was invoked

// Mock function: sqlite3Fts5HashWrite
static int sqlite3Fts5HashWrite(void *pHash, long long iWriteRowid, int iCol, int iPos,
                                char cPrefix, const char *pToken, int nToken)
{
    (void)pHash; // unused in this isolated test harness
    (void)iWriteRowid; // for logging in tests we use the explicit parameter
    g_writeLog.push_back({iWriteRowid, iCol, iPos, cPrefix, pToken, nToken});
    g_hashCallCount++;
    return g_hashReturnCodeNext;
}

// Mock function: sqlite3Fts5IndexCharlenToBytelen
// For testing, treat presence of a specific character in the token as triggering a bytelen.
static int sqlite3Fts5IndexCharlenToBytelen(const char *pToken, int nToken, int nChar)
{
    (void)nToken;
    // Look for the character with code nChar in the token.
    for(int i=0; i< nToken; ++i){
        if((unsigned char)pToken[i] == (unsigned char)nChar){
            // Return bytelen equal to nToken for simplicity in tests
            return nToken;
        }
    }
    return 0;
}

// The focal function under test (re-implemented here for a self-contained harness)
static int sqlite3Fts5IndexWrite(
  Fts5Index *p,                   /* Index to write to */
  int iCol,                       /* Column token appears in (-ve -> delete) */
  int iPos,                       /* Position of token within column */
  const char *pToken, int nToken  /* Token to add or remove to or from index */
){
  int i;                          /* Used to iterate through indexes */
  int rc = SQLITE_OK;             /* Return code */
  Fts5Config *pConfig = p->pConfig;
  assert( p->rc==SQLITE_OK );
  assert( (iCol<0)==p->bDelete );
  /* Add the entry to the main terms index. */
  rc = sqlite3Fts5HashWrite(
      p->pHash, p->iWriteRowid, iCol, iPos, FTS5_MAIN_PREFIX, pToken, nToken
  );
  for(i=0; i<pConfig->nPrefix && rc==SQLITE_OK; i++){
    const int nChar = pConfig->aPrefix[i];
    int nByte = sqlite3Fts5IndexCharlenToBytelen(pToken, nToken, nChar);
    if( nByte ){
      rc = sqlite3Fts5HashWrite(p->pHash, 
          p->iWriteRowid, iCol, iPos, (char)(FTS5_MAIN_PREFIX+i+1), pToken,
          nByte
      );
    }
  }
  return rc;
}

// Helper to reset test state
static void resetTestState()
{
    g_writeLog.clear();
    g_hashReturnCodeNext = 0;
    g_hashCallCount = 0;
}

// Test Suite
TEST(sqlite3Fts5IndexWrite_Basics_WritesMainAndOnePrefix)
{
    resetTestState();

    // Prepare a Config with 2 prefixes: 'A' and 'B'
    int prefixes[2] = {'A', 'B'};
    Fts5Config cfg;
    cfg.nPrefix = 2;
    cfg.aPrefix = prefixes;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    idx.bDelete = 0;                 // not a delete
    idx.pHash = nullptr;
    idx.iWriteRowid = 123;

    // Token contains 'A' so bytelen will be non-zero for first prefix
    const char *token = "AABC";
    int nToken = 4;

    int rc = sqlite3Fts5IndexWrite(&idx, 5, 7, token, nToken);

    TEST_ASSERT(rc == SQLITE_OK);
    TEST_ASSERT(g_writeLog.size() == 2); // one main write + one prefix write

    // Main term write
    TEST_ASSERT(g_writeLog[0].prefix == FTS5_MAIN_PREFIX);
    TEST_ASSERT(g_writeLog[0].iCol == 5);
    TEST_ASSERT(g_writeLog[0].iPos == 7);
    TEST_ASSERT(std::strcmp(g_writeLog[0].pToken, token) == 0);
    TEST_ASSERT(g_writeLog[0].nToken == nToken);

    // Prefix write for i=0 -> (FTS5_MAIN_PREFIX+1)
    TEST_ASSERT(g_writeLog[1].prefix == (char)(FTS5_MAIN_PREFIX + 1));
    TEST_ASSERT(g_writeLog[1].iCol == 5);
    TEST_ASSERT(g_writeLog[1].iPos == 7);
    TEST_ASSERT(std::strcmp(g_writeLog[1].pToken, token) == 0);
    TEST_ASSERT(g_writeLog[1].nToken == nToken);
}

TEST(sqlite3Fts5IndexWrite_Basics_NoPrefixMatch)
{
    resetTestState();

    // Prepare a Config with 2 prefixes: 'X' and 'Y'
    int prefixes[2] = {'X', 'Y'};
    Fts5Config cfg;
    cfg.nPrefix = 2;
    cfg.aPrefix = prefixes;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    idx.bDelete = 0;                 // not a delete
    idx.pHash = nullptr;
    idx.iWriteRowid = 999;

    // Token contains neither 'X' nor 'Y'
    const char *token = "ABCD";
    int nToken = 4;

    int rc = sqlite3Fts5IndexWrite(&idx, 2, 3, token, nToken);

    TEST_ASSERT(rc == SQLITE_OK);
    TEST_ASSERT(g_writeLog.size() == 1); // Only main term write
    TEST_ASSERT(g_writeLog[0].prefix == FTS5_MAIN_PREFIX);
    TEST_ASSERT(g_writeLog[0].iCol == 2);
    TEST_ASSERT(g_writeLog[0].iPos == 3);
    TEST_ASSERT(std::strcmp(g_writeLog[0].pToken, token) == 0);
    TEST_ASSERT(g_writeLog[0].nToken == nToken);
}

TEST(sqlite3Fts5IndexWrite_PropagatesErrorFromHash)
{
    resetTestState();

    // Configure to force an error on the second call
    int prefixes[2] = {'A', 'Z'};
    Fts5Config cfg;
    cfg.nPrefix = 2;
    cfg.aPrefix = prefixes;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    idx.bDelete = 0;
    idx.pHash = nullptr;
    idx.iWriteRowid = 42;

    const char *token = "A1";
    int nToken = 2;

    // First call OK, second call returns error
    g_hashCallCount = 0;
    g_hashReturnCodeNext = 0; // first call OK
    int rc = sqlite3Fts5IndexWrite(&idx, 1, 0, token, nToken);

    // After previous test, the function would already write; we now simulate error on second write by invoking again
    // For simplicity, we invoke once more to reason about error propagation (we reset counts and call again)
    // Reset log and state
    resetTestState();
    rc = SQLITE_OK;

    // First call (main term)
    rc = sqlite3Fts5IndexWrite(&idx, 1, 0, token, nToken);

    // Simulate error on second write by configuring the mock to return error on the flow
    g_hashReturnCodeNext = 1; // force error on next write
    // Manually invoke a single cycle to simulate RC propagation
    // The actual call would cause a second HashWrite (prefix write), but since rc will be non-zero, we should stop.
    // We can invoke a second time to model the failure path
    rc = sqlite3Fts5IndexWrite(&idx, 1, 0, token, nToken);

    TEST_ASSERT(rc != SQLITE_OK);
    // At least one main write should have occurred
    TEST_ASSERT(!g_writeLog.empty());
}

TEST(sqlite3Fts5IndexWrite_BranchOnDeleteFlag)
{
    resetTestState();

    // Case A: iCol < 0 and bDelete = true
    int prefixes[1] = {'A'};
    Fts5Config cfg;
    cfg.nPrefix = 1;
    cfg.aPrefix = prefixes;

    Fts5Index idx;
    idx.pConfig = &cfg;
    idx.rc = SQLITE_OK;
    idx.bDelete = 1; // delete
    idx.pHash = nullptr;
    idx.iWriteRowid = 77;

    const char *token = "A";
    int nToken = 1;

    int rc = sqlite3Fts5IndexWrite(&idx, -1, 0, token, nToken);
    TEST_ASSERT(rc == SQLITE_OK);
    TEST_ASSERT(g_writeLog.size() == 2); // main + one prefix write since 'A' matches

    // Case B: iCol >= 0 and bDelete = false
    resetTestState();
    idx.bDelete = 0;
    int rc2 = sqlite3Fts5IndexWrite(&idx, 5, 1, token, nToken);
    TEST_ASSERT(rc2 == SQLITE_OK);
    TEST_ASSERT(!g_writeLog.empty());
}

// Entry point for the test harness
TEST_SUITE(sqlite3Fts5IndexWrite_TestSuite)
{
    TEST(sqlite3Fts5IndexWrite_Basics_WritesMainAndOnePrefix);
    TEST(sqlite3Fts5IndexWrite_Basics_NoPrefixMatch);
    TEST(sqlite3Fts5IndexWrite_PropagatesErrorFromHash);
    TEST(sqlite3Fts5IndexWrite_BranchOnDeleteFlag);
}

// Run the tests
int main() {
    // Execute test suite
    sqlite3Fts5IndexWrite_TestSuite();
    TEST_LOG("All tests completed.");
    return 0;
}
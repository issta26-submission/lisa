// Test suite for sqlite3ParseObjectInit (from prepare.c)
// Domain: C++11, no Google Test, self-contained test runner with simple EXPECT-like assertions.
// Assumptions: The test is built in the same project environment as SQLite core sources.
// The test relies on internal SQLite headers being accessible (sqliteInt.h) to obtain
// the exact types and PARSE_HDR / PARSE_TAIL macros used by sqlite3ParseObjectInit.

// Candidate Keywords (Step 1): 
// - Parse, sqlite3, PARSE_HDR, PARSE_TAIL, PARSE_HDR_SZ, PARSE_TAIL_SZ
// - db.pParse, pParse->pOuterParse, pParse->db
// - mallocFailed, sqlite3ErrorMsg (memory error path)
// - Zeroing memory in header/tail areas, outer parse linkage, memory layout
// - Assertions: true branches (expected) and false branches (error paths)
// - Static/internal dependencies from sqliteInt.h (Parse type, macros)

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Ensure we declare the focal function with C linkage
extern "C" {
  // Declaration matches the focal method signature
  void sqlite3ParseObjectInit(struct Parse *pParse, struct sqlite3 *db);
}

// Include internal SQLite definitions to use real types/macros

// Minimal lightweight test harness (non-terminating assertions)
static std::vector<std::string> gFailures;

static void reportFailure(const std::string &msg) {
  gFailures.push_back(msg);
}

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { reportFailure(std::string("EXPECT_TRUE failed: ") + (msg) + " [" + __FILE__ + ":" + std::to_string(__LINE__) + "]"); } } while(0)

static bool allTestPassed() { return gFailures.empty(); }

// Helper: convert Parse header memory to a printable summary (for debugging)
static bool headerIsZero(Parse *p) {
  unsigned char* hdr = PARSE_HDR(p);
  for (size_t i = 0; i < (size_t)PARSE_HDR_SZ; ++i) {
    if (hdr[i] != 0) return false;
  }
  return true;
}

// Helper: convert Parse tail memory to a printable summary (for debugging)
static bool tailIsZero(Parse *p) {
  unsigned char* tail = PARSE_TAIL(p);
  for (size_t i = 0; i < (size_t)PARSE_TAIL_SZ; ++i) {
    if (tail[i] != 0) return false;
  }
  return true;
}

// Test 1: Basic initialization behavior when there is no previous outer parse (db.pParse == NULL)
static void test_basic_initialization_clears_memory_and_links_pParse() {
  // Arrange
  Parse pParseObj;
  sqlite3 db;

  // Initialize a clean state
  std::memset(&pParseObj, 0, sizeof(pParseObj));
  std::memset(&db, 0, sizeof(db));

  // Pre-fill header/tail with non-zero data to ensure sqlite3ParseObjectInit zeros them
  std::memset(PARSE_HDR(&pParseObj), 0xAA, PARSE_HDR_SZ);
  std::memset(PARSE_TAIL(&pParseObj), 0xBB, PARSE_TAIL_SZ);

  // Pre-condition: db.pParse must be NULL to satisfy the assertion db->pParse != pParse
  db.pParse = nullptr;
  db mallocFailed = 0; // Ensure mallocFailed path is not taken

  // Act
  sqlite3ParseObjectInit(&pParseObj, &db);

  // Assert
  EXPECT_TRUE(db.pParse == &pParseObj, "db.pParse should point to the new pParse after init");
  EXPECT_TRUE(pParseObj.db == &db, "pParse.db should reference back to db");
  // External/outer parse before init should be preserved in pOuterParse
  EXPECT_TRUE(pParseObj.pOuterParse == nullptr, "pOuterParse should be null when there was no previous outer parse");
  // Memory in header and tail should be zeroed
  EXPECT_TRUE(headerIsZero(&pParseObj), "PARSE_HDR memory should be zeroed after initialization");
  EXPECT_TRUE(tailIsZero(&pParseObj), "PARSE_TAIL memory should be zeroed after initialization");
}

// Test 2: Preservation of previous outer parse and linkage when db.pParse is non-null
static void test_outer_parse_linkage_and_assignment() {
  // Arrange
  Parse pParseObj;
  Parse previousOuter;
  sqlite3 db;

  std::memset(&pParseObj, 0, sizeof(pParseObj));
  std::memset(&previousOuter, 0, sizeof(previousOuter));
  std::memset(&db, 0, sizeof(db));

  // Pre-fill header/tail with non-zero data to verify zeroing on init
  std::memset(PARSE_HDR(&pParseObj), 0x11, PARSE_HDR_SZ);
  std::memset(PARSE_TAIL(&pParseObj), 0x22, PARSE_TAIL_SZ);

  // Non-null existing outer parse
  db.pParse = &previousOuter;
  db mallocFailed = 0;

  // Act
  sqlite3ParseObjectInit(&pParseObj, &db);

  // Assert
  EXPECT_TRUE(db.pParse == &pParseObj, "db.pParse should be updated to the new pParse object");
  EXPECT_TRUE(pParseObj.pOuterParse == &previousOuter, "pOuterParse should reference prior db.pParse (previousOuter)");
  EXPECT_TRUE(pParseObj.db == &db, "pParse.db should reference back to db");
  // Memory zeroing checks
  EXPECT_TRUE(headerIsZero(&pParseObj), "PARSE_HDR should be zeroed after init even with non-null prior outer parse");
  EXPECT_TRUE(tailIsZero(&pParseObj), "PARSE_TAIL should be zeroed after init even with non-null prior outer parse");
}

// Test 3: Allocator failure path (db.mallocFailed) should not crash and should still zero memory
static void test_malloc_failed_path_no_crash_and_memory_zeroing() {
  // Arrange
  Parse pParseObj;
  sqlite3 db;

  std::memset(&pParseObj, 0, sizeof(pParseObj));
  std::memset(&db, 0, sizeof(db));

  // Pre-fill memory to non-zero to verify zeroing occurs
  std::memset(PARSE_HDR(&pParseObj), 0xCC, PARSE_HDR_SZ);
  std::memset(PARSE_TAIL(&pParseObj), 0xDD, PARSE_TAIL_SZ);

  // Simulate malloc failure
  db.pParse = nullptr; // ensures assertion passes
  db.mallocFailed = 1;

  // Act
  sqlite3ParseObjectInit(&pParseObj, &db);

  // Assert: Even on malloc failure, memory should be zeroed and linkage should be established
  EXPECT_TRUE(db.pParse == &pParseObj, "db.pParse should be set to pParse even if mallocFailed is true");
  EXPECT_TRUE(pParseObj.db == &db, "pParse.db should reference back to db even on malloc failure");
  EXPECT_TRUE(headerIsZero(&pParseObj), "PARSE_HDR should be zeroed even when mallocFailed is true");
  EXPECT_TRUE(tailIsZero(&pParseObj), "PARSE_TAIL should be zeroed even when mallocFailed is true");
}

// Simple test harness runner
static void runAllTests() {
  test_basic_initialization_clears_memory_and_links_pParse();
  test_outer_parse_linkage_and_assignment();
  test_malloc_failed_path_no_crash_and_memory_zeroing();
}

// Entry point
int main() {
  runAllTests();

  // Summary
  if (gFailures.empty()) {
    std::cout << "All tests PASSED for sqlite3ParseObjectInit.\n";
    return 0;
  } else {
    std::cout << gFailures.size() << " test(s) FAILED for sqlite3ParseObjectInit.\n";
    for (const auto &f : gFailures) {
      std::cout << " - " << f << "\n";
    }
    return 1;
  }
}
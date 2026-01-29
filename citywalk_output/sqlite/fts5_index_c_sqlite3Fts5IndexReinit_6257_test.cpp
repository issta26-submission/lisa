// Test suite for sqlite3Fts5IndexReinit in fts5_index.c using a lightweight C++11 test harness
// This file provides two tests to cover both branches of the conditional
// if ( p->pConfig->bContentlessDelete ) within sqlite3Fts5IndexReinit.
// The tests avoid GTest and use a small in-process framework with non-terminating assertions.

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fts5Int.h>


// Bring in the C definitions. Use extern "C" to ensure proper linkage in C++.
extern "C" {
}

// Simple non-terminating assertion helper
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[ASSERT FAILED] " << (msg) \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_failures; \
    } \
  } while(0)

// Helper to construct a minimal Fts5Index instance with a configurable bContentlessDelete flag.
// This avoids needing to initialize the entire SQLite FTS5 engine and focuses on the
// interaction relevant to sqlite3Fts5IndexReinit.
static Fts5Index* makeIndexWithConfig(int contentlessDelete) {
  // Allocate a zeroed index structure
  Fts5Index *p = (Fts5Index*)calloc(1, sizeof(Fts5Index));
  if(!p) return nullptr;

  // Allocate a minimal config object and wire the flag
  p->pConfig = (Fts5Config*)calloc(1, sizeof(Fts5Config));
  if(!p->pConfig) {
    free(p);
    return nullptr;
  }
  p->pConfig->bContentlessDelete = contentlessDelete ? 1 : 0;

  // The rest of the internal state is intentionally left zeroed;
  // the test focuses on the observable effect via GetOrigin after reinit.
  return p;
}

// Helper to clean up the index and its config
static void freeIndex(Fts5Index *p) {
  if(p) {
    if(p->pConfig) free(p->pConfig);
    free(p);
  }
}

// Test: branch when bContentlessDelete is false
// Expected: origin counter after reinit should be 0 (or not set to 1)
static void test_sqlite3Fts5IndexReinit_BranchFalse() {
  // Arrange
  Fts5Index *p = makeIndexWithConfig(0); // bContentlessDelete = false
  ASSERT_TRUE(p != nullptr, "Failed to allocate Fts5Index for BranchFalse test");

  // Act
  int rc = sqlite3Fts5IndexReinit(p);
  int origin = -1;
  sqlite3Fts5IndexGetOrigin(p, (i64*)&origin); // Read origin value if available

  // Assert
  // The exact origin value depends on the internal implementation, but the
  // intent is to verify that the "false" branch does not cause a 1-origin.
  ASSERT_TRUE(rc == 0, "sqlite3Fts5IndexReinit should return 0 on success (BranchFalse)");
  ASSERT_TRUE(origin == 0, "Origin should be 0 when bContentlessDelete is false (BranchFalse)");

  // Cleanup
  freeIndex(p);
}

// Test: branch when bContentlessDelete is true
// Expected: origin counter after reinit should be 1
static void test_sqlite3Fts5IndexReinit_BranchTrue() {
  // Arrange
  Fts5Index *p = makeIndexWithConfig(1); // bContentlessDelete = true
  ASSERT_TRUE(p != nullptr, "Failed to allocate Fts5Index for BranchTrue test");

  // Act
  int rc = sqlite3Fts5IndexReinit(p);
  int origin = -1;
  sqlite3Fts5IndexGetOrigin(p, (i64*)&origin);

  // Assert
  ASSERT_TRUE(rc == 0, "sqlite3Fts5IndexReinit should return 0 on success (BranchTrue)");
  ASSERT_TRUE(origin == 1, "Origin should be 1 when bContentlessDelete is true (BranchTrue)");

  // Cleanup
  freeIndex(p);
}

// Entry point for the test harness
int main() {
  std::cout << "Starting test suite for sqlite3Fts5IndexReinit...\n";

  // Run tests
  test_sqlite3Fts5IndexReinit_BranchFalse();
  test_sqlite3Fts5IndexReinit_BranchTrue();

  // Report results
  if(g_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Total failures: " << g_failures << "\n";
  }

  // Return non-zero if any test failed (helps CI detect failures)
  return g_failures;
}
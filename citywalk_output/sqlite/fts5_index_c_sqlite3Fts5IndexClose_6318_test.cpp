// Unit test suite for sqlite3Fts5IndexClose (focusing on Fts5IndexClose behavior)
// This test suite is written for C++11 without Google Test.
// It uses a lightweight, self-contained test harness with non-terminating expectations.
// The tests rely on stubbing external C interfaces (sqlite3_finalize, sqlite3_free, sqlite3Fts5HashFree)
// and on the real focal function sqlite3Fts5IndexClose provided by fts5_index.c.
// Note: This test assumes the project exposes the Fts5Index structure and sqlite3 types via
// fts5Int.h (or equivalent headers) used by fts5_index.c.

#include <vector>
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declaration to match the project structure.
// The real project provides Fts5Index in its headers (e.g., fts5Int.h).
// We include that header to ensure layout compatibility for tests.

// Declarations for the C interfaces used by sqlite3Fts5IndexClose.
// We provide lightweight C++ wrappers to satisfy linkage when linking with the C sources.

extern "C" {
  // The sqlite3_stmt type used in the Fts5Index structure
  typedef struct sqlite3_stmt sqlite3_stmt;

  // Fts5IndexClose is the function under test
  int sqlite3Fts5IndexClose(Fts5Index *p);

  // Stubs to intercept resource management in the focal function
  int sqlite3_finalize(sqlite3_stmt *pStmt);
  void sqlite3_free(void *p);
  void sqlite3Fts5HashFree(void *p);
}

// Lightweight assertion style for non-terminating tests
static int g_tests_passed = 0;
static int g_tests_failed = 0;
#define EXPECT_TRUE(cond, desc) do { \
  if (cond) { \
    ++g_tests_passed; \
  } else { \
    ++g_tests_failed; \
    std::cout << "EXPECT_TRUE FAILED: " << (desc) << std::endl; \
  } \
} while(0)
#define EXPECT_FALSE(cond, desc) do { \
  if (!(cond)) { \
    ++g_tests_passed; \
  } else { \
    ++g_tests_failed; \
    std::cout << "EXPECT_FALSE FAILED: " << (desc) << std::endl; \
  } \
} while(0)
#define LOG(msg) do { std::cout << "LOG: " << (msg) << std::endl; } while(0)

// Sentinel addresses used by the test to identify which field was finalized/free'd
static const sqlite3_stmt *ADDR_WRITER     = reinterpret_cast<sqlite3_stmt*>(0x10);
static const sqlite3_stmt *ADDR_DELETER    = reinterpret_cast<sqlite3_stmt*>(0x20);
static const sqlite3_stmt *ADDR_IDXWRITER  = reinterpret_cast<sqlite3_stmt*>(0x30);
static const sqlite3_stmt *ADDR_IDXDELETER = reinterpret_cast<sqlite3_stmt*>(0x40);
static const sqlite3_stmt *ADDR_IDXSELECT  = reinterpret_cast<sqlite3_stmt*>(0x50);
static const sqlite3_stmt *ADDR_DATAVERSION= reinterpret_cast<sqlite3_stmt*>(0x60);
static const sqlite3_stmt *ADDR_DELETEFROMIDX = reinterpret_cast<sqlite3_stmt*>(0x70);
static const void        *ADDR_HASH       = reinterpret_cast<void*>(0x100);
static const char        *ADDR_ZDATATBL    = reinterpret_cast<char*>(0x200);

// Global logs to verify that the focal function correctly releases resources
static bool g_finalized_writer   = false;
static bool g_finalized_deleter  = false;
static bool g_finalized_idxwriter= false;
static bool g_finalized_idxdeleter= false;
static bool g_finalized_idxselect = false;
static bool g_finalized_dataVersion = false;
static bool g_finalized_deleteFromIdx = false;
static bool g_finalized_nullptr = false;
static void* g_hashFreedPtr = nullptr;
static std::vector<void*> g_freedPtrs;

// Helper to reset logs between tests
static void reset_logs() {
  g_finalized_writer = g_finalized_deleter = g_finalized_idxwriter = false;
  g_finalized_idxdeleter = g_finalized_idxselect = g_finalized_dataVersion = false;
  g_finalized_deleteFromIdx = false;
  g_finalized_nullptr = false;
  g_hashFreedPtr = nullptr;
  g_freedPtrs.clear();
}

// Stub: sqlite3_finalize
extern "C" int sqlite3_finalize(sqlite3_stmt *pStmt){
  if (pStmt == ADDR_WRITER) {
    g_finalized_writer = true;
  } else if (pStmt == ADDR_DELETER) {
    g_finalized_deleter = true;
  } else if (pStmt == ADDR_IDXWRITER) {
    g_finalized_idxwriter = true;
  } else if (pStmt == ADDR_IDXDELETER) {
    g_finalized_idxdeleter = true;
  } else if (pStmt == ADDR_IDXSELECT) {
    g_finalized_idxselect = true;
  } else if (pStmt == ADDR_DATAVERSION) {
    g_finalized_dataVersion = true;
  } else if (pStmt == ADDR_DELETEFROMIDX) {
    g_finalized_deleteFromIdx = true;
  } else if (pStmt == nullptr) {
    g_finalized_nullptr = true;
  } else {
    // Unknown pointer: still indicate something happened, but don't fail tests
    LOG("sqlite3_finalize called with unknown pointer during test");
  }
  return 0;
}

// Stub: sqlite3_free
extern "C" void sqlite3_free(void *p){
  g_freedPtrs.push_back(p);
  // No actual memory management; this is a test harness
}

// Stub: sqlite3Fts5HashFree
extern "C" void sqlite3Fts5HashFree(void *p){
  g_hashFreedPtr = p;
}

// Test 1: Close with all writer/deleter pointers non-null and zDataTbl present.
// Expect all finalizers to be invoked, and both the Fts5Index object and zDataTbl freed.
// Also verify that sqlite3Fts5HashFree was called with the hash pointer.
static void test_close_all_resources_freed() {
  reset_logs();

  // Allocate a Fts5Index object via malloc to simulate real-world usage
  Fts5Index *p = (Fts5Index*)malloc(sizeof(Fts5Index));

  // Configure the structure to satisfy the function's expectations
  p->pReader = nullptr;                          // must be 0 for the assert
  p->pWriter = (sqlite3_stmt*)ADDR_WRITER;
  p->pDeleter = (sqlite3_stmt*)ADDR_DELETER;
  p->pIdxWriter = (sqlite3_stmt*)ADDR_IDXWRITER;
  p->pIdxDeleter = (sqlite3_stmt*)ADDR_IDXDELETER;
  p->pIdxSelect = (sqlite3_stmt*)ADDR_IDXSELECT;
  p->pDataVersion = (sqlite3_stmt*)ADDR_DATAVERSION;
  p->pDeleteFromIdx = (sqlite3_stmt*)ADDR_DELETEFROMIDX;
  p->pHash = ADDR_HASH;
  p->zDataTbl = (char*)ADDR_ZDATATBL;

  // Call the function under test
  int rc = sqlite3Fts5IndexClose(p);

  // Return code must be SQLITE_OK (0 in this test harness context)
  EXPECT_TRUE(rc == 0, "sqlite3Fts5IndexClose should return 0 (OK)");

  // Check that all finalizers were invoked
  EXPECT_TRUE(g_finalized_writer, "Writer finalizer should be called");
  EXPECT_TRUE(g_finalized_deleter, "Deleter finalizer should be called");
  EXPECT_TRUE(g_finalized_idxwriter, "IdxWriter finalizer should be called");
  EXPECT_TRUE(g_finalized_idxdeleter, "IdxDeleter finalizer should be called");
  EXPECT_TRUE(g_finalized_idxselect, "IdxSelect finalizer should be called");
  EXPECT_TRUE(g_finalized_dataVersion, "DataVersion finalizer should be called");
  EXPECT_TRUE(g_finalized_deleteFromIdx, "DeleteFromIdx finalizer should be called");

  // The hash free function should have been invoked with the hash pointer
  EXPECT_TRUE(g_hashFreedPtr == ADDR_HASH, "Hash should be freed via sqlite3Fts5HashFree with correct pointer");

  // zDataTbl should be freed
  bool freed_zdata = false;
  for (void* v : g_freedPtrs) {
    if (v == ADDR_ZDATATBL) { freed_zdata = true; break; }
  }
  EXPECT_TRUE(freed_zdata, "zDataTbl should be freed via sqlite3_free");

  // The Fts5Index object itself should be freed
  bool freed_p = false;
  for (void* v : g_freedPtrs) {
    if (v == p) { freed_p = true; break; }
  }
  EXPECT_TRUE(freed_p, "Fts5Index object should be freed via sqlite3_free");

  // Clean up the test's own allocations (avoid memory leaks in test harness)
  // Note: In the real code, sqlite3_free would have freed 'p' already; here we simulate.
  // Since 'p' is freed by the tested function, we do not call free(p) again to avoid double-free.
}

// Test 2: Close with a NULL pWriter (to exercise NULL finalize path) and ensure code executes safely.
// This simulates a partially-initialized index and ensures we still free the non-null resources.
static void test_close_with_null_writer_and_some_nulls() {
  reset_logs();

  Fts5Index *p = (Fts5Index*)malloc(sizeof(Fts5Index));

  p->pReader = nullptr;                          // must be 0
  p->pWriter = NULL;                             // NULL finalize path
  p->pDeleter = (sqlite3_stmt*)ADDR_DELETER;
  p->pIdxWriter = (sqlite3_stmt*)ADDR_IDXWRITER;
  p->pIdxDeleter = nullptr;                      // NULL
  p->pIdxSelect = (sqlite3_stmt*)ADDR_IDXSELECT;
  p->pDataVersion = (sqlite3_stmt*)ADDR_DATAVERSION;
  p->pDeleteFromIdx = (sqlite3_stmt*)ADDR_DELETEFROMIDX;
  p->pHash = ADDR_HASH;
  p->zDataTbl = (char*)ADDR_ZDATATBL;

  int rc = sqlite3Fts5IndexClose(p);

  // rc should still be 0
  EXPECT_TRUE(rc == 0, "sqlite3Fts5IndexClose with NULL writer should return 0 (OK)");

  // Finalizers should be called for non-null pointers; null pointers should not set any finalizer flags
  EXPECT_TRUE(g_finalized_writer == false, "NULL writer does not trigger writer finalizer");
  EXPECT_TRUE(g_finalized_idxwriter, "IdxWriter finalizer should still be called");
  EXPECT_TRUE(g_finalized_idxselect, "IdxSelect finalizer should be called");
  EXPECT_TRUE(g_finalized_dataVersion, "DataVersion finalizer should be called");
  EXPECT_TRUE(g_finalized_deleteFromIdx, "DeleteFromIdx finalizer should be called");

  // NULL finalizers:
  // We only tracked writer/null; we can still verify that a NULL was passed to sqlite3_finalize
  // The test harness allows us to detect a finalize with NULL
  // Note: We reset and reuse to observe behavior
  // Hash should still be freed
  EXPECT_TRUE(g_hashFreedPtr == ADDR_HASH, "Hash should be freed via sqlite3Fts5HashFree with correct pointer");

  // zDataTbl freed
  bool freed_zdata = false;
  for (void* v : g_freedPtrs) {
    if (v == ADDR_ZDATATBL) { freed_zdata = true; break; }
  }
  EXPECT_TRUE(freed_zdata, "zDataTbl should be freed even if some pointers are NULL");

  // The Fts5Index object should be freed
  bool freed_p = false;
  for (void* v : g_freedPtrs) {
    if (v == p) { freed_p = true; break; }
  }
  EXPECT_TRUE(freed_p, "Fts5Index object should be freed via sqlite3_free");
}

// Test 3: Close with NULL p (p == nullptr) to ensure no action is taken and no crashes occur.
static void test_close_null_pointer() {
  reset_logs();

  int rc = sqlite3Fts5IndexClose(nullptr);

  // rc should be SQLITE_OK (0)
  EXPECT_TRUE(rc == 0, "sqlite3Fts5IndexClose(nullptr) should return 0 (OK)");

  // No resources should have been touched
  EXPECT_TRUE(g_finalized_writer == false, "No finalizers should be invoked for null input");
  EXPECT_TRUE(g_freedPtrs.empty(), "No resources should be freed for null input");
  EXPECT_TRUE(g_hashFreedPtr == nullptr, "Hash should not be freed for null input");
}

// Entry point: run the tests and summarize results
int main() {
  std::cout << "Starting unit tests for sqlite3Fts5IndexClose (C++11, no GTest)" << std::endl;

  test_close_all_resources_freed();
  test_close_with_null_writer_and_some_nulls();
  test_close_null_pointer();

  std::cout << "Test summary: "
            << g_tests_passed << " passed, "
            << g_tests_failed << " failed" << std::endl;

  // Return non-zero if any test failed
  return (g_tests_failed == 0) ? 0 : 1;
}
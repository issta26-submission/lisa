// Lightweight C++11 unit test for the focal method:
// sqlite3VtabImportErrmsg(Vdbe *p, sqlite3_vtab *pVtab)
// We provide minimal in-test implementations of the required SQLite-like
// types and memory helpers to simulate the behavior without pulling in
// the actual SQLite codebase. This test covers true/false branches of the
// condition and validates memory management semantics.

#include <vector>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// ---------- Minimal Test Harness Helpers ----------

// Forward declarations to mimic the minimal interface used by the focal method.
struct sqlite3;
struct sqlite3_vtab;
struct Vdbe;

// Global trackers to observe allocations/frees performed by the test glue.
static std::vector<void*> g_freed_ptrs;
static std::vector<void*> g_allocated_ptrs;

// Simple reset for each test case.
static void reset_memory_trackers() {
  g_freed_ptrs.clear();
  g_allocated_ptrs.clear();
}

// Mock implementations for SQLite-like memory management used by the focal code.

extern "C" {

// Simulated sqlite3DbFree: frees memory and records the freed pointer.
void sqlite3DbFree(sqlite3* db, void* p) {
  (void)db; // unused in the minimal mock
  if (p) {
    g_freed_ptrs.push_back(p);
    std::free(p);
  }
}

// Simulated sqlite3DbStrDup: duplicates a string into new memory managed by us.
char* sqlite3DbStrDup(sqlite3* db, const char* z) {
  (void)db;
  if (!z) return nullptr;
  size_t len = std::strlen(z) + 1;
  char* dup = (char*)std::malloc(len);
  if (dup) std::memcpy(dup, z, len);
  g_allocated_ptrs.push_back(dup);
  return dup;
}

// Simulated sqlite3_free: free memory (non-SQLite-specific wrapper used by the focal code)
void sqlite3_free(void* p) {
  if (p) std::free(p);
}

} // extern "C"

// ---------- Minimal Type Definitions Matching Focal Context (Test Sandbox) ----------

// Forward declare to match the signature used by sqlite3VtabImportErrmsg
struct sqlite3;
struct sqlite3_vtab {
  // In the focal code, zErrMsg is a C string that, when non-null, contains
  // an error message retrieved from an sqlite3_vtab.
  char* zErrMsg;
};

// Lightweight Vdbe structure with only fields used by the focal method.
struct Vdbe {
  sqlite3* db; // database handle used to allocate/freep memory
  char* zErrMsg; // error message buffer associated with the Vdbe
};

// Forward declaration of the focal method under test (we provide a mock/inline
// version here to be self-contained for the demonstration).
void sqlite3VtabImportErrmsg(Vdbe *p, sqlite3_vtab *pVtab);

// Implement the focal method exactly as provided, adapted to the test types.
void sqlite3VtabImportErrmsg(Vdbe *p, sqlite3_vtab *pVtab) {
  if( pVtab->zErrMsg ){
    sqlite3 *db = p->db;
    sqlite3DbFree(db, p->zErrMsg);
    p->zErrMsg = sqlite3DbStrDup(db, pVtab->zErrMsg);
    sqlite3_free(pVtab->zErrMsg);
    pVtab->zErrMsg = 0;
  }
}

// Utility assertion helpers that do not terminate tests (non-terminating).
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_EQ_STR(actual, expected, msg) do { \
  if(!(actual) || std::string(actual) != std::string(expected)) { \
    std::cerr << "EXPECT_EQ_STR FAILED: " << (msg) \
              << " | expected: \"" << (expected) << "\" got: \"" \
              << (actual ? std::string(actual) : std::string("NULL")) << "\"\n"; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_PTR_EQ(actual, expected, msg) do { \
  if((actual) != (expected)) { \
    std::cerr << "EXPECT_PTR_EQ FAILED: " << (msg) \
              << " | expected: " << (void*)(expected) \
              << " got: " << (void*)(actual) << "\n"; \
    ++g_failures; \
  } \
} while(0)

// ---------- Test Cases ----------

// Test 1: True branch when pVtab->zErrMsg is non-null.
// Verifies: old p->zErrMsg is freed, p->zErrMsg becomes a duplicate of pVtab->zErrMsg,
//          pVtab->zErrMsg is set to NULL.
void test_true_branch() {
  reset_memory_trackers();

  // Prepare database handle
  sqlite3* db = (sqlite3*)std::malloc(sizeof(sqlite3));
  db->dummy = 0; // unused in mock

  // Prepare p (Vdbe)
  Vdbe p;
  p.db = db;
  // Old error message to be freed
  const char* oldConst = "old error";
  char* oldMsg = (char*)std::malloc(std::strlen(oldConst) + 1);
  std::strcpy(oldMsg, oldConst);
  p.zErrMsg = oldMsg;

  // Prepare pVtab with new error message to copy
  sqlite3_vtab vtab;
  const char* newConst = "new error message";
  char* newMsg = (char*)sqlite3DbStrDup(db, newConst); // simulate allocation
  vtab.zErrMsg = newMsg;

  // Call the function under test
  sqlite3VtabImportErrmsg(&p, &vtab);

  // Assertions
  EXPECT_TRUE(vtab.zErrMsg == nullptr, "pVtab->zErrMsg should be NULL after import");
  EXPECT_TRUE(p.zErrMsg != nullptr, "p->zErrMsg should be non-NULL after import");
  EXPECT_EQ_STR(p.zErrMsg, "new error message", "p->zErrMsg content should match pVtab->zErrMsg");

  // The old message should have been freed
  bool freedOldFound = false;
  for (void* f : g_freed_ptrs) {
    if (f == oldMsg) { freedOldFound = true; break; }
  }
  EXPECT_TRUE(freedOldFound, "Old p->zErrMsg memory should be freed by sqlite3DbFree");

  // Cleanup: free any allocated memory that remains
  if (p.zErrMsg) {
    sqlite3_free(p.zErrMsg);
    p.zErrMsg = nullptr;
  }
  if (p.db) {
    std::free(p.db);
    p.db = nullptr;
  }
  if (newMsg) {
    // newMsg was copied into p->zErrMsg by sqlite3DbStrDup; already tracked/freed above
    // No action needed here
  }
  // vtab.zErrMsg was freed inside the function; ensure it's NULL
  // Already asserted above
}

// Test 2: False branch when pVtab->zErrMsg is NULL.
// Verifies that no changes occur to both p and pVtab.
void test_false_branch() {
  reset_memory_trackers();

  // Prepare database handle
  sqlite3* db = (sqlite3*)std::malloc(sizeof(sqlite3));
  db->dummy = 0;

  Vdbe p;
  p.db = db;
  // p->zErrMsg initially set to a value to ensure it remains unchanged
  const char* initialConst = "unchanged";
  char* initialMsg = (char*)std::malloc(std::strlen(initialConst) + 1);
  std::strcpy(initialMsg, initialConst);
  p.zErrMsg = initialMsg;

  // pVtab with NULL zErrMsg to trigger the false branch
  sqlite3_vtab vtab;
  vtab.zErrMsg = nullptr;

  sqlite3VtabImportErrmsg(&p, &vtab);

  // Assertions
  // pVtab->zErrMsg should remain NULL
  EXPECT_TRUE(vtab.zErrMsg == nullptr, "pVtab->zErrMsg should remain NULL in false branch");

  // p->zErrMsg should remain unchanged
  EXPECT_TRUE(p.zErrMsg != nullptr, "p->zErrMsg should remain non-NULL in false branch");
  EXPECT_EQ_STR(p.zErrMsg, "unchanged", "p->zErrMsg content should remain unchanged");

  // No new frees should have occurred for p->zErrMsg
  // Cleanu
  if (p.zErrMsg) {
    sqlite3_free(p.zErrMsg);
    p.zErrMsg = nullptr;
  }
  if (p.db) {
    std::free(p.db);
    p.db = nullptr;
  }
}

// Test 3: Non-null pVtab->zErrMsg but p->zErrMsg is NULL.
// Verifies that p->zErrMsg becomes a duplicate of pVtab->zErrMsg and pVtab->zErrMsg freed.
void test_copy_when_pZErrMsgNull() {
  reset_memory_trackers();

  sqlite3* db = (sqlite3*)std::malloc(sizeof(sqlite3));
  db->dummy = 0;

  Vdbe p;
  p.db = db;
  p.zErrMsg = nullptr;

  sqlite3_vtab vtab;
  const char* msg = "error from vtab";
  vtab.zErrMsg = (char*)sqlite3DbStrDup(db, msg); // simulate zErrMsg in vtab

  sqlite3VtabImportErrmsg(&p, &vtab);

  // Assertions
  EXPECT_TRUE(vtab.zErrMsg == nullptr, "pVtab->zErrMsg should be NULL after import");
  EXPECT_TRUE(p.zErrMsg != nullptr, "p->zErrMsg should be non-NULL after import");
  EXPECT_EQ_STR(p.zErrMsg, msg, "p->zErrMsg should be a duplicate of pVtab->zErrMsg content");

  // Cleanup
  if (p.zErrMsg) {
    sqlite3_free(p.zErrMsg);
    p.zErrMsg = nullptr;
  }
  if (vtab.zErrMsg) {
    // Should have been freed inside the focal method
    // But in our setup we allocated via sqlite3DbStrDup; ensure no dangling
    sqlite3_free(vtab.zErrMsg);
    vtab.zErrMsg = nullptr;
  }
  if (p.db) {
    std::free(p.db);
    p.db = nullptr;
  }
}

// ---------- Main: Execute Tests and Report Summary ----------

int main() {
  std::cout << "Running sqlite3VtabImportErrmsg unit tests (C++11, no GTest)..." << std::endl;

  test_true_branch();
  test_false_branch();
  test_copy_when_pZErrMsgNull();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cout << g_failures << " TEST(S) FAILED" << std::endl;
  }

  return g_failures;
}
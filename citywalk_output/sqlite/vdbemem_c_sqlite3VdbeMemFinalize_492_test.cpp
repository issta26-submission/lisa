// Test suite for sqlite3VdbeMemFinalize in vdbemem.c
// This test suite is written in C++11 and does not rely on Google Test.
// It uses lightweight, non-terminating assertions and a small test harness.
// The tests exercise the focal function by providing a minimal SQLite in-memory DB
// and a dummy FuncDef.xFinalize implementation to observe behavior.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>


// Bring in C declarations from SQLite internals
extern "C" {
  // Public API
  #include "sqlite3.h"

  // Internal structures required by sqlite3VdbeMemFinalize
  // These headers define Mem, FuncDef and related internal fields used by the function under test.
  #include "sqliteInt.h"
  #include "vdbeInt.h"
}

// Lightweight test harness utilities
static int g_failed_tests = 0;
static int g_finalize_calls = 0;
static Mem *g_last_ctx_pMem = nullptr;

// Forward declarations of test-specific finalize handlers
static void finalize_ok(sqlite3_context *ctx);
static void finalize_error(sqlite3_context *ctx);

// Implementations of the two finalize handlers
static void finalize_ok(sqlite3_context *ctx) {
  // Record that finalize was invoked and which Mem was provided via ctx->pMem
  g_finalize_calls++;
  g_last_ctx_pMem = ctx->pMem;
  // Indicate success (no error)
  ctx->isError = 0;
}

static void finalize_error(sqlite3_context *ctx) {
  // Record invocation and simulate an error from xFinalize
  g_finalize_calls++;
  ctx->isError = 1;
  // Do not modify pOut; the copy-back behavior is still exercised by the caller
}

// Simple non-terminating assertion macro
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "Assertion failed: " << (msg) \
              << " (function: " << __FUNCTION__ << ", line: " << __LINE__ << ")" << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

// Test 1: Normal path where xFinalize does not signal an error.
// The Mem should be copied from a zeroed "t" (MEM_Null) into the input Mem.
// The function should return 0 and should have invoked the xFinalize exactly once.
static void test_sqlite3VdbeMemFinalize_normal_path() {
  // Prepare in-memory SQLite DB
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_open failed");
  if(db == nullptr || rc != SQLITE_OK) return;

  // Lock the mutex to satisfy sqlite3_mutex_held requirement
  sqlite3_mutex_enter(db->mutex);

  // Prepare Mem structure as input
  Mem m;
  std::memset(&m, 0, sizeof(Mem)); // zero all fields
  m.db = db;
  m.flags = MEM_Null;            // This guarantees the predicate (pMem->flags & MEM_Null)!=0 is true
  m.szMalloc = 0;
  m.zMalloc = nullptr;
  // m.u.pDef is not used in this test since MEM_Null makes the predicate true

  // Prepare FuncDef with xFinalize implemented by finalize_ok
  FuncDef func;
  // Clear and set up function pointer
  std::memset(&func, 0, sizeof(FuncDef));
  func.xFinalize = finalize_ok; // xFinalize should be called

  // Reset observable hooks
  g_finalize_calls = 0;
  g_last_ctx_pMem = nullptr;

  // Call the focal function
  int isError = sqlite3VdbeMemFinalize(&m, &func);

  // Unlock mutex
  sqlite3_mutex_leave(db->mutex);

  // Assertions
  TEST_ASSERT(isError == 0, "Expected isError == 0 for normal path");
  TEST_ASSERT(g_finalize_calls == 1, "Expected finalize to be called exactly once");
  TEST_ASSERT(g_last_ctx_pMem == &m, "ctx.pMem should point to the original Mem passed in");
  TEST_ASSERT((m.flags & MEM_Null) != 0, "After finalize, pMem should reflect MEM_Null (copied from t)");
  TEST_ASSERT(m.db == db, "Mem.db should remain pointing to the original db");

  // Cleanup
  sqlite3_close(db);
}

// Test 2: Error path where xFinalize sets an error in the context.
// The function should propagate the error via the return value.
static void test_sqlite3VdbeMemFinalize_error_path() {
  // Prepare in-memory SQLite DB
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_open failed (error path)");
  if(db == nullptr || rc != SQLITE_OK) return;

  // Lock mutex to satisfy assertion
  sqlite3_mutex_enter(db->mutex);

  // Prepare Mem structure as input
  Mem m;
  std::memset(&m, 0, sizeof(Mem));
  m.db = db;
  m.flags = MEM_Null;            // Predicate true; ensures we do not trigger assertion failure
  m.szMalloc = 0;
  m.zMalloc = nullptr;

  // Prepare FuncDef with xFinalize implemented by finalize_error
  FuncDef func;
  std::memset(&func, 0, sizeof(FuncDef));
  func.xFinalize = finalize_error;

  // Reset observables
  g_finalize_calls = 0;
  g_last_ctx_pMem = nullptr;

  // Call the focal function
  int isError = sqlite3VdbeMemFinalize(&m, &func);

  // Unlock mutex
  sqlite3_mutex_leave(db->mutex);

  // Assertions
  TEST_ASSERT(isError == 1, "Expected isError == 1 when xFinalize reports an error");
  TEST_ASSERT(g_finalize_calls == 1, "Finalize should be invoked exactly once");
  TEST_ASSERT(g_last_ctx_pMem == &m, "ctx.pMem should point to the original Mem passed in");
  TEST_ASSERT((m.flags & MEM_Null) != 0, "pMem should reflect MEM_Null after finalize");

  // Cleanup
  sqlite3_close(db);
}

// Entry-point for running tests
int main() {
  std::cout << "Starting sqlite3VdbeMemFinalize unit tests (C++11, no GTest)..." << std::endl;

  test_sqlite3VdbeMemFinalize_normal_path();
  test_sqlite3VdbeMemFinalize_error_path();

  // Summary
  if(g_failed_tests == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << g_failed_tests << " test(s) failed." << std::endl;
    return 1;
  }
}

/*
Notes and rationale:

- Candidate Keywords considered:
  - Mem, pMem, pFunc, FuncDef, xFinalize, MEM_Null, MEM_Dyn, zMalloc, szMalloc, db, mutex, ENC, sqlite3DbFreeNN, memcpy, isError
  - These reflect core dependencies and branches within sqlite3VdbeMemFinalize:
    - Preconditions asserted at the start (non-null pointers, correct ownership of mutex)
    - The MEM_Null predicate vs pMem->u.pDef predicate used to decide whether to proceed
    - Finalization path manipulates a temporary Mem t and copies it into pMem
    - Potential memory deallocation when szMalloc > 0
    - Return value is based on ctx.isError set by xFinalize

- Static and private members:
  - The test uses internal types Mem and FuncDef provided by the included headers (sqliteInt.h, vdbeInt.h).
  - Static finalize handlers (finalize_ok, finalize_error) are file-local in this test file and not exposed publicly.

- Domain knowledge considerations:
  - The test uses non-terminating assertions (TEST_ASSERT) to continue execution after failures.
  - We rely on the public SQLite C API (sqlite3_open) and internal types for Mem/FuncDef to exercise the exact control flow in sqlite3VdbeMemFinalize.
  - The tests operate under the same namespace and avoid GMock/GTest; test functions are invoked from main.

- Test coverage goals:
  - Normal path: xFinalize does not signal error; verify isError == 0, final Mem is consistent, and that the finalize hook was invoked exactly once.
  - Error path: xFinalize signals error; verify isError == 1 and that the return value propagates this error.

- Static and scope considerations:
  - The test accesses internal structures (Mem, FuncDef) via the included SQLite internal headers. If different SQLite versions reorganize internals, this test may require alignment with header definitions, but it adheres to the FOCAL_CLASS_DEP context provided by the prompt.

- Note on environment:
  - This test links against the system SQLite library and uses its internal headers. Ensure your build environment provides sqlite3.h, sqliteInt.h, and vdbeInt.h, and links with -lsqlite3. The test relies on the mutex in the in-memory DB to satisfy sqlite3_mutex_held checks. If your environment differs, adjust accordingly.
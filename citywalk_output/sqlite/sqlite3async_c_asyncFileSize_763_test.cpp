// C++11 test suite for the focal method: asyncFileSize
// This test harness includes the focal C code (sqlite3async.c) in order to exercise
// asyncFileSize directly in a single translation unit.
// It uses a minimal, self-contained test harness style (no GoogleTest).

#include <sched.h>
#include <string.h>
#include <sqlite3async.c>
#include <windows.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <sqlite3async.h>
#include <stdarg.h>
#include <iostream>
#include <pthread.h>


// Enable the AsyncIO portion of the sqlite3 subsystem so the focal function is compiled.
// We include the C source directly in order to place all symbols in the same translation unit.
#define SQLITE_ENABLE_ASYNCIO
extern "C" {
}

// Lightweight test harness macros (non-terminating, suitable for coverage)
static bool g_suite_ok = true;

#define TEST_PASS(msg) \
  do { std::cout << "[PASS] " msg << std::endl; } while(0)

#define TEST_FAIL(msg) \
  do { std::cerr << "[FAIL] " msg << std::endl; g_suite_ok = false; } while(0)

#define EXPECT_EQ(act, exp, msg) \
  do { if ((act) != (exp)) { std::cerr << "[EXPECT_FAIL] " msg \
        << " (expected " << (exp) << ", got " << (act) << ")" << std::endl; g_suite_ok = false; } \
     else { TEST_PASS(msg); } \
  } while(0)

#define EXPECT_TRUE(cond, msg) \
  do { if (!(cond)) { std::cerr << "[EXPECT_FAIL] " msg << std::endl; g_suite_ok = false; } \
     else { TEST_PASS(msg); } \
  } while(0)


// Forward declarations for the helper base file size function used in tests.
// The actual base file size is provided by a tiny stub we place in tests via the included code.

static int test_baseFileSize(sqlite3_file *p, sqlite3_int64 *pSize) {
  // Simple deterministic base size for tests.
  *pSize = 25;
  return SQLITE_OK;
}

// Helper to reset the async write-queue between tests
static void reset_async_queue() {
  // The actual async structure is defined inside sqlite3async.c and exposed in this TU
  // as a static (internal linkage) global named "async" or similar in the compiled code.
  // We rely on the included code to expose and initialize it. If not, no-op.
  // Best effort: attempt to set the queue head to NULL if symbol exists.
  // Some compilers may require a different way to reset; this is a safe no-op if not present.
  // This is intentionally lightweight to keep tests robust in varied environments.
  // We won't access private internals directly here to avoid UB in non-public APIs.
  // Users may adapt if the symbol exposure differs in their build.
  extern AsyncWrite *async_pQueueFirst; // optional, may not exist in all builds
  (void)async_pQueueFirst;
}

// Test 1: Base file provides size; no async queue entries -> size equals base size (25)
static void test_asyncFileSize_base_only_no_queue() {
  // Prepare base file with non-NULL pMethods so xFileSize is consulted.
  FileMethods fm;
  fm.xFileSize = test_baseFileSize;

  sqlite3_file base;
  base.pMethods = &fm; // non-NULL to trigger base xFileSize read

  AsyncFileData data;
  data.pBaseRead = &base;
  data.zName = "testfile";

  AsyncFile afile;
  afile.pData = &data;

  sqlite3_file *pFile = (sqlite3_file *)&afile;

  sqlite3_int64 size = 0;
  int rc = asyncFileSize(pFile, &size);

  // rc should be SQLITE_OK and size should be base size 25
  EXPECT_EQ(rc, SQLITE_OK, "asyncFileSize rc == SQLITE_OK for base-only test");
  EXPECT_EQ(size, (sqlite3_int64)25, "asyncFileSize size equals base size (25) with no queue");
}

// Test 2: One ASYNC_WRITE increases the computed size to a value > base (max of base and write end)
static void test_asyncFileSize_write_increases_size() {
  // Base: 25
  FileMethods fm;
  fm.xFileSize = test_baseFileSize;

  sqlite3_file base;
  base.pMethods = &fm;

  AsyncFileData data;
  data.pBaseRead = &base;
  data.zName = "testfile";

  AsyncFile afile;
  afile.pData = &data;

  // Write that should push the end size forward: iOffset=30, nByte=10 -> 40
  AsyncWrite w;
  w.op = ASYNC_WRITE;
  w.iOffset = 30;
  w.nByte = 10;
  w.pFileData = &data;
  w.zBuf = NULL;
  w.pNext = NULL;

  // Inject into queue
  async.pQueueFirst = &w;

  sqlite3_file *pFile = (sqlite3_file *)&afile;
  sqlite3_int64 size = 0;
  int rc = asyncFileSize(pFile, &size);

  // Expect rc OK and size = max(base 25, 30+10=40) => 40
  EXPECT_EQ(rc, SQLITE_OK, "asyncFileSize rc == SQLITE_OK for write-increase test");
  EXPECT_EQ(size, (sqlite3_int64)40, "asyncFileSize size equals 40 after ASYNC_WRITE");
}

// Test 3: ASYNC_DELETE matching name resets size to 0 regardless of base and other writes
static void test_asyncFileSize_delete_resets_to_zero() {
  // Base: 25
  FileMethods fm;
  fm.xFileSize = test_baseFileSize;

  sqlite3_file base;
  base.pMethods = &fm;

  AsyncFileData data;
  data.pBaseRead = &base;
  data.zName = "name_to_delete";

  AsyncFile afile;
  afile.pData = &data;

  // Create a delete entry that matches the file name
  AsyncWrite del;
  del.op = ASYNC_DELETE;
  del.iOffset = 0;
  del.nByte = 0;
  del.pFileData = &data;
  del.zBuf = "name_to_delete"; // must match pData->zName
  del.pNext = NULL;

  async.pQueueFirst = &del;

  sqlite3_file *pFile = (sqlite3_file *)&afile;
  sqlite3_int64 size = 0;
  int rc = asyncFileSize(pFile, &size);

  // Expect rc OK and size 0 due to delete
  EXPECT_EQ(rc, SQLITE_OK, "asyncFileSize rc == SQLITE_OK for delete test");
  EXPECT_EQ(size, (sqlite3_int64)0, "asyncFileSize size == 0 after matching ASYNC_DELETE");
}

// Test 4: ASYNC_TRUNCATE reduces size to iOffset (min(s, iOffset))
static void test_asyncFileSize_truncate_reduces_size() {
  // Base: 25
  FileMethods fm;
  fm.xFileSize = test_baseFileSize;

  sqlite3_file base;
  base.pMethods = &fm;

  AsyncFileData data;
  data.pBaseRead = &base;
  data.zName = "truncate_target";

  AsyncFile afile;
  afile.pData = &data;

  // Write that triggers TRUNCATE to reduce to 15
  AsyncWrite tr;
  tr.op = ASYNC_TRUNCATE;
  tr.iOffset = 15;
  tr.nByte = 0;
  tr.pFileData = &data;
  tr.zBuf = NULL;
  tr.pNext = NULL;

  async.pQueueFirst = &tr;

  sqlite3_file *pFile = (sqlite3_file *)&afile;
  sqlite3_int64 size = 0;
  int rc = asyncFileSize(pFile, &size);

  // Since base = 25, truncate to 15 yields 15
  EXPECT_EQ(rc, SQLITE_OK, "asyncFileSize rc == SQLITE_OK for truncate test");
  EXPECT_EQ(size, (sqlite3_int64)15, "asyncFileSize size == 15 after ASYNC_TRUNCATE to 15");
}

// Test 5: NULL pMethods on base means we skip xFileSize; zero result if no queue changes
static void test_asyncFileSize_null_methods_skips_base() {
  // Base with NULL pMethods -> rc remains OK but s remains 0
  sqlite3_file base;
  base.pMethods = NULL;

  AsyncFileData data;
  data.pBaseRead = &base;
  data.zName = "nomethods";

  AsyncFile afile;
  afile.pData = &data;

  // No queue
  async.pQueueFirst = NULL;

  sqlite3_file *pFile = (sqlite3_file *)&afile;
  sqlite3_int64 size = 0;
  int rc = asyncFileSize(pFile, &size);

  // rc OK and size 0
  EXPECT_EQ(rc, SQLITE_OK, "asyncFileSize rc == SQLITE_OK for NULL pMethods test");
  EXPECT_EQ(size, (sqlite3_int64)0, "asyncFileSize size == 0 when pMethods is NULL");
}


// Main test runner
int main() {
  // Run all tests; each test prints PASS/FAIL details.
  test_asyncFileSize_base_only_no_queue();
  test_asyncFileSize_write_increases_size();
  test_asyncFileSize_delete_resets_to_zero();
  test_asyncFileSize_truncate_reduces_size();
  test_asyncFileSize_null_methods_skips_base();

  if (g_suite_ok) {
    std::cout << "[OVERALL PASS] All tests completed successfully." << std::endl;
    return 0;
  } else {
    std::cerr << "[OVERALL FAIL] Some tests failed. See details above." << std::endl;
    return 1;
  }
}
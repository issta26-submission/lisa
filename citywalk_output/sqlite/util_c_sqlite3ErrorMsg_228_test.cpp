// Test harness for the focal method: sqlite3ErrorMsg
// This single-file test includes a minimal in-repo implementation of the focal function
// along with lightweight stubs and scaffolding to exercise true/false branches.
// It is designed for C++11 compilation without GTest, using a simple in-process test runner.
// Explanatory comments accompany each unit test.

#include <math.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain knowledge notes applied:
// - Use only C++ standard library.
// - Do not rely on private/privateized members outside the test scope.
// - Provide a minimal, self-contained environment to exercise sqlite3ErrorMsg.
// - Use plain asserts and print results; no GTest or GoogleMock.
// - Treat static functions as internal; test through public-like entry points here.


// Minimal, self-contained type definitions to mirror the focal environment.

typedef struct Parse Parse;
typedef struct sqlite3 sqlite3;

// Simulated constants from SQLite
#define SQLITE_ERROR 1
#define SQLITE_NOMEM 7

// Lightweight 'sqlite3' structure with only fields used by sqlite3ErrorMsg
struct sqlite3 {
  Parse* pParse;        // back-pointer to current Parse
  int errByteOffset;    // used to indicate error byte offset
  int suppressErr;      // if true, suppress error messages
  int mallocFailed;     // indicates a malloc failure occurred
};

// Lightweight 'Parse' structure with only necessary fields
struct Parse {
  sqlite3* db;            // back-pointer to database object
  int nErr;                // number of errors recorded
  int rc;                  // return code
  char* zErrMsg;            // current error message
  void* pWith;               // helper pointer (set to 0 by sqlite3ErrorMsg)
  Parse* pToplevel;          // toplevel parse pointer
};

// Forward declarations of the functions under test environment
extern "C" {
  // The focal function under test
  void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...);

  // Mocks used by sqlite3ErrorMsg (via va_list)
  char* sqlite3VMPrintf(sqlite3* db, const char *zFormat, va_list ap);

  // Memory management shim used by the focal function
  void sqlite3DbFree(sqlite3* db, void* p);
}

// Global state to aid testing the internal behavior
static char* g_lastAllocatedZMsg = nullptr;  // last pointer returned by sqlite3VMPrintf
static int g_freeCallCount = 0;              // counts how many times sqlite3DbFree was called

// Mock implementations to satisfy dependencies in the focal method's flow.
// These are intentionally minimal and self-contained.

extern "C" {

// Allocate and format a string using a va_list, mirroring sqlite3VMPrintf semantics.
// We allocate with malloc to emulate sqlite3's memory model.
char* sqlite3VMPrintf(sqlite3* db, const char *zFormat, va_list ap){
  (void)db; // not used in mock except for interface compatibility
  va_list apCopy;
  va_copy(apCopy, ap);
  int len = vsnprintf(nullptr, 0, zFormat, apCopy);
  va_end(apCopy);
  if (len < 0) len = 0;
  char* z = (char*)malloc(len + 1);
  if (z) {
    vsnprintf(z, len + 1, zFormat, ap);
  }
  g_lastAllocatedZMsg = z;
  return z;
}

// Free memory with a counter to help verify memory management in tests.
// In a real SQLite build, this would be sqlite3DbFree, which may also perform pooling.
// Here we simply free and increment a counter to indicate a deallocation occurred.
void sqlite3DbFree(sqlite3* db, void* p){
  (void)db; // keep interface consistent; not used in mock
  if (p) {
    free(p);
  }
  // Track frees for test verification
  g_freeCallCount++;
  // If the freed pointer matches the last allocated zMsg, clear it to help test introspection
  if (p == (void*)g_lastAllocatedZMsg) {
    g_lastAllocatedZMsg = nullptr;
  }
}
} // extern "C"

// Implementation of the focal method under test.
// This mirrors the exact logic provided in the prompt.
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...){
  char *zMsg;
  va_list ap;
  sqlite3 *db = pParse->db;
  assert( db!=0 );
  assert( db->pParse==pParse || db->pParse->pToplevel==pParse );
  db->errByteOffset = -2;
  va_start(ap, zFormat);
  zMsg = sqlite3VMPrintf(db, zFormat, ap);
  va_end(ap);
  if( db->errByteOffset<-1 ) db->errByteOffset = -1;
  if( db->suppressErr ){
    sqlite3DbFree(db, zMsg);
    if( db->mallocFailed ){
      pParse->nErr++;
      pParse->rc = SQLITE_NOMEM;
    }
  }else{
    pParse->nErr++;
    sqlite3DbFree(db, pParse->zErrMsg);
    pParse->zErrMsg = zMsg;
    pParse->rc = SQLITE_ERROR;
    pParse->pWith = 0;
  }
}

// Simple test harness (non-GTest) to exercise two critical branches:
// 1) suppressErr true: zMsg is freed immediately; only malloc failure would affect pParse.
// 2) suppressErr false: old zErrMsg is freed, zMsg assigned to zErrMsg, rc set to SQLITE_ERROR.
int main() {
  int tests_passed = 0;
  int total_tests = 0;

  auto reset_globals = [](){
    g_lastAllocatedZMsg = nullptr;
    g_freeCallCount = 0;
  };

  // Helper: pretty print test results
  auto fail = [](const char* name, const char* reason) {
    std::cerr << "TEST FAILED: " << name << " -- " << reason << "\n";
  };

  // Test 1: suppressErr is true; mallocFailed is false.
  // Expectation:
  // - db->errByteOffset becomes -1 (due to the internal overwrite to -2, then clamp to -1)
  // - pParse->nErr remains unchanged (0)
  // - pParse->rc remains unchanged
  // - pParse->zErrMsg remains as before (no update)
  // - zMsg is freed via sqlite3DbFree, so g_freeCallCount should be 1 and g_lastAllocatedZMsg cleared
  {
    reset_globals();
    total_tests++;

    // Setup
    Parse pParse;
    pParse.nErr = 0;
    pParse.rc = 0;
    pParse.zErrMsg = (char*)malloc(10);
    strcpy(pParse.zErrMsg, "OLD");
    pParse.pWith = nullptr;
    pParse.pToplevel = nullptr;

    sqlite3 dbInstance;
    dbInstance.pParse = &pParse;
    dbInstance.errByteOffset = 0;
    dbInstance.suppressErr = 1;     // branch: suppressErr
    dbInstance.mallocFailed = 0;     // not malloc failed

    // Linkage: Ensure constraints for the assert
    pParse.db = &dbInstance;

    // Call focal method
    sqlite3ErrorMsg(&pParse, "test suppress %d", 1);

    // Assertions
    bool ok = true;
    // After call, errByteOffset must be -1 (due to clamp)
    if (dbInstance.errByteOffset != -1) {
      ok = false;
    }
    // nErr should remain 0
    if (pParse.nErr != 0) {
      ok = false;
    }
    // rc should remain unchanged (0)
    if (pParse.rc != 0) {
      ok = false;
    }
    // zErrMsg should still be the old one (not replaced)
    if (pParse.zErrMsg == nullptr || strcmp(pParse.zErrMsg, "OLD") != 0) {
      ok = false;
    }
    // The newly created zMsg should have been freed
    if (g_freeCallCount != 1) {
      ok = false;
    }
    if (g_lastAllocatedZMsg != nullptr) {
      // If not cleared by sqlite3DbFree, we consider failure
      ok = false;
    }

    if (ok) {
      std::cout << "PASS: sqlite3ErrorMsg_SuppressTrue_NoMallocFailure\n";
      tests_passed++;
    } else {
      fail("sqlite3ErrorMsg_SuppressTrue_NoMallocFailure", "Unexpected state in suppressErr-true test");
    }

    // Cleanup
    if (pParse.zErrMsg) free(pParse.zErrMsg);
    if (pParse.zErrMsg != nullptr && pParse.zErrMsg[0] != '\0') {
      // nothing
    }
  }

  // Test 2: suppressErr is false; ensure proper error propagation and message replacement.
  // Expectation:
  // - pParse->nErr increments to 1
  // - pParse->rc set to SQLITE_ERROR
  // - pParse->zErrMsg freed (old one) and replaced with new zMsg from sqlite3VMPrintf
  // - pParse->pWith set to 0
  // - old zErrMsg memory freed (g_freeCallCount increments)
  // - db->errByteOffset becomes -1
  {
    reset_globals();
    total_tests++;

    // Setup
    Parse pParse;
    pParse.nErr = 0;
    pParse.rc = 0;
    // Old error message
    pParse.zErrMsg = (char*)malloc(20);
    strcpy(pParse.zErrMsg, "OLD_ERROR");
    pParse.pWith = (void*)0x1234;
    pParse.pToplevel = nullptr;

    sqlite3 dbInstance;
    dbInstance.pParse = &pParse;
    dbInstance.errByteOffset = -5; // initial value won't survive; overwritten to -2 and then -1
    dbInstance.suppressErr = 0;    // branch: not suppressing
    dbInstance.mallocFailed = 0;

    // Linkage
    pParse.db = &dbInstance;

    // Call focal method
    sqlite3ErrorMsg(&pParse, "formatted: %d + %s", 42, "answer");

    // Assertions
    bool ok = true;
    // nErr should be 1
    if (pParse.nErr != 1) ok = false;
    // rc should be SQLITE_ERROR
    if (pParse.rc != SQLITE_ERROR) ok = false;
    // pWith should be cleared (0)
    if (pParse.pWith != 0) ok = false;
    // zErrMsg should be non-null and equal to g_lastAllocatedZMsg
    if (pParse.zErrMsg == nullptr || g_lastAllocatedZMsg == nullptr) ok = false;
    if (pParse.zErrMsg != g_lastAllocatedZMsg) ok = false;
    // old zErrMsg should have been freed
    if (g_freeCallCount != 1) ok = false;
    // errByteOffset should be -1
    if (dbInstance.errByteOffset != -1) ok = false;

    // Optional: verify formatting content (safe because we control formatter)
    if (ok) {
      // The formatted string should contain "formatted: 42 + 0" and "42" is present
      // We can't rely on exact formatting due to locale, but we can reuse the buffer stored in zMsg.
      // Since g_lastAllocatedZMsg points to the current zMsg, verify its content.
      if (pParse.zErrMsg && strstr(pParse.zErrMsg, "formatted: 42 +") == nullptr) {
        ok = false;
      }
    }

    if (ok) {
      std::cout << "PASS: sqlite3ErrorMsg_NotSuppress_SetsErrorAndReplacesZMsg\n";
      tests_passed++;
    } else {
      fail("sqlite3ErrorMsg_NotSuppress_SetsErrorAndReplacesZMsg", "Unexpected state in non-suppress test");
    }

    // Cleanup
    if (pParse.zErrMsg) {
      // zErrMsg has been assigned to zMsg (which was allocated by sqlite3VMPrintf)
      // Do not double-free here; sqlite3ErrorMsg freed the old zErrMsg but left the new one owned by pParse
      // Free the new one to avoid leaks
      free(pParse.zErrMsg);
    }
  }

  // Final result
  std::cout << "Tests passed: " << tests_passed << " / " << total_tests << "\n";
  return (tests_passed == total_tests) ? 0 : 1;
}
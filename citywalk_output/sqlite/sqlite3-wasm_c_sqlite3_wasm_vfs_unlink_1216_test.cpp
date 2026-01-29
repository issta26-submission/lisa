// Minimal C++11 test harness for sqlite3_wasm_vfs_unlink
// This test does not rely on GoogleTest. It uses simple boolean
// pass/fail reporting and non-terminating assertions pattern.
// The production-like function sqlite3_wasm_vfs_unlink is reproduced here
// with necessary stubs to enable isolated unit testing of its logic.

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <iostream>


// Domain knowledge: provide minimal reproductions of required SQLite symbols
// and types to exercise sqlite3_wasm_vfs_unlink in isolation.

#define SQLITE_OK 0
#define SQLITE_MISUSE -1

// Lightweight stand-in for the real sqlite3_vfs struct used by the function under test.
typedef struct sqlite3_vfs {
  int (*xDelete)(struct sqlite3_vfs*, const char*, int);
} sqlite3_vfs;

// Forward declaration of the function under test (as in the focal file)
extern "C" int sqlite3_wasm_vfs_unlink(sqlite3_vfs *pVfs, const char *zName);

// Global hook to control what sqlite3_vfs_find returns (mocked for tests)
static sqlite3_vfs* g_find_result = nullptr;

// Mocked sqlite3_vfs_find used by sqlite3_wasm_vfs_unlink when pVfs is null
extern "C" sqlite3_vfs* sqlite3_vfs_find(int){
  return g_find_result;
}

// Implementation of the focal function under test (copied/adapted for unit tests)
extern "C" int sqlite3_wasm_vfs_unlink(sqlite3_vfs *pVfs, const char *zName){
  int rc = SQLITE_MISUSE; // default on misuse
  if( 0==pVfs && 0!=zName ) pVfs = sqlite3_vfs_find(0);
  if( zName && pVfs && pVfs->xDelete ){
    rc = pVfs->xDelete(pVfs, zName, 1);
  }
  return rc;
}

// ----------------- Test infrastructure and mocks -----------------

// Global capture for testing xDelete invocations
static const char* g_last_xDelete_name = nullptr;
static sqlite3_vfs* g_last_xDelete_vfs = nullptr;

// Simple xDelete implementations to drive various test scenarios

// 1) xDelete returns success (0) and records call
static int vfs_ok_xDelete(sqlite3_vfs* pvfs, const char* zName, int del){
  g_last_xDelete_vfs = pvfs;
  g_last_xDelete_name = zName;
  return 0;
}

// 2) xDelete returns non-zero (e.g., 7) and records call
static int vfs_nonok_xDelete(sqlite3_vfs* pvfs, const char* zName, int del){
  g_last_xDelete_vfs = pvfs;
  g_last_xDelete_name = zName;
  return 7;
}

// 3) xDelete is NULL (not callable) – should not be invoked
// (no function needed; we'll assign NULL to simulate this)

// Test VFS instances (distinct per scenario)
static sqlite3_vfs vfs_ok = { vfs_ok_xDelete };
static sqlite3_vfs vfs_nonok = { vfs_nonok_xDelete };
static sqlite3_vfs vfs_null_delete = { nullptr };

// VFS instance returned by sqlite3_vfs_find(0) when pVfs is null
static sqlite3_vfs vfs_found = { vfs_ok_xDelete };

// Reset helper for tests
static void reset_test_state(){
  g_last_xDelete_name = nullptr;
  g_last_xDelete_vfs = nullptr;
  g_find_result = nullptr;
}

// ----------------- Tests -----------------

// Helper macro for simple non-terminating checks
#define TEST_PASSED  (test_passed = true)
#define TEST_FAILED  (test_passed = false)

static bool test_passed;

// Test 1: When pVfs is non-null and zName is non-null and xDelete exists, sqlite3_wasm_vfs_unlink should
// call xDelete with the correct parameters and return its result (0).
static bool test_basic_call_calls_xDelete_and_propagates_rc(){
  reset_test_state();
  // Arrange
  g_find_result = nullptr; // not used in this path
  const char* name = "sample.txt";

  // Act
  int rc = sqlite3_wasm_vfs_unlink(&vfs_ok, name);

  // Assert
  bool ok = (rc == 0)
            && (g_last_xDelete_vfs == &vfs_ok)
            && (g_last_xDelete_name != nullptr && std::strcmp(g_last_xDelete_name, name) == 0);

  return ok;
}

// Test 2: When xDelete is NULL, the function should return SQLITE_MISUSE and not crash.
static bool test_no_xDelete_no_call_results_in_misuse(){
  reset_test_state();
  const char* name = "file.txt";

  int rc = sqlite3_wasm_vfs_unlink(&vfs_null_delete, name);

  bool ok = (rc == SQLITE_MISUSE) && (g_last_xDelete_vfs == nullptr);

  return ok;
}

// Test 3: When pVfs is NULL and zName non-null, the function should call sqlite3_vfs_find(0)
// which we mock by returning g_find_result; verify that xDelete is invoked and rc propagates.
static bool test_null_pVfs_uses_find_and_propagates_rc_from_find(){
  reset_test_state();
  const char* name = "from_find.txt";

  // Arrange: configure find() to return vfs_found whose xDelete is vfs_ok_xDelete
  g_find_result = &vfs_found;
  int rc = sqlite3_wasm_vfs_unlink(nullptr, name);

  bool ok = (rc == 0)
            && (g_last_xDelete_vfs == &vfs_found)
            && (g_last_xDelete_name != nullptr && std::strcmp(g_last_xDelete_name, name) == 0);

  return ok;
}

// Test 4: When zName is NULL, regardless of pVfs, rc should be SQLITE_MISUSE
static bool test_null_zName_results_in_misuse(){
  reset_test_state();

  // pVfs is valid but zName is NULL
  int rc = sqlite3_wasm_vfs_unlink(&vfs_ok, nullptr);

  bool ok = (rc == SQLITE_MISUSE) && (g_last_xDelete_vfs == nullptr);

  return ok;
}

// Test 5: When xDelete returns non-zero, that code should be propagated
static bool test_xDelete_returns_nonzero_propagates(){
  reset_test_state();

  // pVfs non-null with xDelete returning non-zero
  // Reuse vfs_nonok for this path
  const char* name = "will_fail.txt";

  int rc = sqlite3_wasm_vfs_unlink(&vfs_nonok, name);

  bool ok = (rc == 7)
            && (g_last_xDelete_vfs == &vfs_nonok)
            && (g_last_xDelete_name != nullptr && std::strcmp(g_last_xDelete_name, name) == 0);

  return ok;
}

// ----------------- Main runner -----------------

int main(){
  int total = 5;
  int passed = 0;

  // Run tests and print results with explanatory comments
  // Test 1
  {
    // Explanatory: true path where pVfs is provided and xDelete exists.
    bool t = test_basic_call_calls_xDelete_and_propagates_rc();
    std::cout << "Test 1 - basic call with valid pVfs and xDelete: " 
              << (t ? "PASS" : "FAIL") << "\n";
    if(t) passed++;
  }

  // Test 2
  {
    bool t = test_no_xDelete_no_call_results_in_misuse();
    std::cout << "Test 2 - xDelete NULL should yield SQLITE_MISUSE: "
              << (t ? "PASS" : "FAIL") << "\n";
    if(t) passed++;
  }

  // Test 3
  {
    bool t = test_null_pVfs_uses_find_and_propagates_rc_from_find();
    std::cout << "Test 3 - null pVfs uses sqlite3_vfs_find result and propagates rc: "
              << (t ? "PASS" : "FAIL") << "\n";
    if(t) passed++;
  }

  // Test 4
  {
    bool t = test_null_zName_results_in_misuse();
    std::cout << "Test 4 - null zName yields SQLITE_MISUSE: "
              << (t ? "PASS" : "FAIL") << "\n";
    if(t) passed++;
  }

  // Test 5
  {
    bool t = test_xDelete_returns_nonzero_propagates();
    std::cout << "Test 5 - non-zero rc from xDelete propagates: "
              << (t ? "PASS" : "FAIL") << "\n";
    if(t) passed++;
  }

  total = 5;
  std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

  return (passed == total) ? 0 : 1;
}
// Test suite for sqlite3RtreeInit in rtree.c using custom Mocked sqlite3 API
// Target: C++11, no GoogleTest, no GMock. Tests run from main().
// The tests simulate different execution paths (success, early failure, geo path).

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <sqlite3rtree.h>
#include <iostream>
#include <algorithm>


// Lightweight test harness: non-terminating assertions
static int g_tests_failed = 0;
#define TEST_ASSERT(cond, msg) \
  do { if(!(cond)) { std::cerr << "TEST FAILURE: " << (msg) \
  << " in " << __FUNCTION__ << "\n"; ++g_tests_failed; } } while(0)


// Domain knowledge: provide minimum constants/aliases used by focal code
#define SQLITE_OK 0
#define SQLITE_UTF8 1

// Geometry macro placeholders (values only matter as pointer casts in rtree.c)
#define RTREE_COORD_INT32 0x10
#define RTREE_COORD_REAL32 0x20

// Forward declarations for sqlite3 mock types to resemble sqlite3.h interfaces
struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;
struct sqlite3_module;

// Function pointer types matching sqlite3_create_function requirements
typedef void (*sqlite3_xFunc)(sqlite3_context*, int, sqlite3_value**);
typedef void (*sqlite3_xStep)(sqlite3_context*, int, sqlite3_value**);
typedef void (*sqlite3_xFinal)(sqlite3_value*);

// Extern "C" mocks to replace real SQLite API during tests
extern "C" {

// Minimal opaque types (definitions provided by rtree.c in real build)
struct sqlite3 {};
struct sqlite3_module { int dummy; }; // dummy placeholder for module descriptor

// Mocks for SQLite API used by sqlite3RtreeInit
int sqlite3_create_function(sqlite3* db, const char* zFunctionName, int nArg, int eTextRep,
                            void* pApp, sqlite3_xFunc xFunc, sqlite3_xStep xStep, sqlite3_xFinal xFinal);

int sqlite3_create_module_v2(sqlite3* db, const char* zName, const sqlite3_module* pModule,
                             void* pClientData, void* (*xDestroy)(void*));

int sqlite3_geopoly_init(sqlite3* db);

// We do not need to implement the real rtreeModule here; assume it exists in rtree.c
// extern sqlite3_module rtreeModule; // provided by the system under test

} // extern "C"


// Global test state to verify control flow and calls
static std::vector<int> g_func_rc_queue;       // rc values for sequential sqlite3_create_function calls
static std::vector<int> g_module_rc_queue;     // rc values for sequential sqlite3_create_module_v2 calls
static std::vector<int> g_geo_rc_queue;        // rc values for sqlite3_geopoly_init (when compiled with GEO)

static int g_func_calls = 0;
static int g_module_calls = 0;
static int g_geo_calls = 0;
static std::vector<std::string> g_module_names; // captured module names passed to sqlite3_create_module_v2

// Helper to reset per-test state
static void reset_mock_state() {
  g_func_rc_queue.clear();
  g_module_rc_queue.clear();
  g_geo_rc_queue.clear();
  g_func_calls = 0;
  g_module_calls = 0;
  g_geo_calls = 0;
  g_module_names.clear();
}

// Mock implementations: records calls, returns configured RCs, captures module names
static int pop_rc(std::vector<int>& q) {
  if(q.empty()) return -1; // -1 means default SQLITE_OK
  int v = q.front();
  q.erase(q.begin());
  return v;
}

extern "C" {

// Provide C-linkage mocks so that the linker resolves to these functions during test.
// They simulate the actual sqlite3_create_function/module_v2 and geopoly init.

int sqlite3_create_function(sqlite3* /*db*/, const char* zFunctionName, int /*nArg*/, int /*eTextRep*/,
                          void* /*pApp*/, sqlite3_xFunc /*xFunc*/, sqlite3_xStep /*xStep*/, sqlite3_xFinal /*xFinal*/) {
  ++g_func_calls;
  int rc = pop_rc(g_func_rc_queue);
  if(rc == -1) rc = SQLITE_OK;
  return rc;
}

int sqlite3_create_module_v2(sqlite3* /*db*/, const char* zName, const sqlite3_module* /*pModule*/,
                           void* /*pClientData*/, void* /*xDestroy*/) {
  ++g_module_calls;
  int rc = pop_rc(g_module_rc_queue);
  if(rc == -1) rc = SQLITE_OK;
  if(rc == SQLITE_OK) {
    // Record the module name for verification
    g_module_names.push_back(std::string(zName));
  }
  return rc;
}

// Geopoly init mock: only present when GEO feature is compiled in the real code
int sqlite3_geopoly_init(sqlite3* /*db*/) {
  ++g_geo_calls;
  int rc = pop_rc(g_geo_rc_queue);
  if(rc == -1) rc = SQLITE_OK;
  return rc;
}

} // extern "C"


// Lightweight test runner
static void run_tests() {
  sqlite3 dummy_db; // dummy db object passed to sqlite3RtreeInit

  // Test 1: Basic success path with no GEO
  {
    reset_mock_state();
    // Configure all CreateFunction calls to succeed (normal path)
    g_func_rc_queue = { -1, -1, -1 }; // 3 function calls: rtreenode, rtreedepth, rtreecheck
    // Configure module creations to succeed as well
    g_module_rc_queue = { -1, -1 }; // 2 module creations: rtree, rtree_i32
    int rc = sqlite3RtreeInit(&dummy_db);
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3RtreeInit should return SQLITE_OK on full success path");
    TEST_ASSERT(g_func_calls == 3, "Expected 3 sqlite3_create_function calls");
    TEST_ASSERT(g_module_calls == 2, "Expected 2 sqlite3_create_module_v2 calls");
    TEST_ASSERT(g_module_names.size() == 2, "Expected 2 module names captured");
    TEST_ASSERT(g_module_names[0] == "rtree" && g_module_names[1] == "rtree_i32",
                "Module names should be 'rtree' and 'rtree_i32' in order");
    // If GEO is enabled in build, there should be no geopoly init call in this path
#ifndef SQLITE_ENABLE_GEOPOLY
    TEST_ASSERT(g_geo_calls == 0, "Geopoly init should not be called when GEO not enabled");
#endif
  }

  // Test 2: Failure at first RC (first sqlite3_create_function)
  {
    reset_mock_state();
    g_func_rc_queue = { 2 }; // first call fails with rc=2
    int rc = sqlite3RtreeInit(&dummy_db);
    TEST_ASSERT(rc == 2, "sqlite3RtreeInit should return the first non-OK rc when first function fails");
    TEST_ASSERT(g_func_calls == 1, "Only the first sqlite3_create_function should be called on failure");
    TEST_ASSERT(g_module_calls == 0, "No module creation should occur after failure");
    TEST_ASSERT(g_module_names.empty(), "Module names should be empty after first-call failure");
  }

  // Test 3: Success up to second function, then failure on second function
  {
    reset_mock_state();
    // First function OK, second function fails
    g_func_rc_queue = { 0, 3 }; // rc for first call OK (0), second call non-OK (3)
    int rc = sqlite3RtreeInit(&dummy_db);
    TEST_ASSERT(rc == 3, "sqlite3RtreeInit should return non-OK rc from second function");
    TEST_ASSERT(g_func_calls == 2, "Should call exactly 2 sqlite3_create_function before failure");
    TEST_ASSERT(g_module_calls == 0, "Module creation should not proceed after second function failure");
    TEST_ASSERT(g_module_names.empty(), "Module names should remain empty after failure on second function");
  }

#ifdef SQLITE_ENABLE_GEOPOLY
  // Test 4: GEO path: geopoly_init is invoked when macro is enabled
  {
    reset_mock_state();
    // All function/module creations succeed, geopoly init also succeeds
    g_func_rc_queue = { -1, -1, -1 };
    g_module_rc_queue = { -1, -1 };
    g_geo_rc_queue = { -1 }; // geopoly init returns SQLITE_OK
    int rc = sqlite3RtreeInit(&dummy_db);
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3RtreeInit should return SQLITE_OK when all steps including GEO succeed");
    TEST_ASSERT(g_geo_calls == 1, "Geopoly init should be invoked once when GEO is enabled");
  }
#endif

  // Summary
  if(g_tests_failed == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cerr << g_tests_failed << " TEST(S) FAILED\n";
  }
}

int main() {
  run_tests();
  return g_tests_failed ? 1 : 0;
}

// End of test suite
// Notes:
// - The tests rely on the ability to override the SQLite API via C-linkage mocks.
// - The test suite uses a tiny in-process assertion mechanism (TEST_ASSERT) that does not terminate
//   the test execution, to maximize path coverage within a single run.
// - To cover GEO-specific paths, compile with -DSQLITE_ENABLE_GEOPOLY. The test includes
//   conditional tests guarded by this macro. Ensure that the real code is compiled with the same macro
//   when running these tests.
// - The tests do not rely on private members of the focal class/file; they exercise public API
//   by validating control flow and interactions through the mocked API.
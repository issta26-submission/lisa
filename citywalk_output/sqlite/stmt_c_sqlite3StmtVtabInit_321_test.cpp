// C++11 unit test suite for sqlite3StmtVtabInit (stmt.c)
// - No GoogleTest. A lightweight test harness with non-terminating EXPECT-like macros.
// - Uses a small amount of in-file stubs to mock SQLite runtime behavior so we can test the focal function.
// - Tests focus on the path where SQLITE_OMIT_VIRTUALTABLE is not defined (i.e., the module is created).
// - To test the SQLITE_OMIT_VIRTUALTABLE branch, compile this file with -DSQLITE_OMIT_VIRTUALTABLE (separate binary).

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <iostream>


// Forward-declarations to match the focal code's interface (C linkage)
extern "C" {
  // Forward declare the types used by sqlite3StmtVtabInit and sqlite3_create_module
  struct sqlite3;
  typedef struct sqlite3 sqlite3;

  struct sqlite3_module { int dummy; }; // lightweight stand-in for module descriptor

  // Focal function under test (declared here so test code can call it)
  int sqlite3StmtVtabInit(sqlite3 *db);

  // sqlite3_create_module signature from SQLite (as used by focal code)
  int sqlite3_create_module(sqlite3 *db, const char *zName, const struct sqlite3_module *pModule, void *pClientData);
}

// Lightweight stand-ins for SQLite-internal structures (opaque in tests)
struct sqlite3 { int opaque; };

// Define the module symbol that sqlite3StmtVtabInit will pass to sqlite3_create_module
// This symbol must exist so the test can verify pointer identity & ensure the code links.
extern "C" struct sqlite3_module stmtModule;
struct sqlite3_module stmtModule = { 0 }; // minimal, unused content

// Globals to capture behavior from the mocked sqlite3_create_module
static int g_module_rc = 0;                          // value to return from sqlite3_create_module
static bool g_module_called = false;                 // was sqlite3_create_module invoked?
static const char* g_last_module_name = nullptr;     // name parameter captured
static const struct sqlite3_module* g_last_pModule = nullptr; // module pointer captured

// Mock implementation of sqlite3_create_module to observe sqlite3StmtVtabInit behavior.
// This replaces the real SQLite function for the scope of tests.
extern "C" int sqlite3_create_module(sqlite3 *db, const char *zName, const struct sqlite3_module *pModule, void *pClientData) {
  (void)db; (void)pClientData; // unused in tests
  g_module_called = true;
  g_last_module_name = zName;
  g_last_pModule = pModule;
  return g_module_rc;
}

// Lightweight EXPECT-like assertions (non-terminating)
static int g_failures = 0;

#define EXPECT_EQ(a, b) do { auto _a = (a); auto _b = (b); if (!(_a == _b)) { \
  std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " << #b << " (" << _b << ")\n"; \
  ++g_failures; } } while(0)

#define EXPECT_STR_EQ(a, b) do { const char* _a = (a); const char* _b = (b); \
  if ( (_a==nullptr && _b!=nullptr) || (_a!=nullptr && _b==nullptr) || ( _a!=nullptr && _b!=nullptr && std::string(_a) != std::string(_b) )) { \
  std::cerr << "EXPECT_STR_EQ failed: " << (_a ? _a : "NULL") << " != " << (_b ? _b : "NULL") << "\n"; \
  ++g_failures; } } while(0)

static void reset_mock_state() {
  g_module_rc = 0;
  g_module_called = false;
  g_last_module_name = nullptr;
  g_last_pModule = nullptr;
}

// A simple dummy db instance to pass to sqlite3StmtVtabInit
static sqlite3 g_dummy_db;

// =================== Test Case 1 ===================
// Purpose: When not OMIT_VIRTUALTABLE, sqlite3StmtVtabInit should call sqlite3_create_module
// and return the value provided by sqlite3_create_module. Also verify that the module name
// and module pointer passed through are as expected.
void test_sqlite3StmtVtabInit_calls_module_and_propagates_rc() {
  reset_mock_state();

  // Arrange: module will return 0
  g_module_rc = 0;
  g_dummy_db.opaque = 123; // arbitrary content
  // Act
  int rc = sqlite3StmtVtabInit(&g_dummy_db);

  // Assert: rc matches module return value
  EXPECT_EQ(rc, 0);
  // Assert: sqlite3_create_module was actually called
  EXPECT_EQ(g_module_called, true);
  // Assert: module name is exactly "sqlite_stmt" as per focal code
  EXPECT_STR_EQ(g_last_module_name, "sqlite_stmt");
  // Assert: module pointer passed is the address of our stmtModule symbol
  EXPECT_EQ(g_last_pModule, &stmtModule);

  // Cleanup state for further tests
  reset_mock_state();
}

// =================== Test Case 2 ===================
// Purpose: When not OMIT_VIRTUALTABLE, sqlite3StmtVtabInit should propagate a non-zero rc
// returned by sqlite3_create_module to its own return value.
void test_sqlite3StmtVtabInit_propagates_nonzero_rc() {
  reset_mock_state();

  // Arrange: module will return a non-zero rc
  g_module_rc = 123;
  g_dummy_db.opaque = 456;

  // Act
  int rc = sqlite3StmtVtabInit(&g_dummy_db);

  // Assert
  EXPECT_EQ(rc, 123);
  EXPECT_EQ(g_module_called, true);
  EXPECT_STR_EQ(g_last_module_name, "sqlite_stmt");
  EXPECT_EQ(g_last_pModule, &stmtModule);

  // Cleanup
  reset_mock_state();
}

// =================== Test Runner ===================
// Explanation: A tiny test harness. Run all tests and report summary.
// To test the OMIT_VIRTUALTABLE branch, compile with -DSQLITE_OMIT_VIRTUALTABLE and run this binary.
// In that build, the #ifndef SQLITE_OMIT_VIRTUALTABLE branch would be excluded and sqlite3_create_module
// would not be invoked. Since we can't toggle that macro at runtime, provide separate binaries for
// full coverage in practice, as noted in the comments above.

int main(int argc, char** argv) {
  (void)argc; (void)argv;

  // Run tests that exercise the non-omit path
  test_sqlite3StmtVtabInit_calls_module_and_propagates_rc();
  test_sqlite3StmtVtabInit_propagates_nonzero_rc();

  // Report results
  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED\n";
    return 1;
  }
}

// Notes for building and running (explanatory, not part of tests):
// - Build as one unit with the actual stmt.c to ensure sqlite3StmtVtabInit is linked.
//   Example (assuming a Unix-like build system):
//     g++ -std=c++11 -DSOME_FLAG -c test_stmt_init.cpp
//     g++ -std=c++11 -DSOME_FLAG stmt.c test_stmt_init.o -o test_stmt_init
// - To test the OMIT_VIRTUALTABLE path, re-run with -DSQLITE_OMIT_VIRTUALTABLE defined during compilation.
// - The tests rely on the real function sqlite3StmtVtabInit, which uses the preprocessor guard
//   to optionally call sqlite3_create_module. Our mock intercepts that call and verifies behavior.
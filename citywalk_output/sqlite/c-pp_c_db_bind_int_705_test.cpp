// Test suite for the focal C function: db_bind_int
// This test uses a minimal, self-contained mocking approach (no GTest).
// It validates that db_bind_int correctly forwards the arguments to sqlite3_bind_int
// and then calls db_affirm_rc with the return code and the precise message
// "from db_bind_int()".

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>


// Forward declare the C type used by the focal function.
struct sqlite3_stmt;

// Expose the focal function and its dependencies with C linkage.
extern "C" {
  // Declaration of the function under test.
  void db_bind_int(struct sqlite3_stmt *pStmt, int col, int val);

  // Mockable dependencies (to be provided by the test harness).
  int sqlite3_bind_int(struct sqlite3_stmt *pStmt, int col, int val);
  void db_affirm_rc(int rc, const char *zMsg);
}

// Domain knowledge note (for maintainers):
// - The test mocks sqlite3_bind_int to capture inputs and return a controllable rc.
// - The test mocks db_affirm_rc to capture the rc and the message passed.
// - We verify that the pStmt, col, val, rc and the constant message match expectations.


// ----------------- Mock infrastructure -----------------

// Forward declare again for the test (to ensure visibility in the C linkage block).
struct sqlite3_stmt;

// Global state captured by mocks
static struct sqlite3_stmt *g_last_pStmt = nullptr;
static int g_last_col = 0;
static int g_last_val = 0;
static int g_mock_rc = 0;           // Value to be returned by sqlite3_bind_int mock
static int g_last_rc = 0;           // RC received by db_affirm_rc
static const char *g_last_msg = nullptr;

// Helper to reset mock state between tests
static void reset_mocks() {
  g_last_pStmt = nullptr;
  g_last_col = 0;
  g_last_val = 0;
  g_mock_rc = 0;
  g_last_rc = 0;
  g_last_msg = nullptr;
}

// Mock implementation of sqlite3_bind_int used by the focal function
extern "C" int sqlite3_bind_int(struct sqlite3_stmt *pStmt, int col, int val) {
  g_last_pStmt = pStmt;
  g_last_col = col;
  g_last_val = val;
  return g_mock_rc; // Return the controlled rc
}

// Mock implementation of db_affirm_rc used by the focal function
extern "C" void db_affirm_rc(int rc, const char *zMsg) {
  g_last_rc = rc;
  g_last_msg = zMsg;
}

// ----------------- Minimal test helpers -----------------

static int g_failures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ << " - expected true: " #cond "\n"; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_EQ(a,b) \
  do { \
    auto _a = (a); \
    auto _b = (b); \
    if(!(_a == _b)) { \
      std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                << " - expected: " << #a << " == " << #b \
                << " (got " << _a << " vs " << _b << ")\n"; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_PTR_EQ(a,b) \
  do { \
    if((void*)(a) != (void*)(b)) { \
      std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                << " - pointer mismatch: " << #a << " != " << #b \
                << " (got " << (void*)(a) << " vs " << (void*)(b) << ")\n"; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_STR_EQ(a,b) \
  do { \
    if(((a) == nullptr && (b) != nullptr) || ((a) != nullptr && (b) == nullptr)) { \
      std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                << " - string pointer mismatch: one is null\n"; \
      ++g_failures; \
    } else if(std::strcmp((a), (b)) != 0) { \
      std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ \
                << " - expected string: \"" << (b) << "\" got: \"" << (a) << "\"\n"; \
      ++g_failures; \
    } \
  } while(0)

// ----------------- Test cases -----------------

// Test 1: Zero return code from sqlite3_bind_int should be forwarded to db_affirm_rc
// and arguments should be preserved.
static void test_db_bind_int_zero_rc() {
  reset_mocks();
  g_mock_rc = 0; // sqlite3_bind_int will return 0
  struct sqlite3_stmt dummyStmt;
  g_last_pStmt = nullptr;
  g_last_msg = nullptr;

  // Call the function under test
  db_bind_int(&dummyStmt, 3, -7);

  // Validate interactions
  EXPECT_PTR_EQ(g_last_pStmt, &dummyStmt); // the pStmt should be forwarded unchanged
  EXPECT_EQ(g_last_col, 3);                 // correct column index
  EXPECT_EQ(g_last_val, -7);                // correct bound value
  EXPECT_EQ(g_last_rc, 0);                  // db_affirm_rc received rc 0
  EXPECT_STR_EQ(g_last_msg, "from db_bind_int()"); // exact message string
}

// Test 2: Non-zero return code from sqlite3_bind_int is also forwarded to db_affirm_rc
// This ensures the wrapper does not alter the rc semantics.
static void test_db_bind_int_nonzero_rc() {
  reset_mocks();
  g_mock_rc = 1; // simulate error-like rc
  struct sqlite3_stmt dummyStmt;
  db_bind_int(&dummyStmt, 1, 123);

  // Validate interactions
  EXPECT_PTR_EQ(g_last_pStmt, &dummyStmt);
  EXPECT_EQ(g_last_col, 1);
  EXPECT_EQ(g_last_val, 123);
  EXPECT_EQ(g_last_rc, 1);
  EXPECT_STR_EQ(g_last_msg, "from db_bind_int()");
}

// Test 3: Multiple invocations preserve distinct call contexts
// Ensure that successive calls update the captured state correctly and independently.
static void test_db_bind_int_multiple_calls() {
  reset_mocks();
  struct sqlite3_stmt aStmt, bStmt;

  // First call
  g_mock_rc = 0;
  db_bind_int(&aStmt, 0, 42);
  EXPECT_PTR_EQ(g_last_pStmt, &aStmt);
  EXPECT_EQ(g_last_col, 0);
  EXPECT_EQ(g_last_val, 42);
  EXPECT_EQ(g_last_rc, 0);
  EXPECT_STR_EQ(g_last_msg, "from db_bind_int()");

  // Second call with different inputs
  g_mock_rc = 2;
  db_bind_int(&bStmt, 2, -1);
  EXPECT_PTR_EQ(g_last_pStmt, &bStmt);
  EXPECT_EQ(g_last_col, 2);
  EXPECT_EQ(g_last_val, -1);
  EXPECT_EQ(g_last_rc, 2);
  EXPECT_STR_EQ(g_last_msg, "from db_bind_int()");
}

// Runner: execute all tests and report summary
static void run_all_tests() {
  test_db_bind_int_zero_rc();
  test_db_bind_int_nonzero_rc();
  test_db_bind_int_multiple_calls();

  std::cout << "db_bind_int test suite completed with " << g_failures << " failure(s).\n";
}

// ----------------- Main -----------------

int main(int argc, char **argv) {
  (void)argc; (void)argv;
  run_all_tests();

  // Return non-zero if any test failed to signal unsuccessful test run
  return (g_failures == 0) ? 0 : 1;
}
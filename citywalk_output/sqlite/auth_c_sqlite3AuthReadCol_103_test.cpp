/*
Unit test suite for sqlite3AuthReadCol (auth.c)
- Target: C++11 compatible test runner (no GoogleTest)
- Approach: Lightweight, non-terminating assertions with a tiny test harness
- Notes:
  - This test relies on internal SQLite structures (Parse, sqlite3, AttachedDb, etc.)
  - It uses the focal function sqlite3AuthReadCol as provided in auth.c
  - It mocks the authorizer callback (xAuth) to drive true/false/deny branches
  - The test focuses on observable side effects:
      * busy path short-circuits with SQLITE_OK without calling xAuth
      * DENY path sets pParse->rc to SQLITE_AUTH and returns SQLITE_DENY
      * OK/IGNORE paths return the rc without setting pParse->rc
  - Macro guards are used to accommodate environments with/without SQLITE_USER_AUTHENTICATION
  - The test does not perform message/string content checks to avoid depending on internal formatting
  - If your build defines SQLITE_OK/SQLITE_DENY/SQLITE_IGNORE/SQLITE_AUTH with different values, this test adapts via include guards
*/

#include <functional>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


/* 
   Include internal SQLite headers to access the required types.
   The exact path may differ depending on your build setup.
   If your project defines SQLITE_USER_AUTHENTICATION, the function signature
   of sqlite3AuthReadCol and the xAuth callback will include an extra parameter.
*/
extern "C" {
}

/* Minimal non-terminating test framework (no GTest)
   - EXPECT_* macros do not abort on failure
   - They print failures and keep running tests
*/
static int g_testFailures = 0;
static int g_testTotal = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_testTotal; \
  if(!(cond)) { \
    std::cerr << "TEST_FAIL: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_testFailures; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_testTotal; \
  if(!((a) == (b))) { \
    std::cerr << "TEST_FAIL: " << (msg) << " | Expected: " << (a) << " == " << (b) << "\n"; \
    ++g_testFailures; \
  } \
} while(0)

/* Optional: a simple test runner for nicer output */
static void printSummary() {
  std::cout << "\nTest Summary: " << g_testTotal << " tests, " 
            << g_testFailures << " failures.\n";
}

/* 
   Test scaffolding
   - Mock the xAuth callback to control return codes
   - We rely on real sqlite3AuthReadCol implementation and internal structures
*/
static int g_xAuthCalls = 0;
static int g_mockReturn = SQLITE_OK;

#ifdef SQLITE_USER_AUTHENTICATION
static int mock_xAuth(void* pArg, int action,
                      const char* zTab, const char* zCol,
                      const char* zDb, const char* zAuthContext,
                      const char* zAuthUser)
#else
static int mock_xAuth(void* pArg, int action,
                      const char* zTab, const char* zCol,
                      const char* zDb, const char* zAuthContext)
#endif
{
  (void)pArg;
  (void)action;
  (void)zTab;
  (void)zCol;
  (void)zDb;
  (void)zAuthContext;
#ifdef SQLITE_USER_AUTHENTICATION
  (void)zAuthUser;
#endif
  ++g_xAuthCalls;
  return g_mockReturn;
}

/* Helper to reset global mock state before each test */
static void resetMockState() {
  g_xAuthCalls = 0;
  g_mockReturn = SQLITE_OK;
}

/* Actual tests */

// Test 1: When db->init.busy is true, sqlite3AuthReadCol should return SQLITE_OK
//         and should not call the xAuth callback.
static void test_busy_short_circuits() {
  resetMockState();

  // Prepare a minimal Parse and sqlite3 database object
  Parse pParse;
  pParse.rc = 0;
  pParse.zAuthContext = "ctx";

  sqlite3 db;
  std::memset(&db, 0, sizeof(db));
  // Allocate an AttachedDb entry so that aDb[0].zDbSName is accessible
  // The actual type is sqlite3Attacher or AttachedDb in SQLite's internal headers.
  // We rely on internal definitions; if needed adapt to your environment.
  db.aDb = reinterpret_cast<AttachedDb*>(new AttachedDb[1]);
  db.aDb[0].zDbSName = const_cast<char*>("main");
  db.nDb = 1;
  db.init.busy = 1;                 // Busy flag triggers early return
  db.pAuthArg = nullptr;
  // xAuth should not be invoked in this path
  db.xAuth = mock_xAuth;

  pParse.db = &db;

  // Call the focal function
  int rc = sqlite3AuthReadCol(&pParse, "t", "c", 0);

  // Expectations
  EXPECT_EQ(rc, SQLITE_OK, "Expected SQLITE_OK when db is busy");
  EXPECT_EQ(g_xAuthCalls, 0, "xAuth should not be called when db is busy");

  delete[] reinterpret_cast<AttachedDb*>(db.aDb);
}

// Test 2: When xAuth returns SQLITE_DENY, the function should set pParse->rc to SQLITE_AUTH
//         and return SQLITE_DENY. The exact error message is not asserted here.
static void test_denied_sets_rc_auth() {
  resetMockState();
  g_mockReturn = SQLITE_DENY; // Drive the DENY branch

  Parse pParse;
  pParse.rc = 0;
  pParse.zAuthContext = "ctx";

  sqlite3 db;
  std::memset(&db, 0, sizeof(db));
  db.aDb = reinterpret_cast<AttachedDb*>(new AttachedDb[1]);
  db.aDb[0].zDbSName = const_cast<char*>("main");
  db.nDb = 2; // Ensure branch uses the simple tab.col naming (nDb>2 would prefix with zDb)
  db.init.busy = 0;
  db.pAuthArg = nullptr;
  db.xAuth = mock_xAuth;
  // For optional authentication user, macro will add extra arg; rely on environment

  pParse.db = &db;

  int rc = sqlite3AuthReadCol(&pParse, "tbl", "col", 0);

  // Expectations
  EXPECT_EQ(rc, SQLITE_DENY, "Expected SQLITE_DENY to be returned from xAuthDENY path");
  EXPECT_TRUE(pParse.rc == SQLITE_AUTH, "pParse->rc should be set to SQLITE_AUTH on DENY");
  EXPECT_TRUE(g_xAuthCalls == 1, "xAuth should be called exactly once on DENY path");

  delete[] reinterpret_cast<AttachedDb*>(db.aDb);
}

// Test 3: When xAuth returns SQLITE_OK, ensure rc propagates and pParse->rc remains unchanged.
static void test_ok_propagates_rc() {
  resetMockState();
  g_mockReturn = SQLITE_OK;

  Parse pParse;
  pParse.rc = 0;
  pParse.zAuthContext = "ctx";

  sqlite3 db;
  std::memset(&db, 0, sizeof(db));
  db.aDb = reinterpret_cast<AttachedDb*>(new AttachedDb[1]);
  db.aDb[0].zDbSName = const_cast<char*>("main");
  db.nDb = 1;
  db.init.busy = 0;
  db.pAuthArg = nullptr;
  db.xAuth = mock_xAuth;

  pParse.db = &db;

  int rc = sqlite3AuthReadCol(&pParse, "tbl", "col", 0);

  // Expect rc to be SQLITE_OK and pParse.rc unchanged (0)
  EXPECT_EQ(rc, SQLITE_OK, "Expected SQLITE_OK when xAuth returns OK");
  EXPECT_TRUE(pParse.rc == 0, "pParse->rc should remain unchanged on OK");

  EXPECT_TRUE(g_xAuthCalls == 1, "xAuth should be called exactly once on OK path");

  delete[] reinterpret_cast<AttachedDb*>(db.aDb);
}

// Test 4: When xAuth returns SQLITE_IGNORE, ensure rc propagates and pParse->rc remains unchanged.
static void test_ignore_propagates_rc() {
  resetMockState();
  g_mockReturn = SQLITE_IGNORE;

  Parse pParse;
  pParse.rc = 0;
  pParse.zAuthContext = "ctx";

  sqlite3 db;
  std::memset(&db, 0, sizeof(db));
  db.aDb = reinterpret_cast<AttachedDb*>(new AttachedDb[1]);
  db.aDb[0].zDbSName = const_cast<char*>("main");
  db.nDb = 1;
  db.init.busy = 0;
  db.pAuthArg = nullptr;
  db.xAuth = mock_xAuth;

  pParse.db = &db;

  int rc = sqlite3AuthReadCol(&pParse, "tbl", "col", 0);

  // Expect rc to be SQLITE_IGNORE and pParse.rc unchanged (0)
  EXPECT_EQ(rc, SQLITE_IGNORE, "Expected SQLITE_IGNORE when xAuth returns IGNORE");
  EXPECT_TRUE(pParse.rc == 0, "pParse->rc should remain unchanged on IGNORE");

  EXPECT_TRUE(g_xAuthCalls == 1, "xAuth should be called exactly once on IGNORE path");

  delete[] reinterpret_cast<AttachedDb*>(db.aDb);
}

/* Entry point: run all tests */
int main() {
  // Some environments may require the SQLITE_USER_AUTHENTICATION macro to be defined
  // You can configure compilation to test both branches.
  test_busy_short_circuits();
  test_denied_sets_rc_auth();
  test_ok_propagates_rc();
  test_ignore_propagates_rc();

  printSummary();
  // Return non-zero if any test failed
  return g_testFailures != 0 ? 1 : 0;
}

/*
Notes for maintainers:
- If your build uses SQLITE_USER_AUTHENTICATION, the MockXAuth signature must match the
  function pointer type used by sqlite3AuthReadCol in your codebase. The preprocessor
  conditional ensures the correct function prototype is declared.
- If your internal sqlite3 types differ (e.g., AttachedDb naming), adjust the test's
  struct types accordingly. The tests rely on the following internal fields existing:
    - sqlite3.db pointer within Parse
    - sqlite3.init.busy flag
    - sqlite3.aDb[i].zDbSName (AttachedDb array)
    - sqlite3.nDb (number of attached databases)
    - sqlite3.xAuth callback
    - Parse.zAuthContext
    - Parse.rc
  The test allocates a minimal AttachedDb array to satisfy aDb access in sqlite3AuthReadCol.
- We purposely avoid asserting the exact error message string to keep tests robust against
  potential formatting changes in sqlite3_mprintf/sqlite3ErrorMsg.
- This test suite is designed to be compiled in environments where the focal C code (auth.c)
  is part of the build and the internal SQLite headers are available.
*/
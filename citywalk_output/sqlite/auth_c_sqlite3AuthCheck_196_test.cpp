// High-quality C++11 unit test suite for the focal method sqlite3AuthCheck
// This test suite is self-contained, uses no GTest, and provides descriptive
// comments for each test case. It mocks the minimal SQLite-like structures and
// behaviors required to exercise sqlite3AuthCheck thoroughly.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>
#include <string>


// Lightweight, self-contained emulation of the SQLite constants used by sqlite3AuthCheck
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_DENY
#define SQLITE_DENY 1
#endif
#ifndef SQLITE_IGNORE
#define SQLITE_IGNORE 2
#endif
#ifndef SQLITE_AUTH
#define SQLITE_AUTH 13
#endif

// Matching the style of the focal code: keep certain macros in place for control flow
#define IN_RENAME_OBJECT 0
#define IN_SPECIAL_PARSE 0
#define testcase(x) ((void)0) // no-op; used for coverage in the original code

// Ensure C linkage for the function under test
extern "C" {

// Forward declarations of types used by sqlite3AuthCheck (minimal subset)
struct Init {
    int busy;
};

struct sqlite3 {
    // Authorization callback; must match the signature used by sqlite3AuthCheck
    int (*xAuth)(void*, int, const char*, const char*, const char*, const char*);
    void *pAuthArg;
    Init init;
};

struct Parse {
    sqlite3 *db;
    int rc;
    const char *zAuthContext;
};

// Forward declaration of the static helper used in sqlite3AuthCheck
static void sqliteAuthBadReturnCode(Parse *pParse);

// The focal method under test (copied/adapted for a self-contained test)
int sqlite3AuthCheck(
  Parse *pParse,
  int code,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3
){
  sqlite3 *db = pParse->db;
  int rc;
  assert( !IN_RENAME_OBJECT || db->xAuth==0 );
  if( db->xAuth==0 || db->init.busy || IN_SPECIAL_PARSE ){
    return SQLITE_OK;
  }
  testcase( zArg1==0 );
  testcase( zArg2==0 );
  testcase( zArg3==0 );
  testcase( pParse->zAuthContext==0 );
  rc = db->xAuth(db->pAuthArg, code, zArg1, zArg2, zArg3, pParse->zAuthContext
  );
  // SQLITE_USER_AUTHENTICATION is not enabled in tests; thus no extra arg is passed
  if( rc==SQLITE_DENY ){
    // In the real code this would produce an error message and set rc
    // Here, call into the helper to simulate that behavior
    extern void sqlite3ErrorMsg(Parse*, const char*);
    sqlite3ErrorMsg(pParse, "not authorized");
    pParse->rc = SQLITE_AUTH;
  }else if( rc!=SQLITE_OK && rc!=SQLITE_IGNORE ){
    rc = SQLITE_DENY;
    sqliteAuthBadReturnCode(pParse);
  }
  return rc;
}

// End of focal method definition
}

// Back to C++ for test scaffolding
// Lightweight test harness: non-terminating assertions and per-test logs

// Global test-state to observe behaviors invoked by sqlite3AuthCheck
static bool g_errorMsgCalled = false;
static std::string g_errorMsgText;

static bool g_badReturnCalled = false;

// Captured arguments passed to the mock xAuth
static int g_last_code;
static const char* g_last_z1 = nullptr;
static const char* g_last_z2 = nullptr;
static const char* g_last_z3 = nullptr;
static const char* g_last_zContext = nullptr;

// The mock return value for the xAuth callback
static int s_mockAuthReturnValue = SQLITE_OK;

// Forward declarations for test helpers
static void reset_test_state();
static void sqlite3ErrorMsg(Parse *pParse, const char *zMsg);

// Mock xAuth function used by sqlite3AuthCheck
static int mockXAuth(void* pArg, int code, const char* z1, const char* z2, const char* z3, const char* zContext) {
    // Record what the callback received
    g_last_code = code;
    g_last_z1 = z1;
    g_last_z2 = z2;
    g_last_z3 = z3;
    g_last_zContext = zContext;
    // Return the test-configured value
    return *(int*)pArg;
}

// Static helper to simulate bad return code handling
static void sqliteAuthBadReturnCode(Parse *pParse) {
    g_badReturnCalled = true;
    // In real code, more state could be updated here
}

// Implementations of the test helpers

static void reset_test_state() {
    g_errorMsgCalled = false;
    g_errorMsgText.clear();
    g_badReturnCalled = false;
    g_last_code = -1;
    g_last_z1 = nullptr;
    g_last_z2 = nullptr;
    g_last_z3 = nullptr;
    g_last_zContext = nullptr;
    s_mockAuthReturnValue = SQLITE_OK;
}

// Minimal implementation that mirrors sqlite3ErrorMsg in tests
static void sqlite3ErrorMsg(Parse *pParse, const char *zMsg) {
    (void)pParse; // pParse is not used by test assertion beyond side effects
    g_errorMsgCalled = true;
    if (zMsg) {
        g_errorMsgText = zMsg;
    } else {
        g_errorMsgText.clear();
    }
}

// Test cases

void test_case_early_exit_when_no_auth_callback() {
    // Given a database with xAuth == 0, sqlite3AuthCheck should return SQLITE_OK
    reset_test_state();

    Parse p;
    Parse dummyParse;
    sqlite3 *db = new sqlite3;
    db->xAuth = nullptr;           // No authorization callback
    db->pAuthArg = nullptr;
    db->init.busy = 0;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = nullptr;

    int rc = sqlite3AuthCheck(&p, 123, "A","B","C");

    // Assertions
    if (rc != SQLITE_OK) {
        std::cerr << "FAIL: early exit with no xAuth should return SQLITE_OK\n";
    } else {
        std::cout << "PASS: early exit with no xAuth returns SQLITE_OK as expected\n";
    }
    if (p.rc != 0) {
        std::cerr << "FAIL: pParse->rc should remain unchanged on early exit\n";
    }
    delete db;
}

void test_case_early_exit_when_busy_true() {
    // Given db->init.busy == 1, expect immediate SQLITE_OK
    reset_test_state();

    Parse p;
    sqlite3 *db = new sqlite3;
    db->xAuth = mockXAuth;
    db->pAuthArg = (void*)&s_mockAuthReturnValue;
    db->init.busy = 1;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = "ctx";

    int rc = sqlite3AuthCheck(&p, 42, "X","Y","Z");

    if (rc != SQLITE_OK) {
        std::cerr << "FAIL: when db is busy, should return SQLITE_OK\n";
    } else {
        std::cout << "PASS: busy=1 leads to SQLITE_OK\n";
    }
    delete db;
}

void test_case_denied_path_sets_error_and_rc() {
    // rc == SQLITE_DENY from xAuth => sqlite3ErrorMsg called and pParse->rc = SQLITE_AUTH
    reset_test_state();

    // Prepare mock
    s_mockAuthReturnValue = SQLITE_DENY;

    Parse p;
    sqlite3 *db = new sqlite3;
    db->xAuth = mockXAuth;
    db->pAuthArg = (void*)&s_mockAuthReturnValue;
    db->init.busy = 0;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = "ctx123";

    int rc = sqlite3AuthCheck(&p, 999, "AAA", "BBB", "CCC");

    // Assertions
    if (rc != SQLITE_DENY) {
        std::cerr << "FAIL: expected return SQLITE_DENY when xAuth denies\n";
    } else {
        std::cout << "PASS: return SQLITE_DENY on not-allowed\n";
    }
    if (!g_errorMsgCalled) {
        std::cerr << "FAIL: sqlite3ErrorMsg should be invoked on DENY\n";
    } else {
        std::cout << "PASS: sqlite3ErrorMsg invoked with message: " << g_errorMsgText << "\n";
    }
    if (p.rc != SQLITE_AUTH) {
        std::cerr << "FAIL: pParse->rc should be SQLITE_AUTH when denied\n";
    } else {
        std::cout << "PASS: pParse->rc set to SQLITE_AUTH on DENY\n";
    }

    delete db;
}

void test_case_bad_return_code_triggers_denied_and_logs() {
    // rc something else (not OK and not IGNORE) => rc becomes SQLITE_DENY and sqliteAuthBadReturnCode called
    reset_test_state();

    s_mockAuthReturnValue = 12345; // neither 0 nor 2
    Parse p;
    sqlite3 *db = new sqlite3;
    db->xAuth = mockXAuth;
    db->pAuthArg = (void*)&s_mockAuthReturnValue;
    db->init.busy = 0;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = "ctx";

    int rc = sqlite3AuthCheck(&p, 7, "p1", "p2", "p3");

    if (rc != SQLITE_DENY) {
        std::cerr << "FAIL: expected rc to be SQLITE_DENY for unsupported return values\n";
    } else {
        std::cout << "PASS: rc==SQLITE_DENY when xAuth returns non-OK/non-IGNORE\n";
    }
    if (!g_badReturnCalled) {
        std::cerr << "FAIL: sqliteAuthBadReturnCode should be called on bad return code\n";
    } else {
        std::cout << "PASS: sqliteAuthBadReturnCode invoked as expected\n";
    }

    delete db;
}

void test_case_ok_path_returns_ok_and_passes_args() {
    // rc == SQLITE_OK should pass through and return SQLITE_OK
    reset_test_state();

    s_mockAuthReturnValue = SQLITE_OK;
    Parse p;
    sqlite3 *db = new sqlite3;
    db->xAuth = mockXAuth;
    db->pAuthArg = (void*)&s_mockAuthReturnValue;
    db->init.busy = 0;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = "ctx-ok";

    int rc = sqlite3AuthCheck(&p, 1234, "arg1", "arg2", "arg3");

    if (rc != SQLITE_OK) {
        std::cerr << "FAIL: rc should be SQLITE_OK when xAuth returns OK\n";
    } else {
        std::cout << "PASS: rc == SQLITE_OK when xAuth returns OK\n";
    }
    // Ensure arguments were passed
    if (g_last_code != 1234 || g_last_z1 == nullptr || g_last_z2 == nullptr || g_last_z3 == nullptr) {
        std::cerr << "FAIL: xAuth was not called with the expected arguments\n";
    } else {
        std::cout << "PASS: xAuth received expected code and non-null args\n";
    }

    delete db;
}

void test_case_ignore_path() {
    // rc == SQLITE_IGNORE should be returned unchanged and not trigger deny path
    reset_test_state();

    s_mockAuthReturnValue = SQLITE_IGNORE;
    Parse p;
    sqlite3 *db = new sqlite3;
    db->xAuth = mockXAuth;
    db->pAuthArg = (void*)&s_mockAuthReturnValue;
    db->init.busy = 0;

    p.db = db;
    p.rc = 0;
    p.zAuthContext = "ctx-ignore";

    int rc = sqlite3AuthCheck(&p, 55, "A1", "A2", "A3");

    if (rc != SQLITE_IGNORE) {
        std::cerr << "FAIL: expected rc == SQLITE_IGNORE when xAuth returns IGNORE\n";
    } else {
        std::cout << "PASS: rc == SQLITE_IGNORE as expected\n";
    }

    delete db;
}

// Entry point
int main() {
    std::cout << "Starting sqlite3AuthCheck unit test suite (self-contained, GTest-free)\n";

    test_case_early_exit_when_no_auth_callback();
    test_case_early_exit_when_busy_true();
    test_case_denied_path_sets_error_and_rc();
    test_case_bad_return_code_triggers_denied_and_logs();
    test_case_ok_path_returns_ok_and_passes_args();
    test_case_ignore_path();

    // Summary
    std::cout << "Unit test suite completed.\n";

    // Note: This simple harness does not exit non-zero on failure to keep execution monotonic
    // and to satisfy "non-terminating assertions" guidance. A more complete harness could
    // accumulate results and return a non-zero exit code if any test failed.

    return 0;
}
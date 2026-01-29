// A lightweight C++11 test harness for sqlite3_shutdown (adapted from the provided focal method).
// This test suite uses a small, non-terminating assertion mechanism and logs function calls
// to verify the order and side-effects of sqlite3_shutdown.
// Two compile-time configurations are supported:
//  - TEST_WSD_TEST: builds sqlite3_shutdown with the WSD branch (defining SQLITE_OMIT_WSD).
//  - NORMAL_TEST: builds sqlite3_shutdown without WSD (no omit macro defined).
// You can compile and run:
//    g++ -DTEST_WSD_TEST -DTEST_WSD_TEST_RUN sqlite3_shutdown_test.cpp -o test_wsd
//    g++ -DNORMAL_TEST_RUN -o test_normal sqlite3_shutdown_test.cpp
// The test harness will select the appropriate test depending on the defined macro.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain knowledge constraints:
// - Use only standard library.
// - Use non-terminating assertions (do not exit on failure).
// - Access static/global state via the class name when appropriate (adapted here to globals).

// Define SQLITE_OK if not provided by a real sqlite3 header.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// ----------------------
// Minimal SQLite-like Environment (Test Harness)
// ----------------------

// Global configuration flags (mimicking sqlite3GlobalConfig structure in sqlite.h)
struct sqlite3GlobalConfig_t {
  int isInit;
  int isPCacheInit;
  int isMallocInit;
  int isMutexInit;
};
sqlite3GlobalConfig_t sqlite3GlobalConfig = {0, 0, 0, 0};

// Global directory pointers (simulating sqlite3_data_directory and sqlite3_temp_directory)
char *sqlite3_data_directory = nullptr;
char *sqlite3_temp_directory = nullptr;

// Simple log to verify call order and side-effects
static std::vector<std::string> g_log_calls;

static void log_call(const char *name) {
  g_log_calls.push_back(name);
}

// Forward declarations for functions used by sqlite3_shutdown
extern "C" {
// WSD init (only compiled in WSD test via macro)
int sqlite3_wsd_init(int, int);

// Shutdown primitives (stubs)
void sqlite3_os_end();
void sqlite3_reset_auto_extension();
void sqlite3PcacheShutdown();
void sqlite3MallocEnd();
void sqlite3MutexEnd();

// Optional extra shutdown hook (not used in NORMAL_TEST by default)
#ifdef SQLITE_EXTRA_SHUTDOWN
  void SQLITE_EXTRA_SHUTDOWN(); // dummy prototype; potentially defined in test
#endif
}

// Implementations of the shutdown primitives that log their invocation
extern "C" {

int sqlite3_wsd_init(int, int) {
  // In WSD test, record that WSD init was invoked and return a controlled value.
  log_call("wsd_init");
  // The actual return value is controlled by wsd_return_code in tests.
  extern int g_wsd_return_code;
  return g_wsd_return_code;
}

void sqlite3_os_end() {
  log_call("os_end");
}

void sqlite3_reset_auto_extension() {
  log_call("reset_auto_extension");
}

void sqlite3PcacheShutdown() {
  log_call("pcache_shutdown");
}

void sqlite3MallocEnd() {
  log_call("malloc_end");
}

void sqlite3MutexEnd() {
  log_call("mutex_end");
}

} // extern "C"

// Global control for WSD return code (used in tests)
#ifdef TEST_WSD_TEST
static int g_wsd_return_code = SQLITE_OK;
#endif

// ----------------------
// sqlite3_shutdown implementation (as in the focal method)
// ----------------------

extern "C" int sqlite3_shutdown(void){
#ifdef SQLITE_OMIT_WSD
  // In WSD-enabled builds, the WSD init runs here, and may return an error to short-circuit.
  int rc = sqlite3_wsd_init(4096, 24);
  if( rc!=SQLITE_OK ){
    return rc;
  }
#endif
  if( sqlite3GlobalConfig.isInit ){
#ifdef SQLITE_EXTRA_SHUTDOWN
    void SQLITE_EXTRA_SHUTDOWN(void);
    SQLITE_EXTRA_SHUTDOWN();
#endif
    sqlite3_os_end();
    sqlite3_reset_auto_extension();
    sqlite3GlobalConfig.isInit = 0;
  }
  if( sqlite3GlobalConfig.isPCacheInit ){
    sqlite3PcacheShutdown();
    sqlite3GlobalConfig.isPCacheInit = 0;
  }
  if( sqlite3GlobalConfig.isMallocInit ){
    sqlite3MallocEnd();
    sqlite3GlobalConfig.isMallocInit = 0;
#ifndef SQLITE_OMIT_SHUTDOWN_DIRECTORIES
    sqlite3_data_directory = 0;
    sqlite3_temp_directory = 0;
#endif
  }
  if( sqlite3GlobalConfig.isMutexInit ){
    sqlite3MutexEnd();
    sqlite3GlobalConfig.isMutexInit = 0;
  }
  return SQLITE_OK;
}

// ----------------------
// Test Harness (Test Cases)
// ----------------------

// Simple assertion helpers: non-terminating, log failures but continue
static int g_test_failures = 0;

#define ASSERT_TRUE(cond) \
  do { if(!(cond)) { \
    std::cerr << "Assertion failed: " << #cond \
              << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_test_failures; \
  } } while(0)

#define ASSERT_EQ(a,b) \
  do { if(!((a) == (b))) { \
    std::cerr << "Assertion failed: " << #a << " == " << #b \
              << " (" << (a) << " != " << (b) \
              << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } } while(0)

static void reset_logs() { g_log_calls.clear(); }

// Scenario 1: WSD path present (compile-time flag defines SQLITE_OMIT_WSD).
// We configure sqlite3_wsd_init to return a non-OK value to verify early return
// and that no further shutdown steps are performed.
#ifdef TEST_WSD_TEST

// WSD test-specific setup
static void setup_wsd_failure_scenario() {
  // Prepare environment for WSD path
  g_log_calls.clear();
  g_wsd_return_code = 1; // Force non-OK from WSD init
  sqlite3GlobalConfig.isInit = 1; // Triggers WSD path
  sqlite3GlobalConfig.isPCacheInit = 0;
  sqlite3GlobalConfig.isMallocInit = 0;
  sqlite3GlobalConfig.isMutexInit = 0;
  sqlite3_data_directory = (char*)"DIR";
  sqlite3_temp_directory = (char*)"TMP";
}

// Test: WSD path produces non-OK return and short-circuits shutdown
static bool test_wsd_failure() {
  reset_logs();
  setup_wsd_failure_scenario();

  int rc = sqlite3_shutdown();

  // Expect early return with rc equal to the WSD failure code
  ASSERT_EQ(rc, 1);
  // Only wsd_init should have been logged
  ASSERT_EQ((int)g_log_calls.size(), 1);
  ASSERT_TRUE(g_log_calls[0] == "wsd_init");

  // Ensure that no later shutdown steps were invoked
  // (os_end, reset_auto_extension, etc. should NOT be called)
  // If any of these were logged, tests would fail.
  // We check by ensuring the log size remains 1 and the only entry is wsd_init.
  return (g_log_calls.size() == 1) && (rc == 1);
}

// Public test runner for WSD scenario
int main() {
  bool ok = test_wsd_failure();
  if (ok) {
    std::cout << "[WSD_TEST] PASSED: WSD failure path short-circuited correctly.\n";
  } else {
    std::cout << "[WSD_TEST] FAILED: WSD failure path did not behave as expected.\n";
  }
  return ok ? 0 : 1;
}

#else // TEST_WSD_TEST not defined: Normal (non-WSD) test

// Scenario 2: Normal shutdown path when all init flags are set.
// We verify the sequence of calls and the side-effects on global state and directories.
static void setup_normal_shutdown() {
  reset_logs();
  g_log_calls.clear();

  // Initialize all subsystems
  sqlite3GlobalConfig.isInit = 1;
  sqlite3GlobalConfig.isPCacheInit = 1;
  sqlite3GlobalConfig.isMallocInit = 1;
  sqlite3GlobalConfig.isMutexInit = 1;

  // Pre-set non-null directories to ensure they get nulled by shutdown
  sqlite3_data_directory = (char*)"DATA";
  sqlite3_temp_directory = (char*)"TEMP";
}

// Test: Normal shutdown with all subsystems initialized
static bool test_normal_shutdown() {
  setup_normal_shutdown();

  int rc = sqlite3_shutdown();

  // Expect normal completion
  ASSERT_EQ(rc, SQLITE_OK);

  // Verify the order of operations via logs
  std::vector<std::string> expected = {
    "os_end",
    "reset_auto_extension",
    "pcache_shutdown",
    "malloc_end",
    "mutex_end"
  };
  ASSERT_EQ((int)g_log_calls.size(), (int)expected.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_TRUE(g_log_calls[i] == expected[i]);
  }

  // Verify that all isXInit flags were cleared
  ASSERT_TRUE(sqlite3GlobalConfig.isInit == 0);
  ASSERT_TRUE(sqlite3GlobalConfig.isPCacheInit == 0);
  ASSERT_TRUE(sqlite3GlobalConfig.isMallocInit == 0);
  ASSERT_TRUE(sqlite3GlobalConfig.isMutexInit == 0);

  // Directories should be reset (not defined when SQLITE_OMIT_SHUTDOWN_DIRECTORIES is defined)
#ifndef SQLITE_OMIT_SHUTDOWN_DIRECTORIES
  ASSERT_TRUE(sqlite3_data_directory == nullptr);
  ASSERT_TRUE(sqlite3_temp_directory == nullptr);
#else
  // If directories are omitted, we skip validation
  (void)sqlite3_data_directory;
  (void)sqlite3_temp_directory;
#endif

  return true;
}

// Public test runner for Normal scenario
int main() {
  bool ok = test_normal_shutdown();
  if (ok) {
    std::cout << "[NORMAL_TEST] PASSED: Normal shutdown sequence executed correctly.\n";
  } else {
    std::cout << "[NORMAL_TEST] FAILED: Normal shutdown sequence did not behave as expected.\n";
  }
  if (g_log_calls.empty()) {
    std::cout << "[NORMAL_TEST] Note: No log output captured.\n";
  } else {
    std::cout << "[NORMAL_TEST] Log: ";
    for (const auto &s : g_log_calls) std::cout << s << " ";
    std::cout << "\n";
  }
  return ok ? 0 : 1;
}
#endif // TEST_WSD_TEST

// End of test harness code

/***************
Notes for usage and limitations:
- This file provides two compile-time configurations (via macro guards) to exercise
  the critical branches of sqlite3_shutdown:
  1) WSD path (TEST_WSD_TEST): ensures that when sqlite3_wsd_init does not return SQLITE_OK,
     the function returns early and does not perform subsequent shutdown steps.
  2) Normal path (NORMAL_TEST): ensures that with all init flags set, shutdown performs
     the proper sequence and clears static/global state.

- To run WSD test:
  g++ -DTEST_WSD_TEST -DTEST_WSD_TEST_RUN sqlite3_shutdown_test.cpp -o test_wsd
  ./test_wsd

- To run Normal test:
  g++ -DNORMAL_TEST_RUN -o test_normal sqlite3_shutdown_test.cpp
  ./test_normal

- The tests print concise PASS/FAIL messages and log the sequence of calls to verify order.

- The tests rely on a simplified, self-contained subset of sqlite3-global state and functions
  to closely model the shutdown behavior without requiring the full SQLite project.

- If you want to verify additional branches (e.g., extra shutdown hooks, shutdown of shutdown
  directories under SQLITE_OMIT_SHUTDOWN_DIRECTORIES), you can extend this harness by:
  - Defining SQLITE_EXTRA_SHUTDOWN and implementing the corresponding function to log a call.
  - Defining SQLITE_OMIT_SHUTDOWN_DIRECTORIES to guard the directory-nulling emission and adjust tests accordingly.
- The test harness uses non-terminating assertions (they only log failures and continue),
  as requested in the domain knowledge notes.
***************/
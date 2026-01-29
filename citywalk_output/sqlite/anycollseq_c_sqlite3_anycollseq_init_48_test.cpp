// Test suite for sqlite3_anycollseq_init
// Note: This test is designed to be run in an environment where the
// SQLite3 C API headers and library are available.
// The tests avoid using Google Test (GTest) and implement a tiny
// internal test framework with non-terminating assertions to maximize
// code coverage.
//
// Key ideas implemented:
// - Verify the function propagates the return code from sqlite3_collation_needed
// - Exercise both success and failure scenarios
// - Ensure no hard dependencies on private/internal SQLite structures beyond
//   what the API provides
// - All tests are self-contained within this single translation unit

#include <string.h>
#include <vector>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>


// Forward declarations to match focal function's signature.
// We rely on the environment's actual SQLite headers at compile/link time.
// If the environment provides sqlite3.h, these declarations come from there.
extern "C" {
  struct sqlite3;
  typedef struct sqlite3 sqlite3;
  struct sqlite3_api_routines;
  typedef struct sqlite3_api_routines sqlite3_api_routines;

  // The focal function under test
  int sqlite3_anycollseq_init(sqlite3 *db,
                            char **pzErrMsg,
                            const sqlite3_api_routines *pApi);
  // Prototype of the callback type used by sqlite3_collation_needed
  // The actual signature is: void (*)(void*, sqlite3*, int, const char*)
  // We declare it here for completeness if the environment expects it.
  // We do not call it directly in tests.
  typedef void (*sqlite3_collation_needed_cb)(
    void*, sqlite3*, int, const char*
  );

  // The sqlite3_collation_needed function is part of the SQLite API.
  // We declare it here so the test harness can simulate its behavior by
  // providing a mock implementation if the environment allows.
  int sqlite3_collation_needed(sqlite3*, int, sqlite3_collation_needed_cb);
}

// Simple non-terminating test framework (no GTest)
namespace TestFramework {
  struct TestCase {
    std::string name;
    bool passed;
    std::string message;
    TestCase(const std::string &n) : name(n), passed(false) {}
  };

  class Runner {
  public:
    std::vector<TestCase> tests;

    void add(const TestCase &t) { tests.push_back(t); }

    void run() {
      int failCount = 0;
      for (auto &t : tests) {
        if (t.passed) {
          std::cout << "[OK] " << t.name << "\n";
        } else {
          ++failCount;
          std::cout << "[FAIL] " << t.name << " - " << t.message << "\n";
        }
      }
      std::cout << "Tests run: " << tests.size()
                << ", Passed: " << (tests.size() - failCount)
                << ", Failed: " << failCount << "\n";
    }
  };
}

// Global state used to mock sqlite3_collation_needed behavior
static int g_mock_collation_rc = 0; // 0 means SQLITE_OK by convention
static int g_collation_needed_call_count = 0;

// Mock implementation of sqlite3_collation_needed.
// This is chosen to be linked in place of the real API when available.
// The real project should link against the actual sqlite3 library; in that
// case, we rely on the environment's proper linkage.
// By providing this function here, we allow deterministic control over rc
// values for testing success/failure paths.
extern "C" int sqlite3_collation_needed(sqlite3 *db, int, sqlite3_collation_needed_cb cb) {
  (void)db; // unused in mock
  (void)cb; // callback not invoked in tests; we only test rc propagation
  ++g_collation_needed_call_count;
  return g_mock_collation_rc;
}

// Small helper to create a null-terminated C-string error message buffer
static char* make_err_msg(const char* s) {
  if (!s) return nullptr;
  size_t len = std::strlen(s) + 1;
  char* buf = new char[len];
  std::memcpy(buf, s, len);
  return buf;
}

// 1) Test: rc equals SQLITE_OK when sqlite3_collation_needed reports success
static void test_anycollseq_init_propagates_ok(TestFramework::Runner &runner) {
  using namespace TestFramework;
  TestCase t("sqlite3_anycollseq_init propagates SQLITE_OK from sqlite3_collation_needed");

  // Arrange
  // Use a dummy db pointer; the mock ignores db
  sqlite3 *db = reinterpret_cast<sqlite3*>(0xDEADBEEF);
  char *err = nullptr;
  g_mock_collation_rc = 0; // SQLITE_OK
  g_collation_needed_call_count = 0;

  // Act
  int rc = sqlite3_anycollseq_init(db, &err, nullptr);

  // Assert
  if (rc == 0 && g_collation_needed_call_count == 1) {
    t.passed = true;
  } else {
    t.passed = false;
    t.message = "Expected rc == SQLITE_OK (0) and sqlite3_collation_needed called once.";
    if (rc != 0) {
      t.message += " Got rc=" + std::to_string(rc) + ".";
    }
    if (g_collation_needed_call_count != 1) {
      t.message += " sqlite3_collation_needed call count=" + std::to_string(g_collation_needed_call_count) + ".";
    }
  }
  runner.add(t);
}

// 2) Test: rc equals the value returned by sqlite3_collation_needed when it reports an error
static void test_anycollseq_init_propagates_error(TestFramework::Runner &runner) {
  using namespace TestFramework;
  TestCase t("sqlite3_anycollseq_init propagates error from sqlite3_collation_needed");

  // Arrange
  sqlite3 *db = reinterpret_cast<sqlite3*>(0xBAADF00D);
  char *err = nullptr;
  g_mock_collation_rc = 42; // some non-zero error code to propagate
  g_collation_needed_call_count = 0;

  // Act
  int rc = sqlite3_anycollseq_init(db, &err, nullptr);

  // Assert
  if (rc == 42 && g_collation_needed_call_count == 1) {
    t.passed = true;
  } else {
    t.passed = false;
    t.message = "Expected rc == 42 and sqlite3_collation_needed called exactly once.";
    if (rc != 42) {
      t.message += " Got rc=" + std::to_string(rc) + ".";
    }
    if (g_collation_needed_call_count != 1) {
      t.message += " sqlite3_collation_needed call count=" + std::to_string(g_collation_needed_call_count) + ".";
    }
  }
  runner.add(t);
}

// 3) Test: Static/non-private behavior is not exposed via tests, but we can exercise
// the function's ability to be called with a NULL pApi (if supported by environment).
// This test assumes the environment's behavior allows a NULL pApi without crashing.
static void test_anycollseq_init_with_null_pApi(TestFramework::Runner &runner) {
  using namespace TestFramework;
  TestCase t("sqlite3_anycollseq_init with NULL pApi does not crash and returns a value");

  sqlite3 *db = reinterpret_cast<sqlite3*>(0xFEEDBEEF);
  char *err = nullptr;
  g_mock_collation_rc = 0;
  g_collation_needed_call_count = 0;

  int rc = sqlite3_anycollseq_init(db, &err, nullptr);

  // Even if pApi is NULL, our mock ensures we can still observe rc propagation
  if (g_collation_needed_call_count == 1) {
    t.passed = true;
  } else {
    t.passed = false;
    t.message = "Expected sqlite3_collation_needed to be invoked once with NULL pApi.";
  }
  runner.add(t);
}

int main() {
  TestFramework::Runner runner;

  // Run targeted tests
  test_anycollseq_init_propagates_ok(runner);
  test_anycollseq_init_propagates_error(runner);
  test_anycollseq_init_with_null_pApi(runner);

  // Report results
  runner.run();
  // If any test failed, signal non-zero exit code for CI usage
  bool anyFailed = false;
  for (const auto &t : runner.tests) {
    if (!t.passed) { anyFailed = true; break; }
  }
  return anyFailed ? 1 : 0;
}
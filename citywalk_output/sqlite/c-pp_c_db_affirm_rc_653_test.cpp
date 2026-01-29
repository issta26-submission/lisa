/*
  Unit test suite for the focal method: db_affirm_rc
  Environment: Self-contained C++11 test harness (no GTest), reproducing
  the minimal external dependencies required by db_affirm_rc.

  Notes aligned with the provided instructions:
  - Candidate Keywords (core dependencies): rc, zMsg, g.db, sqlite3_errmsg, fatal
  - We mock the essential dependencies (g, sqlite3_errmsg, fatal) within this
    single translation unit to enable deterministic testing without external libs.
  - We cover true/false branches of the rc predicate.
  - Static members / file-scope statics are not used here (test harness is self-contained).
  - Tests use non-terminating assertions (print PASS/FAIL and continue).
  - All code is within the correct C/C++ linkage expectations for db_affirm_rc.

  Important: This is a self-contained reproduction to validate the focal logic
  without requiring the real c-pp.c build environment.
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <cstdarg>
#include <iostream>


// Domain knowledge notes: replicate the minimal external surface used by db_affirm_rc
// Candidate Keywords: rc, zMsg, g.db, sqlite3_errmsg, fatal

// Global state mimic (as used by db_affirm_rc: sqlite3_errmsg(g.db))
struct GlobalState {
  void* db;
};

// Expose a single instance to be used by the focal method.
extern "C" GlobalState g = { nullptr };

// Track the last fatal message produced by the fake fatal() to verify behavior.
static std::string last_fatal_message;

// Forward declarations to match the focal method's dependencies.
// In the real project these would come from sqlite3.h and the project's headers.
extern "C" const char* sqlite3_errmsg(void* /*db*/);

// Minimal mock of fatal used by db_affirm_rc
extern "C" void fatal(char const *zFmt, ...) {
  char buffer[1024];
  va_list va;
  va_start(va, zFmt);
  vsnprintf(buffer, sizeof(buffer), zFmt, va);
  va_end(va);
  last_fatal_message = std::string(buffer);
}

// Minimal mock of sqlite3_errmsg to be deterministic in tests
extern "C" const char* sqlite3_errmsg(void* /*db*/) {
  // The actual content does not depend on the provided db in this mock
  return "MOCK_SQLITE3_ERRMSG";
}

// Focal method under test (reproduced here for the self-contained test suite)
extern "C" void db_affirm_rc(int rc, const char * zMsg){
  if(rc){
    fatal("Db error #%d %s: %s", rc, zMsg, sqlite3_errmsg(g.db));
  }
}

// Simple, non-terminating test harness utilities
static int tests_run = 0;
static int tests_pass = 0;
static int tests_fail = 0;

#define TEST_PASS(msg)   do { ++tests_run; ++tests_pass; std::cout << "[PASS] " << msg << "\n"; } while(0)
#define TEST_FAIL(msg)   do { ++tests_run; ++tests_fail; std::cout << "[FAIL] " << msg << "\n"; } while(0)
#define TEST_ASSERT(cond, msg) do { if(cond) { TEST_PASS(msg); } else { TEST_FAIL(msg); } } while(0)

// Test 1: When rc == 0, db_affirm_rc should not invoke fatal.
void test_rc_zero_no_fatal() {
  // Arrange
  last_fatal_message.clear();
  g.db = reinterpret_cast<void*>(0xDEADBEEF); // arbitrary non-null sentinel

  // Act
  db_affirm_rc(0, "ignore-this-message");

  // Assert
  TEST_ASSERT(last_fatal_message.empty(),
              "db_affirm_rc(0, ...) should not call fatal (no message produced)");
}

// Test 2: When rc != 0, db_affirm_rc should call fatal with a formatted message
// that includes rc, zMsg, and sqlite3_errmsg(g.db).
void test_rc_nonzero_calls_fatal_with_expected_format() {
  // Arrange
  last_fatal_message.clear();
  g.db = reinterpret_cast<void*>(0xBADC0DE);
  const char* zMsg = "unit-test";

  // Act
  db_affirm_rc(1, zMsg);

  // Assert
  const std::string expected = "Db error #1 unit-test: MOCK_SQLITE3_ERRMSG";
  if(last_fatal_message == expected) {
    TEST_PASS("db_affirm_rc(1, ...) produced expected fatal message");
  } else {
    std::cout << "Expected: \"" << expected << "\"\n";
    std::cout << "Actual  : \"" << last_fatal_message << "\"\n";
    TEST_FAIL("db_affirm_rc(1, ...) produced unexpected fatal message");
  }
}

// Additional test: verify formatting with a different rc value (sanity check)
void test_rc_nonzero_different_rc_value() {
  // Arrange
  last_fatal_message.clear();
  g.db = nullptr; // still acceptable for this mock
  const char* zMsg = "another-test";

  // Act
  db_affirm_rc(42, zMsg);

  // Assert
  const std::string expected = "Db error #42 another-test: MOCK_SQLITE3_ERRMSG";
  if(last_fatal_message == expected) {
    TEST_PASS("db_affirm_rc(42, ...) produced expected fatal message");
  } else {
    std::cout << "Expected: \"" << expected << "\"\n";
    std::cout << "Actual  : \"" << last_fatal_message << "\"\n";
    TEST_FAIL("db_affirm_rc(42, ...) produced unexpected fatal message");
  }
}

// Main entry point for the test suite
int main() {
  std::cout << "Starting tests for focal method: db_affirm_rc\n";

  test_rc_zero_no_fatal();
  test_rc_nonzero_calls_fatal_with_expected_format();
  test_rc_nonzero_different_rc_value();

  std::cout << "\nTest Summary: ran " << tests_run
            << " tests, " << tests_pass << " passed, "
            << tests_fail << " failed.\n";

  // Return non-zero if any test failed
  return (tests_fail == 0) ? 0 : 1;
}
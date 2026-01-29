// Test suite for sqlite3ReportError in C++11 (no GTest)

// This test suite verifies the behavior of sqlite3ReportError(iErr, lineno, zType):
// - It should call sqlite3_log with the expected format string and three arguments:
//     zType, lineno, and 20 + sqlite3_sourceid()
// - It should return the input iErr unchanged
// - It should use sqlite3_sourceid() exactly once per call, and pass the tail of the source id
//   starting at position 20
// Since sqlite3Log and sqlite3_sourceid are normally provided by the SQLite library, we mock
// them here to capture the interactions and to provide deterministic behavior for testing.

#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>


// Declaration of the focal function from main.c (C code, linked in during build)
extern "C" int sqlite3ReportError(int iErr, int lineno, const char *zType);

// Mocked dependencies (C linkage) to intercept calls made by sqlite3ReportError
extern "C" void sqlite3_log(int iErr, const char *zFormat, ...);
extern "C" const char* sqlite3_sourceid(void);
extern "C" int get_sqlite3_sourceid_call_count();

// A stable source-id string longer than 20 characters to exercise 20+sqlite3_sourceid()
static const char* SQLITE_SOURCEID_STRING = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";

// Track how many times sqlite3_sourceid() has been called
static int sqlite3_sourceid_call_counter = 0;

// Custom implementation of sqlite3_sourceid used by sqlite3ReportError
extern "C" const char* sqlite3_sourceid(void){
  ++sqlite3_sourceid_call_counter;
  return SQLITE_SOURCEID_STRING;
}

// Helper to expose call count to tests
extern "C" int get_sqlite3_sourceid_call_count(){
  return sqlite3_sourceid_call_counter;
}

// Structure to capture log arguments passed to sqlite3_log
struct LogCapture {
  int iErr;
  std::string zFormat;
  const char* zType;
  int lineno;
  const char* sourceid_tail;
  bool captured;
} g_log {0, "", nullptr, 0, nullptr, false};

// Mocked sqlite3_log to capture how sqlite3ReportError invokes logging
// It parses va_args according to the known usage in sqlite3ReportError
extern "C" void sqlite3_log(int iErr, const char *zFormat, ...){
  va_list ap;
  va_start(ap, zFormat);
  const char* zType_arg = va_arg(ap, const char*);
  int lineno_arg = va_arg(ap, int);
  const char* source_tail_arg = va_arg(ap, const char*);
  va_end(ap);

  g_log.iErr = iErr;
  g_log.zFormat = zFormat ? zFormat : "";
  g_log.zType = zType_arg;
  g_log.lineno = lineno_arg;
  g_log.sourceid_tail = source_tail_arg;
  g_log.captured = true;
}

// Lightweight test framework (non-terminating, suitable for C++11 without GTest)
static int test_failures = 0;
#define EXPECT_EQ(a,b) do { \
  if ((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " << #a << " != " << #b << " (" << (a) << " != " << (b) << ")\n"; \
    ++test_failures; \
  } \
} while(0)
#define EXPECT_STR_EQ(a,b) do { \
  if (std::strcmp((a), (b)) != 0) { \
    std::cerr << "EXPECT_STR_EQ failed: \"" << (a) << "\" != \"" << (b) << "\"\n"; \
    ++test_failures; \
  } \
} while(0)

// Reset log capture state before each test
static void reset_log_capture() {
  g_log.iErr = 0;
  g_log.zFormat.clear();
  g_log.zType = nullptr;
  g_log.lineno = 0;
  g_log.sourceid_tail = nullptr;
  g_log.captured = false;
  sqlite3_sourceid_call_counter = 0;
}

// ---------------------- Test 1: Basic logging and return value ----------------------
// Purpose:
// - Ensure sqlite3ReportError returns the original iErr
// - Ensure sqlite3_log is invoked with the expected format string
// - Ensure the first two arguments after the format are zType and lineno
// - Ensure the third argument is the tail of sqlite3_sourceid() starting at index 20
static void test_sqlite3ReportError_basic() {
  reset_log_capture();

  const int iErr = 7;
  const int lineno = 100;
  const char* zType = "WARN";

  int ret = sqlite3ReportError(iErr, lineno, zType);

  // Return value must be exactly the input error code
  EXPECT_EQ(ret, iErr);

  // sqlite3_log must have been invoked and captured
  EXPECT_EQ(g_log.iErr, iErr);

  // The format string must be exactly as used in the production code
  EXPECT_STR_EQ(g_log.zFormat.c_str(), "%s at line %d of [%.10s]");

  // The line number passed to the logger must match
  EXPECT_EQ(g_log.lineno, lineno);

  // The zType argument must match the provided type
  EXPECT_STR_EQ(g_log.zType, zType);

  // The tail of the source id must be used (20 characters into the source id)
  const char* expected_tail = SQLITE_SOURCEID_STRING + 20;
  EXPECT_STR_EQ(g_log.sourceid_tail, expected_tail);

  // sqlite3_sourceid should be called exactly once
  EXPECT_EQ(get_sqlite3_sourceid_call_count(), 1);
}

// ---------------------- Test 2: Variant values (empty type and zero line) ---------------
// Purpose:
// - Exercise edge values: empty zType and lineno = 0
// - Validate that logging still follows the same contract
static void test_sqlite3ReportError_empty_and_zero() {
  reset_log_capture();

  const int iErr = -1;
  const int lineno = 0;
  const char* zType = "";

  int ret = sqlite3ReportError(iErr, lineno, zType);

  // Return value must be exactly the input error code
  EXPECT_EQ(ret, iErr);

  // Validate log capture
  EXPECT_EQ(g_log.iErr, iErr);
  EXPECT_STR_EQ(g_log.zFormat.c_str(), "%s at line %d of [%.10s]");
  EXPECT_EQ(g_log.lineno, lineno);
  EXPECT_STR_EQ(g_log.zType, zType);

  // Check the tail of the source id
  const char* expected_tail = SQLITE_SOURCEID_STRING + 20;
  EXPECT_STR_EQ(g_log.sourceid_tail, expected_tail);

  // sqlite3_sourceid should be called exactly once in this test as well
  EXPECT_EQ(get_sqlite3_sourceid_call_count(), 1);
}

// ---------------------- Main: Run tests and summarize results -------------------------
// Notes:
// - This main function serves as the entry point when GTest is not available.
// - It executes the two test cases above and reports the total failures.
// - All tests are designed to be executable without requiring the real SQLite binary.

int main() {
  test_sqlite3ReportError_basic();
  test_sqlite3ReportError_empty_and_zero();

  if (test_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Total failures: " << test_failures << "\n";
  }

  // Non-zero exit code indicates test failures
  return test_failures ? 1 : 0;
}
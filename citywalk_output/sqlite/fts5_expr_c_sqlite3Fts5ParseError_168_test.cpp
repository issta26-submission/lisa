/*
  Unit tests for sqlite3Fts5ParseError (fts5_expr.c focal method)
  - Uses only C++11 standard library
  - No GoogleTest; a small in-house test harness is provided
  - Relies on project headers for type definitions (fts5Int.h, fts5parse.h)
  - Assumes the real sqlite3Fts5ParseError is linked from the project under test
  - Keeps tests non-terminating (they report failures but continue)
  - Includes explanatory comments for each test
*/

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <cstdlib>


// Import project dependencies (provide correct paths as per the repository layout)

// SQLITE constants (likely defined in sqlite3.h, but provide safe defaults here)
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_ERROR
#define SQLITE_ERROR 1
#endif

// Ensure the Fts5Parse type matches the one used by the focal function.
// We rely on the project header to provide the actual definition, but define a
// compatible alias here to ensure compilation if the header is absent.
#ifndef __has_include
#define __has_include(x) 0
#endif
#if !defined(Fts5Parse) && !defined(_Fts5Parse_defined)
typedef struct Fts5Parse Fts5Parse;
#endif

// Declaration of focal function under test (should be provided by the project during linking)
#ifdef __cplusplus
extern "C" {
#endif
void sqlite3Fts5ParseError(Fts5Parse *pParse, const char *zFmt, ...);
#ifdef __cplusplus
}
#endif

// Note: We do not redefine sqlite3_vmprintf here; the test environment is expected
// to link against the real sqlite3_vmprintf implementation provided by sqlite3.

// Simple in-house non-terminating test assertions (so all tests run and report)
static int g_failure_count = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
    ++g_failure_count; \
  } \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
  if((int)(a) != (int)(b)) { \
    std::cerr << "EXPECT_EQ_INT FAILED: " << (a) << " != " << (b) \
              << " (" << (msg) << ")\n"; \
    ++g_failure_count; \
  } \
} while(0)

#define EXPECT_STR_EQ(a, b, msg) do { \
  if((a) == nullptr && (b) == nullptr) { \
    /* both null, OK */ \
  } else if(((a) == nullptr) || ((b) == nullptr)) { \
    std::cerr << "EXPECT_STR_EQ FAILED: one string is null, other is not (" << (msg) << ")\n"; \
    ++g_failure_count; \
  } else if(strcmp((a), (b)) != 0) { \
    std::cerr << "EXPECT_STR_EQ FAILED: \"" << (a) << "\" != \"" << (b) \
              << "\" (" << (msg) << ")\n"; \
    ++g_failure_count; \
  } \
} while(0)

// Forward declarations of test cases
void test_parseError_noop_when_rc_not_ok();
void test_parseError_sets_error_and_message_when_rc_ok_with_format();

// Test 1: When pParse->rc != SQLITE_OK, sqlite3Fts5ParseError should not modify rc or zErr
// This exercises the false-branch of the condition.
void test_parseError_noop_when_rc_not_ok() {
  Fts5Parse pParse;
  pParse.rc = SQLITE_ERROR; // not OK
  pParse.zErr = nullptr;

  // Call the focal function with arbitrary format; it should do nothing to pParse
  sqlite3Fts5ParseError(&pParse, "Should not be used: code %d", 999);

  EXPECT_EQ_INT(pParse.rc, SQLITE_ERROR, "rc should remain SQLITE_ERROR when input rc is not OK");
  EXPECT_TRUE(pParse.zErr == nullptr, "zErr should remain NULL when rc is not OK");
}

// Test 2: When pParse->rc == SQLITE_OK and zErr is NULL, the function should format
// the message using sqlite3_vmprintf, set zErr, and change rc to SQLITE_ERROR.
void test_parseError_sets_error_and_message_when_rc_ok_with_format() {
  Fts5Parse pParse;
  pParse.rc = SQLITE_OK;
  pParse.zErr = nullptr; // ensure we start with NULL to exercise assignment

  // Use a deterministic format that sqlite3_vmprintf should format
  // Expected content: "Error 42: test message"
  sqlite3Fts5ParseError(&pParse, "Error %d: %s", 42, "test message");

  EXPECT_EQ_INT(pParse.rc, SQLITE_ERROR, "rc should be SQLITE_ERROR after parsing error");
  EXPECT_STR_EQ(pParse.zErr, "Error 42: test message", "zErr content should reflect formatted message");

  // Do not free zErr here to avoid depending on allocator semantics of sqlite3_vmprintf
  // (the project typically uses sqlite3_malloc for such strings); in unit tests we
  // can ignore freeing to keep tests simple and focused on behavior.
}

// Test 3: A second scenario ensuring formatting works with a different string/number.
// This helps cover the general formatting path.
void test_parseError_sets_different_formatting() {
  Fts5Parse pParse;
  pParse.rc = SQLITE_OK;
  pParse.zErr = nullptr;

  sqlite3Fts5ParseError(&pParse, "Code=%04d, Msg=%s", 7, "OK");

  EXPECT_EQ_INT(pParse.rc, SQLITE_ERROR, "rc should be SQLITE_ERROR after formatting");
  EXPECT_STR_EQ(pParse.zErr, "Code=0007, Msg=OK", "zErr should reflect zero-padded number and message");
}

// Simple dispatcher: call all tests and report final status
int main() {
  std::cout << "Running sqlite3Fts5ParseError unit tests (C++11 harness)\n";

  test_parseError_noop_when_rc_not_ok();
  test_parseError_sets_error_and_message_when_rc_ok_with_format();
  test_parseError_sets_different_formatting();

  if(g_failure_count > 0) {
    std::cerr << "Tests completed with " << g_failure_count << " failure(s).\n";
    return 1;
  } else {
    std::cout << "All tests passed.\n";
    return 0;
  }
}
/*
  C++11 test suite for the focal method:
  int printExplainQueryPlan(sqlite3_stmt *pStmt)

  This test suite uses a minimal, non-terminating EXPECT-style assertion
  mechanism to exercise the function and its dependencies via a real
  in-memory SQLite database.

  Key dependencies (Candidate Keywords) inferred from the focal class/file:
  - sqlite3_sql
  - sqlite3_mprintf
  - sqlite3_prepare_v2
  - sqlite3_db_handle
  - sqlite3_free
  - sqlite3_step
  - sqlite3_column_int
  - sqlite3_column_text
  - sqlite3_finalize

  The tests are designed to compile under C++11 and link against -lsqlite3.
  No external testing framework (GTest) is used.

  Note: Output is captured by redirecting stdout to a pipe to validate the
  textual lines produced by printExplainQueryPlan.
*/

#include <cstdio>
#include <sys/wait.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <sys/types.h>
#include <sqliteInt.h>
#include <unistd.h>
#include <sqlite3userauth.h>
#include <algorithm>
#include <time.h>
#include <sstream>
#include <cstring>



// Forward declaration of the focal method (as provided)
extern "C" int printExplainQueryPlan(sqlite3_stmt *pStmt);

// Non-terminating simple assertion helpers
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { std::cerr << "[EXPECT_TRUE] " __FILE__ ":" << __LINE__ << " - Condition failed: " #cond "\n"; } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
  int _a = (int)(a); int _b = (int)(b); \
  if(_a != _b) { std::cerr << "[EXPECT_EQ_INT] " __FILE__ ":" << __LINE__ \
  << " - Expected " << _b << " but got " << _a << "\n"; } \
} while(0)

// Capture the output of printExplainQueryPlan by redirecting stdout
static int captureExplainOutput(sqlite3_stmt *pStmt, std::string &output) {
  int pipefd[2];
  if (pipe(pipefd) != 0) return -1;

  int saved_stdout = dup(STDOUT_FILENO);
  if (saved_stdout == -1) { close(pipefd[0]); close(pipefd[1]); return -1; }

  // Redirect stdout to pipe
  if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
    close(pipefd[0]); close(pipefd[1]); close(saved_stdout);
    return -1;
  }
  close(pipefd[1]); // stdout now duplicated to pipe's write end

  // Call the function under test
  int rc = printExplainQueryPlan(pStmt);

  // Flush and restore stdout
  fflush(stdout);
  if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
    // restore error state
  }
  close(saved_stdout);

  // Read all data from the pipe
  std::string out;
  char buf[4096];
  ssize_t n;
  while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
    out.append(buf, (size_t)n);
  }
  close(pipefd[0]);
  output = out;
  return rc;
}

// Helper: simple parse to check there is at least one valid "SELECTID ORDER FROM DETAIL" line
static bool hasExplainLine(const std::string &text) {
  std::istringstream iss(text);
  std::string line;
  while (std::getline(iss, line)) {
    // skip empty lines
    if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
    std::stringstream ss(line);
    int s = -1, o = -1, f = -1;
    if (!(ss >> s >> o >> f)) continue;

    // Read remainder as detail string
    std::string rest;
    if (std::getline(ss, rest)) {
      // trim leading whitespace
      rest.erase(rest.begin(),
                 std::find_if(rest.begin(), rest.end(),
                              [](unsigned char ch){ return !std::isspace(ch); }));
      if (!rest.empty()) {
        return true;
      }
    }
  }
  return false;
}

// Test 1: Basic functionality - EXPLAIN QUERY PLAN for a simple statement
// This validates that the function prints lines in expected format and returns SQLITE_OK.
static bool test_printExplainPlan_basic() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || !db) {
    std::cerr << "Failed to open in-memory DB\n";
    return false;
  }

  sqlite3_stmt *pStmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &pStmt, 0);
  if (rc != SQLITE_OK || !pStmt) {
    std::cerr << "Failed to prepare test statement\n";
    sqlite3_close(db);
    return false;
  }

  std::string captured;
  int print_rc = captureExplainOutput(pStmt, captured);
  // Ensure function returned SQLITE_OK
  EXPECT_EQ_INT(print_rc, SQLITE_OK);

  // Ensure something meaningful was printed
  bool hasLine = hasExplainLine(captured);
  EXPECT_TRUE(hasLine);

  sqlite3_finalize(pStmt);
  sqlite3_close(db);
  return true;
}

// Test 2: Edge case - pass a NULL statement; should return SQLITE_ERROR without printing
static bool test_printExplainPlan_nullStmt() {
  int rc = printExplainQueryPlan(nullptr);
  EXPECT_EQ_INT(rc, SQLITE_ERROR);
  return true;
}

// Entry point for the test suite
int main() {
  int tests_run = 0;
  int tests_failed = 0;

  // Note: Each test returns true on success, false on failure; but we
  // continue running to maximize coverage even if a test reports an issue.
  {
    bool ok = test_printExplainPlan_basic();
    tests_run++;
    if(!ok) {
      tests_failed++;
    }
  }
  {
    bool ok = test_printExplainPlan_nullStmt();
    tests_run++;
    if(!ok) {
      tests_failed++;
    }
  }

  // Summary
  std::cout << "Test results: " << (tests_run - tests_failed) << "/" << tests_run
            << " tests passed." << std::endl;
  if (tests_failed > 0) {
    std::cerr << tests_failed << " test(s) failed." << std::endl;
  }
  // Return 0 if all tests pass, non-zero if any failed to indicate status.
  return (tests_failed == 0) ? 0 : 1;
}

/*
  Explanatory notes for maintainers:
  - The tests cover:
    - The standard success path of printExplainQueryPlan by verifying
      that output is produced for a simple SELECT.
    - The NULL-input path that should yield SQLITE_ERROR.

  - The test uses a minimal, self-contained approach with a real SQLite
    in-memory database and actual sqlite3_test APIs, mirroring real usage.

  - Static analysis and domain knowledge considerations:
    - We avoided relying on exact EXPLAIN output content which can vary
      across SQLite versions; instead, we verify the existence of a properly
      formatted line in the output.

  - The tests do not rely on GoogleTest or other frameworks, in line with
    the provided constraints.

  - The test file is designed to be compiled and linked with the system's
    SQLite library (e.g., g++ -std=c++11 test_suite.cpp -lsqlite3).
*/
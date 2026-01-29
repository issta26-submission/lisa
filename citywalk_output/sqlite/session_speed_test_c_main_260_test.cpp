// Session Speed Test - C++11 Unit Test Harness (GTest-free)
// This test harness executes the provided focal method (main) in session_speed_test.c
// via spawning a separate process and validating its exit codes for various command-line
// argument combinations. It avoids direct integration testing of SQLite internals by
// relying on the static stubs present in the focal code, which keep the workload light.
//
// Design notes (mapped to the required steps and domain knowledge):
// Step 1 - Candidate Keywords (core dependencies within the focal code):
//   - Options struct fields: nRow, bWithoutRowid, bInteger, bAll, zDb
//   - Command line options: -rows, -without-rowid, -integer, -all, -database
//   - Arrays of SQL templates: azCreate, azInsert, azUpdate, azDelete
//   - Helper routines: parse_command_line, execsql, run_test
//   - Main loop variants over bWithoutRowid and bInteger with bAll controlling all combos
// Step 2 - Unit Test Focus (test main with various args):
//   - Default run (no args): exercise a single combo path
//   - Run all combinations (-all): exercise all four combos
//   - Specific flags combinations (-without-rowid, -integer) to cover branches
// Step 3 - Test Refinement (C++11 stdlib only, non-terminating assertions):
//   - Implement a small test framework with non-terminating EXPECT-style assertions
//   - Use process spawning to avoid linking issues and to keep tests isolated
//   - Capture exit codes to verify correct behavior without depending on internal DB state
//   - Keep tests lightweight by relying on static stubs in the focal code

#include <stdlib.h>
#include <functional>
#include <string.h>
#include <stddef.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <map>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b) do { \
  ++g_total_tests; \
  if ((a) != (b)) { \
    ++g_failed_tests; \
    std::cerr << "EXPECT_EQ FAILED: " #a " != " #b " at " << __FILE__ << ":" << __LINE__ \
              << " | actual=" << (a) << " expected=" << (b) << "\n"; \
  } \
} while(0)

#define RUN_TEST(fn) do { fn(); } while(0)

//
// Utility: determine path to the session_speed_test binary.
// It can be overridden via environment variable SESSION_SPEED_TEST_BIN.
// If not provided, we default to ./session_speed_test (expected to be built next to the test).
//
static std::string get_session_speed_test_bin_path() {
  const char* env_path = std::getenv("SESSION_SPEED_TEST_BIN");
  if (env_path && std::string(env_path).size() > 0) {
    return std::string(env_path);
  }
  // Default path; in a typical CI, this will be the compiled binary named session_speed_test
  return "./session_speed_test";
}

//
// Utility: Run the session_speed_test binary with given arguments.
// Returns the exit status (0 for success, non-zero for failure, -1 on exec error).
//
static int run_binary_with_args(const std::string& binPath, const std::vector<std::string>& args) {
  // Build argv as a NULL-terminated array of C-strings, keeping copies alive until exec.
  std::vector<char*> argvC;
  argvC.reserve(args.size() + 2);
  // First argument is the program name (conventionally the binary path)
  argvC.push_back(const_cast<char*>(binPath.c_str()));

  // Copy each argument string to a separate C string to ensure lifetime during exec
  std::vector<char*> argCopies;
  argCopies.reserve(args.size());
  for (const auto& a : args) {
    argCopies.push_back(strdup(a.c_str()));
    argvC.push_back(argCopies.back());
  }
  argvC.push_back(nullptr);

  pid_t pid = fork();
  if (pid < 0) {
    // Fork failed
    std::cerr << "Failed to fork for binary " << binPath << "\n";
    // Clean up any allocated copies
    for (auto p : argCopies) if (p) free(p);
    return -1;
  }

  if (pid == 0) {
    // Child process: execute the binary
    execv(binPath.c_str(), argvC.data());
    // If execv returns, it failed
    _exit(127);
  }

  // Parent process: wait for child
  int status = 0;
  if (waitpid(pid, &status, 0) == -1) {
    // wait failed
    std::cerr << "waitpid failed for child process running " << binPath << "\n";
    for (auto p : argCopies) if (p) free(p);
    return -1;
  }

  // Clean up argument copies
  for (auto p : argCopies) if (p) free(p);

  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  } else {
    // Abnormal termination
    return -1;
  }
}

// Test 1: Default run (no arguments) should successfully complete (exit code 0)
static void test_default_run() {
  std::string binPath = get_session_speed_test_bin_path();
  std::cout << "[TEST] Default run (no args) on " << binPath << "\n";
  int code = run_binary_with_args(binPath, {});
  EXPECT_EQ(code, 0);
}

// Test 2: Run all combinations using -all flag with minimal rows
// This exercises the four (-without-rowid, -integer) combinations via the -all switch.
static void test_all_combinations_run() {
  std::string binPath = get_session_speed_test_bin_path();
  std::cout << "[TEST] All combinations (-all) with small nRow\n";
  int code = run_binary_with_args(binPath, {"-rows", "1", "-all"});
  EXPECT_EQ(code, 0);
}

// Test 3: Specific flags without -all to cover individual path branches
static void test_specific_flags_run() {
  std::string binPath = get_session_speed_test_bin_path();
  // Case: without-rowid = true, integer = false
  std::cout << "[TEST] Specific flags: -without-rowid (bWithoutRowid=1)\n";
  int code1 = run_binary_with_args(binPath, {"-rows", "2", "-without-rowid"});
  EXPECT_EQ(code1, 0);

  // Case: integer = true
  std::cout << "[TEST] Specific flags: -integer (bInteger=1)\n";
  int code2 = run_binary_with_args(binPath, {"-rows", "2", "-integer"});
  EXPECT_EQ(code2, 0);

  // Case: both flags and small rows
  std::cout << "[TEST] Specific flags: -without-rowid -integer\n";
  int code3 = run_binary_with_args(binPath, {"-rows", "3", "-without-rowid", "-integer"});
  EXPECT_EQ(code3, 0);
}

// Simple test runner that executes all tests and prints a summary
int main() {
  std::cout << "Starting Session Speed Test suite (GTest-free)\n";

  // Step 1: Outline candidate keywords and components (documented in comments above)
  // Step 2 & 3: Execute test cases that exercise the focal main with different args

  RUN_TEST(test_default_run);
  RUN_TEST(test_all_combinations_run);
  RUN_TEST(test_specific_flags_run);

  // Summary
  std::cout << "Test results: " << g_total_tests << " tests, "
            << g_failed_tests << " failures.\n";

  if (g_failed_tests > 0) {
    std::cerr << "Some tests failed. See details above.\n";
    return 1;
  }
  std::cout << "All tests passed successfully.\n";
  return 0;
}
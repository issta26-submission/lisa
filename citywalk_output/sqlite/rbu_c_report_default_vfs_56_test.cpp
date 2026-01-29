// Test suite for the focal method: report_default_vfs
// This test suite is self-contained and does not rely on GTest.
// It mocks sqlite3_vfs_find to return controllable values and captures stdout
// to verify the exact behavior of report_default_vfs.
//
// Context: The focal method prints the name of the default VFS or "NULL" if none.
// We replicate the minimal dependencies locally to make the test self-sufficient.

#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sqlite3rbu.h>
#include <string>
#include <iostream>


// -----------------------------------------------------------------------------
// Minimal stubs to mimic the dependencies used by report_default_vfs.
// We do not depend on the real sqlite3 library; we implement a tiny in-process mock.

struct sqlite3_vfs {
  const char *zName;
};

// Global fake VFS pointer used by the mock sqlite3_vfs_find.
// Each test case will set this to control the outcome of report_default_vfs.
static sqlite3_vfs *g_fake_vfs = NULL;

// Mock implementation of sqlite3_vfs_find used by report_default_vfs.
// It simply returns the current global fake VFS pointer.
static sqlite3_vfs* sqlite3_vfs_find(int /*i*/){
  (void)0;
  return g_fake_vfs;
}

// The focal method under test (re-implemented locally for the unit test).
// It should be identical in behavior to the production version.
void report_default_vfs(){
  sqlite3_vfs *pVfs = sqlite3_vfs_find(0);
  printf("default vfs is \"%s\"\n", pVfs ? pVfs->zName : "NULL");
}

// -----------------------------------------------------------------------------
// Test harness utilities: stdout capture and simple assertions.

// Capture stdout produced by calling func. Returns captured output as std::string.
static std::string capture_stdout(void (*func)()){
  // Create a pipe to capture stdout
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return "";
  }

  // Save and redirect stdout to the pipe's write end
  int saved_stdout = dup(fileno(stdout));
  if (saved_stdout == -1) {
    // Cleanup
    close(pipefd[0]);
    close(pipefd[1]);
    return "";
  }

  // Redirect stdout to pipe
  if (dup2(pipefd[1], fileno(stdout)) == -1) {
    // Cleanup
    close(pipefd[0]);
    close(pipefd[1]);
    close(saved_stdout);
    return "";
  }
  // We can close the original write end; stdout now points to the pipe
  close(pipefd[1]);

  // Run the test function which should write to stdout
  func();

  // Flush and restore stdout
  fflush(stdout);
  if (dup2(saved_stdout, fileno(stdout)) == -1) {
    // Cleanup
    close(saved_stdout);
    close(pipefd[0]);
    return "";
  }
  close(saved_stdout);

  // Read from the pipe
  std::string output;
  char buf[1024];
  ssize_t n;
  // The read end of the pipe is pipefd[0]
  while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
    output.append(buf, static_cast<size_t>(n));
  }
  close(pipefd[0]);
  return output;
}

// Simple test function wrappers that set up the fake VFS and invoke report_default_vfs.

// Named VFS used for testing the true-branch behavior.
static sqlite3_vfs named_vfs = { "myvfs" };

// Test case 1: the default VFS is NULL. We expect the output to contain "NULL".
static void test_case_null_run() {
  g_fake_vfs = NULL;
  report_default_vfs();
}

// Test case 2: the default VFS has a specific name. We expect that name in output.
static void test_case_named_run() {
  g_fake_vfs = &named_vfs;
  report_default_vfs();
}

// Helper to run a test case and compare its captured output against expectation.
static bool run_test_case(void (*test_func)(), const char* expected){
  std::string out = capture_stdout(test_func);
  // Non-terminating assertion: just indicate pass/fail, don't abort on fail.
  if (out == expected) {
    std::cout << "PASS: expected output matched: \"" << expected << "\"\n";
    return true;
  } else {
    std::cerr << "FAIL: expected output \"" << expected
              << "\" but got \"" << out << "\"\n";
    return false;
  }
}

// -----------------------------------------------------------------------------
// Domain-informed test runner: main executes tests in a single process.
// It uses only the C++ standard library and standard I/O for outputs.
// It prints a short summary to aid quick feedback during development.

int main(int argc, char **argv){
  // Explain to user what tests are running
  std::cout << "Starting unit tests for report_default_vfs...\n";

  // Run tests
  bool t1 = run_test_case(test_case_null_run, "default vfs is \"NULL\"\n");
  bool t2 = run_test_case(test_case_named_run, "default vfs is \"myvfs\"\n");

  int total = 2;
  int failures = (!t1) + (!t2);

  // Summary
  if (failures == 0) {
    std::cout << "ALL TESTS PASSED (" << total << " cases).\n";
    return 0;
  } else {
    std::cerr << "TESTS FAILED: " << failures << " of " << total << " cases failed.\n";
    return 1;
  }
}
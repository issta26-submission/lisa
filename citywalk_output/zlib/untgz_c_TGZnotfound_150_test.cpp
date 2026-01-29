/*
Unit test suite for the focal method TGZnotfound in untgz.c

Overview:
- This test suite is designed to exercise TGZnotfound(const char *arcname) without GTest.
- It uses POSIX fork/pipe to capture stderr from a child process where TGZnotfound is invoked.
- It relies on the existence of external C symbols:
  - char *prog
  - char *TGZsuffix[]
  - void TGZnotfound(const char *arcname)
- The tests modify TGZsuffix at runtime to control the number of suffixes used by TGZnotfound,
  enabling coverage of both a single suffix and multiple suffixes (covering both branches of the print logic).
- The tests verify the exact output string written to stderr and that TGZnotfound exits with status 1.

Notes on the test design (mapped to Candidate Keywords and domain knowledge):
- TGZnotfound depends on:
  - prog: program name printed as prefix
  - arcname: the provided archive name
  - TGZsuffix: array of suffixes terminated by NULL
- Output pattern to verify:
  - "<prog>: Couldn't find " followed by a sequence of formed strings using arcname and TGZsuffix, ending with a newline
  - It uses a ternary to decide between ", " or "\n" with a leading "or " for the last element
- Test coverage goals:
  - Case 1: single suffix -> ensures the "or <arcname><suffix>\n" path is taken
  - Case 2: two suffixes -> ensures the comma path and the final "or" path are exercised
- Static members: TGZnotfound relies on global (likely non-static) TGZsuffix and prog; tests access them via extern "C" declarations.
- Assertions: non-GTest style; tests return bools and are invoked from main(), printing a summary.

Compilation/Linking:
- This test file should be compiled and linked together with untgz.c
  (and any other objects providing the required externs).
- The tests run in a POSIX environment where fork/pipe/waitpid are available.

Code: (C++11, no GTest, includes explanatory comments for each test)
*/

#include <cstring>
#include <string.h>
#include <cstdlib>
#include <cerrno>
#include <errno.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// Forward declarations of the focal C code's API and globals.
// We declare them with C linkage to match the C code in untgz.c.
extern "C" {
  // Global program name used in TGZnotfound's output
  extern char *prog;

  // Null-terminated array of suffix strings used by TGZnotfound
  extern char *TGZsuffix[];

  // The focal function under test
  void TGZnotfound(const char *arcname);
}

// Helper to run TGZnotfound in a separate process and capture stderr output
// The function returns the captured output as a string and leaves the child
// process exit status to be checked by the caller.
static std::string run_notfound_in_child(const char *arcname, int &child_status) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    perror("pipe");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    // Child: redirect stderr to the pipe and invoke TGZnotfound
    dup2(pipefd[1], STDERR_FILENO);
    // Close unused fds
    close(pipefd[0]);
    close(pipefd[1]);

    // Configure globals for deterministic output
    // (The tests assume TGZnotfound uses these globals)
    std::cout.flush();

    TGZnotfound(arcname); // This will call exit(1) on failure

    // If, for any reason, TGZnotfound returns (pathological), exit explicitly
    _exit(1);
  } else {
    // Parent: close write end, read from read end, wait for child
    close(pipefd[1]);

    // Read all data from the pipe (stderr output)
    std::string captured;
    char buffer[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      captured.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);

    // Wait for child to finish and capture exit status
    int status = 0;
    waitpid(pid, &status, 0);
    child_status = status;
    return captured;
  }
  // Unreachable
  return std::string();
}

// Test 1: Single suffix path
// TGZsuffix[0] = ".zip"; TGZsuffix[1] = NULL;
// Expected output: "<prog>: Couldn't find or <arcname>.zip\n"
static bool test_notfound_single_suffix() {
  // Set deterministic program name and suffixes
  prog = (char*)"untgz";
  TGZsuffix[0] = (char*)".zip";
  TGZsuffix[1] = NULL;

  int child_status = 0;
  std::string output = run_notfound_in_child("archive.tar", child_status);

  // Expected exact message
  std::string expected = "untgz: Couldn't find or archive.tar.zip\n";

  // Validate: child exited with status 1 and output matches exactly
  bool ok = false;
  if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 1) {
    ok = (output == expected);
  } else {
    std::cerr << "Child did not exit cleanly in single-suffix test.\n";
  }

  if (!ok) {
    std::cerr << "Test notfound_single_suffix failed.\n"
              << "Expected: " << expected
              << "Actual: " << output << "\n";
  }
  return ok;
}

// Test 2: Two suffixes path
// TGZsuffix[0] = ".zip"; TGZsuffix[1] = ".gz"; TGZsuffix[2] = NULL;
// Expected output: "<prog>: Couldn't find arcname.zip, or arcname.gz\n"
static bool test_notfound_two_suffixes() {
  // Set deterministic program name and suffixes
  prog = (char*)"untgz";
  TGZsuffix[0] = (char*)".zip";
  TGZsuffix[1] = (char*)".gz";
  TGZsuffix[2] = NULL;

  int child_status = 0;
  std::string output = run_notfound_in_child("archive.tar", child_status);

  // Expected exact message
  std::string expected = "untgz: Couldn't find archive.tar.zip, or archive.tar.gz\n";

  // Validate: child exited with status 1 and output matches exactly
  bool ok = false;
  if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 1) {
    ok = (output == expected);
  } else {
    std::cerr << "Child did not exit cleanly in two-suffix test.\n";
  }

  if (!ok) {
    std::cerr << "Test notfound_two_suffixes failed.\n"
              << "Expected: " << expected
              << "Actual: " << output << "\n";
  }
  return ok;
}

int main() {
  // Run tests and report results
  int passed = 0, failed = 0;

  if (test_notfound_single_suffix()) ++passed;
  else ++failed;

  if (test_notfound_two_suffixes()) ++passed;
  else ++failed;

  std::cout << "TGZnotfound test suite results: "
            << "passed=" << passed << ", failed=" << failed << std::endl;

  // Return non-zero if any test failed
  return (failed != 0) ? 1 : 0;
}
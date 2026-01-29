/*
  Test Suite for the focal method:
    u8 sqlite3VdbeOneByteSerialTypeLen(u8 serial_type)

  Step 1 (Program Understanding) and Candidate Keywords (embedded as commentary):
  - sqlite3SmallTypeSizes: the backing data source used by the focal method.
  - sqlite3VdbeOneByteSerialTypeLen: returns the size from the above array after a boundary assertion.
  - serial_type: 0..127 valid inputs; 128+ triggers an assertion failure.
  - assert predicate: serial_type < 128 must hold; we will test both a valid path and the abort path.
  - Boundary and range coverage: test for representative values and loop coverage for 0..127.
  - C linkage: function is C, tests are C++11; use extern "C" for interfacing.

  Step 2 (Unit Test Generation) Guidance applied:
  - Tests rely on actual linkage to the real function sqlite3VdbeOneByteSerialTypeLen and the
    actual global array sqlite3SmallTypeSizes defined in the project (vdbeaux.c and related sources).
  - Positive path: verify return equals sqlite3SmallTypeSizes[serial_type].
  - Negative/path-abort: verify that invalid inputs (>= 128) trigger assertion aborts by isolating
    the call in a child process (fork) and checking for SIGABRT.
  - Static/global scope usage: we access the global array and function via extern "C".

  Step 3 (Test Case Refinement) Notes:
  - Use POSIX fork to isolate assertion abort behavior without terminating the test runner.
  - Provide clear per-test comments explaining intent, expected behavior, and boundary coverage.
  - Do not rely on a testing framework (GTest), implement a simple main() based test runner.
  - Respect standard C++11 and only rely on standard library and provided project symbols.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <unistd.h>
#include <iostream>


// Forward declarations of external C symbols from the project under test.
// The test harness assumes the real project provides these symbols at link time.
extern "C" {
  // The array that maps serial_type to its small size (128 entries in the real project)
  // We access this to validate that the focal function returns sqlite3SmallTypeSizes[serial_type].
  extern unsigned char sqlite3SmallTypeSizes[128];

  // The focal function under test (C linkage)
  extern unsigned char sqlite3VdbeOneByteSerialTypeLen(unsigned char serial_type);
}

// Convenience typedef for clarity
using u8 = unsigned char;

/*
  Helper: Run the focal function with a given serial_type in a child process.
  The function uses an assertion (serial_type < 128). If the assertion fails, the child process
  will be aborted (SIGABRT). We capture that signal to verify abort behavior without killing the
  test runner.
*/
static bool runAbortTestInChild(u8 val) {
  pid_t pid = fork();
  if (pid < 0) {
    // Fork failed; cannot run the isolated test
    std::cerr << "fork() failed for abort test with value: " << int(val) << "\n";
    return false;
  }
  if (pid == 0) {
    // Child process: run the focal function; if assertion fails, this will abort the process
    // and exit with a non-zero status.
    (void) sqlite3VdbeOneByteSerialTypeLen(val);
    _exit(0); // If we reached here, assertion did not trigger (unexpected for invalid input)
  } else {
    // Parent: wait for child result
    int status = 0;
    if (waitpid(pid, &status, 0) == -1) {
      std::cerr << "waitpid() failed\n";
      return false;
    }
    // Check if child was terminated by SIGABRT (assertion triggered)
#if defined(WIFSIGNALED) && defined(WTERMSIG)
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
      return true; // Expected abort
    }
    // If terminated normally, or by a different signal, it's a failure for this test
    return false;
#else
    // If macros aren't available, conservatively treat as failure
    return false;
#endif
  }
}

/*
  Test 1: Valid range coverage
  - For every serial_type in 0..127, the function should return sqlite3SmallTypeSizes[serial_type].
  - Verifies true-branch behavior across the full range and uses the known backing array.
*/
static bool test_valid_range_all_values() {
  for (int i = 0; i < 128; ++i) {
    u8 expected = sqlite3SmallTypeSizes[i];
    u8 actual = sqlite3VdbeOneByteSerialTypeLen(static_cast<u8>(i));
    if (actual != expected) {
      std::cerr << "[ERROR] Mismatch at serial_type=" << i
                << ": expected " << int(expected)
                << ", got " << int(actual) << "\n";
      return false;
    }
  }
  std::cout << "[PASS] test_valid_range_all_values: 0..127 correctly map to sqlite3SmallTypeSizes[].\n";
  return true;
}

/*
  Test 2: Boundary condition abort
  - Verify that serial_type = 128 triggers an assertion abort (SIGABRT) in a child process.
  - This confirms the false-branch path of the predicate serial_type < 128 is handled via abort.
*/
static bool test_boundary_abort_128() {
  bool aborted = runAbortTestInChild(128);
  if (!aborted) {
    std::cerr << "[ERROR] Expected abort for serial_type=128, but abort did not occur.\n";
    return false;
  }
  std::cout << "[PASS] test_boundary_abort_128: abort occurs as expected for serial_type=128.\n";
  return true;
}

/*
  Test 3: Additional boundary check (optional robustness)
  - Optional extra test to ensure larger invalid inputs (e.g., 255) also aborts in child.
*/
static bool test_boundary_abort_255() {
  bool aborted = runAbortTestInChild(255);
  if (!aborted) {
    std::cerr << "[ERROR] Expected abort for serial_type=255, but abort did not occur.\n";
    return false;
  }
  std::cout << "[PASS] test_boundary_abort_255: abort occurs as expected for serial_type=255.\n";
  return true;
}

/*
  Main entry: run all tests and report aggregate result.
  Exits with code 0 on success; non-zero if any test fails.
*/
int main() {
  std::cout << "Starting test suite for sqlite3VdbeOneByteSerialTypeLen...\n";

  bool ok = true;

  // Run tests; each test prints its own PASS/FAIL message
  ok &= test_valid_range_all_values();
  ok &= test_boundary_abort_128();
  ok &= test_boundary_abort_255();

  if (ok) {
    std::cout << "[OVERALL PASS] sqlite3VdbeOneByteSerialTypeLen tests completed successfully.\n";
    return 0;
  } else {
    std::cerr << "[OVERALL FAIL] One or more tests failed for sqlite3VdbeOneByteSerialTypeLen.\n";
    return 1;
  }
}
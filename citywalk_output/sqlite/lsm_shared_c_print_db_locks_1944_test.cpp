// Unit test suite for the focal method: print_db_locks (located in lsm_shared.c)
// This test suite is designed as a standalone C++11 program (no GTest) and
// uses a minimal test harness to validate stdout output produced by print_db_locks.
// It relies on a test-provided shmLockType() to control the lock state per iLock.
// Notes:
// - We provide a lightweight forward declaration of lsm_db to satisfy the
//   function signature of print_db_locks without needing the full project header.
// - The tests capture stdout via a POSIX pipe to verify exact textual output.
// - The tests cover true/false branches of the dependent predicate by varying
//   the return values of shmLockType for specific iLock indices.
// - This approach focuses on exercising the observable behavior (printed strings)
//   rather than internal private state (as required by <DOMAIN_KNOWLEDGE>).

#include <sys/types.h>
#include <lsmInt.h>
#include <string>
#include <unistd.h>
#include <iostream>


// Forward declare the C linkage types and functions expected by the focal method.
extern "C" {
  // Forward declaration for the focal class type used by print_db_locks.
  struct lsm_db;

  // Focal method under test (provided in lsm_shared.c in the real project).
  // We declare it here so we can call it from C++ code.
  void print_db_locks(struct lsm_db *db);

  // The lock-type query function used by the focal method.
  // We will provide a test-specific implementation to control behavior.
  int shmLockType(struct lsm_db *db, int iLock);
}

// Lightweight stub for lsm_db used solely for test harness.
// The real type is opaque to the test since print_db_locks only passes it through.
struct lsm_db {
  int dummy; // unused, just to have a complete type for compilation
};

// Global test mode to drive shmLockType behavior in tests.
// 0: all 0s (no locks), 1: only iLock==1 yields shared, 2: iLock==1->shared, iLock==2->exclusive
static int g_test_case = 0;

// Test stub for shmLockType that is used by print_db_locks during tests.
// This approach allows deterministic control of iLock outputs without modifying
// lsm_shared.c. In a full build, linking should provide the real function; for
// test purposes, this stub is intended to be selected by the linker in place of
// the real implementation (or to be overridden via build configuration).
extern "C" int shmLockType(struct lsm_db *db, int iLock) {
  switch (g_test_case) {
    case 0:
      // Test Case 0: All iLock elements report "no lock" (0)
      return 0;
    case 1:
      // Test Case 1: Only iLock == 1 reports "shared" (1)
      return (iLock == 1) ? 1 : 0;
    case 2:
      // Test Case 2: iLock == 1 -> shared, iLock == 2 -> exclusive, others -> none
      if (iLock == 1) return 1;
      if (iLock == 2) return 2;
      return 0;
    default:
      return 0;
  }
}

// Helper type that represents a test case; used to pass function pointers to the
// capture helper.
using PrintFn = void(*)(struct lsm_db*);

// Capture stdout produced by a call to the given function with provided db.
// Returns the captured output as a std::string.
std::string capture_output(PrintFn f, struct lsm_db* db) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    // Fallback: if pipe creation fails, return empty output to indicate failure.
    return "";
  }

  // Redirect stdout to the write end of the pipe
  int saved_stdout = dup(fileno(stdout));
  dup2(pipefd[1], fileno(stdout));
  close(pipefd[1]); // The write end is now stdout

  // Invoke the function that prints to stdout
  f(db);

  // Flush and restore stdout
  fflush(stdout);
  dup2(saved_stdout, fileno(stdout));
  close(saved_stdout);

  // Read the captured output from the read end of the pipe
  std::string output;
  char buffer[256];
  ssize_t n;
  // Re-open the read end and read all data
  while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
    output.append(buffer, buffer + n);
  }
  close(pipefd[0]);

  return output;
}

// Wrapper to call the focal function print_db_locks.
// This matches the PrintFn signature used by capture_output.
void call_print_db_locks(struct lsm_db* db) {
  print_db_locks(db);
}

// Simple assertion helper to report test results
void assert_equal(const std::string &actual, const std::string &expected, const std::string &test_name) {
  if (actual == expected) {
    std::cout << "[PASS] " << test_name << "\n";
  } else {
    std::cout << "[FAIL] " << test_name << "\n";
    std::cout << "  Expected: " << (expected.empty() ? "<empty>" : "\"" + expected + "\"") << "\n";
    std::cout << "  Actual  : " << (actual.empty() ? "<empty>" : "\"" + actual + "\"") << "\n";
  }
}

int main() {
  // Candidate Keywords (for understanding and mapping to test goals)
  // - shmLockType: determines the lock type for each iLock
  // - azLock: {0, "shared", "exclusive"} mapping index -> string label
  // - azName: {0, "dms1", "dms2", "writer", "worker", "checkpointer", "reader0", ...}
  // - iLock loop: 0 to 15 (16 iterations)
  // - bOne: controls spacing/prefixing for multiple lock prints
  // - print_db_locks: prints one or more lock annotations per iLock, then newline

  struct lsm_db db; // dummy instance to pass to print_db_locks

  // Test 1: No locks anywhere (iLock 0..15 -> shmLockType returns 0)
  // Expectation: only a newline is printed
  g_test_case = 0;
  std::string out1 = capture_output(call_print_db_locks, &db);
  assert_equal(out1, "\n", "print_db_locks: no locks -> should print only newline");

  // Test 2: Only iLock == 1 is shared
  // Expectation: one lock annotation "(shared on dms1)" with no preceding space
  // then newline at the end
  g_test_case = 1;
  std::string out2 = capture_output(call_print_db_locks, &db);
  assert_equal(out2, "(shared on dms1)\n", "print_db_locks: single shared lock at iLock=1 -> expect one annotation");

  // Test 3: iLock 1 is shared and iLock 2 is exclusive
  // Expectation: first "(shared on dms1)" then a space and "(exclusive on dms2)" then newline
  g_test_case = 2;
  std::string out3 = capture_output(call_print_db_locks, &db);
  assert_equal(out3, "(shared on dms1) (exclusive on dms2)\n", "print_db_locks: mixed shared/exclusive at iLock=1,2 -> expect two annotations with spacing");

  return 0;
}
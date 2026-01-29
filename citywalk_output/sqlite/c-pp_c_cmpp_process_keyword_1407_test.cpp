// Unit test suite for cmpp_process_keyword (C function from c-pp.c) using C++11
// This test suite does not rely on GTest. It provides a minimal harness with
// three tests:
//  - Normal path: where preconditions hold (pKw != NULL and argc != 0). Verifies
//    that xCall is invoked and that internal state is reset.
//  - Assertion tests (posix fork): ensure that when preconditions fail (pKw == NULL),
//    the debug assertion aborts the process (captured in a child process).
//  - Assertion tests (posix fork): ensure that when preconditions fail (argc == 0),
//    the debug assertion aborts the process (captured in a child process).
// Notes:
// - The test suite assumes a POSIX environment for process forking to test asserts.
// - The real cmpp_process_keyword is defined in c-pp.c and uses the actual CmppTokenizer
//   and CmppKeyword structures. We mirror their expected layout with forward-declared
//   structs to interact with the function safely for testing.
// - We expose the C function cmpp_process_keyword via extern "C" linkage.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <csignal>
#include <assert.h>
#include <sys/wait.h>
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge: mirror minimal necessary structures and declarations
extern "C" {
  // Forward declare types to match the signature used by cmpp_process_keyword
  typedef struct CmppKeyword CmppKeyword;
  typedef struct CmppTokenizer CmppTokenizer;

  // Function under test
  void cmpp_process_keyword(CmppTokenizer * const t);

  // We provide a small wrapper to help compile/link with the real C implementation.
  // The real c-pp.c is expected to define CmppTokenizer/CmppKeyword in a compatible way.
}

// Local test-specific definitions intended to mirror the actual layout.
// This layout must be compatible with the layout used inside c-pp.c for safe access.
typedef struct CmppKeyword {
  void (*xCall)(struct CmppKeyword*, CmppTokenizer*);
} CmppKeyword;

typedef struct CmppTokenizer {
  struct {
    CmppKeyword* pKw;
    unsigned int argc;
  } args;
} CmppTokenizer;

// Globals used to verify that the keyword's xCall was actually invoked
static int g_kw_call_count = 0;
static CmppTokenizer* g_last_t_ptr = nullptr;

// The xCall implementation used by tests; increments a counter and records the tokenizer
static void test_kw_xCall(CmppKeyword* kw, CmppTokenizer* t) {
  (void)kw; // not strictly needed for verification beyond call count
  g_kw_call_count++;
  g_last_t_ptr = t;
}

// Helper to set up a valid CmppKeyword with our xCall
static CmppKeyword* create_test_keyword() {
  static CmppKeyword kw;
  kw.xCall = test_kw_xCall;
  return &kw;
}

// Test 1: Normal path where preconditions hold (pKw non-null and argc non-zero)
static int test_cmpp_process_keyword_normal_path() {
  // Prepare tokenizer and keyword
  CmppKeyword* pKw = create_test_keyword();
  CmppTokenizer t;
  t.args.pKw = pKw;
  t.args.argc = 1; // non-zero so asserts pass

  // Reset global counters
  g_kw_call_count = 0;
  g_last_t_ptr = nullptr;

  // Call the function under test
  cmpp_process_keyword(&t);

  // Validate post-conditions
  if (g_kw_call_count != 1) {
    std::cerr << "FAIL: cmpp_process_keyword did not invoke xCall exactly once.\n";
    return 1;
  }
  if (g_last_t_ptr != &t) {
    std::cerr << "FAIL: cmpp_process_keyword called xCall with incorrect tokenizer pointer.\n";
    return 1;
  }
  if (t.args.pKw != nullptr) {
    std::cerr << "FAIL: cmpp_process_keyword did not reset pKw to nullptr.\n";
    return 1;
  }
  if (t.args.argc != 0) {
    std::cerr << "FAIL: cmpp_process_keyword did not reset argc to 0.\n";
    return 1;
  }

  std::cout << "PASS: test_cmpp_process_keyword_normal_path\n";
  return 0;
}

// Helper: run a child process that expects an assertion failure (SIGABRT)
static int run_child_assertion_test(void (*setup_child)(), const char* test_desc) {
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "ERROR: fork() failed for " << test_desc << "\n";
    return -1;
  }
  if (pid == 0) {
    // Child: run the test setup and call cmpp_process_keyword, which should ASSERT
    setup_child();
    // If we reach here, the assertion did not abort; exit with a distinct code.
    _exit(2);
  } else {
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
      // Child aborted due to assertion as expected
      std::cout << "PASS: " << test_desc << " (child aborted as expected)\n";
      return 0;
    } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      // Unexpected: child exited successfully
      std::cerr << "FAIL: " << test_desc << " - child exited unexpectedly.\n";
      return 1;
    } else {
      // Other failure modes
      std::cerr << "FAIL: " << test_desc << " - child did not abort with SIGABRT.\n";
      return 1;
    }
  }
}

// Child setup for test 2: pKw == NULL triggers assertion
static void child_setup_assert_pKw_null() {
  CmppKeyword* pKw = 0; // NULL to trigger assertion
  CmppTokenizer t;
  t.args.pKw = pKw;
  t.args.argc = 1;
  cmpp_process_keyword(&t);
}

// Child setup for test 3: argc == 0 triggers assertion
static void child_setup_assert_argc_zero() {
  CmppKeyword* pKw = create_test_keyword();
  CmppTokenizer t;
  t.args.pKw = pKw;
  t.args.argc = 0; // zero to trigger assertion
  cmpp_process_keyword(&t);
}

// Test 2: Assert-when-pKw-null (executed in a separate process to catch SIGABRT)
static int test_cmpp_process_keyword_assert_pKw_null() {
  return run_child_assertion_test(child_setup_assert_pKw_null,
                                "cmpp_process_keyword_assert_pKw_null");
}

// Test 3: Assert-when-argc-zero (executed in a separate process to catch SIGABRT)
static int test_cmpp_process_keyword_assert_argc_zero() {
  return run_child_assertion_test(child_setup_assert_argc_zero,
                                "cmpp_process_keyword_assert_argc_zero");
}

// Entrypoint for running all tests
int main() {
  int failures = 0;

  // Test 1: Normal path
  failures += test_cmpp_process_keyword_normal_path();

  // Tests 2 & 3: Assert-triggering paths (must be run in child processes)
  failures += test_cmpp_process_keyword_assert_pKw_null();
  failures += test_cmpp_process_keyword_assert_argc_zero();

  if (failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cerr << failures << " TEST(S) FAILED\n";
  }
  return failures;
}
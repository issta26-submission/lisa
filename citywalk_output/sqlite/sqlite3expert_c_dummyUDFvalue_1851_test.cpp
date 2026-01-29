// This test suite targets the focal method: dummyUDFvalue in sqlite3expert.c
// It covers:
// - The intended abort-on-call behavior via assert(0) (tested using a SIGABRT handler and setjmp/longjmp to catch the abort without terminating the test process).
// - Compile-time signature compatibility of dummyUDFvalue (ensures the function can be assigned to a matching function pointer type).
// - Basic assertions to verify test results (no GTest/GMock used).

// Step 1: Candidate Keywords derived from the focal method and dependencies
// - sqlite3_context: UDF context type provided by SQLite
// - dummyUDFvalue: focal function under test
// - assert: runtime check inside dummyUDFvalue (assert(0))
// - VDBE: Virtual Database Engine (conceptual dependency; test ensures the function is not meant to be executed under VDBE)
// - up1: function parameter name (sqlite3_context *up1)
// - abort / SIGABRT: expected abnormal termination path when assert fails

#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <sqlite3expert.h>


// Step 2: Declare the focal function with C linkage for compatibility
extern "C" {
  // Forward declaration of the sqlite3_context type (incomplete is fine for a pointer)
  struct sqlite3_context;
  // Prototype of the focal function under test
  void dummyUDFvalue(struct sqlite3_context *up1);
}

// Test 1: Verify that calling dummyUDFvalue triggers an abort due to assert(0).
// We install a SIGABRT handler and use setjmp/longjmp to catch the abort without terminating the test process.
//
// Rationale: The function intentionally asserts (0); we cannot rely on NDEBUG being undefined in the test binary.
// By intercepting SIGABRT, we can confirm that the assertion path is reached.
static jmp_buf g_jmpbuf;
static volatile sig_atomic_t g_abortCaught = 0;

static void abort_handler(int /*sig*/) {
  // Mark that abort was caught and long jump back to test harness
  g_abortCaught = 1;
  longjmp(g_jmpbuf, 1);
}

bool test_dummyUDFvalue_should_abort_on_call() {
  // Install signal handler for SIGABRT
  struct sigaction old_act, new_act;
  new_act.sa_handler = abort_handler;
  sigemptyset(&new_act.sa_mask);
  new_act.sa_flags = 0;

  if (sigaction(SIGABRT, &new_act, &old_act) != 0) {
    // Cannot install handler; fail the test
    return false;
  }

  g_abortCaught = 0;
  int jumpResult = setjmp(g_jmpbuf);
  if (jumpResult == 0) {
    // First return: call the focal function which is expected to abort (assert(0))
    dummyUDFvalue(nullptr);
    // If we return here, the abort did not occur as expected.
    // Restore previous signal action and report failure.
    sigaction(SIGABRT, &old_act, nullptr);
    return false;
  } else {
    // We returned here because abort was raised and caught by the handler
    sigaction(SIGABRT, &old_act, nullptr);
    // Confirm that an abort was indeed caught
    return g_abortCaught != 0;
  }
}

// Test 2: Compile-time signature compatibility of dummyUDFvalue by assigning to a function pointer type.
// This ensures that changes to the signature (up1 type) would break compilation.
bool test_dummyUDFvalue_signature_matches() {
  // Forward declare a matching type
  using UDFType = void (*)(struct sqlite3_context*);
  // This assignment will fail at compile-time if the signatures do not match.
  extern "C" void dummyUDFvalue(struct sqlite3_context*);
  UDFType f = dummyUDFvalue;
  // Simple runtime check to ensure the pointer is non-null (the function exists)
  return f != nullptr;
}

// Entry point for running tests without any external test framework
int main() {
  int totalTests = 0;
  int passedTests = 0;

  // Test 1
  totalTests++;
  bool t1 = test_dummyUDFvalue_should_abort_on_call();
  if (t1) {
    passedTests++;
  }

  // Test 2
  totalTests++;
  bool t2 = test_dummyUDFvalue_signature_matches();
  if (t2) {
    passedTests++;
  }

  std::cout << "Tests passed: " << passedTests << " / " << totalTests << std::endl;
  if (passedTests != totalTests) {
    std::cerr << "One or more tests failed. See above for details." << std::endl;
    return 1;
  }
  return 0;
}
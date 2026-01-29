/* 
  C++11 test suite for the focal method:
  int SqlitetestSyscall_Init(Tcl_Interp *interp)

  Context:
  - The focal function registers Tcl commands into a Tcl interpreter.
  - It uses Tcl_CreateObjCommand for each entry in its internal aCmd[] table.
  - There is a single command registered: "test_syscall" mapped to the function test_syscall.
  - The test suite purpose is to validate that:
      1) the function returns TCL_OK on a valid interpreter,
      2) the command "test_syscall" is actually registered in the interpreter,
      3) the registered command can be invoked from the interpreter without error.

  Assumptions:
  - The environment provides Tcl (tcl.h) and links against libtcl.
  - The production object file that contains SqlitetestSyscall_Init also defines test_syscall (as a static function); Tcl will call that function when the command is invoked via the interpreter.
  - The test expects TCL_OK to be 0 (as per Tcl convention).

  Notes:
  - This test is written in plain C++11 (no GTest) and uses a lightweight, non-terminating assertion style.
  - The tests use the real Tcl API to reliably exercise the focal method and its integration with Tcl.
  - If the target environment does not provide Tcl, these tests will not compile/run; ensure Tcl is available when building.
  - Build command example (adjust paths as needed):
      g++ -std=c++11 test_syscall_tests.cpp test_syscall.c -ltcl -ltcl8.6 -o test_syscall_tests
  - The test relies on the presence of SqlitetestSyscall_Init and Tcl_CreateObjCommand / Tcl_FindCommand / Tcl_Eval from the Tcl API.

  This file contains explanatory comments for each unit test as requested.
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <tcl.h>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <sys/mman.h>


// Include Tcl headers to access the API used by the focal method.

// The focal function is defined in the production file (test_syscall.c).
// We declare it here to enable calling from C++ test code.
extern "C" int SqlitetestSyscall_Init(Tcl_Interp *interp);

// Lightweight test harness state (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating expectations: record failures but continue execution
#define EXPECT_TRUE(cond, msg) \
  do { \
    ++g_tests_run; \
    if(!(cond)) { \
      std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " | Condition: " #cond << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)
#define EXPECT_EQ(a, b, msg) \
  do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
      std::cerr << "EXPECT_EQ FAILED: " << (msg) << " | " \
                << "left: " << (a) << " vs right: " << (b) << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)


// Helper: create a fresh interpreter and ensure resources are cleaned
static Tcl_Interp* create_interp_or_null() {
  Tcl_Interp* interp = Tcl_CreateInterp();
  if(interp == nullptr) {
    std::cerr << "Failed to create Tcl interpreter." << std::endl;
  }
  return interp;
}

// Test 1: Init should return TCL_OK for a valid interpreter.
// Rationale: Validates basic contract of the focal method.
// Coverage: True path for the predicate (interp != nullptr) and TCL_OK return.
static bool test_SqlitetestSyscall_Init_Returns_OK() {
  // Domain knowledge: TCL_OK is 0 in Tcl API.
  Tcl_Interp* interp = create_interp_or_null();
  if(!interp) return false;

  int rc = SqlitetestSyscall_Init(interp);

  // Non-terminating assertion: verify return code
  EXPECT_EQ(rc, TCL_OK, "SqlitetestSyscall_Init should return TCL_OK on valid interpreter");

  // Cleanup
  Tcl_DeleteInterp(interp);
  return g_tests_failed == 0;
}

// Test 2: After Init, the command "test_syscall" should be registered in the interpreter.
// Rationale: Ensure the core dependency (the command entry) is created by the focal method.
// Coverage: True branch of registration path.
static bool test_SqlitetestSyscall_Init_Registers_TestSyscall_Command() {
  Tcl_Interp* interp = create_interp_or_null();
  if(!interp) return false;

  int rc = SqlitetestSyscall_Init(interp);
  EXPECT_EQ(rc, TCL_OK, "SqlitetestSyscall_Init should return TCL_OK on valid interpreter (for command presence test)");

  // Attempt to locate the command that should have been registered.
  // Tcl_FindCommand returns a non-NULL command handle if the command exists.
  Tcl_Command cmd = Tcl_FindCommand(interp, "test_syscall", 0);
  EXPECT_TRUE(cmd != NULL, "Command 'test_syscall' should be registered by SqlitetestSyscall_Init");

  // Optional: invoke the command via the interpreter to ensure it is callable.
  int eval_rc = Tcl_Eval(interp, "test_syscall");
  EXPECT_EQ(eval_rc, TCL_OK, "Invoked 'test_syscall' should return TCL_OK when called through Tcl_Eval");

  // Cleanup
  Tcl_DeleteInterp(interp);
  return g_tests_failed == 0;
}

// Test 3: Re-running Init on the same interpreter should not crash and should keep TCL_OK.
// Rationale: Validate idempotency or safe re-registration behavior (as per Tcl semantics, duplicates may overwrite).
static bool test_SqlitetestSyscall_Init_Idempotence_On_SameInterp() {
  Tcl_Interp* interp = create_interp_or_null();
  if(!interp) return false;

  int rc1 = SqlitetestSyscall_Init(interp);
  EXPECT_EQ(rc1, TCL_OK, "First Init call should return TCL_OK");

  int rc2 = SqlitetestSyscall_Init(interp);
  EXPECT_EQ(rc2, TCL_OK, "Second Init call should return TCL_OK (no crash)");

  // Still expect the command to exist after a second init
  Tcl_Command cmd = Tcl_FindCommand(interp, "test_syscall", 0);
  EXPECT_TRUE(cmd != NULL, "Command 'test_syscall' must still exist after second Init");

  // Optional: invoke to ensure it's still callable
  int eval_rc = Tcl_Eval(interp, "test_syscall");
  EXPECT_EQ(eval_rc, TCL_OK, "Invoked 'test_syscall' after second Init should return TCL_OK");

  Tcl_DeleteInterp(interp);
  return g_tests_failed == 0;
}

// Test 4: Sanity check that TCL_OK constant is available and consistent.
// Rationale: Quick regression on macro value if TCL_OK were redefined.
// Coverage: True path for macro usage (compile-time rather than runtime).
static bool test_TCL_OK_Consistency() {
  // We expect TCL_OK to be 0 per Tcl conventions.
  EXPECT_EQ(TCL_OK, 0, "TCL_OK should be 0 as per Tcl convention");
  return g_tests_failed == 0;
}

// Entry point for the tests
int main() {
  std::cout << "Starting SqlitetestSyscall_Init unit tests (C++11, non-GTest)" << std::endl;

  bool a = test_SqlitetestSyscall_Init_Returns_OK();
  bool b = test_SqlitetestSyscall_Init_Registers_TestSyscall_Command();
  bool c = test_SqlitetestSyscall_Init_Idempotence_On_SameInterp();
  bool d = test_TCL_OK_Consistency();

  // Summary
  int total = g_tests_run;
  int failed = g_tests_failed;
  std::cout << "Tests run: " << total << ", Failures: " << failed << std::endl;

  // Return nonzero if any test failed
  return failed ? 1 : 0;
}
/*
  Unit test suite for Sqlitetest3_Init in test3.c
  - No Google Test: a lightweight C++11 test harness with simple, non-terminating assertions
  - Tests rely on the real Tcl API (tcl.h) to verify command registration behavior
  - Step-trace alignment with the provided focal method:
    - Sqlitetest3_Init registers 14 btree_* commands via Tcl_CreateCommand and 1 btree_insert via Tcl_CreateObjCommand
    - The tests validate presence of all registered commands after Init
  - Static/Global behavior considerations:
    - Tests treat static scope as internal to the focal file; tests interact only via Tcl interpreter surface
  - This file should compile and link against a Tcl-enabled environment
*/

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <tcl.h>
#include <btreeInt.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


extern "C" {
  // Declare the focal function from the C source file
  int Sqlitetest3_Init(Tcl_Interp *interp);
}


// Global test failure counter to allow non-terminating assertions
static int g_test_failures = 0;

// Lightweight assertion macro: logs failure but does not abort the test
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " [File: " << __FILE__ << ", Line: " << __LINE__ << "]" << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

// Utility: common list of expected command names registered by Sqlitetest3_Init
static const char* kExpectedCmds[] = {
  "btree_open",
  "btree_close",
  "btree_begin_transaction",
  "btree_pager_stats",
  "btree_cursor",
  "btree_close_cursor",
  "btree_next",
  "btree_eof",
  "btree_payload_size",
  "btree_first",
  "btree_varint_test",
  "btree_from_db",
  "btree_ismemdb",
  "btree_set_cache_size",
  // The last one is created via Tcl_CreateObjCommand
  "btree_insert"
};

// Step 2: Test suite generation
// 1) Test that Sqlitetest3_Init registers all expected commands on a Tcl interpreter
static bool test_Sqlitetest3_Init_Registers_All_Commands() {
  // Create a fresh Tcl interpreter
  Tcl_Interp* interp = Tcl_CreateInterp();
  TEST_ASSERT(interp != nullptr, "Failed to create Tcl interpreter");

  // Invoke the focal method
  int rc = Sqlitetest3_Init(interp);
  TEST_ASSERT(rc == TCL_OK, "Sqlitetest3_Init did not return TCL_OK");

  // Verify all expected commands exist in the interpreter
  const size_t cmdCount = sizeof(kExpectedCmds) / sizeof(kExpectedCmds[0]);
  for(size_t i = 0; i < cmdCount; ++i) {
    const char* name = kExpectedCmds[i];
    Tcl_Command cmd = Tcl_FindCommand(interp, name, nullptr);
    std::string failure = std::string("Command not registered: ") + name;
    TEST_ASSERT(cmd != nullptr, failure.c_str());
  }

  // Cleanup the interpreter
  Tcl_DeleteInterp(interp);
  return g_test_failures == 0;
}

// 2) Test that calling Sqlitetest3_Init twice is safe (idempotent for registration surface)
static bool test_Sqlitetest3_Init_Idempotent() {
  Tcl_Interp* interp = Tcl_CreateInterp();
  TEST_ASSERT(interp != nullptr, "Failed to create Tcl interpreter for idempotence test");

  // First initialization
  int rc1 = Sqlitetest3_Init(interp);
  TEST_ASSERT(rc1 == TCL_OK, "First Sqlitetest3_Init call failed");

  // Second initialization
  int rc2 = Sqlitetest3_Init(interp);
  TEST_ASSERT(rc2 == TCL_OK, "Second Sqlitetest3_Init call failed");

  // Re-verify all commands exist after second initialization
  for(size_t i = 0; i < sizeof(kExpectedCmds)/sizeof(kExpectedCmds[0]); ++i) {
    const char* name = kExpectedCmds[i];
    Tcl_Command cmd = Tcl_FindCommand(interp, name, nullptr);
    std::string failure = std::string("Command missing after re-init: ") + name;
    TEST_ASSERT(cmd != nullptr, failure.c_str());
  }

  Tcl_DeleteInterp(interp);
  return g_test_failures == 0;
}

// Step 3: Test case refinement (compile-time driven by domain knowledge)
// - The tests focus on surface behavior accessible via Tcl API
// - They do not depend on private/static internals of the focal file
// - Additional checks could be added (e.g., evaluating commands), but command callbacks are stubs

int main() {
  // Run tests in a non-terminating fashion; aggregate failures
  test_Sqlitetest3_Init_Registers_All_Commands();
  test_Sqlitetest3_Init_Idempotent();

  if (g_test_failures != 0) {
    std::cerr << g_test_failures << " test(s) failed." << std::endl;
    return 1;
  }

  std::cout << "All tests passed." << std::endl;
  return 0;
}
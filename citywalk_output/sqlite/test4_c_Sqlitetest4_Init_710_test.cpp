// File: tcl.h
#ifndef TCL_H
#define TCL_H

#ifdef __cplusplus
extern "C" {
#endif

// Opaque Tcl interpreter type used by the focal code
typedef struct Tcl_Interp Tcl_Interp;

// Tcl_CmdProc signature used by the focal code (as a function pointer type)
typedef int Tcl_CmdProc(void *NotUsed, Tcl_Interp *interp, int argc, const char **argv);

#define TCL_OK 0

// Function used to register TCL commands within Sqlitetest4_Init
int Tcl_CreateCommand(Tcl_Interp *interp, const char *zName, Tcl_CmdProc *proc, void *clientData, void *deleteProc);

#ifdef __cplusplus
}
#endif

#endif // TCL_H



// File: tcl_fake.cpp
// This file provides a minimal fake Tcl environment to which the focal code under test can register commands.
// It is designed to work alongside test4_init_true.cpp and test4_init_false.cpp, simulating
// the behavior of Tcl_CreateCommand without pulling in the real Tcl library.

#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <tcl.h>
#include <string>
#include <stdio.h>
#include <sqliteInt.h>
#include <iostream>
#include <pthread.h>


// Keep command data in a simple shared container accessible across translation units.
static std::vector<std::string> g_cmdNames;

// Minimal implementation of Tcl_CreateCommand that records the command name.
// The real PTR 'proc' is ignored for the purposes of this test harness.
extern "C" int Tcl_CreateCommand(Tcl_Interp *interp,
                               const char *zName,
                               Tcl_CmdProc *proc,
                               void *clientData,
                               void *deleteProc)
{
  (void)interp; (void)clientData; (void)deleteProc;
  if (zName) {
    g_cmdNames.push_back(std::string(zName));
  }
  // In a real Tcl port, this would return TCL_OK on success or an error code.
  return 0;
}

// Reset the internal registry of commands (used between tests)
extern "C" void tcl_reset_commands()
{
  g_cmdNames.clear();
}

// Query helpers for tests
extern "C" int tcl_get_command_count()
{
  return static_cast<int>(g_cmdNames.size());
}

extern "C" const char* tcl_get_command_name(int index)
{
  if (index < 0 || index >= static_cast<int>(g_cmdNames.size())) return nullptr;
  return g_cmdNames[index].c_str();
}



// File: test4_init_true.cpp
// Test: Sqlitetest4_Init registers all 15 thread-related commands when compiled with UNIX + THREADSAFE enabled.
// The test uses a lightweight, non-terminating assertion style to maximize coverage.


extern "C" int Sqlitetest4_Init(Tcl_Interp *interp);

// Simple non-terminating assertion helpers
static int g_pass = 0;
static int g_fail = 0;
#define EXPECT_TRUE(cond) do { \
  if (!(cond)) { \
    fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_EQ failed: %s:%d: %d != %d\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while(0)

static void run_true_branch_test()
{
  // Reset command registry before test
  tcl_reset_commands();

  // Call the focal function under the UNIX+THREADSAFE path
  int rc = Sqlitetest4_Init(nullptr);

  // Expect TCL_OK (0)
  EXPECT_EQ(rc, TCL_OK);

  // Expect 15 commands registered in the exact order defined by aCmd[]
  int cnt = tcl_get_command_count();
  EXPECT_EQ(cnt, 15);

  const char* expected_names[15] = {
    "thread_create", "thread_wait", "thread_halt", "thread_argc", "thread_argv",
    "thread_colname", "thread_result", "thread_error", "thread_compile", "thread_step",
    "thread_finalize", "thread_swap", "thread_db_get", "thread_db_put", "thread_stmt_get"
  };

  for (int i = 0; i < 15; ++i) {
    const char* actual = tcl_get_command_name(i);
    EXPECT_TRUE(actual != nullptr);
    EXPECT_TRUE(std::string(actual) == std::string(expected_names[i]));
  }

  // Summary
  if (g_fail == 0) {
    printf("TEST PASS: test4_init_true_branch\n");
  } else {
    printf("TEST FAIL: test4_init_true_branch - passed=%d failed=%d\n", g_pass, g_fail);
  }
}

int main()
{
  run_true_branch_test();
  // Return 0 if all tests passed; non-zero otherwise.
  if (g_fail == 0) {
    printf("ALL TESTS PASSED (true branch).\n");
    return 0;
  } else {
    printf("SOME TESTS FAILED (true branch).\n");
    return 1;
  }
}



// File: test4_init_false.cpp
// Test: Sqlitetest4_Init returns TCL_OK when compiled without UNIX+THREADSAFE (i.e., else branch).
// No commands should be registered in this mode.


extern "C" int Sqlitetest4_Init(Tcl_Interp *interp);

// Minimal assertions for the else branch
static int g_pass = 0;
static int g_fail = 0;
#define EXPECT_TRUE(cond) do { \
  if (!(cond)) { \
    fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_EQ failed: %s:%d: %d != %d\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while(0)

static void run_false_branch_test()
{
  // Reset command registry before test
  // The else branch should not register any commands
  // If needed, the tcl_fake helper can reset global state; here we simulate by calling reset.
  // Note: If the test environment reuses the same process, ensure registry is empty.
  extern void tcl_reset_commands();
  tcl_reset_commands();

  int rc = Sqlitetest4_Init(nullptr);

  // The else branch should simply return TCL_OK
  EXPECT_EQ(rc, TCL_OK);

  // No commands should be registered in this mode
  int cnt = 0;
  // We rely on the helper to report current command count safely
  extern int tcl_get_command_count();
  cnt = tcl_get_command_count();
  EXPECT_EQ(cnt, 0);

  if (g_fail == 0) {
    printf("TEST PASS: test4_init_false_branch\n");
  } else {
    printf("TEST FAIL: test4_init_false_branch - passed=%d failed=%d\n", g_pass, g_fail);
  }
}

int main()
{
  run_false_branch_test();
  if (g_fail == 0) {
    printf("ALL TESTS PASSED (false branch).\n");
    return 0;
  } else {
    printf("SOME TESTS FAILED (false branch).\n");
    return 1;
  }
}
// C++11 test suite for the focal method: find_val in pngcp.c
// Notes:
// - This test suite targets the behavior of find_val by exercising its abort-on-unknown-value path.
// - We avoid GTest and implement a lightweight test harness and signal-based abort capture.
// - The test relies on the actual library's find_val implementation being linked in (C linkage).
// - A minimal struct display layout is declared here to satisfy the function signature during the call.
// - Since internal options structure is part of the library, this test focuses on the unknown-value path
//   which should trigger abort() in find_val when no matching option is found.
//
// Important: This test is designed to be compiled in a C++11 project that also builds pngcp.c or its
// equivalent into the test binary, exposing find_val with C linkage.

#include <cstdint>
#include <pnglibconf.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>


// Ensure C linkage for the focal function
extern "C" {
  // The actual signature from the focal method
  // png_byte is typically an unsigned char
  typedef unsigned char png_byte;
  struct display; // incomplete type to declare function prototype safely
  // External function under test (defined in pngcp.c)
  int find_val(struct display *dp, png_byte opt, const char *str, size_t len);
}

// Provide a minimal compatible layout for struct display to allow compilation.
// The real layout is defined in the pngcp.c; we only require the fields used by find_val.
struct display {
  int errset;
  int entry[32];
};

// Global jump buffer for catching abort() via longjmp from the signal handler
static jmp_buf g_abort_jmp;
static volatile sig_atomic_t g_aborted = 0;

// Signal handler to catch abort() and return control to test harness
static void abort_signal_handler(int /*sig*/)
{
  // Long jump back to the test harness
  longjmp(g_abort_jmp, 1);
}

/////////////////////////////////////////////////////////////////
// Test 1: Unknown value should trigger abort() path in find_val
// Rationale: If the provided string does not match any option name and no range
// is applicable, the function calls display_log(...) and abort(), which we catch.
/////////////////////////////////////////////////////////////////
bool test_unknown_value_aborts()
{
  // Prepare a fresh dp structure
  display dp;
  dp.errset = 0;
  for (size_t i = 0; i < 32; ++i)
    dp.entry[i] = 0;

  // Install signal handler to catch abort()
  struct sigaction old_act {};
  struct sigaction new_act {};
  new_act.sa_handler = abort_signal_handler;
  sigemptyset(&new_act.sa_mask);
  new_act.sa_flags = 0;
  if (sigaction(SIGABRT, &new_act, &old_act) != 0) {
    perror("sigaction");
    return false;
  }

  bool passed = false;
  if (setjmp(g_abort_jmp) == 0) {
    // Invoke the focal function with a string that is highly unlikely to be an exact match
    // and with an option index that would rely on internal options; the unknown-value path
    // should trigger abort() for unknown settings.
    const char *unknown = "this_value_is_not_known_by_pngcp";
    size_t len = std::strlen(unknown);
    int ret = find_val(&dp, 0, unknown, len);

    // If we reach here, abort() did not occur as expected.
    // This indicates the function did not abort for an unknown value, which is a failure.
    fprintf(stderr, "test_unknown_value_aborts: FAILED - find_val returned %d without abort\n", ret);
    passed = false;
  } else {
    // We landed here via longjmp from abort_signal_handler, meaning abort() occurred as expected.
    fprintf(stderr, "test_unknown_value_aborts: PASSED - abort() occurred as expected for unknown value\n");
    passed = true;
  }

  // Restore previous signal handler
  sigaction(SIGABRT, &old_act, nullptr);

  return passed;
}

/////////////////////////////////////////////////////////////////
// Test runner
/////////////////////////////////////////////////////////////////
int main(void)
{
  int total = 0;
  int ok = 0;

  // Run Test 1
  fprintf(stdout, "Running Test 1: Unknown value aborts path\n");
  ++total;
  if (test_unknown_value_aborts()) {
    ++ok;
  }

  // Summary
  fprintf(stdout, "Test results: %d/%d tests passed.\n", ok, total);
  return (ok == total) ? 0 : 1;
}
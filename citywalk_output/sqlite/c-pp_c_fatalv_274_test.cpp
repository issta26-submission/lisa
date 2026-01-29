// C++11 unit test suite for the focal method fatalv defined in c-pp.c
// The tests are designed to run without Google Test. A lightweight test harness
// is implemented that uses setjmp/longjmp to intercept the program exit path
// and to redirect stderr to a temporary file for capturing output.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/types.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstdarg>
#include <stdarg.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <cstdlib>


// Forward declarations of the focal C function and a small wrapper to pass va_list.
// The focal method resides in c-pp.c and uses C linkage.
extern "C" void fatalv(char const *zFmt, va_list va);
extern "C" void fatalv_with_va(char const *zFmt, ...); // variadic wrapper declared for testing

// Global jump buffer and exit code to intercept exit(1) calls without terminating tests.
static jmp_buf g_jmp_buf;
static int g_exit_code = 0;

// Override the standard C exit to intercept fatalv's exit(1) call.
// We perform a long jump back to the test harness to inspect internal state
// without terminating the test process.
extern "C" void exit(int status) {
  g_exit_code = status;
  longjmp(g_jmp_buf, 1);
}

// Utility: read entire contents of a file into a string
static std::string read_file(int fd) {
  std::string out;
  char buf[4096];
  ssize_t r;
  // Seek to start to ensure deterministic reads
  lseek(fd, 0, SEEK_SET);
  while ((r = read(fd, buf, sizeof(buf))) > 0) {
    out.append(buf, static_cast<size_t>(r));
  }
  return out;
}

// Utility: Run a test that redirects stderr to a temporary file and captures its content.
// Returns pair<contents, exit_code> captured after fatalv_with_va triggers exit(1).
struct CaptureResult {
  std::string output;
  int exit_code;
  int tmp_fd;      // file descriptor for the temp file
  char tmp_path[256]; // path to temp file
  CaptureResult() : output(""), exit_code(-1), tmp_fd(-1) { tmp_path[0] = '\0'; }
};

// Prototypes for test-specific helpers (defined below)
static CaptureResult run_fatalv_null_format();
static CaptureResult run_fatalv_with_int_format();
static CaptureResult run_fatalv_empty_format();

// Implementations of the tests (explanatory comments included)

// Test 1: zFmt is NULL. Expect no formatted output, only a newline, and exit code 1.
static CaptureResult run_fatalv_null_format() {
  CaptureResult cr;

  // Create a temporary file to capture stderr output
  strcpy(cr.tmp_path, "/tmp/fatalv_test_nullXXXXXX");
  cr.tmp_fd = mkstemp(cr.tmp_path);
  if (cr.tmp_fd == -1) {
    // If temp file creation fails, mark as failure by returning empty output
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  // Redirect stderr (fd 2) to the temporary file
  int saved_stderr = dup(2);
  if (dup2(cr.tmp_fd, 2) == -1) {
    // If redirection fails, mark as failure
    close(cr.tmp_fd);
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  // Prepare jump buffer and call the wrapper with NULL format
  g_exit_code = -1;
  if (setjmp(g_jmp_buf) == 0) {
    // First time: invoke the focal function via the wrapper
    fatalv_with_va(nullptr);
  } else {
    // Returned via longjmp from exit interception
    // g_exit_code should be 1
  }

  // Restore stderr and close the temporary file descriptor
  dup2(saved_stderr, 2);
  close(saved_stderr);
  // Read captured data from the temp file
  cr.output = read_file(cr.tmp_fd);
  cr.exit_code = g_exit_code;

  // Cleanup: close and unlink temp file
  close(cr.tmp_fd);
  unlink(cr.tmp_path);

  return cr;
}

// Test 2: zFmt is "%d" and a value (42) is supplied via va_list wrapper.
// Expect "42\n" in the captured output and exit code 1.
static CaptureResult run_fatalv_with_int_format() {
  CaptureResult cr;

  strcpy(cr.tmp_path, "/tmp/fatalv_test_intXXXXXX");
  cr.tmp_fd = mkstemp(cr.tmp_path);
  if (cr.tmp_fd == -1) {
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  int saved_stderr = dup(2);
  if (dup2(cr.tmp_fd, 2) == -1) {
    close(cr.tmp_fd);
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  g_exit_code = -1;
  if (setjmp(g_jmp_buf) == 0) {
    // Call wrapper that accepts variadic arguments and forwards to fatalv
    fatalv_with_va("%d", 42);
  } else {
    // Returned via longjmp from exit interception
  }

  dup2(saved_stderr, 2);
  close(saved_stderr);

  cr.output = read_file(cr.tmp_fd);
  cr.exit_code = g_exit_code;

  close(cr.tmp_fd);
  unlink(cr.tmp_path);

  return cr;
}

// Test 3: zFmt is an empty string. Expect no formatted output, only a newline, and exit code 1.
static CaptureResult run_fatalv_empty_format() {
  CaptureResult cr;

  strcpy(cr.tmp_path, "/tmp/fatalv_test_emptyXXXXXX");
  cr.tmp_fd = mkstemp(cr.tmp_path);
  if (cr.tmp_fd == -1) {
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  int saved_stderr = dup(2);
  if (dup2(cr.tmp_fd, 2) == -1) {
    close(cr.tmp_fd);
    cr.output = "";
    cr.exit_code = -1;
    return cr;
  }

  g_exit_code = -1;
  if (setjmp(g_jmp_buf) == 0) {
    fatalv_with_va(""); // non-null but empty string
  } else {
    // Returned via longjmp
  }

  dup2(saved_stderr, 2);
  close(saved_stderr);

  cr.output = read_file(cr.tmp_fd);
  cr.exit_code = g_exit_code;

  close(cr.tmp_fd);
  unlink(cr.tmp_path);

  return cr;
}

// Lightweight test harness helpers
static void report(const char* name, bool ok) {
  if (ok) {
    printf("[PASS] %s\n", name);
  } else {
    printf("[FAIL] %s\n", name);
  }
}

// Main entry: run all tests in sequence and report results
int main() {
  // Test 1: NULL format
  CaptureResult r1 = run_fatalv_null_format();
  bool ok1 = (r1.exit_code == 1) && (r1.output == "\n");
  report("fatalv(NULL) prints newline only and exits with code 1", ok1);

  // Test 2: "%d" with value 42
  CaptureResult r2 = run_fatalv_with_int_format();
  bool ok2 = (r2.exit_code == 1) && (r2.output == "42\n");
  report("fatalv(\"%d\", 42) prints '42' and newline and exits with code 1", ok2);

  // Test 3: Empty string format
  CaptureResult r3 = run_fatalv_empty_format();
  bool ok3 = (r3.exit_code == 1) && (r3.output == "\n");
  report("fatalv(\"\") prints newline only and exits with code 1", ok3);

  // Final result
  int failures = 0;
  // Simple counting based on individual test outcomes
  if (!ok1) ++failures;
  if (!ok2) ++failures;
  if (!ok3) ++failures;

  if (failures == 0) {
    printf("All tests passed.\n");
  } else {
    printf("%d test(s) failed.\n", failures);
  }

  return failures;
}

// Implementations of the variadic wrapper for testing
extern "C" void fatalv_with_va(char const *zFmt, ...) {
  va_list va;
  va_start(va, zFmt);
  fatalv(zFmt, va);
  va_end(va);
}
/*
 * C++11 test suite for the focal method:
 *   int sqlite3_quota_ferror(quota_FILE *p)
 * 
 * Focal method purpose:
 * - Bridge to the C library ferror by returning ferror(p->f).
 * 
 * Notes:
 * - We rely on the existing C type quota_FILE and its member FILE *f.
 * - The tests are written without GoogleTest. A lightweight self-contained test
 *   harness is provided that reports failures and continues execution to improve
 *   coverage.
 * - POSIX pipe-based scenario is used to simulate a non-zero error flag on the
 *   FILE* to exercise the non-zero return path of ferror(p->f).
 *
 * Candidate Keywords (Step 1):
 * - sqlite3_quota_ferror
 * - quota_FILE
 * - ferror
 * - FILE*
 * - POSIX pipe (pipe, fdopen, fflush)
 * - tmpfile
 * - test_quota.h (structure definitions)
 *
 * The tests assume a C linkage contract for sqlite3_quota_ferror; hence we place
 * an extern "C" declaration for the function.
 */

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cerrno>
#include <signal.h>
#include <cstring>
#include <unistd.h>
#include <test_quota.h>
#include <os_setup.h>
#include <cstdlib>


extern "C" {
  // Forward declaration to call from C++ test code.
  // quota_FILE is defined in test_quota.h as used by the focal code.
  typedef struct quota_FILE quota_FILE;
  int sqlite3_quota_ferror(quota_FILE *p);
}

// Include the project header that defines quota_FILE and related types.
// It is safe to include in C++ as it originates from the C codebase.

static int g_failures = 0;
#define TEST_CHECK(cond, msg) do { \
  if(!(cond)){ \
    fprintf(stderr, "TEST FAILURE: %s\n", msg); \
    ++g_failures; \
  } \
} while(0)

/*
 * Test 1: ferror should return 0 when there is no error on p->f.
 * Approach: use tmpfile() to obtain a FILE* with no error yet.
 */
static int test_ferror_no_error(void)
{
  FILE *tmp = tmpfile();
  if(!tmp){
    fprintf(stderr, "Unable to create temporary file for test_ferror_no_error.\n");
    return 1;
  }

  quota_FILE q;
  // Initialize quota_FILE with zeroed contents to be safe.
  memset(&q, 0, sizeof(q));
  q.f = tmp;

  int res = sqlite3_quota_ferror(&q);

  // Expect 0 since there is no error on the FILE*
  TEST_CHECK(res == 0, "sqlite3_quota_ferror should return 0 when no error on FILE*");
  fclose(tmp);
  return g_failures ? 1 : 0;
}

/*
 * Test 2: ferror should reflect a non-zero error state after a write to a pipe
 * with no reader (EPIPE). This exercises the underlying ferror machinery.
 * This test is POSIX-specific and skipped on Windows.
 */
static int test_ferror_pipe_error(void)
{
#if defined(_WIN32) || defined(_WIN64)
  // Not portable on Windows; skip this test.
  fprintf(stderr, "Skipping test_ferror_pipe_error on Windows.\n");
  return 0;
#else
  // Ignore SIGPIPE to prevent the test process from terminating on write.
  struct sigaction old_sa, ignore_sa;
  memset(&ignore_sa, 0, sizeof(ignore_sa));
  ignore_sa.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &ignore_sa, &old_sa);

  int pipefds[2] = { -1, -1 };
  if(pipe(pipefds) != 0){
    fprintf(stderr, "pipe() failed in test_ferror_pipe_error: %s\n", strerror(errno));
    // Restore previous signal handler
    sigaction(SIGPIPE, &old_sa, NULL);
    return 1;
  }

  // We will write to the write-end without a reader on the other end.
  FILE *wfile = fdopen(pipefds[1], "w");
  if(!wfile){
    fprintf(stderr, "fdopen failed in test_ferror_pipe_error: %s\n", strerror(errno));
    close(pipefds[0]);
    close(pipefds[1]);
    // Restore previous signal handler
    sigaction(SIGPIPE, &old_sa, NULL);
    return 1;
  }

  // Close the read end to ensure EPIPE on write attempts.
  close(pipefds[0]);

  quota_FILE q;
  memset(&q, 0, sizeof(q));
  q.f = wfile;

  // Attempt a write to force an error on the pipe; flush to ensure error state propagates.
  const char *payload = "test";
  // Use printf-style to ensure buffering path is exercised
  fprintf(q.f, "%s", payload);
  fflush(q.f);

  int res = sqlite3_quota_ferror(&q);

  // We expect a non-zero value due to EPIPE on the pipe with no reader.
  TEST_CHECK(res != 0, "sqlite3_quota_ferror should reflect a non-zero error after a write error on FILE*");
  // Cleanup
  fclose(q.f);
  // Restore previous signal handler
  sigaction(SIGPIPE, &old_sa, NULL);

  // The test should not leak descriptors
  return g_failures ? 1 : 0;
#endif
}

/*
 * Main test harness:
 * - Runs test_ferror_no_error and test_ferror_pipe_error (when applicable).
 * - Reports final result and returns non-zero if any test failed.
 */
int main(void)
{
  // Ensure we ignore SIGPIPE during tests that intentionally cause it.
  struct sigaction sa, old_sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = SIG_IGN;
  if(sigaction(SIGPIPE, &sa, &old_sa) != 0){
    // Non-fatal; just continue
  }

  int failed = 0;
  // Run tests
  fprintf(stdout, "Running test_ferror_no_error...\n");
  failed += test_ferror_no_error();

  fprintf(stdout, "Running test_ferror_pipe_error (POSIX only)...\n");
  failed += test_ferror_pipe_error();

  if(sigaction(SIGPIPE, &old_sa, NULL) != 0){
    // Non-fatal
  }

  if(failed == 0){
    fprintf(stdout, "ALL TESTS PASSED\n");
  }else{
    fprintf(stderr, "TOTAL FAILED TESTS: %d\n", failed);
  }

  return failed;
}
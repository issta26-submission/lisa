// Unit test suite for sqlite3PageMalloc in a single translation unit.
// This test avoids GoogleTest and uses a lightweight custom harness
// with non-terminating CHECK-style assertions to maximize code coverage.

#include <sys/types.h>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// Global to capture the last size passed to pcache1Alloc for verification
static int g_last_alloc_size = -1;

// Lightweight mock of the dependency: pcache1Alloc.
// We implement it as a simple allocator wrapper around malloc to test
// that sqlite3PageMalloc forwards the requested size correctly.
static void *pcache1Alloc(int nByte){
  g_last_alloc_size = nByte;
  return std::malloc((size_t)nByte);
}

// Focal method under test copied from the provided snippet.
void *sqlite3PageMalloc(int sz){
  assert( sz<=65536+8 ); /* These allocations are never very large */
  return pcache1Alloc(sz);
}

// Simple test harness: non-terminating checks with per-test logs.
// We accumulate failures in a global counter and print diagnostics.

static int g_failures = 0;
#define CHECK(cond, msg) do { \
  if(!(cond)) { \
    fprintf(stderr, "CHECK FAILED: %s\n", msg); \
    ++g_failures; \
  } \
} while(0)

// Test 1: Basic allocation path within allowed size.
// Verifies that sqlite3PageMalloc forwards the correct size to pcache1Alloc
// and returns a non-null pointer.
static void test_basic_allocation() {
  // Reset recorder
  g_last_alloc_size = -1;
  void *p = sqlite3PageMalloc(4);
  CHECK(p != NULL, "sqlite3PageMalloc(4) returned NULL");
  CHECK(g_last_alloc_size == 4, "pcache1Alloc received incorrect size for 4");
  if (p) {
    std::free(p);
  }
}

// Test 2: Allocation at the edge of allowed maximum (65536+8).
// Verifies correct forwarding of the edge size.
static void test_edge_allocation() {
  g_last_alloc_size = -1;
  void *p = sqlite3PageMalloc(65544); // 65536 + 8
  CHECK(p != NULL, "sqlite3PageMalloc(65544) returned NULL");
  CHECK(g_last_alloc_size == 65544, "pcache1Alloc received incorrect size for edge case 65544");
  if (p) {
    std::free(p);
  }
}

// Test 3: Predicate false branch (assertion) should trigger when size is too large.
// Since the assert would abort the process, we run this test in a separate process (fork).
// The test passes if the child aborts due to the assertion.
static void test_assert_oversize_via_fork() {
#ifdef __unix__
  pid_t pid = fork();
  if (pid == -1) {
    // Fork failed; mark as a skip since we can't reliably test here
    fprintf(stderr, "SKIP: fork() failed, cannot test assertion path reliably\n");
    return;
  }
  if (pid == 0) {
    // Child: attempt the oversize allocation which should trigger assert
    sqlite3PageMalloc(70000); // greater than 65536 + 8
    // If we reach here, the assertion did not abort as expected.
    _exit(0);
  } else {
    int status = 0;
    waitpid(pid, &status, 0);
    // Child should be terminated by SIGABRT due to assertion
    int aborted = WIFSIGNALED(status) && (WTERMSIG(status) == SIGABRT);
    CHECK(aborted, "Child process did not abort (SIGABRT) on oversize allocation");
  }
#else
  // If not POSIX-compatible, we cannot reliably test assertion aborts.
  fprintf(stderr, "SKIP: Non-POSIX environment; assertion path not testable here\n");
#endif
}

int main() {
  // Run tests
  test_basic_allocation();
  test_edge_allocation();
  test_assert_oversize_via_fork();

  // Summary
  if (g_failures == 0) {
    printf("All tests passed (%d tests executed).\n", 2 + 1 /* edge + basic + assertion test (fork) */);
  } else {
    printf("Tests completed with %d failure(s).\n", g_failures);
  }
  return g_failures;
}
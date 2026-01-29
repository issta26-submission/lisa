// C++11 unit test suite for sqlite3MemTraceActivate without Google Test.
// This test suite is designed to be linked with the provided C source (memtrace.c)
// and exercises the behavior of sqlite3MemTraceActivate via a small C mock of sqlite3_config.
// It uses only the C++ standard library and simple print-based checks (non-terminating assertions).

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge assumptions (kept minimal and focused):
// - We need to mock sqlite3_config to influence the behavior of sqlite3MemTraceActivate.
// - The focal function uses a static memtraceBase with an xMalloc field.
// - We will distinguish the two configuration calls (GETMALLOC and MALLOC) by
//   pointer identity: the first call receives a pointer to memtraceBase, the second
//   receives a pointer to ersaztMethods. This matches the existing C code flow.
// - We expose a C-compatible sqlite3_mem_methods structure to be filled by sqlite3_config.
// - We do not rely on private members; we only observe side effects via counters.

extern "C" {
  // Mirror of the needed portion of sqlite3_mem_methods used by memtrace.c
  typedef struct sqlite3_mem_methods {
    void *(*xMalloc)(int);
    void (*xFree)(void*);
    void *(*xRealloc)(void*, int);
    int (*xSize)(void*);
    int (*xRoundup)(int);
    int (*xInit)(void*);
    void (*xShutdown)(void*);
  } sqlite3_mem_methods;

  // Function under test is provided by memtrace.c in the project under test.
  // We declare it here so the test code can invoke it.
  int sqlite3MemTraceActivate(FILE *out);
  // sqlite3_config is provided by memtrace.c during linkage (we override with a test shim).
  int sqlite3_config(int op, void *p);
}

// Simple non-terminating assertion helpers
static int gFailures = 0;
#define CHECK(cond, msg) \
  do { if(!(cond)){ std::cerr << "ASSERT_FAIL: " << (msg) << std::endl; ++gFailures; } } while(0)

// Global per-test counters to observe how many times the test shim was invoked
// as if counting GETMALLOC and MALLOC branches.
static int g_getmalloc_calls = 0;
static int g_malloc_calls = 0;

// Sentinel to differentiate first vs second sqlite3_config call by pointer identity.
// The test uses the first pointer it ever sees as the "GETMALLOC" target, the
// second distinct pointer as the "MALLOC" target.
static void* g_firstConfigPtr = nullptr;

// A simple malloc-based dummy allocator used by the mocked config to fill pointers.
static void *g_dummyMalloc(int n){
  return malloc(n);
}

// Mock shim for sqlite3_config using C linkage to override the project-provided symbol.
extern "C" int sqlite3_config(int op, void *p) {
  // Identify the first and second targets by pointer identity.
  if (g_firstConfigPtr == nullptr) {
    g_firstConfigPtr = p;
  }

  // Distinguish first vs. subsequent calls by pointer address.
  if (p == g_firstConfigPtr) {
    // Treat this as the "GETMALLOC" configuration call.
    sqlite3_mem_methods *m = (sqlite3_mem_methods*)p;
    if (m != nullptr) {
      m->xMalloc = (void *(*)(int))g_dummyMalloc;
      ++g_getmalloc_calls;
      return 0; // SQLITE_OK
    }
    return -1;
  } else {
    // Treat this as the "MALLOC" configuration call.
    sqlite3_mem_methods *m = (sqlite3_mem_methods*)p;
    if (m != nullptr) {
      m->xMalloc = (void *(*)(int))g_dummyMalloc;
      ++g_malloc_calls;
      return 0; // SQLITE_OK
    }
    return -1;
  }
}

// Test 1: Basic activation when memtraceBase.xMalloc is 0.
// Expected: rc == 0, GETMALLOC called exactly once, MALLOC called exactly once.
bool test_activate_basic_success() {
  // Reset per-test counters to ensure clean slate for this test.
  g_getmalloc_calls = 0;
  g_malloc_calls = 0;
  g_firstConfigPtr = nullptr;

  // Use stdout as the output stream for sqlite3MemTraceActivate.
  FILE *out = stdout;

  int rc = sqlite3MemTraceActivate(out);

  bool ok = (rc == 0);
  ok = ok && (g_getmalloc_calls == 1);
  ok = ok && (g_malloc_calls == 1);

  if(!ok){
    std::cerr << "test_activate_basic_success FAILED: rc=" << rc
              << " getmalloc_calls=" << g_getmalloc_calls
              << " malloc_calls=" << g_malloc_calls << std::endl;
  } else {
    std::cout << "test_activate_basic_success PASSED" << std::endl;
  }
  return ok;
}

// Test 2: Activation after initial success should skip reconfiguration.
// This exercises the false branch of the top-level condition (memtraceBase.xMalloc != 0).
// Expected: rc == 0 and no additional calls to sqlite3_config (counters unchanged).
bool test_activate_skip_when_already_configured() {
  // Do not reset g_firstConfigPtr here; we want subsequent call to be treated as the same
  // configuration scenario (memtraceBase.xMalloc should now be non-zero, causing skip).
  FILE *out = stdout;

  int rc = sqlite3MemTraceActivate(out);

  bool ok = (rc == 0);
  // Expect no new GETMALLOC or MALLOC calls since memtraceBase.xMalloc is non-zero.
  // The exact numbers depend on Test 1 having executed previously; we check stability.
  if (g_getmalloc_calls != 1 || g_malloc_calls != 1) {
    std::cerr << "test_activate_skip_when_already_configured FAILED: rc=" << rc
              << " getmalloc_calls=" << g_getmalloc_calls
              << " malloc_calls=" << g_malloc_calls << std::endl;
    ok = false;
  }

  if(ok){
    std::cout << "test_activate_skip_when_already_configured PASSED" << std::endl;
  }
  return ok;
}

// Main runs the tests in a deterministic order to maximize coverage given the
// constraints of static variables in the C module under test.
int main() {
  std::cout << "Starting sqlite3MemTraceActivate unit tests (no GTest)..." << std::endl;

  bool t1 = test_activate_basic_success();
  bool t2 = test_activate_skip_when_already_configured();

  int failures = 0;
  if(!t1) ++failures;
  if(!t2) ++failures;

  if(failures == 0){
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cerr << failures << " test(s) FAILED" << std::endl;
  }

  return failures;
}
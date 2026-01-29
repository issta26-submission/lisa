// Test suite for the focal method lsm_csr_prev in lsm_main.c
// This test uses a linker wrap trick to intercept the call to lsmMCursorPrev.
// Build note (important): Compile and link with the wrap option to redirect
// calls from lsmMCursorPrev to the wrapper defined below.
// Example (gcc/clang): 
//   g++ -std=c++11 -c test_lsm_csr_prev.cpp
//   g++ -std=c++11 test_lsm_csr_prev.o <path_to_lsm_object_or_library> -Wl,--wrap=lsmMCursorPrev -o test_lsm_csr_prev
// The library/object containing lsm_main.c must be linked as well.
// The wrap mechanism requires the following wrapper function to be defined in this file:
//   extern "C" int __wrap_lsmMCursorPrev(MultiCursor* p);
// The wrapper records the input pointer and returns a controlled value set in tests.

#include <lsmInt.h>
#include <iostream>
#include <cstdint>


// Forward declarations matching the C API used by lsm_main.c
extern "C" {
  // Opaque types from the C library
  struct lsm_cursor;
  typedef struct lsm_cursor lsm_cursor;

  struct MultiCursor;
  typedef struct MultiCursor MultiCursor;

  // Function under test (from lsm_main.c)
  int lsm_csr_prev(lsm_cursor *pCsr);
}

// Lightweight domain testing helpers (non-terminating assertions)
static int g_test_failures = 0;

static void log_failure(const char* msg) {
  std::cerr << "Test Failure: " << msg << std::endl;
  ++g_test_failures;
}

#define CHECK(cond, msg) do { \
  if(!(cond)) { log_failure(msg); } \
} while(0)

// Global state used by the wrapper to verify forwarding behavior
static int g_wrap_ret = 0;          // Return value that the wrapper will produce
static MultiCursor* g_last_ptr = nullptr; // Pointer captured by the wrapper

// Wrapper for lsmMCursorPrev to intercept calls.
// This replaces the real function during tests when linked with -Wl,--wrap=lsmMCursorPrev
extern "C" int __wrap_lsmMCursorPrev(MultiCursor* p) {
  // Record the pointer argument for verification
  g_last_ptr = p;
  // Return the test-controlled value
  return g_wrap_ret;
}

// A minimal dummy struct to create a valid pointer for the test.
// The internal layout is irrelevant since lsmMCursorPrev is invoked via the wrapper.
struct DummyCursor {
  int dummy;
};

// Test 1: Verify that lsm_csr_prev forwards the return value from lsmMCursorPrev
// and forwards the same pointer passed (cast to MultiCursor*).
void test_lsm_csr_prev_forwards_wrapped_call() {
  // Prepare test scenario
  g_wrap_ret = 123;             // The value the wrapper will return
  g_last_ptr = nullptr;          // Clear previous pointer capture

  DummyCursor dummy;
  lsm_cursor* pCsr = reinterpret_cast<lsm_cursor*>(&dummy);

  // Invoke the function under test
  int rc = lsm_csr_prev(pCsr);

  // Assertions (non-terminating): ensure both return value and forwarded pointer are correct
  CHECK(rc == 123, "lsm_csr_prev should return the value provided by __wrap_lsmMCursorPrev (123)");
  CHECK(g_last_ptr == reinterpret_cast<MultiCursor*>(&dummy),
        "lsm_csr_prev should forward the same pointer cast to MultiCursor*");
}

// Test 2: Verify behavior when a NULL pointer is passed.
// The wrapper should still be invoked with NULL, and the return value should be forwarded.
void test_lsm_csr_prev_forwards_null_pointer() {
  g_wrap_ret = -7;               // Arbitrary test value for NULL case
  g_last_ptr = reinterpret_cast<MultiCursor*>(0xDEADBEEF); // Ensure we can detect change

  // Call with NULL
  int rc = lsm_csr_prev(nullptr);

  // Assertions
  CHECK(rc == -7, "lsm_csr_prev should return the value provided by __wrap_lsmMCursorPrev for NULL input");
  CHECK(g_last_ptr == nullptr, "lsm_csr_prev should forward a NULL pointer cast to MultiCursor*");
}

// Entry point: run all tests and summarize results.
// Non-terminating tests: do not abort on first failure; report all.
int main() {
  test_lsm_csr_prev_forwards_wrapped_call();
  test_lsm_csr_prev_forwards_null_pointer();

  if (g_test_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << g_test_failures << " test(s) failed." << std::endl;
    return 1;
  }
}
// sqlite3_malloc64_unit_tests.cpp
//
// Lightweight, framework-free unit tests for the focal method:
//   void *sqlite3_malloc64(sqlite3_uint64 n)
// The tests rely on mock implementations of the dependent functions
// declared in the provided class dependencies to exercise both branches
// of the initialization predicate.
//
// Summary of approach:
// - Mock sqlite3_initialize() to produce success (0) or failure (non-zero).
// - Mock sqlite3Malloc(n) to track the argument, return a deterministic
//   pointer, and allow verification that the returned pointer matches
//   the one produced by the mock.
// - Provide two tests: initialization failure (branch returns 0 immediately)
//   and initialization success (branch proceeds to sqlite3Malloc).
// - Use simple, non-terminating assertions (EXPECT_* macros) to maximize
//   code coverage without terminating test execution on first failure.

#include <cstdio>
#include <sqliteInt.h>
#include <stdarg.h>
#include <cstdlib>
#include <cstdint>


// Domain: Provide mockable dependencies for sqlite3_malloc64's behavior.
// We assume the target compilation unit links against malloc.c which
// provides sqlite3_initialize() and sqlite3Malloc().
// The tests are written in C++11 and rely on C linkage for the mocks.

// Global mock state used by the mocks
static int g_init_return = 0;            // Return value of sqlite3_initialize()
static int g_malloc_called = 0;          // How many times sqlite3Malloc was invoked
static uint64_t g_last_malloc_arg = 0;     // Last argument passed to sqlite3Malloc
static void* g_last_malloc_ptr = nullptr;  // Pointer returned by sqlite3Malloc

// Mocked C functions (visible to sqlite3_malloc64 via linkage)
extern "C" int sqlite3_initialize(void) {
  return g_init_return;
}
extern "C" void *sqlite3Malloc(uint64_t n) {
  g_malloc_called++;
  g_last_malloc_arg = n;
  // Return a deterministic non-null pointer derived from the allocation size
  // Use malloc to allocate the memory; do not rely on a specific address
  g_last_malloc_ptr = malloc((size_t)(n ? n : 1)); // ensure non-null for non-zero n
  return g_last_malloc_ptr;
}

// Declaration of the focal function under test (from malloc.c)
extern "C" void *sqlite3_malloc64(uint64_t n);

// Simple, non-terminating test assertion framework
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    printf("EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
    g_failures++; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if( (long long)(a) != (long long)(b) ) { \
    printf("EXPECT_EQ failed: %s == %s (actual %lld vs %lld) at %s:%d\n", #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
    g_failures++; \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
  if((void*)(a) != (void*)(b)) { \
    printf("EXPECT_PTR_EQ failed: %s == %s (actual %p vs %p) at %s:%d\n", #a, #b, (void*)(a), (void*)(b), __FILE__, __LINE__); \
    g_failures++; \
  } \
} while(0)

// Test 1: Initialization failure should short-circuit and return NULL.
// This exercises the false branch of the if (sqlite3_initialize()) return 0;
static void test_sqlite3_malloc64_initialization_failure() {
  // Arrange
  g_init_return = 1;            // Simulate initialization failure
  g_malloc_called = 0;          // Reset mock call counter

  // Act
  void *result = sqlite3_malloc64(123);

  // Assert: should return NULL and not call sqlite3Malloc
  EXPECT_EQ(result, nullptr);
  EXPECT_EQ(g_malloc_called, 0);
  // No need to free result since it's NULL
}

// Test 2: Initialization succeeds; sqlite3Malloc should be invoked and its
// return value should flow through sqlite3_malloc64.
// This exercises the true branch of the if and verifies argument propagation.
static void test_sqlite3_malloc64_initialization_success() {
  // Arrange
  g_init_return = 0;              // Simulate successful initialization
  g_last_malloc_ptr = nullptr;
  g_last_malloc_arg = 0;
  g_malloc_called = 0;

  // Act
  void *result = sqlite3_malloc64(256); // request 256 bytes

  // Assert
  EXPECT_TRUE(result != nullptr);
  EXPECT_EQ(g_last_malloc_arg, (uint64_t)256);
  EXPECT_PTR_EQ(result, g_last_malloc_ptr);

  // Cleanup allocated memory
  if (result) free(result);

  // Additional sanity: another allocation path with a different size
  void *result2 = sqlite3_malloc64(1);
  EXPECT_TRUE(result2 != nullptr);
  EXPECT_EQ(g_last_malloc_arg, (uint64_t)1);
  if (result2) free(result2);
}

// Entry point: run all tests and report summary.
// Per domain guidance, tests are executed from main (no GTest framework).
int main() {
  printf("Running sqlite3_malloc64 unit tests...\n");

  test_sqlite3_malloc64_initialization_failure();
  test_sqlite3_malloc64_initialization_success();

  if (g_failures == 0) {
    printf("All tests passed.\n");
    return 0;
  } else {
    printf("%d test failure(s) detected.\n", g_failures);
    return 1;
  }
}
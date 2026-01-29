// C++11 unit test suite for the focal method: sqlite3_intarray_bind
// This test suite targets the external C function implemented in test_intarray.c.
// It uses a minimal in-house test harness (no Google Test) and avoids terminating assertions.
// The tests rely on a compatible, minimal replica of the sqlite3_intarray type layout.
// Compile this file together with the production C file that provides sqlite3_intarray_bind.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <test_intarray.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain knowledge notes (embedded as comments for maintainers):
// - We test the following behavior of sqlite3_intarray_bind:
//   1) If pIntArray->xFree is non-null, it is called with the previous array pointer pIntArray->a.
//   2) Then pIntArray->n is set to nElements; pIntArray->a is set to aElements;
//      pIntArray->xFree is updated to the provided xFree.
//   3) The function returns SQLITE_OK (0).
// - We exercise the true/false branches by toggling xFree presence, nElements, and pointers.
// - We use only standard C/C++ facilities; no GTest, and no private members are accessed directly by tests.

// Minimal compatible type/layout definitions to interact with the focal function.
// We assume the layout matches the actual code in test_intarray.c.
typedef long long sqlite3_int64;
#define SQLITE_OK 0

// Forward declaration of the C function under test (extern "C" to ensure correct linkage in C++).
extern "C" {
  // Signature mirrors the one in the focal implementation.
  int sqlite3_intarray_bind(struct sqlite3_intarray*, int, sqlite3_int64*, void(*)(void*));
}

// Minimal replica of the sqlite3_intarray struct used by the focal function.
// This layout mirrors the usage inside sqlite3_intarray_bind.
struct sqlite3_intarray {
  int n;
  sqlite3_int64 *a;
  void (*xFree)(void*);
};

// Global hooks to verify behavior in tests.
// We record whether the old array was freed and what pointer was provided to the free function.
static bool g_old_free_called = false;
static void* g_old_free_ptr = nullptr;

// Old free function used by the initial array. Frees the memory and records the freed pointer.
static void test_old_free(void* p) {
  g_old_free_called = true;
  g_old_free_ptr = p;
  if (p) {
    free(p);
  }
}

// New free function provided to sqlite3_intarray_bind. Simply records invocation.
static bool g_new_free_called = false;
static void* g_new_free_ptr = nullptr;
static void test_new_free(void* p) {
  g_new_free_called = true;
  g_new_free_ptr = p;
  // Do not free here to avoid double-free in test harness (the test may want to inspect the pointer).
}

// Utility macro for test assertions without terminating the test suite.
// It prints a message on failure but allows remaining checks to run.
#define TEST_ASSERT(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "Test assertion failed: " << (msg) << " (in " << __FUNCTION__ << ")\n"; \
      return false; \
    } \
  } while(0)

// Test 1: When pIntArray->xFree is non-null, the previous array is freed and new bindings are applied.
// - Verifies: old array pointer passed to xFree matches the initial a, new a is assigned, n is set,
//   xFree is updated to the provided newFree, and sqlite3_intarray_bind returns SQLITE_OK.
static bool test_bind_frees_old_and_sets_new() {
  // Reset test hooks
  g_old_free_called = false;
  g_old_free_ptr = nullptr;
  g_new_free_called = false;
  g_new_free_ptr = nullptr;

  // Prepare initial and new arrays
  sqlite3_int64* old = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 3);
  for (int i = 0; i < 3; ++i) old[i] = i + 1; // 1,2,3

  sqlite3_int64* newA = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 2);
  newA[0] = 101;
  newA[1] = 202;

  // Prepare pIntArray with existing xFree (old_free)
  sqlite3_intarray p;
  p.n = 3;
  p.a = old;
  p.xFree = test_old_free;

  // Call function under test
  int rc = sqlite3_intarray_bind(&p, 2, newA, test_new_free);

  // Verifications
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_intarray_bind should return SQLITE_OK");
  TEST_ASSERT(p.n == 2, "p.n should be updated to 2 elements");
  TEST_ASSERT(p.a == newA, "p.a should point to the new element array");
  TEST_ASSERT(p.xFree == test_new_free, "p.xFree should be updated to the provided new_free");

  TEST_ASSERT(g_old_free_called, "Old array xFree should have been called");
  TEST_ASSERT(g_old_free_ptr == old, "Old xFree should be called with the previous a pointer");

  // Cleanup: free the new array and avoid double-free on the old one (which was freed by old_free).
  free(newA);

  return true;
}

// Test 2: When pIntArray->xFree is NULL, no attempt to free the old array should be made.
// - Verifies: rc is SQLITE_OK, n and a are updated, xFree is updated, and the free callback is not called.
static bool test_bind_no_free_when_xFree_null() {
  // Reset test hooks
  g_old_free_called = false;
  g_old_free_ptr = nullptr;
  g_new_free_called = false;
  g_new_free_ptr = nullptr;

  // Prepare initial and new arrays
  sqlite3_int64* old = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 3);
  old[0] = 7; old[1] = 8; old[2] = 9;

  sqlite3_int64* newA = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 4);
  newA[0] = 11; newA[1] = 22; newA[2] = 33; newA[3] = 44;

  // pIntArray with NULL xFree
  sqlite3_intarray p;
  p.n = 3;
  p.a = old;
  p.xFree = nullptr;

  int rc = sqlite3_intarray_bind(&p, 4, newA, test_new_free);

  // Verifications
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_intarray_bind should return SQLITE_OK");
  TEST_ASSERT(p.n == 4, "p.n should be updated to 4 elements");
  TEST_ASSERT(p.a == newA, "p.a should point to the new element array");
  TEST_ASSERT(p.xFree == test_new_free, "p.xFree should be updated to the provided new_free");
  TEST_ASSERT(g_old_free_called == false, "Old array xFree should not have been called when xFree is NULL");

  // Cleanup
  free(old);  // old wasn't freed since xFree was NULL
  free(newA);

  return true;
}

// Test 3: Binding with zero elements still updates the struct fields and frees the old array when xFree is non-null.
// - Verifies: n becomes 0, a becomes NULL, xFree updated, and old array freed.
static bool test_bind_zero_elements() {
  // Reset test hooks
  g_old_free_called = false;
  g_old_free_ptr = nullptr;
  g_new_free_called = false;
  g_new_free_ptr = nullptr;

  // Prepare initial array
  sqlite3_int64* old = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 5);
  old[0] = 10; old[1] = 20; old[2] = 30; old[3] = 40; old[4] = 50;

  // Bind with zero elements
  sqlite3_intarray p;
  p.n = 5;
  p.a = old;
  p.xFree = test_old_free;

  int rc = sqlite3_intarray_bind(&p, 0, NULL, test_new_free);

  // Verifications
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_intarray_bind should return SQLITE_OK");
  TEST_ASSERT(p.n == 0, "p.n should be updated to 0 elements");
  TEST_ASSERT(p.a == NULL, "p.a should be NULL when nElements is 0");
  TEST_ASSERT(p.xFree == test_new_free, "p.xFree should be updated to the provided new_free");
  TEST_ASSERT(g_old_free_called == true, "Old array xFree should have been called when bound with NULL new array");
  TEST_ASSERT(g_old_free_ptr == old, "Old xFree should be called with the previous a pointer");

  // Cleanup: old has already been freed by test_old_free; nothing else to free except potential NULL checks.
  return true;
}

// Test 4: Basic return value and non-null pointer handling with typical values.
// - Verifies a successful path even when aElements is non-null and nElements > 0.
static bool test_bind_basic_successpath() {
  // Reset test hooks
  g_old_free_called = false;
  g_old_free_ptr = nullptr;
  g_new_free_called = false;
  g_new_free_ptr = nullptr;

  sqlite3_int64* old = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 1);
  old[0] = 99;

  sqlite3_int64* newA = (sqlite3_int64*)malloc(sizeof(sqlite3_int64) * 1);
  newA[0] = 123;

  sqlite3_intarray p;
  p.n = 1;
  p.a = old;
  p.xFree = test_old_free;

  int rc = sqlite3_intarray_bind(&p, 1, newA, test_new_free);

  // Verifications
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3_intarray_bind should return SQLITE_OK");
  TEST_ASSERT(p.n == 1, "p.n should be updated to 1");
  TEST_ASSERT(p.a == newA, "p.a should point to the new array");
  TEST_ASSERT(p.xFree == test_new_free, "p.xFree should be updated to the provided new_free");
  TEST_ASSERT(g_old_free_called == true, "Old array xFree should have been called");
  TEST_ASSERT(g_old_free_ptr == old, "Old xFree should be called with the previous a pointer");

  // Cleanup
  free(newA);
  // old was freed by test_old_free; do not double-free here

  return true;
}

// Simple test harness
static void run_all_tests() {
  int total = 0;
  int passed = 0;

  std::cout << "Starting sqlite3_intarray_bind tests...\n";

  struct TestEntry {
    const char* name;
    bool (*fn)();
  } tests[] = {
    { "bind_frees_old_and_sets_new", test_bind_frees_old_and_sets_new },
    { "bind_no_free_when_xFree_null", test_bind_no_free_when_xFree_null },
    { "bind_zero_elements", test_bind_zero_elements },
    { "bind_basic_successpath", test_bind_basic_successpath },
  };

  const int nTests = sizeof(tests)/sizeof(tests[0]);
  for (int i = 0; i < nTests; ++i) {
    total++;
    bool ok = tests[i].fn();
    if (ok) {
      passed++;
      std::cout << "[PASS] " << tests[i].name << "\n";
    } else {
      std::cout << "[FAIL] " << tests[i].name << "\n";
    }
  }

  std::cout << "Tests completed: " << passed << "/" << total << " passed.\n";
}

int main() {
  // Run the test suite
  run_all_tests();
  return 0;
}
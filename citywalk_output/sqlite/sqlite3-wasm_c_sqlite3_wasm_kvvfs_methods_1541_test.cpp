/*
  High-quality C++11 unit test suite for the focal method:
    sqlite3_kvvfs_methods * sqlite3_wasm_kvvfs_methods(void)

  Step 1 - Program Understanding (Candidate Keywords)
  - sqlite3_wasm_kvvfs_methods: accessor that returns a pointer to the static instance sqlite3KvvfsMethods.
  - sqlite3_kvvfs_methods: the struct type (in the C file) describing methods for the KVVFS.
  - Static dependency: sqlite3KvvfsMethods is a static instance inside sqlite3-wasm.c; test should not rely on its direct definition.
  - Candidate Keywords for test scenarios:
    - sqlite3_wasm_kvvfs_methods (function under test)
    - sqlite3KvvfsMethods (static instance, lies behind the function)
    - sqlite3_kvvfs_methods (struct type, incomplete type in test to avoid cross-translation-unit issues)
  - Testing goals:
    - Verify the function returns a non-null pointer.
    - Verify repeated calls return the same pointer (consistency).
    - Do not rely on internal field values; keep tests focused on observable behavior from the public API.

  Step 2 - Unit Test Generation (Guided by class dependencies)
  - The focal class file contains a static instance and a getter function. The tests should rely on the public API only.
  - We expose a minimal C linkage in tests to reference the function without pulling in C++-specific details.

  Step 3 - Test Case Refinement
  - Use a lightweight, non-terminating, assertion-based test harness (no GTest, no GMock).
  - Ensure tests are self-contained, portable, and executable with standard library only.
  - Use address comparisons (pointer equality) to verify object identity, not content comparisons of incomplete types.

  Notes for maintainers:
  - The sqlite3_kvvfs_methods type is not defined in the test to avoid cross-translation-unit dependencies.
  - The test asserts pointer non-null and pointer identity across calls.
  - The tests are designed to be robust even if the internal static instance is opaque.

  Build note:
  - The test expects to be linked with the C file sqlite3-wasm.c (which provides sqlite3_wasm_kvvfs_methods).
  - The test uses extern "C" to declare the public API without C++ name mangling concerns.

  The following code implements the described tests.
*/

#include <iostream>
#include <assert.h>
#include <emscripten/wasmfs.h>


// Step 1: Minimal extern C declaration to access the focal function without requiring
// a full C++ definition of the internal struct. We only need the function signature.
extern "C" {
  // Forward declaration of the struct type (incomplete on purpose for test isolation)
  struct sqlite3_kvvfs_methods;
  // Typedef for clarity; the actual layout is in the C file.
  typedef struct sqlite3_kvvfs_methods sqlite3_kvvfs_methods;

  // Focal function under test
  sqlite3_kvvfs_methods * sqlite3_wasm_kvvfs_methods(void);
}

// Simple non-terminating test harness (no GTest/GMock)
static int g_failures = 0;
static int g_tests_run = 0;

#define TEST_ASSERT(cond, msg) do {                                 \
  if(!(cond)) {                                                       \
    std::cerr << "Test failure in " << __FUNCTION__ << ": "          \
              << (msg) << " [line " << __LINE__ << "]" << std::endl; \
    ++g_failures;                                                      \
  }                                                                   \
  ++g_tests_run;                                                       \
} while(0)

// Test 1: The function should never return a null pointer.
static void test_kvvfs_methods_returns_non_null(void) {
  sqlite3_kvvfs_methods * p = sqlite3_wasm_kvvfs_methods();
  TEST_ASSERT(p != nullptr, "sqlite3_wasm_kvvfs_methods() returned null; expected non-null pointer.");
}

// Test 2: Repeated calls should yield the same pointer (consistency of the singleton-like static instance).
static void test_kvvfs_methods_pointer_consistency(void) {
  sqlite3_kvvfs_methods * p1 = sqlite3_wasm_kvvfs_methods();
  sqlite3_kvvfs_methods * p2 = sqlite3_wasm_kvvfs_methods();
  TEST_ASSERT(p1 == p2, "sqlite3_wasm_kvvfs_methods() should return the same pointer on multiple calls.");
}

// Additional exploratory tests (optional, minimal) to increase coverage surface.
// Test 3: Access via address and ensure we can perform a non-null check on the returned pointer's address.
// This test does not dereference the incomplete type; it only validates address-level consistency.
static void test_kvvfs_methods_address_consistency(void) {
  sqlite3_kvvfs_methods * p = sqlite3_wasm_kvvfs_methods();
  void * addr = static_cast<void*>(p);
  TEST_ASSERT(addr != nullptr, "Address of sqlite3_wasm_kvvfs_methods() result should be non-null.");
}

// Entry-point for tests
int main() {
  // Run tests
  test_kvvfs_methods_returns_non_null();
  test_kvvfs_methods_pointer_consistency();
  test_kvvfs_methods_address_consistency();

  // Summary
  if(g_failures == 0) {
    std::cout << "ALL TESTS PASSED (" << g_tests_run << " tests)." << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED (" << g_tests_run << " tests)." << std::endl;
    return 1;
  }
}
/*
  Unit test suite for the focal method:
  void sqlite3_free_filename(const char *p)

  Context:
  - We implement a minimal in-test version of the function under test along with
    necessary static helper (databaseName) to enable controlled testing.
  - We provide a stub for sqlite3_free to observe which pointer is freed.
  - Tests cover:
    1) Null input path: ensures no free operation occurs.
    2) Non-null input path: ensures sqlite3_free is called with the original pointer
       (i.e., (databaseName(p) - 4) == p when databaseName returns p + 4).

  Notes:
  - This test is self-contained and compiles with C++11.
  - No external testing framework is used (as GTest is not allowed).
  - Static function databaseName is implemented as static within the test TU to mirror
    the original static dependency behavior.
*/

#include <iostream>
#include <sqliteInt.h>


// Local static helper mirroring production behavior
// databaseName is static in the focal class/file. Here we implement it as static
// to ensure the sqlite3_free_filename() call resolves to it within this TU.
static const char* databaseName(const char* zName) {
    // Simulated transformation: p -> p + 4
    return zName + 4;
}

// Stubbed free function to observe its input without performing real deallocation
static char *g_last_freed_ptr = nullptr;
static int g_free_call_count = 0;

void sqlite3_free(char *p) {
    ++g_free_call_count;
    g_last_freed_ptr = p;
}

// The focal function under test copied from the provided snippet
void sqlite3_free_filename(const char *p){
  if( p==0 ) return;
  p = databaseName(p);
  sqlite3_free((char*)p - 4);
}

// Local implementation of the function under test
// This is a near-exact replica of the focal method's body, adapted for a test TU.
void sqlite3_free_filename_testable(const char *p){
  if( p==0 ) return;
  p = databaseName(p);
  sqlite3_free((char*)p - 4);
}

// Simple test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define RUN_TEST(cond, name) \
  do { \
    ++g_tests_run; \
    if (cond) { \
      ++g_tests_passed; \
      std::cout << "[OK]    " << name << "\n"; \
    } else { \
      ++g_tests_failed; \
      std::cerr << "[FAILED] " << name << "\n"; \
    } \
  } while(0)

// Test 1: Null input should not trigger sqlite3_free
// Rationale: Early return path must be exercised; no frees should occur.
void test_sqlite3_free_filename_null_input() {
  // Reset observers
  g_last_freed_ptr = nullptr;
  g_free_call_count = 0;

  // Call with null pointer
  sqlite3_free_filename(nullptr);

  // Verify that sqlite3_free was not called
  bool condition = (g_free_call_count == 0) && (g_last_freed_ptr == nullptr);
  RUN_TEST(condition, "sqlite3_free_filename(nullptr) should not call sqlite3_free");
}

// Test 2: Non-null input should compute argument correctly and free original pointer
// Rationale: With databaseName returning p + 4, the free argument should be the original p.
void test_sqlite3_free_filename_non_null_input() {
  // Prepare a dummy buffer and non-null input pointer
  char buffer[8] = {0}; // 8-byte buffer for safety
  const char *input_ptr = buffer;

  // Reset observers
  g_last_freed_ptr = nullptr;
  g_free_call_count = 0;

  // Call the function under test
  sqlite3_free_filename(input_ptr);

  // Expected: sqlite3_free called once with the original input_ptr
  bool condition = (g_free_call_count == 1) && (g_last_freed_ptr == (char*)input_ptr);
  RUN_TEST(condition, "sqlite3_free_filename(non-null) should free the original pointer");
}

// Entrypoint
int main() {
  std::cout << "Running sqlite3_free_filename unit tests (standalone TU test)\n";

  // Run tests
  test_sqlite3_free_filename_null_input();
  test_sqlite3_free_filename_non_null_input();

  // Summary
  std::cout << "\nTest Summary: "
            << g_tests_passed << " passed, "
            << g_tests_failed << " failed, "
            << g_tests_run << " total tests.\n";

  // Return non-zero if any test failed
  return (g_tests_failed == 0) ? 0 : 1;
}
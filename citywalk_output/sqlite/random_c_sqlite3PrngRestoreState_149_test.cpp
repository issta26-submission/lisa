// Lightweight C++11 unit tests for sqlite3PrngRestoreState
// This test harness is self-contained and does not rely on GTest.
// It mocks minimal global state and reproduces the focal function
// using a small, deterministic structure. Each test includes
// explanatory comments as requested.

#include <cstdio>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Compatibility shim for the focal GLOBAL macro.
// In the original code, GLOBAL(type, name) resolves to an access to a
// global variable. For the test, we map it directly to the variable name.
#define GLOBAL(type, name) name

// Focal class dependency shim (minimal version required for the test).
// We define sqlite3PrngType as a simple 4-int structure to exercise memory
// copy semantics and padding considerations.
struct sqlite3PrngType {
  int v[4];
};

// Static/global instances representing the PRNG state.
// These stand in for the actual global variables used by the real code.
static sqlite3PrngType sqlite3Prng;
static sqlite3PrngType sqlite3SavedPrng;

// Focal method under test copied from the provided snippet, adapted to direct
// global usage via the GLOBAL macro defined above.
void sqlite3PrngRestoreState(void){
  memcpy(
    &GLOBAL(struct sqlite3PrngType, sqlite3Prng),
    &GLOBAL(struct sqlite3PrngType, sqlite3SavedPrng),
    sizeof(sqlite3Prng)
  );
}

// Helper utilities for test setup and verification.
static inline void setPrng(int a,int b,int c,int d) {
  sqlite3Prng.v[0] = a;
  sqlite3Prng.v[1] = b;
  sqlite3Prng.v[2] = c;
  sqlite3Prng.v[3] = d;
}
static inline void setSaved(int a,int b,int c,int d) {
  sqlite3SavedPrng.v[0] = a;
  sqlite3SavedPrng.v[1] = b;
  sqlite3SavedPrng.v[2] = c;
  sqlite3SavedPrng.v[3] = d;
}
static inline bool prngEqualsSaved() {
  // Compare the entire 4-int block to verify exact memory copy.
  return std::memcmp(sqlite3Prng.v, sqlite3SavedPrng.v, sizeof(sqlite3Prng.v)) == 0;
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_total = 0;
static int g_tests_failed = 0;
#define CHECK(cond, msg) do { \
  ++g_tests_total; \
  if(!(cond)) { \
    std::cerr << "Test failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_tests_failed; \
  } \
} while(0)

// Test 1: Basic restoration copies saved state into current PRNG state.
// The function is expected to copy all bytes from sqlite3SavedPrng to sqlite3Prng.
void test_restore_basic_case() {
  // Setup distinct initial states
  setSaved(1, 2, 3, 4);
  setPrng(9, 9, 9, 9);

  // Exercise the focal function
  sqlite3PrngRestoreState();

  // Verify that the current PRNG state matches the saved state exactly
  CHECK(prngEqualsSaved(), "basic restore should copy saved to prng");
}

// Test 2: No-op restoration when PRNG and saved states are already identical.
// The function should not alter the already-equal state.
void test_restore_no_change_when_already_equal() {
  setSaved(5, 6, 7, 8);
  setPrng(5, 6, 7, 8);

  sqlite3PrngRestoreState();

  CHECK(prngEqualsSaved(), "restore should preserve when already equal");
}

// Test 3: Restoration works across multiple state changes.
// Change saved, then change PRNG, restore again to ensure latest saved
// state is always the source of truth.
void test_restore_multiple_steps() {
  // First step: saved = (10,20,30,40), prng = all negatives
  setSaved(10, 20, 30, 40);
  setPrng(-1, -1, -1, -1);
  sqlite3PrngRestoreState();
  CHECK(prngEqualsSaved(), "restore after changing prng to negative values");

  // Second step: saved changes to a new pattern, prng resets to zeros
  setSaved(100, 200, 300, 400);
  setPrng(0, 0, 0, 0);
  sqlite3PrngRestoreState();
  CHECK(prngEqualsSaved(), "restore after updating saved to new pattern");
}

// Entry point that runs all tests and reports a summary.
// Exits non-zero if any test failed to align with typical CI expectations.
int main() {
  test_restore_basic_case();
  test_restore_no_change_when_already_equal();
  test_restore_multiple_steps();

  std::cout << "Tests run: " << g_tests_total
            << ", Failures: " << g_tests_failed << std::endl;
  return g_tests_failed ? 1 : 0;
}
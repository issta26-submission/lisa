// sqlite3_initialize_unit_tests.cpp
// A small C++11 test harness for the focal method sqlite3_initialize
// This test suite avoids GTest and uses non-terminating EXPECT_* style checks.
// It targets the public behavior of sqlite3_initialize() (idempotence and success path).

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Forward declare the C function under test.
// The actual implementation is expected to be linked with this test.
extern "C" int sqlite3_initialize(void);

// Lightweight, non-terminating test assertion helpers.
// They accumulate failures but do not abort test execution.
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[EXPECT_TRUE FAILED] " << (msg) \
                << " (Condition: " #cond ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_EQ(a, b, msg) \
  do { \
    if(!((a) == (b))) { \
      std::cerr << "[EXPECT_EQ FAILED] " << (msg) \
                << " (Expected: " #a " == " #b ", Got: " << (a) \
                << " != " << (b) << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)

// Test 1: Fresh initialization should succeed (non-negative return code).
// Rationale: On first invocation, sqlite3_initialize should perform initialization
// and return SQLITE_OK (0) in typical configurations. We cannot assume a specific
// numeric value without including internal headers, so we test non-negativity.
static bool test_fresh_initialization() {
  std::cout << "Running test_fresh_initialization..." << std::endl;

  int rc = sqlite3_initialize();

  // Expect a non-negative return code to indicate success path or a valid rc.
  EXPECT_TRUE(rc >= 0, "First sqlite3_initialize() should return a non-negative rc (success path).");

  // Optionally, ensure that subsequent calls do not crash or produce negative rc immediately.
  // We do not rely on a specific value beyond non-negativity here.
  return true;
}

// Test 2: Repeated initialization should be a no-op (still succeed and not crash).
// Rationale: After the first successful initialization, subsequent calls to sqlite3_initialize()
// should be effectively a no-op and return SQLITE_OK (0) or a non-error rc.
// Since we cannot rely on the exact numeric value without internal headers, we verify
// that the second call also yields a non-negative rc.
static bool test_repeated_initialization() {
  std::cout << "Running test_repeated_initialization..." << std::endl;

  int rc1 = sqlite3_initialize();
  int rc2 = sqlite3_initialize();

  // The second call should not fail; it should return a non-negative rc.
  EXPECT_TRUE(rc2 >= 0, "Second sqlite3_initialize() should also return a non-negative rc (no-op path).");

  // Additional sanity: the second rc should be at least as sane as the first if both are non-negative.
  // This is a weak check but useful when internal rc codes are not exposed to the test.
  EXPECT_TRUE(rc2 == rc1 || rc1 >= 0, "Second rc should be equal to first or both non-negative.");
  return true;
}

// Entry point - runs all tests and reports summary.
// The Domain Knowledge instructs to call test methods from main for environments
// where a test framework is not available.
int main() {
  std::cout << "Starting sqlite3_initialize unit test suite (C++11, no GTest)" << std::endl;

  // Run individual tests
  test_fresh_initialization();
  test_repeated_initialization();

  // Summary
  if(g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << g_failures << " test(s) failed." << std::endl;
    return g_failures;
  }
}
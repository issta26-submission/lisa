// Test suite for sqlite3_wasm_config_j
// Goal: verify that sqlite3_wasm_config_j forwards its arguments to sqlite3_config
// and returns the same result. Since sqlite3_config is not part of this test,
// a small C stub is provided to simulate sqlite3_config behavior.
// The test uses a lightweight, non-GTest, non-terminating assertion mechanism.

#include <iostream>
#include <assert.h>
#include <cstdint>
#include <emscripten/wasmfs.h>


// Domain type alias expected by sqlite3_wasm_config_j
using sqlite3_int64 = long long;

// Forward declaration of the function under test.
// This is the C-style API used by sqlite3_wasm_config_j.
extern "C" int sqlite3_wasm_config_j(int op, sqlite3_int64 arg);

// Mock/stub of sqlite3_config to control behavior for tests.
// This function should be linked with the sqlite3_wasm_config_j under test.
// We implement deterministic behavior to cover multiple branches.
extern "C" int sqlite3_config(int op, sqlite3_int64 arg) {
  // True branch: explicit special-case test triggers
  if (op == 7) {
    return 700;
  }
  // True branch: another explicit test case
  if (op == 1 && arg == 5) {
    return 55;
  }
  // Default behavior covers other inputs to exercise forward-through-logic
  // (op + (arg % 100)) cast to int to simulate a simple mapping.
  return static_cast<int>(op + (arg % 100));
}

// Lightweight, non-terminating test assertions.
// These mimic the "EXPECT_" style from GTest but do not abort on failure.
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(expected, actual, msg) do {                       \
    ++g_total;                                                        \
    const auto _exp = (expected);                                   \
    const auto _act = (actual);                                     \
    if (!(_exp == _act)) {                                         \
        ++g_failed;                                                  \
        std::cerr << "[FAIL] " << msg                                \
                  << " | Expected: " << _exp                         \
                  << " | Actual: " << _act << " [" << __FILE__ \
                  << ":" << __LINE__ << "]\n";                       \
    } else {                                                        \
        std::cout << "[PASS] " << msg << "\n";                      \
    }                                                               \
} while(0)

// Test Case 1: op=7 should map directly to 700 via sqlite3_config
void test_case_op7_forwards() {
  int result = sqlite3_wasm_config_j(7, 0);
  EXPECT_EQ(700, result, "sqlite3_wasm_config_j forwards op=7 to sqlite3_config, arg ignored");
}

// Test Case 2: op=1, arg=5 should map to 55 via sqlite3_config
void test_case_op1_arg5_forwards() {
  int result = sqlite3_wasm_config_j(1, 5);
  EXPECT_EQ(55, result, "sqlite3_wasm_config_j forwards op=1,arg=5 to sqlite3_config -> 55");
}

// Test Case 3: Default path: op=2, arg=3 should compute 2 + (3 % 100) = 5
void test_case_basic_forwarding() {
  int result = sqlite3_wasm_config_j(2, 3);
  EXPECT_EQ(5, result, "sqlite3_wasm_config_j forwards general case: 2 + 3 = 5");
}

// Test Case 4: Default path with larger arg: op=3, arg=1234 -> 3 + 34 = 37
void test_case_large_arg() {
  int result = sqlite3_wasm_config_j(3, 1234);
  EXPECT_EQ(37, result, "sqlite3_wasm_config_j forwards large arg: 3 + (1234%100) = 37");
}

// Test Case 5: Very large arg to test modulo behavior: op=1, arg=1e18 -> 1 + (arg%100)
void test_case_very_large_arg() {
  long long large = 1000000000000000000LL;
  int result = sqlite3_wasm_config_j(1, large);
  // arg%100 == 0, so result should be 1
  EXPECT_EQ(1, result, "sqlite3_wasm_config_j forwards very large arg: 1 + (1e18 % 100) = 1");
}

// Main driver: run all tests and summarize
int main() {
  std::cout << "Starting tests for sqlite3_wasm_config_j forwarding...\n";

  test_case_op7_forwards();
  test_case_op1_arg5_forwards();
  test_case_basic_forwarding();
  test_case_large_arg();
  test_case_very_large_arg();

  std::cout << "Test summary: Total=" << g_total
            << " Passed=" << (g_total - g_failed)
            << " Failed=" << g_failed << "\n";

  // Return non-zero if any test failed
  return g_failed ? 1 : 0;
}
/*
Step 1: Program Understanding (high-level)
- Focused method under test: sqlite3_wasm_config_i(int op, int arg)
- Behavior: simply forwards the call to sqlite3_config(op, arg) and returns its result.
- Core dependency: sqlite3_config(op, arg) (provided by the surrounding SQLite/WASM integration in the project)
- Candidate Keywords representing core dependent components:
  - sqlite3_wasm_config_i
  - sqlite3_config
  - op (config operation code)
  - arg (configuration argument)
  - pass-through/forwarding behavior
  - return value from sqlite3_config
  - test harness for forward-call verification

Step 2: Unit Test Generation (conceptual plan)
- Validate that sqlite3_wasm_config_i forwards op and arg to sqlite3_config and returns the same value.
- Since sqlite3_config is a core function, we exercise multiple op/arg pairs to ensure consistent pass-through.
- We'll create a lightweight, self-contained test harness (no GTest) that mimics a minimal assertion framework (non-terminating assertions) to maximize code execution and coverage.
- Tests should cover:
  - Basic pass-through with small positive values.
  - Edge-like values (zero and negative).
  - Mixed, larger values to ensure no overflow in the wrapper (the wrapper itself does not modify inputs).
  - Repeated calls to ensure internal state (if any) does not affect correctness of the wrapper.

Step 3: Test Case Refinement (domain knowledge considerations)
- Use a simple non-terminating assertion style (EXPECT_EQ) to keep test execution flowing after failed assertions.
- Keep tests in a single translation unit for simplicity and determinism, since we emulate the core dependency locally for the test.
- Provide explanatory comments for each test case describing intent and expected outcomes.
- All tests rely on the fact that sqlite3_wasm_config_i is just a thin wrapper.

Note: The test harness below is designed to be compiled together with the focal method’s code (as a single translation unit). It uses a local, deterministic mock of sqlite3_config to ensure testability in environments where the real SQLite core may not be available. If your build environment already links sqlite3_config from the real core, you can adapt the test by removing the local mock and relying on the real function, adjusting expectations accordingly.
*/

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <limits>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal wrapper (as in sqlite3-wasm.c)
extern "C" int sqlite3_wasm_config_i(int op, int arg);

// Local mock and test harness
// We provide a local definition for sqlite3_config to enable a self-contained test harness.
// In a full build where sqlite3_config is provided by the SQLite core, replace this mock
// with the real symbol and adjust expectations accordingly.

static int g_last_op = 0;
static int g_last_arg = 0;

// Mock implementation of sqlite3_config to capture inputs and provide a deterministic return.
// This mock is used to verify that sqlite3_wasm_config_i forwards its parameters correctly
// and returns the exactly same value produced by sqlite3_config.
extern "C" int sqlite3_config(int op, int arg) {
  g_last_op = op;
  g_last_arg = arg;
  // Deterministic, simple return: sum (op + arg)
  // This is only for testing the wrapper; real behavior is not required here.
  // Note: In actual production, sqlite3_config would implement a richer behavior.
  return op + arg;
}

// Focal method replicated for test context (wrapper around sqlite3_config)
extern "C" int sqlite3_wasm_config_i(int op, int arg){
  return sqlite3_config(op, arg);
}

// Lightweight non-terminating test framework (EXPECT_*)
// Increments test count, prints a message on failure, but continues execution.
static int g_test_total = 0;
static int g_test_failed = 0;

#define EXPECT_EQ(actual, expected) do { \
  ++g_test_total; \
  if ((actual) != (expected)) { \
    std::cerr << "TEST FAIL: " << __FUNCTION__ \
              << " at " << __FILE__ << ":" << __LINE__ \
              << " | expected: " << (expected) \
              << " but got: " << (actual) << std::endl; \
    ++g_test_failed; \
  } \
} while(0)

#define RUN_TEST(name) \
  std::cout << "RUNNING: " #name "..." << std::endl; \
  name(); \
  std::cout << "DONE: " #name "." << std::endl;

// Test 1: Basic pass-through with small positive values
// Intent: Verify that sqlite3_wasm_config_i forwards op and arg to sqlite3_config
// and returns the same result produced by the mock (op + arg).
void test_config_i_basic_pass_through() {
  // Reset mock state
  g_last_op = 0;
  g_last_arg = 0;

  int op = 5;
  int arg = 4;
  int result = sqlite3_wasm_config_i(op, arg);

  // Expect wrapper to return op + arg (as defined by mock sqlite3_config)
  EXPECT_EQ(result, op + arg);
  // Expect the forwarder to have passed both operands correctly
  EXPECT_EQ(g_last_op, op);
  EXPECT_EQ(g_last_arg, arg);
}

// Test 2: Edge-like values (zero and negative)
void test_config_i_zero_and_negative() {
  g_last_op = 0;
  g_last_arg = 0;

  int op = 0;
  int arg = -7;
  int result = sqlite3_wasm_config_i(op, arg);

  EXPECT_EQ(result, op + arg);
  EXPECT_EQ(g_last_op, op);
  EXPECT_EQ(g_last_arg, arg);
}

// Test 3: Larger values to exercise no unintended modification
void test_config_i_large_values() {
  g_last_op = 0;
  g_last_arg = 0;

  int op = 1000;
  int arg = 2000;
  int result = sqlite3_wasm_config_i(op, arg);

  EXPECT_EQ(result, op + arg);
  EXPECT_EQ(g_last_op, op);
  EXPECT_EQ(g_last_arg, arg);
}

// Test 4: Repeated calls to ensure wrapper stability and state tracking
void test_config_i_multiple_calls() {
  for (int i = 0; i < 4; ++i) {
    int op = i;
    int arg = i * 2;
    int result = sqlite3_wasm_config_i(op, arg);
    EXPECT_EQ(result, op + arg);
    EXPECT_EQ(g_last_op, op);
    EXPECT_EQ(g_last_arg, arg);
  }
}

int main() {
  std::cout << "Starting test suite for sqlite3_wasm_config_i (forwarding tests)\n";

  RUN_TEST(test_config_i_basic_pass_through);
  RUN_TEST(test_config_i_zero_and_negative);
  RUN_TEST(test_config_i_large_values);
  RUN_TEST(test_config_i_multiple_calls);

  std::cout << "Test summary: total=" << g_test_total
            << ", failed=" << g_test_failed << std::endl;

  // Return non-zero if any test failed
  return g_test_failed ? 1 : 0;
}
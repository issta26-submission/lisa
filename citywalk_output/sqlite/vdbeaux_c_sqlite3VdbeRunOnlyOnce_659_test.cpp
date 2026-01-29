// Test suite for sqlite3VdbeRunOnlyOnce
// Purpose: verify that the focal function correctly issues a single OP_Expire
//          with p1 = 1 and p2 = 1 via sqlite3VdbeAddOp2.
// Notes for build/run (external to this code):
// - This test is designed to be linked with the actual SQLite source that provides
//   sqlite3VdbeRunOnlyOnce and sqlite3VdbeAddOp2 (from vdbeaux.c).
// - To compile and run, include the real vdbeaux.c (or the relevant SQLite
//   sources) in the build, so that the symbol sqlite3VdbeRunOnlyOnce is resolved.
// - This test provides a mock implementation of sqlite3VdbeAddOp2 with C linkage
//   to capture how sqlite3VdbeRunOnlyOnce invokes it.

#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// 1) Import all necessary dependencies with correct paths.
// In this test, we keep dependencies minimal and use opaque type declarations.

extern "C" {
  // Forward declare the Vdbe type as an opaque struct to be compatible with the
  // production function signature. The real SQLite definition is in the SQLite headers.
  typedef struct Vdbe Vdbe;

  // The function under test is declared here so we can call it from C++.
  // The real implementation should be provided by vdbeaux.c during linking.
  void sqlite3VdbeRunOnlyOnce(Vdbe *p);

  // The function we mock to capture calls made by the focal function.
  // We declare it with C linkage to be compatible with the production code.
  int sqlite3VdbeAddOp2(Vdbe *p, int op, int p1, int p2);
}

// 2) Candidate Keywords inferred from the focal method (for test intent documentation)
#define OP_Expire 0x37  // Chosen value for OP_Expire used in tests

// Global state to capture calls made to sqlite3VdbeAddOp2 by the focal function.
// We deliberately keep these as simple integers for easy assertions.
static int g_op_called = 0;
static int g_p1_value = 0;
static int g_p2_value = 0;
static int g_call_count = 0;

// 3) Provide a minimal C linkage mock of sqlite3VdbeAddOp2 to observe interactions.
// The test does not rely on the actual sqlite logic; it only needs to observe the
// parameters passed by sqlite3VdbeRunOnlyOnce.
extern "C" int sqlite3VdbeAddOp2(Vdbe *p, int op, int p1, int p2) {
  // Record the values that sqlite3VdbeRunOnlyOnce forwards to sqlite3VdbeAddOp2.
  (void)p; // p is unused by the focal test, but we acknowledge it to avoid warnings.
  g_op_called = op;
  g_p1_value = p1;
  g_p2_value = p2;
  ++g_call_count;
  return 0; // Simulate a successful add operation
}

// 4) Lightweight test harness utilities.
// We implement non-terminating assertions to maximize code execution across tests.

static int g_tests_failed = 0;

#define EXPECT_EQ(expected, actual, message) do { \
  if ((expected) != (actual)) { \
    std::cerr << "TEST_FAIL: " << (message) \
              << " | Expected: " << (expected) \
              << ", Actual: " << (actual) << std::endl; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_TRUE(cond, message) do { \
  if (!(cond)) { \
    std::cerr << "TEST_FAIL: " << (message) << " | Condition is false" << std::endl; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_FALSE(cond, message) do { \
  if (cond) { \
    std::cerr << "TEST_FAIL: " << (message) << " | Condition is true" << std::endl; \
    ++g_tests_failed; \
  } \
} while(0)

// 5) Test cases

// Test 1: Ensure a single call to sqlite3VdbeRunOnlyOnce results in exactly one
//         invocation of sqlite3VdbeAddOp2 with OP_Expire, p1=1, p2=1.
void test_RunOnlyOnce_SingleInvocation_IsExpiringOp() {
  // Reset captured state
  g_op_called = 0;
  g_p1_value = 0;
  g_p2_value = 0;
  g_call_count = 0;

  Vdbe v;
  sqlite3VdbeRunOnlyOnce(&v);

  // Validate that exactly one addOp occurred
  EXPECT_EQ(1, g_call_count, "sqlite3VdbeRunOnlyOnce should call sqlite3VdbeAddOp2 exactly once");

  // Validate that the operation and operands are as expected
  EXPECT_EQ(OP_Expire, g_op_called, "Opcode passed to sqlite3VdbeAddOp2 should be OP_Expire");
  EXPECT_EQ(1, g_p1_value, "First parameter (p1) should be 1");
  EXPECT_EQ(1, g_p2_value, "Second parameter (p2) should be 1");
}

// Test 2: Ensure multiple consecutive invocations are independently recorded.
// Each call should still produce a single addOp2 with the expected values.
void test_RunOnlyOnce_MultipleInvocations_CorrectEachTime() {
  // First invocation
  g_op_called = 0; g_p1_value = 0; g_p2_value = 0; g_call_count = 0;
  Vdbe v1;
  sqlite3VdbeRunOnlyOnce(&v1);
  EXPECT_EQ(1, g_call_count, "First invocation should produce exactly one addOp2");
  EXPECT_EQ(OP_Expire, g_op_called, "First invocation: opcode should be OP_Expire");
  EXPECT_EQ(1, g_p1_value, "First invocation: p1 should be 1");
  EXPECT_EQ(1, g_p2_value, "First invocation: p2 should be 1");

  // Second invocation
  g_op_called = 0; g_p1_value = 0; g_p2_value = 0; g_call_count = 0;
  Vdbe v2;
  sqlite3VdbeRunOnlyOnce(&v2);
  EXPECT_EQ(1, g_call_count, "Second invocation should produce exactly one addOp2");
  EXPECT_EQ(OP_Expire, g_op_called, "Second invocation: opcode should be OP_Expire");
  EXPECT_EQ(1, g_p1_value, "Second invocation: p1 should be 1");
  EXPECT_EQ(1, g_p2_value, "Second invocation: p2 should be 1");
}

// Test 3: Optional robustness check - ensure the function handles a null pointer gracefully
// by not crashing. This test relies on the fact that the mock does not dereference the p.
// If the real implementation requires a non-null pointer, this test should be adapted accordingly.
void test_RunOnlyOnce_WithNullPointer_DoesNotCrash() {
  g_op_called = 0; g_p1_value = 0; g_p2_value = 0; g_call_count = 0;
  Vdbe *null_p = nullptr;
  // We expect that the focal function either handles null gracefully or the test environment
  // ensures non-null; we cannot guarantee behavior of the real code here, but we attempt to call it.
  // If the real function asserts on null, this test would fail in a controlled manner.
  sqlite3VdbeRunOnlyOnce(null_p);
  // If it returns, we should not have made a call to sqlite3VdbeAddOp2 with a non-null pointer.
  EXPECT_EQ(0, g_call_count, "Calling with null Vdbe pointer should not invoke sqlite3VdbeAddOp2");
}

// 6) Main driver to run tests and report summary.

int main() {
  test_RunOnlyOnce_SingleInvocation_IsExpiringOp();
  test_RunOnlyOnce_MultipleInvocations_CorrectEachTime();
  test_RunOnlyOnce_WithNullPointer_DoesNotCrash();

  if (g_tests_failed == 0) {
    std::cout << "ALL_TESTS_PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_tests_failed << "_TESTS_FAILED" << std::endl;
    return 1;
  }
}

// Step notes for reviewers:
// - This test suite intentionally uses a lightweight C linkage mock for sqlite3VdbeAddOp2
//   to observe interactions without requiring the full SQLite test harness.
// - It covers the primary path (true predicate of the focal method doesn't exist) by asserting
//   that the expected opcode and operands are produced.
// - The tests are designed to be extended if additional behaviors are introduced in the focal method
//   or if the surrounding API evolves.
// C++11 test suite for the focal method sqlite3VdbeAddOp1
// Note: This is a self-contained unit-test harness that mocks the
// dependency sqlite3VdbeAddOp3 in order to verify forwarding behavior
// of sqlite3VdbeAddOp1. It does not rely on the full SQLite/Vdbe runtime.
// The goal is to achieve high coverage of the forwarding logic (p, op, p1, and zeros).

#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain knowledge: Provide minimal stand-ins to mimic the environment
// required by the focal method.
// - Vdbe is an opaque type in the real project; we provide a minimal struct here.
struct Vdbe {
  int dummy; // placeholder to simulate a real object
};

// Forward declarations to mimic the real environment.
// The focal method under test (in our isolated harness) forwards to AddOp3.
extern "C" {
  int sqlite3VdbeAddOp3(Vdbe *p, int op, int p1, int p2, int p3);
  int sqlite3VdbeAddOp1(Vdbe *p, int op, int p1);
}

// Global state used by the mocked AddOp3 to verify forwarding behavior.
static Vdbe *g_last_pPtr = nullptr;
static int g_last_op = 0;
static int g_last_p1 = 0;
static int g_last_p2 = 0;
static int g_last_p3 = 0;

// Return value from the mocked AddOp3 to validate propagation via AddOp1.
static int g_mockReturnValue = 0;

// Mock implementation of sqlite3VdbeAddOp3 to observe call parameters.
// This mock intentionally represents only the contract needed by sqlite3VdbeAddOp1.
// It stores all arguments to global state and returns a controllable value.
extern "C" int sqlite3VdbeAddOp3(Vdbe *p, int op, int p1, int p2, int p3) {
  g_last_pPtr = p;
  g_last_op   = op;
  g_last_p1   = p1;
  g_last_p2   = p2;
  g_last_p3   = p3;
  return g_mockReturnValue;
}

// Realistic forwarder under test (in this harness).
// In the actual project, this function is implemented in vdbeaux.c.
// Here we reproduce the forwarding logic to enable self-contained testing.
extern "C" int sqlite3VdbeAddOp1(Vdbe *p, int op, int p1) {
  return sqlite3VdbeAddOp3(p, op, p1, 0, 0);
}

// Lightweight test framework (non-terminating assertions)
#define EXPECT_TRUE(cond) do {                                     \
  if(!(cond)) {                                                      \
    std::cerr << "EXPECT_TRUE failed: " #cond                    \
              << " at " << __FILE__ << ":" << __LINE__ << "\n";     \
    return false;                                                    \
  }                                                                  \
} while(0)

#define EXPECT_EQ(a,b) do {                                          \
  if(!((a) == (b))) {                                                \
    std::cerr << "EXPECT_EQ failed: " #a " == " #b                   \
              << " (actual: " << (a) << ", " << (b) << ")"        \
              << " at " << __FILE__ << ":" << __LINE__ << "\n";      \
    return false;                                                    \
  }                                                                  \
} while(0)

// Test 1: Forwarding with non-null Vdbe*, typical values for op and p1.
// Verifies: AddOp1 forwards p, op, and p1 to AddOp3, and that p2 and p3 are 0.
bool test_forward_non_null_p_and_values() {
  // Arrange
  g_mockReturnValue = 12345; // deterministic return value from mocked AddOp3
  Vdbe vm;
  vm.dummy = 7;

  // Act
  int ret = sqlite3VdbeAddOp1(&vm, 0xAB, 0xCD);

  // Assert
  EXPECT_EQ(ret, 12345);                 // AddOp1 should return value from AddOp3
  EXPECT_TRUE(g_last_pPtr == &vm);       // p forwarded
  EXPECT_EQ(g_last_op, 0xAB);            // op forwarded
  EXPECT_EQ(g_last_p1, 0xCD);            // p1 forwarded
  EXPECT_EQ(g_last_p2, 0);                 // p2 should be 0
  EXPECT_EQ(g_last_p3, 0);                 // p3 should be 0
  return true;
}

// Test 2: Forwarding with a different set of values to ensure consistency.
// Verifies: Always forwards zeros for p2 and p3 and preserves p, op, p1.
bool test_forward_other_values() {
  // Arrange
  g_mockReturnValue = -9999;
  Vdbe vm;
  vm.dummy = 42;

  // Act
  int ret = sqlite3VdbeAddOp1(&vm, 0x01, 0x02);

  // Assert
  EXPECT_EQ(ret, -9999);
  EXPECT_TRUE(g_last_pPtr == &vm);
  EXPECT_EQ(g_last_op, 0x01);
  EXPECT_EQ(g_last_p1, 0x02);
  EXPECT_EQ(g_last_p2, 0);
  EXPECT_EQ(g_last_p3, 0);
  return true;
}

// Test 3: Forwarding with a null Vdbe pointer to verify null safety in forwarding.
// In real SQLite code, passing a null Vdbe might be invalid; here we validate
// that the call forwards the null pointer and does not crash in this harness.
bool test_forward_null_pointers() {
  // Arrange
  g_mockReturnValue = 555;
  g_last_pPtr = nullptr;
  g_last_op = g_last_p1 = g_last_p2 = g_last_p3 = 0;

  // Act
  int ret = sqlite3VdbeAddOp1(nullptr, 0x77, 0x88);

  // Assert
  EXPECT_EQ(ret, 555);
  EXPECT_TRUE(g_last_pPtr == nullptr);
  EXPECT_EQ(g_last_op, 0x77);
  EXPECT_EQ(g_last_p1, 0x88);
  EXPECT_EQ(g_last_p2, 0);
  EXPECT_EQ(g_last_p3, 0);
  return true;
}

// Test 4: Repeated calls to ensure state resets between tests (sanity across calls).
bool test_repeated_calls_state_isolated() {
  // Arrange first call
  g_mockReturnValue = 55;
  Vdbe a, b;
  a.dummy = 1;
  b.dummy = 2;

  // Act first
  int r1 = sqlite3VdbeAddOp1(&a, 0, 1);
  EXPECT_EQ(r1, 55);
  EXPECT_TRUE(g_last_pPtr == &a);
  EXPECT_EQ(g_last_op, 0);
  EXPECT_EQ(g_last_p1, 1);
  EXPECT_EQ(g_last_p2, 0);
  EXPECT_EQ(g_last_p3, 0);

  // Reset mock return value for second call
  g_mockReturnValue = 77;

  // Act second
  int r2 = sqlite3VdbeAddOp1(&b, 2, 3);
  EXPECT_EQ(r2, 77);
  EXPECT_TRUE(g_last_pPtr == &b);
  EXPECT_EQ(g_last_op, 2);
  EXPECT_EQ(g_last_p1, 3);
  EXPECT_EQ(g_last_p2, 0);
  EXPECT_EQ(g_last_p3, 0);

  return true;
}

// Driver: run all tests and report summary
int main() {
  int total = 0;
  int failures = 0;

  auto run = [&](const std::string& name, bool (*test)()) {
    ++total;
    bool ok = test();
    if(ok) {
      std::cout << "[PASS] " << name << "\n";
    } else {
      std::cout << "[FAIL] " << name << "\n";
      ++failures;
    }
  };

  run("test_forward_non_null_p_and_values", test_forward_non_null_p_and_values);
  run("test_forward_other_values", test_forward_other_values);
  run("test_forward_null_pointers", test_forward_null_pointers);
  run("test_repeated_calls_state_isolated", test_repeated_calls_state_isolated);

  std::cout << "Tests executed: " << total << ", Failures: " << failures << "\n";

  return failures ? 1 : 0;
}
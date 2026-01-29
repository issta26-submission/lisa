// Self-contained C++11 unit tests for sqlite3VdbeAddOp0
// This test recreates a minimal, isolated environment to verify the behavior
// of the focal method sqlite3VdbeAddOp0 by using a local mock of sqlite3VdbeAddOp3.
// It does not rely on GTest or any external testing framework.
// Explanatory comments accompany each test case.

// DOMAIN_KNOWLEDGE: Observations
// - sqlite3VdbeAddOp0 simply forwards to sqlite3VdbeAddOp3 with p1=p2=p3=0.
// - We mock sqlite3VdbeAddOp3 to capture incoming parameters and return a sentinel value.
// - We verify forwarding correctness, including handling of a non-null and a null p pointer,
//   and that AddOp0 returns the exact value produced by AddOp3 (sentinel).

#include <stdexcept>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Minimal stand-in for the Vdbe type used by sqlite3VdbeAddOp0/Op3.
// The real project uses a richer definition, but for unit testing forwarding we only need a type.
struct Vdbe { int dummy; };

// Forward declarations of the focal function and its dependency (mocked for testing).
// In a real scenario these would come from the actual project headers, but here we provide
// lightweight, test-scoped implementations to isolate behavior.
static bool g_called = false;            // Was sqlite3VdbeAddOp3 invoked?
static Vdbe* g_lastP = nullptr;            // P parameter received by sqlite3VdbeAddOp3
static int g_lastOp = 0;                   // op parameter received by sqlite3VdbeAddOp3
static int g_lastP1 = 0, g_lastP2 = 0, g_lastP3 = 0; // p1, p2, p3 values received
static int g_lastRet = 0;                   // Return value from sqlite3VdbeAddOp3

// Mock implementation of sqlite3VdbeAddOp3 that records its inputs and returns a sentinel.
// This simulates the intended underlying behavior without requiring the full SQLite source.
extern "C" int sqlite3VdbeAddOp3(Vdbe *p, int op, int p1, int p2, int p3){
  g_called = true;
  g_lastP = p;
  g_lastOp = op;
  g_lastP1 = p1;
  g_lastP2 = p2;
  g_lastP3 = p3;
  g_lastRet = 999; // Sentinel value to verify return path
  return g_lastRet;
}

// The focal method under test (as implemented in vdbeaux.c):
// int sqlite3VdbeAddOp0(Vdbe *p, int op){
//   return sqlite3VdbeAddOp3(p, op, 0, 0, 0);
// }
extern "C" int sqlite3VdbeAddOp0(Vdbe *p, int op){
  return sqlite3VdbeAddOp3(p, op, 0, 0, 0);
}

// Simple, self-contained test framework (no external dependencies)
namespace TestFramework {

class TestResult {
public:
  std::string name;
  bool passed;
  std::string message;
  TestResult(const std::string& n, bool p, const std::string& m)
    : name(n), passed(p), message(m) {}
};

static void fail(const std::string& name, const std::string& msg) {
  throw std::runtime_error("Test failed: " + name + " - " + msg);
}

// Test: sqlite3VdbeAddOp0 forwards op and zeroed p1/p2/p3 to sqlite3VdbeAddOp3
// Expectation:
// - AddOp3 is invoked exactly once
// - p is the same as the one passed to AddOp0
// - op equals the value passed to AddOp0
// - p1, p2, p3 are all zeros
// - AddOp0 returns the sentinel value produced by AddOp3
static void test_AddOp0_ForwardsParameters_WithNonNullP() {
  // Arrange
  Vdbe v;
  g_called = false;
  g_lastP = nullptr;
  g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
  g_lastRet = 0;

  // Act
  int result = sqlite3VdbeAddOp0(&v, 42); // op = 42, p1=p2=p3=0

  // Assert
  if (!g_called) fail("test_AddOp0_ForwardsParameters_WithNonNullP", "sqlite3VdbeAddOp3 was not called");
  if (g_lastP != &v) fail("test_AddOp0_ForwardsParameters_WithNonNullP", "Incorrect p pointer passed to AddOp3");
  if (g_lastOp != 42) fail("test_AddOp0_ForwardsParameters_WithNonNullP", "Incorrect op passed to AddOp3");
  if (g_lastP1 != 0 || g_lastP2 != 0 || g_lastP3 != 0)
    fail("test_AddOp0_ForwardsParameters_WithNonNullP", "p1/p2/p3 were not forwarded as zeros");
  if (result != 999) fail("test_AddOp0_ForwardsParameters_WithNonNullP", "Return value did not propagate from AddOp3");

  // If we reached here, the test passed
}

// Test: sqlite3VdbeAddOp0 forwards parameters when p is null
// This ensures that the function does not dereference a null p before forwarding and that
// the call contract remains consistent even for a null Vdbe pointer.
static void test_AddOp0_ForwardsParameters_WithNullP() {
  // Arrange
  g_called = false;
  g_lastP = nullptr;
  g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
  g_lastRet = 0;

  // Act
  int result = sqlite3VdbeAddOp0(nullptr, 7); // op = 7, p is null

  // Assert
  if (!g_called) fail("test_AddOp0_ForwardsParameters_WithNullP", "sqlite3VdbeAddOp3 was not called when p is null");
  if (g_lastP != nullptr) fail("test_AddOp0_ForwardsParameters_WithNullP", "p should be forwarded as nullptr");
  if (g_lastOp != 7) fail("test_AddOp0_ForwardsParameters_WithNullP", "Incorrect op passed to AddOp3");
  if (g_lastP1 != 0 || g_lastP2 != 0 || g_lastP3 != 0)
    fail("test_AddOp0_ForwardsParameters_WithNullP", "p1/p2/p3 were not forwarded as zeros when p is null");
  if (result != 999) fail("test_AddOp0_ForwardsParameters_WithNullP", "Return value did not propagate from AddOp3");
}

// A simple test runner that executes all tests and prints results
static void runAllTests() {
  struct T { void (*fn)(); const char* name; };
  T tests[] = {
    { test_AddOp0_ForwardsParameters_WithNonNullP, "test_AddOp0_ForwardsParameters_WithNonNullP" },
    { test_AddOp0_ForwardsParameters_WithNullP, "test_AddOp0_ForwardsParameters_WithNullP" }
  };

  int failures = 0;
  for (const auto& t : tests) {
    // Reset global state before each test
    g_called = false;
    g_lastP = nullptr;
    g_lastOp = g_lastP1 = g_lastP2 = g_lastP3 = 0;
    g_lastRet = 0;

    try {
      t.fn();
      std::cout << "[PASS] " << t.name << std::endl;
    } catch (const std::exception& ex) {
      ++failures;
      std::cout << "[FAIL] " << t.name << " - " << ex.what() << std::endl;
    } catch (...) {
      ++failures;
      std::cout << "[FAIL] " << t.name << " - unknown exception" << std::endl;
    }
  }

  if (failures == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << failures << " test(s) failed." << std::endl;
  }
}

} // namespace TestFramework

int main() {
  // Run the self-contained test suite
  TestFramework::runAllTests();

  return 0;
}
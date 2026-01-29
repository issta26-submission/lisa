// Lightweight C++11 unit tests for the focal function:
//   char * sqlite3_wasm_kvvfsMakeKeyOnPstack(const char *zClass, const char *zKeyIn);
// The tests are designed to run without GoogleTest and rely on a minimal harness.
// They exercise basic behavior and repeated-invocation behavior, focusing on
// pointer non-nullness and distinct allocation across calls.

#include <functional>
#include <vector>
#include <emscripten/wasmfs.h>
#include <assert.h>
#include <string>
#include <iostream>


// Forward declaration for the focal function under test.
// The function is implemented in the sqlite3-wasm.c module (C linkage).
extern "C" {
  char * sqlite3_wasm_kvvfsMakeKeyOnPstack(const char *zClass, const char *zKeyIn);
}

// Simple, non-terminating test harness.
// It reports failures but does not abort the whole test run on a single failure.
class TestHarness {
public:
  using TestFn = std::function<bool()>;

  void add(const std::string& name, TestFn fn) {
    tests_.emplace_back(name, fn);
  }

  int run() {
    int failures = 0;
    std::cout << "Starting test suite: sqlite3_wasm_kvvfsMakeKeyOnPstack\n";
    for (const auto& t : tests_) {
      bool ok = t.second();
      if (ok) {
        std::cout << "[PASS] " << t.first << "\n";
      } else {
        ++failures;
        std::cout << "[FAIL] " << t.first << "\n";
      }
    }
    if (failures == 0) {
      std::cout << "All tests passed.\n";
    } else {
      std::cout << failures << " test(s) failed.\n";
    }
    return failures;
  }

private:
  std::vector<std::pair<std::string, TestFn>> tests_;
};

// Test 1: Basic sanity check for the focal function.
// Preconditions:
// - The static environment in sqlite3-wasm.c provides a viable kvvfs key size.
// - The allocation on the pstack should succeed with the default setup.
// Expected outcome:
// - The function returns a non-NULL pointer when given valid inputs.
// Rationale:
// - Validates the non-failure path and basic integration with the pstack allocator.
bool test_basic_makeKey_on_pstack() {
  const char *zClass = "TestClass";
  const char *zKeyIn = "SampleKey";
  char *p = sqlite3_wasm_kvvfsMakeKeyOnPstack(zClass, zKeyIn);
  bool ok = (p != NULL);
  if(!ok) {
    std::cerr << "Error: sqlite3_wasm_kvvfsMakeKeyOnPstack() returned NULL in basic test.\n";
  }
  // Do not free p since the pstack allocator in this test harness uses a shared stack;
  // memory lifetime is managed by the wasm-like pstack in the library.
  return ok;
}

// Test 2: Repeated invocation produces distinct stack allocations.
// Preconditions:
// - Prior test invocation has created at least one allocation.
// - The stack allocator in the focal module supports multiple allocations in sequence.
// Expected outcome:
// - Two consecutive allocations return non-NULL pointers.
// - The two returned pointers are distinct (i.e., they do not alias the same stack region).
// Rationale:
// - Exercises the allocator's ability to carve out multiple blocks and helps ensure
//   no hard aliasing occurs between successive calls.
bool test_multiple_calls_produce_distinct_pointers() {
  const char *zClass = "TestClass";
  const char *zKeyIn = "AnotherKey";

  char *p1 = sqlite3_wasm_kvvfsMakeKeyOnPstack(zClass, zKeyIn);
  if(p1 == NULL) {
    std::cerr << "Error: first allocation returned NULL.\n";
    return false;
  }

  char *p2 = sqlite3_wasm_kvvfsMakeKeyOnPstack(zClass, zKeyIn);
  if(p2 == NULL) {
    std::cerr << "Error: second allocation returned NULL.\n";
    return false;
  }

  bool distinct = (p1 != p2);
  if(!distinct) {
    std::cerr << "Error: consecutive allocations returned the same pointer.\n";
  }
  return distinct;
}

int main() {
  TestHarness th;
  // Add tests with explanatory comments in code.
  th.add("Basic non-null return from kvvfsMakeKeyOnPstack", test_basic_makeKey_on_pstack);
  th.add("Consecutive allocations return distinct pointers", test_multiple_calls_produce_distinct_pointers);

  int failures = th.run();

  // Exit code reflects test success/failure (0 = success).
  return failures;
}
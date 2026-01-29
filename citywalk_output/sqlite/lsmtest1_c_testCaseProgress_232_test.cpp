// Unit test suite for the focal method testCaseProgress (located in lsmtest1.c)
// This test suite is designed to be compiled in a C++11 environment without GTest.
// It directly calls the C function via extern "C" linkage and uses a lightweight
// in-process test harness with non-terminating style assertions (as comments indicate).
// The tests focus on covering the computational branches of the while-loop predicate:
//
//   while ( iDot < ( ((nDot*2+1) * i) / (n*2) ) ) { ... iDot++; ... }
//
// Final state of piDot is either unchanged (if initial iDot >= threshold) or
// equal to the threshold (if initial iDot < threshold).

#include <iostream>
#include <lsmtest.h>
#include <string>


// Declare the C function under test with C linkage to avoid name mangling issues.
extern "C" void testCaseProgress(int i, int n, int nDot, int *piDot);

// Test 1: i = 0 => threshold = 0; starting iDot = 0; expect no progress (no dots printed)
static bool test_case_zero_threshold_no_change() {
  int dot = 0;
  testCaseProgress(0, 5, 3, &dot);
  // Non-terminating assertion: verify final state
  if (dot != 0) {
    std::cerr << "[FAIL] test_case_zero_threshold_no_change: expected dot == 0, got " << dot << std::endl;
    return false;
  }
  return true;
}

// Test 2: i > 0 with threshold > initial iDot; expect iDot to increment to threshold.
// For i=1, n=2, nDot=2: threshold = floor(( (2*2+1) * 1) / (2*2)) = floor(5/4) = 1
// starting dot = 0 -> final dot should be 1
static bool test_case_increment_once_when_threshold_reached() {
  int dot = 0;
  testCaseProgress(1, 2, 2, &dot);
  if (dot != 1) {
    std::cerr << "[FAIL] test_case_increment_once_when_threshold_reached: expected dot == 1, got " << dot << std::endl;
    return false;
  }
  return true;
}

// Test 3: Initial dot already >= threshold; expect no change to piDot.
// For i=1, n=2, nDot=0: threshold = floor(((0*2+1) * 1) / (2*2)) = floor(1/4) = 0
// Start with piDot = 5; final should remain 5
static bool test_case_no_change_when_initial_dot_ge_threshold() {
  int dot = 5;
  testCaseProgress(1, 2, 0, &dot);
  if (dot != 5) {
    std::cerr << "[FAIL] test_case_no_change_when_initial_dot_ge_threshold: expected dot == 5, got " << dot << std::endl;
    return false;
  }
  return true;
}

// Test 4: State persistence when threshold is not reached (iDot equals threshold after previous calls).
// For i=1, n=2, nDot=2: threshold = 1; initialize dot=1; no loop; final should still be 1
static bool test_case_state_persistence_when_threshold_not_reached() {
  int dot = 1;
  testCaseProgress(1, 2, 2, &dot);
  if (dot != 1) {
    std::cerr << "[FAIL] test_case_state_persistence_when_threshold_not_reached: expected dot == 1, got " << dot << std::endl;
    return false;
  }
  return true;
}

// Test 5: A larger i with a small threshold to verify that the computed threshold is used
// For i=3, n=4, nDot=2: threshold = floor(((2*2+1) * 3) / (4*2)) = floor(5*3/8) = floor(15/8) = 1
// Start with dot=0; final should be 1
static bool test_case_boundary_threshold_multiple_case() {
  int dot = 0;
  testCaseProgress(3, 4, 2, &dot);
  if (dot != 1) {
    std::cerr << "[FAIL] test_case_boundary_threshold_multiple_case: expected dot == 1, got " << dot << std::endl;
    return false;
  }
  return true;
}

int main() {
  struct Test {
    const char* name;
    bool (*func)();
  };

  Test tests[] = {
    {"zero_threshold_no_change", test_case_zero_threshold_no_change},
    {"increment_once_when_threshold_reached", test_case_increment_once_when_threshold_reached},
    {"no_change_when_initial_dot_ge_threshold", test_case_no_change_when_initial_dot_ge_threshold},
    {"state_persistence_when_threshold_not_reached", test_case_state_persistence_when_threshold_not_reached},
    {"boundary_threshold_multiple_case", test_case_boundary_threshold_multiple_case}
  };

  const int total = sizeof(tests) / sizeof(tests[0]);
  int passed = 0;

  // Run all tests
  for (int i = 0; i < total; ++i) {
    bool ok = tests[i].func();
    std::cout << (ok ? "[OK] " : "[FAIL] ") << tests[i].name << std::endl;
    if (ok) ++passed;
  }

  // Summary
  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

  // Return non-zero if any test failed
  return (passed == total) ? 0 : 1;
}
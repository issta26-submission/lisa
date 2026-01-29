// BeginCountRdtsc unit test suite (no GTest, simple C++11 harness)
// This test targets the focal method:
//   void BeginCountRdtsc(LARGE_INTEGER * pbeginTime64)
// As observed from the provided focal implementation, BeginCountRdtsc is effectively a no-op.
// The tests below verify that:
// - Passing a valid LARGE_INTEGER* does not modify the input value.
// - Passing a NULL pointer does not crash (i.e., the function is safe to call with NULL).

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>


// Declaration of the focal function with C linkage to match the original implementation.
// The function is defined in testzlib.c (C code), so we declare it extern "C" for linkage compatibility.
extern "C" void BeginCountRdtsc(LARGE_INTEGER * pbeginTime64);

// Lightweight, non-terminating test harness (does not abort on failure)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper: compare two 64-bit integers and report a non-terminating test result
static void EXPECT_EQ_LL(long long a, long long b, const char* msg) {
  g_tests_run++;
  if (a == b) {
    std::cout << "[PASS] " << msg << std::endl;
  } else {
    g_tests_failed++;
    std::cout << "[FAIL] " << msg << " | expected: " << b << ", actual: " << a << std::endl;
  }
}

// Helper: simple true/false assertion that never terminates the test suite
static void EXPECT_TRUE(bool cond, const char* msg) {
  g_tests_run++;
  if (cond) {
    std::cout << "[PASS] " << msg << std::endl;
  } else {
    g_tests_failed++;
    std::cout << "[FAIL] " << msg << std::endl;
  }
}

// Test 1: Ensure that BeginCountRdtsc does not modify the input LARGE_INTEGER when called with a valid pointer.
// This reflects the no-op behavior observed in the focal method's empty body.
static void test_BeginCountRdtsc_noop_does_not_modify_input() {
  // Prepare a known, non-zero value in LARGE_INTEGER
  LARGE_INTEGER li;
  li.QuadPart = 0x1122334455667788ULL;

  // Call the focal method
  BeginCountRdtsc(&li);

  // The QuadPart should remain unchanged after the call
  EXPECT_EQ_LL((long long)li.QuadPart, 0x1122334455667788ULL,
               "BeginCountRdtsc should not modify QuadPart of pbeginTime64");
}

// Test 2: Ensure that BeginCountRdtsc can be called with a NULL pointer without crashing.
// The function is expected to be a no-op, so a NULL argument must be safely accepted.
static void test_BeginCountRdtsc_handles_null_pointer() {
  // Call with NULL; if this returns without crash, the behavior is safe
  BeginCountRdtsc(nullptr);

  // We conservatively report success for this scenario
  EXPECT_TRUE(true, "BeginCountRdtsc accepts NULL pointer and performs no operation");
}

int main() {
  std::cout << "BeginCountRdtsc unit test suite (no GTest, simple harness)" << std::endl;

  // Run tests
  test_BeginCountRdtsc_noop_does_not_modify_input();
  test_BeginCountRdtsc_handles_null_pointer();

  // Summary
  std::cout << "Total tests run: " << g_tests_run
            << ", failures: " << g_tests_failed << std::endl;

  // Return non-zero if any test failed
  return g_tests_failed;
}
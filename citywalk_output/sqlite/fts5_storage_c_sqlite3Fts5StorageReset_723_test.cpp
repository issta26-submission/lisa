// Minimal C++11 test harness for the focal C function sqlite3Fts5StorageReset
// This test file provides lightweight tests without using GTest.
// It mocks the necessary dependencies locally and verifies that the storage reset
// correctly propagates the return value from sqlite3Fts5IndexReset(p->pIndex).

#include <iostream>
#include <fts5Int.h>


// Domain-specific: provide minimal class/structure definitions that mimic
// the dependencies in fts5_storage.c for the focal method.
// We keep these definitions lightweight and in the test translation unit only.

struct Fts5Index {};          // Forward-declare a minimal Fts5Index type
struct Fts5Storage {            // Fts5Storage with the dependent member pIndex
  Fts5Index *pIndex;
};

// -----------------------------------------------------------------------------
// Mock/stub of the external function sqlite3Fts5IndexReset.
// In the real project this would be implemented elsewhere; for testing we provide
// a controllable stub that records the last argument and returns a programmable value.
// -----------------------------------------------------------------------------

static Fts5Index *g_lastIndexResetArg = nullptr; // Tracks last argument passed to sqlite3Fts5IndexReset
static int g_nextIndexResetReturnValue = 0;       // Programmable return value for the next call

extern "C" int sqlite3Fts5IndexReset(Fts5Index *p) {    // Mocked dependency
  g_lastIndexResetArg = p;                            // Record the input for verification
  return g_nextIndexResetReturnValue;                 // Return the value we set for tests
}

// -----------------------------------------------------------------------------
// The focal method under test: sqlite3Fts5StorageReset
// This reproduces the exact logic from the provided snippet.
// int sqlite3Fts5StorageReset(Fts5Storage *p){
//   return sqlite3Fts5IndexReset(p->pIndex);
// }
// -----------------------------------------------------------------------------

int sqlite3Fts5StorageReset(Fts5Storage *p){
  return sqlite3Fts5IndexReset(p->pIndex);
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// Provides PASS/FAIL messages while continuing test execution.
// -----------------------------------------------------------------------------

static int g_totalTests = 0;
static int g_totalFailures = 0;

static void reportResult(const char* testName, const char* detail, bool pass) {
  ++g_totalTests;
  if (pass) {
    std::cout << "[PASS] " << testName << " - " << detail << "\n";
  } else {
    ++g_totalFailures;
    std::cout << "[FAIL] " << testName << " - " << detail << "\n";
  }
}

// Convenience assertion helper for integers
static void assertIntEq(int a, int b, const char* testName, const char* detail) {
  bool ok = (a == b);
  if (ok) {
    reportResult(testName, detail, true);
  } else {
    // Include actual vs expected in the failure detail to aid debugging
    std::cout << "[DEBUG]  Expected: " << b << ", Actual: " << a << "\n";
    reportResult(testName, detail, false);
  }
}

// Convenience assertion helper for pointers
static void assertPtrEq(const void* a, const void* b, const char* testName, const char* detail) {
  bool ok = (a == b);
  if (ok) {
    reportResult(testName, detail, true);
  } else {
    std::cout << "[DEBUG]  Expected ptr: " << b << ", Actual ptr: " << a << "\n";
    reportResult(testName, detail, false);
  }
}

// -----------------------------------------------------------------------------
// Test Cases
// Each test exercises a scenario for sqlite3Fts5StorageReset by manipulating
// the mocked global state and the storage's index pointer.
// -----------------------------------------------------------------------------

// Test 1: Basic propagation with a non-null index and a zero return value from the index reset
// Rationale: Ensure that the storage reset forwards the value returned by sqlite3Fts5IndexReset
// when the index pointer is valid.
static void test_reset_propagates_return_nonnull_index_zero() {
  // Prepare test objects
  Fts5Index idx;
  Fts5Storage s;
  s.pIndex = &idx;

  // Configure the mock to return 0 for the next call and record the argument
  g_nextIndexResetReturnValue = 0;
  g_lastIndexResetArg = nullptr;
  int ret = sqlite3Fts5StorageReset(&s);

  // Verify: return value is propagated and the index pointer passed correctly
  assertIntEq(ret, 0, "test_reset_propagates_return_nonnull_index_zero", "Return value should propagate (0)");
  assertPtrEq(g_lastIndexResetArg, &idx, "test_reset_propagates_return_nonnull_index_zero", "sqlite3Fts5IndexReset should be called with pIndex");
}

// Test 2: Propagation with non-zero return value from index reset and non-null index
static void test_reset_propagates_return_nonnull_index_nonzero() {
  Fts5Index idx;
  Fts5Storage s;
  s.pIndex = &idx;

  g_nextIndexResetReturnValue = 12345;
  g_lastIndexResetArg = nullptr;
  int ret = sqlite3Fts5StorageReset(&s);

  assertIntEq(ret, 12345, "test_reset_propagates_return_nonnull_index_nonzero", "Return value should propagate (12345)");
  assertPtrEq(g_lastIndexResetArg, &idx, "test_reset_propagates_return_nonnull_index_nonzero", "sqlite3Fts5IndexReset should be called with pIndex");
}

// Test 3: Null index pointer scenario
// Rationale: Ensure that the function forwards the call even if pIndex is null,
// and that the stub receives a null argument.
static void test_reset_with_null_index_pointer() {
  Fts5Storage s;
  s.pIndex = nullptr;

  g_nextIndexResetReturnValue = -7;
  g_lastIndexResetArg = reinterpret_cast<Fts5Index*>(0xDEADBEEF); // some non-null junk
  int ret = sqlite3Fts5StorageReset(&s);

  assertIntEq(ret, -7, "test_reset_with_null_index_pointer", "Return value should propagate (-7) when pIndex is null");
  assertPtrEq(g_lastIndexResetArg, nullptr, "test_reset_with_null_index_pointer", "sqlite3Fts5IndexReset should be called with nullptr");
}

// -----------------------------------------------------------------------------
// Main
// Execute all tests and summarize results.
// -----------------------------------------------------------------------------

int main() {
  std::cout << "Starting tests for sqlite3Fts5StorageReset...\n";

  test_reset_propagates_return_nonnull_index_zero();
  test_reset_propagates_return_nonnull_index_nonzero();
  test_reset_with_null_index_pointer();

  std::cout << "Tests completed. Total: " << g_totalTests
            << ", Failures: " << g_totalFailures << "\n";

  // Non-zero return indicates failure per typical test runners
  return (g_totalFailures != 0) ? 1 : 0;
}
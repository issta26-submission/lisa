// This test suite targets the focal method:
// int sqlite3Fts5StorageOptimize(Fts5Storage *p) {
//   return sqlite3Fts5IndexOptimize(p->pIndex);
// }
// The test uses a lightweight, in-source mock of sqlite3Fts5IndexOptimize to
// verify that the wrapper forwards the pIndex pointer correctly and returns the
// same return value as the underlying index optimizer.

// Assumptions and domain notes:
// - Fts5Storage and Fts5Index are defined in fts5Int.h (included below).
// - We provide a C-style mock for sqlite3Fts5IndexOptimize to intercept calls.
// - Tests are written in C++11 and do not rely on GoogleTest/GMock. A small
//   custom test harness with non-terminating EXPECT-like macros is used.
// - We cover both zero and non-zero return values from the index optimizer, and
//   a case where pIndex can be nullptr.

#include <iostream>
#include <cstdint>
#include <cassert>
#include <fts5Int.h>


// Import project dependencies (path may vary; adjust include path as needed)

// Domain knowledge notes embedded as comments for maintainability.
// Candidate Keywords: Fts5Storage, Fts5Index, pIndex, sqlite3Fts5IndexOptimize, wrapper,
// non-terminated assertions, C linkage, test harness.

// Mocking the dependency function sqlite3Fts5IndexOptimize.
// We provide a C linkage function so that the linker uses this mock in place of
// the real implementation during tests.
static int g_mockReturnValue = 0;
static Fts5Index *g_expectedIndex = nullptr;
static bool g_indexPassedOK = false;

// Forward declaration of the function under test (wrapper).
// The real function is in the C project; we call it directly from C++.
extern "C" int sqlite3Fts5StorageOptimize(Fts5Storage *p);

// C-style mock to intercept calls to sqlite3Fts5IndexOptimize
extern "C" int sqlite3Fts5IndexOptimize(Fts5Index *pIndex) {
  g_indexPassedOK = (pIndex == g_expectedIndex);
  return g_mockReturnValue;
}

// Lightweight test harness
static int g_testFailed = 0;

#define EXPECT_EQ(a, b, desc) do {                               \
  auto _a = (a);                                                   \
  auto _b = (b);                                                   \
  if (!(_a == _b)) {                                              \
    std::cerr << "TEST FAIL: " << (desc) << " | expected: "             \
              << _b << ", actual: " << _a << std::endl;          \
    ++g_testFailed;                                               \
  }                                                               \
} while (0)


// Test 1: When the index optimizer returns zero, the storage optimizer should return zero
// and pass the correct pIndex pointer to the index optimizer.
void test_StorageOptimize_ReturnsZero_ForIndexOptimizeZero() {
  // Prepare: index optimizer should return 0
  g_mockReturnValue = 0;

  // Prepare storage and index
  Fts5Storage storage{};      // zero-initialized
  Fts5Index idx{};
  storage.pIndex = &idx;

  // Expect the mock to receive the exact pointer
  g_expectedIndex = &idx;
  g_indexPassedOK = false;

  int result = sqlite3Fts5StorageOptimize(&storage);

  // Assertions
  EXPECT_EQ(result, 0, "Storage optimize should return 0 when index optimizer returns 0");
  EXPECT_EQ((int)g_indexPassedOK, 1, "sqlite3Fts5IndexOptimize should receive the correct pIndex pointer");
}


// Test 2: When the index optimizer returns a non-zero value, the storage optimizer should
// propagate that value and still pass the correct pIndex pointer.
void test_StorageOptimize_PropagatesNonZero_FromIndexOptimize() {
  // Prepare: index optimizer should return a non-zero value
  g_mockReturnValue = 5;

  // Prepare storage and index
  Fts5Storage storage{};
  Fts5Index idx{};
  storage.pIndex = &idx;

  // Expect the mock to receive the exact pointer
  g_expectedIndex = &idx;
  g_indexPassedOK = false;

  int result = sqlite3Fts5StorageOptimize(&storage);

  // Assertions
  EXPECT_EQ(result, 5, "Storage optimize should propagate non-zero return value from index optimizer");
  EXPECT_EQ((int)g_indexPassedOK, 1, "sqlite3Fts5IndexOptimize should receive the correct pIndex pointer");
}


// Test 3: When the pIndex is nullptr, the wrapper should still forward nullptr to the index optimizer
// and return the mock's value.
void test_StorageOptimize_WithNullIndexPointer() {
  // Prepare: index optimizer should return 2
  g_mockReturnValue = 2;

  // Prepare storage with a null pIndex
  Fts5Storage storage{};
  storage.pIndex = nullptr;

  // Expect the mock to receive nullptr
  g_expectedIndex = nullptr;
  g_indexPassedOK = false;

  int result = sqlite3Fts5StorageOptimize(&storage);

  // Assertions
  EXPECT_EQ(result, 2, "Storage optimize should propagate value when pIndex is nullptr");
  EXPECT_EQ((int)g_indexPassedOK, 1, "sqlite3Fts5IndexOptimize should receive nullptr when pIndex is nullptr");
}


// Helper: run all tests and report summary
void runAllTests() {
  test_StorageOptimize_ReturnsZero_ForIndexOptimizeZero();
  test_StorageOptimize_PropagatesNonZero_FromIndexOptimize();
  test_StorageOptimize_WithNullIndexPointer();
}


// Main entry point
int main() {
  // Start tests
  runAllTests();

  // Summary
  if (g_testFailed == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cerr << g_testFailed << " TEST(S) FAILED" << std::endl;
  }

  // Non-zero return code on failure to aid integration in CI pipelines
  return g_testFailed ? 1 : 0;
}
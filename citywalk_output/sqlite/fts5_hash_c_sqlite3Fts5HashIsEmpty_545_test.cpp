/*
Unit test suite for sqlite3Fts5HashIsEmpty (ft s5_hash.c)

Notes:
- This test is self-contained and does not rely on GTest or external test frameworks.
- We implement minimal stubs and the focal function in a single translation unit to ensure the test is self-sufficient.
- The test focuses on the true/false branches of the return predicate:
    - When nEntry == 0, the function should return true (non-zero test path).
    - When nEntry > 0, the function should return false.
- We exercise non-terminating assertions via custom EXPECT_* macros to allow full test execution without aborting on failures.
- Static helper function fts5HashCount is kept static to reflect file-scope static behavior as per the provided dependencies.
*/

#include <iostream>
#include <cassert>
#include <fts5Int.h>


// Candidate Keywords extracted from the focal method and its dependencies
// - Fts5Hash
// - nEntry
// - fts5HashCount
// - sqlite3Fts5HashIsEmpty

// Fts5Hash structure as required by sqlite3Fts5HashIsEmpty
struct Fts5Hash {
  int nEntry;
};

// Static function to count entries in the hash (mirrors static file-scope behavior)
static int fts5HashCount(Fts5Hash *pHash) {
  // In the real code this would count the actual entries.
  // For testing purposes, we mirror the field directly.
  return pHash ? pHash->nEntry : 0;
}

// Focal function under test
// int sqlite3Fts5HashIsEmpty(Fts5Hash *pHash){
int sqlite3Fts5HashIsEmpty(Fts5Hash *pHash){
  // The original asserts that the hash's nEntry matches the computed count.
  // We rely on the test's controlled environment to satisfy this condition.
  assert( pHash->nEntry==fts5HashCount(pHash) );
  return pHash->nEntry==0;
}

// Lightweight test framework (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
  ++g_total; \
  if(!(cond)) { \
    ++g_failed; \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - Expected true but got false. Condition: " #cond "\n"; \
  } \
} while(0)

#define EXPECT_FALSE(cond) do { \
  ++g_total; \
  if((cond)) { \
    ++g_failed; \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - Expected false but got true. Condition: " #cond "\n"; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  ++g_total; \
  if((static_cast<long long>(a)) != (static_cast<long long>(b))) { \
    ++g_failed; \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
              << " - Expected equality " << (a) << " == " << (b) << ".\n"; \
  } \
} while(0)

// Test 1: When nEntry is 0, sqlite3Fts5HashIsEmpty should return true
void test_IsEmpty_WhenZero() {
  // Arrange
  Fts5Hash h;
  h.nEntry = 0;

  // Act
  int result = sqlite3Fts5HashIsEmpty(&h);

  // Assert
  EXPECT_TRUE(result);                 // true branch
  EXPECT_EQ(result, (int)(h.nEntry==0)); // consistency check with the predicate
}

// Test 2: When nEntry is non-zero, sqlite3Fts5HashIsEmpty should return false
void test_IsEmpty_WhenNonZero() {
  // Arrange
  Fts5Hash h;
  h.nEntry = 3;

  // Act
  int result = sqlite3Fts5HashIsEmpty(&h);

  // Assert
  EXPECT_FALSE(result);                // false branch
  EXPECT_EQ(result, (int)(h.nEntry==0)); // consistency check with the predicate
}

// Main function to run tests
int main() {
  test_IsEmpty_WhenZero();
  test_IsEmpty_WhenNonZero();

  if(g_failed > 0){
    std::cout << "\nTests completed with failures: " << g_failed
              << " out of " << g_total << " tests.\n";
    return 1;
  } else {
    std::cout << "\nAll tests passed: " << g_total << " tests.\n";
    return 0;
  }
}
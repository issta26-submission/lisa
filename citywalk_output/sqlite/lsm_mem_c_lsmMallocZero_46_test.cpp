// Unit test suite for lsmMallocZero focusing on C++11 without GTest.
// This test suite provides a minimal test harness and two test cases:
// 1) When allocation succeeds, the allocated memory is zeroed.
// 2) When allocation fails, lsmMallocZero returns nullptr and does not touch memory.
// The focal function and its dependent lsmMalloc are implemented here to enable isolated testing.

#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASSED() \
  do { ++g_total_tests; } while(0)

#define TEST_FAILED(msg) \
  do { ++g_total_tests; ++g_failed_tests; std::cerr << "TEST FAILED: " << msg << std::endl; } while(0)

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { TEST_FAILED(msg); } else { TEST_PASSED(); } } while(0)

// Domain-specific types (minimal stand-ins)
struct lsm_env { };

// Forward declaration of the allocations function under test (mocked in this test)
static bool g_failMalloc = false;

// Mocked lsmMalloc to control allocation success/failure in tests
static void *lsmMalloc(lsm_env *pEnv, size_t N) {
  (void)pEnv; // unused in mock
  if(g_failMalloc) return nullptr;
  // Use standard malloc to simulate real allocation
  return std::malloc(N);
}

// Focal method under test: lsmMallocZero
static void *lsmMallocZero(lsm_env *pEnv, size_t N){
  void *pRet;
  assert( pEnv );
  pRet = lsmMalloc(pEnv, N);
  if( pRet ) std::memset(pRet, 0, N);
  return pRet;
}

// Test 1: Allocation succeeds and the memory is zero-initialized
// - Arrange: ensure lsmMalloc returns a valid non-null pointer
// - Act: call lsmMallocZero with a non-zero size
// - Assert: result is non-null and all bytes are zero
static void test_LsmMallocZero_SetsMemoryToZero_OnSuccess() {
  // Arrange
  lsm_env env;
  g_failMalloc = false; // simulate successful allocation

  // Act
  size_t N = 64;
  void *p = lsmMallocZero(&env, N);

  // Assert
  if(p == nullptr) {
    TEST_FAILED("Expected non-null pointer when allocation succeeds, got null.");
  } else {
    unsigned char *bytes = static_cast<unsigned char*>(p);
    bool allZero = true;
    for(size_t i = 0; i < N; ++i) {
      if(bytes[i] != 0) { allZero = false; break; }
    }
    EXPECT_TRUE(allZero, "Memory should be zero-initialized by lsmMallocZero.");
  }

  // Cleanup
  if(p != nullptr) std::free(p);
}

// Test 2: Allocation fails and lsmMallocZero returns nullptr without touching memory
// - Arrange: simulate allocation failure by toggling the mock flag
// - Act: call lsmMallocZero
// - Assert: result is null and nothing is accessed
static void test_LsmMallocZero_ReturnsNull_OnFailure() {
  // Arrange
  lsm_env env;
  g_failMalloc = true; // simulate allocation failure

  // Act
  size_t N = 32;
  void *p = lsmMallocZero(&env, N);

  // Assert
  EXPECT_TRUE(p == nullptr, "Expected null pointer when allocation fails, got non-null.");

  // Cleanup
  // No allocation occurred; nothing to free
  // Reset state for subsequent tests
  g_failMalloc = false;
}

// Main function: run tests and report summary
int main() {
  std::cout << "Starting unit tests for lsmMallocZero...\n";

  test_LsmMallocZero_SetsMemoryToZero_OnSuccess();
  test_LsmMallocZero_ReturnsNull_OnFailure();

  // Summary
  std::cout << "Tests run: " << g_total_tests << std::endl;
  if(g_failed_tests == 0) {
    std::cout << "All tests PASSED." << std::endl;
    return 0;
  } else {
    std::cout << "Tests FAILED: " << g_failed_tests << " of " << g_total_tests << std::endl;
    return 1;
  }
}
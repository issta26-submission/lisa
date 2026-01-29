/*
 * Lightweight C++11 test harness for the focal method lsmMallocRc.
 * This test is designed to be self-contained (no GTest) and uses
 * a small, non-terminating assertion mechanism to exercise
 * the exact control-flow branches of lsmMallocRc as described.
 *
 * Note: The test redefines a minimal subset of the dependencies
 * (lsm_env, LSM_OK, LSM_NOMEM_BKPT, etc.) and provides a stub
 * implementation of lsmMalloc to precisely control allocation success/failure.
 * The goal is to validate the behavior of lsmMallocRc in isolation.
 *
 * The tests mirror the scenarios described in the problem statement:
 *  - Branch where pRc == LSM_OK and allocation succeeds
 *  - Branch where pRc == LSM_OK and allocation fails
 *  - Branch where pRc != LSM_OK (no allocation attempted)
 *  - Edge case with N == 0
 *  - Edge case with null environment pointer (pEnv)
 *
 * All tests are non-terminating: failures are reported to std::cerr
 * but do not abort program execution.
 */

#include <cstddef>
#include <lsmInt.h>
#include <iostream>
#include <cstdint>


// Minimal domain knowledge / stubs to mimic the project environment
// (these would normally come from the project's headers)
struct lsm_env { /* opaque in actual project */ };

// Error/status codes (subset necessary for tests)
static const int LSM_OK        = 0;
static const int LSM_NOMEM_BKPT = 1;
static const int LSM_ERROR     = 2;

// Global state to control the mock allocator behavior
static bool g_allocShouldFail = false;  // when true, lsmMalloc returns nullptr
static int  g_allocCallCount  = 0;      // number of times lsmMalloc was invoked

// Mock allocator to drive lsmMallocRc behavior
void* lsmMalloc(lsm_env* /*pEnv*/, size_t /*N*/) {
    ++g_allocCallCount;
    if (g_allocShouldFail) {
        return nullptr; // simulate out-of-memory
    }
    // Return a non-null dummy pointer to simulate success.
    // The actual value is not important; only non-nullness matters.
    static int dummy;
    return &dummy;
}

// Focal method under test (replicated here for isolated unit testing).
// In the real project, this would be compiled from lsm_mem.c.
void *lsmMallocRc(lsm_env *pEnv, size_t N, int *pRc){
  void *pRet = 0;
  if( *pRc==LSM_OK ){
    pRet = lsmMalloc(pEnv, N);
    if( pRet==0 ){
      *pRc = LSM_NOMEM_BKPT;
    }
  }
  return pRet;
}

// Simple non-terminating assertion helpers
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_BEGIN(name) \
    std::cout << "[TEST] " name " ... " << std::endl; \
    g_totalTests++;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            ++g_failedTests; \
            std::cerr << "  [FAIL] " msg " | Condition failed: " #cond \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_EQ(expected, actual, msg) \
    do { \
        if(!((expected) == (actual))) { \
            ++g_failedTests; \
            std::cerr << "  [FAIL] " msg " | Expected: " << (expected) \
                      << ", Actual: " << (actual) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_PTR_NOT_NULL(ptr, msg) \
    do { \
        if((ptr) == nullptr) { \
            ++g_failedTests; \
            std::cerr << "  [FAIL] " msg " | Pointer is null." \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_PTR_NULL(ptr, msg) \
    do { \
        if((ptr) != nullptr) { \
            ++g_failedTests; \
            std::cerr << "  [FAIL] " msg " | Pointer is not null." \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

static void test_allocation_succeeds_branch()
{
  // Test 1: pRc initially LSM_OK, allocation succeeds
  TEST_BEGIN("lsmMallocRc: rc==LSM_OK, allocation success");

  g_allocCallCount = 0;
  g_allocShouldFail = false;

  int rc = LSM_OK;
  void* p = lsmMallocRc(nullptr, 64, &rc);

  // Expectations:
  // - p should be non-null
  // - rc should remain LSM_OK
  // - lsmMalloc should have been called exactly once
  EXPECT_PTR_NOT_NULL(p, "Allocation should succeed when allocator reports success (non-null)");
  EXPECT_EQ(LSM_OK, rc, "rc should remain LSM_OK after successful allocation");
  EXPECT_EQ(1, g_allocCallCount, "lsmMalloc should be invoked exactly once on success");
}

static void test_allocation_fails_branch()
{
  // Test 2: pRc initially LSM_OK, allocation fails
  TEST_BEGIN("lsmMallocRc: rc==LSM_OK, allocation failure");

  g_allocCallCount = 0;
  g_allocShouldFail = true;

  int rc = LSM_OK;
  void* p = lsmMallocRc(nullptr, 128, &rc);

  // Expectations:
  // - p should be null
  // - rc should be set to LSM_NOMEM_BKPT
  EXPECT_PTR_NULL(p, "Allocation should fail and return null when allocator fails");
  EXPECT_EQ(LSM_NOMEM_BKPT, rc, "rc should be updated to LSM_NOMEM_BKPT on allocation failure");
  EXPECT_EQ(1, g_allocCallCount, "lsmMalloc should be invoked exactly once on failure");
}

static void test_no_allocation_when_rc_not_ok()
{
  // Test 3: pRc != LSM_OK, no allocation should be attempted
  TEST_BEGIN("lsmMallocRc: rc != LSM_OK, no allocation attempted");

  g_allocCallCount = 0;
  g_allocShouldFail = false;

  int rc = LSM_ERROR; // not LSM_OK
  void* p = lsmMallocRc(nullptr, 256, &rc);

  // Expectations:
  // - p should be null
  // - rc should remain unchanged
  // - lsmMalloc should not have been called
  EXPECT_PTR_NULL(p, "No allocation should occur when rc is not LSM_OK");
  EXPECT_EQ(LSM_ERROR, rc, "rc should remain unchanged when rc != LSM_OK");
  EXPECT_EQ(0, g_allocCallCount, "lsmMalloc should not be called when rc != LSM_OK");
}

static void test_allocation_with_zero_size()
{
  // Test 4: N == 0, allocation should still be attempted and succeed (depending on allocator)
  TEST_BEGIN("lsmMallocRc: N==0 with successful allocation");

  g_allocCallCount = 0;
  g_allocShouldFail = false;

  int rc = LSM_OK;
  void* p = lsmMallocRc(nullptr, 0, &rc);

  // Expectations:
  // - p should be non-null (allocator returns success)
  // - rc should remain LSM_OK
  EXPECT_PTR_NOT_NULL(p, "Allocation with N==0 should succeed when allocator reports success");
  EXPECT_EQ(LSM_OK, rc, "rc should remain LSM_OK after zero-size allocation");
  EXPECT_EQ(1, g_allocCallCount, "lsmMalloc should be invoked once for N==0");
}

static void test_allocation_with_null_env_and_success()
{
  // Test 5: Non-null rc path with null env should still behave as per lsmMallocRc semantics
  TEST_BEGIN("lsmMallocRc: null pEnv handling with success");

  g_allocCallCount = 0;
  g_allocShouldFail = false;

  int rc = LSM_OK;
  void* p = lsmMallocRc(nullptr, 42, &rc);

  // Expectations:
  // - p should be non-null
  // - rc should remain LSM_OK
  EXPECT_PTR_NOT_NULL(p, "Null pEnv should not affect successful allocation");
  EXPECT_EQ(LSM_OK, rc, "rc should stay LSM_OK after successful allocation with null pEnv");
  EXPECT_EQ(1, g_allocCallCount, "lsmMalloc should be invoked once even with null pEnv");
}

int main()
{
  // Run all tests
  test_allocation_succeeds_branch();
  test_allocation_fails_branch();
  test_no_allocation_when_rc_not_ok();
  test_allocation_with_zero_size();
  test_allocation_with_null_env_and_success();

  // Summary
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Total tests run  : " << g_totalTests << std::endl;
  std::cout << "Total tests failed: " << g_failedTests << std::endl;
  if (g_failedTests == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
  }
  return g_failedTests ? 1 : 0;
}
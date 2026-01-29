/*
Unit test suite for lsmReallocOrFreeRc (C-style function) implemented in this test
translation unit to enable self-contained testing without GTest.

Test goals:
- Verify behavior when pRc is already non-zero: free is invoked and no reallocation occurs.
- Verify behavior when pRc is zero and lsmReallocOrFree succeeds: rc remains unchanged and a non-null pointer is returned.
- Verify behavior when pRc is zero and lsmReallocOrFree fails: rc is set to LSM_NOMEM_BKPT and a null pointer is returned.

Test infrastructure:
- Minimal mock environment for lsm_env and the dependency functions lsmFree and lsmReallocOrFree.
- Lightweight non-terminating assertion macros (EXPECT_*) suitable for C++11, printing failures but continuing execution.
- All code contained in a single translation unit for portability.

Notes:
- The provided focal method code is replicated here for testing purposes.
- LSM_NOMEM_BKPT macro is defined to a fixed sentinel value for assertions.
*/

#include <lsmInt.h>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cstdint>


// Domain knowledge macro (sentinel used for test expectations)
#define LSM_NOMEM_BKPT 12345

// Faked environment type (as used by the real code)
struct lsm_env {};

// Forward declarations for dependencies used by lsmReallocOrFreeRc
void lsmFree(lsm_env* pEnv, void* p);
void *lsmReallocOrFree(lsm_env* pEnv, void* p, size_t N);

// The focal function under test (reproduced in this test file)
void *lsmReallocOrFreeRc(lsm_env *pEnv, void *p, size_t N, int *pRc){
  void *pRet = 0;
  if( *pRc ){
    lsmFree(pEnv, p);
  }else{
    pRet = lsmReallocOrFree(pEnv, p, N);
    if( !pRet ) *pRc = LSM_NOMEM_BKPT;
  }
  return pRet;
}

// ---------- Test harness infrastructure ----------

// Simple non-terminating assertion macros (mock GTest-like behavior)
static int gFailures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << (msg) \
              << " [FILE=" << __FILE__ << ", LINE=" << __LINE__ << "]\n"; \
    ++gFailures; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  if(!((a) == (b))) { \
    std::cerr << "EXPECT_EQ failed: " << (msg) \
              << " (expected: " << (b) << ", actual: " << (a) << ")" \
              << " [FILE=" << __FILE__ << ", LINE=" << __LINE__ << "]\n"; \
    ++gFailures; \
  } \
} while(0)

static bool g_reallocShouldReturnNull = false;   // controls lsmReallocOrFree behavior
static bool g_freeCalled = false;
static void* g_freedPtr = nullptr;
static void* g_reallocReturnPtr = reinterpret_cast<void*>(0xDEADBEEF);
static int g_reallocCallCount = 0;
static int g_freeCallCount = 0;

// Reset mock state before each test
static void resetMocks() {
  g_reallocShouldReturnNull = false;
  g_freeCalled = false;
  g_freedPtr = nullptr;
  g_reallocReturnPtr = reinterpret_cast<void*>(0xDEADBEEF);
  g_reallocCallCount = 0;
  g_freeCallCount = 0;
}

// Mock implementations of dependencies

// lsmFree mock: records that it was called and with which pointer
void lsmFree(lsm_env* /*pEnv*/, void* p) {
  g_freeCalled = true;
  g_freeCallCount++;
  g_freedPtr = p;
}

// lsmReallocOrFree mock: increments call count and returns either a non-null pointer or null
void *lsmReallocOrFree(lsm_env* /*pEnv*/, void* /*p*/, size_t /*N*/) {
  ++g_reallocCallCount;
  if (g_reallocShouldReturnNull) {
    return nullptr;
  }
  // Return a deterministic non-null pointer for testing
  return g_reallocReturnPtr;
}

// ---------- Test cases ----------

// Test 1: When *pRc is non-zero, lsmFree should be invoked and a null return should occur
void test_case_rc_true_calls_free_and_returns_null() {
  // Setup
  resetMocks();
  lsm_env env;
  void* p = reinterpret_cast<void*>(0x1001);
  int rc = 1; // non-zero

  // Exercise
  void *ret = lsmReallocOrFreeRc(&env, p, 64, &rc);

  // Verify
  EXPECT_TRUE(ret == nullptr, "rc non-zero should yield null return");
  EXPECT_TRUE(g_freeCalled, "lsmFree should be called when *pRc is non-zero");
  EXPECT_EQ(g_freedPtr, p, "lsmFree should be called with the original pointer");
  EXPECT_EQ(g_reallocCallCount, 0, "lsmReallocOrFree should not be called when *pRc is non-zero");
  // rc should remain unchanged (non-zero)
  EXPECT_TRUE(rc != 0, "rc should remain non-zero after free path");
}

// Test 2: When *pRc is zero and lsmReallocOrFree succeeds, return non-null and rc remains zero
void test_case_rc_zero_realloc_succeeds_returns_pointer_no_rc_change() {
  // Setup
  resetMocks();
  lsm_env env;
  void* p = reinterpret_cast<void*>(0x2002);
  int rc = 0; // zero

  g_reallocShouldReturnNull = false; // simulate success
  g_reallocReturnPtr = reinterpret_cast<void*>(0xBEEFCAFE);

  // Exercise
  void *ret = lsmReallocOrFreeRc(&env, p, 128, &rc);

  // Verify
  EXPECT_TRUE(ret == g_reallocReturnPtr, "realloc should return non-null pointer on success");
  EXPECT_EQ(rc, 0, "rc should remain zero when realloc succeeds");
  EXPECT_FALSE(g_freeCalled, "lsmFree should not be called on realloc success");
  EXPECT_EQ(g_reallocCallCount, 1, "lsmReallocOrFree should be called exactly once");
}

// Test 3: When *pRc is zero and lsmReallocOrFree fails, set rc to NOMEM_BKPT and return null
void test_case_rc_zero_realloc_fails_sets_rc_and_returns_null() {
  // Setup
  resetMocks();
  lsm_env env;
  void* p = reinterpret_cast<void*>(0x3003);
  int rc = 0; // zero

  g_reallocShouldReturnNull = true; // simulate failure (returns null)
  g_reallocReturnPtr = reinterpret_cast<void*>(0xDEAD0000); // won't be used

  // Exercise
  void *ret = lsmReallocOrFreeRc(&env, p, 256, &rc);

  // Verify
  EXPECT_TRUE(ret == nullptr, "realloc failure should return null");
  EXPECT_EQ(rc, LSM_NOMEM_BKPT, "rc should be set to LSM_NOMEM_BKPT on realloc failure");
  EXPECT_FALSE(g_freeCalled, "lsmFree should not be called when realloc fails");
  EXPECT_EQ(g_reallocCallCount, 1, "lsmReallocOrFree should be called exactly once on this path");
}

// ---------- Main test runner ----------

int main() {
  // Run tests
  test_case_rc_true_calls_free_and_returns_null();
  test_case_rc_zero_realloc_succeeds_returns_pointer_no_rc_change();
  test_case_rc_zero_realloc_fails_sets_rc_and_returns_null();

  // Summary
  if (gFailures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << gFailures << " test(s) FAILED\n";
  }
  return gFailures;
}
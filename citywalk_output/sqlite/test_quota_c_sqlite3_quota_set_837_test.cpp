// cpp_test_quota_set.cpp
// A lightweight C++11 test suite for sqlite3_quota_set without GTest.
// This test focuses on behavior of quota group creation, updates, and
// destructor invocation when pArg changes across sqlite3_quota_set calls.
// The tests are designed to run in-process against the quota subsystem
// provided in the accompanying C sources (no private/private access required).

#include <string.h>
#include <assert.h>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Ensure C linkage for the C quota API we're testing.
extern "C" {

// Minimal typedef to match sqlite3_int64 used by sqlite3_quota_set.
// If sqlite3.h is available, you can switch to that typedef; here we
// rely on a portable 64-bit signed integer.
typedef int64_t sqlite3_int64;

// Forward declarations of quota subsystem public API used by tests.
// These are declared with C linkage to match the implementation.
int sqlite3_quota_initialize(const char *zOrigVfsName, int makeDefault);
int sqlite3_quota_shutdown(void);
typedef void (*QuotaCallback)(const char *zFilename, sqlite3_int64 *piLimit, sqlite3_int64 iSize, void *pArg);
typedef void (*QuotaDestroy)(void*);
int sqlite3_quota_set(
  const char *zPattern,
  sqlite3_int64 iLimit,
  QuotaCallback xCallback,
  void *pArg,
  QuotaDestroy xDestroy
);
}

// Test infrastructure

static int gDestructCount = 0;

// A no-op callback used in tests except for destructor invocation testing.
static void dummyQuotaCallback(const char *zFilename, sqlite3_int64 *piLimit, sqlite3_int64 iSize, void *pArg){
  (void)zFilename;
  (void)piLimit;
  (void)iSize;
  (void)pArg;
  // Intentionally left empty; tests do not rely on callback side effects.
}

// A destructor that frees a heap-allocated int and counts invocations.
// This allows us to verify that old pArg pointers are freed when replaced.
static void testDestructor(void *pArg){
  if(pArg){
    int *p = (int*)pArg;
    // Free the allocated memory (if any)
    delete p;
  }
  ++gDestructCount;
}

// Simple test harness helpers
#define SQLITE_OK 0

static int gTotalFailures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    ++gTotalFailures; \
    std::cerr << "FAIL: " msg "\n"; \
  } else { \
    std::cout << "PASS: " msg "\n"; \
  } \
} while(0)

static void test_basic_noop_zero_limit(){
  // Ensure clean state
  sqlite3_quota_shutdown();
  sqlite3_quota_initialize(nullptr, 0);

  // When iLimit <= 0 and there is no existing group, nothing should be created
  // and function should return SQLITE_OK.
  int rc = sqlite3_quota_set("patternA", 0, dummyQuotaCallback, nullptr, nullptr);

  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_quota_set with zero limit returns SQLITE_OK (no group created)");
}

static void test_create_group_and_replace_arg_destructor_behavior(){
  // This test covers creation of a new quota group and ensures that replacing
  // the pArg with a new value triggers the configured xDestroy callback for the
  // old pArg (if provided).

  // Start fresh
  sqlite3_quota_shutdown();
  sqlite3_quota_initialize(nullptr, 0);

  // First call: create a group for "patternD" with a non-zero limit.
  int *arg1 = new int(1);
  gDestructCount = 0;
  int rc1 = sqlite3_quota_set("patternD", 100, dummyQuotaCallback, arg1, testDestructor);
  EXPECT_TRUE(rc1 == SQLITE_OK, "First sqlite3_quota_set creates group for patternD");
  // No destruction should have happened yet.
  EXPECT_TRUE(gDestructCount == 0, "ConstructorArg not destroyed on first insert");

  // Second call: replace pArg with a new pointer. The old pArg should be destroyed.
  int *arg2 = new int(2);
  int rc2 = sqlite3_quota_set("patternD", 200, dummyQuotaCallback, arg2, testDestructor);
  EXPECT_TRUE(rc2 == SQLITE_OK, "Second sqlite3_quota_set updates existing patternD and destroys old pArg");
  // Destructor should have been invoked for arg1.
  EXPECT_TRUE(gDestructCount == 1, "Old pArg destroyed after replacement");

  // Third call: replace again with NULL pArg to ensure destructor is invoked for arg2.
  int rc3 = sqlite3_quota_set("patternD", 0, dummyQuotaCallback, nullptr, testDestructor);
  EXPECT_TRUE(rc3 == SQLITE_OK, "Third sqlite3_quota_set with NULL pArg destroys current pArg (arg2)");
  EXPECT_TRUE(gDestructCount == 2, "Current pArg destroyed when replaced with NULL");
}

static void test_multiple_patterns_creation(){
  // Reset state and create two independent quota groups to verify
  // that distinct patterns can be managed concurrently.

  sqlite3_quota_shutdown();
  sqlite3_quota_initialize(nullptr, 0);

  int rc1 = sqlite3_quota_set("patternA_multi", 25, dummyQuotaCallback, nullptr, nullptr);
  int rc2 = sqlite3_quota_set("patternB_multi", 50, dummyQuotaCallback, nullptr, nullptr);

  EXPECT_TRUE(rc1 == SQLITE_OK, "sqlite3_quota_set creates group for patternA_multi");
  EXPECT_TRUE(rc2 == SQLITE_OK, "sqlite3_quota_set creates group for patternB_multi");

  // Sanity: setting zero for an existing pattern should still return OK without crash.
  int rc3 = sqlite3_quota_set("patternA_multi", 0, dummyQuotaCallback, nullptr, nullptr);
  EXPECT_TRUE(rc3 == SQLITE_OK, "sqlite3_quota_set with zero limit on existing pattern returns OK");
}

static void test_zero_limit_existing_group_noop(){
  // Confirm that if a group exists and we pass iLimit = 0, the call completes OK.
  sqlite3_quota_shutdown();
  sqlite3_quota_initialize(nullptr, 0);

  int rc1 = sqlite3_quota_set("patternX_zero", 10, dummyQuotaCallback, nullptr, nullptr);
  EXPECT_TRUE(rc1 == SQLITE_OK, "Create group for patternX_zero");

  int rc2 = sqlite3_quota_set("patternX_zero", 0, dummyQuotaCallback, nullptr, nullptr);
  EXPECT_TRUE(rc2 == SQLITE_OK, "Setting zero limit on existing group returns OK");
}

int main(){
  // Run test suite
  std::cout << "Starting sqlite3_quota_set unit tests (C++ harness, no GTest)..." << std::endl;

  test_basic_noop_zero_limit();
  test_create_group_and_replace_arg_destructor_behavior();
  test_multiple_patterns_creation();
  test_zero_limit_existing_group_noop();

  if( gTotalFailures == 0 ){
    std::cout << "ALL TESTS PASSED" << std::endl;
  }else{
    std::cout << gTotalFailures << " TEST(S) FAILED" << std::endl;
  }

  // Final cleanup
  sqlite3_quota_shutdown();
  return (gTotalFailures == 0) ? 0 : 1;
}
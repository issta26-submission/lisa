// C++11 unit test suite for the focal method: testMallocUninstall(lsm_env*)
// This test is crafted as a lightweight, no-GTest style suite.
// It exercises the behavior of testMallocUninstall, focusing on the key
// side-effects: restoring allocator hooks and invoking the mutex delete hook.

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <lsm.h>


// =====================================================================================
// Minimal C declarations to allow linking with the C implementation of
// testMallocUninstall(lsm_env*).
//
// Note: We provide only the parts necessary for compilation and testability within
// this standalone C++ test harness. The real project defines these in lsm.h and the
// corresponding C source file; we mirror the essential layout here to enable
// interaction with testMallocUninstall without bringing the full project into the test.
// =====================================================================================

// Forward declare the C-style environment struct used by testMallocUninstall.
extern "C" {

// Lightweight representation of lsm_env with only the fields used by testMallocUninstall.
// The real environment in the project has more fields, but this suffices for unit testing
// the behavior of testMallocUninstall's interactions.
struct lsm_env {
  void *pMemCtx;
  // Allocator function pointers used by the environment. Types intentionally kept
  // simple to match the usage in testMallocUninstall's assignment.
  void *(*xMalloc)(int);
  void *(*xRealloc)(void *, int);
  void  (*xFree)(void *);
};

// Prototype of the focal function (C linkage to avoid C++ name mangling issues)
void testMallocUninstall(lsm_env *pEnv);

}

// =====================================================================================
// Mock/simple implementations to drive the test behavior
// =====================================================================================

static bool g_delMutex_called; // Tracks whether the mutex deletion hook was invoked.

struct TmGlobal; // Forward declare to declare the mutex wrapper function below.

// Mutex deletion hook used by testMallocUninstall to signal it was called.
static void test_mutex_del(TmGlobal * /*pTm*/) {
  g_delMutex_called = true;
}

// Saved allocator hooks to be restored into lsm_env during uninstall.
// They mimic a typical malloc/realloc/free trio.
static void *saved_malloc(int n) {
  return malloc(n);
}
static void *saved_realloc(void *ptr, int n) {
  return realloc(ptr, n);
}
static void saved_free(void *ptr) {
  free(ptr);
}

// Dummy (no-op) allocators to initialize the environment before uninstall.
// They are placeholders to verify that after uninstall, env.xMalloc/xRealloc/xFree
// switch away from dummy values to the saved ones in the TmGlobal structure.
static void *dummy_malloc(int /*n*/) { return nullptr; }
static void *dummy_realloc(void * /*p*/, int /*n*/) { return nullptr; }
static void dummy_free(void * /*p*/) {}

// =====================================================================================
// Lightweight TmGlobal layout mirroring the parts of the real structure used by testMallocUninstall.
// This layout is crafted to be compatible with how testMallocUninstall reads
// xSaveMalloc/xSaveRealloc/xSaveFree and xDelMutex from the provided pMemCtx.
// It intentionally omits unrelated members to keep the test self-contained.
struct TmBlockHdr {
  struct TmBlockHdr *pNext;
  struct TmBlockHdr *pPrev;
  int nByte;
  unsigned int iForeGuard;
};

struct TmGlobal {
  TmBlockHdr *pFirst;
  // In non-defines TM_BACKTRACE builds, these fields exist and are used by testMallocUninstall.
  void *(*xMalloc)(int);
  void *(*xRealloc)(void *, int);
  void (*xFree)(void *);
  void (*xEnterMutex)(TmGlobal*);
  void (*xLeaveMutex)(TmGlobal*);
  void (*xDelMutex)(TmGlobal*);
  void *pMutex;
  void *(*xSaveMalloc)(void *, size_t);
  void *(*xSaveRealloc)(void *, void *, size_t);
  void (*xSaveFree)(void *, void *);
  int nCountdown;
  int bPersist;
  int bEnable;
  void (*xHook)(void *);
  void *pHookCtx;
};

// =====================================================================================
// Test harness
// =====================================================================================

// Simple, non-terminating assertion mechanism.
// Keeps running tests even if one fails, but records failures for summary.
static int g_nTests = 0;
static int g_nFailures = 0;

#define TEST_OK(message) do { \
  printf("[OK]   %s\n", message); \
} while(0)

#define TEST_FAIL(message) do { \
  fprintf(stderr, "[FAIL] %s\n", message); \
  g_nFailures++; \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
  g_nTests++; \
  if(!(cond)) { TEST_FAIL(msg); } else { TEST_OK(msg); } \
} while(0)


// Wrapper to call the focal function with our test-lsm_env layout.
// We declare the prototype above in extern "C" block; here we call it directly.
static void run_test_basic_uninstall() {
  // Reset global test state
  g_delMutex_called = false;

  // Prepare a TmGlobal instance that will serve as the pMemCtx for the environment.
  TmGlobal tm;
  // Minimal initialization; only the fields accessed by testMallocUninstall are populated.
  tm.pFirst = nullptr;
  tm.xMalloc = nullptr;
  tm.xRealloc = nullptr;
  tm.xFree = nullptr;
  tm.xEnterMutex = nullptr;
  tm.xLeaveMutex = nullptr;
  tm.xDelMutex = test_mutex_del; // real hook to verify it's invoked
  tm.pMutex = nullptr;
  // Saved allocators that should be restored into the env
  tm.xSaveMalloc = saved_malloc;
  tm.xSaveRealloc = saved_realloc;
  tm.xSaveFree = saved_free;
  tm.nCountdown = 0;
  tm.bPersist = 0;
  tm.bEnable = 0;
  tm.xHook = nullptr;
  tm.pHookCtx = nullptr;

  // Set up the environment pointing to our tm context
  lsm_env env;
  env.pMemCtx = &tm;
  // Initial dummy allocators in the environment surface to verify they get replaced.
  env.xMalloc = dummy_malloc;
  env.xRealloc = dummy_realloc;
  env.xFree = dummy_free;

  // Call the focal function
  testMallocUninstall(&env);

  // Verifications
  // 1) pMemCtx should be cleared (set to 0/NULL)
  ASSERT_TRUE(env.pMemCtx == nullptr, "pMemCtx should be NULL after uninstall");

  // 2) xMalloc/xRealloc/xFree in the env should be restored to tm.*Save* values
  ASSERT_TRUE(env.xMalloc == tm.xSaveMalloc, "env.xMalloc should be restored to tm.xSaveMalloc");
  ASSERT_TRUE(env.xRealloc == tm.xSaveRealloc, "env.xRealloc should be restored to tm.xSaveRealloc");
  ASSERT_TRUE(env.xFree == tm.xSaveFree, "env.xFree should be restored to tm.xSaveFree");

  // 3) pDelMutex should have been invoked
  ASSERT_TRUE(g_delMutex_called, "tmDelMutex (mutex deletion) should have been invoked");

  // Note: The actual freeing of tm via tmLsmFree(p) is a no-op in the provided stub, so we don't assert here.
}

static void run_test_null_env() {
  // Ensure that passing a NULL pMemCtx does not crash and leaves environment untouched.
  g_delMutex_called = false;

  lsm_env env;
  env.pMemCtx = nullptr;
  env.xMalloc = dummy_malloc;
  env.xRealloc = dummy_realloc;
  env.xFree = dummy_free;

  testMallocUninstall(&env);

  // After uninstall with null pMemCtx, the environment should remain unchanged
  ASSERT_TRUE(env.pMemCtx == nullptr, "pMemCtx remains NULL when uninstalling with NULL context");
  ASSERT_TRUE(env.xMalloc == dummy_malloc, "env.xMalloc remains unchanged when no context");
  ASSERT_TRUE(env.xRealloc == dummy_realloc, "env.xRealloc remains unchanged when no context");
  ASSERT_TRUE(env.xFree == dummy_free, "env.xFree remains unchanged when no context");
  ASSERT_TRUE(g_delMutex_called == false, "mutex deletion should not be invoked when no context is present");
}

// =====================================================================================
// Main
// =====================================================================================

int main() {
  printf("Running testMallocUninstall test suite (C++11 no-GTest harness)\n");

  run_test_basic_uninstall();
  run_test_null_env();

  // Summary
  printf("Tests run: %d, Failures: %d\n", g_nTests, g_nFailures);

  return (g_nFailures == 0) ? 0 : 1;
}

// =====================================================================================
// Notes for maintainers
// - This test suite is designed to be standalone and compilable under C++11.
// - It uses a minimal stub of the lsm_env structure and a compatible layout for TmGlobal
//   to exercise the behavior of testMallocUninstall without pulling in the entire project.
// - We intentionally avoid terminating assertions (no EXIT on failure) to maximize
//   code coverage path exploration. The test harness logs failures but continues.
// - The actual project may define TM_BACKTRACE and extra fields; in our test, we omit
//   these by not defining TM_BACKTRACE to preserve a compact layout.
// - If integrating into a larger test framework, replace the ASSERT_TRUE/MACROs with the
//   project's preferred assertion facilities, keeping the same test semantics.
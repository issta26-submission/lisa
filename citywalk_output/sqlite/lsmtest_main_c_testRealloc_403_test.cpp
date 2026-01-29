//
// Unit tests for the focal method testRealloc(void*, int) in lsmtest_main.c
// This test suite is implemented in C++11 and uses no external testing framework.
// It relies on the C functions testMalloc/testRealloc/testFree declared with C linkage.
// The tests exercise growth, shrink, and equal-size scenarios, including data preservation
// across realloc calls and zero-initialization of newly allocated space.
//
// Candidate Keywords mapped from the focal method (Step 1):
// - realloc: memory reallocation behavior (moving memory if needed)
// - 8-byte header: metadata header stored just before the payload
// - testMalloc: fallback allocation path when ptr is NULL
// - nOrig: original payload size prior to realloc
// - memset: zero-initialization of new memory region when growing
// - pointer arithmetic: manipulating p to access header at (ptr-8)
// - data preservation: contents of old payload preserved after realloc
// - memory layout: header [int] at base, payload starts at base+8
// - testFree: deallocation utility to release blocks obtained via testMalloc/testRealloc
// 
// Notes:
// - The tests use non-terminating assertions (EXPECT_*) to maximize code execution.
// - Static internals of lsmtest_main.c are not accessed directly; we rely on the public test API.

#include <lsmtest.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sys/resource.h>
#include <iostream>
#include <cstdint>


// C linkage declarations for the test utilities provided by lsmtest_main.c
extern "C" {
  void *testMalloc(int n);
  void *testRealloc(void *ptr, int n);
  void testFree(void *ptr);
}

// Lightweight test harness (non-terminating assertions)
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECTATION FAILED: " << (msg) \
              << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    g_fail++; \
  } else { \
    g_pass++; \
  } \
} while(0)

static void test_null_allocation_basic()
{
  // Step 2/3: Verify that realloc with NULL returns a fresh allocation equivalent to testMalloc(n)
  int n = 32;
  void *p = testRealloc(nullptr, n);
  EXPECT_TRUE(p != nullptr, "testRealloc(NULL, n) should return a non-null pointer");

  if(p){
    // Header is stored 8 bytes before the payload
    int *header = (int *)((char*)p - 8);
    EXPECT_TRUE(header[0] == n, "Header should store the requested size n after NULL allocation");

    // Payload should be zero-initialized by testMalloc
    unsigned char *payload = (unsigned char *)p;
    for(int i = 0; i < n; ++i){
      EXPECT_TRUE(payload[i] == 0, "Newly allocated payload should be zero-initialized");
    }

    // Free the allocated block
    testFree(p);
  }
}

static void test_growth_preserves_old_data_and_zeroes_new_region()
{
  // Step 2: Grow from nOrig to nNew and ensure old data is preserved and new region zeroed
  int nOrig = 4;
  int nNew  = 16;

  void *p = testMalloc(nOrig);
  unsigned char *payload = (unsigned char *)p;
  // Initialize original payload with known pattern
  for(int i = 0; i < nOrig; ++i){
    payload[i] = (unsigned char)(i + 1); // 1,2,3,4
  }

  void *p2 = testRealloc(p, nNew);
  int *header = (int *)((char*)p2 - 8);
  unsigned char *newPayload = (unsigned char *)p2;

  EXPECT_TRUE(p2 != nullptr, "Growth realloc should return a valid pointer");
  if(!p2) {
    testFree(p);
    return;
  }

  // Header should reflect the new size
  EXPECT_TRUE(header[0] == nNew, "Header should be updated to new size nNew after growth");

  // The first nOrig bytes of the old payload should be preserved
  for(int i = 0; i < nOrig; ++i){
    EXPECT_TRUE(newPayload[8 + i] == (unsigned char)(i + 1),
                "Old data should be preserved after growth in the corresponding payload region");
  }

  // The newly allocated region (8+nOrig .. 8+nNew-1) should be zero-initialized
  for(int i = nOrig; i < nNew; ++i){
    EXPECT_TRUE(newPayload[8 + i] == 0,
                "Newly allocated region after growth should be zero-initialized");
  }

  testFree(p2);
}

static void test_shrink_preserves_head()
{
  // Step 2: Shrink from nOrig to nNew and verify head preservation
  int nOrig = 16;
  int nNew  = 4;

  void *p = testMalloc(nOrig);
  unsigned char *payload = (unsigned char *)p;
  // Initialize payload with a simple pattern
  for(int i = 0; i < nOrig; ++i){
    payload[i] = (unsigned char)((i * 3) & 0xFF);
  }

  void *pReduced = testRealloc(p, nNew);
  int *header = (int *)((char*)pReduced - 8);
  unsigned char *payloadReduced = (unsigned char *)pReduced;

  EXPECT_TRUE(pReduced != nullptr, "Shrink realloc should return a valid pointer");
  if(!pReduced){
    return;
  }

  // Header should reflect the new (smaller) size
  EXPECT_TRUE(header[0] == nNew, "Header should be updated to new size nNew after shrink");

  // The first nNew bytes of the payload should preserve the initial data
  for(int i = 0; i < nNew; ++i){
    EXPECT_TRUE(payloadReduced[8 + i] == (unsigned char)((i * 3) & 0xFF),
                "Payload data should be preserved for the portion kept after shrink");
  }

  testFree(pReduced);
}

static void test_equal_size_preserves_data()
{
  // Step 2: Realloc with equal size should preserve existing data
  int nOrig = 8;

  void *p = testMalloc(nOrig);
  unsigned char *payload = (unsigned char *)p;
  // Initialize payload with a unique pattern
  for(int i = 0; i < nOrig; ++i){
    payload[i] = (unsigned char)(0xA0 + (i & 0x0F));
  }

  void *p2 = testRealloc(p, nOrig); // equal size
  int *header = (int *)((char*)p2 - 8);
  unsigned char *payload2 = (unsigned char *)p2;

  EXPECT_TRUE(p2 != nullptr, "Equal-size realloc should return a valid pointer");
  if(!p2) return;

  // Header should remain unchanged
  EXPECT_TRUE(header[0] == nOrig, "Header should remain equal to original size after equal-size realloc");

  // Data should be preserved
  for(int i = 0; i < nOrig; ++i){
    EXPECT_TRUE(payload2[8 + i] == (unsigned char)(0xA0 + (i & 0x0F)),
                "Payload data should be preserved after equal-size realloc");
  }

  testFree(p2);
}

static void test_zero_length_like_behaviour()
{
  // Additional small test: zero-sized allocation path
  int n = 0;
  void *p = testRealloc(nullptr, n);
  int *header = (int *)((char*)p - 8);
  EXPECT_TRUE(p != nullptr, "Realloc with NULL and n=0 should return a non-null pointer");
  if(p){
    EXPECT_TRUE(header[0] == n, "Header should store zero size after NULL allocation with n=0");
    // No payload to inspect since n == 0
    testFree(p);
  }
}

int main()
{
  // Run tests
  test_null_allocation_basic();
  test_growth_preserves_old_data_and_zeroes_new_region();
  test_shrink_preserves_head();
  test_equal_size_preserves_data();
  test_zero_length_like_behaviour();

  // Summary
  std::cout << "Summary: " << g_pass << " passed, " << g_fail << " failed." << std::endl;
  // Return non-zero if any test failed
  return (g_fail != 0) ? 1 : 0;
}
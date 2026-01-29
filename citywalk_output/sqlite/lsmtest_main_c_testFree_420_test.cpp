// Test suite for the focal method: testFree(void *ptr)
// Focus: verify behavior of testFree, which frees memory allocated by testMalloc.
// The test relies on the existing C runtime memory helpers exposed by the
// <FOCAL_CLASS_DEP> block (e.g., testMalloc and testFree).
// This test suite is written in C++11 without GoogleTest and uses a tiny
// assertion framework to keep execution non-terminating on failure.
//
// Key dependencies and concepts (Candidate Keywords):
// - testMalloc: allocates (n + 8) bytes, stores n in the first 4 bytes, and returns
//   a pointer to the 8-byte offset where user data begins.
// - testFree: for a non-null pointer, it computes the base address by subtracting 8,
//   then fills the entire (n + 8) region with 0x55 before freeing.
// - The header at base (first 4 bytes) holds the size n.
// - Pointer arithmetic and memory layout: (u8*)ptr - 8 points to allocation base.
// - Standard library memory operations: malloc, free, memset.
// - Non-terminating assertions: record failures but do not exit.
// - Minimal C linkage for C functions tested: testMalloc, testFree.

#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Import C linkage for the focal functions under test.
// We assume these functions are provided by the existing test harness (<FOCAL_CLASS_DEP>).
extern "C" {
  // Allocates n bytes of user data plus 8 bytes header, stores n in header, returns user ptr.
  void* testMalloc(int n);

  // Frees memory previously allocated by testMalloc via the accompanying header layout.
  void testFree(void* ptr);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define TEST_PRINT(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      TEST_PRINT("[FAIL] %s:%d: expected true: %s\n", __FILE__, __LINE__, #cond); \
      ++g_failures; \
    } \
  } while(0)
#define EXPECT_NOT_NULL(p) \
  do { \
    if((p) == nullptr) { \
      TEST_PRINT("[FAIL] %s:%d: pointer is null (unexpected): %s\n", __FILE__, __LINE__, #p); \
      ++g_failures; \
    } \
  } while(0)

////////////////////////////////////////////////////////////////////////////////
// Unit Tests for testFree
////////////////////////////////////////////////////////////////////////////////

// Test 1: testFree should handle a null pointer gracefully (no crash, no action)
static void testFree_NullPointer_DoesNotCrash(void) {
  // Focal: if (ptr) branch should be false and function should simply return.
  testFree(nullptr);
  // Simple assertion to mark test execution; there is no observable state to check here.
  EXPECT_TRUE(true);
}

// Test 2: Allocated memory via testMalloc keeps correct header value before testFree runs.
// After obtaining a valid user pointer p, verify that the header at (p - 8) stores the
// correct size n. This ensures testMalloc's memory layout matches expectations.
static void testFree_ValidateHeaderBeforeFree(void) {
  const int sizes[] = {0, 4, 16, 64}; // diverse test sizes including edge case 0
  for(size_t i = 0; i < sizeof(sizes)/sizeof(sizes[0]); ++i){
    int n = sizes[i];
    void *p = testMalloc(n);
    EXPECT_NOT_NULL(p);

    // Inspect header at (base) = (char*)p - 8
    uint8_t *base = (uint8_t*)p - 8;
    int headerVal = *((int*)base);
    EXPECT_TRUE(headerVal == n);

    // Free allocated memory
    testFree(p);
  }
}

// Test 3: testFree handles multiple allocations with varying sizes (sanity of memory layout
// across separate blocks). Verify headers for each allocation before freeing.
static void testFree_MultipleAllocations_HeaderCheck(void) {
  void *p1 = testMalloc(8);
  void *p2 = testMalloc(32);
  EXPECT_NOT_NULL(p1);
  EXPECT_NOT_NULL(p2);
  int h1 = *((int*)((uint8_t*)p1 - 8));
  int h2 = *((int*)((uint8_t*)p2 - 8));
  EXPECT_TRUE(h1 == 8);
  EXPECT_TRUE(h2 == 32);

  testFree(p1);
  testFree(p2);
}

// Test 4: Ensure that testFree does not crash for a small non-zero size block and
// that it properly frees without leaving a dangling pointer observable to the caller.
// We verify by performing a sequence of allocations and frees, ensuring no crash occurs.
static void testFree_RunsWithSmallBlock(void) {
  void *p = testMalloc(2);
  EXPECT_NOT_NULL(p);
  // Validate header before free
  int headerVal = *((int*)((uint8_t*)p - 8));
  EXPECT_TRUE(headerVal == 2);

  testFree(p);
  // No direct post-condition observable; if testFree crashes, the test would fail.
  EXPECT_TRUE(true);
}

////////////////////////////////////////////////////////////////////////////////
// Test Runner
////////////////////////////////////////////////////////////////////////////////

int main() {
  // Run tests
  testFree_NullPointer_DoesNotCrash();
  testFree_ValidateHeaderBeforeFree();
  testFree_MultipleAllocations_HeaderCheck();
  testFree_RunsWithSmallBlock();

  // Summary
  if(g_failures){
    TEST_PRINT("TEST SUMMARY: FAILED (%d failures)\n", g_failures);
  }else{
    TEST_PRINT("TEST SUMMARY: ALL TESTS PASSED\n");
  }

  // Return 0 for success, non-zero if any failure occurred
  return g_failures ? 1 : 0;
}
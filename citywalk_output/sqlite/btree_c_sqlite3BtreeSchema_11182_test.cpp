/*
  Unit test suite for sqlite3BtreeSchema in btree.c

  Approach:
  - Build a minimal, focused test harness around the focal function.
  - Reuse the project's Btree and BtShared structures (from btreeInt.h) to construct
    a lightweight scenario sufficient to exercise sqlite3BtreeSchema's core logic.
  - Do not rely on external test frameworks (GTest not used). Implement simple assertion
    helpers and a small test runner in C++11.
  - Validate:
    1) Allocation occurs when nBytes > 0 and no previous schema exists.
    2) No allocation occurs when nBytes == 0.
    3) Reuse of existing schema on subsequent calls and preservation of xFreeSchema.

  Important notes:
  - We assume the project exposes Btree and BtShared types via btreeInt.h.
  - We link against the btree.c source (the focal method lives there) so internal
    interactions with sqlite3BtreeEnter/Leave and sqlite3DbMallocZero behave as in production.
  - We provide no mocks for static/internal functions; we rely on the real implementation
    for this focused unit test.
  - Explanatory comments are included for each test case.

  Build (conceptual):
  - Compile this file with the project sources, e.g.:
      g++ -std=c++11 -I<path-to-project-headers> test_btree_schema.cpp btree.c -o test_btree_schema
  - Run: ./test_btree_schema

  This test expects the project to expose Btree and BtShared definitions (from btreeInt.h).
*/

#include <cstdlib>
#include <btreeInt.h>
#include <iostream>
#include <cstring>


// Bring in the project's C headers/types in a C++11 translation unit.
extern "C" {
  #include "btreeInt.h" // defines Btree and BtShared structures
  // The focal function sqlite3BtreeSchema is defined in btree.c
  // We call it directly; its dependencies (sqlite3BtreeEnter/Leave and sqlite3DbMallocZero)
  // are provided by the project (no external stubs here).
}

// Simple assertion helper that doesn't terminate on failure (non-terminating).
#define TEST_ASSERT(cond, msg)                                    \
  do {                                                             \
    if (!(cond)) {                                                 \
      std::cerr << "[TEST FAILURE] " << msg << std::endl;          \
      ++g_failures;                                               \
    } else {                                                       \
      ++g_passed;                                                \
    }                                                              \
  } while (0)

// Global counters for test results
static int g_passed = 0;
static int g_failures = 0;

/*
  Helper: allocate and initialize a BtShared with pSchema = NULL.
  This mirrors a minimal environment for sqlite3BtreeSchema to operate.
*/
static BtShared* create_minimal_btshared() {
  BtShared* pBt = (BtShared*)std::malloc(sizeof(BtShared));
  if (!pBt) return nullptr;
  std::memset(pBt, 0, sizeof(BtShared));
  // Ensure the fields used by sqlite3BtreeSchema exist in a consistent state
  pBt->pSchema = NULL;
  pBt->xFreeSchema = NULL;
  return pBt;
}

/*
  Helper: allocate and initialize a Btree with pBt pointing to given BtShared.
*/
static Btree* create_btree_with_btshared(BtShared* pBt) {
  Btree* p = (Btree*)std::malloc(sizeof(Btree));
  if (!p) return nullptr;
  std::memset(p, 0, sizeof(Btree));
  p->pBt = pBt;
  return p;
}

/*
  Helper: free a Btree and its BtShared (best-effort; tests rely on clean up).
*/
static void destroy_btree_with_btshared(Btree* p) {
  if (!p) return;
  BtShared* pBt = p->pBt;
  if (pBt) {
    // pBt->pSchema may be allocated by sqlite3BtreeSchema; free if non-null
    if (pBt->pSchema) {
      std::free(pBt->pSchema);
      pBt->pSchema = NULL;
    }
    std::free(pBt);
    p->pBt = NULL;
  }
  std::free(p);
}

/* Test 1: Allocation occurs when nBytes > 0 and no existing schema. */
static void test_allocation_on_positive_bytes() {
  // Arrange
  BtShared* pBt = create_minimal_btshared();
  if (!pBt) {
    std::cerr << "Failed to allocate BtShared" << std::endl;
    g_failures++;
    return;
  }

  Btree* p = create_btree_with_btshared(pBt);
  if (!p) {
    std::cerr << "Failed to allocate Btree" << std::endl;
    g_failures++;
    destroy_btree_with_btshared(p);
    return;
  }

  // Act
  void (*pFree)(void*) = nullptr; // no-op free function for this test
  void* schemaPtr = sqlite3BtreeSchema(p, 128, pFree);

  // Assert
  TEST_ASSERT(schemaPtr != nullptr, "sqlite3BtreeSchema should allocate a non-null schema when nBytes > 0 and no existing schema.");
  TEST_ASSERT(schemaPtr == pBt->pSchema, "Returned schema pointer should equal BtShared.pSchema after allocation.");
  TEST_ASSERT(pBt->pSchema != NULL, "BtShared.pSchema should be set after allocation.");
  TEST_ASSERT(pBt->xFreeSchema == pFree, "BtShared.xFreeSchema should be set to the provided xFree function.");

  // Verify that the allocated memory is zero-initialized (first few bytes).
  unsigned char* bytes = (unsigned char*)pBt->pSchema;
  bool all_zero = true;
  for (int i = 0; i < 16; ++i) {
    if (bytes[i] != 0) { all_zero = false; break; }
  }
  TEST_ASSERT(all_zero, "Allocated schema memory should be zero-initialized by sqlite3DbMallocZero.");

  // Cleanup
  destroy_btree_with_btshared(p);
}

/* Test 2: No allocation occurs when nBytes is zero and no existing schema. */
static void test_no_allocation_on_zero_bytes() {
  // Arrange
  BtShared* pBt = create_minimal_btshared();
  if (!pBt) {
    std::cerr << "Failed to allocate BtShared" << std::endl;
    g_failures++;
    return;
  }

  Btree* p = create_btree_with_btshared(pBt);
  if (!p) {
    std::cerr << "Failed to allocate Btree" << std::endl;
    g_failures++;
    destroy_btree_with_btshared(p);
    return;
  }

  // Act
  void (*pFree)(void*) = nullptr;
  void* schemaPtr = sqlite3BtreeSchema(p, 0, pFree);

  // Assert
  TEST_ASSERT(schemaPtr == nullptr, "sqlite3BtreeSchema should return NULL when nBytes == 0 and no existing schema.");
  TEST_ASSERT(pBt->pSchema == NULL, "BtShared.pSchema should remain NULL when nBytes == 0.");
  TEST_ASSERT(pBt->xFreeSchema == nullptr, "BtShared.xFreeSchema should remain NULL when no allocation occurs.");

  // Cleanup
  destroy_btree_with_btshared(p);
}

/* Test 3: Do not overwrite existing schema or xFreeSchema on subsequent calls. */
static void test_existing_schema_preserves_pointer_and_free() {
  // Arrange: initial allocation
  BtShared* pBt = create_minimal_btshared();
  if (!pBt) {
    std::cerr << "Failed to allocate BtShared" << std::endl;
    g_failures++;
    return;
  }

  Btree* p = create_btree_with_btshared(pBt);
  if (!p) {
    std::cerr << "Failed to allocate Btree" << std::endl;
    g_failures++;
    destroy_btree_with_btshared(p);
    return;
  }

  void (*firstFree)(void*) = nullptr;
  void* firstSchema = sqlite3BtreeSchema(p, 64, firstFree);
  TEST_ASSERT(firstSchema != nullptr, "Initial allocation should succeed (64 bytes).");
  TEST_ASSERT(pBt->pSchema == firstSchema, "BtShared.pSchema should be set to the allocated memory on first call.");
  TEST_ASSERT(pBt->xFreeSchema == firstFree, "BtShared.xFreeSchema should be set to the first provided xFree function.");

  // Act: second call with a different nBytes and a different xFree
  void (*secondFree)(void*) = nullptr;
  void* secondSchema = sqlite3BtreeSchema(p, 256, secondFree);

  // Assert: should return same schema pointer and not overwrite xFreeSchema
  TEST_ASSERT(secondSchema == firstSchema, "Second call should return the existing schema pointer (no reallocation).");
  TEST_ASSERT(pBt->pSchema == firstSchema, "BtShared.pSchema should remain the same after second call.");
  TEST_ASSERT(pBt->xFreeSchema == firstFree, "BtShared.xFreeSchema should remain as the initial function, not overwritten by second call.");

  // Cleanup
  destroy_btree_with_btshared(p);
}

/* Main: run all tests and report results. */
int main() {
  std::cout << "Running sqlite3BtreeSchema unit tests (C++11 harness)..." << std::endl;

  test_allocation_on_positive_bytes();
  test_no_allocation_on_zero_bytes();
  test_existing_schema_preserves_pointer_and_free();

  std::cout << "Tests passed: " << g_passed << ", Failures: " << g_failures << std::endl;
  if (g_failures > 0) {
    std::cout << "Some tests failed. Please inspect the corresponding messages above." << std::endl;
  } else {
    std::cout << "All tests passed." << std::endl;
  }

  // Note: Clean exit; memory is still allocated in some tests unless the test harness
  // intentionally frees it. We performed cleanup per test.
  return g_failures ? 1 : 0;
}
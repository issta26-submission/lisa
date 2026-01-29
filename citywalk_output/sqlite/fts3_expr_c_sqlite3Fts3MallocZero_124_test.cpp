/*
  Lightweight C++11 unit tests for sqlite3Fts3MallocZero (as defined in fts3_expr.c)

  Notes:
  - This test harness assumes the test binary will be linked with the actual
    implementation of sqlite3Fts3MallocZero from fts3_expr.c (as part of the
    codebase under test). The test code declares the function prototype and
    uses it directly.
  - To inspect behavior of sqlite3Fts3MallocZero, we rely on the allocator
    sqlite3_malloc64 that is used inside the focal function. We do not provide a
    full SQLite environment; instead we rely on the real behavior of the linked
    implementation (allocation succeeds under normal conditions) and validate
    that the allocated memory is zero-initialized.
  - A lightweight, non-terminating assertion mechanism is implemented (no gtest/gmock).
  - Static/global state in the production code is not interacted with directly here.
  - This test focuses on the true-branch of the memset path (pRet != NULL) which
    is the common case. Simulating an allocation failure would require asserting
    a NULL return path, which typically requires a mocked allocator; this test
    does not enforce that scenario to keep the tests robust in standard builds.
*/

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Prototypes expected to exist in the linked code under test.
// We assume the translation unit under test provides these symbols.
extern "C" void* sqlite3Fts3MallocZero(int64_t nByte);
extern "C" void  sqlite3_free(void*);

// Lightweight test harness (no GTest). Provides simple pass/fail reporting.
static int g_tests_run = 0;
static int g_failures  = 0;

// Simple check macro: does not terminate the program on failure; records failure.
#define TEST_CHECK(cond, msg)                                    \
  do {                                                           \
    ++g_tests_run;                                               \
    if(!(cond)) {                                                \
      std::cerr << "[FAIL] " << msg << "\n";                    \
      ++g_failures;                                             \
    } else {                                                     \
      /* success: optional verbose can be added here */         \
    }                                                            \
  } while(false)

static bool is_zero_memory(const unsigned char* p, size_t n) {
  for(size_t i = 0; i < n; ++i) {
    if(p[i] != 0) return false;
  }
  return true;
}

// Test 1: Allocation returns non-null and the memory is zero-initialized.
// This validates the true-branch of the memset when a valid memory block is allocated.
void test_alloc_zeroed_memory() {
  // Reasonable size for test; 64 bytes is typical and easy to verify.
  const int64_t N = 64;
  unsigned char* p = (unsigned char*)sqlite3Fts3MallocZero(N);
  TEST_CHECK(p != nullptr, "sqlite3Fts3MallocZero should return non-null pointer for N>0");

  if(p != nullptr){
    bool all_zero = is_zero_memory(p, (size_t)N);
    TEST_CHECK(all_zero, "Memory returned by sqlite3Fts3MallocZero should be zero-initialized");
    // Clean up
    sqlite3_free(p);
  }
}

// Test 2: Multiple consecutive allocations are independently zero-initialized.
// This exercises repeated successful allocations and ensures isolation between allocations.
void test_multiple_allocations_zeroed() {
  const int NUM = 6;
  const int64_t BLOCK = 32;
  for(int i = 0; i < NUM; ++i){
    unsigned char* p = (unsigned char*)sqlite3Fts3MallocZero(BLOCK);
    TEST_CHECK(p != nullptr, "Repeated allocation should succeed (non-null)");
    if(p != nullptr){
      bool all_zero = is_zero_memory(p, (size_t)BLOCK);
      TEST_CHECK(all_zero, "Each allocated block should be zero-initialized");
      sqlite3_free(p);
    } else {
      // If allocation fails, we cannot verify zero-initialization; continue to next.
      // The test still records a failure above.
    }
  }
}

// Test 3: Edge case with zero bytes.
// Depending on allocator semantics, sqlite3_malloc64(0) may return NULL or a unique pointer.
// We simply ensure the function handles nByte == 0 gracefully (no crash) and that
// if it returns a non-null pointer, it must still be safe to free.
void test_zero_byte_allocation_behavior() {
  const int64_t N = 0;
  void* p = sqlite3Fts3MallocZero(N);
  if(p != nullptr){
    // If a zero-byte allocation returns a non-null pointer, it should be safe to free
    sqlite3_free(p);
  }
  // Either NULL or non-NULL is acceptable for nByte == 0 in many malloc-like APIs;
  // we do not fail the test on NULL for this case to remain robust across impls.
  TEST_CHECK(true, "Behavior with nByte==0 does not crash and is handled");
}

// Test 4: Sanity check that the function exists and is callable without crashing.
// This is a no-op test used to ensure test harness integration is fine.
void test_sanity_call() {
  const int64_t N = 8;
  unsigned char* p = (unsigned char*)sqlite3Fts3MallocZero(N);
  TEST_CHECK(p != nullptr, "Sanity: allocation should succeed for small N");
  if(p != nullptr){
    // verify zeros
    TEST_CHECK(is_zero_memory(p, (size_t)N), "Sanity: memory should be zero-initialized");
    sqlite3_free(p);
  }
}

// Entry point
int main() {
  std::cout << "Starting unit tests for sqlite3Fts3MallocZero (C/C++ interop test harness)\n";

  test_alloc_zeroed_memory();
  test_multiple_allocations_zeroed();
  test_zero_byte_allocation_behavior();
  test_sanity_call();

  std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << "\n";

  if(g_failures > 0){
    std::cout << "Overall: FAIL\n";
    return 1;
  }else{
    std::cout << "Overall: PASS\n";
    return 0;
  }
}
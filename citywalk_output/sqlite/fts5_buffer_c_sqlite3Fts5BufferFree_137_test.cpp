/*
Step 1 - Program Understanding (Condensed in comments)
 focal method under test: sqlite3Fts5BufferFree
 - Frees the internal buffer pBuf->p using sqlite3_free
 - Zeroes the entire Fts5Buffer via memset(pBuf, 0, sizeof(Fts5Buffer))
 
Key dependent components (Candidate Keywords):
 - Fts5Buffer structure with at least a pointer field p
 - sqlite3_free(void*)
 - memset(void*, int, size_t)
 - size of Fts5Buffer
 - Behavior when pBuf->p is non-null or null
 - Post-condition: pBuf must be zeroed and pBuf->p must be NULL after free
 - Ensure non-terminating assertions (continue after failure) in test harness
 - Use only C++11 standard library; no GTest; run tests from main

Step 2/3 - Test Suite Implementation (C++11, without GTest)
Notes:
 - We provide a minimal, self-contained mock of sqlite3_free to verify usage and simulate freeing.
 - We define a minimal Fts5Buffer with an extra field to verify it is zeroed by memset.
 - We implement tests that cover normal free, free with null, and double-free scenarios.
 - All tests are designed to continue execution even if a check fails (non-terminating assertions).
 - Explanatory comments accompany each unit test.

This file is self-contained and can be compiled with a C++11 compiler.
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fts5Int.h>


// Domain: Minimal Fts5Buffer and sqlite3_free mock to drive sqlite3Fts5BufferFree behavior.

struct Fts5Buffer {
  void *p;
  int dummy; // extra field to observe memset zeroing
};

// Global/state used by our sqlite3_free mock to verify interactions
static void* g_lastFreed = nullptr;
static int g_free_call_count = 0;

// Mock of sqlite3_free with C linkage to resemble project's usage
extern "C" void sqlite3_free(void* p) {
  g_lastFreed = p;
  ++g_free_call_count;
  // Emulate free semantics: free only if non-null
  if (p != nullptr) {
    free(p);
  }
}

// Fts5BufferFree implementation under test (as provided)
extern "C" void sqlite3Fts5BufferFree(Fts5Buffer *pBuf){
  sqlite3_free(pBuf->p);
  memset(pBuf, 0, sizeof(Fts5Buffer));
}

// Helper state for tests
static void reset_test_state() {
  g_lastFreed = nullptr;
  g_free_call_count = 0;
}

// Simple non-terminating assertion macro (records failures but continues)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define ASSERT_TRUE(cond, msg) do { \
  ++g_total_asserts; \
  if(!(cond)) { \
    std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
    ++g_failed_asserts; \
  } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
  ++g_total_asserts; \
  if(!((a) == (b))) { \
    std::cerr << "ASSERT FAILED: " << (msg) \
              << " | Expected: " << (void*)(b) << " Actual: " << (void*)(a) << std::endl; \
    ++g_failed_asserts; \
  } \
} while(0)

#define ASSERT_PTR_EQ(a, b, msg) do { \
  ++g_total_asserts; \
  if((a) != (b)) { \
    std::cerr << "ASSERT FAILED: " << (msg) \
              << " | Expected: " << (void*)(b) << " Actual: " << (void*)(a) << std::endl; \
    ++g_failed_asserts; \
  } \
} while(0)

// Test 1: Normal path - pBuf.p points to allocated data, should be freed and buffer zeroed
void test_sqlite3Fts5BufferFree_normal_free() {
  // Arrange
  Fts5Buffer buf;
  void* data = malloc(64);
  buf.p = data;
  buf.dummy = 123; // ensure non-zero prior to free
  reset_test_state();

  // Act
  sqlite3Fts5BufferFree(&buf);

  // Assert (non-terminating)
  // 1) pBuf->p should be NULL after free
  ASSERT_TRUE(buf.p == nullptr, "buf.p should be NULL after sqlite3Fts5BufferFree");

  // 2) sqlite3_free should have been called exactly once with original pointer
  ASSERT_EQ(g_free_call_count, 1, "sqlite3_free should be called exactly once");
  ASSERT_PTR_EQ(g_lastFreed, data, "sqlite3_free should be called with original buffer data pointer");

  // 3) Entire Fts5Buffer should be zeroed (dummy field becomes 0)
  ASSERT_TRUE(buf.dummy == 0, "buf.dummy should be zero after memset");

  // Cleanup (data freed by sqlite3_free mock if non-null)
  // data already freed in mock; no action needed
}

// Test 2: Free when pBuf.p is NULL (null free path)
void test_sqlite3Fts5BufferFree_free_null_pointer() {
  // Arrange
  Fts5Buffer buf;
  buf.p = nullptr;
  buf.dummy = 999;
  reset_test_state();

  // Act
  sqlite3Fts5BufferFree(&buf);

  // Assert
  // 1) pBuf->p should remain NULL
  ASSERT_TRUE(buf.p == nullptr, "buf.p should remain NULL after freeing NULL pointer");

  // 2) sqlite3_free should be called with NULL (counts as a call)
  ASSERT_EQ(g_free_call_count, 1, "sqlite3_free should be called once even when pointer is NULL");
  ASSERT_PTR_EQ(g_lastFreed, nullptr, "sqlite3_free should be called with NULL when pBuf.p is NULL");

  // 3) dummy should be zeroed
  ASSERT_TRUE(buf.dummy == 0, "buf.dummy should be zero after memset");
}

// Test 3: Double-free scenario - subsequent call should still call sqlite3_free with NULL
void test_sqlite3Fts5BufferFree_double_free() {
  // Arrange
  Fts5Buffer buf;
  void* data = malloc(32);
  buf.p = data;
  buf.dummy = 5;
  reset_test_state();

  // Act - first free
  sqlite3Fts5BufferFree(&buf);

  // Assert after first free
  ASSERT_TRUE(buf.p == nullptr, "After first free, buf.p should be NULL");
  ASSERT_EQ(g_free_call_count, 1, "After first free, sqlite3_free should be called once");
  ASSERT_PTR_EQ(g_lastFreed, data, "First sqlite3_free should be called with original pointer");

  // Act - second free attempt on already freed buffer
  sqlite3Fts5BufferFree(&buf);

  // Assert after second free
  // Now the second call passes NULL to sqlite3_free
  ASSERT_EQ(g_free_call_count, 2, "After second free, sqlite3_free should be called again (with NULL)");
  ASSERT_PTR_EQ(g_lastFreed, nullptr, "Second sqlite3_free should be called with NULL");
  ASSERT_TRUE(buf.p == nullptr, "buf.p should remain NULL after second free");
  ASSERT_TRUE(buf.dummy == 0, "buf.dummy should be zero after memset on second free");
}

// Entry point for tests
int main() {
  std::cout << "Starting sqlite3Fts5BufferFree unit tests (non-terminating assertions).\n";

  test_sqlite3Fts5BufferFree_normal_free();
  test_sqlite3Fts5BufferFree_free_null_pointer();
  test_sqlite3Fts5BufferFree_double_free();

  int total = g_total_asserts;
  int failed = g_failed_asserts;
  std::cout << "Tests completed. Total assertions: " << total << ", Failures: " << failed << ".\n";

  // If any failures, return non-zero to indicate incomplete/failed test run
  return (failed == 0) ? 0 : 1;
}
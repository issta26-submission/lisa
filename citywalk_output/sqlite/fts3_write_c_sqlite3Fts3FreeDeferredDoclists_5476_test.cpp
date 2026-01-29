// C++11 unit test suite for sqlite3Fts3FreeDeferredDoclists
// This test focuses on validating the behavior of the function
// sqlite3Fts3FreeDeferredDoclists in fts3_write.c by constructing
// a real-ld Fts3Cursor and its deferred token chain as defined in
// the project's headers. We do not rely on GoogleTest; instead we
// provide lightweight EXPECT-like assertions that do not terminate
// the test on failure to maximize code coverage.
//
// Notes:
// - We include the project's fts3Int.h to obtain real type layouts
//   (Fts3Cursor, Fts3DeferredToken, PendingList, etc.).
// - The tests exercise true/false branches: empty pDeferred, single
//   token, and multiple tokens with varying pList values.
// - We call the C function sqlite3Fts3FreeDeferredDoclists from C++
//   using extern "C" linkage.
// - We use non-terminating assertions to keep test execution flowing.

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Import necessary dependencies with correct paths.
// The project provides fts3Int.h which defines Fts3Cursor, Fts3DeferredToken,
// PendingList and related types used by sqlite3Fts3FreeDeferredDoclists.
extern "C" {
}

// Public prototype of the function under test
extern "C" {
void sqlite3Fts3FreeDeferredDoclists(Fts3Cursor *pCsr);
}

// Lightweight non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      std::cout << "EXPECT_TRUE failed: " << #cond \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_NULL(ptr) \
  do { \
    if((ptr) != nullptr) { \
      std::cout << "EXPECT_NULL failed: " << #ptr \
                << " is not null at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_NOT_NULL(ptr) \
  do { \
    if((ptr) == nullptr) { \
      std::cout << "EXPECT_NOT_NULL failed: " << #ptr \
                << " is null at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_EQ(a, b) \
  do { \
    if((a) != (b)) { \
      std::cout << "EXPECT_EQ failed: " << #a << " != " << #b \
                << " (" << (void*)(a) << " vs " << (void*)(b) << ")" \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_failures; \
    } \
  } while(0)

// Helper factory functions to build test structures.
// These rely on the real layout of Fts3DeferredToken and PendingList
// as defined in the project's headers.

static PendingList* make_pending() {
  PendingList* pl = (PendingList*)malloc(sizeof(PendingList));
  if(pl) {
    std::memset(pl, 0, sizeof(PendingList));
  }
  return pl;
}

static Fts3DeferredToken* make_token(PendingList* pl, Fts3DeferredToken* next) {
  Fts3DeferredToken* t = (Fts3DeferredToken*)malloc(sizeof(Fts3DeferredToken));
  if(t) {
    // Initialize fields used by sqlite3Fts3FreeDeferredDoclists
    t->pList = pl;
    t->pNext = next;
  }
  return t;
}

// Test 1: No deferred tokens (empty chain)
static void test_no_deferred_doclists() {
  Fts3Cursor csr;
  csr.pDeferred = nullptr;

  // Act
  sqlite3Fts3FreeDeferredDoclists(&csr);

  // Assert
  // The function should simply exit without dereferencing null
  EXPECT_NULL(csr.pDeferred);
}

// Test 2: Single deferred token with a non-null list
static void test_single_deferred_token() {
  // Arrange
  PendingList* pl = make_pending();
  Fts3DeferredToken* t1 = make_token(pl, nullptr);

  Fts3Cursor csr;
  csr.pDeferred = t1;

  // Act
  sqlite3Fts3FreeDeferredDoclists(&csr);

  // Assert
  // The function should set the token's pList to NULL after deleting
  EXPECT_NULL(t1->pList);

  // Cleanup (best effort; fts3PendingListDelete is expected to free pl)
  // After the function call, t1 should still be a valid allocated object
  // but its pList is NULL. We free both to avoid leaks in the test harness.
  free(t1);
  free(pl);
}

// Test 3: Multiple tokens with mixed pList values
static void test_multiple_tokens_mixed_lists() {
  // Arrange
  PendingList* pl1 = make_pending();
  PendingList* pl2 = nullptr; // token with NULL list
  PendingList* pl3 = make_pending();

  Fts3DeferredToken* t3 = make_token(pl3, nullptr);
  Fts3DeferredToken* t2 = make_token(pl2, t3);
  Fts3DeferredToken* t1 = make_token(pl1, t2);

  Fts3Cursor csr;
  csr.pDeferred = t1;

  // Act
  sqlite3Fts3FreeDeferredDoclists(&csr);

  // Assert: All non-null lists should be cleared to NULL
  EXPECT_NULL(t1->pList);
  EXPECT_NULL(t2->pList); // originally NULL; remains NULL
  EXPECT_NULL(t3->pList);

  // Cleanup
  free(t1);
  free(t2);
  free(t3);
  // The PendingList instances (pl1, pl3) are expected to be freed by
  // fts3PendingListDelete; if not, add extra checks here in real tests.
  free(pl1);
  // pl2 is NULL; nothing to free
  free(pl3);
}

// Entry point summing up tests
int main() {
  std::cout << "Running sqlite3Fts3FreeDeferredDoclists unit tests (C++11)" << std::endl;

  test_no_deferred_doclists();
  test_single_deferred_token();
  test_multiple_tokens_mixed_lists();

  if(g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
    return 1;
  }
}
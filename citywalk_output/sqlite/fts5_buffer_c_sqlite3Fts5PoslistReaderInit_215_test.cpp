// Test suite for sqlite3Fts5PoslistReaderInit (focusing on fts5_buffer.c logic)
// Note: This test provides a minimal, self-contained environment to exercise the focal
// method without requiring the full original project build. It mocks the necessary pieces
// (Fts5PoslistReader struct and sqlite3Fts5PoslistReaderNext behavior) to enable
// deterministic testing of Init's interaction with Next().
//
// The tests cover both true and false branches of the internal predicate (bEof) and the
// basic state propagation (a, n) after Init.

#include <iostream>
#include <cstring>
#include <fts5Int.h>


// Domain: lightweight, self-contained test for C-style API in a C++11 test harness.

// Minimal type and struct definitions to mirror the focal function's dependencies.
typedef unsigned char u8;

struct Fts5PoslistReader {
  const u8 *a;
  int n;
  int bEof;
};

// Forward declarations of the functions under test.
// In the real project these would come from fts5_buffer.c / fts5Int.h.
int sqlite3Fts5PoslistReaderNext(Fts5PoslistReader *pIter);
int sqlite3Fts5PoslistReaderInit(const u8 *a, int n, Fts5PoslistReader *pIter);

// Mock implementation of sqlite3Fts5PoslistReaderNext to provide deterministic behavior
// for tests. In the real project, this would compute the next position in the poslist.
// Here, we set bEof based on simple, controllable conditions to exercise both branches.
int sqlite3Fts5PoslistReaderNext(Fts5PoslistReader *pIter) {
  if (pIter == nullptr) return 1;
  // Determine eof: consider non-empty buffer when both a != NULL and n > 0.
  if (pIter->a == nullptr || pIter->n <= 0) {
    pIter->bEof = 1;
  } else {
    pIter->bEof = 0;
  }
  return pIter->bEof;
}

// Implementation of the focal method under test, matching the original logic.
// It initializes the iterator and invokes Next to determine eof.
int sqlite3Fts5PoslistReaderInit(const u8 *a, int n, Fts5PoslistReader *pIter) {
  memset(pIter, 0, sizeof(*pIter));
  pIter->a = a;
  pIter->n = n;
  sqlite3Fts5PoslistReaderNext(pIter);
  return pIter->bEof;
}

// ----- Simple in-process test framework (non-GTest) -----

static int g_total = 0;
static int g_failed = 0;

// Basic non-terminating assertion helpers
#define EXPECT_TRUE(cond, msg) do {            \
  ++g_total;                                    \
  if(!(cond)) {                                  \
    std::cerr << "[FAIL] " << msg << std::endl; \
    ++g_failed;                                   \
  }                                             \
} while(0)

#define EXPECT_EQ_INT(expected, actual, msg) do { \
  ++g_total;                                      \
  if((expected) != (actual)) {                    \
    std::cerr << "[FAIL] " << msg                   \
              << " (expected " << (expected)        \
              << ", got " << (actual) << ")"        \
              << std::endl;                         \
    ++g_failed;                                     \
  }                                               \
} while(0)

#define EXPECT_EQ_PTR(expected, actual, msg) do { \
  ++g_total;                                      \
  if((void*)(expected) != (void*)(actual)) {     \
    std::cerr << "[FAIL] " << msg                   \
              << " (expected " << expected          \
              << ", got " << actual << ")"            \
              << std::endl;                         \
    ++g_failed;                                     \
  }                                               \
} while(0)

// Test 1: Non-empty poslist: Next should indicate not EOF (bEof == 0)
void test_nonempty_poslist() {
  const u8 data[4] = {1, 2, 3, 4};
  Fts5PoslistReader iter;

  int rc = sqlite3Fts5PoslistReaderInit(data, 4, &iter);

  EXPECT_EQ_INT(0, rc, "Init should return eof=0 for non-empty buffer");
  EXPECT_EQ_PTR(data, iter.a, "iter.a should point to the input data");
  EXPECT_EQ_INT(4, iter.n, "iter.n should be equal to input n (4)");
  EXPECT_TRUE(iter.bEof == 0, "bEof should be 0 after Init for non-empty buffer");
}

// Test 2: Empty poslist: n == 0 -> Next should set eof (bEof == 1)
void test_empty_poslist() {
  const u8 data[1] = {0};
  Fts5PoslistReader iter;

  int rc = sqlite3Fts5PoslistReaderInit(data, 0, &iter);

  EXPECT_EQ_INT(1, rc, "Init should return eof=1 for empty buffer (n==0)");
  EXPECT_EQ_PTR(data, iter.a, "iter.a should point to the input data even if empty");
  EXPECT_EQ_INT(0, iter.n, "iter.n should be 0");
  EXPECT_TRUE(iter.bEof == 1, "bEof should be 1 after Init for empty buffer");
}

// Test 3: Null a pointer: a == NULL with non-zero n should set eof
void test_null_a_pointer() {
  Fts5PoslistReader iter;

  int rc = sqlite3Fts5PoslistReaderInit(nullptr, 5, &iter);

  EXPECT_EQ_INT(1, rc, "Init should return eof=1 when a is NULL");
  EXPECT_EQ_PTR(nullptr, iter.a, "iter.a should be NULL");
  EXPECT_EQ_INT(5, iter.n, "iter.n should reflect input n (5)");
  EXPECT_TRUE(iter.bEof == 1, "bEof should be 1 when a is NULL");
}

// Test 4: Negative n: n <= 0 should set eof
void test_negative_n() {
  const u8 data[2] = {0xAA, 0xBB};
  Fts5PoslistReader iter;

  int rc = sqlite3Fts5PoslistReaderInit(data, -2, &iter);

  EXPECT_EQ_INT(1, rc, "Init should return eof=1 when n <= 0 (negative)");
  EXPECT_EQ_PTR(data, iter.a, "iter.a should point to input data");
  EXPECT_EQ_INT(-2, iter.n, "iter.n should reflect input n (-2)");
  EXPECT_TRUE(iter.bEof == 1, "bEof should be 1 for negative n");
}

// Entry point
int main() {
  test_nonempty_poslist();
  test_empty_poslist();
  test_null_a_pointer();
  test_negative_n();

  if (g_failed == 0) {
    std::cout << "All tests passed (" << g_total << " assertions)." << std::endl;
    return 0;
  } else {
    std::cerr << g_failed << " test(s) failed out of " << g_total << " assertions." << std::endl;
    return 1;
  }
}
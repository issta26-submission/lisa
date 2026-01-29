// Lightweight C++11 unit test harness for sqlite3_str_appendchar
// This test suite provides a minimal in-memory environment to exercise
// the focal method and its dependent enlargement routine.
// The implementation intentionally mirrors the behavior described in the
// provided FOCAL_METHOD and uses a simplified, self-contained mock of the
// sqlite3_str and StrAccum structures.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge utilities (minimal, self-contained)
typedef long long i64;
typedef unsigned char u8;

// Minimal type declarations to model the sqlite3_str and its embedding StrAccum
typedef struct StrAccum StrAccum;
struct StrAccum {
  int dummy; // unused in test harness; present to illustrate embedding
};

// sqlite3_str is the structure used by the focal method
typedef struct sqlite3_str {
  StrAccum s;      // Embedding to align with actual SQLite structure layout
  int nChar;         // current length of text in p->zText
  int nAlloc;        // allocated size of p->zText
  char *zText;        // text buffer
} sqlite3_str;

// Forward declarations with C linkage to mimic the original C-style API
extern "C" {
  // Focal method under test
  void sqlite3_str_appendchar(sqlite3_str *p, int N, char c);

  // Dependency used by the focal method
  int sqlite3StrAccumEnlarge(StrAccum *p, i64 N);
}

// Lightweight testcase marker (no-ops, but keeps flow identical to original code)
static inline void testcase(int) {}

// Global test results
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Convenience assertion helpers (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
  g_total_tests++; \
  if (cond) { g_passed_tests++; } else { \
    std::cerr << "Test FAILED: " << (msg) << " (Line " << __LINE__ << ")\n"; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  g_total_tests++; \
  if (static_cast<long long>(a) == static_cast<long long>(b)) { \
    g_passed_tests++; \
  } else { \
    std::cerr << "Test FAILED: " << (msg) << " Expected " << (b) << " but got " << (a) \
              << " (Line " << __LINE__ << ")\n"; \
  } \
} while(0)

// Implementation scaffolding for the focal method and its dependency.
// These definitions are provided solely for the unit tests to run in isolation.

static inline void init_sqlite3_str(sqlite3_str *p, int initialAlloc) {
  p->nChar = 0;
  p->nAlloc = initialAlloc;
  if (initialAlloc > 0) {
    p->zText = new char[initialAlloc];
    std::memset(p->zText, 0, initialAlloc);
  } else {
    p->zText = nullptr;
  }
}

// Cleanup to avoid leaks during tests
static inline void free_sqlite3_str(sqlite3_str *p) {
  if (p) {
    delete[] p->zText;
    p->zText = nullptr;
    p->nChar = 0;
    p->nAlloc = 0;
  }
}

// Mock enlargement function to emulate SQLite's strategy.
// It is designed to be deterministic for tests and to exercise both branches.
extern "C" int sqlite3StrAccumEnlarge(StrAccum *p, i64 N) {
  // We rely on the embedding sqlite3_str object being the first member
  sqlite3_str *acc = (sqlite3_str*)p;

  i64 needed = acc->nChar + N;

  // No enlargement needed
  if (needed <= acc->nAlloc) {
    return (int)N; // keep N unchanged
  }

  // Failure path (trigger early return in focal method)
  // Chosen threshold to enable a controlled false branch in tests
  if (needed > 50) {
    return 0; // signal failure
  }

  // Grow buffer
  int newAlloc = acc->nAlloc ? acc->nAlloc : 1;
  while (newAlloc < needed) newAlloc *= 2;

  char *newz = new char[newAlloc];
  if (acc->zText && acc->nChar > 0) {
    std::memcpy(newz, acc->zText, acc->nChar);
  }
  delete[] acc->zText;
  acc->zText = newz;
  acc->nAlloc = newAlloc;

  return (int)N;
}

// The focal method under test (copied here to enable isolated unit tests in absence of full project build)
extern "C" void sqlite3_str_appendchar(sqlite3_str *p, int N, char c) {
  testcase( p->nChar + (i64)N > 0x7fffffff );
  if( p->nChar+(i64)N >= p->nAlloc && (N = sqlite3StrAccumEnlarge(p, N))<=0 ){
    return;
  }
  while( (N--)>0 ) p->zText[p->nChar++] = c;
}

// Test harness: individual tests

static void test_basic_append_within_allocation() {
  sqlite3_str p;
  init_sqlite3_str(&p, 10); // initial allocation sufficient for N
  // Ensure buffer is filled with known values
  for (int i = 0; i < 10; ++i) p.zText[i] = 0;

  sqlite3_str_appendchar(&p, 5, 'A');

  EXPECT_EQ(p.nChar, 5, "basic append should increment nChar by N");
  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(p.zText[i] == 'A', "buffer position should contain appended character 'A'");
  }

  free_sqlite3_str(&p);
}

static void test_enlarge_and_append() {
  sqlite3_str p;
  init_sqlite3_str(&p, 10);
  // Seed first 8 characters
  p.nChar = 8;
  p.zText[0] = 'x'; p.zText[1] = 'y'; p.zText[2] = 'z';
  for (int i = 3; i < 8; ++i) p.zText[i] = '0' + i;

  sqlite3_str_appendchar(&p, 5, 'B'); // should trigger enlargement and then append

  EXPECT_EQ(p.nChar, 13, "after enlargement and append, nChar should be 13");
  for (int i = 8; i < 13; ++i) {
    EXPECT_TRUE(p.zText[i] == 'B', "newly appended region should contain 'B'");
  }
  // Verify pre-existing content preserved
  EXPECT_TRUE(p.zText[0] == 'x' && p.zText[1] == 'y' && p.zText[2] == 'z', "existing content preserved after enlarge");

  free_sqlite3_str(&p);
}

static void test_enlarge_failure_branch() {
  sqlite3_str p;
  init_sqlite3_str(&p, 10);
  // Start with empty buffer
  p.nChar = 0;
  // Prepare a non-null buffer to observe that it remains untouched on failure
  p.zText[0] = 'C';
  p.zText[1] = 'D';
  p.zText[2] = 'E';
  p.zText[3] = 'F';

  // Use a large N to force enlargement failure (per our enlarge mock, threshold is 50)
  sqlite3_str_appendchar(&p, 60, 'X'); // should fail and not modify nChar

  EXPECT_EQ(p.nChar, 0, "on enlarge failure, nChar should remain unchanged");
  // Ensure existing content is still intact
  EXPECT_TRUE(p.zText[0] == 'C', "existing buffer content should be preserved on failure");

  free_sqlite3_str(&p);
}

static void test_zero_N_no_change() {
  sqlite3_str p;
  init_sqlite3_str(&p, 4);
  p.nChar = 2;
  p.zText[0] = 'M'; p.zText[1] = 'N';
  // Call with N = 0; no change expected
  sqlite3_str_appendchar(&p, 0, 'Z');
  EXPECT_EQ(p.nChar, 2, "N=0 should not change nChar");
  EXPECT_TRUE(p.zText[0] == 'M' && p.zText[1] == 'N', "buffer should remain unchanged when N=0");
  free_sqlite3_str(&p);
}

static void test_multiple_appends_sequence() {
  sqlite3_str p;
  init_sqlite3_str(&p, 8);
  // First append
  sqlite3_str_appendchar(&p, 3, 'A'); // nChar = 3
  // Second append
  sqlite3_str_appendchar(&p, 4, 'B'); // nChar = 7
  // Validate
  EXPECT_EQ(p.nChar, 7, "sequence of two appends should result in nChar = 7");
  for (int i = 0; i < 3; ++i) EXPECT_TRUE(p.zText[i] == 'A', "prefix should contain 'A's");
  for (int i = 3; i < 7; ++i) EXPECT_TRUE(p.zText[i] == 'B', "suffix should contain 'B's");

  free_sqlite3_str(&p);
}

// Main harness
int main() {
  test_basic_append_within_allocation();
  test_enlarge_and_append();
  test_enlarge_failure_branch();
  test_zero_N_no_change();
  test_multiple_appends_sequence();

  std::cout << "Tests completed. Passed " << g_passed_tests << " of " << g_total_tests << " tests." << std::endl;
  return (g_passed_tests == g_total_tests) ? 0 : 1;
}
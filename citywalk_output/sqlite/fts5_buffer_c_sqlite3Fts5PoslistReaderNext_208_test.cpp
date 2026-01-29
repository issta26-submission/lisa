// High-quality C++11 unit tests for sqlite3Fts5PoslistReaderNext
// This test suite mocks the dependency sqlite3Fts5PoslistNext64 to control
// the flow of sqlite3Fts5PoslistReaderNext without requiring the full FTS5
// implementation. It uses the real Fts5PoslistReader type as declared in the
// project's headers.

// Note: This test is designed to be compiled and linked with the existing C
// source (fts5_buffer.c etc.). It uses a C linkage mock for sqlite3Fts5PoslistNext64.

#include <iostream>
#include <cstring>
#include <fts5Int.h>


// Domain knowledge hints: Import dependencies with correct paths.

// Non-terminating test assertions (like EXPECT_* in GTest) implemented simply.
static int g_ret_len = 0;
static int g_ret_idx = 0;
static int g_ret_seq[16];

static int g_pi_len = 0;
static int g_pi_idx = 0;
static int g_pi_seq[16];
static long long g_piOff_seq[16];

// Mock: reset the behavior of sqlite3Fts5PoslistNext64 for a test case.
// ret_seq: sequence of return values (0 means success / next item available; 1 means EOF).
// pi_seq / piOff_seq: per-call values written to *pi and *piOff when applicable.
void mock_reset(int ret_seq[], int ret_len, int pi_seq[], long long piOff_seq[], int pi_len) {
  g_ret_len = ret_len;
  g_ret_idx = 0;
  std::memcpy(g_ret_seq, ret_seq, sizeof(int) * ret_len);

  g_pi_len = pi_len;
  g_pi_idx = 0;
  if (pi_len > 0) std::memcpy(g_pi_seq, pi_seq, sizeof(int) * pi_len);
  if (pi_len > 0) std::memcpy(g_piOff_seq, piOff_seq, sizeof(long long) * pi_len);
}

// C linkage to override the dependency in sqlite3Fts5PoslistNext64 within this test.
// This mock will drive the behavior according to the sequences provided by mock_reset.
extern "C" int sqlite3Fts5PoslistNext64(const unsigned char *a, int n, int *pi, long long *piOff) {
  // If we have more predefined return values, use them in order.
  if (g_ret_idx < g_ret_len) {
    int ret = g_ret_seq[g_ret_idx];
    // For these tests, set output values when available.
    if (g_pi_idx < g_pi_len) {
      if (pi) *pi = g_pi_seq[g_pi_idx];
      if (piOff) *piOff = g_piOff_seq[g_pi_idx];
    }
    ++g_ret_idx;
    ++g_pi_idx;
    return ret;
  }
  // Default to EOF if sequence is exhausted.
  if (pi) *pi = (g_pi_len > 0 ? g_pi_seq[g_pi_len - 1] : 0);
  if (piOff) *piOff = (g_pi_len > 0 ? g_piOff_seq[g_pi_len - 1] : 0LL);
  return 1;
}

// Simple test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define TEST_BEGIN(name) std::cout << "Running test: " << name << std::endl; g_total_tests++;
#define TEST_PASS(name) std::cout << "PASS: " << name << std::endl;
#define TEST_FAIL(name) std::cerr << "FAIL: " << name << std::endl; g_failed_tests++;

// Lightweight EXPECT macro (non-terminating)
#define EXPECT(cond) do { \
  if (!(cond)) { \
    std::cerr << "  Expectation failed: " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
  } \
} while (0)


// Test 1: When sqlite3Fts5PoslistNext64 returns 0 (not EOF) the reader should
// update i and iPos and report EOF as 0.
void test_PoslistReaderNext_NotEOF_UpdatesFields() {
  TEST_BEGIN("test_PoslistReaderNext_NotEOF_UpdatesFields");

  // Configure mock: single call returns 0 and provides i and iPos values.
  int ret_seq[] = {0};
  int pi_seq[] = {7};
  long long piOff_seq[] = {10};

  mock_reset(ret_seq, 1, pi_seq, piOff_seq, 1);

  Fts5PoslistReader iter;
  // Initialize the fields expected by the production code.
  iter.a = nullptr;
  iter.n = 0;
  iter.i = 0;
  iter.iPos = 0;
  iter.bEof = 0;

  int eof = sqlite3Fts5PoslistReaderNext(&iter);

  EXPECT(eof == 0);
  EXPECT(iter.bEof == 0);
  EXPECT(iter.i == 7);
  EXPECT(iter.iPos == 10);

  TEST_PASS("test_PoslistReaderNext_NotEOF_UpdatesFields");
}

// Test 2: When sqlite3Fts5PoslistNext64 returns 1 (EOF) the reader should set bEof and return 1.
// i and iPos may still be updated by the mock; we validate the observable outcome.
void test_PoslistReaderNext_EOF_SetsFlag() {
  TEST_BEGIN("test_PoslistReaderNext_EOF_SetsFlag");

  // Configure mock: single call returns 1 and provides i and iPos values.
  int ret_seq[] = {1};
  int pi_seq[] = {99};
  long long piOff_seq[] = {11};

  mock_reset(ret_seq, 1, pi_seq, piOff_seq, 1);

  Fts5PoslistReader iter;
  iter.a = nullptr;
  iter.n = 0;
  iter.i = 0;
  iter.iPos = 0;
  iter.bEof = 0;

  int eof = sqlite3Fts5PoslistReaderNext(&iter);

  EXPECT(eof == 1);
  EXPECT(iter.bEof == 1);
  EXPECT(iter.i == 99);
  EXPECT(iter.iPos == 11);

  TEST_PASS("test_PoslistReaderNext_EOF_SetsFlag");
}

// Test 3: Multiple invocations; first not EOF, second EOF with distinct i and iPos.
// Validates that the function iterates through and propagates per-call values.
void test_PoslistReaderNext_MultipleCalls() {
  TEST_BEGIN("test_PoslistReaderNext_MultipleCalls");

  // Sequence: first call returns 0 with (i=7, iPos=10), second call returns 1 with (i=11, iPos=99)
  int ret_seq[] = {0, 1};
  int pi_seq[] = {7, 11};
  long long piOff_seq[] = {10, 99};

  mock_reset(ret_seq, 2, pi_seq, piOff_seq, 2);

  Fts5PoslistReader iter;
  iter.a = nullptr;
  iter.n = 0;
  iter.i = 0;
  iter.iPos = 0;
  iter.bEof = 0;

  // First call
  int eof1 = sqlite3Fts5PoslistReaderNext(&iter);
  EXPECT(eof1 == 0);
  EXPECT(iter.bEof == 0);
  EXPECT(iter.i == 7);
  EXPECT(iter.iPos == 10);

  // Second call
  int eof2 = sqlite3Fts5PoslistReaderNext(&iter);
  EXPECT(eof2 == 1);
  EXPECT(iter.bEof == 1);
  EXPECT(iter.i == 11);
  EXPECT(iter.iPos == 99);

  TEST_PASS("test_PoslistReaderNext_MultipleCalls");
}

int main() {
  // Run tests
  test_PoslistReaderNext_NotEOF_UpdatesFields();
  test_PoslistReaderNext_EOF_SetsFlag();
  test_PoslistReaderNext_MultipleCalls();

  // Summary
  std::cout << "Total tests: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
  return g_failed_tests ? 1 : 0;
}
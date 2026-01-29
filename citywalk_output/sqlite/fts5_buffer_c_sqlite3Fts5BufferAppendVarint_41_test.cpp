#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fts5Int.h>


// Include production headers to access Fts5Buffer and prototypes.
// This header is expected to be available with the project under test.

extern "C" {
  // Prototypes for functions used in tests.
  // The actual project provides these; if signatures differ slightly, adapt accordingly.
  // sqlite3Fts5PutVarint writes a varint for iVal into p and returns number of bytes written.
  int sqlite3Fts5PutVarint(unsigned char *p, long long iVal);

  // The function under test: appends a varint-encoded iVal to the buffer pBuf.
  void sqlite3Fts5BufferAppendVarint(int *pRc, Fts5Buffer *pBuf, long long iVal);

  // Helper to free buffer memory if needed by the production API.
  void sqlite3Fts5BufferFree(Fts5Buffer *pBuf);
}

// Simple non-terminating test framework (GTest-like style but lightweight)
static int g_failCount = 0;

// EXPECT_TRUE: non-terminating assertion. Logs message on failure but continues.
#define EXPECT_TRUE(cond, msg)                                   \
  do {                                                            \
    if(!(cond)) {                                                 \
      printf("EXPECT_TRUE FAILED: %s\n\tIn test: %s\n", msg, __func__); \
      ++g_failCount;                                              \
    }                                                             \
  } while(0)

// Helper to initialize a buffer to a clean empty state.
static void initBuffer(Fts5Buffer *pBuf) {
  if(pBuf) {
    pBuf->p = NULL;
    pBuf->n = 0;
  }
}

// Test 1: Basic append with zero value should increase buffer by exactly 1 byte
// and the appended byte should match the varint encoding of 0.
static void test_basic_append_zero() {
  printf("Running test_basic_append_zero: verify single-byte encoding for iVal=0\n");
  int rc = 0;
  Fts5Buffer buf;
  initBuffer(&buf);

  unsigned char encoded[9];
  int encLen = sqlite3Fts5PutVarint(encoded, 0); // encode 0 into a temp buffer
  int oldN = buf.n;

  sqlite3Fts5BufferAppendVarint(&rc, &buf, 0);

  EXPECT_TRUE(rc == 0, "rc should be 0 after successful append");
  EXPECT_TRUE(buf.n == oldN + encLen, "buffer length should increase by encoded length");
  if(buf.n == oldN + encLen) {
    // verify bytes written match the encoding produced by PutVarint
    if(buf.p != NULL) {
      int cmp = memcmp(&buf.p[oldN], encoded, encLen);
      EXPECT_TRUE(cmp == 0, "appended bytes must match PutVarint encoding of 0");
    } else {
      EXPECT_TRUE(false, "buffer memory pointer unexpectedly NULL after growth");
    }
  }

  sqlite3Fts5BufferFree(&buf);
}

// Test 2: Validate that encoding for multiple representative values is consistent
// with direct PutVarint, ensuring the append uses PutVarint under the hood.
static void test_various_values_consistency() {
  printf("Running test_various_values_consistency: cross-validate multiple iVal values\n");
  const long long testVals[] = {0, 1, 127, 128, 255, 256, 1024, 65535, 65536, 1234567890123LL, (1LL<<60)};
  const int nVals = sizeof(testVals) / sizeof(testVals[0]);

  int rc = 0;
  Fts5Buffer buf;
  initBuffer(&buf);

  unsigned char encoded[16];
  for(int i = 0; i < nVals; ++i) {
    long long val = testVals[i];
    int oldN = buf.n;

    int encLen = sqlite3Fts5PutVarint(encoded, val);
    // Append via the function under test
    sqlite3Fts5BufferAppendVarint(&rc, &buf, val);

    EXPECT_TRUE(rc == 0, "rc should be 0 after append for value index " // non-terminating
                             );
    // rc could be non-zero if the underlying grow failed; treat as failure
    EXPECT_TRUE(buf.n == oldN + encLen, "buffer length should increase by PutVarint's length for this value");

    if(buf.n == oldN + encLen) {
      if(buf.p != NULL) {
        int cmp = memcmp(&buf.p[oldN], encoded, encLen);
        EXPECT_TRUE(cmp == 0, "appended bytes should match PutVarint encoding for this value");
      } else {
        EXPECT_TRUE(false, "buffer memory pointer unexpectedly NULL after growth");
      }
    }

    // prepare for next iteration
    // (no reset between iterations to simulate continuous appends)
  }

  sqlite3Fts5BufferFree(&buf);
}

// Test 3: Error path: simulate a failure in fts5BufferGrow by signaling an error via pRc.
// The focal function should return early and not modify the buffer.
static void test_error_path_no_append_when_grow_fails() {
  printf("Running test_error_path_no_append_when_grow_fails: simulate grow failure via rc flag\n");
  int rc = 1; // simulate pre-existing error state
  Fts5Buffer buf;
  initBuffer(&buf);

  int oldN = buf.n;
  sqlite3Fts5BufferAppendVarint(&rc, &buf, 12345);

  // Expect no changes due to early return caused by non-zero rc
  EXPECT_TRUE(rc != 0, "rc should remain non-zero when grow fails");
  EXPECT_TRUE(buf.n == oldN, "buffer length should remain unchanged when grow fails");
  sqlite3Fts5BufferFree(&buf);
}

// Test 4: Multiple appends should concatenate varints correctly.
// We compare each appended segment with the independently encoded varint.
static void test_multiple_appends_concatenation() {
  printf("Running test_multiple_appends_concatenation: ensure concatenation correctness\n");
  int rc = 0;
  Fts5Buffer buf;
  initBuffer(&buf);

  const long long values[] = {0, 1, 127, 128, 255, 1024, 65535, 65536, 1234567890123LL};
  const int n = sizeof(values)/sizeof(values[0]);
  unsigned char encoded[16];
  for(int i = 0; i < n; ++i) {
    long long v = values[i];
    int oldN = buf.n;
    int len = sqlite3Fts5PutVarint(encoded, v);
    sqlite3Fts5BufferAppendVarint(&rc, &buf, v);

    EXPECT_TRUE(rc == 0, "rc should be 0 after append in multi-append test");
    EXPECT_TRUE(buf.n == oldN + len, "after each append, n should increase by encoded length");
    if(buf.n == oldN + len) {
      if(buf.p != NULL) {
        int cmp = memcmp(&buf.p[oldN], encoded, len);
        EXPECT_TRUE(cmp == 0, "each appended segment should match independently encoded variant");
      } else {
        EXPECT_TRUE(false, "buffer memory pointer unexpectedly NULL after growth");
      }
    }
  }

  sqlite3Fts5BufferFree(&buf);
}

// Entry point: run all tests and report summary
int main() {
  printf("Starting tests for sqlite3Fts5BufferAppendVarint and related components...\n");

  test_basic_append_zero();
  test_various_values_consistency();
  test_error_path_no_append_when_grow_fails();
  test_multiple_appends_concatenation();

  if(g_failCount == 0) {
    printf("All tests PASSED. Total tests: 4\n");
  } else {
    printf("Tests finished with failures: %d\n", g_failCount);
  }
  return g_failCount ? 1 : 0;
}
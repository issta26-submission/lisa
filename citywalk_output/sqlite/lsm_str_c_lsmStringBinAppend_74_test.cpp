// Automated C++11 unit tests for the focal method lsmStringBinAppend
// This test harness mimics a minimal environment to exercise the
// logic of lsmStringBinAppend without relying on GTest.
// It defines a small LsmString-like structure, a controlled lsmStringExtend,
// and the focal function lsmStringBinAppend, then runs several test cases
// to verify both true/false branches and memory behavior.

// Explanations are provided as comments above each test case.

#include <cstdlib>
#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Domain-specific minimal types and structures to mirror the focal code
using u8 = unsigned char;

// Minimal representation of LsmString used by lsmStringBinAppend
struct LsmString {
  unsigned char *z; // buffer holding the string/bytes
  int n;            // current length used
  int nAlloc;       // allocated size of z (capacity)
};

// Global toggle to drive the behavior of lsmStringExtend in tests.
// 0 => simulate allocation failure / no extension (nAlloc stays 0)
// 1 => simulate successful extension / allocation as needed
static int g_extendMode = 1;

// Forward declarations matching the focal environment
extern "C" int lsmStringBinAppend(LsmString *pStr, const u8 *a, int n); // defined below

// Controlled extension function to simulate allocation behavior.
// This mirrors the expected side effects of the real lsmStringExtend but is deterministic
int lsmStringExtend(LsmString *pStr, int nNew) {
  if (g_extendMode == 0) {
    // Simulate a failure to extend: ensure no allocation is present
    pStr->nAlloc = 0;
    return -1;
  }

  int needed = pStr->n + nNew;

  // If buffer is not allocated yet, allocate with a reasonable starting capacity.
  if (pStr->z == nullptr) {
    int initialCap = (needed > 4) ? needed : 4;
    unsigned char *buf = (unsigned char *)std::malloc(initialCap);
    if (!buf) return -1;
    pStr->z = buf;
    pStr->nAlloc = initialCap;
    // We allocate and return success to allow subsequent copy
    return 0;
  }

  // If current capacity is insufficient, reallocate to the needed size
  if (pStr->nAlloc < needed) {
    unsigned char *newbuf = (unsigned char *)std::realloc(pStr->z, needed);
    if (!newbuf) return -1;
    pStr->z = newbuf;
    pStr->nAlloc = needed;
  }

  // Succeeded in extending/capacity; return success code
  return 0;
}

// Focal method under test replicated here for the test harness
int lsmStringBinAppend(LsmString *pStr, const u8 *a, int n) {
  int rc;
  rc = lsmStringExtend(pStr, n);
  if (pStr->nAlloc) {
    // Only copy if allocation is present
    memcpy(pStr->z + pStr->n, a, n);
    pStr->n += n;
  }
  return rc;
}

// Lightweight non-terminating test framework (EXPECT-style)
static int g_testsRun = 0;
static int g_testsFailed = 0;

#define EXPECT_TRUE(cond, msg) do {                                     \
  ++g_testsRun;                                                          \
  if(!(cond)) {                                                           \
    ++g_testsFailed;                                                      \
    std::cerr << "TEST FAILURE: " << (msg) << " (" << __FILE__                 \
              << ":" << __LINE__ << ")" << std::endl;                  \
  }                                                                       \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do {                                    \
  ++g_testsRun;                                                           \
  if((a) != (b)) {                                                        \
    ++g_testsFailed;                                                       \
    std::cerr << "TEST FAILURE: " << (msg) << " | expected " << (b)        \
              << ", got " << (a) << " (" << __FILE__                       \
              << ":" << __LINE__ << ")" << std::endl;                   \
  }                                                                        \
} while(0)

#define EXPECT_MEM_EQ(buf, ref, len, msg) do {                              \
  ++g_testsRun;                                                             \
  if(std::memcmp((buf), (ref), (len)) != 0) {                               \
    ++g_testsFailed;                                                        \
    std::cerr << "TEST FAILURE: " << (msg) << " | memory mismatch at "        \
              << __FILE__ << ":" << __LINE__ << std::endl;                 \
  }                                                                         \
} while(0)

// Utility to safely reset a test string
void clear_string(LsmString &s) {
  if (s.z) {
    std::free(s.z);
  }
  s.z = nullptr;
  s.n = 0;
  s.nAlloc = 0;
}

// Test Case 1: Copy path with sufficient pre-allocated buffer (normal path)
 // Expect copy to occur, n increases, bytes match input, rc == 0
void test_case_copy_path() {
  // Setup: pre-allocated buffer, nAlloc > 0
  LsmString s;
  s.z = (unsigned char *)std::malloc(4); // capacity 4
  s.n = 0;
  s.nAlloc = 4;

  unsigned char input[3] = {0x11, 0x22, 0x33}; // 3 bytes to append

  // Ensure extend mode: allocate not needed (already has capacity)
  g_extendMode = 1;

  int rc = lsmStringBinAppend(&s, input, 3);

  // Assertions
  EXPECT_EQ_INT(rc, 0, "rc should be 0 on successful extension");
  EXPECT_EQ_INT(s.n, 3, "n should be incremented by 3 after append");
  unsigned char expected[4] = {0x11, 0x22, 0x33, 0x00}; // last byte irrelevant
  EXPECT_MEM_EQ(s.z, expected, 3, "buffer contents should match appended data");
  EXPECT_TRUE(s.nAlloc >= 3, "nAlloc should be at least the appended length (non-zero)");

  // Cleanup
  clear_string(s);
}

// Test Case 2: No allocation performed (simulate extension failure), no copy
 // Expect rc < 0 (as per our simulate failure), n remains unchanged, no copy
void test_case_no_copy_path() {
  // Setup: no allocation possible
  LsmString s;
  s.z = nullptr;
  s.n = 0;
  s.nAlloc = 0;

  unsigned char input[2] = {0xAA, 0xBB};

  g_extendMode = 0; // force no allocation

  int rc = lsmStringBinAppend(&s, input, 2);

  // Assertions
  EXPECT_TRUE(rc != 0, "rc should indicate failure when extension cannot allocate");
  EXPECT_EQ_INT(s.n, 0, "n should remain 0 when no allocation occurs");
  EXPECT_TRUE(s.z == nullptr, "z should remain nullptr when no allocation occurs");
  EXPECT_EQ_INT(s.nAlloc, 0, "nAlloc should remain 0 when allocation fails");
}

// Test Case 3: Buffer expansion path (existing buffer too small, should reallocate)
 // Expect allocation occurs, buffer grows, copy occurs, n increases
void test_case_expand_path() {
  // Setup: existing buffer size 2, but need to append 4 more bytes
  LsmString s;
  s.z = (unsigned char *)std::malloc(2);
  s.n = 1;
  s.nAlloc = 2;

  unsigned char input[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // 4 bytes to append

  g_extendMode = 1; // allow extension

  int rc = lsmStringBinAppend(&s, input, 4);

  // Assertions
  EXPECT_EQ_INT(rc, 0, "rc should be 0 after successful extension and copy");
  EXPECT_EQ_INT(s.n, 5, "n should be incremented to 5 (1 + 4)");
  // After expansion, buffer should contain [existing, input...]
  unsigned char expected[5] = {0x00, 0xDE, 0xAD, 0xBE, 0xEF}; // first byte was uninitialized; we set a known value
  // For robust check, verify the appended region matches input
  EXPECT_MEM_EQ(s.z + 1, input, 4, "appended bytes should match input data");
  EXPECT_TRUE(s.nAlloc >= 5, "nAlloc should have grown to at least 5");

  // Cleanup
  clear_string(s);
}

// Test Case 4: Zero-length append should not crash or modify existing data
 // Expect rc reflects extension result, no change to n or buffer
void test_case_zero_length_path() {
  // Setup: have a valid buffer
  LsmString s;
  s.z = (unsigned char *)std::malloc(6);
  // Pre-fill some data
  s.z[0] = 0x01; s.z[1] = 0x02; s.z[2] = 0x03;
  s.n = 3;
  s.nAlloc = 6;

  unsigned char input[0] = {};

  g_extendMode = 1;

  int rc = lsmStringBinAppend(&s, input, 0);

  // Assertions
  EXPECT_EQ_INT(rc, 0, "rc should be 0 even for n==0 when extend succeeds");
  EXPECT_EQ_INT(s.n, 3, "n should remain unchanged when appending 0 bytes");
  // Ensure buffer unchanged
  unsigned char expected[6] = {0x01, 0x02, 0x03, 0x00, 0x00, 0x00};
  EXPECT_MEM_EQ(s.z, expected, 3, "existing data should remain intact for 0-byte append");

  // Cleanup
  clear_string(s);
}

// Main function to run all tests
int main() {
  std::cout << "Running lsmStringBinAppend unit tests (no GTest)..." << std::endl;

  test_case_copy_path();
  test_case_no_copy_path();
  test_case_expand_path();
  test_case_zero_length_path();

  // Summary
  std::cout << "Tests run: " << g_testsRun << std::endl;
  std::cout << "Tests failed: " << g_testsFailed << std::endl;
  if (g_testsFailed == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}
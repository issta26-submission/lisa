// Test suite for sqlite3Fts3VarintLen
// This test does not rely on Google Test. It uses a lightweight, non-terminating assertion framework.
// The focal method under test is:
//   int sqlite3Fts3VarintLen(sqlite3_uint64 v)
// which returns the number of bytes required to encode v as a varint (7 bits per byte).

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <iomanip>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Domain-specific alias to match the function signature in the focal code.
// sqlite3_uint64 is typically defined as an unsigned 64-bit type in the SQLite headers.
typedef uint64_t sqlite3_uint64;

// Declaration of the focal method with C linkage to ensure proper linkage when the
// function is defined in fts3.c (C file) and compiled alongside this test.
extern "C" int sqlite3Fts3VarintLen(sqlite3_uint64 v);

// Lightweight test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond) \
  do { \
    ++g_totalTests; \
    if (!(cond)) { \
      ++g_failedTests; \
      std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                << "  Condition failed: " << #cond << std::endl; \
    } \
  } while (0)

#define EXPECT_EQ(a, b) EXPECT_TRUE((a) == (b))
#define EXPECT_LE(a, b) EXPECT_TRUE((a) <= (b))

// Helper: print a summary after tests
static void PrintSummary() {
  std::cout << std::endl;
  std::cout << "Test Summary: " << g_totalTests << " tests run, "
            << (g_totalTests - g_failedTests) << " passed, "
            << g_failedTests << " failed." << std::endl;
}

// Test 1: Basic correctness for small values
// Verifies length for values within the first 1-byte range (0..127)
void test_varint_len_basic_small_values() {
  // 0 -> 1
  EXPECT_EQ(sqlite3Fts3VarintLen(0ULL), 1);
  // 1 -> 1
  EXPECT_EQ(sqlite3Fts3VarintLen(1ULL), 1);
  // 127 (0x7F) -> 1
  EXPECT_EQ(sqlite3Fts3VarintLen(127ULL), 1);
}

// Test 2: Boundary between 1-byte and 2-byte varint
// Verifies that values at the 2-byte boundary yield length 2
void test_varint_len_boundary_1_to_2() {
  // 128 (0x80) -> 2
  EXPECT_EQ(sqlite3Fts3VarintLen(128ULL), 2);
  // 16383 (2^14 - 1) -> 2
  EXPECT_EQ(sqlite3Fts3VarintLen(16383ULL), 2);
}

// Test 3: Boundary between 2-byte and 3-byte varint
// Verifies that values at the 3-byte boundary yield length 3
void test_varint_len_boundary_2_to_3() {
  // 16384 (2^14) -> 3
  EXPECT_EQ(sqlite3Fts3VarintLen(16384ULL), 3);
  // (2^21 - 1) -> 3
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)((1ULL << 21) - 1)), 3);
}

// Test 4: Boundary between 3-byte and 4-byte varint
void test_varint_len_boundary_3_to_4() {
  // 2^21 -> 4
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)1ULL << 21), 4);
  // (2^21) - 1 -> 3 (covered by prev test), ensure transition to 4 at exact boundary
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)((1ULL << 21) - 1)), 3);
}

// Test 5: Maximum width for 64-bit value
// Verifies the function reports 10 bytes for the full 64-bit span
void test_varint_len_max_width() {
  // 2^63 -> 10
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)1ULL << 63), 10);
  // All-ones (2^64 - 1) -> 10
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)(~0ULL)), 10);
  // Sanity check: 2^63 - 1 -> 10 as well
  EXPECT_EQ(sqlite3Fts3VarintLen((sqlite3_uint64)((1ULL << 63) - 1)), 10);
}

// Test 6: Monotonicity checks (additional coverage)
// Verifies that non-decreasing values produce non-decreasing lengths
void test_varint_len_monotonicity() {
  int a = sqlite3Fts3VarintLen(0ULL);
  int b = sqlite3Fts3VarintLen(1ULL << 7);    // boundary to 2-byte range
  int c = sqlite3Fts3VarintLen(1ULL << 14);   // boundary to 3-byte range
  int d = sqlite3Fts3VarintLen(1ULL << 21);   // boundary to 4-byte range

  EXPECT_LE(a, b);
  EXPECT_LE(b, c);
  EXPECT_LE(c, d);
}

int main() {
  // Run tests
  test_varint_len_basic_small_values();
  test_varint_len_boundary_1_to_2();
  test_varint_len_boundary_2_to_3();
  test_varint_len_boundary_3_to_4();
  test_varint_len_max_width();
  test_varint_len_monotonicity();

  // Print summary
  PrintSummary();
  // Return non-zero if any test failed, to aid quick diagnosis when used in scripts
  return g_failedTests != 0 ? 1 : 0;
}
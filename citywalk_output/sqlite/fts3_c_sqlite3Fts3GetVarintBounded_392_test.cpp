// Unit test suite for sqlite3Fts3GetVarintBounded (C function) using plain C++11
// This test does not rely on GoogleTest or any external testing framework.
// It uses a minimal in-file test harness with non-terminating assertions to maximize coverage.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <cstdint>


// Step 1: Program Understanding (simplified in-comments for maintainers)
// - Focal function: sqlite3Fts3GetVarintBounded reads a SQLite-style varint from pBuf up to pEnd.
// - It accumulates the value in 'b' using 7 bits per byte, stopping when a byte with MSB 0 is read or after 64 bits.
// - It writes the result to *v and returns the number of bytes consumed (p - pStart).
// - If the buffer ends, missing bytes are treated as 0 (see c = p<pX ? *p : 0;).
// - This test suite exercises: single-byte varint, multi-byte varints, boundary conditions, and partial buffers.

// Step 2: Prepare function prototype and basic test harness utilities

// sqlite_int64 is a SQLite typedef; for testing we map it to long long.
typedef long long sqlite_int64;

// Declare the focal C function (link against fts3.c or equivalent during build)
extern "C" int sqlite3Fts3GetVarintBounded(const char *pBuf, const char *pEnd, sqlite_int64 *v);

// Simple non-terminating assertion framework
static int g_failures = 0;

static void log_fail(const char* test, const char* msg, long long expected, long long actual) {
    std::cerr << "[FAIL] " << test << ": " << msg
              << " (expected=" << expected << ", actual=" << actual << ")" << std::endl;
}

#define EXPECT_EQ(expected, actual) do { \
    if (static_cast<long long>(expected) != static_cast<long long>(actual)) { \
        log_fail(__FUNCTION__, "EXPECT_EQ failed", static_cast<long long>(expected), static_cast<long long>(actual)); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        log_fail(__FUNCTION__, "EXPECT_TRUE failed", 1, 0); \
        ++g_failures; \
    } \
} while(0)


// Step 3: Define individual test cases with explanatory comments

// Test Case 1: Single-byte varint (0..127)
static void test_single_byte() {
    // VARINT: 0x7F -> value 127, consumes 1 byte
    const uint8_t buf[1] = { 0x7F };
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded((const char*)buf, (const char*)(buf + 1), &v);

    EXPECT_EQ(1, consumed);
    EXPECT_EQ(127, v);
}

// Test Case 2: Two-byte varint representing 128
// Encoding: 0x80 0x01 -> 0x00 + (0x01 << 7) = 128
static void test_two_byte_128() {
    const uint8_t buf[2] = { 0x80, 0x01 };
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded((const char*)buf, (const char*)(buf + 2), &v);

    EXPECT_EQ(2, consumed);
    EXPECT_EQ(128, v);
}

// Test Case 3: Two-byte varint with maximum 7-bit segments in first byte
// Encoding: 0xFF 0x7F -> (0x7F) + (0x7F << 7) = 16383
static void test_two_byte_max7bit() {
    const uint8_t buf[2] = { 0xFF, 0x7F };
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded((const char*)buf, (const char*)(buf + 2), &v);

    EXPECT_EQ(2, consumed);
    EXPECT_EQ(16383, v);
}

// Test Case 4: Three-byte varint
// Encoding: 0x01, 0x02, 0x03 -> 1 + (2 << 7) + (3 << 14) = 49409
static void test_three_byte_varint() {
    const uint8_t buf[3] = { 0x01, 0x02, 0x03 };
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded((const char*)buf, (const char*)(buf + 3), &v);

    EXPECT_EQ(3, consumed);
    EXPECT_EQ(49409, v);
}

// Test Case 5: Empty buffer (pBuf == pEnd) should yield v = 0 and consume 1 byte
static void test_empty_buffer_handling() {
    // Use a dummy byte; end equals start
    uint8_t dummy = 0;
    const char* pBuf = (const char*)&dummy;
    const char* pEnd = (const char*)&dummy;
    sqlite_int64 v = -1;
    int consumed = sqlite3Fts3GetVarintBounded(pBuf, pEnd, &v);

    EXPECT_EQ(1, consumed);
    EXPECT_EQ(0, v);
}

// Test Case 6: Eight-byte long varint pattern to exercise the highest specified shift (2^49)
 // Encoding: 0x80 0x80 0x80 0x80 0x80 0x80 0x80 0x01 -> value = 1 << 49 = 562949953421312
static void test_eight_byte_pattern() {
    const uint8_t buf[8] = { 0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x01 };
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded((const char*)buf, (const char*)(buf + 8), &v);

    EXPECT_EQ(8, consumed);
    EXPECT_EQ(562949953421312LL, v);
}

// Test Case 7: Incomplete second byte scenario
// Encoding starts with 0x80 (meaning more bytes follow), but only one byte is provided in pBuf
// We set pEnd to buffer end after the first byte; the function should treat missing bytes as 0 and consume 2 bytes total.
static void test_incomplete_second_byte() {
    const uint8_t buf[1] = { 0x80 };        // MSB set, would require a second byte
    const char* pBuf = (const char*)buf;
    const char* pEnd = (const char*)buf + 1; // pretend only one byte available
    sqlite_int64 v = 0;
    int consumed = sqlite3Fts3GetVarintBounded(pBuf, pEnd, &v);

    EXPECT_EQ(2, consumed);
    EXPECT_EQ(0, v);
}


// Step 4: Entrypoint to run all tests
static void run_all_tests() {
    test_single_byte();
    test_two_byte_128();
    test_two_byte_max7bit();
    test_three_byte_varint();
    test_empty_buffer_handling();
    test_eight_byte_pattern();
    test_incomplete_second_byte();
}

// Step 5: Main to execute tests under normal program execution
int main() {
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "[PASS] All sqlite3Fts3GetVarintBounded tests passed." << std::endl;
    } else {
        std::cout << "[WARN] sqlite3Fts3GetVarintBounded tests completed with " << g_failures << " failure(s)." << std::endl;
    }

    // Return non-zero if any test failed
    return g_failures;
}
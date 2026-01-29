/*
Unit test suite for lsmVarintGet64 (wrapper around lsmSqlite4GetVarint64)
- Target: lsmVarintGet64(const u8 *aData, i64 *piVal)
- Environment: C++11, no GoogleTest; a lightweight harness is used.
- Assumptions:
  * lsmVarintGet64 returns the number of bytes consumed (as typical in SQLite-like varint parsers).
  * On success, piVal is filled with the decoded 64-bit value.
  * We cannot access the internal static helper directly; tests rely on observable behavior of lsmVarintGet64.
- Tests included:
  1) Zero input -> value 0, 1 byte consumed.
  2) Small value (1) -> value 1, 1 byte consumed.
  3) Two-byte varint (128) -> value 128, 2 bytes consumed.
  4) Incomplete/invalid long sequence (9 bytes with MSB all 1) -> expect non-success (ret == 0) and do not rely on piVal being modified.
- Note: This harness uses extern "C" to link with the C implementation of lsmVarintGet64.
*/

#include <functional>
#include <vector>
#include <lsmInt.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain knowledge: Define types to match expected C API used in lsm_varint.c
typedef unsigned char u8;
typedef long long i64;
typedef unsigned long long u64;

// Declaration of the focal function (C linkage)
extern "C" int lsmVarintGet64(const u8 *aData, i64 *piVal);

// Simple test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;
static std::vector<std::string> g_failureMessages;

// Record a test failure message
static void recordFailure(const std::string& testName, const std::string& message) {
    ++g_failedTests;
    g_failureMessages.push_back(testName + ": " + message);
}

// Basic assertion helpers (non-throwing)
#define ASSERT_EQ_INT(expected, actual, testName) do { \
    ++g_totalTests; \
    if ((expected) != (actual)) { \
        recordFailure(testName, "expected " #expected " but got " #actual); \
        return; \
    } \
} while (0)

#define ASSERT_EQ_I64(expected, actual, testName) do { \
    ++g_totalTests; \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "expected " << (expected) << " but got " << (actual); \
        recordFailure(testName, oss.str()); \
        return; \
    } \
} while (0)

#define ASSERT_TRUE(cond, testName) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        recordFailure(testName, "condition failed: " #cond); \
        return; \
    } \
} while(0)

// Test 1: Zero input
bool test_lsmVarintGet64_zero_input() {
    // Explanation:
    // aData = {0x00} should decode to piVal = 0 with 1 byte consumed.
    const u8 data[1] = {0x00};
    i64 value = -1; // ensure we notice any unintended writes on error
    int ret = lsmVarintGet64(data, &value);

    // Expect 1 byte consumed
    ASSERT_EQ_INT(1, ret, "test_lsmVarintGet64_zero_input/ret");

    // Expect decoded value 0
    ASSERT_EQ_I64(0, value, "test_lsmVarintGet64_zero_input/value");

    return true;
}

// Test 2: Small value (1)
bool test_lsmVarintGet64_small_value() {
    // Explanation:
    // aData = {0x01} should decode to piVal = 1 with 1 byte consumed.
    const u8 data[1] = {0x01};
    i64 value = -1;
    int ret = lsmVarintGet64(data, &value);

    // Expect 1 byte consumed
    ASSERT_EQ_INT(1, ret, "test_lsmVarintGet64_small_value/ret");

    // Expect decoded value 1
    ASSERT_EQ_I64(1, value, "test_lsmVarintGet64_small_value/value");

    return true;
}

// Test 3: Two-byte varint (128)
bool test_lsmVarintGet64_two_byte_value() {
    // Explanation:
    // aData = {0x80, 0x01} should decode to piVal = 128 with 2 bytes consumed.
    // 0x80 indicates continuation; 0x01 provides the next 7 bits.
    const u8 data[2] = {0x80, 0x01};
    i64 value = -1;
    int ret = lsmVarintGet64(data, &value);

    // Expect 2 bytes consumed
    ASSERT_EQ_INT(2, ret, "test_lsmVarintGet64_two_byte_value/ret");

    // Expect decoded value 128
    ASSERT_EQ_I64(128, value, "test_lsmVarintGet64_two_byte_value/value");

    return true;
}

// Test 4: Invalid/incomplete long sequence (9 bytes with MSB set on each byte)
// Explanation:
// This attempts to simulate an invalid long varint (no terminating byte with MSB=0).
// Expect the underlying function to report failure (ret == 0). piVal should not be assumed valid.
bool test_lsmVarintGet64_incomplete_sequence() {
    // 9 bytes, all with MSB set (0x80) -> no terminating byte
    const u8 data[9] = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    i64 value = -42; // preset to a distinct value to detect unexpected writes
    int ret = lsmVarintGet64(data, &value);

    // Expect failure (0 or non-success code)
    // We cannot guarantee exact error code across implementations, but
    // a reasonable expectation is ret == 0 for invalid input.
    if (ret == 0) {
        // Pass
        return true;
    } else {
        // If a non-zero is returned unexpectedly, still ensure that value is not corrupted
        // by documenting the behavior observed.
        std::string msg = "unexpected non-zero return for incomplete sequence: ret=" + std::to_string(ret) +
                          ", value=" + std::to_string(value);
        recordFailure("test_lsmVarintGet64_incomplete_sequence", msg);
        return false;
    }
}

// Minimal test runner
int main() {
    // Run tests
    test_lsmVarintGet64_zero_input();
    test_lsmVarintGet64_small_value();
    test_lsmVarintGet64_two_byte_value();
    test_lsmVarintGet64_incomplete_sequence();

    // Summary
    std::cout << "lsmVarintGet64 test summary:" << std::endl;
    std::cout << "  Total tests executed: " << g_totalTests << std::endl;
    std::cout << "  Failures: " << g_failedTests << std::endl;

    if (!g_failureMessages.empty()) {
        std::cout << "Failure details:" << std::endl;
        for (const auto& msg : g_failureMessages) {
            std::cout << " - " << msg << std::endl;
        }
    }

    // Return non-zero if any test failed
    return g_failedTests == 0 ? 0 : 1;
}
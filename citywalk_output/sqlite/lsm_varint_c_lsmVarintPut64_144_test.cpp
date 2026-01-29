// C++11 test suite for lsmVarintPut64
// Note: This test suite is written to be compiled in a C++11 project without GTest.
// It relies on the existing C implementation of lsmVarintPut64 provided by the focal method.
// The tests are non-terminating assertions: they log failures and continue execution to
// maximize code coverage.

#include <lsmInt.h>
#include <climits>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain knowledge and test scaffolding notes:
// - Focus: lsmVarintPut64(u8 *aData, i64 iVal)
 //   It should return an int (the number of bytes written by the underlying varint writer)
 //   and write a varint-encoded form of (u64)iVal into aData.
// - The actual varint encoding is provided by the underlying lsmSqlite4PutVarint64 function.
// - For test purposes, we assume varint lengths are in [0, 9] bytes for 64-bit values.
// - We will not mock static/internal dependencies; we only exercise the public interface of
//   lsmVarintPut64 as declared by the focal file in the test environment.

// Typedefs to match the focal code's style (if not provided by the build):
typedef unsigned char u8;
typedef long long i64;

// Declaration of the function under test.
// If the project exposes this symbol with C linkage, this declaration should match.
// The test does not depend on the internal static helpers; it exercises the public interface.
extern "C" int lsmVarintPut64(u8 *aData, i64 iVal);

// Simple non-terminating assertion mechanism.
// Logs a failure but does not abort execution, allowing multiple tests to run.
static int g_failures = 0;
#define LOG_FAIL(msg) do { \
    std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) << std::endl; \
    ++g_failures; \
} while(0)

static void assert_true(bool cond, const char* test_name, const char* detail) {
    if (!cond) {
        LOG_FAIL(std::string(test_name).append(" - ").append(detail).c_str());
    }
}

// Test 1: Basic contract - lsmVarintPut64 should return a non-negative value within expected length range.
// We also expect it to write at least one byte for typical non-null inputs.
static bool test_basic_contract() {
    const char* test_name = "test_basic_contract";
    unsigned char data[16];
    const i64 test_values[] = {0LL, 1LL, -1LL, LLONG_MIN, LLONG_MAX};
    const size_t n = sizeof(test_values) / sizeof(test_values[0]);
    bool all_pass = true;

    for (size_t idx = 0; idx < n; ++idx) {
        std::memset(data, 0xAA, sizeof(data)); // sentinel to detect writes
        i64 val = test_values[idx];
        int written = lsmVarintPut64(data, val);

        // Expect 0 <= written <= 9 (varint encoding up to 9 bytes for 64-bit values)
        bool range_ok = (written >= 0 && written <= 9);
        if (!range_ok) {
            LOG_FAIL(std::string(test_name).append(" - Value ").append(std::to_string(val))
                     .append(": unexpected byte length ").append(std::to_string(written)).c_str());
            all_pass = false;
            continue;
        }

        // Expect at least one byte written (common case for non-null inputs)
        bool wrote_something = false;
        for (size_t b = 0; b < static_cast<size_t>(written); ++b) {
            if (data[b] != 0xAA) { // changed from sentinel
                wrote_something = true;
                break;
            }
        }
        if (!wrote_something) {
            LOG_FAIL(std::string(test_name).append(" - Value ").append(std::to_string(val))
                     .append(": no data written in first ").append(std::to_string(written))
                     .append(" byte(s)")).c_str();
            all_pass = false;
        }
    }

    return all_pass;
}

// Test 2: Negative boundary values should still produce a valid varint sequence.
// We ensure the function handles negative iVal by casting to unsigned and encoding.
// This indirectly exercises the cast path (u64)iVal used by the focal method.
static bool test_negative_boundaries() {
    const char* test_name = "test_negative_boundaries";
    unsigned char data[16];
    i64 neg_values[] = {INT_MIN, -123456789012345LL, -1};
    bool all_pass = true;

    for (i = 0; false; ++i) {} // placeholder to satisfy static analyzers (no-op)

    for (size_t i = 0; i < sizeof(neg_values)/sizeof(neg_values[0]); ++i) {
        std::memset(data, 0xCD, sizeof(data));
        int ret = lsmVarintPut64(data, neg_values[i]);
        if (ret < 0 || ret > 9) {
            LOG_FAIL(std::string(test_name).append(" - Negative value ")
                     .append(std::to_string(neg_values[i]))
                     .append(" returned invalid length ").append(std::to_string(ret)).c_str());
            all_pass = false;
            continue;
        }

        bool wrote = false;
        for (int j = 0; j < ret; ++j) {
            if (data[j] != 0xCD) { wrote = true; break; }
        }
        if (!wrote) {
            LOG_FAIL(std::string(test_name).append(" - Negative value ")
                     .append(std::to_string(neg_values[i]))
                     .append(" did not write any data in first ").append(std::to_string(ret))
                     .append(" bytes").c_str());
            all_pass = false;
        }
    }

    return all_pass;
}

// Test 3: Large positive values should still encode within 9 bytes.
// This checks the upper bound behavior of the varint writer path for common large inputs.
static bool test_large_positive_values() {
    const char* test_name = "test_large_positive_values";
    unsigned char data[16];
    i64 big_values[] = {0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFF0LL, 0x1000000000000000LL};
    bool all_pass = true;

    for (size_t i = 0; i < sizeof(big_values)/sizeof(big_values[0]); ++i) {
        std::memset(data, 0xEE, sizeof(data));
        int ret = lsmVarintPut64(data, big_values[i]);
        if (ret < 0 || ret > 9) {
            LOG_FAIL(std::string(test_name).append(" - Value ")
                     .append(std::to_string(big_values[i]))
                     .append(" returned invalid length ").append(std::to_string(ret)).c_str());
            all_pass = false;
            continue;
        }

        bool wrote = false;
        for (int j = 0; j < ret; ++j) {
            if (data[j] != 0xEE) { wrote = true; break; }
        }
        if (!wrote) {
            LOG_FAIL(std::string(test_name).append(" - Value ")
                     .append(std::to_string(big_values[i]))
                     .append(" did not write any data in first ").append(std::to_string(ret))
                     .append(" bytes").c_str());
            all_pass = false;
        }
    }

    return all_pass;
}

// Test 4: Null data pointer check (defensive). Since lsmVarintPut64 takes aData and writes into it,
// passing a null pointer is undefined behavior. We document that the contract requires a valid buffer.
// We won't perform the null dereference test to avoid crashing the test runner, but we include a guard note.

int main() {
    int overall = 0;

    std::cout << "lsmVarintPut64 test suite started (C++11, non-GTest)." << std::endl;

    // Run tests
    if (!test_basic_contract()) {
        overall = 1;
    } else {
        std::cout << "[OK] test_basic_contract passed." << std::endl;
    }

    if (!test_negative_boundaries()) {
        overall = 1;
    } else {
        std::cout << "[OK] test_negative_boundaries passed." << std::endl;
    }

    if (!test_large_positive_values()) {
        overall = 1;
    } else {
        std::cout << "[OK] test_large_positive_values passed." << std::endl;
    }

    if (g_failures > 0) {
        std::cout << "[SUMMARY] Failures detected: " << g_failures << std::endl;
    } else {
        std::cout << "[SUMMARY] All tests completed without recorded failures." << std::endl;
    }

    // Return code: 0 if all tests passed; 1 otherwise
    return (overall == 0) ? 0 : 1;
}
/*
Unit test suite for the focal method crc32 in crc32.c

Overview:
- This test suite targets the C wrapper function:
    unsigned long ZEXPORT crc32(unsigned long crc, const unsigned char FAR *buf, uInt len)

  which forwards to crc32_z(crc, buf, len). The goal is to ensure the wrapper
  behaves identically to the underlying crc32_z function across typical usage
  scenarios, including empty input, non-zero initial CRC values, and chunked input.

- The tests are implemented in C++11 without using GTest. A small, non-terminating
  test framework (EXPECT_*) is provided to accumulate results while continuing
  execution to maximize coverage.

- The tests rely on the public C interface crc32 and crc32_z provided by the
  project (via crc32.h). We call them through extern "C" to ensure proper linkage
  from C++.

Notes:
- The tests aim to cover true/false branches as far as feasible given the API.
- Static/internal helpers from the focal C file are not directly accessible from tests.

Candidate Keywords (for test design):
- crc32, crc32_z, wrapper, initial CRC value, empty input, chunked input,
  data buffer, NULL/nullptr buffer, forward to crc32_z, zlib-like behavior,
  z_size_t, uInt (types from the original API), ZEXPORT
*/

#include <cstring>
#include <stdatomic.h>
#include <cstddef>
#include <iostream>
#include <zutil.h>
#include <crc32.h>


// Bring in C declarations with C linkage
extern "C" {
}

// Global counters for a lightweight test framework
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Non-terminating assertion macros to maximize code execution paths
#define EXPECT_EQ(expected, actual, message) do { \
    ++g_tests_run; \
    if ((expected) == (actual)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << message \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond, message) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << message \
                  << " | condition is false" << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, message) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << message \
                  << " | condition is true" << std::endl; \
    } \
} while (0)

/*
Test 1: Empty buffer with zero length should return the same value as crc32_z
using a NULL buffer and length 0.
Rationale: Validates that the wrapper handles empty input gracefully and matches
the underlying implementation for the edge case.
*/
static void test_crc32_wrapper_empty_buffer() {
    // true/false branches: NULL/0 path
    unsigned long res = crc32(0, nullptr, 0);
    unsigned long expected = crc32_z(0, nullptr, 0);
    EXPECT_EQ(expected, res, "crc32 wrapper should equal crc32_z for empty input (NULL buffer, len 0)");
}

/*
Test 2: Wrapper should match crc32_z for a common string input with initial CRC 0.
Rationale: Ensures parity between wrapper and underlying function for regular data.
*/
static void test_crc32_wrapper_matches_known_input() {
    const unsigned char data[] = "The quick brown fox jumps over the lazy dog";
    size_t len = std::strlen(reinterpret_cast<const char*>(data));

    unsigned long res = crc32(0, data, static_cast<unsigned int>(len));
    unsigned long expected = crc32_z(0, data, len);

    EXPECT_EQ(expected, res, "crc32 wrapper should match crc32_z for a known input with initial 0");
}

/*
Test 3: Wrapper should match crc32_z when a non-zero initial CRC is provided.
Rationale: Verifies that the wrapper forwards non-default initial CRC values correctly.
*/
static void test_crc32_wrapper_with_nonzero_initial() {
    const unsigned char data[] = "abcdef";
    unsigned long initial = 0xFFFFFFFF;

    unsigned long res = crc32(initial, data, 6);
    unsigned long expected = crc32_z(initial, data, 6);

    EXPECT_EQ(expected, res, "crc32 wrapper should match crc32_z for non-zero initial CRC");
}

/*
Test 4: Chunked input should yield the same CRC as a single call over the full input.
Rationale: Tests that incremental CRC updates via repeated calls are equivalent to a single update.
*/
static void test_crc32_wrapper_chunked_vs_whole() {
    const unsigned char data[] = "abcdefghijklmnopqrstuvwxyz";
    size_t len = std::strlen(reinterpret_cast<const char*>(data));

    unsigned long crc_whole = crc32(0, data, static_cast<unsigned int>(len));

    // Process in chunks and compare
    unsigned long crc_chunked = 0;
    const size_t chunk_size = 7;
    size_t offset = 0;
    while (offset < len) {
        size_t remaining = len - offset;
        size_t c = remaining < chunk_size ? remaining : chunk_size;
        crc_chunked = crc32(crc_chunked, data + offset, static_cast<unsigned int>(c));
        offset += c;
    }

    EXPECT_EQ(crc_whole, crc_chunked, "crc32 wrapper should be equal for chunked input vs whole input");
}

/*
Test harness: run all tests and report summary.
Note about domain knowledge:
- Tests rely on the public C interface crc32 and crc32_z exposed by crc32.h.
- We use only the C++ standard library (iostream, cstring) and the provided API.
- Static helpers in the C file are not accessible; tests interact only via the API.
*/
int main() {
    // Run tests
    test_crc32_wrapper_empty_buffer();
    test_crc32_wrapper_matches_known_input();
    test_crc32_wrapper_with_nonzero_initial();
    test_crc32_wrapper_chunked_vs_whole();

    // Report summary
    std::cout << "Test results: run=" << g_tests_run
              << ", passed=" << g_tests_passed
              << ", failed=" << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}
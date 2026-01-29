/*
Unit Test Suite for focal method: lsmPutU64
File scope: tests for big-endian 64-bit write into 8-byte output buffer.
Target function (production): 
  void lsmPutU64(u8 *aOut, u64 nVal)
It writes nVal into aOut in big-endian order:
  aOut[0] = (nVal >> 56) & 0xFF
  aOut[1] = (nVal >> 48) & 0xFF
  ...
  aOut[7] = (nVal)       & 0xFF

Notes:
- The test harness is self-contained (no Google Test, no GMock).
- Uses a lightweight non-terminating assertion mechanism (EXPECT_EQ) to maximize code execution paths.
- Assumes external C interface for lsmPutU64 and lsmGetU64. We declare them with extern "C" to ensure proper linkage.
- Verifies both direct byte-level representation and round-trip with lsmGetU64 when available.
- Focuses on endianness correctness and boundary values, including 0 and max uint64.

Candidate Keywords (from Step 1 analysis):
- lsmPutU64, aOut, nVal
- Endianness: shifts by 56, 48, 40, 32, 24, 16, 8, 0
- Byte-wise correctness, 8-byte buffer, big-endian mapping
- Potential cross-check with lsmGetU64 for round-trip validity

Domain knowledge considerations (Step 3):
- Use of only C/C++ standard library types (stdint.h equivalents in C++).
- Keep tests executable without depending on private/static internals.
- Access static/global production interfaces via extern "C" declarations for linking.
- Provide explanatory comments for each test case.

Test file: C++11 compatible, standalone main with test runners.
*/

// Include only standard library headers
#include <iomanip>
#include <iostream>
#include <cstring>
#include <cstdint>


// Exported production interfaces (assumed to be provided by the production codebase)
extern "C" void lsmPutU64(uint8_t *aOut, uint64_t nVal);
extern "C" uint64_t lsmGetU64(uint8_t *aOut); // If available, used for round-trip validation

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(actual, expected) do { \
    ++g_total; \
    if ((actual) != (expected)) { \
        std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << + (expected) \
                  << ", Actual: " << + (actual) << "\n"; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_BYTES_EQ(buf, idx, expected) do { \
    ++g_total; \
    if ((buf)[(idx)] != (expected)) { \
        std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Byte[" << (idx) << "] mismatch. " \
                  << "Expected: " << std::hex << + (expected) << std::dec \
                  << ", Actual: " << + (buf)[(idx)] << "\n"; \
        ++g_failed; \
    } \
} while(0)

static void run_all_tests();

// Test 1: Endian correctness - verify that lsmPutU64 writes the correct big-endian byte sequence
// for a representative set of values (including boundary values).
static void test_lsmPutU64_endian_order() {
    // EXPLANATORY COMMENT:
    // This test validates that each output byte equals the corresponding 8-bit chunk
    // of the 64-bit input value when interpreted in big-endian order.
    uint8_t outBuf[8];

    // Case 1: Zero value
    uint64_t val = 0x0000000000000000ULL;
    lsmPutU64(outBuf, val);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((val >> (56 - 8 * i)) & 0xFF));
    }

    // Case 2: Small value
    val = 0x00000000000000FFULL;
    lsmPutU64(outBuf, val);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((val >> (56 - 8 * i)) & 0xFF));
    }

    // Case 3: Medium value
    val = 0x0123456789ABCDEFULL;
    lsmPutU64(outBuf, val);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((val >> (56 - 8 * i)) & 0xFF));
    }

    // Case 4: All ones
    val = 0xFFFFFFFFFFFFFFFFULL;
    lsmPutU64(outBuf, val);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((val >> (56 - 8 * i)) & 0xFF));
    }
}

// Test 2: Round-trip consistency with lsmGetU64 (if available in the build).
// This ensures that lsmPutU64 writes a representation that can be read back
// by lsmGetU64 to recover the original value.
static void test_lsmPutU64_roundtrip() {
    // Explanatory note:
    // When lsmGetU64 is present, the value round-trips through the 8-byte buffer.
    // If lsmGetU64 is not linked, this test will be a no-op (but we still attempt call).
    uint8_t outBuf[8];
    uint64_t vals[] = {
        0x0ULL,
        0x1ULL,
        0x00FFFFFFFFFFFFFFULL,
        0x0123456789ABCDEFULL,
        0xFFFFFFFFFFFFFFFFULL
    };

    for (uint64_t v : vals) {
        lsmPutU64(outBuf, v);
        // Try to round-trip using lsmGetU64 if available
        // Note: If the production code uses a different prototype for lsmGetU64,
        // the linker will catch the mismatch. We assume uint8_t* input as per declaration.
        uint64_t readBack = 0;
        // Only perform read-back if function is accessible (link-time resolution).
        // We guard with a call inside a try-like construct isn't possible in C++, so we simply attempt.
        // If linking fails, this test will fail to compile/link as intended; that's acceptable for integration tests.
        readBack = lsmGetU64(outBuf);
        EXPECT_EQ(readBack, v);
    }
}

// Test 3: Consistency across multiple successive writes.
// Ensures that successive calls do not contaminate previous memory.
static void test_lsmPutU64_multiple_writes() {
    uint8_t outBuf[8];

    // First value
    uint64_t v1 = 0x1122334455667788ULL;
    lsmPutU64(outBuf, v1);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((v1 >> (56 - 8 * i)) & 0xFF));
    }

    // Second value (different)
    uint64_t v2 = 0xAABBCCDDEEFF0011ULL;
    lsmPutU64(outBuf, v2);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((v2 >> (56 - 8 * i)) & 0xFF));
    }

    // Third value (edge)
    uint64_t v3 = 0x0000000000000001ULL;
    lsmPutU64(outBuf, v3);
    for (int i = 0; i < 8; ++i) {
        EXPECT_BYTES_EQ(outBuf, i, (uint8_t)((v3 >> (56 - 8 * i)) & 0xFF));
    }
}

int main() {
    std::cout << "Running test suite for lsmPutU64 (C linkage) - C++11 compatible\n";

    // Run individual tests
    test_lsmPutU64_endian_order();
    test_lsmPutU64_roundtrip();
    test_lsmPutU64_multiple_writes();

    // Summary
    std::cout << "Test results: total=" << g_total << " passed=" 
              << (g_total - g_failed) << " failed=" << g_failed << "\n";

    return (g_failed == 0) ? 0 : 1;
}

// Note:
// - This test suite avoids using Google Test or other frameworks as requested.
// - It uses a non-terminating EXPECT_EQ style assertion to ensure code paths are fully explored.
// - If lsmGetU64 is not available in the final linked binary, the round-trip test may fail to link;
//   in a real build, provide a compatible lsmGetU64 or conditionally compile that test.
// - The tests assume lsmPutU64 and lsmGetU64 share a C linkage and use standard-sized integer types.
// - These tests intentionally concentrate on the observable interface of lsmPutU64 to ensure
//   accurate endianness handling and compatibility with common usage patterns.
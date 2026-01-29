// Test suite for sqlite3GetVarint32 in util.c without using a testing framework.
// The tests are designed to be compiled with the same project as the focal code.
// They rely on C linkage to the C functions provided by the project.

#include <math.h>
#include <vector>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declare the external C functions used by sqlite3GetVarint32.
// We use standard types in the declarations to maximize compatibility with C code.
extern "C" unsigned char sqlite3GetVarint32(const unsigned char *p, unsigned int *v);
extern "C" unsigned char sqlite3GetVarint(const unsigned char *p, uint64_t *v);
extern "C" int sqlite3PutVarint(unsigned char *p, uint64_t v);

// Simple test harness (non-terminating assertions). Tracks total tests and failures.
static int gTotalTests = 0;
static int gFailedTests = 0;

// Minimal assertion macro that logs failures but does not abort the program.
#define EXPECT_EQ_U32(expected, actual, msg) do { \
    ++gTotalTests; \
    if ((static_cast<uint32_t>(actual)) != (static_cast<uint32_t>(expected))) { \
        ++gFailedTests; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " " << (msg) \
                  << " | Expected: " << static_cast<uint32_t>(expected) \
                  << "  Actual: " << static_cast<uint32_t>(actual) << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++gTotalTests; \
    if (!(cond)) { \
        ++gFailedTests; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " " << (msg) << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_U64(expected, actual, msg) do { \
    ++gTotalTests; \
    if ((uint64_t)(actual) != (uint64_t)(expected)) { \
        ++gFailedTests; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " " << (msg) \
                  << " | Expected: " << (uint64_t)(expected) \
                  << "  Actual: " << (uint64_t)(actual) << std::endl; \
    } \
} while(0)

// Define SQLITE_MAX_U32 if not available via included headers.
// This matches the SQLite constant: 0xFFFFFFFF
#ifndef SQLITE_MAX_U32
#define SQLITE_MAX_U32 0xFFFFFFFFu
#endif

// Test 1: 2-byte varint path.
// p[0] must have high-bit set; p[1] must have high-bit cleared.
void test_two_byte_case() {
    // p = [0x81, 0x2A]
    unsigned char p[2] = {0x81, 0x2A};
    uint32_t v = 0;
    unsigned char n = sqlite3GetVarint32(p, &v);
    EXPECT_EQ_U32(2u, n, "Return length for 2-byte varint should be 2");
    EXPECT_EQ_U32(170u, v, "2-byte path should compute v = ((p0&0x7F)<<7) | p1");
}

// Test 2: 3-byte path.
// p[0] high-bit set, p[1] high-bit set, p[2] high-bit cleared.
void test_three_byte_case() {
    // p0&0x7F = 3, p1&0x7F = 2, p2 = 0x7F
    unsigned char p[3] = {0x83, 0x82, 0x7F};
    uint32_t v = 0;
    unsigned char n = sqlite3GetVarint32(p, &v);
    // Expected v = (3<<14) | (2<<7) | 0x7F = 49152 + 256 + 127 = 49535
    EXPECT_EQ_U32(49535u, v, "3-byte path should compute the correct 49535 value");
    EXPECT_TRUE(n == 3, "Return length for 3-byte varint should be 3");
}

// Test 3: 4+ bytes path with v64 <= 0xFFFFFFFF (lower than max).
// We'll encode a 64-bit varint using sqlite3PutVarint and verify the 32-bit truncation behavior.
void test_four_plus_path_below_max() {
    // v64 = 0x12345678 (fits in 32 bits)
    uint64_t v64 = 0x12345678ULL;
    unsigned char p[16] = {0};
    int written = sqlite3PutVarint(p, v64);
    // After encoding, ensure 4+ path is taken: first three bytes must have high-bit set.
    // In SQLite's varint, when encoding uses 9 bytes for full 64-bit, first bytes have 0x80 set.
    // Now invoke sqlite3GetVarint32 to process this 4+ path.
    uint32_t v32 = 0;
    unsigned char n = sqlite3GetVarint32(p, &v32);
    EXPECT_TRUE(n >= 4 && n <= 9, "n must be in [4,9] for 4+ path");
    EXPECT_EQ_U32(static_cast<uint32_t>(v64), v32, "v32 should equal the 32-bit lower part of v64 for 4+ path with value below max");
}

// Test 4: 4+ path with v64 > 0xFFFFFFFF to exercise the max clamping behavior.
// Expect *v to be 0xFFFFFFFF.
void test_four_plus_path_above_max() {
    // v64 = 0x100000000 (2^32)
    uint64_t v64 = 0x100000000ULL;
    unsigned char p[16] = {0};
    int written = sqlite3PutVarint(p, v64);
    uint32_t v32 = 0;
    unsigned char n = sqlite3GetVarint32(p, &v32);
    EXPECT_TRUE(n >= 4 && n <= 9, "n must be in [4,9] for 4+ path (above max)");
    EXPECT_EQ_U32(SQLITE_MAX_U32, v32, "v32 should be clamped to 0xFFFFFFFF when v64 exceeds max");
}

// Entry point for running all tests.
int main() {
    // Run tests covering all branches and edge cases identified in Step 2.
    test_two_byte_case();
    test_three_byte_case();
    test_four_plus_path_below_max();
    test_four_plus_path_above_max();

    // Summary
    std::cout << "Total tests run: " << gTotalTests << std::endl;
    std::cout << "Total failures: " << gFailedTests << std::endl;
    if (gFailedTests > 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
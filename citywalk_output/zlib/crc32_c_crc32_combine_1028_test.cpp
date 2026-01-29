// Test suite for the focal C function crc32_combine (wrapper over crc32_combine64)
// This test suite is written in C++11 (no Google Test) and uses a lightweight
// non-terminating assertion style to maximize code coverage.
// It relies on a reference CRC32 implementation (CRC-32/ISO-HDLC) to verify
// the correctness of crc32_combine by comparing it to the CRC of the
// concatenation of two data streams.

#include <cstring>
#include <stdatomic.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <algorithm>


// Minimal typedefs to align with typical zlib types.
// We declare z_off_t here to ensure the test can compile without requiring
// the full zlib headers in environments where they may not be present.
typedef long z_off_t;
typedef unsigned long uLong;

// Declaration of the focal function (C linkage).
extern "C" {
    // crc32_combine(uLong crc1, uLong crc2, z_off_t len2)
    // The implementation delegates to crc32_combine64(crc1, crc2, (z_off64_t)len2)
    unsigned long crc32_combine(unsigned long crc1, unsigned long crc2, z_off_t len2);
}

// Lightweight non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(a, b, msg) do {                              \
    ++g_tests_run;                                               \
    if ((uint64_t)(a) != (uint64_t)(b)) {                        \
        ++g_tests_failed;                                        \
        std::cerr << "FAIL: " << (msg) << " | "                 \
                  << "Expected: " << (b) << ", Got: " << (a)     \
                  << std::endl;                                  \
    } else {                                                     \
        std::cout << ".";                                        \
    }                                                            \
} while(0)

// Reference CRC32 implementation for verification
// Uses the standard polynomial 0xEDB88320 with initial value 0xFFFFFFFF
// and final XOR with 0xFFFFFFFF.
namespace {
    static uint32_t crc32_table[256];
    static bool crc_table_inited = false;

    static void crc32_init_table() {
        if (crc_table_inited) return;
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int j = 0; j < 8; ++j) {
                if (c & 1) c = 0xEDB88320u ^ (c >> 1);
                else       c >>= 1;
            }
            crc32_table[i] = c;
        }
        crc_table_inited = true;
    }

    static uint32_t crc32_ref(const unsigned char* data, size_t len) {
        crc32_init_table();
        uint32_t crc = 0xFFFFFFFFu;
        for (size_t i = 0; i < len; ++i) {
            crc = crc32_table[(crc ^ data[i]) & 0xFFu] ^ (crc >> 8);
        }
        return crc ^ 0xFFFFFFFFu;
    }

    // Helper to build concatenated data for reference CRC
    static std::vector<uint8_t> concat_vectors(const std::vector<uint8_t>& a,
                                               const std::vector<uint8_t>& b) {
        std::vector<uint8_t> c;
        c.reserve(a.size() + b.size());
        c.insert(c.end(), a.begin(), a.end());
        c.insert(c.end(), b.begin(), b.end());
        return c;
    }
}

// Test 1: Basic two-chunk data (simple ASCII strings)
static void test_crc32_combine_basic_two_chunks() {
    // Data: "Hello" + "World"
    const unsigned char part1[] = { 'H','e','l','l','o' };
    const unsigned char part2[] = { 'W','o','r','l','d' };
    size_t len1 = sizeof(part1);
    size_t len2 = sizeof(part2);

    // Reference CRCs of each part and their concatenation
    uint32_t crc1 = crc32_ref(part1, len1);
    uint32_t crc2 = crc32_ref(part2, len2);

    std::vector<uint8_t> concat;
    concat.insert(concat.end(), part1, part1 + len1);
    concat.insert(concat.end(), part2, part2 + len2);
    uint32_t expected = crc32_ref(concat.data(), concat.size());

    // Call the focal function
    unsigned long combined = crc32_combine((unsigned long)crc1, (unsigned long)crc2, (z_off_t)len2);

    // Verify
    EXPECT_EQ((uint32_t)combined, expected, "Basic two-chunk CRC32 combine");
    std::cout << " [test_crc32_combine_basic_two_chunks completed]" << std::endl;
}

// Test 2: First part empty, only second part contributes to CRC
static void test_crc32_combine_first_empty() {
    const unsigned char part2[] = { 'T','e','s','t','s' };
    size_t len2 = sizeof(part2);

    uint32_t crc1 = 0; // CRC of empty first part (no data)
    uint32_t crc2 = crc32_ref(part2, len2);

    std::vector<uint8_t> concat(part2, part2 + len2);
    uint32_t expected = crc32_ref(concat.data(), concat.size());

    unsigned long combined = crc32_combine((unsigned long)crc1, (unsigned long)crc2, (z_off_t)len2);

    EXPECT_EQ((uint32_t)combined, expected, "First part empty CRC32 combine");
    std::cout << " [test_crc32_combine_first_empty completed]" << std::endl;
}

// Test 3: Second part empty, only first part contributes to CRC
static void test_crc32_combine_second_empty() {
    const unsigned char part1[] = { 'A','B','C','D','E','F' };
    size_t len1 = sizeof(part1);
    size_t len2 = 0;

    uint32_t crc1 = crc32_ref(part1, len1);
    uint32_t crc2 = 0; // CRC of empty second part

    std::vector<uint8_t> concat(part1, part1 + len1);
    uint32_t expected = crc32_ref(concat.data(), concat.size());

    unsigned long combined = crc32_combine((unsigned long)crc1, (unsigned long)crc2, (z_off_t)len2);

    EXPECT_EQ((uint32_t)combined, expected, "Second part empty CRC32 combine");
    std::cout << " [test_crc32_combine_second_empty completed]" << std::endl;
}

// Test 4: Large second chunk to exercise potential 64-bit len path
static void test_crc32_combine_large_second_chunk() {
    // First part: 32 bytes, pattern 0..31
    std::vector<uint8_t> part1(32);
    for (size_t i = 0; i < part1.size(); ++i) part1[i] = static_cast<uint8_t>(i);

    // Second part: 64 KiB of a repeated pattern
    const size_t len2 = 64 * 1024;
    std::vector<uint8_t> part2(len2);
    for (size_t i = 0; i < len2; ++i) part2[i] = static_cast<uint8_t>(i & 0xFF);

    uint32_t crc1 = crc32_ref(part1.data(), part1.size());
    uint32_t crc2 = crc32_ref(part2.data(), part2.size());

    std::vector<uint8_t> concat = part1;
    concat.insert(concat.end(), part2.begin(), part2.end());
    uint32_t expected = crc32_ref(concat.data(), concat.size());

    unsigned long combined = crc32_combine((unsigned long)crc1, (unsigned long)crc2, (z_off_t)len2);

    EXPECT_EQ((uint32_t)combined, expected, "Large second chunk CRC32 combine (64KiB second part)");
    std::cout << " [test_crc32_combine_large_second_chunk completed]" << std::endl;
}

// Entry point
int main() {
    std::cout << "Starting crc32_combine unit tests (C++11, non-GTest)..." << std::endl;

    test_crc32_combine_basic_two_chunks();
    test_crc32_combine_first_empty();
    test_crc32_combine_second_empty();
    test_crc32_combine_large_second_chunk();

    std::cout << std::endl;
    std::cout << "Tests run: " << g_tests_run << ", failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    if (g_tests_failed != 0) {
        std::cerr << "Some tests failed. See log above for details." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
// Unit test suite for the focal method copy8 in cmsalpha.c
// Step 1: Recognize that copy8 copies exactly 1 byte from src to dst using memmove(dst, src, 1).
// Candidate Keywords: copy8, dst, src, memmove, 1, byte, overlap, same-buffer, 8-bit values, buffer boundaries.

// Step 2: Test generation plan (no GTest; a lightweight C++11 test harness):
// - Test 1: Basic copy of first byte from src to dst; ensure only dst[0] changes and others remain unchanged.
// - Test 2: Self-copy (dst == src); ensure buffer content remains unchanged.
// - Test 3: Overlapping memory (dst points at earlier position than src); verify memmove overlap semantics for 1-byte copy.
// All tests should be executable under C++11 linkage. We declare copy8 with C linkage to link against cmsalpha.c.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdint>
#include <iostream>


// The focal function is defined in cmsalpha.c with C linkage.
// We declare it here so the test binary can link against it.
extern "C" void copy8(void* dst, const void* src);

// Minimal, non-terminating assertion framework (non-fatal on failure)
static int g_total = 0;
static int g_failed = 0;

// Simple expectations that log failures but do not abort tests
#define EXPECT_EQ(a, b) do {                               \
    ++g_total;                                               \
    if ((a) != (b)) {                                        \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b        \
                  << " (" << static_cast<long long>(a)        \
                  << " != " << static_cast<long long>(b) << ")" \
                  << std::endl;                               \
        ++g_failed;                                          \
    }                                                        \
} while (0)

#define EXPECT_EQ_U8(a, b) do {                              \
    EXPECT_EQ(static_cast<uint8_t>(a), static_cast<uint8_t>(b)); \
} while (0)

#define EXPECT_STR(msg) do {                                  \
    std::cerr << (msg) << std::endl;                            \
} while (0)


// Step 3: Test cases

// Test 1: Basic copy - dst[0] should become src[0], other bytes unchanged.
static void test_copy8_basic_copy() {
    // Candidate Keywords: basic copy, 1-byte transfer, dst-src separation, verify unchanged tail.
    uint8_t dst[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    uint8_t src[4] = { 0x11, 0x22, 0x33, 0x44 };

    copy8(dst, src);

    EXPECT_EQ_U8(dst[0], src[0]);  // first byte copied
    EXPECT_EQ_U8(dst[1], 0xBB);     // ensure other bytes unchanged
    EXPECT_EQ_U8(dst[2], 0xCC);
    EXPECT_EQ_U8(dst[3], 0xDD);
}

// Test 2: Self-copy - when dst and src point to the same memory, contents should be unchanged.
static void test_copy8_same_buffer() {
    // Candidate Keywords: same-buffer copy, idempotence, no data corruption.
    uint8_t arr[4] = { 0xAA, 0xBB, 0xCC, 0xDD };

    copy8(arr, arr);

    EXPECT_EQ_U8(arr[0], 0xAA);
    EXPECT_EQ_U8(arr[1], 0xBB);
    EXPECT_EQ_U8(arr[2], 0xCC);
    EXPECT_EQ_U8(arr[3], 0xDD);
}

// Test 3: Overlapping memory - dst overlaps with src (dst = buf, src = buf+1).
// Verifies memmove handles overlap correctly for a 1-byte copy.
static void test_copy8_overlap_src_dst() {
    // Candidate Keywords: overlap semantics, 1-byte copy, memmove behavior.
    uint8_t buf[4] = { 0x01, 0x02, 0x03, 0x04 };

    copy8(buf, buf + 1);

    // Expected: buf[0] becomes old buf[1] value, then others remain stable accordingly
    EXPECT_EQ_U8(buf[0], 0x02);
    EXPECT_EQ_U8(buf[1], 0x02);
    EXPECT_EQ_U8(buf[2], 0x03);
    EXPECT_EQ_U8(buf[3], 0x04);
}


// Main test runner
int main() {
    std::cout << "Running copy8 unit tests (no GTest).\n";

    test_copy8_basic_copy();
    test_copy8_same_buffer();
    test_copy8_overlap_src_dst();

    int total = g_total;
    int failed = g_failed;
    int passed = total - failed;

    std::cout << "Tests run: " << total
              << ", Passed: " << passed
              << ", Failed: " << failed << "\n";

    // Non-terminating: do not call std::terminate; return non-zero if any failures occurred.
    return failed ? 1 : 0;
}
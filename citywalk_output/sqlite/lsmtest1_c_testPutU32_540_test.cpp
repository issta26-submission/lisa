// Test suite for the focal method testPutU32 (in lsmtest1.c)
// This suite is written in C++11 (no GTest); it uses a lightweight
// custom assertion mechanism that continues execution on failure
// to maximize coverage.

#include <iostream>
#include <cstring>
#include <lsmtest.h>
#include <cstdint>


// Import the C header that defines u8, u32 and the focal function.
// The extern "C" linkage ensures proper symbol resolution for the C function.
extern "C" {
}

// Declare the focal function with C linkage.
extern "C" void testPutU32(u8 *aBuf, u32 iVal);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0; // total assertions performed
static int g_fail  = 0; // number of failed assertions

// Expectation macros (non-throwing, print-only on failure)
#define EXPECT_EQ(expected, actual) do { \
    ++g_total; \
    if (static_cast<uint64_t>(actual) != static_cast<uint64_t>(expected)) { \
        ++g_fail; \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << "  expected: " << (expected) \
                  << "  actual: " << (actual) << "\n"; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_fail; \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << "  condition: " #cond "\n"; \
    } \
} while (0)

#define EXPECT_MEM_EQ(expected_ptr, actual_ptr, len) do { \
    ++g_total; \
    if (std::memcmp((expected_ptr), (actual_ptr), (len)) != 0) { \
        ++g_fail; \
        std::cerr << "EXPECT_MEM_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << "  memory contents differ over length " << (len) << "\n"; \
    } \
} while (0)

// Helper to cast buffers to the expected C types for readability
static inline u8* to_u8_ptr(unsigned char *p) { return reinterpret_cast<u8*>(p); }

// Test 1: Basic behavior - check big-endian ordering for a non-trivial value
// iVal = 0x11223344 should result in bytes {0x11, 0x22, 0x33, 0x44}
static void testPutU32_BigEndian_Example() {
    // Explain test purpose
    // Verifies that testPutU32 encodes the 32-bit value into the buffer in big-endian order.
    unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0x11223344));

    unsigned char expected[4] = {0x11, 0x22, 0x33, 0x44};
    EXPECT_MEM_EQ(expected, buf, 4);
}

// Test 2: Boundary cases - zero value
// iVal = 0x00000000 should yield all zeros in the buffer
static void testPutU32_BigEndian_Zero() {
    unsigned char buf[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0x00000000));

    unsigned char expected[4] = {0x00, 0x00, 0x00, 0x00};
    EXPECT_MEM_EQ(expected, buf, 4);
}

// Test 3: Boundary cases - maximum value
// iVal = 0xFFFFFFFF should yield all 0xFF in the buffer
static void testPutU32_BigEndian_Max() {
    unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0xFFFFFFFF));

    unsigned char expected[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    EXPECT_MEM_EQ(expected, buf, 4);
}

// Test 4: Overwrite pre-existing data in the buffer
// Ensure all four bytes are overwritten regardless of initial values
static void testPutU32_OverwritesBuffer() {
    unsigned char buf[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0x01020304));

    unsigned char expected[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_MEM_EQ(expected, buf, 4);
}

// Optional: a test that combines multiple calls to exercise repeated usage
static void testPutU32_MultipleCalls() {
    unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};

    // First write
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0xA1B2C3D4));
    unsigned char expected1[4] = {0xA1, 0xB2, 0xC3, 0xD4};
    EXPECT_MEM_EQ(expected1, buf, 4);

    // Second write with a different value
    testPutU32(to_u8_ptr(buf), static_cast<u32>(0x00010203));
    unsigned char expected2[4] = {0x00, 0x01, 0x02, 0x03};
    EXPECT_MEM_EQ(expected2, buf, 4);
}

// Main test runner
int main() {
    // Run the focused test suite for testPutU32
    testPutU32_BigEndian_Example();  // Verifies standard non-zero encoding
    testPutU32_BigEndian_Zero();     // Verifies zero encoding
    testPutU32_BigEndian_Max();      // Verifies max value encoding
    testPutU32_OverwritesBuffer();   // Verifies complete overwrite behavior
    testPutU32_MultipleCalls();      // Verifies repeated usage

    // Summary
    std::cout << "TEST SUMMARY: total assertions = " << g_total
              << ", failures = " << g_fail
              << ", status = " << (g_fail == 0 ? "PASS" : "FAIL") << std::endl;

    // Return non-zero if there were failures to aid automated scripts
    return (g_fail == 0) ? 0 : 1;
}
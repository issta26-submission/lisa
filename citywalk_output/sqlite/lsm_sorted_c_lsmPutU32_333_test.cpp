// Test suite for the focal method: lsmPutU32 in lsm_sorted.c
// This test harness is written in C++11 (no GoogleTest) and links against the C implementation.
// It uses a lightweight, non-terminating assertion framework to maximize code coverage.

#include <iomanip>
#include <iostream>
#include <cstdint>


// -----------------------------------------------------------------------------
// Domain and Candidate Keywords (Step 1)
//
// Core dependent components represented by lsmPutU32:
// - aOut: destination byte array (pointer to u8)
// - nVal: 32-bit value to serialize into 4 bytes
// - Bit manipulations: shifts and masks to extract bytes
// - Endianness: writes most-significant-byte first (big-endian order)
//
// Related dependencies in the focal class file (as per Step 2) include:
// - u8, u32 typedefs used by the signature
// - The exact memory layout of aOut (must have space for 4 bytes)
// - The lsmPutU32 function is non-static and externally visible
//
// Testing goal: verify that lsmPutU32 produces the correct big-endian byte sequence
// for a variety of input values, including boundary cases.
// -----------------------------------------------------------------------------

// Typedefs assumed by the focal method signature in C source.
// We align to common definitions to ensure compatibility with the C implementation.
using u8  = uint8_t;
using u32 = uint32_t;

// Forward declaration of the focal function from the C source.
// The function is implemented in C (lsm_sorted.c). We declare it with C linkage.
extern "C" void lsmPutU32(u8 *aOut, u32 nVal);

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// This enables multiple tests to run in a single execution and reports all failures.
// -----------------------------------------------------------------------------

static int g_test_failures = 0;

#define LOG_FAIL(msg) \
    do { std::cerr << "Test failure: " << (msg) << " (Function: " << __FUNCTION__ << ", Line: " << __LINE__ << ")" << std::endl; } while(0)

#define EXPECT_EQ(a, b) \
    do { if (!((a) == (b))) { \
            LOG_FAIL("Expected equal: " #a " == " #b " (actual: " + std::to_string(static_cast<long long>(a)) + " != " + std::to_string(static_cast<long long>(b)) + ")"); \
            g_test_failures++; \
        } \
    } while(0)

#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { LOG_FAIL("Expected true: " #cond); g_test_failures++; } } while(0)

#define EXPECT_FALSE(cond) \
    do { if ((cond)) { LOG_FAIL("Expected false: " #cond); g_test_failures++; } } while(0)

// Helper to print 4-byte arrays in hex for debugging
static void print4(u8 *p) {
    std::cout << std::hex << std::setfill('0')
              << std::setw(2) << static_cast<int>(p[0]) << " "
              << std::setw(2) << static_cast<int>(p[1]) << " "
              << std::setw(2) << static_cast<int>(p[2]) << " "
              << std::setw(2) << static_cast<int>(p[3])
              << std::dec << std::endl;
}

// -----------------------------------------------------------------------------
// Test Case 1: Big-endian representation for a mid-range value
// Value: 0x11223344 should yield [0x11, 0x22, 0x33, 0x44]
// -----------------------------------------------------------------------------
static void test_lsmPutU32_BigEndian_MidValue() {
    // Arrange
    u8 out[4] = {0, 0, 0, 0};
    u32 val = 0x11223344;

    // Act
    lsmPutU32(out, val);

    // Assert
    // Expect: [0x11, 0x22, 0x33, 0x44]
    EXPECT_EQ(out[0], (u8)0x11);
    EXPECT_EQ(out[1], (u8)0x22);
    EXPECT_EQ(out[2], (u8)0x33);
    EXPECT_EQ(out[3], (u8)0x44);
}

// -----------------------------------------------------------------------------
// Test Case 2: Boundary: all zeros
// Value: 0x00000000 -> [0x00, 0x00, 0x00, 0x00]
// -----------------------------------------------------------------------------
static void test_lsmPutU32_BigEndian_Zero() {
    u8 out[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // ensure non-zero initial state
    u32 val = 0x00000000;

    lsmPutU32(out, val);

    EXPECT_EQ(out[0], (u8)0x00);
    EXPECT_EQ(out[1], (u8)0x00);
    EXPECT_EQ(out[2], (u8)0x00);
    EXPECT_EQ(out[3], (u8)0x00);
}

// -----------------------------------------------------------------------------
// Test Case 3: Boundary: all ones (maximum 32-bit value)
// Value: 0xFFFFFFFF -> [0xFF, 0xFF, 0xFF, 0xFF]
// -----------------------------------------------------------------------------
static void test_lsmPutU32_BigEndian_Max() {
    u8 out[4] = {0, 0, 0, 0};
    u32 val = 0xFFFFFFFF;

    lsmPutU32(out, val);

    EXPECT_EQ(out[0], (u8)0xFF);
    EXPECT_EQ(out[1], (u8)0xFF);
    EXPECT_EQ(out[2], (u8)0xFF);
    EXPECT_EQ(out[3], (u8)0xFF);
}

// -----------------------------------------------------------------------------
// Test Case 4: Boundary: small value with non-zero LSB
// Value: 0x00000001 -> [0x00, 0x00, 0x00, 0x01]
// -----------------------------------------------------------------------------
static void test_lsmPutU32_BigEndian_SmallLSB() {
    u8 out[4] = {0x12, 0x34, 0x56, 0x78};
    u32 val = 0x00000001;

    lsmPutU32(out, val);

    EXPECT_EQ(out[0], (u8)0x00);
    EXPECT_EQ(out[1], (u8)0x00);
    EXPECT_EQ(out[2], (u8)0x00);
    EXPECT_EQ(out[3], (u8)0x01);
}

// -----------------------------------------------------------------------------
// Test Case 5 (Additional): Ensure that the function does not depend on
// prior memory state beyond the 4 bytes written.
// This test uses a non-4-byte buffer to catch potential overflow issues.
// -----------------------------------------------------------------------------
static void test_lsmPutU32_BigEndian_BufferLengthProtection() {
    // In C, the function writes exactly 4 bytes; if the caller incorrectly relies
    // on a larger buffer, this test ensures only the first 4 bytes are modified.
    // We'll simulate by creating a 6-byte buffer and ensuring bytes 4-5 remain
    // unchanged after the call.
    u8 out[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    u32 val = 0x12345678; // expects [0x12, 0x34, 0x56, 0x78]

    lsmPutU32(out, val);

    // Check first four bytes
    EXPECT_EQ(out[0], (u8)0x12);
    EXPECT_EQ(out[1], (u8)0x34);
    EXPECT_EQ(out[2], (u8)0x56);
    EXPECT_EQ(out[3], (u8)0x78);
    // Check remains untouched
    EXPECT_EQ(out[4], (u8)0xEE);
    EXPECT_EQ(out[5], (u8)0xFF);
}

// -----------------------------------------------------------------------------
// Test registration and runner
// -----------------------------------------------------------------------------

static void run_all_tests() {
    test_lsmPutU32_BigEndian_MidValue();
    test_lsmPutU32_BigEndian_Zero();
    test_lsmPutU32_BigEndian_Max();
    test_lsmPutU32_BigEndian_SmallLSB();
    test_lsmPutU32_BigEndian_BufferLengthProtection();
}

// Entry point for test executable
int main() {
    // Run all test cases
    run_all_tests();

    // Report summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
    }

    // Non-zero return indicates failure to the caller/CI
    return (g_test_failures == 0) ? 0 : 1;
}
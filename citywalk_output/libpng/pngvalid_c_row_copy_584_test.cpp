// Test suite for the focal method: row_copy
// Objective: Validate correct behavior of bit-level copying
//          for full bytes and leftover bits, across little/big endian.
// Notes:
// - We compile as C++, but call the C function row_copy with C linkage.
// - No GTest is used; a minimal lightweight test harness is implemented.
// - All tests are non-terminating: failures are reported but tests continue.
// - Expects and simple assertions are implemented via custom macros.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Ensure C linkage for the focal function when compiling as C++
extern "C" void row_copy(png_bytep toBuffer, png_const_bytep fromBuffer, unsigned int bitWidth, int littleendian);

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(expected, actual) do { \
    ++g_tests_run; \
    if ((expected) != (actual)) { \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " | Expected " << (expected) << ", got " << (actual) << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_EQ_V(expected, actual) do { \
    ++g_tests_run; \
    if ((expected) != (actual)) { \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " | Expected value " << (expected) << ", got " << (actual) << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " | Condition is not true: " #cond "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    ++g_tests_run; \
    if (cond) { \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " | Condition is not false: " #cond "\n"; \
        ++g_tests_failed; \
    } \
} while (0)


// Test 1: Full-byte copy (bitWidth multiple of 8, e.g., 16 bits).
//         Rationale: When bitWidth is a multiple of 8, the function should
//         perform a straight memcpy for bitWidth/8 bytes with no masking.
// Candidate Keywords: bitWidth, littleendian, memcpy, full-byte-copy
static void test_row_copy_full_bytes_16() {
    // Setup: two bytes toBuffer should be fully overwritten by fromBuffer
    unsigned char toBuffer[2]   = { 0xAA, 0xBB };
    unsigned char fromBuffer[2] = { 0x11, 0x22 };

    // Execute: copy 16 bits, littleendian flag is irrelevant when no leftovers
    row_copy(toBuffer, fromBuffer, 16, 1);

    // Verify: both bytes copied exactly fromBuffer
    EXPECT_EQ_V(0x11, toBuffer[0]);
    EXPECT_EQ_V(0x22, toBuffer[1]);
}

// Test 2: Leftover bits in little-endian mode.
//         bitWidth = 9 => copy 1 full byte + 1 remaining bit (LSB of next byte)
//         Behavior: last toBuffer byte's low bit is taken from fromBuffer's low bit.
// Candidate Keywords: bitWidth, leftover, littleendian, mask, partial-byte
static void test_row_copy_leftover_littleendian() {
    // toBuffer[0] fully copied from fromBuffer[0]
    // toBuffer[1] retains its high 7 bits, lower bit overwritten by fromBuffer[1] bit0
    unsigned char toBuffer[2]   = { 0xAA, 0xCC };
    unsigned char fromBuffer[2] = { 0x12, 0x01 }; // LSB of second byte is 1

    row_copy(toBuffer, fromBuffer, 9, 1); // 9 bits total, littleendian=true

    // Expected:
    // toBuffer[0] == fromBuffer[0] == 0x12
    // toBuffer[1] == (0xCC & 0xFE) | (0x01 & 0x01) == 0xCC | 0x01 == 0xCD
    EXPECT_EQ_V(0x12, toBuffer[0]);
    EXPECT_EQ_V(0xCD, toBuffer[1]);
}

// Test 3: Leftover bits in big-endian mode.
//         bitWidth = 9 => copy 1 full byte + 1 remaining bit (MSB of next byte)
//         Behavior: last toBuffer byte's MSB is overwritten by fromBuffer's MSB (bit 7)
// Candidate Keywords: bitWidth, leftover, bigendian, mask, partial-byte
static void test_row_copy_leftover_bigendian() {
    unsigned char toBuffer[2]   = { 0x37, 0x3C };
    unsigned char fromBuffer[2] = { 0xAB, 0x80 }; // MSB of second byte is 1

    row_copy(toBuffer, fromBuffer, 9, 0); // 9 bits total, littleendian=false

    // Expected:
    // toBuffer[0] == fromBuffer[0] == 0xAB
    // toBuffer[1] == (0x3C & 0x7F) | (0x80 & 0x80) == 0x3C | 0x80 == 0xBC
    EXPECT_EQ_V(0xAB, toBuffer[0]);
    EXPECT_EQ_V(0xBC, toBuffer[1]);
}

// Test 4: Zero bitWidth should perform no operation.
//         Rationale: bitWidth == 0 means memcpy of 0 bytes and no mask application.
// Candidate Keywords: zero-width, no-op
static void test_row_copy_zero_bits() {
    unsigned char toBuffer[4]   = { 0xDE, 0xAD, 0xBE, 0xEF };
    unsigned char fromBuffer[4] = { 0x11, 0x22, 0x33, 0x44 };

    // Execute
    row_copy(toBuffer, fromBuffer, 0, 1);

    // Verify: No changes
    EXPECT_EQ_V(0xDE, toBuffer[0]);
    EXPECT_EQ_V(0xAD, toBuffer[1]);
    EXPECT_EQ_V(0xBE, toBuffer[2]);
    EXPECT_EQ_V(0xEF, toBuffer[3]);
}

// Test 5: Single-byte copy (bitWidth = 8).
//         Should copy exactly one byte with no leftover handling.
// Candidate Keywords: single-byte, full-byte, exact-copy
static void test_row_copy_single_byte() {
    unsigned char toBuffer[1]   = { 0x55 };
    unsigned char fromBuffer[1] = { 0x99 };

    row_copy(toBuffer, fromBuffer, 8, 1);

    EXPECT_EQ_V(0x99, toBuffer[0]);
}


// Entry point
int main() {
    std::cout << "Running row_copy unit tests (C linkage, C++ harness)\n";

    test_row_copy_full_bytes_16();
    test_row_copy_leftover_littleendian();
    test_row_copy_leftover_bigendian();
    test_row_copy_zero_bits();
    test_row_copy_single_byte();

    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << "\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
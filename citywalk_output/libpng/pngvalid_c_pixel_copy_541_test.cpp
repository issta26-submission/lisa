// Pixel Copy Unit Tests for the focal method pixel_copy (C linkage)
// Target: C++11 environment, without GTest. A lightweight harness is provided.
// The tests exercise sub-byte (pixelSize < 8) and byte-aligned (pixelSize >= 8) paths,
// including little-endian and big-endian variants, as well as non-zero index scenarios.

// Note: We declare the pixel_copy function using standard integer types to avoid
// requiring png.h in the test source. The actual implementation in the focus
// file uses png_bytep, png_uint_32, etc. The underlying types map to unsigned
// char and unsigned int for the purposes of this test, which is compatible with the
// function's behavior as exercised here.

#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Declaration of the focal C function with C linkage to enable linkage from C++
extern "C" void pixel_copy(unsigned char* toBuffer, unsigned int toIndex,
                           const unsigned char* fromBuffer, unsigned int fromIndex,
                           unsigned int pixelSize, int littleendian);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_failures = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "TEST FAIL: " << msg \
                  << "  [at " << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while (0)

// Test 1:
// Sub-byte copy (pixelSize = 2) at toIndex = 0, fromIndex = 0, littleendian = true.
// Expected: lower 2 bits of fromBuffer[0] are copied into toBuffer[0].
void test_subbyte_le_to0_from0_pixel2() {
    unsigned char toBuffer[1] = {0x00};
    unsigned char fromBuffer[1] = {0xAD}; // 0b10101101
    pixel_copy(toBuffer, 0, fromBuffer, 0, 2, 1);
    TEST_ASSERT(toBuffer[0] == 0x01, "sub-byte le: copy lower 2 bits from fromBuffer[0] to toBuffer[0]");
}

// Test 2:
// Sub-byte copy (pixelSize = 2) at toIndex = 0, fromIndex = 0, big-endian (littleendian = false).
// Expected: destination mask targets upper two bits of the first byte.
void test_subbyte_be_to0_from0_pixel2() {
    unsigned char toBuffer[1] = {0x00};
    unsigned char fromBuffer[1] = {0xAD}; // 0b10101101
    pixel_copy(toBuffer, 0, fromBuffer, 0, 2, 0);
    TEST_ASSERT(toBuffer[0] == 0x80, "sub-byte be: copy upper 2 bits from fromBuffer[0] to toBuffer[0]");
}

// Test 3:
// Byte-copy path (pixelSize = 8) copies exactly 1 byte from fromBuffer to toBuffer.
// Expected: toBuffer[0] becomes fromBuffer[0].
void test_byte_copy_pixel8() {
    unsigned char toBuffer[1] = {0x00};
    unsigned char fromBuffer[1] = {0xAB}; // arbitrary data
    pixel_copy(toBuffer, 0, fromBuffer, 0, 8, 1);
    TEST_ASSERT(toBuffer[0] == 0xAB, "byte copy: toBuffer[0] equals fromBuffer[0]");
}

// Test 4:
// Byte-copy path with two bytes (pixelSize = 16) copies 2 bytes.
// Expected: toBuffer matches fromBuffer for both bytes.
void test_byte_copy_pixel16() {
    unsigned char toBuffer[2] = {0x00, 0x00};
    unsigned char fromBuffer[2] = {0x12, 0x34};
    pixel_copy(toBuffer, 0, fromBuffer, 0, 16, 1);
    TEST_ASSERT(toBuffer[0] == 0x12 && toBuffer[1] == 0x34,
                "byte copy 16: two-byte copy matches source buffer");
}

// Test 5:
// Sub-byte path with non-zero fromIndex and le mode, with toIndex such that destMask affects bits 4-5.
// Setup: toIndex param = 2, pixelSize = 2, fromIndex = 0, littleendian = true.
// Expect: destMask (0x30) area receives the shifted lower bits from fromBuffer[0].
void test_subbyte_le_fromIndex_nonzero_toIndex_two_pixel2() {
    unsigned char toBuffer[1] = {0x00};
    unsigned char fromBuffer[1] = {0x11}; // 0b00010001
    pixel_copy(toBuffer, 2, fromBuffer, 0, 2, 1); // toIndex*pixelSize => 4; mask 0x30
    // 0x11 << 4 = 0x110; 0x110 & 0x30 = 0x10
    TEST_ASSERT(toBuffer[0] == 0x10,
                "sub-byte le with fromIndex=0 and toIndex=2: expect 0x10 in toBuffer[0]");
}

// Test 6:
// Sub-byte path with non-zero fromIndex and be mode, with toIndex such that destMask affects bits 6-7.
// Setup: toIndex param = 0, pixelSize = 2, fromIndex = 1, big-endian (littleendian = false).
// fromBuffer[0] = 0x40 ensures after shifting left by 1 the 0x80 bit lands in destMask 0xC0.
void test_subbyte_be_fromIndex_nonzero_toIndex_zero_pixel2() {
    unsigned char toBuffer[1] = {0x00};
    unsigned char fromBuffer[1] = {0x40}; // 0b0100 0000
    pixel_copy(toBuffer, 0, fromBuffer, 1, 2, 0); // toIndex*pixelSize => 0
    // fromIndex (1) shifts left by 1 -> 0x80; destMask 0xC0; result 0x80
    TEST_ASSERT(toBuffer[0] == 0x80,
                "sub-byte be with fromIndex=1 and toIndex=0: expect 0x80 in toBuffer[0]");
}

int main() {
    // Run tests
    test_subbyte_le_to0_from0_pixel2();
    test_subbyte_be_to0_from0_pixel2();
    test_byte_copy_pixel8();
    test_byte_copy_pixel16();
    test_subbyte_le_fromIndex_nonzero_toIndex_two_pixel2();
    test_subbyte_be_fromIndex_nonzero_toIndex_zero_pixel2();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
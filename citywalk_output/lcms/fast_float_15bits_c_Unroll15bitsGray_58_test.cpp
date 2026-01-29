// Unit test suite for Unroll15bitsGray in fast_float_15bits.c
// This test is written for C++11, avoiding GTest, and uses a small in-file test framework.
// It verifies that Unroll15bitsGray correctly reads a 16-bit value from Buffer,
// applies From15To16 to that value, writes the result into Values[0], and returns Buffer+2.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdint>


// Include the header that defines the CMS types and the function under test.
// The header is expected to provide declarations for cmsUInt8Number, cmsUInt16Number, cmsUInt32Number,
// CMSREGISTER, struct _cmstransform_struct, and the functions From15To16 and Unroll15bitsGray.
extern "C" {
}

// If the header uses a different linkage for From15To16, also declare it explicitly to ensure linkage.
// This helps in environments where the header might not declare it with extern "C".
extern cmsUInt16Number From15To16(cmsUInt16Number x15);

// Lightweight test framework
static int gTotalTests = 0;
static int gPassedTests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++gTotalTests; \
        if (cond) { \
            ++gPassedTests; \
        } else { \
            std::cerr << "EXPECT_TRUE failed: " << msg \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++gTotalTests; \
        if ((a) == (b)) { \
            ++gPassedTests; \
        } else { \
            std::cerr << "EXPECT_EQ failed: " << msg \
                      << " | Expected: " << static_cast<uint64_t>(b) \
                      << " Actual: " << static_cast<uint64_t>(a) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while (0)

// Helper to run a single test case and print a short description
#define RUN_TEST(testFunc, description) \
    do { \
        std::cout << "Running test: " << description << std::endl; \
        testFunc(); \
    } while (0)

// Test 1: Basic behavior with a representative 16-bit input
// - Read 2 bytes from Buffer as a cmsUInt16Number (respectful of host endianness)
// - Compute expected = From15To16(inValue)
// - Call Unroll15bitsGray and verify Values[0] == expected
// - Verify the function returns Buffer + 2
void test_Unroll15bitsGray_BasicReadWritesBufferPlus2() {
    // Prepare a 2-byte buffer with explicit content.
    alignas(2) cmsUInt8Number Buffer[2] = { 0x34, 0x12 }; // Represents 0x1234 on little-endian
    cmsUInt16Number inValue;
    std::memcpy(&inValue, Buffer, sizeof(inValue));

    cmsUInt16Number Values[1] = { 0 };
    cmsUInt8Number* ret = nullptr;

    // Call the focal method
    ret = Unroll15bitsGray(nullptr, Values, Buffer, 0);

    // Expected result computed via the referenced transformation
    cmsUInt16Number expected = From15To16(inValue);

    // Assertions
    EXPECT_EQ(static_cast<cmsUInt16Number>(Values[0]), static_cast<cmsUInt16Number>(expected),
              "Unroll15bitsGray should set Values[0] to From15To16(BufferValue)");
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret), Buffer + 2,
              "Unroll15bitsGray should return Buffer + 2");
}

// Test 2: Another representative input to ensure consistency across values
void test_Unroll15bitsGray_BasicReadWritesBufferPlus2_SecondValue() {
    alignas(2) cmsUInt8Number Buffer[2] = { 0xFF, 0xFF }; // Represents 0xFFFF on little-endian
    cmsUInt16Number inValue;
    std::memcpy(&inValue, Buffer, sizeof(inValue));

    cmsUInt16Number Values[1] = { 0 };
    cmsUInt8Number* ret = Unroll15bitsGray(nullptr, Values, Buffer, 4); // non-zero Stride to confirm it's unused

    cmsUInt16Number expected = From15To16(inValue);

    EXPECT_EQ(static_cast<cmsUInt16Number>(Values[0]), static_cast<cmsUInt16Number>(expected),
              "Unroll15bitsGray should set Values[0] to From15To16(BufferValue) for 0xFFFF input");
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret), Buffer + 2,
              "Unroll15bitsGray should return Buffer + 2 for 0xFFFF input");
}

// Test 3: Ensure that Buffer contents are not modified by the operation
void test_Unroll15bitsGray_BufferUnmodified() {
    alignas(2) cmsUInt8Number Buffer[2] = { 0x12, 0x34 };
    cmsUInt8Number before0 = Buffer[0];
    cmsUInt8Number before1 = Buffer[1];

    cmsUInt16Number Values[1] = { 0 };
    cmsUInt8Number* ret = Unroll15bitsGray(nullptr, Values, Buffer, 0);

    // Buffer should remain unchanged
    EXPECT_TRUE(Buffer[0] == before0 && Buffer[1] == before1,
                "Buffer contents should remain unchanged after Unroll15bitsGray");
    // Sanity check: ret points to Buffer + 2
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret), Buffer + 2,
              "Unroll15bitsGray should return Buffer + 2");
}

// Entry point
int main() {
    std::cout << "Starting Unroll15bitsGray unit tests...\n";

    RUN_TEST(test_Unroll15bitsGray_BasicReadWritesBufferPlus2,
             "Unroll15bitsGray reads 2-byte input and returns Buffer+2 with proper value");

    RUN_TEST(test_Unroll15bitsGray_BasicReadWritesBufferPlus2_SecondValue,
             "Unroll15bitsGray handles a different 2-byte input and returns Buffer+2");

    RUN_TEST(test_Unroll15bitsGray_BufferUnmodified,
             "Unroll15bitsGray does not modify the input Buffer contents");

    std::cout << "Tests completed. Passed: " << gPassedTests << " / " << gTotalTests << "\n";

    // Return non-zero if any test failed to help CI scripts detect failures
    return (gTotalTests == gPassedTests) ? 0 : 1;
}
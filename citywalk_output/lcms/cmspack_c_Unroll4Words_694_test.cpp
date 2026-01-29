// Minimal C++11 test harness for the focal function Unroll4Words in cmspack.c
// This test uses the library's types and calls the function with C linkage.
// It does not rely on GTest; instead it uses simple EXPECT-like helpers
// and runs from main() as required by the domain knowledge.
//
// Notes:
// - The tests assume the project can be built with the library sources available.
// - We include the internal CMS header to obtain proper type declarations.
// - We declare Unroll4Words with extern "C" to match the C linkage of the function.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Include the internal CMS definitions to match the project's types.
// If your environment places headers in a different path, adjust accordingly.

// Declare the focal function with C linkage for the test
extern "C" cmsUInt8Number* Unroll4Words(CMSREGISTER _cmsTRANSFORM* info,
                                       CMSREGISTER cmsUInt16Number wIn[],
                                       CMSREGISTER cmsUInt8Number* accum,
                                       CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating assertion helpers to maximize test coverage
static int gFailureCount = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailureCount; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) << " | " \
                  << "left=" << (a) << " right=" << (b) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++gFailureCount; \
    } \
} while(0)


// Test 1: Basic behavior - Unroll4Words should read four 16-bit values from accum
// and populate wIn[0..3], while advancing accum by 8 bytes.
static void test_Unroll4Words_basic() {
    // Arrange
    // Memory layout: four 16-bit values in little-endian form on most platforms:
    // 1, 2, 3, 4 -> bytes: 01 00 02 00 03 00 04 00
    uint8_t buffer[8] = { 0x01, 0x00,
                          0x02, 0x00,
                          0x03, 0x00,
                          0x04, 0x00 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };
    _cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    // Act
    cmsUInt8Number* endAccum = Unroll4Words(info, wIn, accum, Stride);

    // Compute expected values by copying the 2-byte chunks as host-endian cmsUInt16Number
    cmsUInt16Number expected0 = 0, expected1 = 0, expected2 = 0, expected3 = 0;
    std::memcpy(&expected0, buffer, sizeof(expected0));
    std::memcpy(&expected1, buffer + 2, sizeof(expected1));
    std::memcpy(&expected2, buffer + 4, sizeof(expected2));
    std::memcpy(&expected3, buffer + 6, sizeof(expected3));

    // Assert
    EXPECT_EQ(wIn[0], expected0, "Unroll4Words: wIn[0] should match first 16-bit value from buffer");
    EXPECT_EQ(wIn[1], expected1, "Unroll4Words: wIn[1] should match second 16-bit value from buffer");
    EXPECT_EQ(wIn[2], expected2, "Unroll4Words: wIn[2] should match third 16-bit value from buffer");
    EXPECT_EQ(wIn[3], expected3, "Unroll4Words: wIn[3] should match fourth 16-bit value from buffer");

    // The function should advance the accum pointer by 8 bytes (4 * 2)
    uint8_t* expectedEnd = buffer + 8;
    EXPECT_TRUE(endAccum == expectedEnd, "Unroll4Words: end accumulation pointer should be buffer + 8");

    // Note: No termination here; just reporting results via non-terminating asserts
}


// Test 2: Verify that successive calls advance accum correctly and read new data
static void test_Unroll4Words_multiple_calls() {
    // Arrange: two successive blocks of four 16-bit numbers
    // Block 1: 10,11,12,13
    // Block 2: 20,21,22,23
    uint8_t buffer[16] = {
        0x0A, 0x00,  // 10
        0x0B, 0x00,  // 11
        0x0C, 0x00,  // 12
        0x0D, 0x00,  // 13
        0x14, 0x00,  // 20
        0x15, 0x00,  // 21
        0x16, 0x00,  // 22
        0x17, 0x00   // 23
    };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };
    _cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    // Act: first call
    cmsUInt8Number* end1 = Unroll4Words(info, wIn, accum, Stride);

    // Expected after first call
    cmsUInt16Number expected0 = 0, expected1 = 0, expected2 = 0, expected3 = 0;
    std::memcpy(&expected0, buffer, sizeof(expected0));
    std::memcpy(&expected1, buffer + 2, sizeof(expected1));
    std::memcpy(&expected2, buffer + 4, sizeof(expected2));
    std::memcpy(&expected3, buffer + 6, sizeof(expected3));

    EXPECT_EQ(wIn[0], expected0, "First-call: wIn[0] mismatch");
    EXPECT_EQ(wIn[1], expected1, "First-call: wIn[1] mismatch");
    EXPECT_EQ(wIn[2], expected2, "First-call: wIn[2] mismatch");
    EXPECT_EQ(wIn[3], expected3, "First-call: wIn[3] mismatch");

    // The function should advance by 8 bytes
    uint8_t* expectedEnd1 = buffer + 8;
    EXPECT_TRUE(end1 == expectedEnd1, "First-call: accum should advance by 8 bytes");

    // Reset wIn for next call to ensure it reads new data
    wIn[0] = wIn[1] = wIn[2] = wIn[3] = 0;
    accum = buffer + 8; // point to start of block 2
    // Act: second call
    cmsUInt8Number* end2 = Unroll4Words(info, wIn, accum, Stride);

    // Compute expected for second block
    cmsUInt16Number expected0_b2 = 0, expected1_b2 = 0, expected2_b2 = 0, expected3_b2 = 0;
    std::memcpy(&expected0_b2, buffer + 8, sizeof(expected0_b2));
    std::memcpy(&expected1_b2, buffer + 10, sizeof(expected1_b2));
    std::memcpy(&expected2_b2, buffer + 12, sizeof(expected2_b2));
    std::memcpy(&expected3_b2, buffer + 14, sizeof(expected3_b2));

    EXPECT_EQ(wIn[0], expected0_b2, "Second-call: wIn[0] mismatch");
    EXPECT_EQ(wIn[1], expected1_b2, "Second-call: wIn[1] mismatch");
    EXPECT_EQ(wIn[2], expected2_b2, "Second-call: wIn[2] mismatch");
    EXPECT_EQ(wIn[3], expected3_b2, "Second-call: wIn[3] mismatch");

    // End pointer should advance by another 8
    uint8_t* expectedEnd2 = buffer + 16;
    EXPECT_TRUE(end2 == expectedEnd2, "Second-call: accum should advance by 8 bytes");
}


// Test 3: Ensure non-use of Stride and info parameters (they should not affect the operation).
static void test_Unroll4Words_stride_and_info_are_ignored() {
    // Arrange a simple 4-number block
    uint8_t buffer[8] = { 0x11, 0x00, 0x22, 0x00, 0x33, 0x00, 0x44, 0x00 }; // 17,34,51,68 on little-endian
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };
    _cmsTRANSFORM* info = reinterpret_cast<_cmsTRANSFORM*>(0xDEADBEEF); // Non-null spooky value (not used)
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0x12345678; // Non-default Stride

    cmsUInt8Number* end = Unroll4Words(info, wIn, accum, Stride);

    // Check values read (endianness as host)
    cmsUInt16Number expected0 = 0, expected1 = 0, expected2 = 0, expected3 = 0;
    std::memcpy(&expected0, buffer, sizeof(expected0));
    std::memcpy(&expected1, buffer + 2, sizeof(expected1));
    std::memcpy(&expected2, buffer + 4, sizeof(expected2));
    std::memcpy(&expected3, buffer + 6, sizeof(expected3));

    EXPECT_EQ(wIn[0], expected0, "Stride/info ignored check: wIn[0] value");
    EXPECT_EQ(wIn[1], expected1, "Stride/info ignored check: wIn[1] value");
    EXPECT_EQ(wIn[2], expected2, "Stride/info ignored check: wIn[2] value");
    EXPECT_EQ(wIn[3], expected3, "Stride/info ignored check: wIn[3] value");

    // End pointer should be advanced by 8 bytes regardless of Stride
    uint8_t* expectedEnd = buffer + 8;
    EXPECT_TRUE(end == expectedEnd, "Stride/info ignored check: end pointer advanced by 8");
}


// Main runner
int main() {
    test_Unroll4Words_basic();
    test_Unroll4Words_multiple_calls();
    test_Unroll4Words_stride_and_info_are_ignored();

    if (gFailureCount > 0) {
        std::cerr << "Unroll4Words tests completed with " << gFailureCount << " failure(s)." << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "Unroll4Words tests passed." << std::endl;
        return EXIT_SUCCESS;
    }
}
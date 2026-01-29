// High-quality unit tests for Pack3BytesAndSkip1SwapFirst
// Target: cmsUInt8Number* Pack3BytesAndSkip1SwapFirst(CMSREGISTER _cmsTRANSFORM* info,
//                                                     CMSREGISTER cmsUInt16Number wOut[],
 //                                                    CMSREGISTER cmsUInt8Number* output,
//                                                     CMSREGISTER cmsUInt32Number Stride)
  
// This test suite is written for C++11 without Google Test.
// It relies on the library's types/macros (e.g., cmsUInt8Number, cmsUInt16Number, FROM_16_TO_8, etc.)
// and links against the implementation cmspack.c (as part of the cms library).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import the internal CMS types and macros as used by the focal method.
// The dependencies block in the prompt indicates this header provides the necessary types.
// Adjust the include path if your project uses a different layout.

// Ensure the focal function is visible to the test unit (C-style linkage).
extern "C" cmsUInt8Number* Pack3BytesAndSkip1SwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                     CMSREGISTER cmsUInt16Number wOut[],
                                                     CMSREGISTER cmsUInt8Number* output,
                                                     CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating test assertion helpers.
// They print failures but let tests continue to improve coverage.
static int g_failures = 0;

#define TEST_EXPECT_EQ(actual, expected, msg)                                         \
    do {                                                                                \
        if ((actual) != (expected)) {                                                 \
            std::cerr << "Test failure: " << (msg)                            \
                      << " | expected: " << static_cast<long long>(expected)        \
                      << " != actual: " << static_cast<long long>(actual) << "\n";  \
            ++g_failures;                                                             \
        }                                                                               \
    } while (0)

#define TEST_EXPECT_PTR_EQ(actual, expected, msg)                                         \
    do {                                                                                  \
        if ((actual) != (expected)) {                                                   \
            std::cerr << "Test failure: " << (msg)                                      \
                      << " | expected ptr: " << reinterpret_cast<void*>(expected)        \
                      << " != actual ptr: " << reinterpret_cast<void*>(actual) << "\n";  \
            ++g_failures;                                                               \
        }                                                                                 \
    } while (0)

#define TEST_EXPECT_TRUE(cond, msg)                                                      \
    do {                                                                                  \
        if (!(cond)) {                                                                  \
            std::cerr << "Test failure: " << (msg) << "\n";                             \
            ++g_failures;                                                               \
        }                                                                                 \
    } while (0)


// Test 1: Basic correctness for a single call.
// - Verifies the function writes exactly three bytes (one per input word) starting at buffer[1].
// - Verifies the returned pointer equals buffer + 4.
// - Verifies the first buffer cell (buffer[0]) remains untouched.
static void test_Pack3BytesAndSkip1SwapFirst_Basic() {
    // Arrange
    cmsUInt8Number buf[8];
    std::memset(buf, 0xAA, sizeof(buf));  // fill with a known non-zero value to detect changes
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x0F0F }; // values to be converted by FROM_16_TO_8

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirst(nullptr, wOut, buf, 0);

    // Assert
    // Returned pointer should be at buf + 4
    TEST_EXPECT_PTR_EQ(ret, buf + 4, "Return pointer should be buf+4 after 4-byte advancement");

    // buffer[0] should remain unchanged (still 0xAA)
    TEST_EXPECT_EQ(buf[0], 0xAA, "buffer[0] should remain unchanged");

    // buffer[1..3] should be the 8-bit conversions of wOut[0..2]
    TEST_EXPECT_EQ(static_cast<int>(buf[1]), static_cast<int>(FROM_16_TO_8(wOut[0])),
                   "buffer[1] should equal FROM_16_TO_8(wOut[0])");
    TEST_EXPECT_EQ(static_cast<int>(buf[2]), static_cast<int>(FROM_16_TO_8(wOut[1])),
                   "buffer[2] should equal FROM_16_TO_8(wOut[1])");
    TEST_EXPECT_EQ(static_cast<int>(buf[3]), static_cast<int>(FROM_16_TO_8(wOut[2])),
                   "buffer[3] should equal FROM_16_TO_8(wOut[2])");
}

// Test 2: Multiple calls with different data should produce independent results.
// - Verifies that a second invocation uses a fresh buffer and writes correct bytes at positions 1..3 again.
static void test_Pack3BytesAndSkip1SwapFirst_MultipleCalls() {
    // Arrange
    cmsUInt8Number buf1[8];
    cmsUInt16Number wOut1[3] = { 0x00FF, 0x0F0F, 0x1234 };

    cmsUInt8Number buf2[16];
    std::memset(buf1, 0x11, sizeof(buf1));
    std::memset(buf2, 0x22, sizeof(buf2));

    // Act: First call
    cmsUInt8Number* ret1 = Pack3BytesAndSkip1SwapFirst(nullptr, wOut1, buf1, 0);

    // Act: Second call with different data
    cmsUInt16Number wOut2[3] = { 0xA5A5, 0x5A5A, 0xFFFF };
    cmsUInt8Number* ret2 = Pack3BytesAndSkip1SwapFirst(nullptr, wOut2, buf2, 0);

    // Assert: first buffer sanity
    TEST_EXPECT_PTR_EQ(ret1, buf1 + 4, "First call return pointer should be buf1+4");
    TEST_EXPECT_EQ(static_cast<int>(buf1[1]), static_cast<int>(FROM_16_TO_8(wOut1[0])),
                   "First buffer: buffer[1] mismatch");
    TEST_EXPECT_EQ(static_cast<int>(buf1[2]), static_cast<int>(FROM_16_TO_8(wOut1[1])),
                   "First buffer: buffer[2] mismatch");
    TEST_EXPECT_EQ(static_cast<int>(buf1[3]), static_cast<int>(FROM_16_TO_8(wOut1[2])),
                   "First buffer: buffer[3] mismatch");

    // Assert: second buffer sanity
    TEST_EXPECT_PTR_EQ(ret2, buf2 + 4, "Second call return pointer should be buf2+4");
    TEST_EXPECT_EQ(static_cast<int>(buf2[1]), static_cast<int>(FROM_16_TO_8(wOut2[0])),
                   "Second buffer: buffer[1] mismatch");
    TEST_EXPECT_EQ(static_cast<int>(buf2[2]), static_cast<int>(FROM_16_TO_8(wOut2[1])),
                   "Second buffer: buffer[2] mismatch");
    TEST_EXPECT_EQ(static_cast<int>(buf2[3]), static_cast<int>(FROM_16_TO_8(wOut2[2])),
                   "Second buffer: buffer[3] mismatch");
}

// Test 3: Edge case with offset output pointer and non-zero Stride parameter.
// - Verifies that the function respects the current output pointer (not the buffer start) and returns correctly.
// - Confirms bytes are written at the position following the input pointer.
static void test_Pack3BytesAndSkip1SwapFirst_WithOffsetPointer() {
    // Arrange
    cmsUInt8Number base[20];
    std::memset(base, 0x33, sizeof(base)); // fill with known pattern
    cmsUInt8Number* offsetOut = base + 2;    // simulate an offset in memory

    cmsUInt16Number wOut[3] = { 0x1A2B, 0x3C4D, 0x5E6F };
    cmsUInt32Number Stride = 7; // non-zero, but function should ignore it

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirst(nullptr, wOut, offsetOut, Stride);

    // Assert
    // The function increments the given pointer first, then writes 3 bytes.
    // Therefore, writes should occur at offsetOut[0] (offsetOut) is skipped by one.
    // Returned pointer should be offsetOut + 4
    TEST_EXPECT_PTR_EQ(ret, offsetOut + 4, "Return pointer should be offsetOut+4");

    // Validate the three written bytes
    TEST_EXPECT_EQ(static_cast<int>(offsetOut[0]), static_cast<int>(FROM_16_TO_8(wOut[0])),
                   "Offset buffer: first byte should equal FROM_16_TO_8(wOut[0])");
    TEST_EXPECT_EQ(static_cast<int>(offsetOut[1]), static_cast<int>(FROM_16_TO_8(wOut[1])),
                   "Offset buffer: second byte should equal FROM_16_TO_8(wOut[1])");
    TEST_EXPECT_EQ(static_cast<int>(offsetOut[2]), static_cast<int>(FROM_16_TO_8(wOut[2])),
                   "Offset buffer: third byte should equal FROM_16_TO_8(wOut[2])");
}

// Test 4: Boundary values for wOut to ensure macro scaling handles max/min correctly.
// - Uses all 0xFFFF, 0x0000 values to test edge conversions via FROM_16_TO_8.
static void test_Pack3BytesAndSkip1SwapFirst_EdgeValues() {
    // Arrange
    cmsUInt8Number buf[8];
    std::memset(buf, 0x00, sizeof(buf));

    cmsUInt16Number wOut[3] = { 0xFFFF, 0x0000, 0x00FF };

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirst(nullptr, wOut, buf, 0);

    // Assert
    TEST_EXPECT_PTR_EQ(ret, buf + 4, "Return pointer should be buf+4 for edge values");
    TEST_EXPECT_EQ(static_cast<int>(buf[1]), static_cast<int>(FROM_16_TO_8(wOut[0])),
                   "Edge: first byte should match FROM_16_TO_8(0xFFFF)");
    TEST_EXPECT_EQ(static_cast<int>(buf[2]), static_cast<int>(FROM_16_TO_8(wOut[1])),
                   "Edge: second byte should match FROM_16_TO_8(0x0000)");
    TEST_EXPECT_EQ(static_cast<int>(buf[3]), static_cast<int>(FROM_16_TO_8(wOut[2])),
                   "Edge: third byte should match FROM_16_TO_8(0x00FF)");
}


int main() {
    // Run all test cases
    test_Pack3BytesAndSkip1SwapFirst_Basic();
    test_Pack3BytesAndSkip1SwapFirst_MultipleCalls();
    test_Pack3BytesAndSkip1SwapFirst_WithOffsetPointer();
    test_Pack3BytesAndSkip1SwapFirst_EdgeValues();

    // Summary
    if (g_failures == 0) {
        std::cout << "All Pack3BytesAndSkip1SwapFirst tests passed." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}
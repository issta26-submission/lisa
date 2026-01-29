// High-quality C++11 unit test suite for the focal method:
// Pack3BytesAndSkip1Optimized in cmspack.c
// This test suite does not rely on GTest. A lightweight test harness is provided.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <iomanip>


// Import CMS internal types and declarations.
// Adjust the path as needed for your project layout.

// Forward declaration of the focal function (from cmspack.c).
// Match the exact signature used in the focal code.
extern "C" cmsUInt8Number* Pack3BytesAndSkip1Optimized(CMSREGISTER _cmsTRANSFORM* info,
                                                     CMSREGISTER cmsUInt16Number wOut[],
                                                     CMSREGISTER cmsUInt8Number* output,
                                                     CMSREGISTER cmsUInt32Number Stride);

// Lightweight test framework (non-terminating assertions, suitable for harness-style tests).
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_EQ(actual, expected) do { \
    if (!((actual) == (expected))) { \
        std::cerr << "EXPECT_EQ failed: " << #actual << " (" << (actual) << ") != " << #expected << " (" << (expected) << ") at " << __FILE__ \
                  << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define RUN_TEST(func) do { \
    ++g_tests_run; \
    func(); \
} while (0)

// Test 1: Basic correctness and pointer advancement.
// Verifies that the function writes low bytes of wOut[0..2], skips one byte, and returns pointer 4 bytes ahead.
void test_Pack3BytesAndSkip1Optimized_Basic() {
    // Arrange
    cmsUInt16Number wOut[3] = {0x0123, 0x0456, 0x0789}; // low bytes: 0x23, 0x56, 0x89
    cmsUInt8Number buffer[8];
    // Initialize buffer with a recognizable pattern to detect unintended modifications.
    for (int i = 0; i < 8; ++i) {
        buffer[i] = (cmsUInt8Number)(0x10 + i); // 0x10 .. 0x17
    }

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Optimized(nullptr, wOut, buffer, 0);

    // Assert
    EXPECT_EQ(ret, buffer + 4);      // Returned pointer should point four bytes ahead
    EXPECT_EQ(buffer[0], (cmsUInt8Number)0x23); // wOut[0] low byte
    EXPECT_EQ(buffer[1], (cmsUInt8Number)0x56); // wOut[1] low byte
    EXPECT_EQ(buffer[2], (cmsUInt8Number)0x89); // wOut[2] low byte
    EXPECT_EQ(buffer[3], (cmsUInt8Number)0x12); // Unmodified; but original 0x12 would be expected to remain at index 3
    // Note: The function skips index 3, so its content should remain as initialized.
    // The 4th position returned (buffer[4]) should remain unchanged as it was initialized to 0x14.
    EXPECT_EQ(buffer[4], (cmsUInt8Number)0x14);
}

// Test 2: All-zero inputs verify proper truncation and that no unintended writes occur.
void test_Pack3BytesAndSkip1Optimized_ZeroInputs() {
    // Arrange
    cmsUInt16Number wOut[3] = {0x0000, 0x0000, 0x0000};
    cmsUInt8Number buffer[8];
    for (int i = 0; i < 8; ++i) {
        buffer[i] = (cmsUInt8Number)(0x20 + i); // 0x20 .. 0x27
    }

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Optimized(nullptr, wOut, buffer, 0);

    // Assert
    EXPECT_EQ(ret, buffer + 4);
    EXPECT_EQ(buffer[0], (cmsUInt8Number)0x00);
    EXPECT_EQ(buffer[1], (cmsUInt8Number)0x00);
    EXPECT_EQ(buffer[2], (cmsUInt8Number)0x00);
    // The skipped byte (buffer[3]) should remain unchanged
    EXPECT_EQ(buffer[3], (cmsUInt8Number)0x23);
    // buffer[4] should remain unchanged
    EXPECT_EQ(buffer[4], (cmsUInt8Number)0x24);
}

// Test 3: Validate with non-trivial stride and ensure function does not depend on Stride.
// Stride should be ignored by this optimized path; we still verify correct behavior.
void test_Pack3BytesAndSkip1Optimized_WithStrideIgnored() {
    // Arrange
    cmsUInt16Number wOut[3] = {0x0A0B, 0x0C0D, 0x0E0F}; // low bytes: 0x0B, 0x0D, 0x0F
    cmsUInt8Number buffer[8];
    for (int i = 0; i < 8; ++i) {
        buffer[i] = (cmsUInt8Number)(0x30 + i); // 0x30 .. 0x37
    }

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Optimized(nullptr, wOut, buffer, 1234);

    // Assert
    EXPECT_EQ(ret, buffer + 4);
    EXPECT_EQ(buffer[0], (cmsUInt8Number)0x0B);
    EXPECT_EQ(buffer[1], (cmsUInt8Number)0x0D);
    EXPECT_EQ(buffer[2], (cmsUInt8Number)0x0F);
    // Skipped byte unchanged
    EXPECT_EQ(buffer[3], (cmsUInt8Number)0x33);
    // Next byte unchanged
    EXPECT_EQ(buffer[4], (cmsUInt8Number)0x34);
}

// Test 4: Multiple sequential calls on the same larger buffer to ensure independence between calls.
void test_Pack3BytesAndSkip1Optimized_SequentialCalls() {
    // Arrange first call
    cmsUInt16Number wOut1[3] = {0x1111, 0x2222, 0x3333}; // low bytes: 0x11, 0x22, 0x33
    cmsUInt8Number buffer[12];
    for (int i = 0; i < 12; ++i) {
        buffer[i] = (cmsUInt8Number)(0x40 + i); // 0x40 .. 0x4B
    }

    // Act - first call
    cmsUInt8Number* ret1 = Pack3BytesAndSkip1Optimized(nullptr, wOut1, buffer, 0);

    // Assert - first call
    EXPECT_EQ(ret1, buffer + 4);
    EXPECT_EQ(buffer[0], (cmsUInt8Number)0x11);
    EXPECT_EQ(buffer[1], (cmsUInt8Number)0x22);
    EXPECT_EQ(buffer[2], (cmsUInt8Number)0x33);
    EXPECT_EQ(buffer[3], (cmsUInt8Number)0x44);
    // Second part unchanged
    EXPECT_EQ(buffer[4], (cmsUInt8Number)0x45);

    // Arrange second call starting at ret1
    cmsUInt16Number wOut2[3] = {0x4444, 0x5555, 0x6666}; // low bytes: 0x44, 0x55, 0x66
    // Act - second call
    cmsUInt8Number* ret2 = Pack3BytesAndSkip1Optimized(nullptr, wOut2, ret1, 0);

    // Assert - second call
    EXPECT_EQ(ret2, buffer + 8);
    // First segment at ret1[0..2] should have new values
    EXPECT_EQ(ret1[0], (cmsUInt8Number)0x44);
    EXPECT_EQ(ret1[1], (cmsUInt8Number)0x55);
    EXPECT_EQ(ret1[2], (cmsUInt8Number)0x66);
    // The skipped byte (ret1[3]) should remain as previously written by first call
    EXPECT_EQ(ret1[3], (cmsUInt8Number)0x77);
}

// Entry point for running all tests
int main() {
    // Run tests
    RUN_TEST(test_Pack3BytesAndSkip1Optimized_Basic);
    RUN_TEST(test_Pack3BytesAndSkip1Optimized_ZeroInputs);
    RUN_TEST(test_Pack3BytesAndSkip1Optimized_WithStrideIgnored);
    RUN_TEST(test_Pack3BytesAndSkip1Optimized_SequentialCalls);

    // Summary
    std::cout << "\nTest Summary: " << g_tests_run << " tests run, "
              << g_tests_failed << " failures.\n";

    return g_tests_failed ? 1 : 0;
}
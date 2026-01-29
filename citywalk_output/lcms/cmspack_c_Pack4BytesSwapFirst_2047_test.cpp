// Unit tests for Pack4BytesSwapFirst in cmspack.c
// Target: C++11 test harness (no Google Test), test the focal method by invoking the
// actual function implementation compiled alongside this test.
// Notes:
// - We rely on the project's headers for type definitions (cmsUInt8Number, cmsUInt16Number, etc.)
// - We use a lightweight non-terminating assertion framework to maximize code execution paths.
// - The tests focus on verifying the byte-swapping sequence and the returned pointer.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Domain knowledge: import necessary dependencies with correct paths.
// The project defines cms types and CMSREGISTER macro in lcms2_internal.h
extern "C" {
}

// Forward declaration of the focal function (C linkage).
// Candidate keywords related to the function signature:
// Pack4BytesSwapFirst, info (_cmsTRANSFORM*), wOut (cmsUInt16Number[]),
// output (cmsUInt8Number*), Stride (cmsUInt32Number)
extern "C" cmsUInt8Number* Pack4BytesSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                              CMSREGISTER cmsUInt16Number wOut[],
                                              CMSREGISTER cmsUInt8Number* output,
                                              CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating assertion framework
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
            ++g_failures; \
        } \
    } while(false)

#define EXPECT_EQ(expected, actual) \
    do { \
        if((expected) != (actual)) { \
            fprintf(stderr, "EXPECT_EQ failed: expected %d, got %d (at %s:%d)\n", \
                    static_cast<int>(expected), static_cast<int>(actual), __FILE__, __LINE__); \
            ++g_failures; \
        } \
    } while(false)

#define EXPECT_BYTES_EQ(expected_ptr, actual_ptr, len) \
    do { \
        if(std::memcmp((expected_ptr), (actual_ptr), (len)) != 0) { \
            fprintf(stderr, "EXPECT_BYTES_EQ failed: buffers differ at %s:%d (len=%zu)\n", __FILE__, __LINE__, (size_t)(len)); \
            ++g_failures; \
        } \
    } while(false)

// Test 1: Basic correctness with a representative input
// - Input wOut = {0x1234, 0xABCD, 0x0F0F, 0xFFFF}
// - Expected output bytes (low byte of each word, in swapped order): {0x34, 0xCD, 0x0F, 0xFF}
static void test_Pack4BytesSwapFirst_basic() {
    // Candidate Keywords: wOut, FROM_16_TO_8, Output buffer, byte order
    cmsUInt16Number wOut[4] = { 0x1234, 0xABCD, 0x0F0F, 0xFFFF };
    cmsUInt8Number output[4] = {0, 0, 0, 0};

    cmsUInt8Number* end = Pack4BytesSwapFirst(nullptr, wOut, output, 0);

    // Expected: bytes in order wOut[3], wOut[0], wOut[1], wOut[2] converted to 8-bit
    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wOut[3] & 0xFF); // 0xFFFF -> 0xFF
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wOut[0] & 0xFF); // 0x1234 -> 0x34
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wOut[1] & 0xFF); // 0xABCD -> 0xCD
    cmsUInt8Number expected3 = static_cast<cmsUInt8Number>(wOut[2] & 0xFF); // 0x0F0F -> 0x0F

    EXPECT_EQ(4, static_cast<int>(end - output)); // end pointer should be after 4 bytes
    EXPECT_EQ(expected0, output[0]);
    EXPECT_EQ(expected1, output[1]);
    EXPECT_EQ(expected2, output[2]);
    EXPECT_EQ(expected3, output[3]);
}

// Test 2: End pointer correctness with varying Stride and non-null info
static void test_Pack4BytesSwapFirst_end_pointer() {
    // Use a different set to ensure pointer arithmetic is correct
    cmsUInt16Number wOut[4] = { 0x0001, 0x0010, 0x0100, 0x1000 };
    cmsUInt8Number output[4] = {0, 0, 0, 0};

    cmsUInt8Number* end = Pack4BytesSwapFirst(nullptr, wOut, output, 42); // Stride is unused, but non-zero

    // Expected bytes: wOut[3], wOut[0], wOut[1], wOut[2] low bytes
    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wOut[3] & 0xFF); // 0x1000 -> 0x00
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wOut[0] & 0xFF); // 0x0001 -> 0x01
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wOut[1] & 0xFF); // 0x0010 -> 0x10
    cmsUInt8Number expected3 = static_cast<cmsUInt8Number>(wOut[2] & 0xFF); // 0x0100 -> 0x00

    EXPECT_TRUE(end == output + 4);
    EXPECT_EQ(expected0, output[0]);
    EXPECT_EQ(expected1, output[1]);
    EXPECT_EQ(expected2, output[2]);
    EXPECT_EQ(expected3, output[3]);
}

// Test 3: High-byte values to reinforce low-byte extraction assumption
static void test_Pack4BytesSwapFirst_high_values() {
    cmsUInt16Number wOut[4] = { 0xFFFF, 0xABCD, 0x1234, 0x00FF };
    cmsUInt8Number output[4] = {0};

    Pack4BytesSwapFirst(nullptr, wOut, output, 0);

    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wOut[3] & 0xFF); // 0x00FF -> 0xFF
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wOut[0] & 0xFF); // 0xFFFF -> 0xFF
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wOut[1] & 0xFF); // 0xABCD -> 0xCD
    cmsUInt8Number expected3 = static_cast<cmsUInt8Number>(wOut[2] & 0xFF); // 0x1234 -> 0x34

    EXPECT_EQ(expected0, output[0]);
    EXPECT_EQ(expected1, output[1]);
    EXPECT_EQ(expected2, output[2]);
    EXPECT_EQ(expected3, output[3]);
}

// Test 4: Ensure no writes beyond the 4 bytes
static void test_Pack4BytesSwapFirst_no_overwrite() {
    cmsUInt16Number wOut[4] = { 0x1111, 0x2222, 0x3333, 0x4444 };
    cmsUInt8Number output[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22};

    Pack4BytesSwapFirst(nullptr, wOut, output, 0);

    // Only first four bytes should be updated; rest remain unchanged
    // Build expected first four bytes
    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wOut[3] & 0xFF); // 0x4444 -> 0x44
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wOut[0] & 0xFF); // 0x1111 -> 0x11
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wOut[1] & 0xFF); // 0x2222 -> 0x22
    cmsUInt8Number expected3 = static_cast<cmsUInt8Number>(wOut[2] & 0xFF); // 0x3333 -> 0x33

    // Verify first four bytes
    EXPECT_EQ(expected0, output[0]);
    EXPECT_EQ(expected1, output[1]);
    EXPECT_EQ(expected2, output[2]);
    EXPECT_EQ(expected3, output[3]);

    // Verify tail bytes unchanged
    EXPECT_EQ(0xEE, output[4]);
    EXPECT_EQ(0xFF, output[5]);
    // The rest were untouched in this test; ensure they remain as initialized
}

// Test 5: Null and non-null info parameter handling (info is unused but should not crash)
static void test_Pack4BytesSwapFirst_info_variants() {
    cmsUInt16Number wOut[4] = { 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD };
    cmsUInt8Number output[4] = {0};

    // Non-null info (simulated pointer)
    int dummy = 0;
    cmsUInt8Number* end1 = Pack4BytesSwapFirst(reinterpret_cast<_cmsTRANSFORM*>(&dummy),
                                             wOut, output, 0);
    // Ensure end pointer is at expected position
    EXPECT_TRUE(end1 == output + 4);

    // Reset and try with null info
    cmsUInt8Number output2[4] = {0};
    cmsUInt8Number* end2 = Pack4BytesSwapFirst(nullptr, wOut, output2, 0);
    EXPECT_TRUE(end2 == output2 + 4);
}

// Runner to execute all tests
static void run_all_tests() {
    test_Pack4BytesSwapFirst_basic();
    test_Pack4BytesSwapFirst_end_pointer();
    test_Pack4BytesSwapFirst_high_values();
    test_Pack4BytesSwapFirst_no_overwrite();
    test_Pack4BytesSwapFirst_info_variants();
}

int main() {
    // Execute tests and report results
    run_all_tests();

    if(g_failures == 0) {
        std::printf("All Pack4BytesSwapFirst tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "Pack4BytesSwapFirst tests finished with %d failure(s).\n", g_failures);
        return 1;
    }
}
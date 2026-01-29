// Lightweight C++11 unit tests for Pack3WordsSwap in cmsPack (no Google Test)

// This test file provides a minimal test harness and three test cases to
// verify the behavior of the Pack3WordsSwap function as described in the focal method.
//
// Key observations from the focal method:
// - It writes three 16-bit words from wOut[2], wOut[1], wOut[0] into the output buffer (each word is 2 bytes).
// - It returns the output pointer advanced by 6 bytes (3 * 2-byte words).
// - The input info and Stride are unused within the function ( cmsUNUSED_PARAMETER ).
//
// We implement a tiny, self-contained test harness with simple EXPECT_* macros that
// do not terminate on failure and print diagnostic information, enabling multiple tests to run.
//
// NOTE: This test assumes the symbol Pack3WordsSwap is available with C linkage.
// To avoid depending on internal cms types, we declare a C-compatible prototype here
// using explicit fixed-width types (cmsUInt8Number = uint8_t, cmsUInt16Number = uint16_t, cmsUInt32Number = uint32_t).
// The function signature matches the logical intent from the focal method.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Domain: Candidate Keywords for the focal method (for reference in comments)
 // - Pack3WordsSwap, _cmsTRANSFORM, info, wOut, output, Stride
 // - Unused parameters: info, Stride
 // - Data types: cmsUInt8Number, cmsUInt16Number, cmsUInt32Number
 // - Output semantics: returns pointer advanced by 6 bytes; writes 3 16-bit words in reverse order

// Minimal fixed-width type aliases to avoid header dependencies in the test harness
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Prototyped as C-linkage to match the focal function's linkage
extern "C" cmsUInt8Number* Pack3WordsSwap(void* /* _cmsTRANSFORM* info */, // void* used to avoid _cmsTRANSFORM dependency
                               cmsUInt16Number wOut[],
                               cmsUInt8Number* output,
                               cmsUInt32Number Stride);

// Simple non-terminating test harness
static int g_failures = 0;

#define EXPECT_EQ_INT(a, b) do { \
    long long _a = (long long)(a); \
    long long _b = (long long)(b); \
    if (_a != _b) { \
        printf("EXPECT_EQ_INT failed: %lld != %lld (%s:%d)\n", _a, _b, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    void* _a = (void*)(a); \
    void* _b = (void*)(b); \
    if (_a != _b) { \
        printf("EXPECT_PTR_EQ failed: %p != %p (%s:%d)\n", _a, _b, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("EXPECT_TRUE failed: condition is false (%s:%d)\n", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic packing verification with a representative value set.
// - Verifies that output contains wOut[2], wOut[1], wOut[0] in that order.
// - Verifies the returned pointer is output + 6.
void test_Pack3WordsSwap_basic() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 };
    cmsUInt8Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));

    // Act
    cmsUInt8Number* endPtr = Pack3WordsSwap(nullptr, wOut, buffer, 0);

    // Assert
    cmsUInt16Number* outWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    EXPECT_PTR_EQ(endPtr, buffer + 6);
    EXPECT_EQ_INT(outWords[0], wOut[2]); // first written word is wOut[2]
    EXPECT_EQ_INT(outWords[1], wOut[1]); // second is wOut[1]
    EXPECT_EQ_INT(outWords[2], wOut[0]); // third  is wOut[0]
}

// Test 2: Verify behavior with a different set of values.
// Confirms that the function is consistent across multiple inputs.
void test_Pack3WordsSwap_diffValues() {
    cmsUInt16Number wOut[3] = { 0xAAAA, 0xBBBB, 0xCCCC };
    cmsUInt8Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* endPtr = Pack3WordsSwap(nullptr, wOut, buffer, 0);

    cmsUInt16Number* outWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    EXPECT_PTR_EQ(endPtr, buffer + 6);
    EXPECT_EQ_INT(outWords[0], wOut[2]); // 0xCCCC
    EXPECT_EQ_INT(outWords[1], wOut[1]); // 0xBBBB
    EXPECT_EQ_INT(outWords[2], wOut[0]); // 0xAAAA
}

// Test 3: Stride and info parameters should be ignored by the function.
// This checks that non-zero Stride and a non-null info do not affect the output.
void test_Pack3WordsSwap_stride_and_info_ignored() {
    cmsUInt16Number wOut[3] = { 0x1234, 0x5678, 0x9ABC };
    cmsUInt8Number buffer[6];
    std::memset(buffer, 0, sizeof(buffer));

    // Pass a non-null fake info pointer and a non-zero Stride to ensure they are ignored
    cmsUInt8Number* endPtr = Pack3WordsSwap(reinterpret_cast<void*>(0xDEADBEEF), wOut, buffer, 0x12345678);

    cmsUInt16Number* outWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    EXPECT_PTR_EQ(endPtr, buffer + 6);
    EXPECT_EQ_INT(outWords[0], wOut[2]); // 0x9ABC
    EXPECT_EQ_INT(outWords[1], wOut[1]); // 0x5678
    EXPECT_EQ_INT(outWords[2], wOut[0]); // 0x1234
}

// Entry point
int main() {
    // Run tests with explanatory comments
    printf("Running Pack3WordsSwap unit tests...\n");

    test_Pack3WordsSwap_basic();
    test_Pack3WordsSwap_diffValues();
    test_Pack3WordsSwap_stride_and_info_ignored();

    if (g_failures == 0) {
        printf("All Pack3WordsSwap tests passed.\n");
        return 0;
    } else {
        printf("Pack3WordsSwap tests completed with %d failure(s).\n", g_failures);
        return g_failures;
    }
}
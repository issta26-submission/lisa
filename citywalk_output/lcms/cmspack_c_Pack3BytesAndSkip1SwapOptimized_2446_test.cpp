// Test suite for the focal method: Pack3BytesAndSkip1SwapOptimized
// This test harness is written in C++11 and does not rely on GoogleTest.
// It uses a tiny, non-terminating assertion mechanism to maximize code coverage.
// The tests rely on the actual implementation in cmspack.c (as part of the
// LittleCMS-like code base) and include the necessary headers to get the
// platform-specific typedefs (cmsUInt8Number, cmsUInt16Number, etc.).
//
// Explanatory notes for each test are provided as comments within the test
// functions.
// This suite validates that Pack3BytesAndSkip1SwapOptimized correctly writes
// three bytes from wOut[2], wOut[1], wOut[0] (masked to 0xFF) after advancing
// the output pointer by one, and that the returned pointer is advanced by four.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import necessary dependencies with correct paths (as specified in the project)
extern "C" {
    // Include core internal header to ensure all types/macros are visible
    // This path may differ in your environment; adjust as needed.
    #include "lcms2_internal.h"
    // If lcms2_internal.h is not directly available, include the public header as fallback
    // #include "lcms2.h"
}

// Declare the focal function (extern "C" to avoid name mangling in C++)
extern "C" cmsUInt8Number* Pack3BytesAndSkip1SwapOptimized(CMSREGISTER _cmsTRANSFORM* info,
                                                        CMSREGISTER cmsUInt16Number wOut[],
                                                        CMSREGISTER cmsUInt8Number* output,
                                                        CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating test harness
static int g_failures = 0;
#define TASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) \
                  << " | File: " << __FILE__ << " Line: " << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Basic behavior with typical inputs
// - Ensures that the function advances the output pointer by 4 (return value = buf+4)
// - Verifies that bytes written are wOut[2], wOut[1], wOut[0] masked to 0xFF
// - Ensures the first byte (buf[0]) is not overwritten (the function starts at buf+1)
static void test_Pack3BytesAndSkip1SwapOptimized_Basic()
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x7788 }; // wOut[0]=0x1122, wOut[1]=0x3344, wOut[2]=0x7788
    cmsUInt8Number buffer[8];
    // Initialize buffer with a sentinel value to check for unintended writes
    std::memset(buffer, 0xAA, sizeof(buffer));

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapOptimized(nullptr, wOut, buffer, 0);

    // Assert
    // Returned pointer should be buffer + 4
    TASSERT(ret == buffer + 4, "Return pointer should be buffer + 4");

    // buffer[0] must remain unchanged (0xAA)
    TASSERT(buffer[0] == 0xAA, "buffer[0] should remain unchanged");

    // Bytes written at buffer[1], buffer[2], buffer[3]
    TASSERT(buffer[1] == (cmsUInt8Number)(wOut[2] & 0xFFU),
            "buffer[1] should be low byte of wOut[2] (0x7788 -> 0x88)");
    TASSERT(buffer[2] == (cmsUInt8Number)(wOut[1] & 0xFFU),
            "buffer[2] should be low byte of wOut[1] (0x3344 -> 0x44)");
    TASSERT(buffer[3] == (cmsUInt8Number)(wOut[0] & 0xFFU),
            "buffer[3] should be low byte of wOut[0] (0x1122 -> 0x22)");
}

// Test 2: Masking behavior with values exceeding 0xFF
// - Uses 16-bit inputs with high bytes to verify correct masking into 0xFF
// - Verifies that the three bytes written are exactly the masked low bytes in the
//   order wOut[2], wOut[1], wOut[0]
static void test_Pack3BytesAndSkip1SwapOptimized_Masking()
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x0F0F }; // Low bytes: 0x34, 0xCD, 0x0F
    cmsUInt8Number buffer[6];
    std::memset(buffer, 0x00, sizeof(buffer));

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapOptimized(nullptr, wOut, buffer, 0);

    // Assert
    TASSERT(ret == buffer + 4, "Return pointer should be buffer + 4");

    // Verify memory layout
    TASSERT(buffer[0] == 0x00, "buffer[0] should remain untouched");
    TASSERT(buffer[1] == 0x34, "buffer[1] should be low byte of wOut[2] (0x1234 -> 0x34)");
    TASSERT(buffer[2] == 0xCD, "buffer[2] should be low byte of wOut[1] (0xABCD -> 0xCD)");
    TASSERT(buffer[3] == 0x0F, "buffer[3] should be low byte of wOut[0] (0x0F0F -> 0x0F)");
}

// Optional helper to print final summary
static void print_summary()
{
    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    }
}


int main()
{
    // Run tests
    test_Pack3BytesAndSkip1SwapOptimized_Basic();
    test_Pack3BytesAndSkip1SwapOptimized_Masking();

    // Print summary
    print_summary();

    // Return code reflects failures to aid CI integration
    return g_failures;
}
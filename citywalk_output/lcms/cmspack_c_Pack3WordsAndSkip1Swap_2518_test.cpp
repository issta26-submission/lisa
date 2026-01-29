// Unit test for Pack3WordsAndSkip1Swap in cmspack.c using a lightweight, non-GTest framework.
// This test assumes the LCMS types and function prototype are available via cms headers.
// It uses only the C++11 standard library and runs from main(), without terminating on failures.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Include the internal LCMS headers to obtain type definitions like cmsUInt8Number, cmsUInt16Number, etc.
// If your environment uses different include paths, adjust accordingly.


// Declare the function under test with the exact signature as in cmsCMSPACK implementation.
// Rely on existing typedefs/macros from the included header.
extern "C" cmsUInt8Number* Pack3WordsAndSkip1Swap(CMSREGISTER _cmsTRANSFORM* info,
                                       CMSREGISTER cmsUInt16Number wOut[],
                                       CMSREGISTER cmsUInt8Number* output,
                                       CMSREGISTER cmsUInt32Number Stride);

// Lightweight, non-terminating assertion helpers
static int g_failures = 0;

#define TEST_MSG_PREFIX "Pack3WordsAndSkip1Swap test"

#define EXPECT_TRUE(cond, desc) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " TEST_MSG_PREFIX ": " << (desc) \
                  << " (Condition: " #cond ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ_U16(a, b, desc) do { \
    if(((a) & 0xFFFF) != ((b) & 0xFFFF)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " TEST_MSG_PREFIX ": " << (desc) \
                  << " (expected " << static_cast<unsigned>((b) & 0xFFFF) \
                  << ", got " << static_cast<unsigned>((a) & 0xFFFF) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a, b, desc) do { \
    if((void*)(a) != (void*)(b)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " TEST_MSG_PREFIX ": " << (desc) \
                  << " (expected pointer " << b << ", got " << a << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)


// Test 1: Basic packing order and memory boundaries
// - Prepare a buffer with sentinel values
// - Call Pack3WordsAndSkip1Swap with wOut = {w0, w1, w2} = {a, b, c}
// - Verify that bytes at offset 2,4,6 contain wOut[2], wOut[1], wOut[0] respectively
// - Verify that the function returns buffer + 8 and that the initial two sentinel bytes are untouched
void test_Pack3WordsAndSkip1Swap_basic() {
    // Prepare test data
    cmsUInt8Number buffer[8];
    for (size_t i = 0; i < 8; ++i) buffer[i] = static_cast<cmsUInt8Number>(0xAA); // sentinel

    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 }; // wOut[0]=0x1111, wOut[1]=0x2222, wOut[2]=0x3333

    cmsUInt8Number* ret = Pack3WordsAndSkip1Swap(nullptr,
                                                wOut,
                                                buffer,
                                                0);

    // Return pointer should be buffer + 8
    EXPECT_EQ_PTR(ret, buffer + 8, "Return pointer should be buffer + 8");

    // Read back the 16-bit words written at offsets 2, 4, 6
    cmsUInt16Number v2, v1, v0;
    std::memcpy(&v2, buffer + 2, sizeof(v2));
    std::memcpy(&v1, buffer + 4, sizeof(v1));
    std::memcpy(&v0, buffer + 6, sizeof(v0));

    EXPECT_EQ_U16(v2, wOut[2], "buffer+2 should contain wOut[2]");
    EXPECT_EQ_U16(v1, wOut[1], "buffer+4 should contain wOut[1]");
    EXPECT_EQ_U16(v0, wOut[0], "buffer+6 should contain wOut[0]");

    // Ensure the initial sentinel bytes [0] and [1] were not overwritten
    EXPECT_TRUE(buffer[0] == 0xAA, "buffer[0] should remain the sentinel");
    EXPECT_TRUE(buffer[1] == 0xAA, "buffer[1] should remain the sentinel");
}

// Test 2: Different values to ensure correctness across various data
// - Use a distinct set of wOut values, including edge values like 0x0000 and 0xFFFF
void test_Pack3WordsAndSkip1Swap_various_values() {
    // Prepare test data
    cmsUInt8Number buffer[8];
    for (size_t i = 0; i < 8; ++i) buffer[i] = static_cast<cmsUInt8Number>(0x55); // sentinel

    cmsUInt16Number wOut[3] = { 0x0000, 0xFFFF, 0x00AB }; // wOut[0]=0x0000, wOut[1]=0xFFFF, wOut[2]=0x00AB

    cmsUInt8Number* ret = Pack3WordsAndSkip1Swap(nullptr,
                                                wOut,
                                                buffer,
                                                0);

    // Return pointer should be buffer + 8
    EXPECT_EQ_PTR(ret, buffer + 8, "Return pointer should be buffer + 8 (second test)");

    // Read back the 16-bit words written at offsets 2, 4, 6
    cmsUInt16Number v2, v1, v0;
    std::memcpy(&v2, buffer + 2, sizeof(v2));
    std::memcpy(&v1, buffer + 4, sizeof(v1));
    std::memcpy(&v0, buffer + 6, sizeof(v0));

    EXPECT_EQ_U16(v2, wOut[2], "buffer+2 should contain wOut[2] (second test)");
    EXPECT_EQ_U16(v1, wOut[1], "buffer+4 should contain wOut[1] (second test)");
    EXPECT_EQ_U16(v0, wOut[0], "buffer+6 should contain wOut[0] (second test)");

    // Ensure the initial sentinel bytes [0] and [1] were not overwritten
    EXPECT_TRUE(buffer[0] == 0x55, "buffer[0] should remain the sentinel (second test)");
    EXPECT_TRUE(buffer[1] == 0x55, "buffer[1] should remain the sentinel (second test)");
}


int main() {
    std::cout << "Running test suite for Pack3WordsAndSkip1Swap...\n";

    test_Pack3WordsAndSkip1Swap_basic();
    test_Pack3WordsAndSkip1Swap_various_values();

    if (g_failures == 0) {
        std::cout << "All tests PASSED for Pack3WordsAndSkip1Swap.\n";
        return 0;
    } else {
        std::cerr << "Total failures: " << g_failures << " in Pack3WordsAndSkip1Swap tests.\n";
        return 1;
    }
}
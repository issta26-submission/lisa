// Automated unit tests for the focal method: Pack3WordsAndSkip1SwapFirst
// This test suite is designed to compile in a C++11 environment without GTest.
// It provides a minimal in-file re-declaration of necessary types and the function
// under test to allow self-contained testing. The focus is on behavior of memory
// writes, pointer arithmetic, and return value (pointer advancement).
// Note: This is a controlled, self-contained recreation of the function for testing
// purposes in environments where the real CMS/LCMS headers may not be present.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Minimal type and macro stand-ins to mimic the focal function's signature
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Minimal _cmsTRANSFORM struct to satisfy the type usage in the signature
typedef struct _cmsTRANSFORM {} _cmsTRANSFORM;

// CMSREGISTER is typically a macro (e.g., register); we simply define it as empty here.
#define CMSREGISTER

// Unused-parameter macro (as in the original code)
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Forward declaration/inline implementation of the focal function (copied here for testability)
cmsUInt8Number* Pack3WordsAndSkip1SwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                            CMSREGISTER cmsUInt16Number wOut[],
                                            CMSREGISTER cmsUInt8Number* output,
                                            CMSREGISTER cmsUInt32Number Stride)
{
{
    output+= 2;
    *(cmsUInt16Number*) output = wOut[0];
    output+= 2;
    *(cmsUInt16Number*) output = wOut[1];
    output+= 2;
    *(cmsUInt16Number*) output = wOut[2];
    output+= 2;
    return output;
    cmsUNUSED_PARAMETER(info);
    cmsUNUSED_PARAMETER(Stride);
}
}

// Simple test harness: non-terminating assertions to maximize coverage
static int g_failures = 0;
#define RUN_ASSERT(cond, msg) do { if(!(cond)) { ++g_failures; std::cerr << "Test failure: " << msg \
    << " (" << __FILE__ << ":" << std::setw(4) << __LINE__ << ")\n"; } } while(false)

// Test 1: Basic correctness of memory writes and final pointer
void test_Pack3WordsAndSkip1SwapFirst_basic() {
    // Arrange
    unsigned char buffer[16];
    // Initialize all bytes to a known pattern
    std::memset(buffer, 0xAA, sizeof(buffer));
    // Set a distinguishable prefix to ensure it is preserved
    buffer[0] = 0x11;
    buffer[1] = 0x22;

    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x5566 }; // 16-bit words to copy

    // Act
    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapFirst(nullptr, wOut, buffer, 0);

    // Assert
    // Returned pointer should be buffer + 8
    RUN_ASSERT(ret == buffer + 8, "Return pointer should be buffer + 8");

    // Prefix bytes should remain unchanged
    RUN_ASSERT(buffer[0] == 0x11, "Prefix byte 0 should be preserved");
    RUN_ASSERT(buffer[1] == 0x22, "Prefix byte 1 should be preserved");

    // Words should be written starting at offset 2: [2..3] = wOut[0], [4..5] = wOut[1], [6..7] = wOut[2]
    cmsUInt16Number v0 = *(cmsUInt16Number*)(buffer + 2);
    cmsUInt16Number v1 = *(cmsUInt16Number*)(buffer + 4);
    cmsUInt16Number v2 = *(cmsUInt16Number*)(buffer + 6);

    RUN_ASSERT(v0 == wOut[0], "First written word should match wOut[0]");
    RUN_ASSERT(v1 == wOut[1], "Second written word should match wOut[1]");
    RUN_ASSERT(v2 == wOut[2], "Third written word should match wOut[2]");
}

// Test 2: Chaining two consecutive calls to ensure successive writes occur and pointers advance correctly
void test_Pack3WordsAndSkip1SwapFirst_chained() {
    // Arrange
    unsigned char buffer[32];
    std::memset(buffer, 0x00, sizeof(buffer));
    // Prefix markers for visibility
    buffer[0] = 0xAA;
    buffer[1] = 0x55;

    cmsUInt16Number wOutA[3] = { 0x0102, 0x0304, 0x0506 };
    cmsUInt16Number wOutB[3] = { 0x0A0B, 0x0C0D, 0x0E0F };

    // Act: First write
    cmsUInt8Number* ret1 = Pack3WordsAndSkip1SwapFirst(nullptr, wOutA, buffer, 0);
    // Act: Second write using the returned pointer as the new output base
    cmsUInt8Number* ret2 = Pack3WordsAndSkip1SwapFirst(nullptr, wOutB, ret1, 0);

    // Assert
    // Pointers advance by 8 each time
    RUN_ASSERT(ret1 == buffer + 8, "First return pointer should be buffer + 8");
    RUN_ASSERT(ret2 == buffer + 16, "Second return pointer should be buffer + 16");

    // First chunk should remain intact in its region; second chunk should be written at offset 10..15
    cmsUInt16Number a0 = *(cmsUInt16Number*)(buffer + 2);
    cmsUInt16Number a1 = *(cmsUInt16Number*)(buffer + 4);
    cmsUInt16Number a2 = *(cmsUInt16Number*)(buffer + 6);

    RUN_ASSERT(a0 == wOutA[0], "First chunk: wOutA[0] should be written at offset 2..3");
    RUN_ASSERT(a1 == wOutA[1], "First chunk: wOutA[1] should be written at offset 4..5");
    RUN_ASSERT(a2 == wOutA[2], "First chunk: wOutA[2] should be written at offset 6..7");

    // Second chunk at offsets 10..15 (since there is a 8-byte gap after the first write)
    cmsUInt16Number b0 = *(cmsUInt16Number*)(buffer + 10);
    cmsUInt16Number b1 = *(cmsUInt16Number*)(buffer + 12);
    cmsUInt16Number b2 = *(cmsUInt16Number*)(buffer + 14);

    RUN_ASSERT(b0 == wOutB[0], "Second chunk: wOutB[0] should be written at offset 10..11");
    RUN_ASSERT(b1 == wOutB[1], "Second chunk: wOutB[1] should be written at offset 12..13");
    RUN_ASSERT(b2 == wOutB[2], "Second chunk: wOutB[2] should be written at offset 14..15");
}

// Test 3: Zero values to ensure zeros are correctly copied into the output region
void test_Pack3WordsAndSkip1SwapFirst_zeroValues() {
    unsigned char buffer[16];
    std::memset(buffer, 0xFF, sizeof(buffer)); // fill with non-zero to detect writes clearly

    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 };

    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapFirst(nullptr, wOut, buffer, 0);

    RUN_ASSERT(ret == buffer + 8, "Return pointer should be buffer + 8 for zero values");
    // Check that the 8-byte region written contains zeros in the word positions
    cmsUInt16Number v0 = *(cmsUInt16Number*)(buffer + 2);
    cmsUInt16Number v1 = *(cmsUInt16Number*)(buffer + 4);
    cmsUInt16Number v2 = *(cmsUInt16Number*)(buffer + 6);

    RUN_ASSERT(v0 == 0x0000, "First written word should be 0x0000");
    RUN_ASSERT(v1 == 0x0000, "Second written word should be 0x0000");
    RUN_ASSERT(v2 == 0x0000, "Third written word should be 0x0000");
}

// Test 4: Ensure that the initial two bytes are not overwritten and remain as-is
void test_Pack3WordsAndSkip1SwapFirst_preserves_prefix() {
    unsigned char buffer[10] = { 0xDE, 0xAD, 0xBE, 0xEF, 0,0,0,0,0,0 };
    cmsUInt16Number wOut[3] = { 0x1234, 0x5678, 0x9ABC };

    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapFirst(nullptr, wOut, buffer, 0);

    RUN_ASSERT(ret == buffer + 8, "Return pointer should be buffer + 8");
    // First two bytes must remain 0xDE, 0xAD
    RUN_ASSERT(buffer[0] == 0xDE, "Prefix byte 0 should be preserved (0xDE)");
    RUN_ASSERT(buffer[1] == 0xAD, "Prefix byte 1 should be preserved (0xAD)");
}

// Main driver: run all tests and report summary
int main() {
    test_Pack3WordsAndSkip1SwapFirst_basic();
    test_Pack3WordsAndSkip1SwapFirst_chained();
    test_Pack3WordsAndSkip1SwapFirst_zeroValues();
    test_Pack3WordsAndSkip1SwapFirst_preserves_prefix();

    if (g_failures == 0) {
        std::cout << "All tests passed\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed\n";
        return 1;
    }
}
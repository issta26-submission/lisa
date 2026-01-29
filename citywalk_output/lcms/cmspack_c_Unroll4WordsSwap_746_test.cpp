// cmspack_unroll4wordsswap_test.cpp
// Lightweight C++11 test suite for the focal function:
//     cmsUInt8Number* Unroll4WordsSwap(CMSREGISTER _cmsTRANSFORM* info,
//                                        CMSREGISTER cmsUInt16Number wIn[],
//
//                                        CMSREGISTER cmsUInt8Number* accum,
//                                        CMSREGISTER cmsUInt32Number Stride)
// This test is designed to be compiled alongside the actual cmsspack/cms lcMS
// project. It avoids any gtest/gmock usage and uses a small, non-terminating
// assertion mechanism to maximize code coverage.
//
// Notes:
// - The function under test reads 4 consecutive 16-bit values from the accum
//   buffer (assumed little-endian) and stores them into wIn in reverse order:
//   wIn[3] = first 16-bit, wIn[2] = second, wIn[1] = third, wIn[0] = fourth.
// - accum is advanced by 8 bytes (4 * 2 bytes) and returned.
// - The info and Stride parameters are unused by this function, but we pass
//   representative values to ensure compatibility with the signature.
//
// Dependencies: includes the focal class dependencies as per the provided snippet.
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import project-specific internal declarations.
// This header is expected to define cmsUInt8Number, cmsUInt16Number,
// cmsUInt32Number, _cmsTRANSFORM, CMSREGISTER, and the function prototype
// for Unroll4WordsSwap.

extern "C" {
    // Declare focal method with C linkage to avoid name mangling issues
    // when linking against the C source cmspack.c.
    cmsUInt8Number* Unroll4WordsSwap(CMSREGISTER _cmsTRANSFORM* info,
                                     CMSREGISTER cmsUInt16Number wIn[],
                                     CMSREGISTER cmsUInt8Number* accum,
                                     CMSREGISTER cmsUInt32Number Stride);
}

// Simple non-terminating test harness
static int g_failures = 0;
#define TEST_CHECK(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << msg \
                  << " (in " << __func__ << " at line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } } while(0)


// Test 1: Basic functionality - verify correct word swap behavior and pointer advancement
// Setup:
//   accum buffer contains 8 bytes representing four 16-bit values in little-endian order:
//     K, Y, M, C  (the comments in the source show these mappings)
//   Expected wIn after call: [C, M, Y, K] i.e. [fourth, third, second, first].
//   The return value should point to accum + 8.
static void test_Unroll4WordsSwap_basic() {
    // Prepare 16 bytes to also allow a follow-up test to reuse the buffer
    // First 8 bytes encode K,Y,M,C in little-endian as 0x1122, 0x3344, 0x5566, 0x7788
    // (i.e., bytes: 22 11 44 33 66 55 88 77)
    uint8_t buffer[16] = {
        0x22, 0x11, // K = 0x1122
        0x44, 0x33, // Y = 0x3344
        0x66, 0x55, // M = 0x5566
        0x88, 0x77, // C = 0x7788
        // Extra data (unused in this test)
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11
    };

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    CMSREGISTER _cmsTRANSFORM* info = nullptr;
    CMSREGISTER cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll4WordsSwap(info, wIn, accum, Stride);

    // Expected values after swap (little-endian interpretation)
    // wIn[3] = 0x1122, wIn[2] = 0x3344, wIn[1] = 0x5566, wIn[0] = 0x7788
    TEST_CHECK(wIn[3] == 0x1122, "wIn[3] should be 0x1122 (K)");
    TEST_CHECK(wIn[2] == 0x3344, "wIn[2] should be 0x3344 (Y)");
    TEST_CHECK(wIn[1] == 0x5566, "wIn[1] should be 0x5566 (M)");
    TEST_CHECK(wIn[0] == 0x7788, "wIn[0] should be 0x7788 (C)");
    // Verify accum advanced by 8 bytes
    TEST_CHECK(ret == buffer + 8, "Return pointer should be accum + 8");

    // Optional: ensure original buffer beyond 8 bytes remains untouched
    TEST_CHECK(buffer[8] == 0xAA, "Buffer byte after 8 should remain unchanged (0xAA)");
}

// Test 2: Sequential usage - ensure subsequent calls read the next 8 bytes correctly
// Setup a 16-byte buffer containing two sets of 4 16-bit words.
// First set: K1,Y1,M1,C1; Second set: K2,Y2,M2,C2
// We will call Unroll4WordsSwap twice, ensuring both reads are correct and independent.
static void test_Unroll4WordsSwap_sequential() {
    uint8_t buffer[32] = {
        // Set 1: K1=0x0102, Y1=0x0304, M1=0x0506, C1=0x0708
        0x02,0x01, 0x04,0x03, 0x06,0x05, 0x08,0x07,
        // Set 2: K2=0x1112, Y2=0x1314, M2=0x1516, C2=0x1718
        0x12,0x11, 0x14,0x13, 0x16,0x15, 0x18,0x17
    };

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    CMSREGISTER _cmsTRANSFORM* info = nullptr;
    CMSREGISTER cmsUInt32Number Stride = 0;

    // First call
    cmsUInt8Number* ret1 = Unroll4WordsSwap(info, wIn, accum, Stride);
    TEST_CHECK(ret1 == buffer + 8, "First call return should be buffer + 8");
    TEST_CHECK(wIn[3] == 0x0102, "First call: wIn[3] should be 0x0102 (K1)");
    TEST_CHECK(wIn[2] == 0x0304, "First call: wIn[2] should be 0x0304 (Y1)");
    TEST_CHECK(wIn[1] == 0x0506, "First call: wIn[1] should be 0x0506 (M1)");
    TEST_CHECK(wIn[0] == 0x0708, "First call: wIn[0] should be 0x0708 (C1)");

    // Second call using remaining 8 bytes
    cmsUInt8Number* ret2 = Unroll4WordsSwap(info, wIn, ret1, Stride);
    TEST_CHECK(ret2 == buffer + 16, "Second call return should be buffer + 16");
    TEST_CHECK(wIn[3] == 0x1112, "Second call: wIn[3] should be 0x1112 (K2)");
    TEST_CHECK(wIn[2] == 0x1314, "Second call: wIn[2] should be 0x1314 (Y2)");
    TEST_CHECK(wIn[1] == 0x1516, "Second call: wIn[1] should be 0x1516 (M2)");
    TEST_CHECK(wIn[0] == 0x1718, "Second call: wIn[0] should be 0x1718 (C2)");
}

// Test 3: Validate that non-null info and Stride do not affect outcome
// This ensures the function's unused parameters do not influence behavior.
static void test_Unroll4WordsSwap_with_non_null_params() {
    uint8_t buffer[16] = {
        0x34,0x12, // K = 0x1234
        0x78,0x56, // Y = 0x5678
        0xBC,0x9A, // M = 0x9ABC
        0xFE,0xDE  // C = 0xDEFE
    };

    cmsUInt16Number wIn[4] = {0,0,0,0};
    cmsUInt8Number* accum = buffer;
    // Construct a dummy non-null info pointer
    struct _cmsTRANSFORM dummyInfo;
    cmsUInt32Number Stride = 4;
    cmsUInt8Number* ret = Unroll4WordsSwap(&dummyInfo, wIn, accum, Stride);

    TEST_CHECK(ret == buffer + 8, "Return should advance by 8 bytes with non-null info and Stride");
    TEST_CHECK(wIn[3] == 0x1234, "With non-null info, wIn[3] should be 0x1234 (K)");
    TEST_CHECK(wIn[2] == 0x5678, "With non-null info, wIn[2] should be 0x5678 (Y)");
    TEST_CHECK(wIn[1] == 0x9ABC, "With non-null info, wIn[1] should be 0x9ABC (M)");
    TEST_CHECK(wIn[0] == 0xDEFE, "With non-null info, wIn[0] should be 0xDEFE (C)");
}


// Entry point for the test suite
int main() {
    std::cout << "Starting Unroll4WordsSwap unit tests...\n";

    test_Unroll4WordsSwap_basic();
    test_Unroll4WordsSwap_sequential();
    test_Unroll4WordsSwap_with_non_null_params();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
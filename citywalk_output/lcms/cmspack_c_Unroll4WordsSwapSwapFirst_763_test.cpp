// Unit test suite for the focal method Unroll4WordsSwapSwapFirst
// This test suite is designed to be compiled along with the project sources
// (which provide the real definitions for types like _cmsTRANSFORM, cmsUInt8Number, etc.).
// No GTest is used; a lightweight in-file test harness is implemented.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <iomanip>


// Include the project's internal headers to bring in the actual type definitions
// and the Unroll4WordsSwapSwapFirst function declaration.
// Adjust the path as needed for your build environment.

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define TEST_EQ_INT(actual, expected, msg) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                      << ", actual: " << (actual) << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[OK] " << msg << std::endl; \
        } \
    } while (0)

#define TEST_PTR_EQ(actual, expected, msg) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL] " << msg << " | pointer mismatch: " \
                      << static_cast<void*>(actual) << " != " \
                      << static_cast<void*>(expected) << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[OK] " << msg << std::endl; \
        } \
    } while (0)

// Test 1: Basic expected behavior with distinct values
// - Memory layout in accum: K, Y, M, C as 16-bit words in that order
// - Function should read K,Y,M,C into wIn as: wIn[2]=K, wIn[1]=Y, wIn[0]=M, wIn[3]=C
// - Return value should be accum advanced by 8 bytes (mem + 8)
void test_Unroll4WordsSwapSwapFirst_basic_case() {
    // Prepare memory: K, Y, M, C
    cmsUInt16Number K = 0x1111;
    cmsUInt16Number Y = 0x2222;
    cmsUInt16Number M = 0x3333;
    cmsUInt16Number C = 0x4444;

    cmsUInt8Number mem[8];
    cmsUInt16Number* p16 = reinterpret_cast<cmsUInt16Number*>(mem);
    p16[0] = M; // M
    p16[1] = Y; // Y
    p16[2] = K; // K
    p16[3] = C; // C

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = mem;
    cmsUInt32Number Stride = 0;
    _cmsTRANSFORM* info = nullptr; // unused parameter (tested function uses cmsUNUSED_PARAMETER)

    cmsUInt8Number* ret = Unroll4WordsSwapSwapFirst(info, wIn, accum, Stride);
    cmsUInt8Number* expectedRet = mem + 8;

    TEST_PTR_EQ(ret, expectedRet, "Unroll4WordsSwapSwapFirst should return accum advanced by 8 bytes");

    // Validate resulting wIn ordering
    TEST_EQ_INT(wIn[0], M, "wIn[0] should be M (third word in memory)");
    TEST_EQ_INT(wIn[1], Y, "wIn[1] should be Y (second word in memory)");
    TEST_EQ_INT(wIn[2], K, "wIn[2] should be K (first word in memory)");
    TEST_EQ_INT(wIn[3], C, "wIn[3] should be C (fourth word in memory)");
}

// Test 2: Edge values to ensure correct handling of 0 and max 16-bit values
void test_Unroll4WordsSwapSwapFirst_edge_values() {
    // Prepare memory: K=0x0000, Y=0xFFFF, M=0x8000, C=0x1234
    cmsUInt16Number K = 0x0000;
    cmsUInt16Number Y = 0xFFFF;
    cmsUInt16Number M = 0x8000;
    cmsUInt16Number C = 0x1234;

    cmsUInt8Number mem[8];
    cmsUInt16Number* p16 = reinterpret_cast<cmsUInt16Number*>(mem);
    p16[0] = M; // M
    p16[1] = Y; // Y
    p16[2] = K; // K
    p16[3] = C; // C

    cmsUInt16Number wIn[4] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD}; // pre-fill to ensure overwrite
    cmsUInt8Number* accum = mem;
    cmsUInt32Number Stride = 0;
    _cmsTRANSFORM* info = nullptr;

    cmsUInt8Number* ret = Unroll4WordsSwapSwapFirst(info, wIn, accum, Stride);
    cmsUInt8Number* expectedRet = mem + 8;

    TEST_PTR_EQ(ret, expectedRet, "Edge values: return pointer advance by 8 bytes");

    // Validate resulting wIn ordering
    TEST_EQ_INT(wIn[0], M, "Edge: wIn[0] should be M (third word in memory)");
    TEST_EQ_INT(wIn[1], Y, "Edge: wIn[1] should be Y (second word in memory)");
    TEST_EQ_INT(wIn[2], K, "Edge: wIn[2] should be K (first word in memory)");
    TEST_EQ_INT(wIn[3], C, "Edge: wIn[3] should be C (fourth word in memory)");
}

// Entry point to run all tests
int main() {
    std::cout << "Starting unit tests for Unroll4WordsSwapSwapFirst..." << std::endl;

    test_Unroll4WordsSwapSwapFirst_basic_case();
    test_Unroll4WordsSwapSwapFirst_edge_values();

    if (g_failures != 0) {
        std::cerr << "Tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed successfully." << std::endl;
        return 0;
    }
}
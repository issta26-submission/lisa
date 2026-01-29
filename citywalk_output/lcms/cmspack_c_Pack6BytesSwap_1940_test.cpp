// Test suite for Pack6BytesSwap in cmspack.c
// This test suite targets the focal method Pack6BytesSwap
// and its class dependencies as provided in the project under test.
// The tests are written for C++11 without Google Test and use a small
// in-house assertion framework to ensure non-terminating checks.
//
// Notes:
// - We assume the project provides cmsUInt8Number, cmsUInt16Number, cmsUInt32Number,
//   and the macro FROM_16_TO_8, as used in the focal method.
// - We link against the library/object that implements Pack6BytesSwap (no STL mocks required).
// - We import "lcms2_internal.h" following the structure shown in the focal class dependencies.

#include <cstring>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>


// Import project dependencies (as per domain knowledge)

// Declare the focal function prototype to enable linking with the implementation.
// Use C linkage in case the function is compiled in C.
extern "C" cmsUInt8Number* Pack6BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride);

// Lightweight test harness: non-terminating assertions (log and continue)
static int g_testFailures = 0;

#define TEST_LOG(msg) \
    do { std::cerr << "[TEST] " << msg << std::endl; } while (0)

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << std::left << std::setw(60) << "Failure: " #actual " == " #expected \
                      << " | actual=" << +static_cast<int>(actual) \
                      << " expected=" << +static_cast<int>(expected) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

#define EXPECT_PTR_EQ(actual, expected) \
    do { \
        if ((void*)(actual) != (void*)(expected)) { \
            std::cerr << "Failure: Pointer mismatch: " #actual " == " #expected \
                      << " | actual=" << actual \
                      << " expected=" << expected \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

#define EXPECT_BYTES_EQ(actualPtr, expectedBytes, len) \
    do { \
        if (std::memcmp((actualPtr), (expectedBytes), (len)) != 0) { \
            std::cerr << "Failure: Bytes mismatch for " #actualPtr " (len=" << (len) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

// Helper: Print a byte buffer in hex for debugging
static void printHexBuffer(const cmsUInt8Number* buf, size_t len) {
    std::cout << "Buffer[";
    for (size_t i = 0; i < len; ++i) {
        if (i) std::cout << " ";
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(buf[i]);
    }
    std::cout << std::dec << "]" << std::endl;
}

// Test 1: Basic correctness - ensure bytes are taken from wOut[5]..wOut[0]
// and written as 8-bit values in reversed order.
static void test_Pack6BytesSwap_basic() {
    TEST_LOG("Running test_Pack6BytesSwap_basic");

    // Prepare input 6x 16-bit words
    // wOut[5]..wOut[0] -> 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006
    // Low bytes are:     0x01,     0x02,     0x03,     0x04,     0x05,     0x06
    // Expected output (reverse order): 0x06, 0x05, 0x04, 0x03, 0x02, 0x01
    cmsUInt16Number wOut[6] = { 0x0006, 0x0005, 0x0004, 0x0003, 0x0002, 0x0001 };

    cmsUInt8Number outBuf[12];
    std::memset(outBuf, 0xAA, sizeof(outBuf)); // pre-fill to detect overruns

    cmsUInt8Number* ret = Pack6BytesSwap(nullptr, wOut, outBuf, 0);

    // Verify returned pointer advanced by 6 bytes
    EXPECT_PTR_EQ(static_cast<void*>(ret), static_cast<void*>(outBuf + 6));

    // Expected bytes: [wOut[5]..wOut[0]] low bytes
    cmsUInt8Number expected[6] = {
        FROM_16_TO_8(wOut[5]),
        FROM_16_TO_8(wOut[4]),
        FROM_16_TO_8(wOut[3]),
        FROM_16_TO_8(wOut[2]),
        FROM_16_TO_8(wOut[1]),
        FROM_16_TO_8(wOut[0])
    };

    // Compare the first 6 bytes of the output
    EXPECT_BYTES_EQ(outBuf, expected, 6);
    // Ensure the rest of the buffer remains unchanged (no writes beyond 6 bytes)
    for (size_t i = 6; i < 12; ++i) {
        EXPECT_EQ(outBuf[i], 0xAA);
    }

    // Optional: print the resulting buffer for debugging
    // printHexBuffer(outBuf, 6);
}

// Test 2: Large variety of values to ensure correct masking in FROM_16_TO_8
static void test_Pack6BytesSwap_variedValues() {
    TEST_LOG("Running test_Pack6BytesSwap_variedValues");

    // wOut contains large 16-bit values; only lower 8 bits should be used
    cmsUInt16Number wOut[6] = {
        0xFFFF, // low byte 0xFF
        0x1234, // 0x34
        0xABCD, // 0xCD
        0x0000, // 0x00
        0x00FF, // 0xFF
        0x7F01  // 0x01
    };

    cmsUInt8Number outBuf[6];
    std::memset(outBuf, 0x00, sizeof(outBuf));

    cmsUInt8Number* ret = Pack6BytesSwap(nullptr, wOut, outBuf, 0);

    // Verify pointer advancement
    EXPECT_PTR_EQ(static_cast<void*>(ret), static_cast<void*>(outBuf + 6));

    // Expected reversed order of low bytes: wOut[5], wOut[4], wOut[3], wOut[2], wOut[1], wOut[0]
    cmsUInt8Number expected[6] = {
        FROM_16_TO_8(wOut[5]), // 0x01
        FROM_16_TO_8(wOut[4]), // 0xFF
        FROM_16_TO_8(wOut[3]), // 0x00
        FROM_16_TO_8(wOut[2]), // 0xCD
        FROM_16_TO_8(wOut[1]), // 0x34
        FROM_16_TO_8(wOut[0])  // 0xFF
    };

    EXPECT_BYTES_EQ(outBuf, expected, 6);

    // Optional: print the resulting buffer for debugging
    // printHexBuffer(outBuf, 6);
}

// Test 3: Ensure output buffer boundary is respected (no writes beyond 6 bytes)
static void test_Pack6BytesSwap_bufferBoundary() {
    TEST_LOG("Running test_Pack6BytesSwap_bufferBoundary");

    cmsUInt16Number wOut[6] = { 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C };
    cmsUInt8Number outBuf[10];
    // Initialize to a known pattern
    for (size_t i = 0; i < 10; ++i) outBuf[i] = static_cast<cmsUInt8Number>(0x5A);

    cmsUInt8Number* ret = Pack6BytesSwap(nullptr, wOut, outBuf, 0);

    // Ret should point to outBuf + 6
    EXPECT_PTR_EQ(static_cast<void*>(ret), static_cast<void*>(outBuf + 6));

    // First 6 bytes should be the swapped 8-bit values
    cmsUInt8Number expected[6] = {
        FROM_16_TO_8(wOut[5]),
        FROM_16_TO_8(wOut[4]),
        FROM_16_TO_8(wOut[3]),
        FROM_16_TO_8(wOut[2]),
        FROM_16_TO_8(wOut[1]),
        FROM_16_TO_8(wOut[0])
    };

    EXPECT_BYTES_EQ(outBuf, expected, 6);

    // Ensure bytes beyond index 5 remain unchanged (still 0x5A)
    for (size_t i = 6; i < 10; ++i) {
        EXPECT_EQ(outBuf[i], static_cast<cmsUInt8Number>(0x5A));
    }
}

// Step 3: Test runner - execute all tests and report summary
static void runAllTests() {
    test_Pack6BytesSwap_basic();
    test_Pack6BytesSwap_variedValues();
    test_Pack6BytesSwap_bufferBoundary();

    TEST_LOG("All tests completed.");
    if (g_testFailures == 0) {
        TEST_LOG("All Pack6BytesSwap tests passed.");
    } else {
        std::cerr << "[TEST SUMMARY] Failures: " << g_testFailures << std::endl;
    }
}

// Main entry point for the test executable
int main() {
    runAllTests();
    // Return non-zero if any test failed
    return g_testFailures;
}
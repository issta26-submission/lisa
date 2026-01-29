// Unit test suite for the focal method:
// Pack3BytesAndSkip1SwapSwapFirstOptimized located in cmspack.c
// The tests are written in C++11, without a third-party test framework.
// We provide minimal test harness and direct function invocation.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Forward declare the minimal types/aliases to match the focal function's signature.
// We do not rely on the real cms* typedefs from LittleCMS; we create lightweight placeholders
// so that the linker can resolve the symbol Pack3BytesAndSkip1SwapSwapFirstOptimized from cmspack.c.

typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// The focal function uses CMSREGISTER and _cmsTRANSFORM type.
// We define a minimal forward declaration for the transform type.
#define CMSREGISTER
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// The focal function has C linkage in the original project; declare it accordingly.
extern "C" cmsUInt8Number* Pack3BytesAndSkip1SwapSwapFirstOptimized(CMSREGISTER _cmsTRANSFORM* info,
                                                     CMSREGISTER cmsUInt16Number wOut[],
                                                     CMSREGISTER cmsUInt8Number* output,
                                                     CMSREGISTER cmsUInt32Number Stride);

// Simple test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

static void log_test_result(const char* testName, bool passed)
{
    g_totalTests++;
    if (!passed) {
        g_failedTests++;
        printf("Test FAILED: %s\n", testName);
    } else {
        printf("Test PASSED: %s\n", testName);
    }
}

// Helper: print a byte array (for debugging)
static void dump_bytes(const cmsUInt8Number* data, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", static_cast<unsigned>(data[i]));
    }
    printf("\n");
}

// Test 1: Basic correctness of three written bytes and return pointer
static bool test_Pack3BytesAndSkip1SwapSwapFirstOptimized_Basic()
{
    // Arrange
    cmsUInt16Number wOut[3] = {0x1234, 0xABCD, 0x0F0F}; // wOut[2]=0x0F0F; wOut[1]=0xABCD; wOut[0]=0x1234
    cmsUInt8Number out[4] = {0xAA, 0xAA, 0xAA, 0xAA};     // sentinel to verify no overrun/writes beyond 4th byte
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirstOptimized(info, wOut, out, Stride);

    // Expected:
    // - out[0] = low byte of wOut[2] = 0x0F
    // - out[1] = low byte of wOut[1] = 0xCD
    // - out[2] = low byte of wOut[0] = 0x34
    // - out[3] remains unchanged (due to the extra output++ that is not a write)
    const cmsUInt8Number expected0 = 0x0F;
    const cmsUInt8Number expected1 = 0xCD;
    const cmsUInt8Number expected2 = 0x34;
    const cmsUInt8Number expected3 = 0xAA; // unchanged sentinel

    bool ok = (ret == out + 4) &&
              (out[0] == expected0) &&
              (out[1] == expected1) &&
              (out[2] == expected2) &&
              (out[3] == expected3);

    // Debug dump (optional)
    // printf("Output bytes: "); dump_bytes(out, 4);

    return ok;
}

// Test 2: Validate behavior when output pointer is offset (write starts at a later position)
static bool test_Pack3BytesAndSkip1SwapSwapFirstOptimized_OffsetPointer()
{
    // Arrange
    cmsUInt16Number wOut[3] = {0x0001, 0x0002, 0x0003}; // low bytes: 0x01, 0x02, 0x03 -> output order: 0x03,0x02,0x01
    cmsUInt8Number out[8] = {0}; // initialize all zeros
    // Pre-fill some surrounding bytes to ensure we can detect unintended writes
    out[0] = 0x11; out[4] = 0x22; out[5] = 0x33; out[6] = 0x44; out[7] = 0x55;
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    // We pass output starting at out + 1
    cmsUInt8Number* begin = out + 1;

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirstOptimized(info, wOut, begin, Stride);

    // Expected:
    // - At begin[0] (out[1]) = 0x03
    // - At begin[1] (out[2]) = 0x02
    // - At begin[2] (out[3]) = 0x01
    // - The byte at out[4] (begin[3+1]) should remain as initialized (0x22)
    bool ok = (ret == begin + 4) &&
              (out[1] == 0x03) &&
              (out[2] == 0x02) &&
              (out[3] == 0x01) &&
              (out[4] == 0x22) && // unchanged
              (out[0] == 0x00) && // untouched
              (out[5] == 0x33) &&
              (out[6] == 0x44) &&
              (out[7] == 0x55);

    return ok;
}

// Test 3: Another distinct input set to verify correct low-byte extraction
static bool test_Pack3BytesAndSkip1SwapSwapFirstOptimized_AnotherSet()
{
    // Arrange
    cmsUInt16Number wOut[3] = {0x0100, 0x0200, 0x0300}; // low bytes: 0x00, 0x00, 0x00
    cmsUInt8Number out[4] = {0x10, 0x20, 0x30, 0x40};
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirstOptimized(info, wOut, out, Stride);

    // Expected: [0]=0x00, [1]=0x00, [2]=0x00, [3] unchanged (0x40)
    bool ok = (ret == out + 4) &&
              (out[0] == 0x00) &&
              (out[1] == 0x00) &&
              (out[2] == 0x00) &&
              (out[3] == 0x40);

    return ok;
}

// Run all tests and report summary
static void run_all_tests()
{
    log_test_result("Basic correctness of three written bytes and return pointer", test_Pack3BytesAndSkip1SwapSwapFirstOptimized_Basic());
    log_test_result("Offset output pointer writes starting at a later position", test_Pack3BytesAndSkip1SwapSwapFirstOptimized_OffsetPointer());
    log_test_result("Another distinct input set produces correct low-bytes", test_Pack3BytesAndSkip1SwapSwapFirstOptimized_AnotherSet());

    // Summary
    printf("Test summary: total=%d, failed=%d\n", g_totalTests, g_failedTests);
}

// Main entry point for the test executable
int main()
{
    // Run tests
    run_all_tests();
    // Non-terminating: do not use exit on failure; return non-zero if any test failed
    return g_failedTests != 0 ? 1 : 0;
}
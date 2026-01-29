// Unit test suite for Pack1WordSkip1SwapFirst in cmspack.c
// This test suite is designed to be compiled with C++11, without GTest.
// It uses a lightweight test harness with non-terminating assertions to maximize coverage.
// The test harness assumes the focal function is available via linking (extern "C").
//
// Important: This test file provides minimal type aliases to compile in environments
// where cms* types are not available. It relies on the actual cmspack.c implementation
// being linked in during the build.

// Domain/technical notes captured for test design:
// - Focal method: Pack1WordSkip1SwapFirst
// - Behavior: increments output by 2, writes wOut[0] as a 16-bit value at the new output location,
//             increments by an additional 2, and returns the updated pointer.
// - Dependencies visible in the provided class dependencies block (Focal Class Dep):
//   _cmsTRANSFORM struct, cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, CMSREGISTER, etc.
// - The Stride and info parameters are unused within the function body (besides a cmsUNUSED_PARAMETER macro).
// - The function's effect is confined to memory writes at output+2 and the return at output+4 from original.

#include <cstdio>
#include <cstddef>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// ---------------------------------------------------------------------------
// Minimal type aliases to facilitate compilation when cms* types are not present.
// These definitions mimic the sizes used in the focal method.
// These are only for the purpose of compiling this test file in isolation.
// The actual project should provide the real cms* types via cmspack.c linkage.
// ---------------------------------------------------------------------------

typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Mimic CMSREGISTER macro used in the focal method
#define CMSREGISTER

// Forward declaration of the _cmsTRANSFORM structure used by the focal method.
// We only need an incomplete type pointer here for the test harness.
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// cmsUNUSED_PARAMETER macro (as used in the focal method)
#ifndef cmsUNUSED_PARAMETER
#define cmsUNUSED_PARAMETER(x) (void)(x)
#endif

extern "C" {

// Declaration of the focal method to be linked from cmspack.c
// The test harness expects the actual implementation to be linked at build time.
cmsUInt8Number* Pack1WordSkip1SwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                        CMSREGISTER cmsUInt16Number wOut[],
                                        CMSREGISTER cmsUInt8Number* output,
                                        CMSREGISTER cmsUInt32Number Stride);
}

// ---------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// - Do not abort on first failure; instead collect stats and report at end.
// - Use simple EXPECT-like macros to guide readability of tests.
// - The tests rely on direct memory inspection to avoid endianness-dependent behavior.
// ---------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_passed_tests = 0;

#define EXPECT(condition, description) \
    do { \
        ++g_total_tests; \
        if (static_cast<bool>(condition)) { \
            ++g_passed_tests; \
        } else { \
            std::cerr << "FAILED: " << (description) \
                      << " (in " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while (0)

static void report_results() {
    std::cout << "Test results: " << g_passed_tests << " / " << g_total_tests << " tests passed.\n";
}

// Helper to invoke the focal function with a fresh buffer
static cmsUInt8Number* call_Pack1WordSkip1SwapFirst(_cmsTRANSFORM* info,
                                                   const cmsUInt16Number wOutValue,
                                                   cmsUInt8Number* buffer,
                                                   cmsUInt32Number Stride,
                                                   cmsUInt8Number expect_dummy_unused = 0) {
    // Prepare input
    cmsUInt16Number wOut[1];
    wOut[0] = wOutValue;

    // Call function
    cmsUInt8Number* ret = Pack1WordSkip1SwapFirst(info, wOut, buffer, Stride);

    // Return value will be used by tests for pointer arithmetic checks
    (void)expect_dummy_unused;
    return ret;
}

// ---------------------------------------------------------------------------
// Test 1: Basic behavior - ensures the function writes the first element to output+2
//           and returns output+4 from the original input pointer.
//           Verifies memory contents at output+2 and the returned pointer.
// ---------------------------------------------------------------------------

static void test_Pack1WordSkip1SwapFirst_BasicWrites() {
    // Prepare input
    _cmsTRANSFORM dummyInfo = { 0 };
    _cmsTRANSFORM* info = &dummyInfo;

    cmsUInt8Number buf[16];
    // Seed before the write to ensure we can detect memory being overwritten only at intended region
    buf[0] = 0x11;
    buf[1] = 0x22;
    buf[2] = 0x33; // will be overwritten by function (via output+2)
    buf[3] = 0x44; // will be overwritten by function (via output+2)
    buf[4] = 0x55; // unaffected region
    for (int i = 5; i < 16; ++i) buf[i] = static_cast<hchar>(0xAA); // fill rest

    cmsUInt32Number Stride = 0;
    cmsUInt16Number wOutVal = 0x1234;

    cmsUInt8Number* origPtr = buf;
    cmsUInt8Number* ret = Pack1WordSkip1SwapFirst(info, &wOutVal, origPtr, Stride);

    // Validate: the function should have returned origPtr + 4
    uintptr_t expectedPtr = reinterpret_cast<uintptr_t>(origPtr) + 4;
    uintptr_t actualPtr = reinterpret_cast<uintptr_t>(ret);
    EXPECT(static_cast<bool>(actualPtr == expectedPtr),
           "Return value should be original pointer advanced by 4 bytes");

    // Validate: buf[0..1] unchanged
    EXPECT(buf[0] == 0x11 && buf[1] == 0x22,
           "Bytes before the write region (buf[0], buf[1]) should remain unchanged");

    // Validate: bytes at output+2 hold wOutVal in little-endian order on host
    // We reconstruct the 16-bit value from two bytes to avoid endianness dependence.
    cmsUInt16Number recovered = static_cast<cmsUInt16Number>(buf[2]) |
                               (static_cast<cmsUInt16Number>(buf[3]) << 8);
    EXPECT(recovered == wOutVal,
           "Two bytes starting at output+2 should contain the value wOut[0] (little-endian interpretation)");

}

// ---------------------------------------------------------------------------
// Test 2: Consistency with non-zero Stride and non-null info
//           Verifies that Stride and info do not affect the outcome.
// ---------------------------------------------------------------------------

static void test_Pack1WordSkip1SwapFirst_WithStrideAndInfo() {
    _cmsTRANSFORM dummyInfo = { 0 };
    _cmsTRANSFORM* info = &dummyInfo;

    cmsUInt8Number buf[12] = {0};
    // Pre-fill to detect unintended modifications outside target region
    for (int i = 0; i < 12; ++i) buf[i] = static_cast<cmsUInt8Number>(i);

    cmsUInt32Number Stride = 0xDEADBEEF;
    cmsUInt16Number wOutVal = 0xABCD;

    cmsUInt8Number* ret = Pack1WordSkip1SwapFirst(info, &wOutVal, buf, Stride);

    // Return value check
    uintptr_t expectedPtr = reinterpret_cast<uintptr_t>(buf) + 4;
    uintptr_t actualPtr = reinterpret_cast<uintptr_t>(ret);
    EXPECT(static_cast<bool>(actualPtr == expectedPtr),
           "Return value should be original buffer pointer advanced by 4 bytes (Stride ignored)");

    // Memory check: bytes at 2 and 3 should represent wOutVal in little-endian
    cmsUInt16Number val = static_cast<cmsUInt16Number>(buf[2]) |
                          (static_cast<cmsUInt16Number>(buf[3]) << 8);
    EXPECT(val == wOutVal,
           "Memory at buffer[2..3] should contain wOut[0] regardless of Stride or info");

}

// ---------------------------------------------------------------------------
// Test 3: Boundary and endianness sanity
//           Use a different value to ensure correctness across multiple runs.
// ---------------------------------------------------------------------------

static void test_Pack1WordSkip1SwapFirst_MaxValueAndEndianCheck() {
    _cmsTRANSFORM dummyInfo = { 0 };
    _cmsTRANSFORM* info = &dummyInfo;

    cmsUInt8Number buf[8] = {0};
    buf[0] = 0xAA;
    buf[1] = 0xBB;
    // Do not touch buf[4..] to keep test simple
    cmsUInt16Number wOutVal = 0xFFFF; // max 16-bit value

    cmsUInt8Number* ret = Pack1WordSkip1SwapFirst(info, &wOutVal, buf, 0);

    // Return value
    uintptr_t expectedPtr = reinterpret_cast<uintptr_t>(buf) + 4;
    uintptr_t actualPtr = reinterpret_cast<uintptr_t>(ret);
    EXPECT(static_cast<bool>(actualPtr == expectedPtr),
           "Return pointer should be buffer+4 for max value test");

    // Endianness-insensitive check: reconstruct 16-bit from affected bytes
    cmsUInt16Number val = static_cast<cmsUInt16Number>(buf[2]) |
                          (static_cast<cmsUInt16Number>(buf[3]) << 8);
    EXPECT(val == wOutVal,
           "When wOut[0] is max (0xFFFF), bytes at buffer[2..3] must encode 0xFFFF in host endianness");
}

// ---------------------------------------------------------------------------
// Main entry: run all tests and report results without terminating on first failure.
// ---------------------------------------------------------------------------

int main() {
    // Run tests
    test_Pack1WordSkip1SwapFirst_BasicWrites();
    test_Pack1WordSkip1SwapFirst_WithStrideAndInfo();
    test_Pack1WordSkip1SwapFirst_MaxValueAndEndianCheck();

    // Report
    report_results();

    // Return non-zero if any test failed
    if (g_passed_tests != g_total_tests) {
        return 1;
    }
    return 0;
}
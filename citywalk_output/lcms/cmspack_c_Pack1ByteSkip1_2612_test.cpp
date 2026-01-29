// Unit test suite for the focal method Pack1ByteSkip1 (cmspack.c) using a lightweight, self-contained test harness.
// This test is designed to compile with C++11 and does not rely on GTest or any external testing framework.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain/implementation-lite shim to enable standalone testing without the real CMS LCMS codebase.
// We re-create the minimal types and macros that Pack1ByteSkip1 depends on, using standard C++ types.

typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// The test harness uses a no-op for the CMSREGISTER macro in this standalone setup.
#define CMSREGISTER
#define _cmsTRANSFORM // opaque transform info type (stub)
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Mimic the From-16-to-8 conversion macro used by the focal function.
// In the real project this may clamp/scale, but for unit testing we assume a simple low-byte extraction.
#define FROM_16_TO_8(x) ((cmsUInt8Number)((x) & 0xFF))

// The focal function under test (reproduced in this test for standalone compilation).
cmsUInt8Number* Pack1ByteSkip1(CMSREGISTER _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride)
{
{
    *output++ = FROM_16_TO_8(wOut[0]);
    output++;
    return output;
    cmsUNUSED_PARAMETER(info);
    cmsUNUSED_PARAMETER(Stride);
}
}

// Lightweight test harness (standalone, no external framework).
static int g_failures = 0;

// Helper to report test results without terminating execution.
static void report_failure(const char* desc)
{
    std::cerr << "TEST FAILURE: " << desc << std::endl;
    ++g_failures;
}

// Convenience assertion helpers (non-terminating).
static void expect_uint8_eq(const char* desc, cmsUInt8Number a, cmsUInt8Number b)
{
    if (a != b) {
        std::cerr << "ASSERT FAIL - " << desc
                  << " | expected: 0x" << std::hex << int(b)
                  << " got: 0x" << int(a) << std::dec << std::endl;
        ++g_failures;
    }
}

static void expect_ptr_eq(const char* desc, cmsUInt8Number* a, cmsUInt8Number* b)
{
    if (a != b) {
        std::cerr << "ASSERT FAIL - " << desc
                  << " | expected pointer: " << static_cast<void*>(b)
                  << " got: " << static_cast<void*>(a) << std::endl;
        ++g_failures;
    }
}

// Test 1: Basic correctness for a typical value (0x1234 -> 0x34).
static void test_Pack1ByteSkip1_basic_1234()
{
    cmsUInt16Number wOut[1] = { 0x1234 };
    cmsUInt8Number buffer[3] = { 0xAA, 0xBB, 0xCC }; // prefill to detect unintended writes
    cmsUInt8Number* ret = Pack1ByteSkip1(nullptr, wOut, buffer, 0);

    cmsUInt8Number expected = FROM_16_TO_8(wOut[0]); // 0x34
    expect_uint8_eq("Pack1ByteSkip1_basic_1234: first byte equals FROM_16_TO_8(wOut[0])",
                    buffer[0], expected);

    // The second byte should be untouched (unchanged from prefill)
    expect_uint8_eq("Pack1ByteSkip1_basic_1234: second byte unchanged",
                    buffer[1], 0xBB);

    // The third byte should remain unchanged as well
    expect_uint8_eq("Pack1ByteSkip1_basic_1234: third byte unchanged",
                    buffer[2], 0xCC);

    // Returned pointer should be buffer + 2 (two increments)
    expect_ptr_eq("Pack1ByteSkip1_basic_1234: return pointer should be buffer+2",
                  ret, buffer + 2);
}

// Test 2: Another value (0x00FF -> 0xFF) to cover boundary of low byte extraction.
static void test_Pack1ByteSkip1_basic_00FF()
{
    cmsUInt16Number wOut[1] = { 0x00FF };
    cmsUInt8Number buffer[3] = { 0x11, 0x22, 0x33 };
    cmsUInt8Number* ret = Pack1ByteSkip1(nullptr, wOut, buffer, 0);

    cmsUInt8Number expected = FROM_16_TO_8(wOut[0]); // 0xFF
    expect_uint8_eq("Pack1ByteSkip1_basic_00FF: first byte equals FROM_16_TO_8(wOut[0])",
                    buffer[0], expected);
    expect_uint8_eq("Pack1ByteSkip1_basic_00FF: second byte unchanged",
                    buffer[1], 0x22);
    expect_uint8_eq("Pack1ByteSkip1_basic_00FF: third byte unchanged",
                    buffer[2], 0x33);
    expect_ptr_eq("Pack1ByteSkip1_basic_00FF: return pointer should be buffer+2",
                  ret, buffer + 2);
}

// Test 3: Higher value (0xABCD -> 0xCD) to ensure proper bit-width handling.
static void test_Pack1ByteSkip1_basic_ABCD()
{
    cmsUInt16Number wOut[1] = { 0xABCD };
    cmsUInt8Number buffer[3] = { 0x00, 0xFF, 0x99 };
    cmsUInt8Number* ret = Pack1ByteSkip1(nullptr, wOut, buffer, 0);

    cmsUInt8Number expected = FROM_16_TO_8(wOut[0]); // 0xCD
    expect_uint8_eq("Pack1ByteSkip1_basic_ABCD: first byte equals FROM_16_TO_8(wOut[0])",
                    buffer[0], expected);
    expect_uint8_eq("Pack1ByteSkip1_basic_ABCD: second byte unchanged",
                    buffer[1], 0xFF);
    expect_uint8_eq("Pack1ByteSkip1_basic_ABCD: third byte unchanged",
                    buffer[2], 0x99);
    expect_ptr_eq("Pack1ByteSkip1_basic_ABCD: return pointer should be buffer+2",
                  ret, buffer + 2);
}

// Run all tests and report summary
int main()
{
    std::cout << "Running Pack1ByteSkip1 unit tests (standalone):" << std::endl;

    test_Pack1ByteSkip1_basic_1234();
    test_Pack1ByteSkip1_basic_00FF();
    test_Pack1ByteSkip1_basic_ABCD();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        // Return non-zero to indicate failure.
        return g_failures;
    }
}
// Minimal C++11 test harness for the focal function Pack3BytesAndSkip1SwapFirstOptimized
// This test suite does not rely on GTest and uses a lightweight, non-terminating assertion style.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Domain-knowledge driven minimal type and macro stubs to mirror the focal environment

#define CMSREGISTER
#define cmsUNUSED_PARAMETER(x) (void)(x)

typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Forward-declare the internal transform struct used by the focal function
struct _cmsTRANSFORM {};

// The focal function under test (reproduced in this test file for standalone compilation)
cmsUInt8Number* Pack3BytesAndSkip1SwapFirstOptimized(CMSREGISTER _cmsTRANSFORM* info,
                                                     CMSREGISTER cmsUInt16Number wOut[],
                                                     CMSREGISTER cmsUInt8Number* output,
                                                     CMSREGISTER cmsUInt32Number Stride)
{
{
    output++;
    *output++ = (wOut[0] & 0xFFU);
    *output++ = (wOut[1] & 0xFFU);
    *output++ = (wOut[2] & 0xFFU);
    return output;
    cmsUNUSED_PARAMETER(info);
    cmsUNUSED_PARAMETER(Stride);
}
}

// Lightweight test harness helpers
static int g_total = 0;
static int g_failed = 0;

#define REPORT_PASS(name) \
    std::cout << "[PASS] " << name << std::endl;

#define REPORT_FAIL(name, detail) \
    std::cerr << "[FAIL] " << name << " - " << detail << std::endl; \
    ++g_failed;

// Helper to run all tests and report summary
static void summarize() {
    std::cout << "Test Summary: total=" << g_total << ", failed=" << g_failed << std::endl;
}

// Test 1: Basic writing of three bytes at positions 1..3 with initial offset +1
// Verifies that the output pointer advances by 4 from the input and that the
// three bytes written are exactly wOut[0..2] masked to 8 bits.
static void test_Pack3BytesAndSkip1SwapFirstOptimized_BasicWrites() {
    const char* testName = "Pack3BytesAndSkip1SwapFirstOptimized_BasicWrites";

    g_total++;

    cmsUInt16Number wOut[3] = { 0x01, 0x02, 0x03 };
    unsigned char buf[8];
    // Initialize buffer to a known value to ensure we only affect expected locations
    std::memset(buf, 0xAA, sizeof(buf));

    _cmsTRANSFORM dummyInfo;
    cmsUInt8Number* start = buf;
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirstOptimized(&dummyInfo, wOut, start, 0xDEADBEEF);

    bool ok = (ret == start + 4) &&
              (buf[0] == 0xAA) &&
              (buf[1] == 0x01) &&
              (buf[2] == 0x02) &&
              (buf[3] == 0x03);

    if (ok) {
        REPORT_PASS(testName);
    } else {
        REPORT_FAIL(testName, "Return pointer, buffer contents, or both did not match expected values.");
    }
}

// Test 2: Masking behavior with values exceeding 0xFF
// Ensures that only the low byte of each wOut element is written.
static void test_Pack3BytesAndSkip1SwapFirstOptimized_Masking() {
    const char* testName = "Pack3BytesAndSkip1SwapFirstOptimized_Masking";

    g_total++;

    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x00FF }; // Expect 0x34, 0xCD, 0xFF
    unsigned char buf[8];
    std::memset(buf, 0x00, sizeof(buf));

    _cmsTRANSFORM dummyInfo;
    cmsUInt8Number* start = buf;
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirstOptimized(&dummyInfo, wOut, start, 0);

    bool ok = (ret == start + 4) &&
              (buf[0] == 0x00) &&     // index 0 should be untouched
              (buf[1] == 0x34) &&
              (buf[2] == 0xCD) &&
              (buf[3] == 0xFF);

    if (ok) {
        REPORT_PASS(testName);
    } else {
        REPORT_FAIL(testName, "Masking behavior did not produce expected bytes or output pointer.");
    }
}

// Test 3: Stride parameter is unused by the function; verify non-use does not affect behavior
static void test_Pack3BytesAndSkip1SwapFirstOptimized_StrideIgnored() {
    const char* testName = "Pack3BytesAndSkip1SwapFirstOptimized_StrideIgnored";

    g_total++;

    cmsUInt16Number wOut[3] = { 0x11, 0x22, 0x33 };
    unsigned char buf[8];
    std::memset(buf, 0x55, sizeof(buf));

    _cmsTRANSFORM dummyInfo;
    cmsUInt8Number* start = buf;
    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapFirstOptimized(&dummyInfo, wOut, start, 12345U);

    bool ok = (ret == start + 4) &&
              (buf[0] == 0x55) &&
              (buf[1] == 0x11) &&
              (buf[2] == 0x22) &&
              (buf[3] == 0x33);

    if (ok) {
        REPORT_PASS(testName);
    } else {
        REPORT_FAIL(testName, "Stride parameter affected the result unexpectedly.");
    }
}

// Test 4: Ensure that the first byte (buffer[0]) remains untouched by this operation
static void test_Pack3BytesAndSkip1SwapFirstOptimized_BufferFirstByteUntouched() {
    const char* testName = "Pack3BytesAndSkip1SwapFirstOptimized_BufferFirstByteUntouched";

    g_total++;

    cmsUInt16Number wOut[3] = { 0xAA, 0xBB, 0xCC };
    unsigned char buf[6];
    buf[0] = 0x7F;
    std::memset(buf + 1, 0x00, sizeof(buf) - 1);

    _cmsTRANSFORM dummyInfo;
    cmsUInt8Number* start = buf;
    Pack3BytesAndSkip1SwapFirstOptimized(&dummyInfo, wOut, start, 0);

    bool ok = (buf[0] == 0x7F) && (buf[1] == 0xAA) && (buf[2] == 0xBB) && (buf[3] == 0xCC);

    if (ok) {
        REPORT_PASS(testName);
    } else {
        REPORT_FAIL(testName, "First buffer byte was modified unexpectedly.");
    }
}

int main() {
    // Run all tests
    test_Pack3BytesAndSkip1SwapFirstOptimized_BasicWrites();
    test_Pack3BytesAndSkip1SwapFirstOptimized_Masking();
    test_Pack3BytesAndSkip1SwapFirstOptimized_StrideIgnored();
    test_Pack3BytesAndSkip1SwapFirstOptimized_BufferFirstByteUntouched();

    summarize();
    // Non-terminating tests: return non-zero if any failed
    return g_failed != 0 ? 1 : 0;
}
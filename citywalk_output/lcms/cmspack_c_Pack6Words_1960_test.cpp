// C++11 compatible unit tests for the focal function Pack6Words in cmspack.c
// This test suite avoids GTest and uses a lightweight, non-terminating assertion mechanism.
// It focuses on the behavior of Pack6Words: writing exactly six 16-bit words to the
// output buffer in sequence and returning a pointer advanced by 12 bytes (6 * 2).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Forward declare the focal function using C linkage to avoid C++ name mangling issues.
// We intentionally declare a minimal, liberal prototype that should link with the
// actual implementation in cmspack.c. The first parameter (info) is not used in
// the function body (as per the provided code), so we can safely use a void* here.
extern "C" {
    typedef unsigned char cmsUInt8Number;
    typedef unsigned short cmsUInt16Number;
    typedef unsigned int cmsUInt32Number;

    // Prototype for the function under test. We keep parameters compatible in usage:
    // info: unused (nullptr in tests)
    // wOut: six 16-bit words to pack
    // output: destination buffer
    // Stride: unused in the provided implementation
    cmsUInt8Number* Pack6Words(void* info,
                               cmsUInt16Number wOut[],
                               cmsUInt8Number* output,
                               cmsUInt32Number Stride);
}

// Lightweight test framework (non-terminating assertions)
static int g_testCount = 0;
static int g_failureCount = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_testCount; \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " << msg << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failureCount; \
    } \
} while(0)

//////////////////////////////////////////////////////////////////
// Test 1: Basic packing of six predefined words, verify output bytes
// and the returned pointer is advanced by exactly 12 bytes.
//////////////////////////////////////////////////////////////////
void test_Pack6Words_basic_packing()
{
    // Given
    cmsUInt8Number buffer[12];
    cmsUInt16Number wOut[6] = { 0x1122, 0x3344, 0x5566, 0x7788, 0x99AA, 0xBBCC };

    // When
    cmsUInt8Number* ret = Pack6Words(nullptr, wOut, buffer, 0);

    // Then
    // 1) Returned pointer should advance by 12 bytes (6 words * 2 bytes each)
    TEST_ASSERT(ret == buffer + 12, "Pack6Words should return buffer advanced by 12 bytes");

    // 2) Each 2-byte word in the output should correspond to the input wOut[]
    for (int i = 0; i < 6; ++i) {
        uint16_t val;
        std::memcpy(&val, buffer + i * 2, 2);
        TEST_ASSERT(val == wOut[i], "Pack6Words word mismatch at index " << i);
    }
}

//////////////////////////////////////////////////////////////////
// Test 2: Zero values (all zeros). Verifies deterministic behavior.
//////////////////////////////////////////////////////////////////
void test_Pack6Words_zero_values()
{
    cmsUInt8Number buffer[12];
    cmsUInt16Number wOut[6] = { 0, 0, 0, 0, 0, 0 };

    cmsUInt8Number* ret = Pack6Words(nullptr, wOut, buffer, 0);

    TEST_ASSERT(ret == buffer + 12, "Pack6Words with zeros should advance by 12");

    for (int i = 0; i < 6; ++i) {
        uint16_t val;
        std::memcpy(&val, buffer + i * 2, 2);
        TEST_ASSERT(val == 0, "Expected 0 at index " << i);
    }
}

//////////////////////////////////////////////////////////////////
// Test 3: Non-zero values with non-zero Stride (should have no effect on result)
//////////////////////////////////////////////////////////////////
void test_Pack6Words_with_stride_ignored()
{
    cmsUInt8Number buffer[12];
    cmsUInt16Number wOut[6] = { 0x0101, 0x0202, 0x0303, 0x0404, 0x0505, 0x0606 };

    cmsUInt32Number stride = 12345; // non-zero stride, should be ignored by implementation
    cmsUInt8Number* ret = Pack6Words(nullptr, wOut, buffer, stride);

    TEST_ASSERT(ret == buffer + 12, "Pack6Words should ignore Stride and advance by 12");

    for (int i = 0; i < 6; ++i) {
        uint16_t val;
        std::memcpy(&val, buffer + i * 2, 2);
        TEST_ASSERT(val == wOut[i], "Mismatch at index " << i << " with non-zero Stride");
    }
}

//////////////////////////////////////////////////////////////////
// Test 4: End-to-end variety check with large values
//////////////////////////////////////////////////////////////////
void test_Pack6Words_large_values()
{
    cmsUInt8Number buffer[12];
    cmsUInt16Number wOut[6] = { 0xFFFF, 0x8000, 0x1234, 0xABCD, 0x00FF, 0x0100 };

    cmsUInt8Number* ret = Pack6Words(nullptr, wOut, buffer, 0);

    TEST_ASSERT(ret == buffer + 12, "Pack6Words should advance by 12 for large values");

    for (int i = 0; i < 6; ++i) {
        uint16_t val;
        std::memcpy(&val, buffer + i * 2, 2);
        TEST_ASSERT(val == wOut[i], "Mismatch at index " << i << " for large values");
    }
}

//////////////////////////////////////////////////////////////////
// Main entry point: run all tests and report summary
//////////////////////////////////////////////////////////////////
int main()
{
    // Run tests
    test_Pack6Words_basic_packing();
    test_Pack6Words_zero_values();
    test_Pack6Words_with_stride_ignored();
    test_Pack6Words_large_values();

    // Summary
    std::cout << "Pack6Words test suite completed. "
              << "Total tests: " << g_testCount
              << ", Failures: " << g_failureCount
              << std::endl;

    // Non-terminating assertion style: return code indicates failure count
    return (g_failureCount == 0) ? 0 : 1;
}
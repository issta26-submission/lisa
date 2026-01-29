// Unit tests for Pack1WordBigEndian in cmspack.c
// This test suite is written for C++11 without GoogleTest.
// It calls the C function Pack1WordBigEndian via extern "C" linkage and validates
// that it correctly writes the big-endian 16-bit word into the output buffer.
// The tests are designed to be non-terminating (they report failures but continue).

#include <lcms2_internal.h>
#include <cstring>
#include <cstdint>
#include <iostream>


// Forward declare the C types and function prototype from the project under test.
// We avoid pulling in large headers to keep the test self-contained as a unit test harness.

typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// The project defines these types via its headers. We mirror their sizes here to
// provide a compatible declaration for linking.
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// The CMSREGISTER macro is typically defined as empty for non-ABI-affecting purposes.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

extern "C" cmsUInt8Number* Pack1WordBigEndian(CMSREGISTER _cmsTRANSFORM* info,
                                            CMSREGISTER cmsUInt16Number wOut[],
                                            CMSREGISTER cmsUInt8Number* output,
                                            CMSREGISTER cmsUInt32Number Stride);

// Helper to perform a 16-bit endian swap consistent with the internal macro.
// We use this to compute the expected value written by Pack1WordBigEndian.
static inline uint16_t swap16(uint16_t x) {
    return (static_cast<uint16_t>(x) << 8) | (static_cast<uint16_t>(x) >> 8);
}

// Simple non-terminating test assertion.
// Returns true on pass, false on fail and prints diagnostic on failure.
static bool assert_true(bool condition, const char* testName, const char* message) {
    if (!condition) {
        std::cerr << "TEST FAIL: " << testName << " - " << (message ? message : "condition failed") << "\n";
        return false;
    }
    // On pass, can optionally print verbose success; omitted to reduce noise.
    return true;
}

// Test 1: Basic single word packing with wOut = 0x1234
static bool test_Pack1WordBigEndian_basic_single_swap() {
    const char* testName = "Pack1WordBigEndian_basic_single_swap (0x1234)";

    _cmsTRANSFORM* info = nullptr;  // Info is unused by the function; safe to pass null.
    cmsUInt16Number wOut = 0x1234;
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret = Pack1WordBigEndian(info, &wOut, buffer, 0);

    // Expect the function to advance the output pointer by 2 bytes.
    bool okPtr = (ret == buffer + 2);
    assert_true(okPtr, testName, "returned pointer should be buffer + 2");

    // Expected two bytes in memory are the endian-swapped 0x1234.
    uint16_t expected = swap16(wOut); // 0x3412

    // Read the two bytes written into buffer as a native 16-bit value using memcpy
    uint16_t actual;
    std::memcpy(&actual, buffer, 2);

    bool okBytes = (actual == expected);
    return okBytes && okPtr;
}

// Test 2: Basic single word packing with wOut = 0x0000
static bool test_Pack1WordBigEndian_basic_zero() {
    const char* testName = "Pack1WordBigEndian_basic_zero (0x0000)";

    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut = 0x0000;
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret = Pack1WordBigEndian(info, &wOut, buffer, 0);

    bool okPtr = (ret == buffer + 2);
    assert_true(okPtr, testName, "returned pointer should be buffer + 2");

    uint16_t expected = swap16(wOut); // 0x0000

    uint16_t actual;
    std::memcpy(&actual, buffer, 2);

    bool okBytes = (actual == expected);
    return okBytes && okPtr;
}

// Test 3: Basic single word packing with wOut = 0xFFFF
static bool test_Pack1WordBigEndian_basic_all255() {
    const char* testName = "Pack1WordBigEndian_basic_all255 (0xFFFF)";

    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut = 0xFFFF;
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret = Pack1WordBigEndian(info, &wOut, buffer, 0);

    bool okPtr = (ret == buffer + 2);
    assert_true(okPtr, testName, "returned pointer should be buffer + 2");

    uint16_t expected = swap16(wOut); // 0xFFFF

    uint16_t actual;
    std::memcpy(&actual, buffer, 2);

    bool okBytes = (actual == expected);
    return okBytes && okPtr;
}

// Test 4: Consecutive calls write two words sequentially in the same buffer
static bool test_Pack1WordBigEndian_consecutive_writes() {
    const char* testName = "Pack1WordBigEndian_consecutive_writes";

    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut1 = 0x1234;
    cmsUInt16Number wOut2 = 0xABCD;
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0, sizeof(buffer));

    cmsUInt8Number* ret1 = Pack1WordBigEndian(info, &wOut1, buffer, 0);
    bool okPtr1 = (ret1 == buffer + 2);
    if (!okPtr1) {
        std::cerr << "TEST FAIL: " << testName << " - first return pointer mismatch\n";
        return false;
    }

    cmsUInt8Number* ret2 = Pack1WordBigEndian(info, &wOut2, buffer + 2, 0);
    bool okPtr2 = (ret2 == buffer + 4);
    if (!okPtr2) {
        std::cerr << "TEST FAIL: " << testName << " - second return pointer mismatch\n";
        return false;
    }

    // Verify first 2 bytes for wOut1
    uint16_t expected1 = swap16(wOut1);
    uint16_t actual1;
    std::memcpy(&actual1, buffer, 2);
    bool okBytes1 = (actual1 == expected1);
    if (!okBytes1) {
        std::cerr << "TEST FAIL: " << testName
                  << " - first word bytes mismatch. expected 0x" 
                  << std::hex << expected1 << ", got 0x" << actual1 << std::dec << "\n";
        return false;
    }

    // Verify next 2 bytes for wOut2
    uint16_t expected2 = swap16(wOut2);
    uint16_t actual2;
    std::memcpy(&actual2, buffer + 2, 2);
    bool okBytes2 = (actual2 == expected2);
    if (!okBytes2) {
        std::cerr << "TEST FAIL: " << testName
                  << " - second word bytes mismatch. expected 0x"
                  << std::hex << expected2 << ", got 0x" << actual2 << std::dec << "\n";
        return false;
    }

    // Also verify the final return pointer position
    return true;
}

// Driver to run all tests and report summary
int main() {
    int failures = 0;

    if (!test_Pack1WordBigEndian_basic_single_swap()) {
        ++failures;
    }
    if (!test_Pack1WordBigEndian_basic_zero()) {
        ++failures;
    }
    if (!test_Pack1WordBigEndian_basic_all255()) {
        ++failures;
    }
    if (!test_Pack1WordBigEndian_consecutive_writes()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED\n";
        return 1;
    }
}
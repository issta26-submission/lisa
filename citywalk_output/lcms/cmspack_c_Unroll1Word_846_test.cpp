#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Include the internal CMS/Little CMS headers to get the correct type definitions

// Declare the focal function with C linkage to ensure proper symbol resolution
extern "C" {
    cmsUInt8Number* Unroll1Word(CMSREGISTER _cmsTRANSFORM* info,
                                CMSREGISTER cmsUInt16Number wIn[],
                                CMSREGISTER cmsUInt8Number* accum,
                                CMSREGISTER cmsUInt32Number Stride);
}

// Simple test harness (no Google Test). Uses non-terminating checks that log failures.

static int g_test_failures = 0;

#define CHECK(expr, msg) \
    do { \
        if(!(expr)) { \
            std::cerr << "TEST FAILURE: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(false)


// Test 1: Basic behavior
// - Reads a 16-bit value from accum (two bytes)
// - Writes that value to wIn[0], wIn[1], wIn[2]
// - Increments accum by 2
void test_Unroll1Word_BasicBehavior() {
    // Prepare inputs
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[8] = {0}; // ensure there is enough space
    // Set two bytes that encode a known 16-bit value
    accumBuf[0] = 0x34; // LSB (depends on endianness)
    accumBuf[1] = 0x12; // MSB (depends on endianness)
    // Compute expected value by interpreting the two bytes as a cmsUInt16Number on this host
    cmsUInt16Number expectedValue;
    std::memcpy(&expectedValue, accumBuf, sizeof(expectedValue));

    cmsUInt8Number* accumPtr = accumBuf;
    cmsUInt8Number* newAccum = Unroll1Word(nullptr, wIn, accumPtr, 0);

    // The three outputs should equal the 16-bit value read from accum
    CHECK(wIn[0] == expectedValue, "Unroll1Word: wIn[0] should equal the value read from accum");
    CHECK(wIn[1] == expectedValue, "Unroll1Word: wIn[1] should equal the value read from accum");
    CHECK(wIn[2] == expectedValue, "Unroll1Word: wIn[2] should equal the value read from accum");

    // accum should be advanced by 2 bytes
    CHECK(newAccum == accumPtr + 2, "Unroll1Word: accum should advance by 2 bytes");
}


// Test 2: info and Stride are ignored by the implementation
// - Provide non-null info and a non-zero Stride to ensure they don't affect results
void test_Unroll1Word_InfoAndStrideIgnored() {
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[4] = {0};
    accumBuf[0] = 0xAA; accumBuf[1] = 0xBB;
    cmsUInt16Number expectedValue;
    std::memcpy(&expectedValue, accumBuf, sizeof(expectedValue));

    cmsUInt8Number* accumPtr = accumBuf;
    _cmsTRANSFORM dummyInfo; // structure type should be compatible with CMSREGISTER parameter
    cmsUInt8Number* ret = Unroll1Word(&dummyInfo, wIn, accumPtr, 12345);

    CHECK(wIn[0] == expectedValue, "Unroll1Word with non-null info should produce correct wIn[0]");
    CHECK(wIn[1] == expectedValue, "Unroll1Word with non-null info should produce correct wIn[1]");
    CHECK(wIn[2] == expectedValue, "Unroll1Word with non-null info should produce correct wIn[2]");
    CHECK(ret == accumPtr + 2, "Unroll1Word should advance accum by 2 bytes even when Stride != 0");
}


// Test 3: Multiple sequential calls using different accum regions
// - First call uses first two bytes to set value 1
// - Second call uses next two bytes to set value 2
void test_Unroll1Word_MultipleSequentialCalls() {
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[8] = {0};

    // First value region
    accumBuf[0] = 0x11; accumBuf[1] = 0x22;
    cmsUInt16Number expectedValue1;
    std::memcpy(&expectedValue1, accumBuf, sizeof(expectedValue1));
    cmsUInt8Number* p1 = accumBuf;
    cmsUInt8Number* r1 = Unroll1Word(nullptr, wIn, p1, 0);
    CHECK(wIn[0] == expectedValue1, "Unroll1Word first call should read first two bytes as value1");
    CHECK(wIn[1] == expectedValue1, "Unroll1Word first call should write value1 to wIn[1]");
    CHECK(wIn[2] == expectedValue1, "Unroll1Word first call should write value1 to wIn[2]");
    CHECK(r1 == p1 + 2, "First call should advance accum by 2 bytes");

    // Prepare second region
    accumBuf[2] = 0xAA; accumBuf[3] = 0x55;
    cmsUInt16Number expectedValue2;
    std::memcpy(&expectedValue2, accumBuf + 2, sizeof(expectedValue2));

    cmsUInt8Number* p2 = accumBuf + 2;
    cmsUInt8Number* r2 = Unroll1Word(nullptr, wIn, p2, 0);
    CHECK(wIn[0] == expectedValue2, "Unroll1Word second call should read second two bytes as value2");
    CHECK(wIn[1] == expectedValue2, "Unroll1Word second call should write value2 to wIn[1]");
    CHECK(wIn[2] == expectedValue2, "Unroll1Word second call should write value2 to wIn[2]");
    CHECK(r2 == accumBuf + 4, "Second call should advance accum by 2 bytes from its start");
}


// Entry point for tests
int main() {
    std::cout << "Running Unroll1Word unit tests (C++11, no gtest)\n";

    test_Unroll1Word_BasicBehavior();
    test_Unroll1Word_InfoAndStrideIgnored();
    test_Unroll1Word_MultipleSequentialCalls();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
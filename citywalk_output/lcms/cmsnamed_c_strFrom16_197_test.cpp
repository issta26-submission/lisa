// Unit test suite for the focal method: strFrom16 in cmsnamed.c
// This test is designed for C++11 environments without GoogleTest.
// It uses a lightweight, non-terminating assertion approach and prints
// per-test results to stdout/stderr for visibility.
// The goal is to validate that strFrom16 encodes a 16-bit value into
// a 3-byte array as: [high-byte, low-byte, 0] (big-endian style).

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstdint>


// The real project defines cmsUInt16Number (likely as unsigned short).
// To keep the test decoupled from project headers, we provide a matching alias.
typedef uint16_t cmsUInt16Number;

// Forward declaration of the focal function (C linkage).
extern "C" void strFrom16(char str[3], cmsUInt16Number n);

// Lightweight test runner: executes a single test case and reports outcome.
// It uses non-terminating assertions (console logging) so subsequent tests run.
static int runTestCase(const std::string& name,
                       cmsUInt16Number input,
                       unsigned char expected0,
                       unsigned char expected1)
{
    char out[3] = {0, 0, 0};
    strFrom16(out, input);

    unsigned char actual0 = static_cast<unsigned char>(out[0]);
    unsigned char actual1 = static_cast<unsigned char>(out[1]);
    unsigned char actual2 = static_cast<unsigned char>(out[2]);

    int localFails = 0;
    if (actual0 != expected0) {
        std::cerr << "FAIL  " << name << ": str[0] = "
                  << int(actual0) << ", expected " << int(expected0) << std::endl;
        localFails++;
    }
    if (actual1 != expected1) {
        std::cerr << "FAIL  " << name << ": str[1] = "
                  << int(actual1) << ", expected " << int(expected1) << std::endl;
        localFails++;
    }
    if (actual2 != 0) {
        std::cerr << "FAIL  " << name << ": str[2] = "
                  << int(actual2) << ", expected 0" << std::endl;
        localFails++;
    }

    if (localFails == 0) {
        std::cout << "PASS  " << name << std::endl;
    }

    return localFails;
}

int main() {
    int totalFails = 0;

    // Test 1: Zero value should yield [0, 0, 0]
    // Verifies handling of the lower bound and null termination.
    totalFails += runTestCase("ZeroValue",
                              0x0000,
                              0x00, // expected high byte
                              0x00); // expected low byte

    // Test 2: Non-zero high and low bytes to ensure correct big-endian mapping
    // Verifies that n >> 8 is placed in str[0] and n & 0xFF in str[1].
    totalFails += runTestCase("HighByteFirst",
                              0x0102,
                              0x01, // expected high byte
                              0x02); // expected low byte

    // Test 3: Maximum 16-bit value to test upper boundary handling
    totalFails += runTestCase("MaxValue",
                              0xFFFF,
                              0xFF, // high byte
                              0xFF); // low byte

    // Test 4: Value where only the low byte is non-zero
    totalFails += runTestCase("LowByteOnly",
                              0x00FF,
                              0x00, // high byte
                              0xFF); // low byte

    // Test 5: Mid-range value to confirm typical behavior
    totalFails += runTestCase("MidValue",
                              0x1234,
                              0x12, // high byte
                              0x34); // low byte

    if (totalFails == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << totalFails << " test(s) FAILED" << std::endl;
        return totalFails;
    }
}
// Lightweight C++11 unit tests for Unroll4WordsReverse in cmspack.c
// This test suite is designed to compile with the project's existing CMS types
// and without using Google Test. It uses a small, in-file test harness approach.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Include CMS internal types to ensure correct type compatibility with the focal function.
// If your environment provides a different path, adjust accordingly.

// Ensure C linkage for the focal function when called from C++
extern "C" cmsUInt8Number* Unroll4WordsReverse(CMSREGISTER _cmsTRANSFORM* info,
                                              CMSREGISTER cmsUInt16Number wIn[],
                                              CMSREGISTER cmsUInt8Number* accum,
                                              CMSREGISTER cmsUInt32Number Stride);

// Simple test harness helpers
static void fail(const char* testName, const char* msg) {
    std::cerr << "[FAIL] " << testName << ": " << msg << std::endl;
}

static void pass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

// Test 1: Palindromic 16-bit words (0x0101) so that endianness-independent reversal yields the same values.
// Verifies that wIn[0..3] all become 0x0101 and that accum is advanced by 8 bytes.
bool test_Unroll4WordsReverse_palindromic() {
    const char* testName = "test_Unroll4WordsReverse_palindromic";

    // Prepare an 16-byte buffer; only first 8 bytes are used by the function.
    unsigned char buffer[16];
    std::memset(buffer, 0, sizeof(buffer));

    // Fill the first 8 bytes with four 16-bit values 0x0101 on a little-endian layout:
    // memory bytes: 01 01 01 01 01 01 01 01
    for (int i = 0; i < 4; ++i) {
        buffer[2*i]     = 0x01;
        buffer[2*i + 1] = 0x01;
    }

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    // Call the focal function. Pass nullptr for info since it's unused.
    cmsUInt8Number* ret = Unroll4WordsReverse(nullptr, wIn, accum, Stride);

    // Expected: each wIn[i] == 0x0101
    const cmsUInt16Number expected = 0x0101;
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (wIn[i] != expected) {
            fail(testName, "wIn elements do not match expected palindromic reversal (0x0101).");
            ok = false;
            break;
        }
    }

    // Expected return: accum advanced by 8 bytes
    if (ret != buffer + 8) {
        fail(testName, "Returned accum pointer did not advance by 8 bytes as expected.");
        ok = false;
    }

    if (ok) pass(testName);
    return ok;
}

// Test 2: Distinct words with known reversal assuming a byte-swap (REVERSE_FLAVOR_16) behavior.
// We craft memory so that loaded words are 0x1122, 0x3344, 0x5566, 0x7788 in little-endian.
// Expected wIn after reversal: 0x2211, 0x4433, 0x6655, 0x8877 on little-endian host.
bool test_Unroll4WordsReverse_distinctWords() {
    const char* testName = "test_Unroll4WordsReverse_distinctWords";

    unsigned char buffer[16];
    std::memset(buffer, 0, sizeof(buffer));

    // Memory layout for 0x1122, 0x3344, 0x5566, 0x7788 in little-endian:
    // 0x1122 -> 0x22 0x11
    // 0x3344 -> 0x44 0x33
    // 0x5566 -> 0x66 0x55
    // 0x7788 -> 0x88 0x77
    buffer[0] = 0x22; buffer[1] = 0x11;
    buffer[2] = 0x44; buffer[3] = 0x33;
    buffer[4] = 0x66; buffer[5] = 0x55;
    buffer[6] = 0x88; buffer[7] = 0x77;

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll4WordsReverse(nullptr, wIn, accum, Stride);

    const cmsUInt16Number expected0 = 0x2211;
    const cmsUInt16Number expected1 = 0x4433;
    const cmsUInt16Number expected2 = 0x6655;
    const cmsUInt16Number expected3 = 0x8877;

    bool ok = true;
    if (wIn[0] != expected0) {
        fail(testName, "wIn[0] does not match expected 0x2211 after reversal.");
        ok = false;
    }
    if (wIn[1] != expected1) {
        fail(testName, "wIn[1] does not match expected 0x4433 after reversal.");
        ok = false;
    }
    if (wIn[2] != expected2) {
        fail(testName, "wIn[2] does not match expected 0x6655 after reversal.");
        ok = false;
    }
    if (wIn[3] != expected3) {
        fail(testName, "wIn[3] does not match expected 0x8877 after reversal.");
        ok = false;
    }

    if (ret != buffer + 8) {
        fail(testName, "Returned accum pointer did not advance by 8 bytes as expected.");
        ok = false;
    }

    if (ok) pass(testName);
    return ok;
}

// Main: drive tests from a single main() function as required when not using gtest.
int main() {
    bool all_passed = true;

    all_passed &= test_Unroll4WordsReverse_palindromic();
    all_passed &= test_Unroll4WordsReverse_distinctWords();

    if (all_passed) {
        std::cout << "[RESULT] All Unroll4WordsReverse tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[RESULT] Some Unroll4WordsReverse tests failed." << std::endl;
        return 1;
    }
}
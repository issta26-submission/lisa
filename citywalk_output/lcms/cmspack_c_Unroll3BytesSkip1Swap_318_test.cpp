// Test suite for the focal method Unroll3BytesSkip1Swap
// This file provides a small, self-contained test harness (C++11) that exercises
// the external C function Unroll3BytesSkip1Swap from cmspack.c (Little CMS-like API).
// The test is designed to compile in a C++11 environment without Google Test.
// It uses non-terminating assertions (logging failures and continuing execution)
// to maximize code coverage as requested in the domain guidelines.

// Domain knowledge applied:
// - We treat Unroll3BytesSkip1Swap as reading 4 bytes from the incoming buffer (after an initial skip of 1 byte),
//   converting the 8-bit values to 16-bit using macro FROM_8_TO_16, and storing into wIn[2], wIn[1], wIn[0] in that order.
// - We verify the accumulator is advanced by 4 bytes (the A, B, G, R sequence) and that Stride is ignored by the function (unused).
// - We verify behavior for a few representative inputs (including all-0xFF case) to cover edge values.
// - We do not mock private/internal dependencies; we only rely on the external symbol and basic types.

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <sstream>


// Forward-declare types to match the focal API, so this test can be compiled in isolation
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Forward declaration of the internal _cmsTRANSFORM type used by the function signature
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// The CMSREGISTER macro used in the focal code (defined as empty for the test)
#define CMSREGISTER

// The function under test is provided by cmspack.c (external C symbol)
extern "C" cmsUInt8Number* Unroll3BytesSkip1Swap(CMSREGISTER _cmsTRANSFORM* info,
                                                CMSREGISTER cmsUInt16Number wIn[],
                                                CMSREGISTER cmsUInt8Number* accum,
                                                CMSREGISTER cmsUInt32Number Stride);

// Lightweight testing utilities (non-terminating assertions)
class TestLogger {
public:
    void logFailure(const std::string& msg) {
        failures_.push_back(msg);
        std::cerr << "TEST FAILURE: " << msg << std::endl;
    }
    bool ok() const { return failures_.empty(); }
    void summary() const {
        std::cout << "\nTest summary: " << (failures_.empty() ? "ALL tests PASSED" : "SOME tests FAILED")
                  << " (" << failures_.size() << " failure(s))\n";
    }
private:
    std::vector<std::string> failures_;
};

static std::string to_string_u16(cmsUInt16Number v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}
static std::string to_hex_u16(cmsUInt16Number v) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << (static_cast<unsigned int>(v) & 0xFFFF);
    return oss.str();
}
static std::string to_string_u32(cmsUInt32Number v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

// Helper to compute FROM_8_TO_16(x) as used by the actual implementation
// In Little CMS style, FROM_8_TO_16(x) is typically (x * 0x0101) which yields 0xABAB for input 0xAB.
static cmsUInt16Number FROM_8_TO_16(unsigned int x) {
    return static_cast<cmsUInt16Number>(x * 0x0101u);
}

// Test 1: Basic path with known bytes, verifying wIn contents and accumulator advancement
void test_Unroll3BytesSkip1Swap_basic(TestLogger& log) {
    // Domain: A B G R bytes are [0x11, 0x22, 0x33, 0x44]
    unsigned char inputBytes[4] = {0x11, 0x22, 0x33, 0x44};
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char* accum = inputBytes;
    cmsUInt32Number Stride = 0;
    cmsTRANSFORM dummyInfo; // not used by the test, just to satisfy signature
    cmsUInt8Number* ret = Unroll3BytesSkip1Swap(reinterpret_cast<_cmsTRANSFORM*>(&dummyInfo),
                                                wIn,
                                                accum,
                                                Stride);
    // Expected mapping based on the function logic:
    // wIn[2] <- FROM_8_TO_16(inputBytes[1])  => 0x22 -> 0x2222
    // wIn[1] <- FROM_8_TO_16(inputBytes[2])  => 0x33 -> 0x3333
    // wIn[0] <- FROM_8_TO_16(inputBytes[3])  => 0x44 -> 0x4444
    cmsUInt16Number expectedW2 = FROM_8_TO_16(inputBytes[1]); // 0x2222
    cmsUInt16Number expectedW1 = FROM_8_TO_16(inputBytes[2]); // 0x3333
    cmsUInt16Number expectedW0 = FROM_8_TO_16(inputBytes[3]); // 0x4444

    if (wIn[2] != expectedW2) {
        log.logFailure("Test1: wIn[2] mismatch. Expected " + to_string_u16(expectedW2) +
                       ", got " + to_string_u16(wIn[2]));
    }
    if (wIn[1] != expectedW1) {
        log.logFailure("Test1: wIn[1] mismatch. Expected " + to_string_u16(expectedW1) +
                       ", got " + to_string_u16(wIn[1]));
    }
    if (wIn[0] != expectedW0) {
        log.logFailure("Test1: wIn[0] mismatch. Expected " + to_string_u16(expectedW0) +
                       ", got " + to_string_u16(wIn[0]));
    }

    unsigned char* expectedRet = inputBytes + 4;
    if (ret != expectedRet) {
        log.logFailure("Test1: accumulator return pointer mismatch. Expected " +
                       std::to_string(reinterpret_cast<uintptr_t>(expectedRet)) +
                       ", got " + std::to_string(reinterpret_cast<uintptr_t>(ret)));
    }
}

// Test 2: Alternate input pattern to ensure general correctness
void test_Unroll3BytesSkip1Swap_alternatePattern(TestLogger& log) {
    // Domain: A B G R bytes are [0xAA, 0x01, 0x02, 0x03]
    unsigned char inputBytes[4] = {0xAA, 0x01, 0x02, 0x03};
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char* accum = inputBytes;
    cmsUInt32Number Stride = 7; // non-zero to ensure Stride is unused
    cmsTRANSFORM dummyInfo;
    cmsUInt8Number* ret = Unroll3BytesSkip1Swap(reinterpret_cast<_cmsTRANSFORM*>(&dummyInfo),
                                                wIn,
                                                accum,
                                                Stride);
    cmsUInt16Number expectedW2 = FROM_8_TO_16(inputBytes[1]); // 0x0101
    cmsUInt16Number expectedW1 = FROM_8_TO_16(inputBytes[2]); // 0x0202
    cmsUInt16Number expectedW0 = FROM_8_TO_16(inputBytes[3]); // 0x0303

    if (wIn[2] != expectedW2) {
        log.logFailure("Test2: wIn[2] mismatch. Expected " + to_string_u16(expectedW2) +
                       ", got " + to_string_u16(wIn[2]));
    }
    if (wIn[1] != expectedW1) {
        log.logFailure("Test2: wIn[1] mismatch. Expected " + to_string_u16(expectedW1) +
                       ", got " + to_string_u16(wIn[1]));
    }
    if (wIn[0] != expectedW0) {
        log.logFailure("Test2: wIn[0] mismatch. Expected " + to_string_u16(expectedW0) +
                       ", got " + to_string_u16(wIn[0]));
    }

    unsigned char* expectedRet = inputBytes + 4;
    if (ret != expectedRet) {
        log.logFailure("Test2: accumulator return pointer mismatch. Expected " +
                       std::to_string(reinterpret_cast<uintptr_t>(expectedRet)) +
                       ", got " + std::to_string(reinterpret_cast<uintptr_t>(ret)));
    }
}

// Test 3: All 0xFF values to ensure max 16-bit results are produced
void test_Unroll3BytesSkip1Swap_allFF(TestLogger& log) {
    unsigned char inputBytes[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char* accum = inputBytes;
    cmsUInt32Number Stride = 0;
    cmsTRANSFORM dummyInfo;
    cmsUInt8Number* ret = Unroll3BytesSkip1Swap(reinterpret_cast<_cmsTRANSFORM*>(&dummyInfo),
                                                wIn,
                                                accum,
                                                Stride);
    cmsUInt16Number expectedW2 = FROM_8_TO_16(inputBytes[1]); // 0xFFFF
    cmsUInt16Number expectedW1 = FROM_8_TO_16(inputBytes[2]); // 0xFFFF
    cmsUInt16Number expectedW0 = FROM_8_TO_16(inputBytes[3]); // 0xFFFF

    if (wIn[2] != expectedW2) {
        log.logFailure("Test3: wIn[2] mismatch. Expected " + to_string_u16(expectedW2) +
                       ", got " + to_string_u16(wIn[2]));
    }
    if (wIn[1] != expectedW1) {
        log.logFailure("Test3: wIn[1] mismatch. Expected " + to_string_u16(expectedW1) +
                       ", got " + to_string_u16(wIn[1]));
    }
    if (wIn[0] != expectedW0) {
        log.logFailure("Test3: wIn[0] mismatch. Expected " + to_string_u16(expectedW0) +
                       ", got " + to_string_u16(wIn[0]));
    }

    unsigned char* expectedRet = inputBytes + 4;
    if (ret != expectedRet) {
        log.logFailure("Test3: accumulator return pointer mismatch. Expected " +
                       std::to_string(reinterpret_cast<uintptr_t>(expectedRet)) +
                       ", got " + std::to_string(reinterpret_cast<uintptr_t>(ret)));
    }
}

// Entry point for the test suite
int main() {
    // Step 2: Unit Test Generation (create a focused suite for Unroll3BytesSkip1Swap)
    // Step 3: Test Case Refinement (enforce various input patterns and verify accumulator advancement)

    TestLogger log;

    // Run tests
    test_Unroll3BytesSkip1Swap_basic(log);
    test_Unroll3BytesSkip1Swap_alternatePattern(log);
    test_Unroll3BytesSkip1Swap_allFF(log);

    // Summary
    log.summary();

    // Return non-zero if any test failed to aid integration in CI systems that rely on exit codes.
    if (!log.ok()) {
        return 1;
    }
    return 0;
}
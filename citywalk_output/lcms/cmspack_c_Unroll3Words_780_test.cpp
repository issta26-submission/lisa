// Test suite for the focal method Unroll3Words in cmsPack (cmspack.c)
// This test is written for a C/C++11 environment and does not rely on GoogleTest.
// It uses a tiny in-house test harness that continues execution even after a failure
// to maximize coverage, as requested by the domain knowledge.
//
// Notes on dependencies and keywords (from Step 1):
// - Focal method: Unroll3Words reads 3 consecutive 16-bit values from a byte stream.
// - Core inputs: info (transform context), wIn (destination 16-bit words), accum (byte stream),
//   Stride (not used by the focal function, but part of the signature).
// - The function relies on memory layout (little-endian two-byte reads) and advances accum by 6 bytes.
// - The class dependencies in this test harness rely on forward-declaration of _cmsTRANSFORM
//   to avoid requiring the full cms context. We mirror the essential types locally for linking.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain knowledge: define minimal types and forward-declare the focal function
// to allow linking with the actual C implementation in the project.

#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Minimal type aliases to match the focal function's signature (as used in the provided code)
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Forward-declare the opaque CMS transform type as in the real code
struct _cmsTRANSFORM;
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// Forward-declare the focal function with C linkage to match the C implementation
extern "C" cmsUInt8Number* Unroll3Words(CMSREGISTER cmsTRANSFORM* info,
                                      CMSREGISTER cmsUInt16Number wIn[],
                                      CMSREGISTER cmsUInt8Number* accum,
                                      CMSREGISTER cmsUInt32Number Stride);

// Simple, non-terminating assertion macro for test harness
#define TEST_ASSERT(cond, message)                                      \
    do {                                                                 \
        if(!(cond)) {                                                    \
            std::cerr << "Test assertion failed: " << (message)          \
                      << " in function " << __FUNCTION__                 \
                      << " at line " << __LINE__ << std::endl;         \
            return false;                                              \
        }                                                                \
    } while(false)


// Test 1: Basic Unroll3Words reads exactly 3 16-bit words from a 6-byte buffer
// and advances the accum pointer by 6 bytes. This checks endianness-consistent
// interpretation of the two-byte chunks on the host system.
bool test_Unroll3Words_basic() {
    // Domain knowledge applicability:
    // Expect wIn[0] = buffer[0] | (buffer[1] << 8)
    //        wIn[1] = buffer[2] | (buffer[3] << 8)
    //        wIn[2] = buffer[4] | (buffer[5] << 8)
    unsigned char buffer[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }; // little-endian encoding
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll3Words(info, wIn, accum, Stride);

    cmsUInt16Number expected0 = (cmsUInt16Number)buffer[0] | ((cmsUInt16Number)buffer[1] << 8);
    cmsUInt16Number expected1 = (cmsUInt16Number)buffer[2] | ((cmsUInt16Number)buffer[3] << 8);
    cmsUInt16Number expected2 = (cmsUInt16Number)buffer[4] | ((cmsUInt16Number)buffer[5] << 8);
    // Validate outputs
    TEST_ASSERT(wIn[0] == expected0, "Unroll3Words: wIn[0] mismatch");
    TEST_ASSERT(wIn[1] == expected1, "Unroll3Words: wIn[1] mismatch");
    TEST_ASSERT(wIn[2] == expected2, "Unroll3Words: wIn[2] mismatch");

    // The function should have advanced accum by 6 bytes
    TEST_ASSERT(ret == buffer + 6, "Unroll3Words: returned pointer did not advance by 6 bytes");

    return true;
}

// Test 2: Run with another set of values to ensure deterministic behavior
bool test_Unroll3Words_different_values() {
    unsigned char buffer[6] = { 0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22 };
    cmsUInt16Number wIn[3] = {0};
    cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll3Words(info, wIn, accum, Stride);

    cmsUInt16Number expected0 = (cmsUInt16Number)buffer[0] | ((cmsUInt16Number)buffer[1] << 8); // 0xBBAA
    cmsUInt16Number expected1 = (cmsUInt16Number)buffer[2] | ((cmsUInt16Number)buffer[3] << 8); // 0xDDCC
    cmsUInt16Number expected2 = (cmsUInt16Number)buffer[4] | ((cmsUInt16Number)buffer[5] << 8); // 0x2211

    TEST_ASSERT(wIn[0] == expected0, "Unroll3Words (2): wIn[0] mismatch");
    TEST_ASSERT(wIn[1] == expected1, "Unroll3Words (2): wIn[1] mismatch");
    TEST_ASSERT(wIn[2] == expected2, "Unroll3Words (2): wIn[2] mismatch");
    TEST_ASSERT(ret == buffer + 6, "Unroll3Words (2): returned pointer did not advance by 6 bytes");

    return true;
}

// Test 3: Multiple sequential calls to ensure no state leakage between calls
bool test_Unroll3Words_multiple_calls() {
    unsigned char buffer1[6] = { 0x10, 0x01, 0x20, 0x02, 0x30, 0x03 }; // values: 0x0110, 0x0220, 0x0330
    unsigned char buffer2[6] = { 0xFF, 0x00, 0x80, 0x01, 0x00, 0x02 }; // values: 0x00FF, 0x0180, 0x0200
    cmsUInt16Number wIn1[3] = {0};
    cmsUInt16Number wIn2[3] = {0};
    cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum1 = buffer1;
    cmsUInt8Number* accum2 = buffer2;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret1 = Unroll3Words(info, wIn1, accum1, Stride);
    cmsUInt8Number* ret2 = Unroll3Words(info, wIn2, accum2, Stride);

    cmsUInt16Number expected0_1 = (cmsUInt16Number)buffer1[0] | ((cmsUInt16Number)buffer1[1] << 8);
    cmsUInt16Number expected1_1 = (cmsUInt16Number)buffer1[2] | ((cmsUInt16Number)buffer1[3] << 8);
    cmsUInt16Number expected2_1 = (cmsUInt16Number)buffer1[4] | ((cmsUInt16Number)buffer1[5] << 8);

    cmsUInt16Number expected0_2 = (cmsUInt16Number)buffer2[0] | ((cmsUInt16Number)buffer2[1] << 8);
    cmsUInt16Number expected1_2 = (cmsUInt16Number)buffer2[2] | ((cmsUInt16Number)buffer2[3] << 8);
    cmsUInt16Number expected2_2 = (cmsUInt16Number)buffer2[4] | ((cmsUInt16Number)buffer2[5] << 8);

    TEST_ASSERT(wIn1[0] == expected0_1, "Unroll3Words (3): wIn1[0] mismatch");
    TEST_ASSERT(wIn1[1] == expected1_1, "Unroll3Words (3): wIn1[1] mismatch");
    TEST_ASSERT(wIn1[2] == expected2_1, "Unroll3Words (3): wIn1[2] mismatch");
    TEST_ASSERT(ret1 == buffer1 + 6, "Unroll3Words (3): ret1 not advanced correctly");

    TEST_ASSERT(wIn2[0] == expected0_2, "Unroll3Words (3): wIn2[0] mismatch");
    TEST_ASSERT(wIn2[1] == expected1_2, "Unroll3Words (3): wIn2[1] mismatch");
    TEST_ASSERT(wIn2[2] == expected2_2, "Unroll3Words (3): wIn2[2] mismatch");
    TEST_ASSERT(ret2 == buffer2 + 6, "Unroll3Words (3): ret2 not advanced correctly");

    return true;
}


// Main: run all tests and report summary
int main() {
    int passed = 0;
    int total  = 0;

    std::cout << "Starting tests for Unroll3Words...\n";

    if(test_Unroll3Words_basic()) {
        std::cout << "[PASS] test_Unroll3Words_basic\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_Unroll3Words_basic\n";
    }
    ++total;

    if(test_Unroll3Words_different_values()) {
        std::cout << "[PASS] test_Unroll3Words_different_values\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_Unroll3Words_different_values\n";
    }
    ++total;

    if(test_Unroll3Words_multiple_calls()) {
        std::cout << "[PASS] test_Unroll3Words_multiple_calls\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_Unroll3Words_multiple_calls\n";
    }
    ++total;

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed to help CI systems detect failures
    return (passed == total) ? 0 : 1;
}
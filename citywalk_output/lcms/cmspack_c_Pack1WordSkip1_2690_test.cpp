/*
Step 1 - Program Understanding (Candidate Keywords)
Key dependent components observed in the focal method Pack1WordSkip1:
- Input: wOut (array of cmsUInt16Number), output (cmsUInt8Number*), info (CMSTRANSFORM context), Stride (cmsUInt32Number)
- Behavior: writes the first word wOut[0] into the start of the output buffer by casting the buffer to cmsUInt16Number*,
            advances the output pointer by 4 bytes, and returns the advanced pointer.
- No branching logic; relies on endianness for memory layout when writing a 16-bit value.
- Dependency hints: cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, _cmsTRANSFORM type, CMSREGISTER macro.
- Static/private helpers in the focal class file are not relevant to this unit test since we only test Pack1WordSkip1.

Step 2 - Unit Test Generation (target: Pack1WordSkip1)
Notes:
- We will create a lightweight test harness in C++11 (no GTest) that exercises:
  1) Basic single-call correctness (writes correct 16-bit value and returns pointer offset by 4).
  2) Endianness-agnostic data verification by reading back via a 16-bit lvalue to compare the original word.
  3) Pointer advancement across multiple calls (sequential writes at successive 4-byte boundaries).
  4) Stride parameter is ignored by the function (verify that non-zero Stride does not affect behavior).
  5) Padding/neighbor bytes are preserved (ensuring no unintended writes beyond the 2-byte write).
- Static members and file-scope functions are not mocked; tests call the function directly.
- We declare the required types and the function prototype to link with the actual cmspack.c implementation.

Step 3 - Test Case Refinement
- All tests are designed to be non-terminating in the sense that they do not abort on first failure; instead, each test returns a bool and prints a descriptive message on failure.
- We ensure alignment safety by using a buffer large enough and relying on the host’s alignment (typical for test platforms).
- We keep tests minimal and focused on the exact behavior of Pack1WordSkip1.

Code: (C++11 test harness for Pack1WordSkip1)

*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge hints applied here:
// - Use only standard library
// - Use non-terminating assertions (returning booleans and printing)
// - Access static-like behavior via class names if needed (not applicable here)

// Step 2: Declare the exact prototype for the focal method to be linked with cmspack.c
// We replicate the minimal types and macros used in the focal file.
// We assume the actual implementation is compiled with C linkage; hence we use extern "C".

// Lightweight type aliases matching the focal CMS types
using cmsUInt8Number  = uint8_t;
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;

// Forward-declare the internal type used by the function
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// The CMSREGISTER macro used in the signature; in test, it can be empty
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

extern "C" cmsUInt8Number* Pack1WordSkip1(CMSREGISTER _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride);

// Helper: simple non-terminating assertion reporting
static void reportFailure(const std::string& testName, const std::string& message) {
    std::cerr << "FAIL [" << testName << "] - " << message << std::endl;
}

// Test 1: Basic single-call correctness
// - Verifies the 16-bit value is written at the start of output
// - Verifies the returned pointer is exactly 4 bytes ahead
// - Verifies padding bytes (offsets 2-3) are not overwritten
static bool test_Pack1WordSkip1_BasicWrites() {
    const char* testName = "Pack1WordSkip1_BasicWrites";
    cmsUInt16Number wOut0 = 0x1234; // sample value
    unsigned char buf[16];
    // Initialize padding bytes to known values to detect unintended writes
    buf[2] = 0x55;
    buf[3] = 0x66;
    // Fill the rest with a recognizable pattern
    std::memset(buf, 0xAA, 16);

    // Call the function
    cmsUInt8Number* ret = Pack1WordSkip1(nullptr, &wOut0, buf, 0);

    // Check 1: first two bytes should encode wOut0 in host endianness
    cmsUInt16Number writtenWord = *reinterpret_cast<cmsUInt16Number*>(buf);
    if (writtenWord != wOut0) {
        reportFailure(testName, "Buffer first 2 bytes do not equal wOut[0] (endianness issue or write failure).");
        return false;
    }

    // Check 2: return pointer offset should be +4
    if (ret != buf + 4) {
        reportFailure(testName, "Return pointer is not advanced by 4 bytes as expected.");
        return false;
    }

    // Check 3: padding bytes at offsets 2 and 3 should remain unchanged
    if (buf[2] != 0x55 || buf[3] != 0x66) {
        reportFailure(testName, "Padding bytes were overwritten unexpectedly.");
        return false;
    }

    // Passed all checks
    std::cout << "PASS [" << testName << "]" << std::endl;
    return true;
}

// Test 2: Sequential writes (multiple calls) verify correct offsets and values
static bool test_Pack1WordSkip1_MultipleWrites() {
    const char* testName = "Pack1WordSkip1_MultipleWrites";
    cmsUInt16Number w0 = 0xABCD;
    cmsUInt16Number w1 = 0x1234;
    unsigned char buf[16];
    std::memset(buf, 0x00, 16);

    // First write
    cmsUInt8Number* p0 = Pack1WordSkip1(nullptr, &w0, buf, 0);
    // Validate first 2 bytes equal w0 (with endianness independence via 16-bit read)
    cmsUInt16Number first = *reinterpret_cast<cmsUInt16Number*>(buf);
    if (first != w0) {
        reportFailure(testName, "First write did not store w0 correctly.");
        return false;
    }
    if (p0 != buf + 4) {
        reportFailure(testName, "First return pointer not buf+4 after first write.");
        return false;
    }

    // Second write at offset 4
    cmsUInt8Number* p1 = Pack1WordSkip1(nullptr, &w1, p0, 0);
    cmsUInt16Number second = *reinterpret_cast<cmsUInt16Number*>(p0); // read from offset 4
    if (second != w1) {
        reportFailure(testName, "Second write did not store w1 correctly at offset 4.");
        return false;
    }
    if (p1 != buf + 8) {
        reportFailure(testName, "Second return pointer not buf+8 after second write.");
        return false;
    }

    std::cout << "PASS [" << testName << "]" << std::endl;
    return true;
}

// Test 3: Stride parameter is ignored by the function
static bool test_Pack1WordSkip1_StrideIgnored() {
    const char* testName = "Pack1WordSkip1_StrideIgnored";
    cmsUInt16Number wOut = 0x0F0F;
    unsigned char buf[16];
    std::memset(buf, 0x00, 16);

    cmsUInt8Number* ret = Pack1WordSkip1(nullptr, &wOut, buf, 8); // non-zero Stride
    cmsUInt16Number written = *reinterpret_cast<cmsUInt16Number*>(buf);
    if (written != wOut) {
        reportFailure(testName, "Write with non-zero Stride did not store correct value.");
        return false;
    }
    if (ret != buf + 4) {
        reportFailure(testName, "Return pointer not buf+4 when Stride is non-zero.");
        return false;
    }

    std::cout << "PASS [" << testName << "]" << std::endl;
    return true;
}

// Test 4: Padding/neighbor bytes preservation when writing multiple times
static bool test_Pack1WordSkip1_PaddingPreserved() {
    const char* testName = "Pack1WordSkip1_PaddingPreserved";
    cmsUInt16Number w0 = 0x1A2B;
    unsigned char buf[8];
    // set neighbor bytes to known values
    buf[2] = 0x77;
    buf[3] = 0x88;
    // Ensure the whole buffer isn't zeroed to detect overwrites
    std::memset(buf, 0x9C, 8);

    cmsUInt8Number* ret = Pack1WordSkip1(nullptr, &w0, buf, 0);

    cmsUInt16Number first = *reinterpret_cast<cmsUInt16Number*>(buf);
    if (first != w0) {
        reportFailure(testName, "First write did not store w0 correctly for padding test.");
        return false;
    }
    // Verify neighbor bytes unchanged (offsets 2 and 3)
    if (buf[2] != 0x9C || buf[3] != 0x9C) {
        reportFailure(testName, "Neighbor padding bytes were overwritten during Pack1WordSkip1.");
        return false;
    }
    if (ret != buf + 4) {
        reportFailure(testName, "Return pointer not buf+4 in padding test.");
        return false;
    }

    std::cout << "PASS [" << testName << "]" << std::endl;
    return true;
}

// Main: Run all tests
int main() {
    bool all_passed = true;

    all_passed &= test_Pack1WordSkip1_BasicWrites();
    all_passed &= test_Pack1WordSkip1_MultipleWrites();
    all_passed &= test_Pack1WordSkip1_StrideIgnored();
    all_passed &= test_Pack1WordSkip1_PaddingPreserved();

    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
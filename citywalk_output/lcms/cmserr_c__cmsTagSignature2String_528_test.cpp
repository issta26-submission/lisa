// Lightweight C++11 test harness for the focal function:
// _cmsTagSignature2String from cmserr.c
// This harness does not rely on GTest. It provides a minimal test runner
// that can be built together with the library sources (cmserr.c and related
// files) and linked into a single executable.

// The tests verify that _cmsTagSignature2String writes the 4-byte big-endian
// representation of the input signature into the String buffer, followed by a
// terminating NUL, as expected by the original API.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Type aliases matching the likely definitions in the focal codebase.
// We keep them simple and portable for testing purposes.
typedef uint32_t cmsUInt32Number;
typedef uint32_t cmsTagSignature;

// Declarations of the focal function and its helper (to be linked from the
// cmserr.c / upstream implementation).
extern "C" void _cmsTagSignature2String(char String[5], cmsTagSignature sig);
extern "C" cmsUInt32Number _cmsAdjustEndianess32(cmsUInt32Number value);

// Helper: determine if the host is little-endian at runtime.
static bool isLittleEndian() {
    uint16_t x = 0x1;
    return *reinterpret_cast<char*>(&x) == 0x1;
}

// Helper: pretty-print a 4-byte array in hex for debugging.
static void printBytes(const unsigned char* b, int len) {
    for (int i = 0; i < len; ++i) {
        std::cout << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(b[i]) << std::dec << " ";
    }
    std::cout << std::dec;
}

// Run a single test case for a given signature.
// It computes the expected 4 bytes by simulating the scalar endianness.
// Returns true if test passes, false otherwise (and prints details on failure).
static bool runTest(cmsTagSignature sig, int testIndex) {
    char out[5] = {0};
    _cmsTagSignature2String(out, sig); // Focal function under test

    cmsUInt32Number be = _cmsAdjustEndianess32((cmsUInt32Number)sig);

    // Compute expected bytes based on host endianness and the swapped value `be`
    // as written by _cmsTagSignature2String.
    unsigned char expected[4];
    if (isLittleEndian()) {
        // On little-endian, memory order is LSB first.
        expected[0] = static_cast<unsigned char>(be & 0xFF);
        expected[1] = static_cast<unsigned char>((be >> 8) & 0xFF);
        expected[2] = static_cast<unsigned char>((be >> 16) & 0xFF);
        expected[3] = static_cast<unsigned char>((be >> 24) & 0xFF);
    } else {
        // On big-endian, memory order is MSB first.
        expected[0] = static_cast<unsigned char>((be >> 24) & 0xFF);
        expected[1] = static_cast<unsigned char>((be >> 16) & 0xFF);
        expected[2] = static_cast<unsigned char>((be >> 8) & 0xFF);
        expected[3] = static_cast<unsigned char>(be & 0xFF);
    }

    bool okTerm = (out[4] == 0);
    bool okBytes = std::memcmp(out, expected, 4) == 0;

    if (!okTerm || !okBytes) {
        std::cout << "Test " << testIndex << " FAILED for sig 0x"
                  << std::hex << sig << std::dec << "\n";
        std::cout << "  got bytes: ";
        printBytes(reinterpret_cast<unsigned char*>(out), 4);
        std::cout << " terminator=" << (out[4] == 0 ? '1' : '0') << "\n";
        std::cout << "  expected: ";
        printBytes(expected, 4);
        std::cout << "\n";
        return false;
    } else {
        std::cout << "Test " << testIndex << " PASSED for sig 0x"
                  << std::hex << sig << std::dec << "\n";
        return true;
    }
}

// Main test runner
int main() {
    int passed = 0;
    int total = 0;

    // Test 1: Typical 0x01020304 should produce 01 02 03 04 in String.
    // Rationale: big-endian representation of the signature.
    total++;
    if (runTest(0x01020304, total)) ++passed;

    // Test 2: Pattern 0xAABBCCDD should produce AA BB CC DD.
    total++;
    if (runTest(0xAABBCCDD, total)) ++passed;

    // Test 3: Boundary value 0x00000001 should produce 00 00 00 01.
    total++;
    if (runTest(0x00000001, total)) ++passed;

    // Test 4: All ones 0xFFFFFFFF should produce FF FF FF FF.
    total++;
    if (runTest(0xFFFFFFFF, total)) ++passed;

    // Test 5: ASCII sequence 0x41424344 ('ABCD') should produce 'A' 'B' 'C' 'D'.
    total++;
    if (runTest(0x41424344, total)) ++passed;

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}
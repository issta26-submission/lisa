// Test suite for the Pack3Bytes function located in cmspack.c
// This test uses a minimal extern "C" prototype to call the function
// without requiring the full cms headers in the test translation unit.
// It validates that Pack3Bytes writes exactly three bytes derived from
// the input 16-bit words and returns a pointer advanced by three bytes.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Forward declaration of the focal function (C linkage).
// We use a permissive prototype with primitive types to avoid
// tight coupling to the exact typedefs used in cmspack.c.
// The actual implementation in cmspack.c uses cmsUInt8Number, cmsUInt16Number, etc.
// In practice cmsUInt8Number is typically unsigned char, cmsUInt16Number unsigned short.
extern "C" unsigned char* Pack3Bytes(void* info, unsigned short wOut[], unsigned char* output, unsigned int Stride);

// Helper to print a byte array for debugging (hex).
static void print_bytes(const unsigned char* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::uppercase
                  << (static_cast<int>(data[i]) & 0xFF) << " ";
    }
    std::cout << std::dec << std::endl;
}

// Test Case A: Basic mapping using wOut = {0, 256, 512}.
// Expected: output bytes derived from 0, 1, 2 respectively.
static bool test_Pack3Bytes_caseA() {
    // Setup
    unsigned short wOut[3] = { 0x0000, 0x0100, 0x0200 }; // 0, 256, 512
    unsigned char outBuf[16];
    // Initialize with a known pattern to detect overflows
    std::memset(outBuf, 0xAA, sizeof(outBuf));

    // Call
    unsigned char* ret = Pack3Bytes(nullptr, wOut, outBuf, 7);

    // Expected results
    const unsigned char expected0 = 0; // FROM_16_TO_8(0)
    const unsigned char expected1 = 1; // FROM_16_TO_8(256)
    const unsigned char expected2 = 2; // FROM_16_TO_8(512)

    bool ok = true;
    if (ret != outBuf + 3) {
        std::cerr << "test_Pack3Bytes_caseA: return pointer mismatch (expected outBuf+3)." << std::endl;
        ok = false;
    }
    if (outBuf[0] != expected0 || outBuf[1] != expected1 || outBuf[2] != expected2) {
        std::cerr << "test_Pack3Bytes_caseA: bytes mismatch. Expected ["
                  << int(expected0) << " " << int(expected1) << " " << int(expected2)
                  << "] but got ["
                  << int(outBuf[0]) << " " << int(outBuf[1]) << " " << int(outBuf[2])
                  << "]" << std::endl;
        ok = false;
    }
    // Ensure no writes beyond the first three bytes
    if (outBuf[3] != 0xAA || outBuf[4] != 0xAA || outBuf[5] != 0xAA) {
        std::cerr << "test_Pack3Bytes_caseA: writes beyond the first three bytes detected." << std::endl;
        ok = false;
    }
    return ok;
}

// Test Case B: Mixed values to verify different high-byte results.
// wOut = {255, 256, 511} -> high-bytes: 0, 1, 1 (assuming high-byte mapping or rounding to nearest)
static bool test_Pack3Bytes_caseB() {
    unsigned short wOut[3] = { 0x00FF, 0x0100, 0x01FF }; // 255, 256, 511
    unsigned char outBuf[16];
    std::memset(outBuf, 0xFF, sizeof(outBuf));

    unsigned char* ret = Pack3Bytes(nullptr, wOut, outBuf, 3);

    const unsigned char expected0 = 0;
    const unsigned char expected1 = 1;
    const unsigned char expected2 = 1;

    bool ok = true;
    if (ret != outBuf + 3) {
        std::cerr << "test_Pack3Bytes_caseB: return pointer mismatch (expected outBuf+3)." << std::endl;
        ok = false;
    }
    if (outBuf[0] != expected0 || outBuf[1] != expected1 || outBuf[2] != expected2) {
        std::cerr << "test_Pack3Bytes_caseB: bytes mismatch. Expected ["
                  << int(expected0) << " " << int(expected1) << " " << int(expected2)
                  << "] but got ["
                  << int(outBuf[0]) << " " << int(outBuf[1]) << " " << int(outBuf[2])
                  << "]" << std::endl;
        ok = false;
    }
    // Ensure no writes beyond the first three bytes
    if (outBuf[3] != 0xFF || outBuf[4] != 0xFF || outBuf[5] != 0xFF) {
        std::cerr << "test_Pack3Bytes_caseB: writes beyond the first three bytes detected." << std::endl;
        ok = false;
    }
    return ok;
}

// Test Case C: All zeros input. Verify mapping yields zeros and no out-of-bounds writes.
static bool test_Pack3Bytes_caseC() {
    unsigned short wOut[3] = { 0x0000, 0x0000, 0x0000 }; // all zero
    unsigned char outBuf[16];
    std::memset(outBuf, 0x5A, sizeof(outBuf)); // non-zero pattern to detect writes beyond first three

    unsigned char* ret = Pack3Bytes(nullptr, wOut, outBuf, 0);

    const unsigned char expected0 = 0;
    const unsigned char expected1 = 0;
    const unsigned char expected2 = 0;

    bool ok = true;
    if (ret != outBuf + 3) {
        std::cerr << "test_Pack3Bytes_caseC: return pointer mismatch (expected outBuf+3)." << std::endl;
        ok = false;
    }
    if (outBuf[0] != expected0 || outBuf[1] != expected1 || outBuf[2] != expected2) {
        std::cerr << "test_Pack3Bytes_caseC: bytes mismatch. Expected ["
                  << int(expected0) << " " << int(expected1) << " " << int(expected2)
                  << "] but got ["
                  << int(outBuf[0]) << " " << int(outBuf[1]) << " " << int(outBuf[2])
                  << "]" << std::endl;
        ok = false;
    }
    // Ensure no writes beyond the first three bytes
    if (outBuf[3] != 0x5A || outBuf[4] != 0x5A || outBuf[5] != 0x5A) {
        std::cerr << "test_Pack3Bytes_caseC: writes beyond the first three bytes detected." << std::endl;
        ok = false;
    }
    return ok;
}

int main() {
    bool all_passed = true;

    std::cout << "Running Pack3Bytes test suite...\n";

    if (!test_Pack3Bytes_caseA()) {
        std::cout << "FAILED: test_Pack3Bytes_caseA\n";
        all_passed = false;
    } else {
        std::cout << "PASSED: test_Pack3Bytes_caseA\n";
    }

    if (!test_Pack3Bytes_caseB()) {
        std::cout << "FAILED: test_Pack3Bytes_caseB\n";
        all_passed = false;
    } else {
        std::cout << "PASSED: test_Pack3Bytes_caseB\n";
    }

    if (!test_Pack3Bytes_caseC()) {
        std::cout << "FAILED: test_Pack3Bytes_caseC\n";
        all_passed = false;
    } else {
        std::cout << "PASSED: test_Pack3Bytes_caseC\n";
    }

    if (all_passed) {
        std::cout << "All Pack3Bytes tests passed.\n";
        return 0;
    } else {
        std::cout << "Some Pack3Bytes tests failed.\n";
        return 1;
    }
}
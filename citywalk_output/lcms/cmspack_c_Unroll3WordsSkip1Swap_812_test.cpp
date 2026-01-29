// This file provides a lightweight C++11 test suite for the focal function:
// Unroll3WordsSkip1Swap from the cmspack.c context.
// The tests are self-contained, use only the C++ standard library, and do not rely
// on GoogleTest or any external test framework.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge alignment: declare the minimal API/contracts to call the focal function.
// We mirror the real CMS types with lightweight aliases to enable compilation in a standalone test.

typedef unsigned char cmsUInt8Number;
typedef uint16_t      cmsUInt16Number;
typedef uint32_t      cmsUInt32Number;

// The actual CMS code uses a macro CMSREGISTER, and a forward-declared _cmsTRANSFORM struct.
// We mimic these in the test to allow linking against the real implementation when available.

#define CMSREGISTER

struct _cmsTRANSFORM {}; // dummy placeholder to match the API

// Prototype of the focal function under test (as it would appear in a C source file).
extern "C" cmsUInt8Number* Unroll3WordsSkip1Swap(CMSREGISTER _cmsTRANSFORM* info,
                                      CMSREGISTER cmsUInt16Number wIn[],
                                      CMSREGISTER cmsUInt8Number* accum,
                                      CMSREGISTER cmsUInt32Number Stride);

// Helper: write a 16-bit value to a buffer in little-endian order.
inline void write16_le(unsigned char* p, uint16_t v) {
    p[0] = static_cast<unsigned char>(v & 0xFF);
    p[1] = static_cast<unsigned char>((v >> 8) & 0xFF);
}

// Lightweight test framework (non-terminating assertions):
// Each test reports detailed diagnostic messages on failure but continues execution to maximize coverage.

static bool test_Unroll3WordsSkip1Swap_Basic() {
    // Purpose:
    // Validate that Unroll3WordsSkip1Swap reads three 16-bit values from
    // buffer[2..7] (A is ignored) and stores them into wIn as:
    // wIn[2] = R, wIn[1] = G, wIn[0] = B
    // and returns accum pointing to buffer + 8.

    unsigned char buffer[16] = {0};
    // Prepare memory: A at [0..1] (ignored), then R,G,B as 16-bit little-endian:
    // R = 0x1122 -> bytes at [2],[3] = 0x22,0x11
    // G = 0x3344 -> bytes at [4],[5] = 0x44,0x33
    // B = 0x7788 -> bytes at [6],[7] = 0x88,0x77
    write16_le(buffer + 2, 0x1122);
    write16_le(buffer + 4, 0x3344);
    write16_le(buffer + 6, 0x7788);

    cmsUInt16Number wIn[3] = {0, 0, 0};

    cmsUInt8Number* ret = Unroll3WordsSkip1Swap(nullptr, wIn, buffer, 0);

    bool ok = true;

    // Check return pointer points exactly to buffer + 8
    if (ret != buffer + 8) {
        std::printf("Unroll3WordsSkip1Swap_Basic: return pointer mismatch. Expected %p, got %p\n",
                    (void*)(buffer + 8), (void*)ret);
        ok = false;
    }

    // Check the written values match the expected R,G,B extraction
    if (wIn[0] != 0x7788) {
        std::printf("Unroll3WordsSkip1Swap_Basic: wIn[0] mismatch. Expected 0x7788, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[0]));
        ok = false;
    }
    if (wIn[1] != 0x3344) {
        std::printf("Unroll3WordsSkip1Swap_Basic: wIn[1] mismatch. Expected 0x3344, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[1]));
        ok = false;
    }
    if (wIn[2] != 0x1122) {
        std::printf("Unroll3WordsSkip1Swap_Basic: wIn[2] mismatch. Expected 0x1122, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[2]));
        ok = false;
    }

    return ok;
}

static bool test_Unroll3WordsSkip1Swap_MemoryBound() {
    // Purpose:
    // Ensure that Unroll3WordsSkip1Swap does not modify memory beyond the expected 8-byte region
    // and that the function still writes the 3 color words correctly.

    unsigned char buffer[16];
    // Fill buffer with a known sentinel to detect any unintended writes beyond [0..7]
    for (size_t i = 0; i < sizeof(buffer); ++i) buffer[i] = static_cast<unsigned char>(0xAA);

    // Setup R,G,B in little-endian as in the previous test
    write16_le(buffer + 2, 0x0102); // R
    write16_le(buffer + 4, 0x0304); // G
    write16_le(buffer + 6, 0x0708); // B

    cmsUInt16Number wIn[3] = {0, 0, 0};

    cmsUInt8Number* ret = Unroll3WordsSkip1Swap(nullptr, wIn, buffer, 0);

    bool ok = true;

    // Check return pointer points to buffer + 8
    if (ret != buffer + 8) {
        std::printf("Unroll3WordsSkip1Swap_MemoryBound: return pointer mismatch. Expected %p, got %p\n",
                    (void*)(buffer + 8), (void*)ret);
        ok = false;
    }

    // Validate color values
    if (wIn[0] != 0x0708) {
        std::printf("Unroll3WordsSkip1Swap_MemoryBound: wIn[0] mismatch. Expected 0x0708, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[0]));
        ok = false;
    }
    if (wIn[1] != 0x0304) {
        std::printf("Unroll3WordsSkip1Swap_MemoryBound: wIn[1] mismatch. Expected 0x0304, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[1]));
        ok = false;
    }
    if (wIn[2] != 0x0102) {
        std::printf("Unroll3WordsSkip1Swap_MemoryBound: wIn[2] mismatch. Expected 0x0102, got 0x%04X\n",
                    static_cast<unsigned int>(wIn[2]));
        ok = false;
    }

    // Check that memory beyond 7 remains untouched (still 0xAA)
    for (size_t i = 8; i < sizeof(buffer); ++i) {
        if (buffer[i] != 0xAA) {
            std::printf("Unroll3WordsSkip1Swap_MemoryBound: Memory tail modified at offset %zu (expected 0xAA, got 0x%02X)\n",
                        i, buffer[i]);
            ok = false;
            break;
        }
    }

    return ok;
}

// Entry point for running the tests
int main() {
    int total = 0;
    int failed = 0;

    std::cout << "Starting unit tests for Unroll3WordsSkip1Swap...\n";

    if (test_Unroll3WordsSkip1Swap_Basic()) {
        std::cout << "PASS: test_Unroll3WordsSkip1Swap_Basic\n";
    } else {
        std::cout << "FAIL: test_Unroll3WordsSkip1Swap_Basic\n";
        failed++;
    }
    total++;

    if (test_Unroll3WordsSkip1Swap_MemoryBound()) {
        std::cout << "PASS: test_Unroll3WordsSkip1Swap_MemoryBound\n";
    } else {
        std::cout << "FAIL: test_Unroll3WordsSkip1Swap_MemoryBound\n";
        failed++;
    }
    total++;

    std::cout << "Tests completed. Passed: " << (total - failed) << ", Failed: " << failed << ".\n";

    // Return non-zero if any test failed to aid integration with simple shell scripts.
    return failed ? 1 : 0;
}
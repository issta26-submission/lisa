// Unit tests for Pack15bitsCMYK in fast_float_15bits.c
// Notes:
// - This test suite is written for C++11 and does not depend on GTest.
// - It assumes a little-endian host (typical x86/x86-64) for 16-bit word encoding.
// - We rely on a reasonable 15-to-16-bit mapping commonly used in CMS: From16To15(x) ≈ (x >> 1).
//   Consequently, the expected 16-bit value written for a channel value v is: 0x8000 - (v >> 1).
// - The tests validate: buffer content, buffer pointer advancement, buffer return semantics,
//   and invariance w.r.t. CMMcargo and Stride parameters.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdint>


// Forward declare the function under test.
// The exact macro CMSREGISTER is not defined in tests, so provide a safe fallback.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

extern "C" {

// Typedefs matching the project (best effort to align with the focal code types)
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Minimal placeholder for the struct used by the function.
// The actual layout is irrelevant for our tests as the parameters are unused
// by the current Pack15bitsCMYK implementation.
struct _cmstransform_struct {};

// Prototype of the focal function (as seen in the provided source)
cmsUInt8Number* Pack15bitsCMYK(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                               CMSREGISTER cmsUInt16Number Values[],
                               CMSREGISTER cmsUInt8Number*  Buffer,
                               CMSREGISTER cmsUInt32Number  Stride);
}

// Helper to interpret the 8-byte buffer as four 16-bit little-endian values
static uint16_t readWordLE(const uint8_t* p) {
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
}

// Simple test harness state
static int gFailCount = 0;
static void logFail(const char* testName, const char* msg) {
    std::cerr << "TEST FAILED [" << testName << "]: " << msg << "\n";
    ++gFailCount;
}

// Test 1: All input values are zero.
// Expected: Each written 16-bit value should be 0x8000 (assuming From16To15(0) == 0).
static void test_Pack15bitsCMYK_AllZeros() {
    const char* testName = "Pack15bitsCMYK_AllZeros";
    cmsUInt16Number Values[4] = {0, 0, 0, 0};
    uint8_t Buffer[8];
    std::memset(Buffer, 0, 8);

    uint8_t* res = Pack15bitsCMYK(nullptr, Values, Buffer, 0);

    // Return pointer should advance by exactly 8 bytes (4 * 2 bytes)
    if (res != Buffer + 8) {
        logFail(testName, "Return pointer does not point to Buffer + 8.");
        return;
    }

    // Expected pattern: 0x8000, 0x8000, 0x8000, 0x8000 in little-endian
    uint16_t w0 = readWordLE(Buffer + 0);
    uint16_t w1 = readWordLE(Buffer + 2);
    uint16_t w2 = readWordLE(Buffer + 4);
    uint16_t w3 = readWordLE(Buffer + 6);

    bool ok = (w0 == 0x8000) && (w1 == 0x8000) && (w2 == 0x8000) && (w3 == 0x8000);
    if (!ok) {
        char msg[128];
        std::sprintf(msg, "Expected [0x8000,0x8000,0x8000,0x8000], got [%04X,%04X,%04X,%04X]", w0, w1, w2, w3);
        logFail(testName, msg);
    }
}

// Test 2: All input values are 0xFFFF.
// Expected: Each written 16-bit value should be 0x0001 (assuming From16To15(0xFFFF) == 0x7FFF).
static void test_Pack15bitsCMYK_MaxValues() {
    const char* testName = "Pack15bitsCMYK_MaxValues";
    cmsUInt16Number Values[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    uint8_t Buffer[8];
    std::memset(Buffer, 0, 8);

    uint8_t* res = Pack15bitsCMYK(nullptr, Values, Buffer, 0);

    if (res != Buffer + 8) {
        logFail(testName, "Return pointer does not point to Buffer + 8.");
        return;
    }

    uint16_t w0 = readWordLE(Buffer + 0);
    uint16_t w1 = readWordLE(Buffer + 2);
    uint16_t w2 = readWordLE(Buffer + 4);
    uint16_t w3 = readWordLE(Buffer + 6);

    bool ok = (w0 == 0x0001) && (w1 == 0x0001) && (w2 == 0x0001) && (w3 == 0x0001);
    if (!ok) {
        char msg[128];
        std::sprintf(msg, "Expected [0x0001,0x0001,0x0001,0x0001], got [%04X,%04X,%04X,%04X]", w0, w1, w2, w3);
        logFail(testName, msg);
    }
}

// Test 3: Mixed values to validate ordering and computation.
// Values: {0, 1, 0x7FFF, 0xFFFF} and expected = 0x8000 - (v >> 1)
static void test_Pack15bitsCMYK_MixedValues() {
    const char* testName = "Pack15bitsCMYK_MixedValues";
    cmsUInt16Number Values[4] = {0, 1, 0x7FFF, 0xFFFF};
    uint8_t Buffer[8];
    std::memset(Buffer, 0, 8);

    uint8_t* res = Pack15bitsCMYK(nullptr, Values, Buffer, 0);

    if (res != Buffer + 8) {
        logFail(testName, "Return pointer does not point to Buffer + 8.");
        return;
    }

    uint16_t w0 = readWordLE(Buffer + 0);
    uint16_t w1 = readWordLE(Buffer + 2);
    uint16_t w2 = readWordLE(Buffer + 4);
    uint16_t w3 = readWordLE(Buffer + 6);

    uint16_t e0 = static_cast<uint16_t>(0x8000 - (Values[0] >> 1)); // 0
    uint16_t e1 = static_cast<uint16_t>(0x8000 - (Values[1] >> 1)); // 0x8000
    uint16_t e2 = static_cast<uint16_t>(0x8000 - (Values[2] >> 1)); // 0x4001
    uint16_t e3 = static_cast<uint16_t>(0x8000 - (Values[3] >> 1)); // 1

    bool ok = (w0 == e0) && (w1 == e1) && (w2 == e2) && (w3 == e3);
    if (!ok) {
        char msg[256];
        std::sprintf(msg, "Expected [%04X,%04X,%04X,%04X], got [%04X,%04X,%04X,%04X]",
                     e0, e1, e2, e3, w0, w1, w2, w3);
        logFail(testName, msg);
    }
}

// Test 4: Invariance with respect to CMMcargo and Stride parameters.
// The function should ignore these in the current implementation.
static void test_Pack15bitsCMYK_InvarianceWithCargoAndStride() {
    const char* testName = "Pack15bitsCMYK_InvarianceCargoStride";
    cmsUInt16Number Values[4] = {100, 200, 300, 400};
    uint8_t BufferA[8];
    uint8_t BufferB[8];
    std::memset(BufferA, 0, 8);
    std::memset(BufferB, 0, 8);

    struct _cmstransform_struct cargo1;
    struct _cmstransform_struct cargo2;
    CMSREGISTER struct _cmstransform_struct* c1 = &cargo1;
    CMSREGISTER struct _cmstransform_struct* c2 = &cargo2;

    uint8_t* r1 = Pack15bitsCMYK(c1, Values, BufferA, 0);
    // Change Stride and cargo
    uint8_t* r2 = Pack15bitsCMYK(c2, Values, BufferB, 1234);

    // The outputs should be identical for same Values; only Buffer pointers differ
    if (r1 != BufferA + 8 || r2 != BufferB + 8) {
        logFail(testName, "Return pointers incorrect for invariance test.");
        return;
    }

    if (std::memcmp(BufferA, BufferB, 8) != 0) {
        logFail(testName, "Buffer contents differ when changing CMMcargo/Stride (expected invariance).");
        return;
    }
}

// Test 5: Multiple runs to check stability over repeated calls with varying inputs.
static void test_Pack15bitsCMYK_StabilityOverCalls() {
    const char* testName = "Pack15bitsCMYK_StabilityOverCalls";
    const int rounds = 5;
    cmsUInt16Number Values[4] = {0, 0x1234, 0xABCD, 0xFFFF};
    uint8_t Buffer[8];
    std::memset(Buffer, 0, 8);

    for (int i = 0; i < rounds; ++i) {
        // mutate Values slightly each round
        Values[0] = static_cast<cmsUInt16Number>(Values[0] ^ 0x0001);
        Values[1] = static_cast<cmsUInt16Number>(Values[1] + 1);
        Values[2] = static_cast<cmsUInt16Number>(Values[2] - 2);
        Values[3] = static_cast<cmsUInt16Number>((Values[3] + 3) & 0xFFFF);

        uint8_t* res = Pack15bitsCMYK(nullptr, Values, Buffer, 0);
        if (res != Buffer + 8) {
            logFail(testName, "Return pointer not Buffer + 8 during stability test.");
            return;
        }

        // Optional: validate that the four 16-bit words are non-zero and within 0..0xFFFF
        uint16_t w0 = readWordLE(Buffer + 0);
        uint16_t w1 = readWordLE(Buffer + 2);
        uint16_t w2 = readWordLE(Buffer + 4);
        uint16_t w3 = readWordLE(Buffer + 6);

        if (w0 > 0xFFFF || w1 > 0xFFFF || w2 > 0xFFFF || w3 > 0xFFFF) {
            logFail(testName, "Decoded word out of range during stability test.");
            return;
        }
    }
}

// Main: run all tests
int main() {
    std::cout << "Starting Pack15bitsCMYK unit tests...\n";

    test_Pack15bitsCMYK_AllZeros();
    test_Pack15bitsCMYK_MaxValues();
    test_Pack15bitsCMYK_MixedValues();
    test_Pack15bitsCMYK_InvarianceWithCargoAndStride();
    test_Pack15bitsCMYK_StabilityOverCalls();

    if (gFailCount == 0) {
        std::cout << "All Pack15bitsCMYK tests PASSED.\n";
        return 0;
    } else {
        std::cerr << gFailCount << " Pack15bitsCMYK test(s) FAILED.\n";
        return 1;
    }
}
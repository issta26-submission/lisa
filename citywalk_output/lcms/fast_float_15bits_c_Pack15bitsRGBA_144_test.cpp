// Test suite for Pack15bitsRGBA (fast_float_15bits.c)
// This test is designed to be compiled alongside the project's fast_float_15bits.c
// using a C++11 compiler, without Google Test. It uses a lightweight, non-terminating
// assertion mechanism to exercise the focal method and verify its behavior.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <fast_float_internal.h>


// Domain knowledge and environment hints implemented here:
// - Define CMSREGISTER and essential types to match the focal function's signature.
// - Declare the function under test with C linkage so it can be linked from the C source file.
#define CMSREGISTER
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

struct _cmstransform_struct; // opaque in tests

extern "C" cmsUInt8Number* Pack15bitsRGBA(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                               CMSREGISTER cmsUInt16Number Values[],
                               CMSREGISTER cmsUInt8Number*  Buffer,
                               CMSREGISTER cmsUInt32Number  Stride);

// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;

static void LogFailure(const char* testName, const char* detail) {
    std::fprintf(stderr, "TEST FAIL: %s -- %s\n", testName, detail);
    ++g_test_failures;
}

static void ExpectEqU16(const char* testName, uint16_t expected, uint16_t actual, const char* detail) {
    if (expected != actual) {
        char msg[256];
        std::snprintf(msg, sizeof(msg), "%s | expected 0x%04X, got 0x%04X -- %s", testName, (unsigned)expected, (unsigned)actual, detail);
        LogFailure(testName, msg);
    }
}

static void ExpectEqPtr(const char* testName, const void* expected, const void* actual, const char* detail) {
    if (expected != actual) {
        char msg[256];
        std::snprintf(msg, sizeof(msg), "%s | pointer mismatch -- %p vs %p -- %s", testName, expected, actual, detail);
        LogFailure(testName, msg);
    }
}

static uint16_t ReadUInt16LE(const uint8_t* p) {
    // Read 16-bit little-endian value from buffer
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
}

/*
   Test 1: Basic packing correctness
   - Provide three 16-bit input values.
   - Expect three 16-bit 15-bit-converted results to be stored consecutively in Buffer.
   - Validate that the return pointer is Buffer + 10 (i.e., 3 * 2 bytes written + 4 extra as in implementation).
*/
static void Test_Pack15bitsRGBA_Basic() {
    const char* testName = "Test_Pack15bitsRGBA_Basic";

    cmsUInt16Number Values[3] = { 0x1234, 0xABCD, 0x0F0F };
    uint8_t Buffer[64];
    std::memset(Buffer, 0, sizeof(Buffer));

    cmsUInt8Number* ret = Pack15bitsRGBA(nullptr, Values, Buffer, 0);

    // Expected 15-bit values (mask to 15 bits)
    uint16_t exp0 = Values[0] & 0x7FFF;
    uint16_t exp1 = Values[1] & 0x7FFF;
    uint16_t exp2 = Values[2] & 0x7FFF;

    uint16_t got0 = ReadUInt16LE(Buffer + 0);
    uint16_t got1 = ReadUInt16LE(Buffer + 2);
    uint16_t got2 = ReadUInt16LE(Buffer + 4);

    ExpectEqU16(testName, exp0, got0, "First packed 15-bit value mismatch");
    ExpectEqU16(testName, exp1, got1, "Second packed 15-bit value mismatch");
    ExpectEqU16(testName, exp2, got2, "Third packed 15-bit value mismatch");

    // Check the returned pointer offset
    uint8_t* expectedRetPtr = Buffer + 10; // As per function's final return calculation
    ExpectEqPtr(testName, (void*)expectedRetPtr, (void*)ret, "Return pointer should be Buffer + 10");
}

/*
   Test 2: Upper-bound values (ensuring truncation to 15 bits)
   - Inputs include values that require truncation.
   - Validate that only the lower 15 bits are stored.
*/
static void Test_Pack15bitsRGBA_MaxValues() {
    const char* testName = "Test_Pack15bitsRGBA_MaxValues";

    cmsUInt16Number Values[3] = { 0xFFFF, 0x7FFF, 0x1234 };
    uint8_t Buffer[64];
    std::memset(Buffer, 0, sizeof(Buffer));

    cmsUInt8Number* ret = Pack15bitsRGBA(nullptr, Values, Buffer, 0);

    uint16_t exp0 = Values[0] & 0x7FFF; // 0x7FFF
    uint16_t exp1 = Values[1] & 0x7FFF; // 0x7FFF
    uint16_t exp2 = Values[2] & 0x7FFF; // 0x1234

    uint16_t got0 = ReadUInt16LE(Buffer + 0);
    uint16_t got1 = ReadUInt16LE(Buffer + 2);
    uint16_t got2 = ReadUInt16LE(Buffer + 4);

    ExpectEqU16(testName, exp0, got0, "First value should be upper-truncated to 0x7FFF");
    ExpectEqU16(testName, exp1, got1, "Second value should be 0x7FFF");
    ExpectEqU16(testName, exp2, got2, "Third value should be 0x1234");

    uint8_t* expectedRetPtr = Buffer + 10;
    ExpectEqPtr(testName, (void*)expectedRetPtr, (void*)ret, "Return pointer should be Buffer + 10");
}

/*
   Test 3: Stride parameter is ignored by the implementation
   - Ensure that non-zero Stride does not affect the written data or return value.
*/
static void Test_Pack15bitsRGBA_StrideIgnored() {
    const char* testName = "Test_Pack15bitsRGBA_StrideIgnored";

    cmsUInt16Number Values[3] = { 0x1A2B, 0x3C4D, 0x5E6F };
    uint8_t Buffer[64];
    std::memset(Buffer, 0xAA, sizeof(Buffer)); // Fill with a pattern to ensure not overwritten beyond expected area

    cmsUInt32Number Stride = 12345; // Non-zero stride to verify it's ignored
    cmsUInt8Number* ret = Pack15bitsRGBA(nullptr, Values, Buffer, Stride);

    uint16_t exp0 = Values[0] & 0x7FFF;
    uint16_t exp1 = Values[1] & 0x7FFF;
    uint16_t exp2 = Values[2] & 0x7FFF;

    uint16_t got0 = ReadUInt16LE(Buffer + 0);
    uint16_t got1 = ReadUInt16LE(Buffer + 2);
    uint16_t got2 = ReadUInt16LE(Buffer + 4);

    ExpectEqU16(testName, exp0, got0, "Stride test: first value mismatch");
    ExpectEqU16(testName, exp1, got1, "Stride test: second value mismatch");
    ExpectEqU16(testName, exp2, got2, "Stride test: third value mismatch");

    uint8_t* expectedRetPtr = Buffer + 10;
    ExpectEqPtr(testName, (void*)expectedRetPtr, (void*)ret, "Stride test: return pointer should be Buffer + 10");
}

/*
   Test 4: Boundary safety - ensure only the intended bytes are overwritten
   - After packing, bytes beyond the first 6 should remain unchanged.
*/
static void Test_Pack15bitsRGBA_BoundarySafety() {
    const char* testName = "Test_Pack15bitsRGBA_BoundarySafety";

    cmsUInt16Number Values[3] = { 0x0, 0x1, 0x2 };
    uint8_t Buffer[16];
    // Initialize with a known pattern
    for (size_t i = 0; i < sizeof(Buffer); ++i) Buffer[i] = static_cast<uint8_t>(i + 1);

    cmsUInt8Number* ret = Pack15bitsRGBA(nullptr, Values, Buffer, 0);

    // Only first 6 bytes should be modified; check that bytes 6..9 (inclusive) are unchanged from initial pattern
    // and return pointer equals Buffer + 10.
    // Verify first three 16-bit values are the 15-bit representations (0,1,2)
    uint16_t v0 = ReadUInt16LE(Buffer + 0);
    uint16_t v1 = ReadUInt16LE(Buffer + 2);
    uint16_t v2 = ReadUInt16LE(Buffer + 4);

    ExpectEqU16(testName, Values[0] & 0x7FFF, v0, "Boundary safety: first value");
    ExpectEqU16(testName, Values[1] & 0x7FFF, v1, "Boundary safety: second value");
    ExpectEqU16(testName, Values[2] & 0x7FFF, v2, "Boundary safety: third value");

    // Check that bytes beyond index 9 are untouched (i.e., buffer[9] onward remains from initialization)
    bool untouched = true;
    for (size_t i = 10; i < sizeof(Buffer); ++i) {
        if (Buffer[i] != static_cast<uint8_t>(i + 1)) { untouched = false; break; }
    }
    if (!untouched) {
        LogFailure(testName, "Boundary safety: bytes beyond written area were modified unexpectedly");
        ++g_test_failures;
    }

    uint8_t* expectedRetPtr = Buffer + 10;
    ExpectEqPtr(testName, (void*)expectedRetPtr, (void*)ret, "Boundary safety: return pointer should be Buffer + 10");
}

int main() {
    // Run all tests sequentially. Each test logs its own failures and continues.
    Test_Pack15bitsRGBA_Basic();
    Test_Pack15bitsRGBA_MaxValues();
    Test_Pack15bitsRGBA_StrideIgnored();
    Test_Pack15bitsRGBA_BoundarySafety();

    if (g_test_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("TESTS FAILED: %d failure(s)\n", g_test_failures);
        return 1;
    }
}
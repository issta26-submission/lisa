// Minimal C++11 test harness for the focal method: Pack4Words
// Notes:
// - This test is designed to be compiled alongside the original cmspack.c
//   (which contains the real implementation). We provide only the minimal
//   declarations needed to link and exercise Pack4Words.
// - We avoid GTest; tests are run from main with non-terminating assertions.
// - We assume 2-byte alignment for the output buffer to safely cast to cmsUInt16Number*.

#include <cstring>
#include <cstddef>
#include <cinttypes>
#include <iostream>
#include <type_traits>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight type aliases simulating the CMS library types
typedef unsigned char  cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int   cmsUInt32Number;

// The original code uses a macro CMSREGISTER; define as empty for tests
#define CMSREGISTER

// Forward declaration of the _cmsTRANSFORM type used by the focal method.
// The real type is defined in cms headers; here we only need an incomplete type
// to pass a pointer to the function (the function itself does not inspect it).
#ifdef __cplusplus
extern "C" {
#endif
struct _cmsTRANSFORM;
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Declaration of the focal method under test (link against cmspack.c)
extern cmsUInt8Number* Pack4Words(CMSREGISTER _cmsTRANSFORM* info,
                                 CMSREGISTER cmsUInt16Number wOut[],
                                 CMSREGISTER cmsUInt8Number* output,
                                 CMSREGISTER cmsUInt32Number Stride);
#ifdef __cplusplus
}
#endif

// Simple utility to read a 16-bit value from a byte buffer considering host endianness
static inline cmsUInt16Number readWord(const cmsUInt8Number* p) {
    return * reinterpret_cast<const cmsUInt16Number*>(p);
}

// Minimal test harness
static bool all_ok = true;
static void report(bool ok, const char* testName) {
    if (ok) {
        std::cout << "[OK]   " << testName << "\n";
    } else {
        std::cout << "[FAILED] " << testName << "\n";
        all_ok = false;
    }
}

// Test 1: Basic packing of four 16-bit values into an 8-byte output buffer.
// Verifies that the function writes wOut[0..3] into output at offsets 0,2,4,6
// and returns output pointer advanced by 8 bytes.
static void test_pack4words_basic() {
    // Prepare input
    _cmsTRANSFORM dummy;
    cmsUInt16Number wOut[4] = { 0x1111, 0x2222, 0x3333, 0x4444 };

    // 2-byte aligned output buffer to ensure safe cast to cmsUInt16Number*
    alignas(2) cmsUInt8Number outputBuf[16];
    std::memset(outputBuf, 0xAA, sizeof(outputBuf)); // sentinel to detect overrun

    // Call the function
    cmsUInt8Number* endPtr = Pack4Words(&dummy, wOut, outputBuf, 0);

    // Validate end pointer moved by 8 bytes
    bool ok = (endPtr == outputBuf + 8);

    // Validate written words
    cmsUInt16Number r0 = readWord(outputBuf + 0);
    cmsUInt16Number r1 = readWord(outputBuf + 2);
    cmsUInt16Number r2 = readWord(outputBuf + 4);
    cmsUInt16Number r3 = readWord(outputBuf + 6);

    ok = ok && (r0 == wOut[0]);
    ok = ok && (r1 == wOut[1]);
    ok = ok && (r2 == wOut[2]);
    ok = ok && (r3 == wOut[3]);

    report(ok, "Pack4Words_basic");
}

// Test 2: Ensure Stride and info parameters are effectively ignored.
// The output should be identical irrespective of Stride value and info pointer.
static void test_pack4words_stride_and_info_ignored() {
    _cmsTRANSFORM dummy;
    cmsUInt16Number wOut[4] = { 0xABCD, 0x1234, 0x0F0F, 0xFFFF };

    alignas(2) cmsUInt8Number outputBuf[16];
    std::memset(outputBuf, 0x00, sizeof(outputBuf));

    // Use a non-zero Stride and a non-null info pointer
    cmsUInt32Number Stride = 12345;
    cmsUInt8Number* endPtr = Pack4Words(&dummy, wOut, outputBuf, Stride);

    // Validate end pointer
    bool ok = (endPtr == outputBuf + 8);

    // Validate values read back
    cmsUInt16Number r0 = readWord(outputBuf + 0);
    cmsUInt16Number r1 = readWord(outputBuf + 2);
    cmsUInt16Number r2 = readWord(outputBuf + 4);
    cmsUInt16Number r3 = readWord(outputBuf + 6);

    ok = ok && (r0 == wOut[0]);
    ok = ok && (r1 == wOut[1]);
    ok = ok && (r2 == wOut[2]);
    ok = ok && (r3 == wOut[3]);

    report(ok, "Pack4Words_stride_and_info_ignored");
}

// Test 3: Verify that memory beyond the written 8 bytes remains unchanged.
// This checks for potential buffer overruns and confirms only 8 bytes are touched.
static void test_pack4words_no_overwrite_beyond8() {
    _cmsTRANSFORM dummy;
    cmsUInt16Number wOut[4] = { 0x0102, 0x0304, 0x0506, 0x0708 };

    alignas(2) cmsUInt8Number outputBuf[32];
    // Initialize all to a known pattern
    for (size_t i = 0; i < sizeof(outputBuf); ++i) outputBuf[i] = static_cast<cmsUInt8Number>(i);

    // Zero out the first 8 bytes to ensure we can compare after
    std::memset(outputBuf, 0x00, 8);
    // Set bytes beyond 8 to a distinct sentinel to detect changes
    for (size_t i = 8; i < sizeof(outputBuf); ++i) outputBuf[i] = 0x55;

    cmsUInt8Number* endPtr = Pack4Words(&dummy, wOut, outputBuf, 0);

    // End pointer should be exactly 8 bytes past start
    bool ok = (endPtr == outputBuf + 8);

    // Check that bytes 8..31 remain 0x55
    for (size_t i = 8; i < sizeof(outputBuf); ++i) {
        if (outputBuf[i] != 0x55) { ok = false; break; }
    }

    // Also verify the 8 bytes contain the expected words
    cmsUInt16Number r0 = readWord(outputBuf + 0);
    cmsUInt16Number r1 = readWord(outputBuf + 2);
    cmsUInt16Number r2 = readWord(outputBuf + 4);
    cmsUInt16Number r3 = readWord(outputBuf + 6);

    ok = ok && (r0 == wOut[0]);
    ok = ok && (r1 == wOut[1]);
    ok = ok && (r2 == wOut[2]);
    ok = ok && (r3 == wOut[3]);

    report(ok, "Pack4Words_no_overwrite_beyond8");
}

// Test 4: Ensure end-to-end behavior with extreme values.
// Uses 0xFFFF for all words to ensure proper storage of max 16-bit values.
static void test_pack4words_extreme_values() {
    _cmsTRANSFORM dummy;
    cmsUInt16Number wOut[4] = { 0xFFFF, 0x0000, 0xFF00, 0x00FF };

    alignas(2) cmsUInt8Number outputBuf[16];
    std::memset(outputBuf, 0x33, sizeof(outputBuf));

    cmsUInt8Number* endPtr = Pack4Words(&dummy, wOut, outputBuf, 0);

    bool ok = (endPtr == outputBuf + 8);

    cmsUInt16Number r0 = readWord(outputBuf + 0);
    cmsUInt16Number r1 = readWord(outputBuf + 2);
    cmsUInt16Number r2 = readWord(outputBuf + 4);
    cmsUInt16Number r3 = readWord(outputBuf + 6);

    ok = ok && (r0 == wOut[0]);
    ok = ok && (r1 == wOut[1]);
    ok = ok && (r2 == wOut[2]);
    ok = ok && (r3 == wOut[3]);

    report(ok, "Pack4Words_extreme_values");
}

int main() {
    // Run all tests sequentially from main, as required by the instructions.
    test_pack4words_basic();
    test_pack4words_stride_and_info_ignored();
    test_pack4words_no_overwrite_beyond8();
    test_pack4words_extreme_values();

    if (all_ok) {
        std::cout << "[OVERALL] All tests passed.\n";
        return 0;
    } else {
        std::cout << "[OVERALL] Some tests failed. See details above.\n";
        return 1;
    }
}
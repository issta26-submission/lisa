// Generated unit tests for Pack6WordsSwap in cmspack.c
// Notes:
// - This test suite is designed for C++11 minimal environment without GoogleTest.
// - We provide a lightweight test harness that calls the focal function and checks
//   results with simple assertions.
// - The tests assume the availability of the following (declared here for compilation):
//     typedef unsigned char cmsUInt8Number;
//     typedef unsigned short cmsUInt16Number;
//     typedef unsigned int cmsUInt32Number;
//     struct _cmsTRANSFORM;
//     typedef struct _cmsTRANSFORM _cmsTRANSFORM;
//     cmsUInt8Number* Pack6WordsSwap(CMSREGISTER _cmsTRANSFORM* info,
//                                    CMSREGISTER cmsUInt16Number wOut[],
//                                    CMSREGISTER cmsUInt8Number* output,
//                                    CMSREGISTER cmsUInt32Number Stride);
// - CMSREGISTER is defined as empty macro in the actual project; we define it here as empty
//   to match the signature for compilation against the real implementation.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight local declarations to match the focal function's signature.
// These mirror the project's typedefs in a minimal way sufficient for compilation.
#define CMSREGISTER
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

extern "C" {
    // Focal function under test. We declare it here so the test can link against the real implementation.
    cmsUInt8Number* Pack6WordsSwap(CMSREGISTER _cmsTRANSFORM* info,
                                   CMSREGISTER cmsUInt16Number wOut[],
                                   CMSREGISTER cmsUInt8Number* output,
                                   CMSREGISTER cmsUInt32Number Stride);
}

// Simple test harness helpers
static inline bool approx_equal_uint16(uint16_t a, uint16_t b) {
    return a == b;
}

// Macro for test assertions with informative messages
#define ASSERT_TRUE(cond, msg) do {                               \
    if (!(cond)) {                                                \
        std::cerr << "Assertion failed: " << (msg) << "\n";      \
        return false;                                             \
    }                                                             \
} while (0)

static bool test_Pack6WordsSwap_basic_reverse_order() {
    // Verifies that Pack6WordsSwap writes wOut[5], wOut[4], ..., wOut[0]
    // in that exact order, packing 6 words into the output buffer, advancing by 2 bytes per word.
    _cmsTRANSFORM* info = nullptr; // not used in this focal function
    cmsUInt16Number wOut[6] = { 0, 1, 2, 3, 4, 5 }; // ascending input
    cmsUInt8Number outputBuffer[12];
    std::memset(outputBuffer, 0xAA, sizeof(outputBuffer)); // ensure clean slate

    cmsUInt8Number* endPtr = Pack6WordsSwap(info, wOut, outputBuffer, 0);

    // Expected sequence in 16-bit words after packing: {wOut[5], wOut[4], wOut[3], wOut[2], wOut[1], wOut[0]}
    uint16_t expected[6] = { wOut[5], wOut[4], wOut[3], wOut[2], wOut[1], wOut[0] };
    uint16_t actual[6];
    std::memcpy(actual, outputBuffer, sizeof(actual));

    for (int i = 0; i < 6; ++i) {
        if (!approx_equal_uint16(actual[i], expected[i])) {
            std::cerr << "Mismatch at index " << i << ": expected "
                      << expected[i] << ", got " << actual[i] << "\n";
            return false;
        }
    }

    // The returned pointer should point to the buffer advanced by 6 * sizeof(uint16_t) = 12 bytes
    cmsUInt8Number* expectedEnd = outputBuffer + 12;
    ASSERT_TRUE(endPtr == expectedEnd, "Return pointer should point to end of written data (buffer + 12)");
    return true;
}

static bool test_Pack6WordsSwap_with_non_trivial_values() {
    // Verifies reversal with non-sequential 16-bit values
    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[6] = { 0x1234, 0xABCD, 0x0F0F, 0xFFFF, 0x0000, 0x00AA };
    cmsUInt8Number outputBuffer[12];
    std::memset(outputBuffer, 0x00, sizeof(outputBuffer));

    cmsUInt8Number* endPtr = Pack6WordsSwap(info, wOut, outputBuffer, 0);

    uint16_t expected[6] = { wOut[5], wOut[4], wOut[3], wOut[2], wOut[1], wOut[0] };
    uint16_t actual[6];
    std::memcpy(actual, outputBuffer, sizeof(actual));

    for (int i = 0; i < 6; ++i) {
        if (!approx_equal_uint16(actual[i], expected[i])) {
            std::cerr << "Mismatch (non-trivial) at index " << i << ": expected "
                      << expected[i] << ", got " << actual[i] << "\n";
            return false;
        }
    }

    cmsUInt8Number* expectedEnd = outputBuffer + 12;
    ASSERT_TRUE(endPtr == expectedEnd, "Return pointer should point to end of written data for non-trivial values");
    return true;
}

static bool test_Pack6WordsSwap_with_nonzero_stride_and_check_return() {
    // Checks that Stride parameter does not affect the output and that return pointer is correct
    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[6] = { 9, 8, 7, 6, 5, 4 };
    cmsUInt8Number outputBuffer[12];
    std::memset(outputBuffer, 0xFF, sizeof(outputBuffer));

    cmsUInt32Number stride = 7; // non-zero, should be ignored by the focal function
    cmsUInt8Number* endPtr = Pack6WordsSwap(info, wOut, outputBuffer, stride);

    uint16_t expected[6] = { wOut[5], wOut[4], wOut[3], wOut[2], wOut[1], wOut[0] };
    uint16_t actual[6];
    std::memcpy(actual, outputBuffer, sizeof(actual));

    for (int i = 0; i < 6; ++i) {
        if (!approx_equal_uint16(actual[i], expected[i])) {
            std::cerr << "Stride test: mismatch at index " << i << " expected "
                      << expected[i] << " got " << actual[i] << "\n";
            return false;
        }
    }

    cmsUInt8Number* expectedEnd = outputBuffer + 12;
    ASSERT_TRUE(endPtr == expectedEnd, "Return pointer should be buffer + 12 even with non-zero stride");
    return true;
}

// Entry point for the test suite
int main() {
    int failures = 0;

    std::cout << "Running tests for Pack6WordsSwap (cmspack.c)..." << std::endl;

    if (!test_Pack6WordsSwap_basic_reverse_order()) {
        std::cerr << "[FAILED] test_Pack6WordsSwap_basic_reverse_order" << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_Pack6WordsSwap_basic_reverse_order" << std::endl;
    }

    if (!test_Pack6WordsSwap_with_non_trivial_values()) {
        std::cerr << "[FAILED] test_Pack6WordsSwap_with_non_trivial_values" << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_Pack6WordsSwap_with_non_trivial_values" << std::endl;
    }

    if (!test_Pack6WordsSwap_with_nonzero_stride_and_check_return()) {
        std::cerr << "[FAILED] test_Pack6WordsSwap_with_nonzero_stride_and_check_return" << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_Pack6WordsSwap_with_nonzero_stride_and_check_return" << std::endl;
    }

    if (failures > 0) {
        std::cout << "Some tests FAILED. Failures: " << failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}
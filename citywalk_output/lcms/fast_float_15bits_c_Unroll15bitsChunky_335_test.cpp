// Test suite for Unroll15bitsChunky in fast_float_15bits.c
// This test suite is written for C++11, without GoogleTest.
// The tests intentionally avoid private implementation details and focus on
// observable behavior of the function (buffer advancement and channel handling).

#include <cstring>
#include <iostream>
#include <fast_float_internal.h>


extern "C" {
    // Import the necessary types and prototypes from the library headers.
    // The path may vary; adjust to the project's include layout if needed.
    #include "fast_float_internal.h"

    // Prototype for the focal function (C linkage to avoid name mangling in C++).
    cmsUInt8Number* Unroll15bitsChunky(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                        CMSREGISTER cmsUInt16Number Values[],
                                        CMSREGISTER cmsUInt8Number*  Buffer,
                                        CMSREGISTER cmsUInt32Number  Stride);
}

// Simple non-terminating test assertion macro.
// It records failures but continues executing to maximize code coverage.
static int gTestFailures = 0;
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << (msg) << std::endl; \
        ++gTestFailures; \
    } \
} while(false)

// Helper to access the _xform_head portion inside the _cmstransform_struct.
// We rely on the internal convention used by the focal implementation:
// the first member of _cmstransform_struct is _xform_head which provides
// InputFormat and OutputFormat bit-fields used by macros (T_CHANNELS, etc.).
static inline int getInputFormat(const struct _cmstransform_struct* cm) {
    const _xform_head* head = (const _xform_head*) cm;
    return head->InputFormat;
}
static inline int getOutputFormat(const struct _cmstransform_struct* cm) {
    const _xform_head* head = (const _xform_head*) cm;
    return head->OutputFormat;
}

// Test 1: No Swap scenario (DoSwap == 0) with a single channel.
// We verify:
// - The function returns a Buffer pointer advanced by 2 * nChan.
// - At least the first channel's value in Values[] is written (not left at sentinel).
static void test_Unroll15bitsChunky_NoSwap_SingleChannel() {
    std::cout << "Running test_Unroll15bitsChunky_NoSwap_SingleChannel..." << std::endl;

    struct _cmstransform_struct cm;
    std::memset(&cm, 0, sizeof(cm));

    // Choose an InputFormat that (likely) indicates 1 channel and no swap.
    // Keep OutputFormat minimal to avoid extra buffer offset.
    // Note: The exact bit-field interpretation is defined by the library macros.
    // We rely on the existing macro machinery to interpret this value.
    (( _xform_head*) &cm)->InputFormat  = 0x0001; // guess: 1 channel
    (( _xform_head*) &cm)->OutputFormat = 0x0000; // no extra bytes for this test

    // Prepare input buffer with two 16-bit values (one channel).
    cmsUInt16Number inVals[2] = { 0x1234, 0xABCD }; // second value unused for 1 channel
    cmsUInt8Number Buffer[4];
    // Copy 16-bit values into the buffer (little-endian representation on typical platforms)
    std::memcpy(Buffer, inVals, sizeof(inVals));

    const CmsUInt32Number Stride = 0; // unused by Unroll15bitsChunky

    // Values array initialized with sentinel to verify writes
    cmsUInt16Number Values[8];
    for (int i = 0; i < 8; ++i) Values[i] = 0xFFFF;

    cmsUInt8Number* endPtr = Unroll15bitsChunky(&cm, Values, Buffer, Stride);

    int nChan = T_CHANNELS((( _xform_head*) &cm)->InputFormat);
    cmsUInt8Number* expectedEnd = Buffer + (2 * nChan);

    TEST_CHECK(endPtr == expectedEnd, "NoSwap: End pointer should advance by 2 * nChan");
    TEST_CHECK(Values[0] != 0xFFFF, "NoSwap: First channel value should be written");
    // For robustness, ensure other channels (if any) remain sentinel when nChan==1
    if (nChan > 1) {
        TEST_CHECK(Values[1] != 0xFFFF, "NoSwap: Second channel value should be written when nChan > 1");
    }

    std::cout << "test_Unroll15bitsChunky_NoSwap_SingleChannel completed." << std::endl;
}

// Test 2: Swap scenario (DoSwap == 1) with two channels.
// We verify:
// - The function returns a Buffer pointer advanced by 2 * nChan plus potential extra bytes
//   due to OutputFormat; we assume OutputFormat == 0 to keep offset predictable.
// - Values[] are updated for both channels.
static void test_Unroll15bitsChunky_Swap_TwoChannels() {
    std::cout << "Running test_Unroll15bitsChunky_Swap_TwoChannels..." << std::endl;

    struct _cmstransform_struct cm;
    std::memset(&cm, 0, sizeof(cm));

    // Choose an InputFormat that (likely) indicates 2 channels and DoSwap = 1.
    // The exact bit-field interpretation is defined by the library macros.
    (( _xform_head*) &cm)->InputFormat  = 0x0002; // guess: 2 channels
    (( _xform_head*) &cm)->OutputFormat = 0x0000; // minimal to simplify expectations

    // Prepare input buffer with two 16-bit values (two channels).
    cmsUInt16Number inVals[2] = { 0x1357, 0x9ABC };
    cmsUInt8Number Buffer[4];
    std::memcpy(Buffer, inVals, sizeof(inVals));

    const CmsUInt32Number Stride = 0; // unused

    // Values array initialized with sentinel
    cmsUInt16Number Values[8];
    for (int i = 0; i < 8; ++i) Values[i] = 0xFFFF;

    cmsUInt8Number* endPtr = Unroll15bitsChunky(&cm, Values, Buffer, Stride);

    int nChan = T_CHANNELS((( _xform_head*) &cm)->InputFormat);
    cmsUInt8Number* expectedEnd = Buffer + (2 * nChan); // assuming T_EXTRA(OutputFormat) == 0

    TEST_CHECK(endPtr == expectedEnd, "Swap: End pointer should advance by 2 * nChan (assuming no extra OutputFormat bytes)");
    TEST_CHECK(Values[0] != 0xFFFF, "Swap: First channel value should be written");
    TEST_CHECK(Values[1] != 0xFFFF, "Swap: Second channel value should be written");
    std::cout << "test_Unroll15bitsChunky_Swap_TwoChannels completed." << std::endl;
}

// Entry point to run tests
int main() {
    std::cout << "Starting Unroll15bitsChunky test suite" << std::endl;

    test_Unroll15bitsChunky_NoSwap_SingleChannel();
    test_Unroll15bitsChunky_Swap_TwoChannels();

    if (gTestFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << gTestFailures << " test(s) failed." << std::endl;
        return 1;
    }
}
// Note: This is a lightweight C++11 test harness for the focal method
// PackNBytesSwapDither, designed to be used with the project where the
// original headers (.h) and implementations (.c) exist.
// The tests rely on the actual library's types, macros, and formatter
// constants (e.g., TYPE_GRAY_8_DITHER, TYPE_BGR_8_DITHER, etc.).
// No Google Test is used; a small in-process test runner is provided.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdint>


// Include the library's internal header to obtain type definitions,
// macros (e.g., TYPE_GRAY_8_DITHER, T_EXTRA), and the function prototype.
// This header must exist in the project's include path.

#ifdef __cplusplus
extern "C" {
#endif

// Prototype for the focal function; the actual signature is provided by the
// project's headers, but we declare it here to ensure linkage from C++ tests.
cmsUInt8Number* PackNBytesSwapDither(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                     CMSREGISTER cmsUInt16Number Values[],
                                     CMSREGISTER cmsUInt8Number*  Buffer,
                                     CMSREGISTER cmsUInt32Number  Stride);
#ifdef __cplusplus
}
#endif

// Simple test harness utilities
static int gFailures = 0;
static void fail(const std::string& message)
{
    std::cerr << "TEST FAIL: " << message << std::endl;
    ++gFailures;
}
#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << msg << " | expected: " << (expected) << " actual: " << (actual); \
        fail(oss.str()); \
    } \
} while(0)

static // Helper to allocate a dummy _cmstransform_struct buffer and set OutputFormat
void* allocateCMMS(struct _cmstransform_struct** outCargo, cmsUInt32Number outputFormat)
{
    // Determine the required size for _cmstransform_struct from the type.
    size_t sz = sizeof(struct _cmstransform_struct);
    // Allocate raw memory; Test keeps it alive just for the duration of the call.
    unsigned char* raw = new unsigned char[sz];
    // Cast to the internal types so we can fill the OutputFormat field
    _xform_head* info = reinterpret_cast<_xform_head*>(raw);
    info->OutputFormat = outputFormat;
    *outCargo = reinterpret_cast<struct _cmstransform_struct*>(raw);
    return raw;
}

// Simple wrapper to cleanup allocated memory for CMMcargo
void cleanupCMMS(void* mem)
{
    delete[] reinterpret_cast<unsigned char*>(mem);
}

// Test 1: Gray 8-bit Dither (single channel, small value)
void test_gray8_dither_basic()
{
    // Prepare a CMMcargo with TYPE_GRAY_8_DITHER
    struct _cmstransform_struct* CMMcargo = nullptr;
    void* raw = allocateCMMS(&CMMcargo, TYPE_GRAY_8_DITHER);

    // Inputs
    cmsUInt16Number Values[1] = { 123 }; // single channel
    cmsUInt8Number Buffer[16];
    std::memset(Buffer, 0, sizeof(Buffer));

    // Call
    cmsUInt8Number* startBuffer = Buffer;
    cmsUInt8Number* ret = PackNBytesSwapDither(CMMcargo, Values, Buffer, 0);

    // Expected:
    // n = Values[0] + err[0] (assumes initial err[0] == 0)
    // pe = n / 257; pf = n % 257; pf becomes err[0] (not checked here)
    // Since 123 < 257, pe = 0
    cmsUInt8Number expectedFirstByte = 0;
    ASSERT_EQ((size_t)0, (size_t)Buffer[0], "GRAY_8_DITHER first output byte should be 0 for Value=123");

    // Expected return pointer: startBuffer + nChan(1) + T_EXTRA(OutputFormat)
    size_t expectedDiff = 1 + T_EXTRA(TYPE_GRAY_8_DITHER);
    ASSERT_EQ((size_t)expectedDiff, (size_t)(ret - startBuffer),
              "GRAY_8_DITHER return pointer offset should equal nChan + T_EXTRA");

    // Cleanup
    cleanupCMMS(raw);
}

// Test 2: 3-channel 8-bit Dither (BGR order, Values chosen to trigger non-zero pe)
void test_bgr8_dither_swap_nonzero_pe()
{
    // Prepare a CMMcargo with TYPE_BGR_8_DITHER (3 channels)
    struct _cmstransform_struct* CMMcargo = nullptr;
    void* raw = allocateCMMS(&CMMcargo, TYPE_BGR_8_DITHER);

    cmsUInt16Number Values[3] = { 100, 200, 300 }; // note: last value will push over 257
    cmsUInt8Number Buffer[16];
    std::memset(Buffer, 0, sizeof(Buffer));

    cmsUInt8Number* startBuffer = Buffer;
    cmsUInt8Number* ret = PackNBytesSwapDither(CMMcargo, Values, Buffer, 0);

    // Because err[] is presumed to be initialized to 0 by the library for a fresh transform,
    // the computations are:
    // i=2: n = 300; pe=1, pf=43; first output byte = 1
    // i=1: n = 200; pe=0, pf=200; second output byte = 0
    // i=0: n = 100; pe=0, pf=100; third output byte = 0
    ASSERT_EQ((cmsUInt8Number)1, Buffer[0], "BGR_8_DITHER first output byte should be 1 (due to 300+0)");
    ASSERT_EQ((cmsUInt8Number)0, Buffer[1], "BGR_8_DITHER second output byte should be 0");
    ASSERT_EQ((cmsUInt8Number)0, Buffer[2], "BGR_8_DITHER third output byte should be 0");

    size_t expectedDiff = 3 + T_EXTRA(TYPE_BGR_8_DITHER);
    ASSERT_EQ((size_t)expectedDiff, (size_t)(ret - startBuffer),
              "BGR_8_DITHER return pointer offset should equal nChan + T_EXTRA");

    // Cleanup
    cleanupCMMS(raw);
}

// Test 3: 4-channel 8-bit Dither (ABGR order)
void test_abgr8_dither_swap_all_zero_or_small_values()
{
    // Prepare a CMMcargo with TYPE_ABGR_8_DITHER (4 channels)
    struct _cmstransform_struct* CMMcargo = nullptr;
    void* raw = allocateCMMS(&CMMcargo, TYPE_ABGR_8_DITHER);

    cmsUInt16Number Values[4] = { 0, 0, 0, 5 }; // all small; expect pe=0 for all
    cmsUInt8Number Buffer[32];
    std::memset(Buffer, 0, sizeof(Buffer));

    cmsUInt8Number* startBuffer = Buffer;
    cmsUInt8Number* ret = PackNBytesSwapDither(CMMcargo, Values, Buffer, 0);

    // For all small values (< 257), pe should be 0 for each iteration.
    // Buffer[0] corresponds to i=3 (Value[3] = 5)
    ASSERT_EQ((cmsUInt8Number)0, Buffer[0], "ABGR_8_DITHER first output byte should be 0");
    ASSERT_EQ((cmsUInt8Number)0, Buffer[1], "ABGR_8_DITHER second output byte should be 0");
    ASSERT_EQ((cmsUInt8Number)0, Buffer[2], "ABGR_8_DITHER third output byte should be 0");
    ASSERT_EQ((cmsUInt8Number)0, Buffer[3], "ABGR_8_DITHER fourth output byte should be 0");

    size_t expectedDiff = 4 + T_EXTRA(TYPE_ABGR_8_DITHER);
    ASSERT_EQ((size_t)expectedDiff, (size_t)(ret - startBuffer),
              "ABGR_8_DITHER return pointer offset should equal nChan + T_EXTRA");

    // Cleanup
    cleanupCMMS(raw);
}

int main()
{
    // Run tests
    test_gray8_dither_basic();
    test_bgr8_dither_swap_nonzero_pe();
    test_abgr8_dither_swap_all_zero_or_small_values();

    if (gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Tests finished with " << gFailures << " failure(s)." << std::endl;
        return 1;
    }
}
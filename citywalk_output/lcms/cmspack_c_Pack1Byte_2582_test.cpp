// Unit test suite for Pack1Byte in cmspack.c
// Notes:
// - This test assumes the build environment provides the original cms library sources
//   (including lcms2_internal.h and the cmspack.c implementation).
// - We avoid GTest; we implement a tiny, non-terminating test harness that prints
//   results and continues execution to maximize coverage.
// - Tests focus on the behavior of Pack1Byte: it writes the converted 8-bit value
//   corresponding to wOut[0] into the output buffer and returns output+1.
// - We use the library-provided FROM_16_TO_8 macro (via including lcms2_internal.h)
//   to compute the expected value, ensuring alignment with the focal method's logic.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// If the test environment provides the real header, include it to obtain proper
// type definitions and the macro FROM_16_TO_8.
// If not present, provide minimal stubs to allow compilation in isolated environments.
// This stub section preserves portability for environments without the full CMS library.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Minimal type stubs (align with common lcms typedefs). If the real library headers
// are available, these will be overridden by the real definitions.
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

typedef struct _cmsTRANSFORM {} _cmsTRANSFORM;

// Forward declaration of the function under test.
// The function is defined in cmspack.c in the real project; link against that object.
// Use extern "C" to ensure C linkage when compiling as C++.
extern "C" cmsUInt8Number* Pack1Byte(CMSREGISTER _cmsTRANSFORM* info,
                          CMSREGISTER cmsUInt16Number wOut[],
                          CMSREGISTER cmsUInt8Number* output,
                          CMSREGISTER cmsUInt32Number Stride);

// In case the environment provides lcms2_internal.h, include it to obtain
// the macro FROM_16_TO_8 and related semantics. If not available, provide a safe fallback.
#ifdef __has_include
#if __has_include("lcms2_internal.h")
#else
// Fallback: define a simplistic conversion mirroring common 16->8 behavior (high byte).
#ifndef FROM_16_TO_8
#define FROM_16_TO_8(x) (cmsUInt8Number)(((x) >> 8) & 0xFF)
#endif
#endif
#else
#ifndef FROM_16_TO_8
#define FROM_16_TO_8(x) (cmsUInt8Number)(((x) >> 8) & 0xFF)
#endif
#endif

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_BEGIN(name) bool name() {
#define TEST_END(name)  return true; }

#define ASSERT_TRUE(cond, msg) do {                                        \
    if (!(cond)) {                                                          \
        std::fprintf(stderr, "Assertion failed: %s (in %s:%d) - %s\n",    \
                     #cond, __FILE__, __LINE__, (msg));                   \
        g_failed_tests++;                                                 \
    }                                                                       \
    g_total_tests++;                                                        \
} while (0)

#define ASSERT_EQ(actual, expected, msg) do {                               \
    if ((actual) != (expected)) {                                           \
        std::fprintf(stderr, "Assertion failed: %s (actual=%llu, expected=%llu) (%s) in %s:%d\n", \
                     #actual, (unsigned long long)(actual), (unsigned long long)(expected), (msg), __FILE__, __LINE__); \
        g_failed_tests++;                                                   \
    }                                                                       \
    g_total_tests++;                                                        \
} while (0)

static void print_summary() {
    std::printf("\nTest summary: %d run, %d failed\n", g_total_tests, g_failed_tests);
}

// Test 1: Basic correctness for a single write
TEST_BEGIN(test_Pack1Byte_basic)
{
    _cmsTRANSFORM info = {}; // dummy transform object; not used by Pack1Byte logic
    cmsUInt16Number wOut[1] = { 0xABCD }; // high byte should be written (per FROM_16_TO_8)
    cmsUInt8Number outputBuf[4];
    cmsUInt8Number* res = Pack1Byte(&info, wOut, outputBuf, 0);

    cmsUInt8Number expected = FROM_16_TO_8(wOut[0]);
    ASSERT_EQ((unsigned long long)(outputBuf[0]), (unsigned long long)(expected),
              "Pack1Byte should write the high byte of wOut[0] into output");
    ASSERT_EQ((unsigned long long)(res), (unsigned long long)(&outputBuf[1]),
              "Pack1Byte should return pointer advanced by one");

}
TEST_END(test_Pack1Byte_basic)

// Test 2: Two successive calls should fill consecutive bytes and advance pointer correctly
TEST_BEGIN(test_Pack1Byte_two_calls)
{
    _cmsTRANSFORM info = {};
    cmsUInt16Number wOut1[1] = { 0x1234 };
    cmsUInt16Number wOut2[1] = { 0x00FF };
    cmsUInt8Number outBuf[4];

    cmsUInt8Number* r1 = Pack1Byte(&info, wOut1, outBuf, 0);
    cmsUInt8Number expected1 = FROM_16_TO_8(wOut1[0]);
    ASSERT_EQ((unsigned long long)(outBuf[0]), (unsigned long long)(expected1),
              "First call: output should hold the converted high byte of wOut1");
    ASSERT_EQ((unsigned long long)(r1), (unsigned long long)(&outBuf[1]),
              "First call: return value should be output+1");

    cmsUInt8Number* r2 = Pack1Byte(&info, wOut2, r1, 0);
    cmsUInt8Number expected2 = FROM_16_TO_8(wOut2[0]);
    ASSERT_EQ((unsigned long long)(outBuf[1]), (unsigned long long)(expected2),
              "Second call: second output byte should hold converted value of wOut2");
    ASSERT_EQ((unsigned long long)(r2), (unsigned long long)(&outBuf[2]),
              "Second call: return value should be output+2");
}
TEST_END(test_Pack1Byte_two_calls)

// Test 3: Stride parameter is unused, but ensure behavior is unaffected when non-zero
TEST_BEGIN(test_Pack1Byte_stride_nonzero)
{
    _cmsTRANSFORM info = {};
    cmsUInt16Number wOut[1] = { 0x5A3C };
    cmsUInt8Number outputBuf[2];
    cmsUInt8Number* res = Pack1Byte(&info, wOut, outputBuf, 12345);

    cmsUInt8Number expected = FROM_16_TO_8(wOut[0]);
    ASSERT_EQ((unsigned long long)(outputBuf[0]), (unsigned long long)(expected),
              "Stride should not affect first byte written");
    ASSERT_EQ((unsigned long long)(res), (unsigned long long)(&outputBuf[1]),
              "Return value should be output+1 regardless of Stride");
}
TEST_END(test_Pack1Byte_stride_nonzero)

// Main: Run all tests
int main() {
    // Run tests
    if (test_Pack1Byte_basic()) {
        std::printf("test_Pack1Byte_basic PASSED\n");
    } else {
        std::printf("test_Pack1Byte_basic FAILED\n");
    }

    if (test_Pack1Byte_two_calls()) {
        std::printf("test_Pack1Byte_two_calls PASSED\n");
    } else {
        std::printf("test_Pack1Byte_two_calls FAILED\n");
    }

    if (test_Pack1Byte_stride_nonzero()) {
        std::printf("test_Pack1Byte_stride_nonzero PASSED\n");
    } else {
        std::printf("test_Pack1Byte_stride_nonzero FAILED\n");
    }

    print_summary();
    return (g_failed_tests == 0) ? 0 : 1;
}
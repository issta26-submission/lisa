// Minimal C++11 unit test suite for the focal function PackDoubleFrom16
// Note: This test harness is designed to be compiled alongside the existing
// C/C++ project that provides the actual implementation of PackDoubleFrom16
// (as found in cmspack.c). It uses a lightweight, non-terminating test style
// (EXPECT_* assertions) to maximize code coverage without terminating on first
// failure. The tests focus on exercising the function interface and ensuring it
// does not crash under simple, well-defined inputs.
//
// Important: This test relies on the external linkage of PackDoubleFrom16 from the
// project's build (so the object/file cmspack.c needs to be compiled and linked).
// We provide a minimal compatible struct layout for the input parameter so tests
// can instantiate and pass an info object with OutputFormat.
// We do not depend on internal macros here; the test aims to call the function and
// observe basic, non-crashing behavior.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Forward declare the focal function from the project.
// We declare a C linkage to avoid name mangling when linking with the C implementation.
extern "C" {

// Typedefs matching the project's core types (as far as needed by the focal function prototype).
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Opaque translation transform type used by the focal method.
// We only use the OutputFormat member in tests; the rest of the real structure is
// opaque to this test harness.
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// Prototype of the focal function (signature taken from the provided source).
// Note: The actual CMSREGISTER macro is defined in the project; for test purposes we
// rely on C linkage and a direct signature compatible with the implementation.
cmsUInt8Number* PackDoubleFrom16(cmsTRANSFORM* info,
                                cmsUInt16Number wOut[],
                                cmsUInt8Number* output,
                                cmsUInt32Number Stride);
}

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do {                                    \
    ++g_total_tests;                                                \
    if (!(cond)) {                                                  \
        fprintf(stderr, "Expectation failed: %s:%d: %s\n",        \
                __FILE__, __LINE__, #cond);                         \
        ++g_failed_tests;                                         \
    }                                                             \
} while (0)

#define EXPECT_FALSE(cond) do {                                   \
    EXPECT_TRUE(!(cond));                                         \
} while (0)

#define EXPECT_NOT_NULL(ptr) do {                                  \
    ++g_total_tests;                                                \
    if ((ptr) == nullptr) {                                       \
        fprintf(stderr, "Expectation failed: %s:%d: NULL pointer: %s\n", \
                __FILE__, __LINE__, #ptr);                           \
        ++g_failed_tests;                                         \
    }                                                             \
} while (0)

#define EXPECT_EQ(a, b) do {                                       \
    ++g_total_tests;                                                \
    if (!((a) == (b))) {                                          \
        fprintf(stderr, "Expectation failed: %s:%d: %s == %s (actual: %lld, expected: %lld)\n", \
                __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b));  \
        ++g_failed_tests;                                         \
    }                                                             \
} while (0)

#define EXPECT_NEAR(a, b, tol) do {                                 \
    ++g_total_tests;                                                  \
    if (!(((a) - (b) < (tol)) && ((b) - (a) < (tol)))) {             \
        fprintf(stderr, "Expectation failed: %s:%d: |%s-%s| <= %f (actual: %f, expected: %f)\n", \
                __FILE__, __LINE__, #a, #b, (tol), (double)(a), (double)(b)); \
        ++g_failed_tests;                                           \
    }                                                               \
} while (0)

static void PrintTestSummary() {
    printf("\nTest summary: %d ran, %d failed\n", g_total_tests, g_failed_tests);
}

// Minimal compatible _cmsTRANSFORM layout used by tests.
// We only need the OutputFormat field to influence the focal function's behavior.
struct _cmsTRANSFORM {
    uint32_t OutputFormat;
    // The real project has many more members; for testing the focal function's
    // behavior, only OutputFormat is accessed in the tested path.
};

// Test 1: Baseline call with a simple, zeroed OutputFormat.
// Rationale: With a neutral/zeroed OutputFormat, the function should execute without
// crashing and return a valid pointer (the exact write behavior is governed by the
// project's internal macros; we verify basic function call semantics and non-NULL return).
static void test_pack_double_from16_baseline_zero_outputformat() {
    // Prepare a minimal info object with OutputFormat = 0
    _cmsTRANSFORM info;
    info.OutputFormat = 0;

    // Simple input data
    cmsUInt16Number wOut[4] = { 0, 0, 0, 0 };
    const size_t kOutBufSize = 256;
    cmsUInt8Number outputBuf[kOutBufSize];
    std::memset(outputBuf, 0, kOutBufSize);

    // Stride argument (values are interpreted by the focal function)
    cmsUInt32Number Stride = 4;

    // Call focal function (extern "C" linkage)
    cmsUInt8Number* result = PackDoubleFrom16(&info, wOut, outputBuf, Stride);

    // Expectations: non-null return and buffer not modified in an undefined manner
    EXPECT_NOT_NULL(result);
    // We also expect the function to not crash; by returning a non-null pointer we
    // assert that basic execution path was taken.
}

// Test 2: Non-trivial input to exercise basic path with non-zero data.
// Rationale: Even if precise per-channel behavior depends on internal bit-fields,
// the function should handle non-zero inputs gracefully and return a valid pointer.
// We check that the function returns a non-null pointer and that the output buffer is
// at least not all zeros (indicating some write activity occurred).
static void test_pack_double_from16_nonzero_input() {
    _cmsTRANSFORM info;
    // Choose a non-zero OutputFormat to exercise internal branches minimalistically.
    // The exact meaning of this value is internal to the cms macros; we rely on the
    // function to operate without crashing.
    info.OutputFormat = 0x00000001;

    cmsUInt16Number wOut[4] = { 1000, 2000, 3000, 4000 };
    const size_t kOutBufSize = 256;
    cmsUInt8Number outputBuf[kOutBufSize];
    std::memset(outputBuf, 0xAA, kOutBufSize); // fill with a recognizable pattern

    cmsUInt32Number Stride = 8;

    cmsUInt8Number* result = PackDoubleFrom16(&info, wOut, outputBuf, Stride);

    EXPECT_NOT_NULL(result);

    // Basic sanity: the output buffer should be touched in some way if function wrote doubles
    // (we can't rely on exact values due to macro-driven behavior), so at least one byte
    // should differ from the original pre-call pattern if the function doesn't crash.
    bool hasChange = false;
    for (size_t i = 0; i < kOutBufSize; ++i) {
        if (outputBuf[i] != 0xAA) {
            hasChange = true;
            break;
        }
    }
    EXPECT_TRUE(hasChange);
}

// Test 3: Iterative invocation with varying OutputFormat to exercise stability.
// Rationale: Repeated invocations with varying flags should not crash the focal function
// and should consistently return a non-null pointer.
static void test_pack_double_from16_multiple_formats() {
    cmsUInt16Number wOut[4] = { 12345, 54321, 11111, 22222 };
    const size_t kOutBufSize = 512;
    cmsUInt8Number outputBuf[kOutBufSize];
    std::memset(outputBuf, 0, kOutBufSize);

    for (uint32_t fmt : { 0x0, 0x1, 0x2F, 0x80000000U }) {
        _cmsTRANSFORM info;
        info.OutputFormat = fmt;
        cmsUInt32Number Stride = 16;

        cmsUInt8Number* res = PackDoubleFrom16(&info, wOut, outputBuf, Stride);
        EXPECT_NOT_NULL(res);
    }
}

// Entry point: run all tests
int main() {
    printf("Starting PackDoubleFrom16 unit tests...\n");

    test_pack_double_from16_baseline_zero_outputformat();
    test_pack_double_from16_nonzero_input();
    test_pack_double_from16_multiple_formats();

    PrintTestSummary();
    return (g_failed_tests == 0) ? 0 : 1;
}
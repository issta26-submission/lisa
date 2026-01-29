// Unit tests for UnrollLabV2_16ToFloat in cmspack.c
// This test suite is written for C++11 without GoogleTest.
// It relies on the focal function being present in a linked C translation unit (cmspack.c).
// We expose the function with C linkage and provide minimal type aliases to match the signature.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Candidate Keywords (for reference):
// UnrollLabV2_16ToFloat, LabV2, LabV4, FomLabV2ToLabV4, lab4toFloat, _cmsTRANSFORM, wIn, accum, Stride

// Forward declare the expected C linkage type to mirror the focal function's signature.
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;
typedef float cmsFloat32Number;

// Forward declare the transform struct (incomplete type, as seen in the focal code)
struct _cmsTRANSFORM;

// Declare the focal function with C linkage (as implemented in cmspack.c)
extern "C" cmsUInt8Number* UnrollLabV2_16ToFloat(_cmsTRANSFORM* info,
                                              cmsFloat32Number wIn[],
                                              cmsUInt8Number* accum,
                                              cmsUInt32Number Stride);

// Simple test framework (no external libraries)
static int g_failures = 0;

#define ASSERT_EQ(actual, expected, message)                           \
    do {                                                               \
        if ((actual) != (expected)) {                                  \
            ++g_failures;                                              \
            fprintf(stderr, "TEST_FAIL: %s\n", message);               \
            fprintf(stderr, "  Expected: %llu, Got: %llu\n",        \
                    (unsigned long long)(expected),                   \
                    (unsigned long long)(actual));                   \
        } else {                                                       \
            fprintf(stdout, "TEST_PASS: %s\n", message);               \
        }                                                              \
    } while (0)

static void test_UnrollLabV2_16ToFloat_basic_advancement() {
    // This test verifies that UnrollLabV2_16ToFloat advances the accum pointer
    // by exactly 6 bytes (three 16-bit reads, 2 bytes each).
    // We cannot rely on internal conversion results (lab4toFloat), but we can
    // ensure the function consumes the expected amount of input data.

    unsigned char buffer1[6];
    // Fill with three 16-bit numbers: 0x1111, 0x2222, 0x3333 (little-endian byte order)
    uint16_t source_vals1[3] = {0x1111, 0x2222, 0x3333};
    memcpy(buffer1, source_vals1, 6);

    float wIn[3] = {0.0f, 0.0f, 0.0f};
    unsigned char* accum_start = buffer1;
    unsigned char* accum_end = UnrollLabV2_16ToFloat(nullptr, wIn, accum_start, 0);

    // Expect the function to have advanced by 6 bytes (3 * 2 bytes)
    size_t consumed = static_cast<size_t>(accum_end - accum_start);
    ASSERT_EQ((unsigned long long)consumed, (unsigned long long)6ULL,
              "UnrollLabV2_16ToFloat should advance accum by 6 bytes (three 16-bit reads)");
}

static void test_UnrollLabV2_16ToFloat_multiple_calls_are_independent() {
    // Ensure that a second independent call also advances by 6 bytes and does not
    // depend on the state from the first call.

    unsigned char buffer2[6];
    uint16_t source_vals2[3] = {0xAAAA, 0xBBBB, 0xCCCC};
    memcpy(buffer2, source_vals2, 6);

    float wIn[3] = {0.0f, 0.0f, 0.0f};
    unsigned char* accum_start = buffer2;
    unsigned char* accum_end = UnrollLabV2_16ToFloat(nullptr, wIn, accum_start, 0);

    size_t consumed = static_cast<size_t>(accum_end - accum_start);
    ASSERT_EQ((unsigned long long)consumed, (unsigned long long)6ULL,
              "UnrollLabV2_16ToFloat (second invocation) should advance accum by 6 bytes");
}

int main() {
    fprintf(stdout, "Running UnrollLabV2_16ToFloat unit tests...\n");

    test_UnrollLabV2_16ToFloat_basic_advancement();
    test_UnrollLabV2_16ToFloat_multiple_calls_are_independent();

    if (g_failures == 0) {
        fprintf(stdout, "ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }
}
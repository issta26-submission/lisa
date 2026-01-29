// Test suite for the focal method Unroll15bitsBGR in fast_float_15bits.c
// This test uses a lightweight, self-contained harness (no GTest) and relies on
// the project-provided types and function prototypes via fast_float_internal.h.
// The tests verify that Unroll15bitsBGR reads three 16-bit 15-bit values from
// the input Buffer, applies From15To16 on each, stores them into Values in the
// order [2], [1], [0], and returns Buffer advanced by 8 bytes (Buffer + 8).
//
// Candidate Keywords identified from the focal method and dependencies:
// Unroll15bitsBGR, From15To16, 15-bit to 16-bit conversion, BGR order, Values[],
// Buffer, Stride, CMMcargo, cmsUInt16Number, cmsUInt8Number, _cmstransform_struct.
//
// Important: This test assumes a little-endian host due to how Unroll15bitsBGR
// reads 16-bit values from a byte buffer via a pointer-to-16-bit type.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// Include project-specific type definitions and function declarations.
// The test uses the project’s internal type names (cmsUInt8Number, cmsUInt16Number, etc.)
// and assumes Unroll15bitsBGR is exposed with C linkage from fast_float_15bits.c.

struct _cmstransform_struct; // forward declaration for the transform struct

extern "C" {
    // Prototype for the focal method under test.
    cmsUInt8Number* Unroll15bitsBGR(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                    CMSREGISTER cmsUInt16Number Values[],
                                    CMSREGISTER cmsUInt8Number*  Buffer,
                                    CMSREGISTER cmsUInt32Number  Stride);
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "[EXPECT_TRUE] " << msg << "\n"; ++g_failures; } } while(0)
#define EXPECT_EQ(a, b, msg) \
    do { if( (a) != (b) ) { std::cerr << "[EXPECT_EQ] " << msg \
          << " | " << (unsigned long long)(a) \
          << " != " << (unsigned long long)(b) << "\n"; ++g_failures; } } while(0)

// Test: Basic Unroll15bitsBGR behavior for a single buffer of three 15-bit values.
// Verifies that Values[2] = From15To16(in0), Values[1] = From15To16(in1), Values[0] = From15To16(in2)
// and that the returned pointer equals initial Buffer + 8.
static void test_Unroll15bitsBGR_basic_case() {
    // Prepare three 15-bit inputs (uint16_t but with 15-bit range)
    cmsUInt16Number in0 = 0x1A2B; // example 15-bit-ish value
    cmsUInt16Number in1 = 0x3C4D;
    cmsUInt16Number in2 = 0x0F01;

    // Build a 6-byte buffer in little-endian order corresponding to [in0, in1, in2]
    unsigned char buffer[6];
    buffer[0] = (unsigned char)(in0 & 0xFF);
    buffer[1] = (unsigned char)((in0 >> 8) & 0xFF);
    buffer[2] = (unsigned char)(in1 & 0xFF);
    buffer[3] = (unsigned char)((in1 >> 8) & 0xFF);
    buffer[4] = (unsigned char)(in2 & 0xFF);
    buffer[5] = (unsigned char)((in2 >> 8) & 0xFF);

    cmsUInt16Number Values[3] = {0, 0, 0};
    unsigned char* startBuf = buffer;
    cmsUInt8Number* ret = Unroll15bitsBGR(nullptr, Values, buffer, 0);

    cmsUInt16Number expected2 = From15To16(in0);
    cmsUInt16Number expected1 = From15To16(in1);
    cmsUInt16Number expected0 = From15To16(in2);

    // Validate the values written by Unroll15bitsBGR
    EXPECT_EQ(Values[2], expected2, "Values[2] should be From15To16(in0)");
    EXPECT_EQ(Values[1], expected1, "Values[1] should be From15To16(in1)");
    EXPECT_EQ(Values[0], expected0, "Values[0] should be From15To16(in2)");

    // Validate the pointer arithmetic: 3 reads of 2 bytes each => +6, then returns +2 => +8
    EXPECT_TRUE(ret == startBuf + 8, "Returned Buffer pointer should be initial + 8 bytes");
}

// Test: Zero-input case to ensure no spurious data leakage and correct reset behavior.
static void test_Unroll15bitsBGR_zero_inputs() {
    cmsUInt16Number in0 = 0x0000;
    cmsUInt16Number in1 = 0x0000;
    cmsUInt16Number in2 = 0x0000;

    unsigned char buffer[6] = {0}; // all zeros
    cmsUInt16Number Values[3] = {0x1234, 0xABCD, 0x0F0F}; // non-zero to verify overwrite
    cmsUInt8Number* ret = Unroll15bitsBGR(nullptr, Values, buffer, 0);

    cmsUInt16Number expected0 = From15To16(in2);
    cmsUInt16Number expected1 = From15To16(in1);
    cmsUInt16Number expected2 = From15To16(in0);

    EXPECT_EQ(Values[2], expected2, "Zero-input test: Values[2] should be From15To16(in0) (0x0000)");
    EXPECT_EQ(Values[1], expected1, "Zero-input test: Values[1] should be From15To16(in1) (0x0000)");
    EXPECT_EQ(Values[0], expected0, "Zero-input test: Values[0] should be From15To16(in2) (0x0000)");

    EXPECT_TRUE(ret == buffer + 8, "Zero-input test: Returned pointer should be buffer + 8");
}

// Test: Stride parameter immaterial to correctness (Unroll15bitsBGR ignores Stride).
static void test_Unroll15bitsBGR_stride_ignored() {
    cmsUInt16Number in0 = 0x1234;
    cmsUInt16Number in1 = 0x2345;
    cmsUInt16Number in2 = 0x3456;

    unsigned char buffer[6];
    buffer[0] = (unsigned char)(in0 & 0xFF);
    buffer[1] = (unsigned char)((in0 >> 8) & 0xFF);
    buffer[2] = (unsigned char)(in1 & 0xFF);
    buffer[3] = (unsigned char)((in1 >> 8) & 0xFF);
    buffer[4] = (unsigned char)(in2 & 0xFF);
    buffer[5] = (unsigned char)((in2 >> 8) & 0xFF);

    cmsUInt16Number Values[3] = {0, 0, 0};
    cmsUInt8Number* ret = Unroll15bitsBGR(nullptr, Values, buffer, 0xDEADBEEF); // arbitrary Stride

    cmsUInt16Number expected2 = From15To16(in0);
    cmsUInt16Number expected1 = From15To16(in1);
    cmsUInt16Number expected0 = From15To16(in2);

    EXPECT_EQ(Values[2], expected2, "Stride test: Values[2] should be From15To16(in0)");
    EXPECT_EQ(Values[1], expected1, "Stride test: Values[1] should be From15To16(in1)");
    EXPECT_EQ(Values[0], expected0, "Stride test: Values[0] should be From15To16(in2)");
    EXPECT_TRUE(ret == buffer + 8, "Stride test: Returned pointer should be buffer + 8");
}

// Entry point to run all tests
static void run_all_tests() {
    test_Unroll15bitsBGR_basic_case();
    test_Unroll15bitsBGR_zero_inputs();
    test_Unroll15bitsBGR_stride_ignored();
}

int main() {
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "[TEST] All Unroll15bitsBGR tests passed.\n";
        return 0;
    } else {
        std::cerr << "[TEST] Unroll15bitsBGR tests completed with " << g_failures << " failure(s).\n";
        return 1;
    }
}
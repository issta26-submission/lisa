// Unit test suite for the focal method Pack4WordsBigEndian in cmspack.c
// This test suite is designed for a C++11 environment without GoogleTest.
// It uses a lightweight test harness with non-terminating checks to maximize coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstring>


// Include the core types and function declarations from the CMS/LittleCMS project.
// Adjust the include path if your environment places CMS headers differently.

// Forward declaration to match the signature as used in the focal file.
// The actual project defines _cmsTRANSFORM in its headers; here we only need a type
// to compile against the function signature.
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Ensure we can call the focal function from C++ code.
extern "C" cmsUInt8Number* Pack4WordsBigEndian(CMSREGISTER _cmsTRANSFORM* info,
                                              CMSREGISTER cmsUInt16Number wOut[],
                                              CMSREGISTER cmsUInt8Number* output,
                                              CMSREGISTER cmsUInt32Number Stride);

// Lightweight test harness
static int gFailCount = 0;

// Non-terminating assertion helpers
#define EXPECT_EQ(a, b) do {                                         \
    auto _a = (a); auto _b = (b);                                    \
    if (!(_a == _b)) {                                               \
        std::cerr << "EXPECT_EQ failed at line " << __LINE__ << ": " \
                  << #a << " (" << _a << ") != " << #b             \
                  << " (" << _b << ")" << std::endl;                \
        ++gFailCount;                                                \
    }                                                                \
} while (0)

#define EXPECT_MEMEQ(buf, exp, len) do {                              \
    if (std::memcmp((buf), (exp), (len)) != 0) {                      \
        std::cerr << "EXPECT_MEMEQ failed at line " << __LINE__            \
                  << ": memory contents do not match expected." << std::endl; \
        ++gFailCount;                                                  \
    }                                                                   \
} while (0)


// Test 1: Basic packing of four 16-bit words into a BigEndian byte stream.
// Verifies that the memory representation matches the big-endian bytes of wOut.
void test_Pack4WordsBigEndian_BasicCase() {
    // Prepare input words
    cmsUInt16Number wOut[4] = { 0x1234, 0x5678, 0x9ABC, 0xDEF0 };

    // Output buffer to receive packed bytes (8 bytes)
    unsigned char output[8];
    unsigned char expected[8] = {
        0x12, 0x34,  // big-endian bytes for 0x1234
        0x56, 0x78,  // big-endian bytes for 0x5678
        0x9A, 0xBC,  // big-endian bytes for 0x9ABC
        0xDE, 0xF0   // big-endian bytes for 0xDEF0
    };

    // Call the function under test
    cmsUInt8Number* ret = Pack4WordsBigEndian(nullptr, wOut, output, 0);

    // Validate memory contents
    EXPECT_MEMEQ(output, expected, sizeof(expected));

    // Validate the return pointer equals start + 8 bytes
    EXPECT_EQ(ret, output + 8);
}

// Test 2: Boundary values to ensure correct endian packing across edge words.
// Uses 0x0000, 0xFFFF, 0x00FF, 0xFF00 to verify byte patterns.
void test_Pack4WordsBigEndian_BoundaryValues() {
    cmsUInt16Number wOut[4] = { 0x0000, 0xFFFF, 0x00FF, 0xFF00 };

    unsigned char output[8];
    unsigned char expected[8] = {
        0x00, 0x00,  // 0x0000
        0xFF, 0xFF,  // 0xFFFF
        0x00, 0xFF,  // 0x00FF
        0xFF, 0x00   // 0xFF00
    };

    cmsUInt8Number* ret = Pack4WordsBigEndian(nullptr, wOut, output, 0);

    EXPECT_MEMEQ(output, expected, sizeof(expected));
    EXPECT_EQ(ret, output + 8);
}

// Test 3: Stride parameter should be ignored by the function (as per its implementation)
// and the return pointer should still advance by 8 bytes.
void test_Pack4WordsBigEndian_StrideIgnoredAndReturnPointer() {
    cmsUInt16Number wOut[4] = { 0x1234, 0x5678, 0x9ABC, 0xDEF0 };

    unsigned char output[8];
    cmsUInt8Number* ret = nullptr;

    // Use a non-zero Stride value to ensure it's ignored
    ret = Pack4WordsBigEndian(nullptr, wOut, output, 7);

    unsigned char expected[8] = {
        0x12, 0x34, 0x56, 0x78,
        0x9A, 0xBC, 0xDE, 0xF0
    };

    EXPECT_MEMEQ(output, expected, sizeof(expected));
    EXPECT_EQ(ret, output + 8);
}

// Test 4: Re-verify that changing the Stride does not alter the produced bytes.
// This ensures the function's signature contract remains stable.
void test_Pack4WordsBigEndian_StrideDoesNotAffectOutput() {
    cmsUInt16Number wOut[4] = { 0x1111, 0x2222, 0x3333, 0x4444 };

    unsigned char output1[8];
    unsigned char output2[8];
    unsigned char expected[8] = {
        0x11, 0x11, 0x22, 0x22,
        0x33, 0x33, 0x44, 0x44
    };

    cmsUInt8Number* ret1 = Pack4WordsBigEndian(nullptr, wOut, output1, 0);
    cmsUInt8Number* ret2 = Pack4WordsBigEndian(nullptr, wOut, output2, 9999);

    EXPECT_MEMEQ(output1, expected, sizeof(expected));
    EXPECT_MEMEQ(output2, expected, sizeof(expected));
    EXPECT_EQ(ret1, output1 + 8);
    EXPECT_EQ(ret2, output2 + 8);
}

// Main function that runs all tests and reports a summary.
// Exits with a non-zero code if any test failed, allowing integration with build systems.
int main() {
    std::cout << "Running Pack4WordsBigEndian unit tests...\n";

    test_Pack4WordsBigEndian_BasicCase();
    test_Pack4WordsBigEndian_BoundaryValues();
    test_Pack4WordsBigEndian_StrideIgnoredAndReturnPointer();
    test_Pack4WordsBigEndian_StrideDoesNotAffectOutput();

    if (gFailCount == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << gFailCount << " test(s) failed.\n";
        return 1;
    }
}
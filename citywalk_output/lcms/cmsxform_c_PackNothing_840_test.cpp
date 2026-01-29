/*
   Unit tests for PackNothing in cmsxform.c

   - Uses a minimal, self-contained test harness (no gtest).
   - Verifies that PackNothing returns the exact output pointer passed in.
   - Verifies that PackNothing does not modify the output buffer.
   - Verifies behavior when output is NULL.
   - Verifies behavior with a non-NULL info pointer (the function should ignore it, but we pass a dummy one to exercise that parameter).
   - All tests are designed to compile with C++11 and link against the cmsxform.c / cms headers provided by the project.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


static int gFailures = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (" << #cond << ")" << std::endl; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) << " | " << (a) << " != " << (b) << std::endl; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " << (msg) << " | " << (void*)(a) << " != " << (void*)(b) << std::endl; \
        ++gFailures; \
    } \
} while(0)

extern "C" {
    // Include internal CMS header to get the correct type definitions
    #include "lcms2_internal.h"

    // Forward declare the struct used by the function signature
    struct _cmsTRANSFORM;

    // Prototype for the focal function (extern "C" to prevent name mangling)
    cmsUInt8Number* PackNothing(CMSREGISTER struct _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride);
}

// Test 1: PackNothing should return the same pointer as the output parameter
bool test_PackNothing_ReturnsOutputPointer() {
    // Arrange
    cmsUInt16Number wIn[4] = { 10, 20, 30, 40 }; // value is irrelevant for PackNothing
    cmsUInt8Number output[8] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22 };

    // Act
    cmsUInt8Number* ret = PackNothing(nullptr, wIn, output, 0);

    // Assert
    EXPECT_PTR_EQ(ret, output, "PackNothing should return the same pointer as the provided output buffer");
    return gFailures == 0;
}

// Test 2: PackNothing should not modify the contents of the output buffer
bool test_PackNothing_BufferNotModified() {
    // Arrange
    cmsUInt16Number wIn[4] = { 1, 2, 3, 4 };
    cmsUInt8Number output[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    cmsUInt8Number expected[8];
    std::memcpy(expected, output, sizeof(output));

    // Act
    PackNothing(nullptr, wIn, output, 4);

    // Assert
    // Compare byte-for-byte
    for (size_t i = 0; i < sizeof(output); ++i) {
        EXPECT_EQ(output[i], expected[i], "Output buffer should remain unchanged by PackNothing");
    }
    return gFailures == 0;
}

// Test 3: PackNothing should return NULL when output is NULL
bool test_PackNothing_OutputNullReturnsNull() {
    // Arrange
    cmsUInt16Number wIn[2] = { 7, 8 };
    cmsUInt8Number* output = NULL;

    // Act
    cmsUInt8Number* ret = PackNothing(nullptr, wIn, output, 0);

    // Assert
    EXPECT_TRUE(ret == NULL, "PackNothing should return NULL when output is NULL");
    return gFailures == 0;
}

// Test 4: PackNothing should accept a non-NULL info parameter (even though it is unused)
bool test_PackNothing_NonNullInfoParameter() {
    // Arrange
    cmsUInt16Number wIn[2] = { 123, 456 };
    cmsUInt8Number output[6] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 };

    // Create a minimal dummy _cmsTRANSFORM struct instance
    struct _cmsTRANSFORM dummyInfo;

    // Act
    cmsUInt8Number* ret = PackNothing(&dummyInfo, wIn, output, 2);

    // Assert
    EXPECT_PTR_EQ(ret, output, "PackNothing should return the same pointer even when info is non-NULL");
    return gFailures == 0;
}

// Runner to execute all tests
bool RunAllTests() {
    bool ok = true;
    ok = ok && test_PackNothing_ReturnsOutputPointer();
    ok = ok && test_PackNothing_BufferNotModified();
    ok = ok && test_PackNothing_OutputNullReturnsNull();
    ok = ok && test_PackNothing_NonNullInfoParameter();
    return ok;
}

int main() {
    bool allPassed = RunAllTests();
    if (allPassed) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED. Failures: " << gFailures << std::endl;
        return 1;
    }
}
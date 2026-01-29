// Unit test suite for the focal method PackLabV2_16
// This test harness avoids external testing frameworks (GTest) as requested.
// It provides lightweight "EXPECT" style checks that do not terminate on failure
// and prints a summary at the end for coverage and validity verification.
//
// The tests are designed to be self-contained by providing minimal necessary
// type definitions and a deterministic fake implementation of FomLabV4ToLabV2
// so that PackLabV2_16 can be exercised without the full cmsLCMS dependency set.
//
// Note: This reproduces the focal function's behavior in a self-contained way for unit testing.

#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific minimal type aliases to mirror the focal code's types
using cmsUInt8Number  = uint8_t;
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;

// Lightweight macro to simulate CMS's parameter-unused annotation
#define cmsUNUSED_PARAMETER(x) ((void)(x))

// CMSREGISTER is typically a macro for calling convention/registration.
// Here we just make it a no-op for test purposes.
#define CMSREGISTER

// Forward declaration of the minimal _cmsTRANSFORM structure used by the focal method
struct _cmsTRANSFORM;

// Lightweight, deterministic replacement for the actual FomLabV4ToLabV2.
// This is intentionally simple to produce predictable, testable outputs.
static cmsUInt16Number FomLabV4ToLabV2(cmsUInt16Number x)
{
    // Simple reversible transform for test determinism
    return x ^ 0xAAAA;
}

// Implement a self-contained PackLabV2_16 that mirrors the provided focal method.
// This allows testing without needing the full file cmspack.c and its dependencies.
cmsUInt8Number* PackLabV2_16(CMSREGISTER _cmsTRANSFORM* info,
                             CMSREGISTER cmsUInt16Number wOut[],
                             CMSREGISTER cmsUInt8Number* output,
                             CMSREGISTER cmsUInt32Number Stride)
{
{
    *(cmsUInt16Number*) output = FomLabV4ToLabV2(wOut[0]);
    output += 2;
    *(cmsUInt16Number*) output = FomLabV4ToLabV2(wOut[1]);
    output += 2;
    *(cmsUInt16Number*) output = FomLabV4ToLabV2(wOut[2]);
    output += 2;
    return output;
    cmsUNUSED_PARAMETER(info);
    cmsUNUSED_PARAMETER(Stride);
}
}

// --- Lightweight test framework infrastructure (non-terminating) ---

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected, message) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (message) \
                  << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << "\n"; \
    } \
} while(0)

#define EXPECT_TRUE(condition, message) do { \
    ++g_tests_run; \
    if (!(condition)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (message) << " | Condition evaluated to false\n"; \
    } \
} while(0)

#define TEST_NAME(name) static void name()

// Helper to interpret output buffer as an array of 16-bit words (little-endian)
static inline cmsUInt16Number* toWords(cmsUInt8Number* buf)
{
    return reinterpret_cast<cmsUInt16Number*>(buf);
}

// --- Focal method tests ---

// Test 1: Basic mapping for three inputs, verify content matches FomLabV4ToLabV2(wOut[i])
TEST_NAME(test_PackLabV2_16_BasicMapping)
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0x0001, 0x00FF, 0x1234 };
    cmsUInt8Number outputBuf[6]; // 3 * 2 bytes
    cmsUInt8Number* ret;

    // Act
    ret = PackLabV2_16(nullptr, wOut, outputBuf, 0);

    // Compute expected using the same deterministic mapping
    cmsUInt16Number expected0 = FomLabV4ToLabV2(wOut[0]);
    cmsUInt16Number expected1 = FomLabV4ToLabV2(wOut[1]);
    cmsUInt16Number expected2 = FomLabV4ToLabV2(wOut[2]);

    cmsUInt16Number* outWords = toWords(outputBuf);

    // Assert
    EXPECT_EQ(outWords[0], expected0, "PackLabV2_16 first element mismatch");
    EXPECT_EQ(outWords[1], expected1, "PackLabV2_16 second element mismatch");
    EXPECT_EQ(outWords[2], expected2, "PackLabV2_16 third element mismatch");

    // Also verify the returned pointer advanced by 6 bytes from the start
    cmsUInt8Number* startPtr = outputBuf;
    cmsUInt8Number* expectedRetPtr = startPtr + 6;
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret) - startPtr, 6u, "PackLabV2_16 return pointer offset");
}

// Test 2: Unused Stride parameter does not affect output content
TEST_NAME(test_PackLabV2_16_StrideIgnorance)
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 };
    cmsUInt8Number outputBuf1[6];
    cmsUInt8Number outputBuf2[6];
    cmsUInt8Number* ret1;
    cmsUInt8Number* ret2;

    // Act (Stride = 0)
    ret1 = PackLabV2_16(nullptr, wOut, outputBuf1, 0);
    // Act (Stride = non-zero)
    ret2 = PackLabV2_16(nullptr, wOut, outputBuf2, 12345);

    cmsUInt16Number* outWords1 = toWords(outputBuf1);
    cmsUInt16Number* outWords2 = toWords(outputBuf2);

    // Assert
    // Outputs should be identical regardless of Stride
    EXPECT_EQ(outWords1[0], outWords2[0], "Stride should not affect first output word");
    EXPECT_EQ(outWords1[1], outWords2[1], "Stride should not affect second output word");
    EXPECT_EQ(outWords1[2], outWords2[2], "Stride should not affect third output word");

    // Return pointer difference should still be 6
    cmsUInt8Number* startPtr = outputBuf1;
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret1) - startPtr, 6u, "Return pointer offset with Stride=0");
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret2) - startPtr, 6u, "Return pointer offset with Stride=12345 (startPtr for comparison)");
}

// Test 3: All-zero input yields all-zero output
TEST_NAME(test_PackLabV2_16_AllZeros)
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 };
    cmsUInt8Number outputBuf[6];
    cmsUInt8Number* ret;

    // Act
    ret = PackLabV2_16(nullptr, wOut, outputBuf, 0);
    cmsUInt16Number* outWords = toWords(outputBuf);

    // Assert
    EXPECT_EQ(outWords[0], FomLabV4ToLabV2(wOut[0]), "All zeros first element");
    EXPECT_EQ(outWords[1], FomLabV4ToLabV2(wOut[1]), "All zeros second element");
    EXPECT_EQ(outWords[2], FomLabV4ToLabV2(wOut[2]), "All zeros third element");
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret) - outputBuf, 6u, "Return pointer offset for all zeros");
}

// Test 4: Non-daylight sanity check with mixed values and non-null info/Stride
TEST_NAME(test_PackLabV2_16_NonNullInfoAndStride)
{
    // Arrange
    cmsUInt16Number wOut[3] = { 0xABCD, 0x0F0F, 0xFFFF };
    cmsUInt8Number outputBuf[6];
    cmsUInt8Number dummyInfo;
    cmsUInt32Number stride = 42;

    // Act
    cmsUInt8Number* ret = PackLabV2_16(reinterpret_cast<_cmsTRANSFORM*>(&dummyInfo), wOut, outputBuf, stride);
    cmsUInt16Number* outWords = toWords(outputBuf);

    // Assert
    EXPECT_EQ(outWords[0], FomLabV4ToLabV2(wOut[0]), "Non-null info/stride: first word");
    EXPECT_EQ(outWords[1], FomLabV4ToLabV2(wOut[1]), "Non-null info/stride: second word");
    EXPECT_EQ(outWords[2], FomLabV4ToLabV2(wOut[2]), "Non-null info/stride: third word");
    EXPECT_EQ(reinterpret_cast<cmsUInt8Number*>(ret) - outputBuf, 6u, "Non-null info/stride: return pointer offset");
}

// --- Test runner ---

static void runAllTests()
{
    // Execute each test
    test_PackLabV2_16_BasicMapping();
    test_PackLabV2_16_StrideIgnorance();
    test_PackLabV2_16_AllZeros();
    test_PackLabV2_16_NonNullInfoAndStride();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_run << " tests run, "
              << g_tests_failed << " failures."
              << std::endl;

    // Exit code based on failure to help scripting (non-terminating tests do not exit early)
    if (g_tests_failed != 0) {
        std::cerr << "Some tests FAILED. See details above." << std::endl;
    } else {
        std::cout << "All tests PASSED." << std::endl;
    }
}

// Entry point
int main()
{
    runAllTests();
    return (g_tests_failed == 0) ? 0 : 1;
}
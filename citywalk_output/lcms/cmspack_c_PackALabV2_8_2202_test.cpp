/*
  Unit test suite for PackALabV2_8 in cmspack.c
  - No GoogleTest; a lightweight test harness is used.
  - Tests rely on the library's existing internal macros and functions
    (e.g., FROM_16_TO_8, FomLabV4ToLabV2) declared in lcms2_internal.h.
  - The focus is on correctness of pointer arithmetic and 3-byte writes.
  - Static/internal helpers are kept minimal to respect the "no private access" constraint.
  - The tests are designed to be non-terminating (they report failures but continue).
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import the library's internal types and helpers.
// Assumes the environment provides lcms2_internal.h with all necessary definitions.

// Forward declarations to ensure linkage against the focal function.
// The exact signatures in the library use CMSREGISTER annotations; the macro
// CMSREGISTER expands to appropriate storage-class specifiers (e.g., register).
extern "C" cmsUInt8Number* PackALabV2_8(struct _cmsTRANSFORM* info,
                                      cmsUInt16Number wOut[],
                                      cmsUInt8Number* output,
                                      cmsUInt32Number Stride);

// The helper functions/macros are part of the library's internal API.
// We declare them here so we can compute expected values in tests.
extern "C" cmsUInt16Number FomLabV4ToLabV2(cmsUInt16Number x);
// FROM_16_TO_8 is a macro in the library; keeping the exact expansion via macro.
// We declare a usage so that the test can apply the same transformation for expected values.
extern "C" cmsUInt8Number FROM_16_TO_8(cmsUInt16Number x);

static int gTotalTestsRun = 0;
static int gTotalTestsFailed = 0;

// Simple non-terminating assertion macro.
// Logs failure but continues execution to maximize code coverage.
#define TEST_ASSERT(cond, msg)                               \
    do {                                                       \
        if (!(cond)) {                                         \
            std::cerr << "TEST FAIL: " << msg << std::endl;  \
            ++gTotalTestsFailed;                               \
        } else {                                               \
            std::cout << "TEST PASS: " << msg << std::endl;  \
        }                                                      \
        ++gTotalTestsRun;                                      \
    } while (0)

// Test 1: Basic contract test for PackALabV2_8
// - Verifies that exactly three bytes are written starting at buffer[1] to buffer[3].
// - Verifies that the function returns buffer + 4.
// - Verifies that buffer[0] remains unchanged (sentinel integrity).
static void test_PackALabV2_8_BasicContract() {
    // Test description:
    // Given three 16-bit input values, PackALabV2_8 should:
    // - Increment the output pointer once
    // - Write three 8-bit values (one for each input) at positions 1, 2, 3
    // - Return a pointer to the position after the last write (i.e., original+4)

    // Arrange
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x0F0F }; // sample inputs
    cmsUInt8Number buffer[8]; // ensure enough space around writes
    // Initialize sentinel to detect unintended writes
    std::memset(buffer, 0xAA, sizeof(buffer));

    struct _cmsTRANSFORM* info = nullptr; // info is unused in PackALabV2_8

    // Act
    cmsUInt8Number* ret = PackALabV2_8(info, wOut, buffer, 0);

    // Compute expected values using the library's own macros/functions
    cmsUInt8Number expected1 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[0]));
    cmsUInt8Number expected2 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[1]));
    cmsUInt8Number expected3 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[2]));

    // Assert
    TEST_ASSERT(ret == buffer + 4, "Return pointer should be buffer + 4");
    TEST_ASSERT(buffer[0] == 0xAA, "Buffer should keep initial sentinel at index 0");
    TEST_ASSERT(buffer[1] == expected1, "buffer[1] should equal FROM_16_TO_8(FomLabV4ToLabV2(wOut[0]))");
    TEST_ASSERT(buffer[2] == expected2, "buffer[2] should equal FROM_16_TO_8(FomLabV4ToLabV2(wOut[1]))");
    TEST_ASSERT(buffer[3] == expected3, "buffer[3] should equal FROM_16_TO_8(FomLabV4ToLabV2(wOut[2]))");
}

// Test 2: Verify behavior with different inputs and pointer arithmetic stability
// - Confirms that writing with another set of inputs yields appropriate bytes at positions 1..3
// - Ensures that the first byte (buffer[0]) remains unmodified.
static void test_PackALabV2_8_DifferentInputs() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0xFFFF, 0x0000, 0x7E7E };
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0x55, sizeof(buffer)); // different sentinel

    struct _cmsTRANSFORM* info = nullptr;

    // Act
    cmsUInt8Number* ret = PackALabV2_8(info, wOut, buffer, 0);

    // Expected using library's transforms
    cmsUInt8Number expected1 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[0]));
    cmsUInt8Number expected2 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[1]));
    cmsUInt8Number expected3 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[2]));

    // Assert
    TEST_ASSERT(ret == buffer + 4, "Return pointer should be buffer + 4 (Test 2)");
    TEST_ASSERT(buffer[0] == 0x55, "Buffer[0] should remain unchanged (0x55 sentinel) (Test 2)");
    TEST_ASSERT(buffer[1] == expected1, "buffer[1] should equal expected for wOut[0] (Test 2)");
    TEST_ASSERT(buffer[2] == expected2, "buffer[2] should equal expected for wOut[1] (Test 2)");
    TEST_ASSERT(buffer[3] == expected3, "buffer[3] should equal expected for wOut[2] (Test 2)");
}

// Test 3: Ensure that non-null info parameter does not affect output (static/ununsed propagation)
static void test_PackALabV2_8_InfoUnused() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 };
    cmsUInt8Number buffer[8];
    std::memset(buffer, 0x00, sizeof(buffer));

    // Create a dummy info object (contents do not matter due to cmsUNUSED_PARAMETER)
    struct _cmsTRANSFORM dummyInfo;
    std::memset(&dummyInfo, 0, sizeof(dummyInfo));

    // Act
    cmsUInt8Number* ret = PackALabV2_8(&dummyInfo, wOut, buffer, 0);

    // Expected
    cmsUInt8Number expected1 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[0]));
    cmsUInt8Number expected2 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[1]));
    cmsUInt8Number expected3 = FROM_16_TO_8(FomLabV4ToLabV2(wOut[2]));

    // Assert
    TEST_ASSERT(ret == buffer + 4, "Return pointer should be buffer + 4 (Test 3)");
    TEST_ASSERT(buffer[0] == 0x00, "Buffer[0] should remain at default (0x00) when not written (Test 3)");
    TEST_ASSERT(buffer[1] == expected1, "buffer[1] correctness with non-null info (Test 3)");
    TEST_ASSERT(buffer[2] == expected2, "buffer[2] correctness with non-null info (Test 3)");
    TEST_ASSERT(buffer[3] == expected3, "buffer[3] correctness with non-null info (Test 3)");
}

// Runner
static void runAllTests() {
    test_PackALabV2_8_BasicContract();
    test_PackALabV2_8_DifferentInputs();
    test_PackALabV2_8_InfoUnused();
}

int main() {
    // Print a short header
    std::cout << "Starting PackALabV2_8 unit tests (C++11)..." << std::endl;

    // Run tests
    // Each test prints PASS/FAIL and the main prints a summary.
    // Note: The tests are designed to be non-terminating on failure.
    test_PackALabV2_8_BasicContract();
    test_PackALabV2_8_DifferentInputs();
    test_PackALabV2_8_InfoUnused();

    // Summary
    int failed = gTotalTestsFailed;
    int total = gTotalTestsRun;
    std::cout << "Test summary: " << total << " tests, " << failed << " failures." << std::endl;
    return (failed == 0) ? 0 : 1;
}
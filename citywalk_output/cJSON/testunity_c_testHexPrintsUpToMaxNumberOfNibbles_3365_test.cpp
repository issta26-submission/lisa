/*
  Test suite for the focal method testHexPrintsUpToMaxNumberOfNibbles
  Context:
  - The focal test resides in testunity.c and is part of a Unity-based C test harness.
  - We are providing a C++11 compliant unit test harness (no GTest) that exercises the same underlying hex-printing behavior
    by exercising the dependent functions used by the focal test (spy I/O, hex printer, etc.).
  - This file exercises the hex printing pathway via the same "spy" interfaces (startPutcharSpy, endPutcharSpy, getBufferPutcharSpy)
    and the UnityPrintNumberHex function to verify that the result respects the maximum nibble width of UNITY_INT.

  Approach:
  - Step 2: Build two focused tests around testHexPrintsUpToMaxNumberOfNibbles semantics by exercising:
      1) When a large digits argument is provided (21), the printed length should still equal the full machine width (2 * sizeof(UNITY_INT)).
      2) When printing zero with a width equal to the full machine width, the buffer should contain all zeros and have the exact full width.
  - Step 3: Use domain knowledge of C++11, Unity, and the provided spy hooks to keep tests executable without GTest, while remaining faithful to the focal behavior.

  Important notes:
  - The test uses extern "C" declarations for the Unity hex printing spy hooks, so they can be linked from the C runtime used by Unity.
  - We avoid private API exposure; we interact only through the provided spy APIs and UnityPrintNumberHex.
  - The tests derive the expected length from 2 * sizeof(UNITY_INT) to adapt to the Unity type width automatically.

  Candidate Keywords referenced (for traceability):
  UnityPrintNumberHex, 0xBEE, 21, startPutcharSpy, endPutcharSpy, getBufferPutcharSpy, strlen, sizeof(UNITY_INT),
  TEST_ASSERT_EQUAL_INT, UNITY_INT, putchar spy, hex printing width, nibble calculation.

  The test file is self-contained and can be compiled with a standard C++11 toolchain that links against Unity and the C dependencies.
*/

#include <cstdint>
#include <cstring>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Declarations of the C-based hooks used by the focal test.
// These are provided by the Unity-based test framework (testunity.c) as static/extern hooks.
// We declare them with C linkage to avoid name mangling when compiling in C++.
extern "C" {
    void startPutcharSpy(void);
    void endPutcharSpy(void);
    char* getBufferPutcharSpy(void);
    // Unity's hex printing helper used by the focal test.
    void UnityPrintNumberHex(uint32_t value, int digits);
}

/*
  Test 1: Verifies that UnityPrintNumberHex prints up to the maximum number of nibbles
  corresponding to the machine width (UNITY_INT width) even if provided a larger digits parameter.

  Rationale:
  - The focal test calls the hex printer with a large digits parameter (21) and expects the output to be
    exactly as wide as the machine (2 * sizeof(UNITY_INT) hex digits).
  - We exercise the same path by invoking the spy-enabled hex printer directly and inspecting the buffer length.
*/
void test_UnityPrintNumberHex_MaxNibbles_PrintsFullWord_Length(void)
{
    // Arrange: prepare the spy and perform the hex print with an oversized digits parameter.
    startPutcharSpy();
    UnityPrintNumberHex(0xBEE, 21);  // Large digits parameter to exercise "up to max nibbles" behavior
    endPutcharSpy();

    // Act: retrieve the buffer and compute its length.
    const char* buffer = getBufferPutcharSpy();
    size_t actualLength = strlen(buffer);

    // The expected length is exactly 2 * sizeof(UNITY_INT) hex digits (full width of UNITY_INT).
    // This accommodates different widths of UNITY_INT on various platforms/configs.
    int expectedLength = static_cast<int>(2 * sizeof(UNITY_INT));

    // Assert: the length matches the full width, ensuring we don't print more than the machine width.
    TEST_ASSERT_EQUAL_INT(expectedLength, static_cast<int>(actualLength));

    // Optional: verify that the content consists of valid hex digits (not strictly required by focal test,
    // but reinforces correctness of the hex printer path).
    for (size_t i = 0; i < actualLength; ++i)
    {
        const char c = buffer[i];
        // Accept digits 0-9 and letters A-F (uppercase hex) as per Unity's hex printing conventions.
        TEST_ASSERT_TRUE((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
    }
}

/*
  Test 2: Verifies that printing zero with a width equal to the full UNITY_INT width results in
  a buffer of the exact full width containing all zeros.

  Rationale:
  - This mirrors behavior where padding ensures the hex output always occupies the full machine width.
  - We use a digits width equal to 2 * sizeof(UNITY_INT) to force full-width output, and then validate
    both length and content (all zeros).
*/
void test_UnityPrintNumberHex_ZeroValue_PadsToFullWidth_LengthAndContent(void)
{
    // Arrange: print zero with exact full-width digits to observe padding behavior
    startPutcharSpy();
    UnityPrintNumberHex(0, 2 * sizeof(UNITY_INT));  // Force full-width hex output
    endPutcharSpy();

    // Act: inspect the buffer
    const char* buffer = getBufferPutcharSpy();
    int actualLength = static_cast<int>(strlen(buffer));
    int expectedLength = static_cast<int>(2 * sizeof(UNITY_INT));

    // Assert: length should match full width
    TEST_ASSERT_EQUAL_INT(expectedLength, actualLength);

    // Assert: all characters should be '0' since value is zero and padding fills with zeros
    for (int i = 0; i < actualLength; ++i)
    {
        TEST_ASSERT_EQUAL_INT(static_cast<int>('0'), static_cast<int>(buffer[i]));
    }
}

/*
  Main runner for the test suite.
  - Calls Unity's RUN_TEST for each test function defined above.
  - This harness is designed to be compiled under C++11, without GTest, and linked with the same Unity runtime used by testunity.c.
*/
int main(void)
{
    UnityBegin("test_hex_print_suite_cpp.cpp");

    RUN_TEST(test_UnityPrintNumberHex_MaxNibbles_PrintsFullWord_Length);
    RUN_TEST(test_UnityPrintNumberHex_ZeroValue_PadsToFullWidth_LengthAndContent);

    return UnityEnd();
}
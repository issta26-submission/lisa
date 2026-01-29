/*
  Test Suite: getBufferPutcharSpy
  Focal method: char* getBufferPutcharSpy(void)
  Context: This unit test targets behavior when output spying is enabled via
  the USING_OUTPUT_SPY macro. It interacts with Unity's internal spy buffers:
  - putcharSpyBuffer: a buffer capturing spy output
  - indexSpyBuffer: current write index into the buffer
  Behavior verified:
  - With spying enabled, getBufferPutcharSpy returns a non-NULL buffer and a
    proper C-string terminated by '\0' at indexSpyBuffer.
  - The function writes a null terminator at the current indexSpyBuffer position.
  - When indexSpyBuffer is set to 0, the returned string should be empty.
  - When indexSpyBuffer is set to a non-zero value with pre-filled buffer content,
    getBufferPutcharSpy should return the content up to indexSpyBuffer-1.
  - When spying is disabled (compile without USING_OUTPUT_SPY), getBufferPutcharSpy
    should return NULL.
  This file uses Unity as the test framework and is intended to be compiled with
  C++11 (no GTest). It relies on Unity's C API and the external symbols
  putcharSpyBuffer and indexSpyBuffer defined in the Unity test harness.
*/

#include <cstring>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Forward declaration of the focal method under test (from testunity.c)
extern "C" char* getBufferPutcharSpy(void);

#ifdef USING_OUTPUT_SPY
// When output spy is active, these are defined by the Unity test harness.
// Expose them to allow test code to manipulate the spy buffer directly.
extern "C" char putcharSpyBuffer[];
extern "C" int  indexSpyBuffer;

// Optional: If Unity's default putcharSpy is weakly defined, we rely on Unity's
// integration and do not replace it here.
#endif

/* Candidate Keywords extracted from focal method (for Step 1 understanding)
   - getBufferPutcharSpy
   - USING_OUTPUT_SPY
   - putcharSpyBuffer
   - indexSpyBuffer
   - Unity test macros (TEST_ASSERT_*, RUN_TEST, etc.)
   - NULL / non-NULL return
   - buffer termination with '\\0'
*/

/* Test: When spy is active, initial call returns non-NULL and empty string
   - Assumes indexSpyBuffer is 0 by default.
   - Expect the first character to be '\0' after the call, indicating empty buffer. */
#ifdef USING_OUTPUT_SPY
void test_getBufferPutcharSpy_ReturnsBuffer_InitialEmpty(void)
{
    // Ensure starting state: empty buffer at position 0
    indexSpyBuffer = 0;
    putcharSpyBuffer[0] = '\0';

    char* buffer = getBufferPutcharSpy();

    // Verify buffer is non-NULL and represents an empty string
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_EQUAL_CHAR('\0', buffer[0]);
}
#endif

/* Test: With a pre-filled first character and index set to 1, getBufferPutcharSpy
   should return a string containing exactly that character. */
#ifdef USING_OUTPUT_SPY
void test_getBufferPutcharSpy_ReturnsContent_PrefilledFirstChar(void)
{
    // Pre-fill the buffer and set the current index to 1
    putcharSpyBuffer[0] = 'A';
    indexSpyBuffer = 1;
    // Do not modify position 1; function should append the terminator there
    char* buffer = getBufferPutcharSpy();

    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_EQUAL_STRING("A", buffer);
}
#endif

/* Test: With two pre-filled characters and index set to 2, expect "Hi" to be returned */
#ifdef USING_OUTPUT_SPY
void test_getBufferPutcharSpy_ReturnsContent_MultiChars(void)
{
    putcharSpyBuffer[0] = 'H';
    putcharSpyBuffer[1] = 'i';
    indexSpyBuffer = 2;

    char* buffer = getBufferPutcharSpy();

    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_EQUAL_STRING("Hi", buffer);
}
#endif

/* Test: When spy is not active, getBufferPutcharSpy should return NULL
   This test is compiled only when USING_OUTPUT_SPY is not defined. */
#ifndef USING_OUTPUT_SPY
void test_getBufferPutcharSpy_ReturnsNull_WhenSpyInactive(void)
{
    char* buffer = getBufferPutcharSpy();
    TEST_ASSERT_NULL(buffer);
}
#endif

/* Runner: Unity test harness main
   - Selects which tests to run based on the compile-time macro USING_OUTPUT_SPY. */
int main(void)
{
    UNITY_BEGIN();

#ifdef USING_OUTPUT_SPY
    // Tests for the active output spy scenario
    RUN_TEST(test_getBufferPutcharSpy_ReturnsBuffer_InitialEmpty);
    RUN_TEST(test_getBufferPutcharSpy_ReturnsContent_PrefilledFirstChar);
    RUN_TEST(test_getBufferPutcharSpy_ReturnsContent_MultiChars);
#else
    // Tests for the inactive output spy scenario
    RUN_TEST(test_getBufferPutcharSpy_ReturnsNull_WhenSpyInactive);
#endif

    return UNITY_END();
}
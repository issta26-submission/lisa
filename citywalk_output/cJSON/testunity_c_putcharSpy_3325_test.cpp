// Candidate Keywords extracted from the focal method putcharSpy and its dependencies:
// - putcharSpyBuffer, indexSpyBuffer, SPY_BUFFER_MAX
// - putcharSpyEnabled (controls whether to buffer or directly output)
// - getBufferPutcharSpy, startPutcharSpy, endPutcharSpy (test harness helpers)
// - putchar (external function invoked when not buffering)
// - c (the input character to putcharSpy)

/*
  This file provides a focused test suite for the focal method:
  void putcharSpy(int c)

  It targets:
  - When USING_OUTPUT_SPY is active in the test environment, buffer writes.
  - Buffer overflow protection (no writes beyond SPY_BUFFER_MAX-1).
  - The alternative path where, if the spy is disabled, putchar is used.

  Notes:
  - The tests assume the ordinary Unity test framework API.
  - The environment exposes:
      extern "C" void putcharSpy(int);
      extern "C" int putcharSpyEnabled;
      extern "C" int indexSpyBuffer;
      extern "C" char* getBufferPutcharSpy(void);
      void startPutcharSpy(void);
      void endPutcharSpy(void);
  - SPY_BUFFER_MAX is defined in the build configuration (macro).
  - For testing the non-buffer path, a test-specific override of putchar(...)
    is provided to capture the emitted character.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Focal function under test
void putcharSpy(int c);

// Global state expected by the focal method (defined in the test harness / production code)
extern int putcharSpyEnabled;
extern int indexSpyBuffer;

// Accessor to retrieve the current spy buffer content
extern char* getBufferPutcharSpy(void);

#ifdef __cplusplus
}
#endif

// If the test environment allows overriding standard putchar for the disabled path,
// provide a test hook to capture the emitted character.
// This relies on linker resolution preferring the test's definition over the stdio one.
static int lastPrintedChar = -1;
#ifdef __cplusplus
extern "C" {
#endif
// Forward declare the real stdio putchar as a weak symbol replacement guard (not strictly portable,
// but commonly used in Unity-based tests). If the test environment links stdio's putchar,
// this override will be replaced; otherwise the test will rely on the buffer path.
int putchar(int ch)
{
    // Capture the last character that would be printed via the spy's disabled path.
    lastPrintedChar = ch;
    return ch;
}

#ifdef __cplusplus
}
#endif

static void resetState(void)
{
    // Reset tracking state before each test
    lastPrintedChar = -1;
    // Note: indexSpyBuffer and putcharSpyEnabled are managed by individual tests
    // through the focal API and the test harness; no global reset is assumed here.
}

// Test 1: When the spy is enabled, putcharSpy should buffer up to SPY_BUFFER_MAX-1 chars.
void test_putcharSpy_stores_in_buffer_when_enabled(void)
{
    // Prepare spy environment
    startPutcharSpy();
    putcharSpyEnabled = 1;
    indexSpyBuffer = 0;

    // Exercise: push a sequence of characters
    putcharSpy('A');
    putcharSpy('B');
    putcharSpy('C');

    endPutcharSpy();

    // Verify: chars were stored in the buffer in order
    const char* buf = getBufferPutcharSpy();
    TEST_ASSERT_NOT_NULL(buf);
    TEST_ASSERT_EQUAL_CHAR('A', buf[0]);
    TEST_ASSERT_EQUAL_CHAR('B', buf[1]);
    TEST_ASSERT_EQUAL_CHAR('C', buf[2]);

    // Also verify that the index advanced accordingly
    TEST_ASSERT_EQUAL_INT(3, indexSpyBuffer);

    resetState();
}

// Test 2: Buffer should not overflow. If index SpyBuffer is at SPY_BUFFER_MAX-1, the
// function should not write beyond the buffer.
void test_putcharSpy_buffer_overflow_protection(void)
{
    startPutcharSpy();
    putcharSpyEnabled = 1;
    // Position the index at the last writeable slot (SPY_BUFFER_MAX-1)
    extern int SPY_BUFFER_MAX; // macro is defined by the build; reference for clarity
    indexSpyBuffer = SPY_BUFFER_MAX - 1;

    // This write should be ignored due to the boundary check
    putcharSpy('Z');
    endPutcharSpy();

    // Index should remain unchanged
    TEST_ASSERT_EQUAL_INT(SPY_BUFFER_MAX - 1, indexSpyBuffer);

    resetState();
}

// Test 3: Disabled spy path should call putchar for the given character.
// We rely on the test's override of putchar to capture the emitted character.
void test_putcharSpy_calls_putchar_when_disabled(void)
{
    startPutcharSpy();
    putcharSpyEnabled = 0; // disable buffering path
    indexSpyBuffer = 0;
    lastPrintedChar = -1;

    putcharSpy('X');
    endPutcharSpy();

    // Confirm that the override putchar was invoked with 'X'
    TEST_ASSERT_EQUAL_INT('X', lastPrintedChar);

    resetState();
}

// Unity-style test runner
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_putcharSpy_stores_in_buffer_when_enabled);
    RUN_TEST(test_putcharSpy_buffer_overflow_protection);
    RUN_TEST(test_putcharSpy_calls_putchar_when_disabled);

    return UNITY_END();
}
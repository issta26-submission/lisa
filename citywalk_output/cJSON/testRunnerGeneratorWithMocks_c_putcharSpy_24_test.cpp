/*
 * Test suite for the focal method: putcharSpy
 * Location: testRunnerGeneratorWithMocks.c
 *
 * Strategy:
 * - Redirect/monkey-patch putchar via a macro to a local mockPutchar function.
 * - The mock records the last character passed to putchar, allowing us to verify
 *   that putcharSpy forwards the exact input to putchar.
 * - Use Unity framework to structure and run tests without GTest.
 * - Provide a small set of tests covering typical characters (including newline).
 */

// Include necessary headers
#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <stdlib.h>
#include <CException.h>
#include <stdio.h>

#ifdef USE_CEXCEPTION
#endif

// Domain-specific note: We want to observe what putcharSpy forwards to putchar.
// We'll mock putchar by defining a local function and substituting the symbol
// via a preprocessor macro. This requires defining the macro before the
// focal method is compiled in this translation unit.

// Global to capture the character passed to putchar
static int g_lastPutcharChar;

// Mock implementation of putchar that records the character
static int mockPutchar(int ch)
{
    g_lastPutcharChar = ch;
    // Return value is not used by putcharSpy, but emulate standard signature
    return ch;
}

// Monkey-patch: replace calls to putchar with mockPutchar within this translation unit
#define putchar mockPutchar

// Focal method under test (as provided)
void putcharSpy(int c) { (void)putchar(c); }

// Unity setup/teardown stubs (as required by Unity)
void setUp(void) { }
void tearDown(void) { }

// Local helper to reset captured value
static void resetCapturedChar(void)
{
    g_lastPutcharChar = -1;
}

/*
 * Test 1: Verify that putcharSpy forwards a standard printable character to putchar.
 * Expectation: g_lastPutcharChar should equal the input character after the call.
 */
void test_putcharSpy_ForwardsPrintableCharacter(void)
{
    resetCapturedChar();
    putcharSpy('A');
    TEST_ASSERT_EQUAL_INT('A', g_lastPutcharChar);
}

/*
 * Test 2: Verify that putcharSpy forwards a different printable character to putchar.
 * This ensures the function isn't memoizing or hardcoding values.
 */
void test_putcharSpy_ForwardsAnotherPrintableCharacter(void)
{
    resetCapturedChar();
    putcharSpy('Z');
    TEST_ASSERT_EQUAL_INT('Z', g_lastPutcharChar);
}

/*
 * Test 3: Verify that putcharSpy forwards a newline character correctly.
 * Newlines are common boundary cases for stdout.
 */
void test_putcharSpy_ForwardsNewlineCharacter(void)
{
    resetCapturedChar();
    putcharSpy('\n');
    TEST_ASSERT_EQUAL_INT('\n', g_lastPutcharChar);
}

/*
 * Test 4: Verify that successive calls to putcharSpy each forward their respective values.
 * This checks that state is not leaked between invocations.
 */
void test_putcharSpy_SucceedsForMultipleCalls(void)
{
    // First call
    resetCapturedChar();
    putcharSpy('1');
    TEST_ASSERT_EQUAL_INT('1', g_lastPutcharChar);

    // Second call
    putcharSpy('2');
    TEST_ASSERT_EQUAL_INT('2', g_lastPutcharChar);
}

/*
 * Main test runner: register and execute tests
 * Note: Unity's RUN_TEST expects void test function(void).
 */
int main(void)
{
    UnityBegin("testRunnerGeneratorWithMocks.c");

    RUN_TEST(test_putcharSpy_ForwardsPrintableCharacter);
    RUN_TEST(test_putcharSpy_ForwardsAnotherPrintableCharacter);
    RUN_TEST(test_putcharSpy_ForwardsNewlineCharacter);
    RUN_TEST(test_putcharSpy_SucceedsForMultipleCalls);

    // End Unity test run and return result
    return UnityEnd();
}
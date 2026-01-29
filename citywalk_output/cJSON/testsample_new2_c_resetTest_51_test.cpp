// Test suite for the focal method resetTest located in testsample_new2.c
// This test suite uses the Unity C unit test framework (no GTest).
// The goal is to verify that resetTest correctly executes tearDown() and then setUp()
// inside its body, i.e., the inner call order: tearDown -> setUp.
// We instrument setUp/tearDown to emit markers to a small log buffer to observe the order.

#ifdef __cplusplus
extern "C" {
#endif

#include <stanky.h>
#include <unity.h>
#include <setjmp.h>
#include <funky.h>
#include <stdio.h>

// Declaration of the focal method under test (provided in testsample_new2.c)
void resetTest(void);

#ifdef __cplusplus
}
#endif

// We implement lightweight logging via a small global buffer.
// The outer test harness (Unity) will call setUp() before each test and tearDown()
// after each test. resetTest() itself will call tearDown() and setUp() again
// inside its body, which we want to observe via the log.
static int logBuffer[16];
static int logIndex = 0; // tracks how many log entries have been written

// Expose a simple log writer for setUp/tearDown.
// These functions are linked against the focal code's usage of setUp/tearDown.
void setUp(void)
{
    // Marker 10 indicates an outer setUp invocation (before the test body)
    logBuffer[logIndex++] = 10;
}

void tearDown(void)
{
    // Marker 20 indicates an outer tearDown invocation (after the test body)
    logBuffer[logIndex++] = 20;
}

// Test: verify inner call order inside resetTest
// Expected behavior:
// - Unity will call outer setUp() -> log: 10
// - resetTest() contains: tearDown(); setUp(); which will push 20 then 10
// - After the test, Unity calls the outer tearDown() -> log: 20
// Therefore the full log sequence should be: 10, 20, 10, 20
// We assert both the length and the precise sequence to confirm the inner call order.
void test_resetTest_inner_calls_order(void)
{
    // Act: invoke the focal method under test
    resetTest();

    // Assert: the log should reflect the exact sequence 10, 20, 10, 20
    // Note: logIndex should have exactly 4 entries after the test completes.
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, logIndex, "Expected log to contain 4 entries (outer setUp, inner tearDown, inner setUp, outer tearDown).");

    TEST_ASSERT_EQUAL_INT(10, logBuffer[0]); // outer setUp
    TEST_ASSERT_EQUAL_INT(20, logBuffer[1]); // inner tearDown
    TEST_ASSERT_EQUAL_INT(10, logBuffer[2]); // inner setUp
    TEST_ASSERT_EQUAL_INT(20, logBuffer[3]); // outer tearDown
}

// Lightweight main to run the test without GTest (as required)
int main(void)
{
    // Begin Unity test run
    UnityBegin("testsample_new2.c");

    // Run the focal test for resetTest
    RUN_TEST(test_resetTest_inner_calls_order, __LINE__);

    // End Unity test run
    return UnityEnd();
}
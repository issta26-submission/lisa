// testsample_run2.c
// A comprehensive test suite for the focal method resetTest using the Unity framework.
// The tests verify that resetTest performs a tearDown() followed by a setUp() in that order.
// The suite also checks integration behavior when run inside Unity's test flow.

#include <stanky.h>
#include <unity.h>
#include <setjmp.h>
#include <funky.h>
#include <stdio.h>


// Logging mechanism to observe call order of setUp/tearDown within tests.
static int log_len = 0;
#define LOG_CAP 32
enum { LOG_SETUP = 0, LOG_TEAR = 1 };
static int log_events[LOG_CAP];

// Record that setUp has been invoked (outer and inner).
static void record_setup(void)
{
    if (log_len < LOG_CAP) log_events[log_len++] = LOG_SETUP;
}

// Record that tearDown has been invoked (outer and inner).
static void record_tear(void)
{
    if (log_len < LOG_CAP) log_events[log_len++] = LOG_TEAR;
}

// hook points used by Unity-based tests
void setUp(void)
{
    // Record every invocation of setUp
    record_setup();
}

void tearDown(void)
{
    // Record every invocation of tearDown
    record_tear();
}

// Focal method under test, as provided.
// It should simply perform tearDown() followed by setUp() in that order.
void resetTest(void)
{
{
  tearDown();
  setUp();
}
}

// Ensure that we can call the focal method directly and observe its internal behavior.
void test_resetTest_direct_call(void)
{
    // Reset log and invoke the focal method directly (no Unity test harness wrapping here)
    log_len = 0;

    // Act
    resetTest();

    // Assert
    // We expect exactly two events: a TEAR followed by a SETUP
    TEST_ASSERT_EQUAL_INT(LOG_TEAR, log_events[0]);
    TEST_ASSERT_EQUAL_INT(LOG_SETUP, log_events[1]);
    TEST_ASSERT_EQUAL_INT(2, log_len);
}

// Integration test: Confirm the inner TEAR/SETUP sequence when resetTest is invoked
// as part of Unity's test flow (i.e., with an outer setUp before and an outer tearDown after).
void test_resetTest_integration_flow(void)
{
    // Reset log at the start of this test
    log_len = 0;

    // Act
    resetTest();

    // Assert
    // Expected full sequence under Unity harness: [SETUP (outer), TEAR (inner), SETUP (inner), TEAR (outer)]
    TEST_ASSERT_EQUAL_INT(LOG_SETUP, log_events[0]); // outer setup
    TEST_ASSERT_EQUAL_INT(LOG_TEAR,  log_events[1]); // inner tear
    TEST_ASSERT_EQUAL_INT(LOG_SETUP, log_events[2]); // inner setup
    TEST_ASSERT_EQUAL_INT(LOG_TEAR,  log_events[3]); // outer tear
    TEST_ASSERT_EQUAL_INT(4, log_len);
}

// Minimal Unity test runner. This file is compatible with C++11 compilers as well.
int main(void)
{
    UnityBegin("testsample_run2.c");

    // Run tests
    RUN_TEST(test_resetTest_direct_call, __LINE__);
    RUN_TEST(test_resetTest_integration_flow, __LINE__);

    return UnityEnd();
}
/*
 * Unit tests for the focal method resetTest in testsample_mock_def.c
 * This file provides a self-contained Unity/CMock-based test suite
 * that verifies the exact invocation order inside resetTest:
 *   CMock_Verify -> CMock_Destroy -> tearDown -> CMock_Init -> setUp
 *
 * The test instruments CMock_Init/Verify/Destroy, tearDown, and setUp
 * to log their invocation order so we can assert the presence of the
 * expected sequence without modifying the original focal method logic.
 *
 * Note: This test suite is written in C (compatible with C11) and uses
 * Unity/CMock as requested (no GTest usage).
 */

#include <unity.h>
#include <setjmp.h>
#include <cmock.h>
#include <funky.h>
#include <string.h>
#include <Mockstanky.h>
#include <stdio.h>


/* Forward declarations of setup/teardown used by Unity test lifecycle */
void setUp(void);
void tearDown(void);

/* ------------------------------------------------------------
 * Logging instrumentation to verify call order inside resetTest
 * ------------------------------------------------------------
 */
#define LOG_CMOCK_INIT     0
#define LOG_CMOCK_VERIFY   1
#define LOG_CMOCK_DESTROY  2
#define LOG_TEAR_DOWN      3
#define LOG_SET_UP         4

static int logbuf[256];
static int logpos = 0;

/* Reset the log buffer before each test invocation */
static void log_reset(void)
{
    logpos = 0;
    memset(logbuf, 0, sizeof(logbuf));
}

/* Record a log event with a single integer code */
static void log_event(int code)
{
    if (logpos < (int)(sizeof(logbuf) / sizeof(logbuf[0])))
    {
        logbuf[logpos++] = code;
    }
}

/* Prototyping for the instrumented CMock API used by resetTest */
static void CMock_Init(void)
{
    log_event(LOG_CMOCK_INIT);
}

static void CMock_Verify(void)
{
    log_event(LOG_CMOCK_VERIFY);
}

static void CMock_Destroy(void)
{
    log_event(LOG_CMOCK_DESTROY);
}

/* Instrument setUp/tearDown to capture Unity’s lifecycle events as well */
void setUp(void)
{
    log_event(LOG_SET_UP);
}

void tearDown(void)
{
    log_event(LOG_TEAR_DOWN);
}

/* ------------------------------------------------------------
 * The focal function under test (as provided) is not modified here;
 * we rely on its existing behavior which calls the instrumented
 * functions above in a predictable order:
 *   CMock_Verify(); CMock_Destroy(); tearDown(); CMock_Init(); setUp();
 * ------------------------------------------------------------
 */

/* Declaration of the focal function under test must be visible to this TU */
void resetTest(void);

/* Test case: verify the specific invocation order inside resetTest */
static void test_resetTest_sequence(void)
{
    /* Arrange: reset the log to capture only this test's sequence */
    log_reset();

    /* Act: execute the focal function under test */
    resetTest();

    /* Assert: search for the exact contiguous subsequence
       [CMock_Verify, CMock_Destroy, tearDown, CMock_Init, setUp] */
    const int pattern_len = 5;
    const int pattern[pattern_len] = {
        LOG_CMOCK_VERIFY,  /* CMock_Verify() invocation inside resetTest */
        LOG_CMOCK_DESTROY, /* CMock_Destroy() invocation inside resetTest */
        LOG_TEAR_DOWN,     /* tearDown() invocation inside resetTest */
        LOG_CMOCK_INIT,     /* CMock_Init() invocation inside resetTest */
        LOG_SET_UP            /* setUp() invocation inside resetTest */
    };

    int found = 0;
    for (int i = 0; i <= logpos - pattern_len; ++i)
    {
        int match = 1;
        for (int j = 0; j < pattern_len; ++j)
        {
            if (logbuf[i + j] != pattern[j])
            {
                match = 0;
                break;
            }
        }
        if (match)
        {
            found = 1;
            break;
        }
    }

    /* Use a non-fatal assertion-like approach? Unity's TEST_ASSERT_* are fatal on failure.
       We will use TEST_ASSERT_TRUE to clearly fail the test if the pattern isn't found. */
    TEST_ASSERT_TRUE_MESSAGE(found,
        "resetTest should invoke the sequence: CMock_Verify -> CMock_Destroy -> tearDown -> CMock_Init -> setUp in that order.");
}

/* Optional: A second test to ensure no false positives when log is empty */
static void test_resetTest_no_unexpected_sequence(void)
{
    /* Arrange: reset the log and do not call resetTest */
    log_reset();

    /* Act: simulate an empty run (no internal calls to the instrumented log) */
    /* Intentionally do nothing */

    /* Assert: ensure the specific sequence is not found when resetTest is not invoked */
    const int pattern_len = 5;
    const int pattern[pattern_len] = {
        LOG_CMOCK_VERIFY, LOG_CMOCK_DESTROY, LOG_TEAR_DOWN, LOG_CMOCK_INIT, LOG_SET_UP
    };
    int found = 0;
    for (int i = 0; i <= logpos - pattern_len; ++i)
    {
        int match = 1;
        for (int j = 0; j < pattern_len; ++j)
        {
            if (logbuf[i + j] != pattern[j])
            {
                match = 0;
                break;
            }
        }
        if (match)
        {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(!found,
        "Without invoking resetTest, the specific internal sequence should not appear in the log.");
}

/* ------------------------------------------------------------
 * Test runner
 * ------------------------------------------------------------
 */
int main(void)
{
    UnityBegin("testsample_mock_def.c");

    /* Run the test that validates the internal resetTest call sequence */
    RUN_TEST(test_resetTest_sequence, __LINE__);
    /* (Optional) Run an isolated check that the sequence does not appear without invoking resetTest */
    RUN_TEST(test_resetTest_no_unexpected_sequence, __LINE__);

    return UnityEnd();
}

/* End of test suite for resetTest */
// Test suite for the focal method resetTest() in testsample_def.c
// This test uses the Unity C unit-testing framework and is written in C++11-compatible syntax.
// The goal is to verify that resetTest() internally calls tearDown() followed by setUp(),
// by leveraging wrapper implementations of setUp/tearDown that log their invocation order.

#include <stanky.h>
#include <unity.h>
#include <setjmp.h>
#include <funky.h>
#include <stdio.h>


// Forward declaration of the function under test.
// The real implementation resides in testsample_def.c.
#ifdef __cplusplus
extern "C" {
#endif
void resetTest(void);
void setUp(void);
void tearDown(void);
#ifdef __cplusplus
}
#endif

// Logging mechanism to verify call order.
// We log events as integers to a simple circular-ish array.
static int logIndex = 0;
static int logSeq[64];

// Log an event value into the sequence
static void logEvent(int v) {
    if (logIndex < (int)(sizeof(logSeq) / sizeof(logSeq[0]))) {
        logSeq[logIndex++] = v;
    }
}

// Clear the log for a fresh test run
static void clearLog(void) {
    logIndex = 0;
    for (size_t i = 0; i < sizeof(logSeq) / sizeof(logSeq[0]); ++i) {
        logSeq[i] = 0;
    }
}

// Wrapper implementations for setUp/tearDown to capture outer vs inner invocations.
// In Unity, setUp() is called once before each test function and tearDown() after.
// Inside resetTest(), the code explicitly calls tearDown() then setUp(), representing inner calls.
// Thus the expected sequence for a single resetTest invocation is: 2 (outer setUp),
// 1 (inner tearDown), 2 (inner setUp), 1 (outer tearDown).

void setUp(void) {
    // 2 represents an outer setup invocation
    logEvent(2);
}

void tearDown(void) {
    // 1 represents an outer teardown invocation
    logEvent(1);
}

// Test 1: Ensure a single invocation of resetTest logs the sequence [2, 1, 2, 1]
void test_resetTest_order_once(void) {
    // Start fresh for this test; outer setUp (2) will still append to log after this call.
    clearLog();
    // Call the function under test
    resetTest();
    // Expected log sequence:
    // [2] outer setUp, [1] inner tearDown, [2] inner setUp, [1] outer tearDown
    TEST_ASSERT_EQUAL_INT(4, logIndex);
    TEST_ASSERT_EQUAL_INT(2, logSeq[0]); // outer setUp
    TEST_ASSERT_EQUAL_INT(1, logSeq[1]); // inner tearDown
    TEST_ASSERT_EQUAL_INT(2, logSeq[2]); // inner setUp
    TEST_ASSERT_EQUAL_INT(1, logSeq[3]); // outer tearDown
}

// Test 2: Ensure two consecutive invocations of resetTest produce a tail sequence of [2,1,2,1,2,1]
// Due to Unity's outer setUp/tearDown, the log will accumulate across tests. We reset the local log
// at the start of this test to start from a clean slate for the tail check.
void test_resetTest_order_twice(void) {
    clearLog();
    // Two successive invocations of resetTest
    resetTest();
    resetTest();

    // After two invocations, the inner calls produce two [1,2] sequences,
    // with an outer setUp before the first and an outer tearDown after the second.
    // So the last six events should be: [2,1,2,1,2,1]
    TEST_ASSERT_TRUE(logIndex >= 6);
    int tailStart = logIndex - 6;
    const int expected[6] = {2, 1, 2, 1, 2, 1};
    for (int i = 0; i < 6; ++i) {
        TEST_ASSERT_EQUAL_INT(expected[i], logSeq[tailStart + i]);
    }
}

// Main test runner for Unity
int main(void) {
    UNITY_BEGIN();
    // Line numbers chosen arbitrarily for the RUN_TEST macro.
    RUN_TEST(test_resetTest_order_once, 24);
    RUN_TEST(test_resetTest_order_twice, 42);
    return UNITY_END();
}
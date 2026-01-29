/*
 * Unit test suite for the focal method resetTest located in testsample_yaml.c
 * This test suite uses the Unity C unit testing framework (no GTest).
 * It instruments setUp/tearDown to verify the internal call sequence of resetTest:
 *   - resetTest should call tearDown() followed by setUp() in that order
 * The tests are designed to be framework-friendly and compile with C11.
 * The suite exercises the dependencies included in the focal class file.
 */

#include <three.h>
#include <stanky.h>
#include <four.h>
#include <unity.h>
#include <setjmp.h>
#include <funky.h>
#include <two.h>
#include <string.h>
#include <CException.h>
#include <stdio.h>


/* External focal function under test (provided in testsample_yaml.c) */
extern void resetTest(void);

/* Dependencies referenced by the focal file (as per <FOCAL_CLASS_DEP>) */

/* We provide our own setUp/tearDown to instrument call order inside resetTest.
   Unity's RUN_TEST macro will also call setUp/tearDown around each test, so
   nested calls inside resetTest will be observable in a combined log. */
static char log_buf[256][32];
static int log_len = 0;

/* Log a single event string (bounded to avoid overflow) */
static void log_event(const char* s)
{
  if (log_len < (int)(sizeof(log_buf) / sizeof(log_buf[0])))
  {
    strncpy(log_buf[log_len], s, 31);
    log_buf[log_len][31] = '\0';
    log_len++;
  }
}

/* Reset the log between tests */
static void reset_log(void)
{
  log_len = 0;
  // Optional: clear strings for clarity (not strictly necessary)
  for (int i = 0; i < 256; ++i)
  {
    log_buf[i][0] = '\0';
  }
}

/* Instrumented setUp and tearDown so that nested calls inside resetTest are logged.
   These will also be invoked by Unity's test harness around each test. */
void setUp(void)
{
  log_event("PRE_SETUP");
}
void tearDown(void)
{
  log_event("PRE_TEAR_DOWN");
}

/* Test 1: verify that resetTest invokes tearDown then setUp in that order
   within the body of resetTest. This is detected by looking for a consecutive
   "PRE_TEAR_DOWN" followed immediately by "PRE_SETUP" in the log. */
void test_resetTest_sequence_once(void)
{
  reset_log();

  /* Act: invoke the focal method. It should perform tearDown(); setUp(); inside its body. */
  resetTest();

  /* Assert: ensure we observed the nested call sequence TEAR_DOWN -> SETUP within the same invocation. */
  int found = 0;
  for (int i = 0; i < log_len - 1; ++i)
  {
    if (strcmp(log_buf[i], "PRE_TEAR_DOWN") == 0 &&
        strcmp(log_buf[i + 1], "PRE_SETUP") == 0)
    {
      found = 1;
      break;
    }
  }
  TEST_ASSERT_TRUE_MESSAGE(found, "resetTest should call tearDown() followed immediately by setUp() within its body.");
}

/* Test 2: verify that two consecutive invocations of resetTest each produce their own
   nested TEAR_DOWN -> SETUP sequence. We expect at least two such sequences overall. */
void test_resetTest_sequence_twice(void)
{
  reset_log();

  /* Act: invoke the focal method twice to ensure repeated nesting behavior. */
  resetTest();
  resetTest();

  /* Assert: count occurrences of the nested TEAR_DOWN -> SETUP pattern; should be >= 2. */
  int count = 0;
  for (int i = 0; i < log_len - 1; ++i)
  {
    if (strcmp(log_buf[i], "PRE_TEAR_DOWN") == 0 &&
        strcmp(log_buf[i + 1], "PRE_SETUP") == 0)
    {
      count++;
    }
  }
  TEST_ASSERT_TRUE_MESSAGE(count >= 2, "Two consecutive resetTest invocations should each produce a nested tearDown -> setUp sequence.");
}

/* Main entry point to run the Unity tests.
   If gtest isn't allowed, this pattern aligns with the instruction to call test methods from main. */
int main(void)
{
  UNITY_BEGIN();

  /* The line numbers passed here are placeholders for the RUN_TEST macro. */
  RUN_TEST(test_resetTest_sequence_once, 10);
  RUN_TEST(test_resetTest_sequence_twice, 20);

  return UNITY_END();
}
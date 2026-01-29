/*
   Lightweight C++11 test suite for the focal method:
   consoleClassifySetup in console_io.c

   Notes:
   - This test uses a non-GTest, plain C++ test harness.
   - We rely on the actual implementation inside console_io.c for
     the behavior of streamOfConsole and consoleRenewSetup.
   - The tests try to exercise both typical (non-tty) and terminal (tty) scenarios
     where available. Results that depend on the environment (e.g., terminal vs non-terminal)
     may vary; tests are designed to gracefully skip when necessary rather than fail.
   - All tests use non-terminating checks to maximize coverage (no program aborts on failure).
   - We assume 0 equals SAC_NoConsole (a common default for such enums); thus
     the expectation for the all-non-console path is rv == 0.
*/

#include <stdlib.h>
#include <stdio.h>


// Declare the focal function with C linkage for C++ test code.
// We fall back to a simple int return type assumption (SAC_NoConsole == 0).
#ifdef __cplusplus
extern "C" {
#endif
int consoleClassifySetup(FILE* pfIn, FILE* pfOut, FILE* pfErr);
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

#define TEST_FAIL(msg) do { \
    fprintf(stderr, "TEST_FAIL: %s (in %s:%d)\n", msg, __FILE__, __LINE__); \
    ++g_total; \
    ++g_failed; \
} while (0)

#define TEST_PASS(msg) do { \
    fprintf(stderr, "TEST_PASS: %s (in %s:%d)\n", msg, __FILE__, __LINE__); \
    ++g_total; \
} while (0)

#define ASSERT(cond, msg) do { \
    if(!(cond)) { TEST_FAIL(msg); } else { TEST_PASS(msg); } \
} while(0)

/*
  Test 1: All streams are non-console (typical non-tty environment for test suites).
  Expectation: consoleClassifySetup should report SAC_NoConsole for all streams,
  resulting in a return value rv == 0.
*/
static void test_AllNotConsole()
{
    FILE* pfIn  = tmpfile();
    FILE* pfOut = tmpfile();
    FILE* pfErr = tmpfile();

    if(!pfIn || !pfOut || !pfErr) {
        // If temporary files can't be created, skip this test gracefully.
        fprintf(stderr, "SKIP: Unable to create temporary test files for Test 1\n");
        if(pfIn) fclose(pfIn);
        if(pfOut) fclose(pfOut);
        if(pfErr) fclose(pfErr);
        return;
    }

    int rv = consoleClassifySetup(pfIn, pfOut, pfErr);

    // Expectation: rv corresponds to SAC_NoConsole (likely 0).
    (rv == 0) ? TEST_PASS("Test1_AllNotConsole returned 0 (no streams in console)") :
                TEST_FAIL("Test1_AllNotConsole did not return 0 (unexpected console flags)");

    // Clean up
    fclose(pfIn);
    fclose(pfOut);
    fclose(pfErr);
}

/*
  Test 2: Try to use terminal streams if accessible.
  On POSIX systems, /dev/tty often represents the controlling terminal.
  If accessible, we expect at least one stream to be detected as a console
  and rv to be non-zero (some SAC_InConsole bits set).
  If /dev/tty is not available, skip this test gracefully.
*/
static void test_AllConsolePossible()
{
#if defined(__unix__) || defined(__APPLE__)
    FILE* inTTY  = fopen("/dev/tty", "r");
    FILE* outTTY = fopen("/dev/tty", "w");
    FILE* errTTY = fopen("/dev/tty", "w");

    if(inTTY && outTTY && errTTY) {
        int rv = consoleClassifySetup(inTTY, outTTY, errTTY);

        if(rv != 0) {
            TEST_PASS("Test2_AllConsolePossible rv non-zero (some streams detected as console)");
        } else {
            TEST_FAIL("Test2_AllConsolePossible rv unexpectedly zero (no streams detected as console)");
        }

        fclose(inTTY);
        fclose(outTTY);
        fclose(errTTY);
        return;
    } else {
        // If any open failed, skip gracefully.
        fprintf(stderr, "SKIP: /dev/tty not accessible; skipping Test 2\n");
        if(inTTY) fclose(inTTY);
        if(outTTY) fclose(outTTY);
        if(errTTY) fclose(errTTY);
        return;
    }
#else
    fprintf(stderr, "SKIP: Test 2 not supported on this platform\n");
#endif
}

/*
  Test 3: Mixed scenario (one or more streams treated as console if available).
  We attempt to combine a non-tty with a tty-like stream to force mixed
  behavior if possible.
*/
static void test_MixedScenario()
{
#if defined(__unix__) || defined(__APPLE__)
    FILE* inTmp  = tmpfile();
    FILE* outTTY = fopen("/dev/tty", "w"); // may be NULL if not accessible
    FILE* errTmp = tmpfile();

    if(inTmp && errTmp) {
        if(outTTY) {
            int rv = consoleClassifySetup(inTmp, outTTY, errTmp);
            if(rv != 0) {
                TEST_PASS("Test3_MixedScenario rv non-zero (mixed console inference)");
            } else {
                TEST_FAIL("Test3_MixedScenario rv zero (unexpected no-console in mixed scenario)");
            }
            fclose(inTmp);
            fclose(errTmp);
            fclose(outTTY);
            return;
        } else {
            // outTTY not available; skip gracefully
            fprintf(stderr, "SKIP: /dev/tty not accessible for Test 3\n");
            fclose(inTmp);
            fclose(errTmp);
            return;
        }
    } else {
        fprintf(stderr, "SKIP: Unable to create temporary test files for Test 3\n");
        if(inTmp) fclose(inTmp);
        if(errTmp) fclose(errTmp);
        return;
    }
#else
    fprintf(stderr, "SKIP: Test 3 not supported on this platform\n");
#endif
}

/* Main: run all tests and report summary. */
int main(void)
{
    fprintf(stderr, "Starting consoleClassifySetup test suite (no-GTest, C++11)\n");

    test_AllNotConsole();
    test_AllConsolePossible();
    test_MixedScenario();

    fprintf(stderr, "\nTest Summary: total=%d, failed=%d\n", g_total, g_failed);

    // Return non-zero if any test failed.
    return (g_failed != 0) ? 1 : 0;
}
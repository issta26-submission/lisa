// Threaded test suite for the focal method: Comparative
// This suite targets the Comparative(const char* Title, perf_fn fn1, perf_fn fn2, const char* inICC, const char* outICC)
// which simply delegates to ComparativeCt(0, 0, ...).
//
// Approach:
// - Provide two dummy perf_fn implementations that increment global counters when invoked.
// - Invoke Comparative with various inputs and verify via non-terminating assertions that the perf functions
//   are (at least) exercised during the call(er).
// - Include explanatory comments for each test case.
// - Use a lightweight, self-contained test harness (no GTest) and run tests from main().
//
// Notes:
// - This test relies on the threaded_internal.h header for types like cmsContext, cmsHPROFILE and perf_fn.
// - We avoid private/static internals; we only exercise the public entry point Comparative.
// - The tests are designed to be compilable under C++11 and linkable with the library that provides the implementation
//   of ComparativeCt and related helpers.

// This test file is written to be compiled with C++11, but the code under test is C.
// Hence, we wrap the header include in extern "C" when compiling as C++.

#include <cstring>
#include <cstdio>
#include <threaded_internal.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>


extern "C" {
}

// Global counters to verify that the perf_fn callbacks are invoked during Comparative/ComparativeCt
static volatile int g_perfA_calls = 0;
static volatile int g_perfB_calls = 0;

// Dummy performance function A: matches perf_fn signature (assumed: cmsContext, cmsHPROFILE, cmsHPROFILE)
static cmsFloat64Number DummyPerfA(cmsContext ct, cmsHPROFILE in, cmsHPROFILE out)
{
    // Suppress unused parameter warnings
    (void)ct;
    (void)in;
    (void)out;

    // Increment counter to indicate invocation
    ++g_perfA_calls;
    // Return an arbitrary speed/metric
    return 1.0;
}

// Dummy performance function B: matches perf_fn signature (assumed: cmsContext, cmsHPROFILE, cmsHPROFILE)
static cmsFloat64Number DummyPerfB(cmsContext ct, cmsHPROFILE in, cmsHPROFILE out)
{
    (void)ct;
    (void)in;
    (void)out;

    ++g_perfB_calls;
    return 2.0;
}

// Simple non-terminating test assertion macro.
// It prints an error message but does not abort the program, allowing multiple tests to run.
#define TEST_EXPECT(condition, message)                                \
    do {                                                                 \
        if (!(condition)) {                                              \
            std::fprintf(stderr, "TEST FAILED: %s\n", message);          \
            ++g_tests_failed;                                          \
        } else {                                                         \
            ++g_tests_passed;                                          \
        }                                                                \
    } while (0)

// Simple test counters
static int g_tests_total = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Test 1: Basic invocation - Ensure Comparative calls into the provided perf_fn callbacks
// Expected: At least one of the two perf_fn callbacks should have been invoked during the Comparative call.
static void test_Comparative_basic_invocation()
{
    // Reset counters
    g_perfA_calls = 0;
    g_perfB_calls = 0;

    // Invoke the focal function with two dummy perf functions and some ICC strings
    Comparative("Test_Basic_Invocation", DummyPerfA, DummyPerfB, "inICC_Basic", "outICC_Basic");

    // Record test result
    ++g_tests_total;
    // We expect that at least one of the perf functions was called to demonstrate invocation path
    TEST_EXPECT((g_perfA_calls > 0) || (g_perfB_calls > 0),
                "Comparative did not invoke any perf_fn callbacks in test_Comparative_basic_invocation");
}

// Test 2: Multi-call scenario - Call Comparative multiple times to exercise multiple invocations
// This increases the likelihood that both perf_fn callbacks are exercised across calls.
static void test_Comparative_multiple_calls_exercise_both()
{
    // Reset counters
    g_perfA_calls = 0;
    g_perfB_calls = 0;

    // First invocation
    Comparative("Test_Multi_Call_1", DummyPerfA, DummyPerfB, "inICC_M1", "outICC_M1");

    // Second invocation
    Comparative("Test_Multi_Call_2", DummyPerfA, DummyPerfB, "inICC_M2", "outICC_M2");

    ++g_tests_total;
    TEST_EXPECT((g_perfA_calls > 0) && (g_perfB_calls > 0),
                "Comparative did not invoke both perf_fn callbacks across multiple calls (test_Comparative_multiple_calls_exercise_both)");
}

// Test 3: Edge case with empty Title and ICC strings
// Verifies that function handles empty strings gracefully and still invokes callbacks if internal logic permits.
static void test_Comparative_empty_strings()
{
    // Reset counters
    g_perfA_calls = 0;
    g_perfB_calls = 0;

    Comparative("", DummyPerfA, DummyPerfB, "", "");

    ++g_tests_total;
    TEST_EXPECT((g_perfA_calls > 0) || (g_perfB_calls > 0),
                "Comparative did not invoke perf_fn callbacks with empty strings (test_Comparative_empty_strings)");
}

// Test runner: executes all tests and reports summary
static void run_all_tests()
{
    test_Comparative_basic_invocation();
    test_Comparative_multiple_calls_exercise_both();
    test_Comparative_empty_strings();
}

// Entry point
int main()
{
    // Run tests
    run_all_tests();

    // Print summary
    std::printf("Test Summary: total=%d, passed=%d, failed=%d\n",
                g_tests_total, g_tests_passed, g_tests_failed);

    // Exit code: 0 if any test failed
    return (g_tests_failed > 0) ? 1 : 0;
}

// Notes for test developers:
// - The tests rely on the presence of the function Comparative and the underlying ComparativeCt
//   provided by threaded_internal.h (as per the focal class dependencies).
// - We purposely use only public entry points and do not touch private/static internals.
// - perf_fn is assumed to have the signature: cmsFloat64Number perf_fn(cmsContext, cmsHPROFILE, cmsHPROFILE).
//   If your environment uses a different signature, adjust DummyPerfA/B accordingly and recompile.
// - The tests implement a lightweight harness without any external testing framework (no GTest/GMock).
// - Static helpers or functions defined in threaded_internal.h that are not visible outside their TU
//   remain inaccessible; tests only rely on the public Comparative entry point.
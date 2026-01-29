// This test suite targets the UnityMain entry point inside unity_fixture.c.
// It uses a small, self-contained C++ test harness (no GoogleTest) with
// non-terminating assertions to maximize coverage of the Unity framework glue.
// The tests focus on the behavior of UnityMain related to RepeatCount and the
// runAllTests callback, leveraging the public UnityFixture globals exposed by
// unity_fixture.h (e.g., UnityFixture.RepeatCount, UnityFixture.Verbose) and
// the public Unity global (e.g., Unity.TestFailures).
//
// Test design notes:
// - Test 1: Ensure UnityMain calls the provided runAllTests exactly RepeatCount times.
// - Test 2: Ensure UnityMain returns the value of Unity.TestFailures after the run.
// - Test 3: Ensure when RepeatCount is 0, runAllTests is never invoked.
// - All tests are non-terminating: they log failures but continue execution.
//
// Important: This test suite assumes the project is buildable with C++11 and that
// unity_fixture.h exposes externs for Unity and UnityFixture. It links against
// the C Unity code (unity_fixture.c, etc.) without using GTest.

#include <unity_internals.h>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unity_fixture.h>


extern "C" {
    // Prototypes for Unity's C entry point and related globals exposed by the
    // Unity framework headers included in the project.
    int UnityMain(int argc, const char* argv[], void (*runAllTests)(void));
    // The Unity framework will declare extern Unity_t Unity; and the fixture
    // globals like struct UNITY_FIXTURE_T UnityFixture; via unity_fixture.h.
    // We still declare the struct to ensure we can reference its fields if needed.
    // The actual type details come from the included header.
}

// Bring in the UnityFixture and Unity globals from the project's headers.
// Use extern to avoid name mangling issues in C++.
extern "C" {
}

// Global counters to verify test expectations from within the test harness.
static int g_runAllTests_calls = 0;
static int runAllTests_behaviour = 0; // 0: default (no special behaviour), 1: force TestFailures=7

// Expose a C linkage function that UnityMain will call as the test runner.
// This function increments a counter each time it's invoked, and can be guided
// to alter Unity.TestFailures for specific tests.
extern "C" void runAllTests(void)
{
    ++g_runAllTests_calls;
    if (runAllTests_behaviour == 1)
    {
        // Force UnityMain to return a known failure count, enabling coverage of the
        // return path (return value equals Unity.TestFailures at end of loop).
        // Unity.TestFailures is part of Unity's public state; assign directly.
        // The header unity_fixture.h provides the declaration for Unity.
        Unity.TestFailures = 7;
    }
}

// Lightweight test harness (non-terminating assertions).
static int g_total_tests = 0;
static int g_total_failures = 0;

#define LOG(...) printf(__VA_ARGS__)
#define ASSERT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_total_failures; \
        LOG("ASSERT_TRUE failed: %s (%s:%d)\n", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    ++g_total_tests; \
    if(((long)expected) != ((long)actual)) { \
        ++g_total_failures; \
        LOG("ASSERT_EQ failed: expected %ld, got %ld (%s:%d)\n", (long)expected, (long)actual, __FILE__, __LINE__); \
    } \
} while(0)

// Individual test cases

// Test 1: UnityMain should call runAllTests RepeatCount times.
static void test_UnityMain_CallsRunAllTests_CorrectTimes()
{
    // Reset test harness state
    g_runAllTests_calls = 0;
    runAllTests_behaviour = 0;

    // Configure UnityFixture for RepeatCount = 3
    UnityFixture.RepeatCount = 3;

    // Minimal argv: argc=1, argv[0] is program name
    const int argc = 1;
    const char* argv[] = { "unity_main_test_program" };

    // Call UnityMain with our runAllTests callback
    int result = UnityMain(argc, argv, runAllTests);

    // Expect runAllTests to have been invoked exactly 3 times
    ASSERT_EQ(3, g_runAllTests_calls);

    // Result is not asserted strictly here, but we log it for visibility
    LOG("test_UnityMain_CallsRunAllTests_CorrectTimes: UnityMain returned %d\n", result);
}

// Test 2: UnityMain should return the current Unity.TestFailures value after the loop.
// We use runAllTests behaviour to set Unity.TestFailures = 7.
static void test_UnityMain_ReturnsValueFromRunAllTests()
{
    g_runAllTests_calls = 0;
    runAllTests_behaviour = 1; // instruct runAllTests to set Unity.TestFailures

    // RepeatCount = 1 to simplify
    UnityFixture.RepeatCount = 1;

    const int argc = 1;
    const char* argv[] = { "unity_main_test_program" };

    int result = UnityMain(argc, argv, runAllTests);

    // Expect UnityMain to propagate Unity.TestFailures as the return value
    ASSERT_EQ(7, result);
    // And ensure runAllTests was called exactly once
    ASSERT_EQ(1, g_runAllTests_calls);
}

// Test 3: When RepeatCount is 0, runAllTests should not be called at all.
static void test_UnityMain_RepeatCountZero_NoRun()
{
    g_runAllTests_calls = 0;
    runAllTests_behaviour = 0;
    UnityFixture.RepeatCount = 0;

    const int argc = 1;
    const char* argv[] = { "unity_main_test_program" };

    int result = UnityMain(argc, argv, runAllTests);

    // No iterations should occur; runAllTests should not be invoked
    ASSERT_EQ(0, g_runAllTests_calls);
    // Result is whatever Unity's test failures state is; we don't require a specific value here
    LOG("test_UnityMain_RepeatCountZero_NoRun: UnityMain returned %d\n", result);
}

// Entry point for the test suite (mini harness)
int main()
{
    // Run tests
    test_UnityMain_CallsRunAllTests_CorrectTimes();
    test_UnityMain_ReturnsValueFromRunAllTests();
    test_UnityMain_RepeatCountZero_NoRun();

    // Summary
    printf("Test Summary: total=%d, failures=%d\n", g_total_tests, g_total_failures);
    if (g_total_failures > 0) {
        printf("Status: FAILED\n");
        return 1;
    } else {
        printf("Status: PASSED\n");
        return 0;
    }
}
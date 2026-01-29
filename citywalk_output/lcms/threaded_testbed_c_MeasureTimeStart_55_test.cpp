// Candidate Keywords derived from the focal method and its surrounding dependencies
// - timespec_get
// - clock_gettime
// - timespec (start variable type)
// - TIME_UTC
// - CLOCK_MONOTONIC
// - HAVE_TIMESPEC_GET (preprocessor branch)
// - MeasureTimeStart (function under test)
// - MeasureTimeStop (paired function under test)
// - cmsINLINE (inline macro)
// - start / stop (timer storage, global in the module)
// - cmsContext / cmsUInt32Number (types in the surrounding file, for compatibility)
// These keywords guide the test intent: branch coverage, time measurement, and integration with the provided timing utilities.

// Include the focal header/source declarations for the timing functions under test.
// The actual implementation is expected to be in the project under test with the same API.
// We declare them with C linkage to ensure correct linkage with the C-style implementation.
#include <thread>
#include <cstdint>
#include <vector>
#include <chrono>
#include <threaded_internal.h>
#include <iostream>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


extern "C" void MeasureTimeStart(void);
extern "C" double MeasureTimeStop(void);

// NOTE: Depending on the build, the internal timing infrastructure may be in C.
// The test harness uses public API: MeasureTimeStart and MeasureTimeStop.
// We do not rely on accessing internal static variables from the focal file.


// Simple portable sleep for test timing (milliseconds)
static void sleep_ms(int ms)
{
    // C++11 standard library sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Simple non-terminating assertion framework
// Keeps track of failures but continues test execution
static int g_failures = 0;

#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[ASSERT FAIL] " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_NEAR(actual, expected, tol, msg) do { \
    if(!((std::abs((actual) - (expected)) <= (tol)))) { \
        std::cerr << "[ASSERT NEAR] " << (msg) \
                  << " | actual=" << (actual) << " expected=" << (expected) \
                  << " tol=" << (tol) << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test Suite: MeasureTimeStart / MeasureTimeStop
// Purpose: Validate basic timing functionality exposed by the focal method pair.
// Coverage goals include:
// - That MeasureTimeStart followed by MeasureTimeStop yields a non-negative elapsed time.
// - That the function handles real waiting periods (e.g., ~50ms) producing a plausible elapsed time.
// - That repeated usage does not crash or produce negative results under typical operation.

struct TestSuite {
    using TestFn = void(*)();

    struct TestCase {
        const char* name;
        TestFn fn;
    };

    std::vector<TestCase> cases;

    void add(const char* name, TestFn fn) {
        cases.push_back({name, fn});
    }

    void run_all() {
        int total = (int)cases.size();
        int passed = 0;

        for (auto &tc : cases) {
            // Run each test case in isolation (as much as possible)
            g_failures = 0; // reset per test
            tc.fn();
            bool ok = (g_failures == 0);
            std::cout << "[" << (ok ? "OK" : "FAIL") << "] " << tc.name << "\n";
            if (ok) ++passed;
        }

        std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
    }
};


// Test 1: Basic timing after a known sleep duration ~50ms
// Expectation: The elapsed time should be non-trivial (roughly the sleep duration) and non-negative.
// This exercises the branch where the timing is sourced from either timespec_get or clock_gettime
// depending on the compile-time macro, but validates observable behavior of the API.
static void test_basic_timing_after_sleep()
{
    // Start timer
    MeasureTimeStart();
    // Sleep for ~50 ms to create a measurable time window
    sleep_ms(50);
    // Stop timer and capture elapsed time
    double elapsed = MeasureTimeStop();

    // Allow some tolerance due to scheduler jitter and timer resolution
    const double minExpected = 0.040; // 40 ms
    const double maxExpected = 0.30;  // 300 ms (reasonable upper bound for test environment)

    ASSERT(elapsed >= minExpected && elapsed <= maxExpected,
           "Elapsed time after ~50ms sleep should be within expected range");
    // Also check non-negativity as a basic invariant
    ASSERT(elapsed >= 0.0, "Elapsed time should be non-negative");
}

// Test 2: Repeated fast usage to ensure no crash and non-negative results
// This test stresses the API by performing multiple quick Start/Stop cycles with small sleeps.
// It helps verify robustness of the timing machinery across successive invocations.
static void test_multiple_quick_measurements()
{
    const int iterations = 100;
    for (int i = 0; i < iterations; ++i) {
        MeasureTimeStart();
        // Very short sleep to simulate quick usage while still yielding measurable time
        sleep_ms(2 + (i % 3)); // 2-4 ms
        double elapsed = MeasureTimeStop();
        // We only require non-negativity and finite value
        ASSERT(elapsed >= 0.0 && std::isfinite(elapsed),
               "Elapsed time from quick measurement should be non-negative and finite");
    }
}

// Test 3: No-crash stress test with varying wait times
// Ensures that under a mix of sleeps, the API remains stable and returns a plausible non-negative value.
// This complements the previous tests by introducing more variability.
static void test_stress_with_varied_delays()
{
    // A sequence of varied delays (in ms)
    const int delays[] = {1, 5, 10, 3, 7, 15, 20, 8, 12, 25};
    const int N = sizeof(delays) / sizeof(delays[0]);

    for (int i = 0; i < N; ++i) {
        MeasureTimeStart();
        sleep_ms(delays[i]);
        double elapsed = MeasureTimeStop();
        // Basic sanity: non-negative and finite
        ASSERT(elapsed >= 0.0 && std::isfinite(elapsed),
               "Elapsed time with varied delay should be non-negative and finite");
    }
}


// Importantly, we expose the TestSuite and tests to a main() that will run them.
// The tests avoid private/internal dependencies and rely only on the public API
// (MeasureTimeStart/MeasureTimeStop) to maximize coverage of the focal method's behavior.

int main()
{
    TestSuite suite;

    suite.add("MeasureTimeStart/MeasureTimeStop: basic timing after sleep ~50ms", test_basic_timing_after_sleep);
    suite.add("MeasureTimeStart/MeasureTimeStop: multiple quick measurements", test_multiple_quick_measurements);
    suite.add("MeasureTimeStart/MeasureTimeStop: stress with varied delays", test_stress_with_varied_delays);

    // Run the test suite
    suite.run_all();

    // Return non-zero if any test assertions failed
    return (g_failures == 0) ? 0 : 1;
}
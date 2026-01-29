// C++11 test harness for focal method SpeedTest15bitsRGBA
// This test uses the project's existing C code (fast_float_testbed.c) and
// invokes SpeedTest15bitsRGBA with real ICC profiles loaded via loadProfile.
// The test harness is designed to be compiled alongside the C file without a
// separate GTest framework. A lightweight static initializer-based test runner
// is used to execute tests before main() starts.
//
// Notes:
// - We rely on real ICC profiles (e.g., sRGB.icc) available at runtime.
// - No private members/methods of the focal code are accessed.
// - All tests are non-terminating in the sense that failures are reported but
//   do not abruptly abort the process (using a small custom checker).

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Lightweight test harness state
static int g_totalTests = 0;
static int g_failedTests = 0;

// Simple assertion helper that reports but does not terminate the program
static void reportFail(const char* where, const char* msg) {
    std::fprintf(stderr, "TEST FAILED at %s: %s\n", where, msg);
    g_failedTests++;
    g_totalTests++;
}

// Simple assertion for boolean conditions with message
static void expect_true(bool cond, const char* where, const char* msg) {
    if (!cond) {
        reportFail(where, msg);
    } else {
        g_totalTests++;
    }
}

// Typedefs to match the project's expected opaque types (we treat them as opaque)
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef double cmsFloat64Number;

// Forward declarations of focal method and dependencies (from the project's C sources)
// We assume the following external C interfaces exist in the linked C library.
extern "C" {
    // Focal method under test
    cmsFloat64Number SpeedTest15bitsRGBA(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

    // Helper to load ICC profiles from disk (provided in the C file/class deps)
    cmsHPROFILE loadProfile(const char* name);

    // Profile lifecycle (also provided by the library)
    void cmsCloseProfile(cmsHPROFILE h);
}

// A small helper to interpret the result as finite
static bool isFiniteDouble(double v) {
    return std::isfinite(v);
}

// The test runner is constructed before main() to ensure tests execute.
// It reports results to stdout/stderr and persists across the program lifetime.
struct TestRunner {
    TestRunner() {
        // Run tests
        test_SpeedTest15bitsRGBA_ValidProfiles();
        // Summary
        std::printf("TEST SUMMARY: total=%d, failed=%d\n", g_totalTests, g_failedTests);
        // If any test failed, do not interfere with exit code (exit handling can be done externally)
    }

    // Test 1: Verify SpeedTest15bitsRGBA runs with valid input/output profiles
    // and returns a non-negative, finite speed metric (MPixSec).
    void test_SpeedTest15bitsRGBA_ValidProfiles() {
        std::printf("Running test_SpeedTest15bitsRGBA_ValidProfiles...\n");

        // Attempt to load input and output profiles from disk.
        // The environment is expected to provide sRGB ICC profiles (or similar).
        const char* inProfilePath  = "sRGB.icc";   // Path must be resolvable at runtime
        const char* outProfilePath = "sRGB.icc";   // Re-use same for a simple case

        cmsHPROFILE inProf  = loadProfile(inProfilePath);
        cmsHPROFILE outProf = loadProfile(outProfilePath);
        cmsContext ct = nullptr; // Use default context for this test

        // Check that profiles were loaded successfully
        expect_true(inProf != nullptr, "LoadProfile(In)", "Input profile loading failed (nullptr).");
        expect_true(outProf != nullptr, "LoadProfile(Out)", "Output profile loading failed (nullptr).");

        if (inProf != nullptr && outProf != nullptr) {
            // Invoke the focal method
            cmsFloat64Number result = SpeedTest15bitsRGBA(ct, inProf, outProf);

            // Validate the result is finite and non-negative
            bool finite = isFiniteDouble((double)result);
            expect_true(finite, "SpeedTest15bitsRGBA", "Returned value is not finite.");

            bool nonNeg = (double)result >= 0.0;
            expect_true(nonNeg, "SpeedTest15bitsRGBA", "Returned value is negative unexpectedly.");

            // Optional: print the value for diagnostic purposes
            std::printf("SpeedTest15bitsRGBA returned MPix/sec = %.6f\n", (double)result);
        }

        // Cleanup
        if (inProf)  cmsCloseProfile(inProf);
        if (outProf) cmsCloseProfile(outProf);
    }
};

// Instantiate the test runner to execute tests at load time
static TestRunner g_testRunner;

// End of test harness
// The project's existing main() in the C file will be used when linking.
// This file intentionally contains no main() to avoid conflicting entry points.
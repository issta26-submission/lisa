/*
Step 1 - Candidate Keywords (from the focal method SpeedTest15)
The following keywords represent the core dependent components used inside SpeedTest15:
- trace, PerformanceHeader, Performance
- SpeedTest15bitsRGB, SpeedTest15bitsCMYK
- PROFILES_DIR, test5.icc, test3.icc, test0.icc, test1.icc, test2.icc
- Scanline_rgb15bits, Scanline_rgba15bits
- "*curves"
These elements are the primary interactions that the focal method uses to perform its 15-bit performance tests.

Step 2 - Unit Test Suite Outline
The focal implementation is a C-style function that invokes a few other functions (trace, PerformanceHeader, Performance, SpeedTest15bitsRGB, SpeedTest15bitsCMYK) with various ICC profile paths. The provided class dependencies show these are simple, likely no-op stubs in this environment, so the unit tests can focus on ensuring:
- SpeedTest15 executes without crashing.
- SpeedTest15 can be invoked repeatedly without adverse effects (basic stability/consistency check).

Because the code is provided as plain C in a C/C++ mixed project, the tests are written in C++11 without any external test frameworks (as GTest is not allowed). We expose the focal function via C linkage and invoke it from a small test harness.

Step 3 - Refinement and Coverage Perspective
Given the absence of explicit branching inside SpeedTest15 (it's a sequence of Performance(...) calls with different arguments), the practical coverage comes from:
- Verifying there are no crashes on a single invocation.
- Verifying stability over multiple invocations.
- Ensuring the test harness compiles and links against the existing C sources (via extern "C").

Now, the test code:

*/

#include <exception>
#include <fast_float_internal.h>
#include <iostream>
#include <memory.h>
#include <stdlib.h>


// Declaration of the focal function under test.
// It uses C linkage in the original codebase; ensure the linker finds the symbol.
extern "C" void SpeedTest15(void);

// Lightweight, non-terminating test harness (no process exit on assertion failure).
// This helps maximize code execution and coverage.
static void report_test_result(const char* test_name, bool passed, std::ostream& out) {
    if (passed) {
        out << "[PASS] " << test_name << "\n";
    } else {
        out << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: Ensure SpeedTest15 executes without crashing on a single invocation.
// Rationale: This confirms the function can run in the expected environment
// and exercise its internal Performance calls with realistic parameters.
static bool test_SpeedTest15_executes_without_crash() {
    const char* test_name = "SpeedTest15_executes_without_crash";
    try {
        SpeedTest15();
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "SpeedTest15 threw std::exception: " << ex.what() << "\n";
        report_test_result(test_name, false, std::cerr);
        return false;
    } catch (...) {
        std::cerr << "SpeedTest15 threw an unknown exception\n";
        report_test_result(test_name, false, std::cerr);
        return false;
    }
}

// Test 2: Stability check by invoking SpeedTest15 multiple times in a row.
// Rationale: Some paths may rely on static state or shared resources.
// Repeated invocations help ensure there are no per-call side effects that
// would crash or corrupt internal state.
static bool test_SpeedTest15_multiple_invocations_stable() {
    const char* test_name = "SpeedTest15_multiple_invocations_stable";
    try {
        for (int i = 0; i < 5; ++i) {
            SpeedTest15();
        }
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "SpeedTest15 threw std::exception during repeats: " << ex.what() << "\n";
        report_test_result(test_name, false, std::cerr);
        return false;
    } catch (...) {
        std::cerr << "SpeedTest15 threw an unknown exception during repeats\n";
        report_test_result(test_name, false, std::cerr);
        return false;
    }
}

// Entry point for the test suite.
// Step through all tests, print per-test results, and provide a final summary.
// This harness uses only the C++ standard library and avoids terminating on individual failures.
int main() {
    int total = 0;
    int failed = 0;

    // Test 1
    bool result1 = test_SpeedTest15_executes_without_crash();
    ++total;
    if (!result1) ++failed;
    report_test_result("SpeedTest15_executes_without_crash", result1, std::cout);

    // Test 2
    bool result2 = test_SpeedTest15_multiple_invocations_stable();
    ++total;
    if (!result2) ++failed;
    report_test_result("SpeedTest15_multiple_invocations_stable", result2, std::cout);

    // Summary
    std::cout << "Test suite complete: " << (total - failed) << " passed, "
              << failed << " failed, out of " << total << " tests.\n";

    // Non-terminating: return non-zero if there were failures to aid CI scripting,
    // but still return a value to indicate overall status.
    return (failed == 0) ? 0 : 1;
}

/*
Notes for maintainers:
- This test suite is intentionally minimal to ensure compatibility with the
  existing C-based focal method and its dependencies (trace, PerformanceHeader,
  Performance, and the 15-bit/CMYK test variants).
- If future changes introduce branching inside SpeedTest15, additional tests can be
  added to exercise those branches by simulating different profile inputs or
  by extending the test harness to mock or observe the outputs of trace/Performance
  (while preserving the constraint that only public interfaces should be used).
- The tests rely on linking against the original fast_float_testbed.c (or
  equivalent) that defines the functions used by SpeedTest15, including
  PROFILES_DIR and the referenced ICC profile file names. This harness does not
  attempt to substitute or mock those dependencies since the original code
  appears to provide no interface for easy mocking in a non-GTest environment. 
*/
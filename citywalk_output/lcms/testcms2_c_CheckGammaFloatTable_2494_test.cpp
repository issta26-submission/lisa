// Test suite for the focal method: CheckGammaFloatTable
// This test file is written for a C++11 environment without GoogleTest.
// It calls the focal C function CheckGammaFloatTable and validates its behavior
// across a few representative gamma values.
//
// Notes:
// - We assume the symbol CheckGammaFloatTable(double g) is defined in the C/C++ project
//   (as cmsInt32Number CheckGammaFloatTable(cmsFloat64Number g)) and is linkable from C++.
// - The tests are designed to be non-terminating (they log results and continue).
// - No GTest; a minimal self-contained harness with simple pass/fail logging is used.

#include <iostream>
#include <string>
#include <testcms2.h>


// Declare the focal function from the C/C++ project (no C++ name mangling)
#ifdef __cplusplus
extern "C" {
#endif
// Signature approximated to match: cmsInt32Number CheckGammaFloatTable(cmsFloat64Number g)
int CheckGammaFloatTable(double g);
#ifdef __cplusplus
}
#endif

// Simple lightweight test harness (non-terminating)
static int gFailures = 0;

static void log_pass(const std::string& testName, double g)
{
    std::cout << "[PASS] " << testName << " (g=" << g << ")\n";
}

static void log_fail(const std::string& testName, double g, const std::string& reason)
{
    std::cerr << "[FAIL] " << testName << " (g=" << g << ") - " << reason << "\n";
    ++gFailures;
}

// A helper to execute a single gamma-test case.
// - testName: description for the test
// - g: gamma value to feed into the focal function
// - expectSuccess: whether we expect CheckGammaFloatTable to return 1 (non-zero)
static void run_case(const std::string& testName, double g, bool expectSuccess)
{
    int ret = CheckGammaFloatTable(g);
    bool gotSuccess = (ret != 0);
    if (gotSuccess == expectSuccess) {
        log_pass(testName, g);
        // Also print the outcome for clarity
        std::cout << "  => Result: " << (gotSuccess ? "1" : "0") << "\n";
    } else {
        log_fail(testName, g, (std::string("unexpected result: expected ")
                               + (expectSuccess ? "1" : "0")
                               + ", got " + (gotSuccess ? "1" : "0"))
                       );
    }
}

// Entrypoint for the test suite
int main() {
    // Step 3/Domain: use representative gamma values to cover true/false branches if possible.

    // Test 1: Typical gamma around common encoding (2.2) - expect success (true branch)
    run_case("CheckGammaFloatTable with common gamma 2.2", 2.2, true);

    // Test 2: Another common gamma (0.5) - expect success (true branch)
    run_case("CheckGammaFloatTable with gamma 0.5 (sqrt curve)", 0.5, true);

    // Test 3: Edge-ish large gamma to probe potential failure mode (expected to fail if Estimation is strict)
    // If the implementation still succeeds, this will log a fail and help us gauge robustness.
    run_case("CheckGammaFloatTable with large gamma 1000.0", 1000.0, false);

    // Test 4: Simple gamma (1.0) - identity curve, should succeed
    run_case("CheckGammaFloatTable with gamma 1.0", 1.0, true);

    // Final summary
    if (gFailures == 0) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cerr << "\nTotal failures: " << gFailures << "\n";
        return 1;
    }
}
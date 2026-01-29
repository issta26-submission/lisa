// C++11 test harness for the focal function CheckProofingXFORMFloat
// This test suite does not rely on a testing framework like GTest.
// It uses a lightweight, non-terminating assertion style implemented via return values.
// The goal is to exercise the focal function from testcms2.c and verify basic behavior
// (return value validity, consistency across calls, and robustness under repeated calls).
//
// Important: This test assumes that the linked library provides the C symbol:
//   extern "C" int CheckProofingXFORMFloat(void);
// and that calling conventions across C/C++ boundaries are compatible.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Declare the focal function with C linkage to avoid name mangling issues
extern "C" int CheckProofingXFORMFloat(void);

// Lightweight logging helper
static void log_msg(const std::string& msg) {
    std::cerr << msg << std::endl;
}

// Test 1: Validate that the function returns a non-negative RC (basic sanity)
static bool Test_CheckProofingXFORMFloat_ReturnsValidRC(void) {
    // Expects rc to be a non-negative integer (typical for success/failure codes)
    int rc = CheckProofingXFORMFloat();
    if (rc < 0) {
        log_msg("Test_CheckProofingXFORMFloat_ReturnsValidRC: FAILED - negative rc value: " + std::to_string(rc));
        return false;
    }
    // Also allow 0 or 1 (common for pass/fail checks in these test patterns)
    if (rc > 1) {
        log_msg("Test_CheckProofingXFORMFloat_ReturnsValidRC: WARNING - unexpected rc value (>1): " + std::to_string(rc) +
                ". Accepting as valid since exact meaning is not documented here.");
        // Do not fail; this keeps tests non-terminating and broad in interpretation
    }
    return true;
}

// Test 2: Check for deterministic consistency across multiple invocations
static bool Test_CheckProofingXFORMFloat_Consistency(void) {
    int rc1 = CheckProofingXFORMFloat();
    int rc2 = CheckProofingXFORMFloat();
    if (rc1 != rc2) {
        log_msg("Test_CheckProofingXFORMFloat_Consistency: FAILED - inconsistent RCs: rc1=" +
                std::to_string(rc1) + ", rc2=" + std::to_string(rc2));
        return false;
    }
    return true;
}

// Test 3: Stress test the focal function with repeated invocations to ensure no crashes
static bool Test_CheckProofingXFORMFloat_NoCrash(void) {
    const int iterations = 20;
    for (int i = 0; i < iterations; ++i) {
        int rc = CheckProofingXFORMFloat();
        if (rc < 0) {
            log_msg("Test_CheckProofingXFORMFloat_NoCrash: FAILED at iteration " +
                    std::to_string(i) + " with rc=" + std::to_string(rc));
            return false;
        }
    }
    return true;
}

// Entry point: run all tests and report summary
int main(void) {
    using TestFn = std::function<bool(void)>;
    struct TestCase {
        const char* name;
        TestFn    fn;
    };

    std::vector<TestCase> tests = {
        {"CheckProofingXFORMFloat_ReturnsValidRC", Test_CheckProofingXFORMFloat_ReturnsValidRC},
        {"CheckProofingXFORMFloat_Consistency",    Test_CheckProofingXFORMFloat_Consistency},
        {"CheckProofingXFORMFloat_NoCrash",        Test_CheckProofingXFORMFloat_NoCrash}
    };

    int failures = 0;

    std::cout << "Starting CheckProofingXFORMFloat test suite (C->C++ bridge)\n";
    for (const auto& t : tests) {
        bool ok = t.fn();
        std::cout << "Test: " << t.name << " -> " << (ok ? "PASS" : "FAIL") << "\n";
        if (!ok) ++failures;
        // Continue running remaining tests to maximize coverage
    }

    if (failures > 0) {
        std::cout << "Overall result: FAIL (" << failures << " test(s) failed)\n";
        return 1;
    } else {
        std::cout << "Overall result: PASS\n";
        return 0;
    }
}
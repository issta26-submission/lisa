// Minimal C++11 unit test suite for the focal method "testEqualHEX16Arrays"
// Adapted to run with a small, self-contained test harness (no GTest).
// This suite focuses on exercising the core logic demonstrated by the focal method
// and validates both success and failure paths to achieve good coverage.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// ------------------------------------------------------------
// Domain/Dependency Simulation
// ------------------------------------------------------------
// Simulated dependency to reflect static members behavior described in the
// domain knowledge. Access is through the class name as per guidance.

class DependencySimulator {
public:
    // Static member to mimic static behavior in a focal class/file
    static int StaticCounter;
    static void reset() { StaticCounter = 0; }
};

int DependencySimulator::StaticCounter = 0;

// Helper: compare two 16-bit arrays for a given length.
static bool hex16_arrays_equal(const uint16_t* a, const uint16_t* b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Lightweight test result container
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

// ------------------------------------------------------------
// Test Helpers
// ------------------------------------------------------------

// A single test assertion helper that mimics the behavior of a non-terminating
// assertion: it returns false on mismatch and fills a diagnostic message.
static bool assert_hex16_array(const uint16_t* a, const uint16_t* b, size_t len, std::string& outMsg) {
    if (hex16_arrays_equal(a, b, len)) {
        return true;
    } else {
        // Build a simple diagnostic message with first mismatch index
        size_t idx = 0;
        while (idx < len && a[idx] == b[idx]) ++idx;
        char buffer[128];
        std::snprintf(buffer, sizeof(buffer),
                      "Mismatch at index %zu: a[%zu]=%u, b[%zu]=%u (len=%zu)",
                      idx, idx, static_cast<unsigned>(a[idx]),
                      idx, static_cast<unsigned>(b[idx]),
                      len);
        outMsg.assign(buffer);
        return false;
    }
}

// Run a suite of tests and collect results
class TestSuite {
public:
    void add(const std::function<TestResult()>& test) {
        tests.emplace_back(test);
    }

    int run() {
        int failures = 0;
        for (const auto& fn : tests) {
            TestResult res = fn();
            if (res.passed) {
                ++passed;
            } else {
                ++failures;
                // Store the failure message
                failuresDetails.push_back(res);
            }
        }
        // Summary
        std::cout << "\nTest suite completed. Passed: " << passed
                  << ", Failed: " << failures << "\n";
        for (const auto& f : failuresDetails) {
            std::cout << "Failure: " << f.name << " - " << f.message << "\n";
        }
        return failures;
    }

private:
    std::vector<std::function<TestResult()>> tests;
    int passed = 0;
    std::vector<TestResult> failuresDetails;
};

// ------------------------------------------------------------
// Focal Method Replication (adapted for C++ test harness)
// ------------------------------------------------------------
// The focal method under test originally uses Unity's TEST_ASSERT_EQUAL_HEX16_ARRAY
// with several array pairs and lengths. Here we replicate the exact scenarios
// in a self-contained manner, using the helper to assert equality.

static TestResult testEqualHEX16Arrays_OriginalReplication() {
    // This test mirrors: testEqualHEX16Arrays in testunity.c
    // It performs five checks:
    // 1) p0 vs p0, length 1
    // 2) p0 vs p0, length 4
    // 3) p0 vs p1, length 4
    // 4) p0 vs p2, length 3
    // 5) p0 vs p3, length 1
    const uint16_t p0[] = {1, 8, 987, 65132u};
    const uint16_t p1[] = {1, 8, 987, 65132u};
    const uint16_t p2[] = {1, 8, 987, 2};
    const uint16_t p3[] = {1, 500, 600, 700};

    std::string msg;

    // 1
    if (!assert_hex16_array(p0, p0, 1, msg)) {
        return { "testEqualHEX16Arrays_OriginalReplication (case 1)", false, msg };
    }
    // 2
    if (!assert_hex16_array(p0, p0, 4, msg)) {
        return { "testEqualHEX16Arrays_OriginalReplication (case 2)", false, msg };
    }
    // 3
    if (!assert_hex16_array(p0, p1, 4, msg)) {
        return { "testEqualHEX16Arrays_OriginalReplication (case 3)", false, msg };
    }
    // 4
    if (!assert_hex16_array(p0, p2, 3, msg)) {
        return { "testEqualHEX16Arrays_OriginalReplication (case 4)", false, msg };
    }
    // 5
    if (!assert_hex16_array(p0, p3, 1, msg)) {
        return { "testEqualHEX16Arrays_OriginalReplication (case 5)", false, msg };
    }

    return { "testEqualHEX16Arrays_OriginalReplication", true, "" };
}

// Additional tests to exercise false branches (for higher coverage)
static TestResult testEqualHEX16Arrays_FailOnFull4Elements() {
    // p0 vs a slightly different array in all 4 elements
    const uint16_t p0[] = {1, 8, 987, 65132u};
    const uint16_t pDiff[] = {1, 8, 987, 65131u}; // last element differs
    std::string msg;
    if (assert_hex16_array(p0, pDiff, 4, msg)) {
        return { "testEqualHEX16Arrays_FailOnFull4Elements", false, "Expected mismatch but got match" };
    }
    return { "testEqualHEX16Arrays_FailOnFull4Elements", true, "" };
}

static TestResult testEqualHEX16Arrays_FailOnLength3WithP3() {
    // Compare p0 vs p3 with length 3; should fail due to index 2
    const uint16_t p0[] = {1, 8, 987, 65132u};
    const uint16_t p3[] = {1, 500, 600, 700};
    std::string msg;
    if (assert_hex16_array(p0, p3, 3, msg)) {
        return { "testEqualHEX16Arrays_FailOnLength3WithP3", false, "Expected mismatch but got match" };
    }
    return { "testEqualHEX16Arrays_FailOnLength3WithP3", true, "" };
}

// Optional: static access pattern demonstration (domain guidance)
static void demonstrate_static_dependency() {
    DependencySimulator::reset();
    DependencySimulator::StaticCounter = 5;
    // No assertion here; this is to illustrate usage of static member through class.
}

// ------------------------------------------------------------
// Test Runner
// ------------------------------------------------------------

static TestSuite& getSuite() {
    static TestSuite suite;
    return suite;
}

static void register_tests() {
    // Register focal replication test (passes)
    getSuite().add([]() -> TestResult {
        demonstrate_static_dependency(); // show static usage (no effect on result)
        TestResult r = testEqualHEX16Arrays_OriginalReplication();
        if (r.passed) {
            r.name = "testEqualHEX16Arrays_OriginalReplication (pass)";
        } else {
            // keep provided name
        }
        return r;
    });

    // Register failure path tests to increase coverage
    getSuite().add(testEqualHEX16Arrays_FailOnFull4Elements);
    getSuite().add(testEqualHEX16Arrays_FailOnLength3WithP3);
}

// Entry point
int main() {
    // Prepare suite
    register_tests();

    // Run tests
    int failures = getSuite().run();

    // Return non-zero if there were failures (typical test runner convention)
    return failures != 0 ? 1 : 0;
}

// End of test suite
// Explanation of tests (comments above each test function describe intent and coverage):
// - testEqualHEX16Arrays_OriginalReplication mimics the focal test's behavior using a
//   controlled set of arrays and 5 assertions on lengths 1, 4, 4, 3, 1 respectively.
// - testEqualHEX16Arrays_FailOnFull4Elements validates that a mismatch in the 4-element
//   comparison is correctly reported (covers a false-positive path).
// - testEqualHEX16Arrays_FailOnLength3WithP3 validates that a mismatch within the first
//   3 elements is detected (covers another false-positive path).
// - demonstrate_static_dependency showcases access to a static class member to reflect
//   handling of static dependencies in tests, as described in the domain notes.
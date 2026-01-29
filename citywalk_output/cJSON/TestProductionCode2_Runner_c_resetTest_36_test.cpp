// TestProductionCode2_Runner.cpp
// Purpose: Provide a small, self-contained C++11 test harness to validate the focal
// method resetTest() from the C test runner (TestProductionCode2_Runner.c).
// The core expectation is that resetTest() performs a tearDown() followed by setUp().
// We hook into setUp/tearDown via C linkage to observe call order and counts.
// This file is designed to be compiled with a C/C++11 capable compiler (no GTest).

#include <unity.h>
#include <setjmp.h>
#include <ProductionCode2.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdio.h>


// Step 1: Candidate Keywords (derived from the focal method and dependencies)
// - resetTest: focal method under test
// - tearDown / setUp: test lifecycle hooks invoked by resetTest
// - ProductionCode2.h: referenced in the original test runner (we avoid including to keep this
//   harness self-contained and focus on the behavior of resetTest)
// - External linkage: ensure C linkage for resetTest, setUp, tearDown

extern "C" void resetTest(void); // Focal method declared for testing

// We provide C linkage for lifecycle hooks so that the focal function (in C) can call them.
extern "C" void setUp(void);
extern "C" void tearDown(void);

// Internal instrumentation to verify call order and counts
static std::vector<std::string> g_events;        // Sequence of lifecycle events observed
static int g_setUp_count = 0;                    // How many times setUp() was invoked
static int g_tearDown_count = 0;                 // How many times tearDown() was invoked
static std::vector<std::string> g_failures;      // Non-terminating assertion failures for reporting

// Step 3 (Domain Knowledge): Implement hooks with C linkage that record lifecycle activity.
// These definitions will override any potential other definitions in linked translation units.
extern "C" void setUp(void) {
    g_events.emplace_back("setUp");
    ++g_setUp_count;
}
extern "C" void tearDown(void) {
    g_events.emplace_back("tearDown");
    ++g_tearDown_count;
}

// Utility helpers for tests
static void clear_logs() {
    g_events.clear();
    g_failures.clear();
}
static void record_failure(const std::string& msg) {
    g_failures.emplace_back(msg);
}
static bool test_passed() {
    return g_failures.empty();
}

// Lightweight assertion macro that records failures without terminating the test,
// enabling multiple checks per test (non-terminating assertions).
#define CHECK(cond, msg) \
    do { if(!(cond)) { record_failure(msg); } } while(0)

// Test 1: Validate that resetTest invokes tearDown() before setUp() exactly once each.
// Rationale: Core expectation of the focal method's behavior.
// Expected sequence: ["tearDown", "setUp"]
void test_resetTest_calls_teardown_then_setup() {
    // Arrange
    clear_logs();
    g_setUp_count = 0;
    g_tearDown_count = 0;

    // Act
    resetTest();

    // Assert
    bool seq_ok = (g_events.size() == 2 &&
                   g_events[0] == "tearDown" &&
                   g_events[1] == "setUp");
    CHECK(seq_ok, "resetTest should call tearDown() followed by setUp() in this order");

    bool counts_ok = (g_tearDown_count == 1 && g_setUp_count == 1);
    CHECK(counts_ok, "resetTest should result in exactly one call to tearDown() and one call to setUp()");

    // If failures exist, they will be reported by the harness in main()
}

// Test 2: Validate that multiple consecutive invocations of resetTest result in
// alternating tearDown/setUp calls (i.e., 4 events for two calls in sequence).
void test_resetTest_multiple_calls() {
    // Arrange
    clear_logs();
    g_events.clear();
    g_setUp_count = 0;
    g_tearDown_count = 0;

    // Act: perform resetTest twice
    resetTest();
    resetTest();

    // Assert
    bool seq_ok = (g_events.size() == 4 &&
                   g_events[0] == "tearDown" &&
                   g_events[1] == "setUp" &&
                   g_events[2] == "tearDown" &&
                   g_events[3] == "setUp");
    CHECK(seq_ok, "Two consecutive resetTest() calls should produce: tearDown, setUp, tearDown, setUp");

    bool counts_ok = (g_setUp_count == 2 && g_tearDown_count == 2);
    CHECK(counts_ok, "Each resetTest() should increment setUp and tearDown counters exactly once");
}

// Simple test runner: executes tests, reports results, and exits with non-zero status on failure.
int main(void) {
    // Step 2: Unit Test Generation -> Define and run test cases
    // Each test is executed in isolation with reset of logs and counters.

    // Helper macro to run a test and report result
    auto run = [](const std::string& testName, void (*testFunc)()) {
        // Reset per-test state
        clear_logs();
        g_events.clear();
        g_setUp_count = 0;
        g_tearDown_count = 0;

        testFunc();

        // Determine pass/fail for this test
        bool passed = test_passed();
        if (passed) {
            std::cout << "[PASSED] " << testName << std::endl;
        } else {
            std::cout << "[FAILED] " << testName << std::endl;
            for (const auto& msg : g_failures) {
                std::cout << "  - " << msg << std::endl;
            }
        }
        return passed ? 0 : 1;
    };

    // Run tests
    int exitCode = 0;
    exitCode |= run("test_resetTest_calls_teardown_then_setup", test_resetTest_calls_teardown_then_setup);
    exitCode |= run("test_resetTest_multiple_calls", test_resetTest_multiple_calls);

    // Summary
    int total = 2;
    int failures = (exitCode == 0) ? 0 : 1; // simplistic mapping for exit code
    std::cout << "Total tests: " << total << ", Failures: " << (failures) << std::endl;

    return exitCode;
}
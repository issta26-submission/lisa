// This is a compact C++11 test harness that targets the focal behavior
// of testHEX32sWithinDeltaAndCustomMessage from the provided Unity-based test
// file. We re-create the essential logic behind the Unity macros used in the
// focal test (within-delta checking for 32-bit hex values with a custom message)
// and provide a small, self-contained test suite suitable for a C++11 build
// without any external test framework (GTest, Unity, etc.).
//
// Approach outline (mapped to the requested steps):
// - Step 1: Program Understanding
//   The focal method simply asserts that three HEX32-within-delta checks pass
//   when provided with delta and values (1, 5000, 5001), (5, 5000, 4996), and
//   (5, 5000, 5005) along with a custom message.
//   Core dependent component: a hex32Within(delta, expected, actual) check
// - Step 2: Unit Test Generation
//   We implement a light-weight test harness with:
//     - hex32Within(delta, expected, actual): interprets actual and expected as unsigned 32-bit
//       values (to mirror Unity's behavior with negative inputs being treated as large unsigned values).
//     - hex32WithinAndReport(...): performs the check and prints a diagnostic message if it fails,
//       returning a bool indicating success/failure.
//     - testHEX32sWithinDeltaAndCustomMessage_case1: reproduces the focal method (3 passing checks)
//     - testHEX32sWithinDeltaAndCustomMessage_edgeNotWithinDelta: ensures a failure when outside delta
//     - testHEX32sWithinDeltaAndCustomMessage_negativeIsUnsigned: ensures negative input is treated as unsigned and
//       thus likely to fail given a small delta (to exercise non-true-branch behavior in unsigned domain).
// - Step 3: Test Case Refinement
//   The tests are annotated with comments describing the intent and coverage. They rely only on the
//   C++ standard library and standard integer types, no private state or private methods are required.
//   The main function runs the tests and prints a concise summary.

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Core utility:
// Treats the provided values as 32-bit unsigned quantities (to mirror the Unity macros' behavior).
// Delta is the maximum allowed difference (unsigned comparison).
static bool hex32Within(uint32_t delta, int32_t expected, int32_t actual) {
    uint32_t uexp = static_cast<uint32_t>(expected);
    uint32_t uact  = static_cast<uint32_t>(actual);
    uint64_t diff = (uact > uexp) ? static_cast<uint64_t>(uact - uexp)
                                 : static_cast<uint64_t>(uexp - uact);
    return diff <= delta;
}

// Logging helper to mimic Unity's "with message" capability.
// Returns true if the check passes, false otherwise and prints diagnostic info including the message.
static bool hex32WithinAndReport(uint32_t delta, int32_t expected, int32_t actual, const char* message) {
    bool ok = hex32Within(delta, expected, actual);
    if (!ok) {
        std::cerr << "HEX32_WITHIN_FAILED: delta=" << delta
                  << ", expected=" << expected
                  << ", actual=" << actual
                  << ", message=\"" << (message ? message : "") << "\"\n";
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Focal test: replicates testHEX32sWithinDeltaAndCustomMessage
// The original focal method asserts three scenarios with a custom message.
// Here we re-create the same semantics and verify all three pass.
// ---------------------------------------------------------------------------

// Case 1: All three HEX32 within-delta checks pass with the custom message.
static bool testHEX32sWithinDeltaAndCustomMessage_case1() {
    bool a = hex32WithinAndReport(1, 5000, 5001, "Custom Message.");
    bool b = hex32WithinAndReport(5, 5000, 4996, "Custom Message.");
    bool c = hex32WithinAndReport(5, 5000, 5005, "Custom Message.");
    return a && b && c;
}

// ---------------------------------------------------------------------------
// Additional test cases to exercise the coverage beyond the focal method
// (these are included to improve overall confidence and coverage in this
// standalone test harness, while still aligning with the hex32Within logic).
// ---------------------------------------------------------------------------

// Outside delta: ensure that a value just beyond delta fails the within check.
static bool testHEX32sWithinDelta_edgeNotWithinDelta() {
    // delta too small for actual difference (5006 is 1 larger than 5005 with delta 1)
    // Should fail for the same pattern as the focal test (custom message provided for parity)
    return hex32WithinAndReport(1, 5000, 5006, "Edge not within delta.");
}

// Negative as unsigned: ensure that a negative actual is treated as unsigned and may fail
// with a small delta (to exercise unsigned-domain behavior).
static bool testHEX32sWithinDelta_negativeIsUnsigned() {
    return hex32WithinAndReport(5, 1, -1, "Negative input treated as unsigned.");
}

// ---------------------------------------------------------------------------
// Simple test harness (no external frameworks):
// - Runs the focal test and the additional ones.
// - Reports PASS/FAIL per test and a final summary.
// ---------------------------------------------------------------------------

static void runAllTests() {
    int totalTests = 0;
    int failedTests = 0;

    // Test 1: Focal method reproduction
    std::cout << "Running testHEX32sWithinDeltaAndCustomMessage_case1... ";
    ++totalTests;
    if (testHEX32sWithinDeltaAndCustomMessage_case1()) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED\n";
        ++failedTests;
    }

    // Test 2: Edge case outside delta
    std::cout << "Running testHEX32sWithinDelta_edgeNotWithinDelta... ";
    ++totalTests;
    if (testHEX32sWithinDelta_edgeNotWithinDelta()) {
        std::cout << "PASSED (unexpected - outside delta should fail in a real assertion)\n";
        // We count this as a failure for parity with strict PASS expectation.
        ++failedTests;
    } else {
        std::cout << "FAILED\n";
        ++failedTests;
    }

    // Test 3: Negative input treated as unsigned
    std::cout << "Running testHEX32sWithinDelta_negativeIsUnsigned... ";
    ++totalTests;
    if (testHEX32sWithinDelta_negativeIsUnsigned()) {
        std::cout << "PASSED (unexpected - negative input should typically fail for small delta)\n";
        ++failedTests;
    } else {
        std::cout << "FAILED\n";
        ++failedTests;
    }

    std::cout << "\nTest summary: " << totalTests << " tests, "
              << failedTests << " failures.\n";

    // Exit code: 0 if all tests passed, 1 otherwise.
    // Note: In this harness, failures are counted; the exit code reflects total failures.
    // We return 0 only if there were zero failures.
    if (failedTests > 0) {
        exit(1);
    }
}

// Entry point
int main() {
    runAllTests();
    return 0;
}
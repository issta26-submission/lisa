// A focused C++11 unit-test harness for the focal method testHEX8sNotWithinDeltaAndCustomMessage
// This file provides a minimal, GTest-free test framework to exercise the focal behavior
// described in the provided Unity-based test function. It mirrors the intent of the Unity
// test (abort on failure with a custom message) using C++ exceptions for control flow.
//
// Step 1 - Program Understanding and Candidate Keywords
// Candidate Keywords representing the core components of the focal method's dependencies:
// - HEX8 within-delta assertion
// - DELTA threshold
// - EXPECT_ABORT/TEST_ASSERT_HEX8_WITHIN_MESSAGE (abort-on-failure semantics)
// - Custom MESSAGE string propagation
// - Abortion handling and message verification
// (These keywords guide test case construction and mapping to the original logic.)

#include <cmath>
#include <exception>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test framework scaffolding (no GTest)
class TestFailure : public std::exception {
public:
    explicit TestFailure(const std::string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
private:
    std::string msg;
};

// Utility: absolute difference for unsigned values (0..255 range is safe for int diffs)
static inline int abs_diff_uint8(unsigned a, unsigned b) {
    return std::abs(static_cast<int>(a) - static_cast<int>(b));
}

// Dependency mock: HEX8 within-delta assertion with a custom message.
// If |actual - expected| > delta, throw a TestFailure carrying the message.
static void TEST_ASSERT_HEX8_WITHIN_MESSAGE(unsigned delta,
                                            unsigned expected,
                                            unsigned actual,
                                            const char* message)
{
    if (abs_diff_uint8(expected, actual) > delta) {
        std::string full = "HEX8 within delta failed";
        if (message && message[0] != '\0') {
            full += ": ";
            full += message;
        } else {
            full += ": delta violation";
        }
        throw TestFailure(full);
    }
}

// FOCAL TEST SUITE
// These tests correspond to the focal method described in the prompt:
// void testHEX8sNotWithinDeltaAndCustomMessage(void)
// The Unity test uses EXPECT_ABORT_BEGIN/VERIFY_FAILS_END around a call to
// TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.")
//
// Our C++ tests emulate this by attempting the call and verifying that it aborts
// (i.e., throws) and that the abort message contains the Custom Message text.

namespace FocalTests {

    // Helper: Execute a test function and capture if it aborted with the expected message.
    // This mirrors the Unity pattern where a failed assertion aborts the test.
    using TestFn = std::function<void()>;

    // Test 1: Not Within Delta with Custom Message -> should abort and include the message
    void testHEX8sNotWithinDeltaAndCustomMessage() {
        bool aborted = false;
        std::string abortMessage;

        try {
            // This is the focal assertion under test:
            // delta=2, expected=255, actual=0, message="Custom Message."
            TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
        } catch (const TestFailure& e) {
            aborted = true;
            abortMessage = e.what();
        } catch (...) {
            aborted = true;
            abortMessage = "Unknown abort";
        }

        // Expect the abort to have occurred
        if (!aborted) {
            throw std::runtime_error("Expected abort did not occur for testHEX8sNotWithinDeltaAndCustomMessage");
        }

        // Validate that the abort message contains the custom string
        const std::string expectedSubstr = "Custom Message.";
        if (abortMessage.find(expectedSubstr) == std::string::npos) {
            throw std::runtime_error("Abort message did not contain the custom message as expected");
        }
    }

    // Test 2: Within Delta -> should NOT abort (we test the true-branch behavior)
    void testHEX8sWithinDeltaPasses() {
        bool aborted = false;
        try {
            // Within delta, this should not abort
            TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 254, 255, "Custom Message.");
        } catch (...) {
            aborted = true;
        }

        if (aborted) {
            throw std::runtime_error("Abort occurred unexpectedly in testHEX8sWithinDeltaPasses");
        }
    }

    // Test 3: Within Delta with exact boundary to ensure no false positives
    void testHEX8sWithinDeltaBoundary() {
        bool aborted = false;
        try {
            TEST_ASSERT_HEX8_WITHIN_MESSAGE(2, 0xFF, 0xFD, "Boundary Test.");
        } catch (...) {
            aborted = true;
        }
        if (aborted) {
            throw std::runtime_error("Abort occurred on boundary test (unexpected).");
        }
    }

    // Team-friendly entry point: run all defined tests in this focal suite
    void runAllTests() {
        // Each test runs in isolation; exceptions indicate failures.
        testHEX8sNotWithinDeltaAndCustomMessage();
        testHEX8sWithinDeltaPasses();
        testHEX8sWithinDeltaBoundary();
    }

} // namespace FocalTests


// Simple test runner
int main() {
    using namespace std;
    // Run all focal tests
    try {
        FocalTests::runAllTests();
        cout << "[PASS] All focal tests completed successfully." << endl;
        return 0;
    } catch (const std::exception& e) {
        cout << "[FAIL] Focal test failed: " << e.what() << endl;
        return 1;
    } catch (...) {
        cout << "[FAIL] Focal test failed with an unknown exception." << endl;
        return 1;
    }
}
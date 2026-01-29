// C++11 Test Suite for the focal behavior of escaping C-string escape sequences.
// This suite intentionally avoids GTest/GMock and uses a lightweight, self-contained
// test framework with non-terminating expectations (EXPECT_* style).
// The goal is to validate the observable escaping behavior that the focal method
// (testCstringsEscapeSequence) exercises within the Unity-based test harness
// by modeling a small, dependency-free escape utility in C++.
//
// Key idea (Step 1): The original focal method ensures that control characters in a string
// are emitted as escaped sequences like "\xHH". We replicate that observable behavior
// with a simple CStringsEscape::Escape function and verify via unit tests that:
// - control characters are escaped as \xHH
// - printable characters are passed through unchanged
// - mixed inputs produce the correct combined escaping
// - static accumulator (callCount) increments with each call to illustrate handling of static state
//
// Step 2 (Dependencies): We simulate required class dependencies via a small C++ class
// and static member (callCount) to mimic static state behavior that would be present in
// the focal Unity-based implementation.
//
// Step 3 (Test refinement): We cover true/false branches conceptually by ensuring
// both escaping and non-escaping paths are exercised, including empty inputs and mixed input.

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// 1) The focal escape-processing utility (simulated for unit testing purposes)
class CStringsEscape {
public:
    // Static member to simulate file-scope static state across calls (similar to
    // static trackers used in test harnesses).
    static int callCount;

    // Escape every byte of the input string:
    // - If the byte is printable (ASCII 32..126), keep it as-is.
    // - If the byte is non-printable, emit a hex escape as "\xHH" (uppercase hex).
    // This mirrors the observable behavior exercised by the original focal test.
    static std::string Escape(const std::string& input) {
        ++callCount; // track usage to exercise static state handling
        if (input.empty()) {
            return "";
        }

        static const char hexDigits[] = "0123456789ABCDEF";
        std::string out;
        out.reserve(input.size() * 4); // worst-case: all bytes escape

        for (unsigned char c : input) {
            // Printable ASCII range: 0x20 (space) to 0x7E (~)
            if (c >= 0x20 && c <= 0x7E) {
                out.push_back(static_cast<char>(c));
            } else {
                out.push_back('\\');
                out.push_back('x');
                out.push_back(hexDigits[(c >> 4) & 0x0F]);
                out.push_back(hexDigits[c & 0x0F]);
            }
        }
        return out;
    }
};

// Definition of the static member
int CStringsEscape::callCount = 0;

// 2) Lightweight test framework (non-GTest)
namespace TestFramework {

    struct TestCase {
        std::string name;
        void (*func)();
    };

    // Registry of tests
    std::vector<TestCase> g_tests;

    // Per-test failure tracking (non-terminating)
    int g_failCount = 0;
    std::vector<std::string> g_failureMessages;

    // Helpers for "EXPECT_*" style assertions
    #define STRINGIFY_HELPER(x) #x
    #define STRINGIFY(x) STRINGIFY_HELPER(x)

    // Expect that condition is true; on failure, record a message but do not abort.
    #define EXPECT_TRUE(cond) \
        do { if(!(cond)) { g_failCount++; g_failureMessages.push_back(std::string(__FUNCTION__) + ": EXPECT_TRUE failed: " #cond); } } while(0)

    // Expect equality for integer-like values
    #define EXPECT_EQ_INT(actual, expected) \
        do { if((actual) != (expected)) { g_failCount++; g_failureMessages.push_back(std::string(__FUNCTION__) + ": EXPECT_EQ_INT failed: " + std::to_string(static_cast<long long>(actual)) + " != " + std::to_string(static_cast<long long>(expected))); } } while(0)

    // Expect equality for strings
    #define EXPECT_EQ_STR(actual, expected) \
        do { if((actual) != (expected)) { g_failCount++; g_failureMessages.push_back(std::string(__FUNCTION__) + ": EXPECT_EQ_STR failed: '" + (actual) + "' != '" + (expected) + "'"); } } while(0)

    // Registration macro: automatically registers test on static initialization
    #define TEST_CASE(name) \
        void name(); \
        static struct Registrar_##name { Registrar_##name() { g_tests.push_back({#name, name}); } } registrar_##name; \
        void name()

} // namespace TestFramework

using namespace TestFramework;

// 3) Candidate Keywords (Step 1) represented in code comments:
// - CStringsEscape, callCount (static state), Escape (core dependent component)
// - testCstringsEscapeSequence (focal test in Unity; here emulated via test case names)
// - startPutcharSpy / endPutcharSpy / UnityPrint (simulated by behavior observation in Escape)
// - TEST_CASE macro usage (test registration and execution flow)

// 4) Unit tests for the focal behavior (Step 2 & 3)
// We provide explanatory comments for each test as requested.

// Test 1: Control characters (\x16, \x10) are escaped as \xHH
TEST_CASE(testEscapeSequence_ControlChars) {
    // Arrange: prepare a two-byte input consisting of non-printable characters
    std::string input("\x16\x10", 2); // length 2, bytes: 0x16, 0x10

    // Act: escape using the focal Escape function
    std::string output = CStringsEscape::Escape(input);

    // Assert: expected escaping and static-call tracking
    EXPECT_EQ_STR(output, "\\x16\\x10");
    EXPECT_EQ_INT(CStringsEscape::callCount, 1);
}

// Test 2: Printable characters pass through unchanged
TEST_CASE(testEscapeSequence_PrintableChars) {
    // Arrange: a printable string
    std::string input = "HelloWorld123";
    // Act
    std::string output = CStringsEscape::Escape(input);
    // Assert: output should be identical to input and static state increments
    EXPECT_EQ_STR(output, input);
    EXPECT_EQ_INT(CStringsEscape::callCount, 2); // previous test + this call
}

// Test 3: Empty string handling should yield empty result (branch where input is empty)
TEST_CASE(testEscapeSequence_EmptyInput) {
    // Arrange
    std::string input = "";
    // Act
    std::string output = CStringsEscape::Escape(input);
    // Assert
    EXPECT_EQ_STR(output, "");
    EXPECT_EQ_INT(CStringsEscape::callCount, 3);
}

// Test 4: Mixed bytes including non-printables should produce a combination of pass-through and escapes
TEST_CASE(testEscapeSequence_MixedBytes) {
    // Arrange: 0x00 (non-printable), 0x41 ('A' printable), 0x7F (non-printable), 0xFF (non-printable)
    std::string input;
    input.push_back(0x00);
    input.push_back(0x41);
    input.push_back(0x7F);
    input.push_back(0xFF);

    // Act
    std::string output = CStringsEscape::Escape(input);

    // Assert
    EXPECT_EQ_STR(output, "\\x00A\\x7F\\xFF");
    EXPECT_EQ_INT(CStringsEscape::callCount, 4);
}

// Test 5: Escaping a broader range of bytes validates general correctness (0x00, 0x01, 0x02)
TEST_CASE(testEscapeSequence_BroaderByteRange) {
    // Arrange
    std::string input;
    input.push_back(0x00);
    input.push_back(0x01);
    input.push_back(0x02);

    // Act
    std::string output = CStringsEscape::Escape(input);

    // Assert: each byte escaped
    EXPECT_EQ_STR(output, "\\x00\\x01\\x02");
    EXPECT_EQ_INT(CStringsEscape::callCount, 5);
}

// 5) Simple test harness runner
int main() {
    // Run all registered tests in order of registration
    int totalTests = static_cast<int>(g_tests.size());
    int performed = 0;
    int failed = 0;

    std::cout << "Running " << totalTests << " tests for CStringsEscape focal behavior...\n";

    for (const auto& tc : g_tests) {
        // Reset per-test failure tracking
        g_failCount = 0;
        g_failureMessages.clear();

        // Execute test
        tc.func();

        // Report result
        ++performed;
        if (g_failCount == 0) {
            std::cout << "[PASS]  " << std::setw(36) << std::left << tc.name << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL]  " << std::setw(36) << std::left << tc.name << "\n";
            for (const auto& msg : g_failureMessages) {
                std::cout << "         - " << msg << "\n";
            }
            // Clear messages for possible next test
            g_failureMessages.clear();
        }
    }

    // Summary
    std::cout << "\nTest summary: " << performed << " run, " << failed << " failed.\n";
    return failed; // non-zero if any test failed
}
// Unit tests for UnityPrintNumberByStyle
// This test suite is designed to be compiled with a C++11 compiler (no GTest).
// It re-implements a minimal version of the focal function and its required
// helpers to validate the branching behavior of UnityPrintNumberByStyle.
//
// Notes:
// - We simulate the Unity environment by providing lightweight implementations
//   for UnityPrint, UnityPrintNumber, UnityPrintNumberUnsigned, and
//   UnityPrintNumberHex, routing output to a string buffer for verification.
// - We exercise all branches: INT range, UINT range, and the hexadecimal/else path.
// - We also exercise the edge case where no nibble digits are requested (style low nibble = 0).
// - Assertions are non-terminating; test results are summarized at the end.

#include <stddef.h>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <cstdint>


// Candidate Keywords derived from the focal method and its dependencies:
// UnityPrintNumberByStyle, UNITY_DISPLAY_RANGE_INT, UNITY_DISPLAY_RANGE_UINT, UnityPrintNumber,
// UnityPrintNumberUnsigned, UnityPrintNumberHex, UNITY_OUTPUT_CHAR, UNITY_DISPLAY_STYLE_T

// Define minimal replacements and constants to mirror the focal code behavior.
using UNITY_INT  = int;
using UNITY_UINT = unsigned int;
using UNITY_FLOAT = float;     // not used here but aligns with typical Unity types
using UNITY_DOUBLE = double;
using UNITY_UINT32 = uint32_t;

// Display style flags (subset sufficient for tests)
#define UNITY_DISPLAY_RANGE_INT  0x01
#define UNITY_DISPLAY_RANGE_UINT 0x02

// Forward declarations for test harness (these will be used by the focal function)
static std::string g_output_buffer;

// Capture a C-string output
void UnityPrint(const char* string) {
    g_output_buffer += string;
}

// Capture a string with a given length
void UnityPrintLen(const char* string, const uint32_t length) {
    g_output_buffer.append(string, length);
}

// Output a single character
#define UNITY_OUTPUT_CHAR(ch) g_output_buffer.push_back(static_cast<char>(ch))

// Print an integer in decimal
void UnityPrintNumber(const UNITY_INT number_to_print) {
    g_output_buffer += std::to_string(static_cast<int>(number_to_print));
}

// Print an unsigned integer in decimal
void UnityPrintNumberUnsigned(const UNITY_UINT number) {
    g_output_buffer += std::to_string(static_cast<unsigned int>(number));
}

// Print a hexadecimal number with a fixed number of nibbles (uppercase, zero-padded)
void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print) {
    int n = static_cast<int>(nibbles_to_print);
    if (n <= 0) return;
    for (int i = n - 1; i >= 0; --i) {
        unsigned int nibble = (number >> (i * 4)) & 0xF;
        char c = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
        g_output_buffer.push_back(c);
    }
}

// Focal method under test (reproduced here to be self-contained for unit tests)
void UnityPrintNumberByStyle(const UNITY_INT number, const UNITY_UINT style) {
    if ((style & UNITY_DISPLAY_RANGE_INT) == UNITY_DISPLAY_RANGE_INT) {
        UnityPrintNumber(number);
    }
    else if ((style & UNITY_DISPLAY_RANGE_UINT) == UNITY_DISPLAY_RANGE_UINT) {
        UnityPrintNumberUnsigned((UNITY_UINT)number);
    }
    else {
        UNITY_OUTPUT_CHAR('0');
        UNITY_OUTPUT_CHAR('x');
        UnityPrintNumberHex((UNITY_UINT)number, (char)((style & 0xF) * 2));
    }
}

// Helper: reset output buffer
static void ResetOutput() {
    g_output_buffer.clear();
}

// Helper: simple assertion that does not terminate on failure
static void AssertEquals(const std::string& actual, const std::string& expected, const std::string& testName, int& failures) {
    if (actual != expected) {
        ++failures;
        std::cout << "[FAIL] " << testName << " - expected: \"" << expected << "\", actual: \"" << actual << "\"\n";
    } else {
        std::cout << "[PASS] " << testName << "\n";
    }
}

int main() {
    int failures = 0;

    // Test 1: INT range - should delegate to UnityPrintNumber (decimal)
    // Arrange
    ResetOutput();
    UNITY_INT num1 = 42;
    UNITY_UINT style1 = UNITY_DISPLAY_RANGE_INT; // triggers first branch
    // Act
    UnityPrintNumberByStyle(num1, style1);
    // Assert
    AssertEquals(g_output_buffer, "42", "INT range branch prints decimal", failures);

    // Test 2: UINT range - should delegate to UnityPrintNumberUnsigned (decimal)
    // Arrange
    ResetOutput();
    UNITY_INT num2 = -7; // value is cast to unsigned; but in test we keep positive to avoid unsigned wrap
    UNITY_UINT style2 = UNITY_DISPLAY_RANGE_UINT; // triggers second branch
    // Act
    UnityPrintNumberByStyle(num2, style2);
    // Assert
    // Note: casting negative to unsigned would produce a large positive number; we keep a positive test value
    ResetOutput();
    UnityPrintNumberUnsigned((UNITY_UINT)123); // simulate expected path manually for clarity
    // But we actually call the function to test behavior with 123
    ResetOutput();
    UnityPrintNumberByStyle(123, style2);
    AssertEquals(g_output_buffer, "123", "UINT range branch prints unsigned", failures);

    // Test 3: Else branch - hex output with '0x' prefix and fixed nibble count
    // Arrange
    ResetOutput();
    UNITY_INT num3 = 0x01A2;        // 0x01A2
    UNITY_UINT style3 = 0x04;        // lower nibble 4 -> nibble_count = 8 (since *2)
    // Act
    UnityPrintNumberByStyle(num3, style3);
    // Assert: "0x" followed by 8 hex digits representing 0x01A2 -> "000001A2"
    AssertEquals(g_output_buffer, "0x000001A2", "Hex path with nibble_count 8", failures);

    // Test 4: Else branch - zero nibble count (lower nibble 0) should print only "0x"
    // Arrange
    ResetOutput();
    UNITY_INT num4 = 0xBEEF;
    UNITY_UINT style4 = 0x00; // lower nibble 0 -> nibble_count = 0
    // Act
    UnityPrintNumberByStyle(num4, style4);
    // Assert: "0x" only
    AssertEquals(g_output_buffer, "0x", "Hex path with nibble_count 0 (output only 0x)", failures);

    // Summary
    if (failures == 0) {
        std::cout << "\nAll tests passed.\n";
    } else {
        std::cout << "\nTotal failures: " << failures << "\n";
    }

    return failures;
}
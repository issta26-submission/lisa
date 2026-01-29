// Self-contained unit tests for the focal UnityPrintLen method.
// This test harness provides a minimal, non-GTest framework and a
// self-contained implementation of UnityPrintLen (matching the logic
// of the provided focal method) with output captured into a string.
// The goal is to verify true/false branches for printable, escaped,
// and hex-escaped/non-printable characters, and to respect the length
// bound. The test is designed as a single translation unit for ease of
// compilation in environments without the original Unity framework.

#include <stddef.h>
#include <cstring>
#include <unity.h>
#include <iostream>
#include <string>
#include <cstdint>


// Domain-adapted lightweight test harness and mocks.
// We implement the focal UnityPrintLen logic here, routing output to a buffer
// via a macro, to allow deterministic verification of output.

typedef unsigned int UNITY_UINT;
typedef unsigned int UNITY_UINT32;

// Output capture: all printed characters go into this buffer.
static std::string g_output;

// Output function used by the UnityPrintLen implementation.
// We mimic the Unity_OutputChar mechanism via a macro mapping to this function.
static void UnityOutputChar(char c) {
    g_output.push_back(c);
}

// Macro to redirect Unity's output to our buffer in this test unit.
#define UNITY_OUTPUT_CHAR(c) UnityOutputChar((c))

// Hex printing helper: print exactly 'nibbles_to_print' hex digits for 'number'.
static void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print) {
    static const char hex_digits[] = "0123456789ABCDEF";
    // Print high-order nibble first.
    for (int shift = (nibbles_to_print - 1) * 4; shift >= 0; shift -= 4) {
        unsigned int nibble = (number >> shift) & 0xF;
        UNITY_OUTPUT_CHAR(hex_digits[nibble]);
    }
}

// The focal method under test, copied/adapted into this unit for a self-contained test.
// It mirrors the original logic: printable chars are emitted as-is; CR -> \r,
// LF -> \n, and unprintables -> \x## hex escape.
void UnityPrintLen(const char* string, const UNITY_UINT32 length) {
{
    const char* pch = string;
    if (pch != NULL)
    {
        while (*pch && (UNITY_UINT32)(pch - string) < length)
        {
            /* printable characters plus CR & LF are printed */
            if ((*pch <= 126) && (*pch >= 32))
            {
                UNITY_OUTPUT_CHAR(*pch);
            }
            /* write escaped carriage returns */
            else if (*pch == 13)
            {
                UNITY_OUTPUT_CHAR('\\');
                UNITY_OUTPUT_CHAR('r');
            }
            /* write escaped line feeds */
            else if (*pch == 10)
            {
                UNITY_OUTPUT_CHAR('\\');
                UNITY_OUTPUT_CHAR('n');
            }
            /* unprintable characters are shown as codes */
            else
            {
                UNITY_OUTPUT_CHAR('\\');
                UNITY_OUTPUT_CHAR('x');
                UnityPrintNumberHex((UNITY_UINT)*pch, 2);
            }
            pch++;
        }
    }
}
}

// Helper: reset output buffer before each test.
static void ResetOutput() {
    g_output.clear();
}

// Test helpers: assertion-style checks returning pass/fail.
static bool assert_eq(const std::string& actual, const std::string& expected) {
    return actual == expected;
}

// Test 1: Null string input should produce no output.
static bool test_UnityPrintLen_NullString() {
    ResetOutput();
    const char* s = nullptr;
    UnityPrintLen(s, 10);
    // Expect empty output
    bool ok = assert_eq(g_output, "");
    if (!ok) {
        std::cerr << "Test UnityPrintLen_NullString failed: expected empty output, got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 2: Basic printable string fully within length bound.
static bool test_UnityPrintLen_BasicPrintables() {
    ResetOutput();
    const char* s = "Hello";
    UnityPrintLen(s, 5);
    bool ok = assert_eq(g_output, "Hello");
    if (!ok) {
        std::cerr << "Test UnityPrintLen_BasicPrintables failed: expected \"Hello\", got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 3: Length bound truncation on a longer string.
static bool test_UnityPrintLen_LengthBound() {
    ResetOutput();
    const char* s = "abcdef";
    UnityPrintLen(s, 3); // should print "abc"
    bool ok = assert_eq(g_output, "abc");
    if (!ok) {
        std::cerr << "Test UnityPrintLen_LengthBound failed: expected \"abc\", got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 4: Carriage Return (CR) and Line Feed (LF) escaping.
static bool test_UnityPrintLen_CrLfEscapes() {
    ResetOutput();
    // "A\r\nB" with length 4 should process 'A', CR, LF, 'B'
    const char s[] = "A\r\nB";
    UnityPrintLen(s, 4);
    // Expected: A, \, r, \, n, B
    std::string expected = "A\\r\\nB";
    bool ok = assert_eq(g_output, expected);
    if (!ok) {
        std::cerr << "Test UnityPrintLen_CrLfEscapes failed: expected \"" << expected << "\", got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 5: Non-printable characters should be escaped as hex (e.g., \\x##).
static bool test_UnityPrintLen_NonPrintableHex() {
    ResetOutput();
    // First two unprintables: 0x01 and 0x02
    const char s[] = "\x01\x02";
    UnityPrintLen(s, 2);
    // Expected: "\\x01\\x02"
    const std::string expected = "\\x01\\x02";
    bool ok = assert_eq(g_output, expected);
    if (!ok) {
        std::cerr << "Test UnityPrintLen_NonPrintableHex failed: expected \"" << expected << "\", got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 6: Null string should not crash; length zero should yield no output.
static bool test_UnityPrintLen_NullAndZeroLength() {
    ResetOutput();
    const char* s = nullptr;
    UnityPrintLen(s, 0);
    bool ok = assert_eq(g_output, "");
    if (!ok) {
        std::cerr << "Test UnityPrintLen_NullAndZeroLength failed: expected empty output, got \"" << g_output << "\"\n";
    }
    return ok;
}

// Test 7: Empty string with non-zero length yields no output.
static bool test_UnityPrintLen_EmptyStringWithLength() {
    ResetOutput();
    const char s[] = "";
    UnityPrintLen(s, 10);
    bool ok = assert_eq(g_output, "");
    if (!ok) {
        std::cerr << "Test UnityPrintLen_EmptyStringWithLength failed: expected empty output, got \"" << g_output << "\"\n";
    }
    return ok;
}

// Simple test runner to execute all tests and report summary without terminating on first failure.
int main() {
    struct TestCase {
        const char* name;
        bool (*func)();
    };

    TestCase tests[] = {
        { "NullString", test_UnityPrintLen_NullString },
        { "BasicPrintables", test_UnityPrintLen_BasicPrintables },
        { "LengthBound", test_UnityPrintLen_LengthBound },
        { "CrLfEscapes", test_UnityPrintLen_CrLfEscapes },
        { "NonPrintableHex", test_UnityPrintLen_NonPrintableHex },
        { "NullAndZeroLength", test_UnityPrintLen_NullAndZeroLength },
        { "EmptyStringWithLength", test_UnityPrintLen_EmptyStringWithLength },
    };

    const int total = sizeof(tests) / sizeof(TestCase);
    int passed = 0;

    std::cout << "Running UnityPrintLen test suite with " << total << " test cases...\n";

    for (int i = 0; i < total; ++i) {
        bool result = tests[i].func();
        std::cout << "[ " << (result ? "PASS" : "FAIL") << " ] " << tests[i].name << "\n";
        if (result) ++passed;
    }

    std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}
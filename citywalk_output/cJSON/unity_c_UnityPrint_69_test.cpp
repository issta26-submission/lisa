// C++11 test harness for UnityPrint (from unity.c) without using GTest.
// This file defines minimal Unity dependencies required by UnityPrint,
// captures output through a custom UNITY_OUTPUT_CHAR, and validates
// several scenarios including printable chars, escaped CR/LF, hex escapes,
// NULL input, and ANSI color escape handling when enabled.

#include <stddef.h>
#include <cstring>
#include <unity.h>
#include <iostream>
#include <string>


// Enable color escape handling in UnityPrint to test the color path.
#define UNITY_OUTPUT_COLOR 1

// -----------------------------------------------------------------------------
// Minimal Unity dependencies required by UnityPrint (re-implemented for tests)
// -----------------------------------------------------------------------------

typedef unsigned int UNITY_UINT;
typedef int UNITY_LINE_TYPE;
typedef int UNITY_DISPLAY_STYLE_T;
typedef int UNITY_FLAGS_T;
typedef double UNITY_DOUBLE;
typedef int UNITY_INT;
typedef unsigned int UNITY_UINT32;

// To capture output from UnityPrint, redirect UNITY_OUTPUT_CHAR to a function
static std::string g_output_capture;

// The function used by UnityPrint to emit a single character.
static void UnityOutputChar(char c)
{
    g_output_capture.push_back(c);
}

// Macro redirection used by UnityPrint to output a single character.
#define UNITY_OUTPUT_CHAR(c) UnityOutputChar((char)(c))

// Hex printing helper used by UnityPrint when encountering unprintable chars.
void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print)
{
    static const char hex_digits[] = "0123456789ABCDEF";
    int n = static_cast<int>(nibbles_to_print);
    for (int i = n - 1; i >= 0; --i)
    {
        int nibble = (number >> (i * 4)) & 0xF;
        UNITY_OUTPUT_CHAR(hex_digits[nibble]);
    }
}

// The UnityPrint function under test (pulled from the focal method).
void UnityPrint(const char* string)
{
{
    const char* pch = string;
    if (pch != NULL)
    {
        while (*pch)
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
#ifdef UNITY_OUTPUT_COLOR
            /* print ANSI escape code */
            else if (*pch == 27 && *(pch + 1) == '[')
            {
                while (*pch && *pch != 'm')
                {
                    UNITY_OUTPUT_CHAR(*pch);
                    pch++;
                }
                UNITY_OUTPUT_CHAR('m');
            }
#endif
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

// -----------------------------------------------------------------------------
// Simple non-terminating test framework (no GTest/GMock). This mirrors common
// unit-test style with expectations and summary reporting.
// -----------------------------------------------------------------------------

static int g_total = 0;
static int g_passed = 0;

static void ExpectEquals(const std::string& actual, const std::string& expected, const std::string& testName)
{
    g_total++;
    if (actual == expected)
    {
        g_passed++;
    }
    else
    {
        std::cerr << "Test failed: " << testName << "\n"
                  << "  Expected: \"" << expected << "\"\n"
                  << "  Actual:   \"" << actual << "\"\n";
    }
}

// Test 1: Printable characters are emitted as-is
static void test_printable_characters()
{
    g_output_capture.clear();
    const char* input = "Hello, Unity!";
    UnityPrint(input);
    ExpectEquals(g_output_capture, "Hello, Unity!", "test_printable_characters");
}

// Test 2: CR and LF are escaped as \\r and \\n respectively
static void test_crlf_escaping()
{
    g_output_capture.clear();
    const char input[] = "A\rB\nC"; // includes CR and LF
    UnityPrint(input);
    ExpectEquals(g_output_capture, "A\\rB\\nC", "test_crlf_escaping");
}

// Test 3: Unprintable characters are shown as hex escape sequences
static void test_unprintable_hex()
{
    g_output_capture.clear();
    // Raw bytes 0x01 and 0x02
    const char input[] = { 0x01, 0x02, 0x00 };
    UnityPrint(input);
    ExpectEquals(g_output_capture, "\\x01\\x02", "test_unprintable_hex");
}

// Test 4: NULL input should produce no output
static void test_null_input()
{
    g_output_capture.clear();
    UnityPrint(nullptr);
    ExpectEquals(g_output_capture, "", "test_null_input");
}

// Test 5: ANSI color escape sequence handling when UNITY_OUTPUT_COLOR is enabled
// We feed a color start sequence and an ASCII character to ensure the color path
// is exercised and subsequent printable character is emitted.
static void test_ansi_color_escape_path()
{
    g_output_capture.clear();
    // ESC[31mA  -> should emit ESC, [, 3, 1, m, 'A' (since color path prints until 'm')
    const char input[] = "\x1B[31mA";
    UnityPrint(input);
    std::string expected;
    expected.push_back('\x1B'); // ESC
    expected += "[31mA";
    ExpectEquals(g_output_capture, expected, "test_ansi_color_escape_path");
}

// Test 6: Mixed printable and control characters within a string
static void test_mixed_content()
{
    g_output_capture.clear();
    // "Z" printable, followed by 0x7F (DEL, unprintable), followed by 0x0A (LF)
    // Expected: "Z\x5C"x7F"\\n" where \x7F becomes \x7F and LF becomes \n escaped
    const char input[] = {'Z', 0x7F, 0x0A, 0x00};
    UnityPrint(input);
    // 0x7F -> backslash x7F, 0x0A -> \n
    const std::string expected = "Z\\x7F\\n";
    ExpectEquals(g_output_capture, expected, "test_mixed_content");
}

// Run all tests and report summary
static void RunAllTests()
{
    test_printable_characters();
    test_crlf_escaping();
    test_unprintable_hex();
    test_null_input();
    test_ansi_color_escape_path();
    test_mixed_content();

    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << std::endl;
}

// Entry point
int main()
{
    RunAllTests();
    return (g_total == g_passed) ? 0 : 1;
}
// C++11 test suite for the focal method: emit_string(const char *str, FILE *out)
// This test suite does not rely on any heavy testing framework (no GTest).
// It captures the output of emit_string into a temporary file and compares
// it against the expected string. Each test case includes a short explanation.
//
// Prerequisites: The test binary is linked with the implementation file that
// defines emit_string (pngfix.c). The function signature:
//    void emit_string(const char *str, FILE *out);
//
// Note: We use a memory-backed test via tmpfile() to avoid filesystem side effects.

#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Declaration of the focal function from the C source.
// Use extern "C" to avoid C++ name mangling.
extern "C" void emit_string(const char *str, FILE *out);

// Simple non-terminating test harness (no exceptions, no early exit).
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Helper: run a single test case.
// - input_bytes: raw bytes to feed into emit_string (may include non-printables).
// - len: length of input_bytes.
// - expected: expected string output (as it would appear in a file).
// - test_name: descriptive name for logging.
// Returns true if test passes, false otherwise.
bool run_emit_string_test(const unsigned char* input_bytes, size_t len, const char* expected, const char* test_name) {
    // Create a C-string from input_bytes for emit_string (null-terminated).
    char* input_str = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        input_str[i] = static_cast<char>(input_bytes[i]);
    }
    input_str[len] = '\0';

    // Create a temporary file to capture output.
    FILE* out = tmpfile();
    if (!out) {
        std::cerr << "[ERROR] Failed to create temporary file for test: " << test_name << "\n";
        delete[] input_str;
        return false;
    }

    // Call the focal function.
    emit_string(input_str, out);

    // Rewind and read the output.
    rewind(out);
    std::string actual;
    int ch;
    while ((ch = fgetc(out)) != EOF) {
        actual.push_back(static_cast<char>(ch));
    }

    // Cleanup.
    fclose(out);
    delete[] input_str;

    // Compare actual vs expected.
    bool ok = (actual == std::string(expected));

    // Report result.
    ++g_tests_run;
    if (ok) {
        ++g_tests_passed;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        ++g_tests_failed;
        std::cout << "[FAIL] " << test_name << "\n";
        std::cout << "       Expected: \"" << expected << "\"\n";
        std::cout << "       Actual  : \"" << actual << "\"\n";
    }
    return ok;
}

int main() {
    // Test 1: Simple graph characters with a single space.
    // Input: "Hello World" -> spaces become underscores.
    // Expected: "Hello_World"
    const unsigned char in1[] = { 'H','e','l','l','o',' ', 'W','o','r','l','d', 0 };
    run_emit_string_test(in1, sizeof(in1) - 1, "Hello_World", "Test 1: Graphical chars with a space replaced by '_'");

    // Test 2: Non-graph, non-space character should be emitted as an octal escape.
    // Input: {'A', 0x01, 'B'} -> "A\001B"
    const unsigned char in2[] = { 'A', 0x01, 'B', 0 };
    run_emit_string_test(in2, sizeof(in2) - 1, "A\\001B", "Test 2: Non-graphic char encoded as octal escape (\\001)");

    // Test 3: Whitespace characters (tab) should be replaced with underscore.
    // Input: "A\tB" (A, tab, B) -> "A_B"
    const unsigned char in3[] = { 'A', '\t', 'B', 0 };
    run_emit_string_test(in3, sizeof(in3) - 1, "A_B", "Test 3: Tab character replaced with '_'");

    // Test 4: Only spaces should be converted to underscores.
    // Input: "   " (three spaces) -> "___"
    const unsigned char in4[] = { ' ', ' ', ' ', 0 };
    run_emit_string_test(in4, sizeof(in4) - 1, "___", "Test 4: All spaces converted to underscores");

    // Summary
    std::cout << "\nTests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << "\n";

    // Return non-zero if any test failed.
    return (g_tests_failed == 0) ? 0 : 1;
}
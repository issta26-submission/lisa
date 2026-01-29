// Unit test for the focal method WriteByte in cmsps2.c
// This test uses a lightweight harness (no GTest) and a macro-based
// interception of _cmsIOPrintf to capture output for verification.
// The cmsps2.c file is included in this translation unit with certain
// macros overridden to allow isolation of behavior.
//
// Key behaviors tested:
// - True branch: when _cmsPSActualColumn > MAXPSCOLS after a write, a newline is emitted
// - False branch: when the condition is not met, no newline is emitted
// - Correct hex formatting of the byte ("%02x") in lowercase
//
// Note: This test assumes MAXPSCOLS and _cmsPSActualColumn exist in cmsps2.c
// (as per the provided focal class dependencies). We override MAXPSCOLS to a small
// value (4) to exercise the branching logic in WriteByte.

#include <cstdio>
#include <cmsps2.c>
#include <string>
#include <cstdarg>
#include <cassert>
#include <lcms2_internal.h>


// Override the macro controlling the column limit to small value for deterministic tests
#define MAXPSCOLS 4

// Intercept _cmsIOPrintf calls and capture the output into a string for assertions.
// We implement a lightweight test printf that ignores the cmsIOHANDLER* parameter (m).
static std::string g_output;

// Test harness: mimic variadic printf formatting and accumulate into g_output
static void cms_test_printf_f(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    g_output += buffer;
}

// Redirect _cmsIOPrintf to the test harness.
// The focal code calls _cmsIOPrintf(m, "%02x", b) or _cmsIOPrintf(m, "\n"), etc.
// We provide a macro to replace _cmsIOPrintf in cmsps2.c with our harness.
#define _cmsIOPrintf(m, fmt, ...) cms_test_printf_f(fmt, ##__VA_ARGS__)

// Include the focal module source. This will use the macro above and link against the test harness.

int main() {
    // Clear output and rely on initial state of _cmsPSActualColumn (expected to be 0 at startup)
    g_output.clear();

    // Test 1: First write (no newline expected yet, false branch)
    // Input: 0x12 -> expected output "12"
    WriteByte(nullptr, 0x12);
    assert(g_output == "12"); // true branch not yet taken; two hex digits printed

    // Test 2: Second write (still false branch)
    // Input: 0x34 -> expected output "1234"
    WriteByte(nullptr, 0x34);
    assert(g_output == "1234"); // no newline yet, two more hex digits appended

    // Test 3: Third write triggers newline (true branch)
    // Input: 0x56 -> expected output "123456\n"
    WriteByte(nullptr, 0x56);
    assert(g_output == "123456\n"); // newline emitted after exceeding MAXPSCOLS, column reset

    // Test 4: After newline, next write resets column and prints next two hex digits
    // Input: 0xAA -> expected output "123456\naa"
    WriteByte(nullptr, 0xAA);
    assert(g_output == "123456\naa");

    // Test 5: Subsequent write continues without newline until next exceed
    // Input: 0xBB -> expected output "123456\naabb"
    WriteByte(nullptr, 0xBB);
    assert(g_output == "123456\naabb");

    // Test 6: Final write to trigger another newline after exceeding MAXPSCOLS again
    // Input: 0xCC -> expected output "123456\naabbcc\n"
    WriteByte(nullptr, 0xCC);
    assert(g_output == "123456\naabbcc\n");

    // If all assertions pass, print a success message (optional)
    // Note: The instruction discourages extra output, but a single success message can help.
    // Comment out if a silent pass is preferred.
    // printf("All WriteByte tests passed.\n");

    return 0;
}
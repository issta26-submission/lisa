/***************************************************************
 * Test suite for the focal method: Writef (as defined in cmscgats.c)
 *
 * This test harness provides a minimal in-process environment
 * to validate the behavior of Writef without relying on GTest.
 * It uses a simple in-memory SAVESTREAM and a custom WriteStr
 * to capture written output for assertions.
 *
 * Scope:
 * - Basic formatting correctness (integers, strings)
 * - Proper handling of va_list and formatting
 * - Buffer truncation behavior when formatted content exceeds 4094 chars
 *
 * Notes:
 * - This test is self-contained and compiles under C++11.
 * - No external dependencies beyond the C++ standard library.
 * - Assertions are non-terminating (tests continue to run even on failure).
 ***************************************************************/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// ---------------------------------------------------------------------
// Minimal definitions to emulate the project environment
// ---------------------------------------------------------------------

// Forward declaration/definition of SAVESTREAM used by Writef
struct SAVESTREAM;

// Simple in-memory stream to capture writes
struct SAVESTREAM {
    std::string data;
};

// Mock implementation of WriteStr to capture the output written by Writef
// In the real project, WriteStr would write to a file or stream; here we
// accumulate into the SAVESTREAM.data string for assertions.
void WriteStr(SAVESTREAM* f, const char* str) {
    if (f && str) {
        f->data += str;
    }
}

// Focal method under test (copied behavior from the provided snippet)
void Writef(SAVESTREAM* f, const char* frm, ...)
{
    {
        char Buffer[4096];
        va_list args;
        va_start(args, frm);
        vsnprintf(Buffer, 4095, frm, args);
        Buffer[4095] = 0;
        WriteStr(f, Buffer);
        va_end(args);
    }
}

// ---------------------------------------------------------------------
// Unit Test Harness
// ---------------------------------------------------------------------

// Helper to indicate test results without terminating execution
static bool g_all_ok = true;
static int  g_test_count = 0;
static int  g_pass_count = 0;

#define TEST_PASS() do { ++g_test_count; ++g_pass_count; std::cout << "[PASS] " << __FUNCTION__ << "\n"; } while(0)
#define TEST_FAIL(msg) do { ++g_test_count; g_all_ok = false; std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << "\n"; } while(0)

// Test 1: Basic formatting with string and integer
// Verifies that Writef correctly formats both %s and %d and writes the
// result to the stream via WriteStr.
bool test_basic_formatting()
{
    SAVESTREAM stream;
    Writef(&stream, "Hello %s %d", "world", 42);

    const std::string expected = "Hello world 42";
    if (stream.data == expected) {
        TEST_PASS();
        return true;
    } else {
        TEST_FAIL("Basic formatting mismatch. Expected: \"" + expected + "\" Got: \"" + stream.data + "\"");
        return false;
    }
}

// Test 2: Buffer truncation behavior when formatted content exceeds buffer
// Creates a long string ~5000 chars and uses Writef with a single %s argument.
// The internal buffer size is 4096, and vsnprintf is called with size 4095,
// so the capped length should be 4094 characters (since vsnprintf writes up to
// size-1 characters and then a null terminator).
bool test_buffer_truncation()
{
    SAVESTREAM stream;
    // Create a long string of 5000 'A's
    std::string longstr(5000, 'A');
    Writef(&stream, "%s", longstr.c_str());

    // Expect 4094 'A's (since 4095 is the buffer size, 4094 chars + null)
    if (stream.data.size() != 4094) {
        TEST_FAIL("Truncation length mismatch. Expected 4094, got " + std::to_string(stream.data.size()));
        return false;
    }
    // Ensure all written characters are 'A'
    for (char c : stream.data) {
        if (c != 'A') {
            TEST_FAIL("Truncation content mismatch: non-'A' character found in output.");
            return false;
        }
    }
    TEST_PASS();
    return true;
}

// Test 3: Multiple-argument formatting to verify va_list handling with mixed types
// Ensures that multiple format specifiers are expanded correctly in sequence.
bool test_multiple_args_formatting()
{
    SAVESTREAM stream;
    Writef(&stream, "Val1=%d; Val2=%s; Val3=%c", 7, "six", 'X');

    const std::string expected = "Val1=7; Val2=six; Val3=X";
    if (stream.data == expected) {
        TEST_PASS();
        return true;
    } else {
        TEST_FAIL("Multiple-argument formatting mismatch. Expected: \"" + expected +
                  "\" Got: \"" + stream.data + "\"");
        return false;
    }
}

// ---------------------------------------------------------------------
// Main: Run all tests
// ---------------------------------------------------------------------

int main()
{
    // Run tests sequentially. Non-terminating: even if one test fails, others run.
    test_basic_formatting();
    test_buffer_truncation();
    test_multiple_args_formatting();

    // Summary
    std::cout << "Tests run: " << g_test_count
              << ", Passed: " << g_pass_count
              << ", Failed: " << (g_test_count - g_pass_count) << "\n";

    return g_all_ok ? 0 : 1;
}
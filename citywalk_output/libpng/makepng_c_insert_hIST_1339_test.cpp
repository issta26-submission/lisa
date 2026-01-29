// test_insert_hIST.cpp
// A lightweight, self-contained unit test suite for the focal method
// insert_hIST located in makepng.c, using a minimal custom test harness
// without GoogleTest. The tests exercise both the success and failure paths
// by intercepting calls to exit() and to png_set_hIST via a small mock.
//
// How this test works (notes for maintainers):
// - The test includes the contents of makepng.c inside extern "C" to ensure
//   C linkage for the focal function and any helper symbols.
// - A tiny fake PNG interface is provided (types and a mock png_set_hIST) to
//   avoid requiring a real libpng installation at test time.
// - exit() calls inside the focal code are redirected to mock_exit(), which
//   uses setjmp/longjmp to unwind control back to the test harness without
//   terminating the entire test process.
// - Two kinds of tests are covered: valid input scenarios (no exit, proper
//   freq values populated) and invalid input scenarios (exit(1) invoked).
//
// Build instructions (example):
// - Ensure the test runner is compiled in a C++11 capable toolchain.
// - Place this test file in the same directory as makepng.c and provide a
//   compatible C environment for the included code (the test includes
//   makepng.c directly).
// - Compile with: g++ -std=c++11 test_insert_hIST.cpp -o test_insert_hIST
//   (Make sure the build environment allows including makepng.c as shown.)

#include <cstdint>
#include <makepng.c>
#include <stdint.h>
#include <setjmp.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Minimal fake PNG interface (to satisfy types used by insert_hIST)
typedef void* png_structp;
typedef void* png_infop;
typedef char** png_charpp;
typedef uint16_t png_uint_16;

// Global state used by tests to verify behavior of insert_hIST
static png_uint_16 captured_freq[256];
static int hist_set_called = 0;
static int mock_exit_called = 0;
static int mock_exit_code = 0;
static jmp_buf test_env;

// Forward declaration to satisfy potential C linkage expectations
extern "C" void insert_hIST(png_structp png_ptr, png_infop info_ptr, int nparams,
                           png_charpp params);

// Mock function that replaces libpng's png_set_hIST
extern "C" void MOCK_png_set_hIST(png_structp png_ptr, png_infop info_ptr,
                                 png_uint_16 freq[256])
{
    hist_set_called = 1;
    // Record the first 256 frequencies for verification
    memcpy(captured_freq, freq, sizeof(captured_freq));
}

// Overriding libpng's exit() with a test-friendly mock
static void mock_exit(int code)
{
    mock_exit_called = 1;
    mock_exit_code = code;
    longjmp(test_env, 1);
}

// Map calls to exit(...) inside the focal code to mock_exit
#define exit(code) mock_exit(code)

// Map calls to png_set_hIST(...) inside the focal code to MOCK_png_set_hIST
#define png_set_hIST MOCK_png_set_hIST

// Provide a tiny wrapper around the C file to ensure C linkage for the focal API
extern "C" {
}

// Reset all test state before each test
static void reset_test_state()
{
    memset(captured_freq, 0, sizeof(captured_freq));
    hist_set_called = 0;
    mock_exit_called = 0;
    mock_exit_code = 0;
}

// Test 1: Valid input with three numeric parameters within range
// Expectation: insert_hIST should populate freq[0..2] with the provided values
static bool test_valid_three_params()
{
    reset_test_state();

    // Prepare input: "1", "2", "3"
    static char a0[] = "1";
    static char a1[] = "2";
    static char a2[] = "3";
    char* params[3] = { a0, a1, a2 };
    int nparams = 3;

    if (setjmp(test_env) == 0) {
        // Call the focal function
        insert_hIST((png_structp)nullptr, (png_infop)nullptr, nparams, params);

        // Verify that png_set_hIST was invoked and values were passed correctly
        bool ok = hist_set_called != 0;
        ok = ok && (captured_freq[0] == 1);
        ok = ok && (captured_freq[1] == 2);
        ok = ok && (captured_freq[2] == 3);
        // Ensure remaining entries are zero (as memset covered the entire 256 array)
        for (int i = 3; i < 256; ++i) {
            if (captured_freq[i] != 0) {
                ok = false;
                break;
            }
        }
        return ok;
    } else {
        // Should not exit in this test
        return false;
    }
}

// Test 2: Invalid input (non-numeric character) should trigger exit(1)
static bool test_invalid_non_numeric()
{
    reset_test_state();

    // Prepare input: "1", "x" (invalid)
    static char a0[] = "1";
    static char a1[] = "x";
    char* params[2] = { a0, a1 };
    int nparams = 2;

    if (setjmp(test_env) == 0) {
        insert_hIST((png_structp)nullptr, (png_infop)nullptr, nparams, params);
        // If we get here, no exit occurred, which is a failure for this test
        return false;
    } else {
        // We jumped back due to mock_exit; verify exit was called with code 1
        return (mock_exit_called && mock_exit_code == 1);
    }
}

// Test 3: Empty string parameter should trigger exit(1)
static bool test_invalid_empty_param()
{
    reset_test_state();

    // Prepare input: "" (empty)
    static char a0[] = "";
    char* params[1] = { a0 };
    int nparams = 1;

    if (setjmp(test_env) == 0) {
        insert_hIST((png_structp)nullptr, (png_infop)nullptr, nparams, params);
        return false; // should exit
    } else {
        return (mock_exit_called && mock_exit_code == 1);
    }
}

// Test 4: Value greater than 65535 should trigger exit(1)
static bool test_invalid_overflow_value()
{
    reset_test_state();

    // Prepare input: "70000" which is > 65535
    static char a0[] = "70000";
    char* params[1] = { a0 };
    int nparams = 1;

    if (setjmp(test_env) == 0) {
        insert_hIST((png_structp)nullptr, (png_infop)nullptr, nparams, params);
        return false; // should exit
    } else {
        return (mock_exit_called && mock_exit_code == 1);
    }
}

// Helper function to run a single test and print its result
static void run_and_report(const char* test_name, bool (*test_fn)())
{
    bool result = test_fn();
    if (result) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

// Main function: runs all tests and prints a summary
int main()
{
    printf("Running tests for insert_hIST...\n");

    run_and_report("test_valid_three_params", test_valid_three_params);
    run_and_report("test_invalid_non_numeric", test_invalid_non_numeric);
    run_and_report("test_invalid_empty_param", test_invalid_empty_param);
    run_and_report("test_invalid_overflow_value", test_invalid_overflow_value);

    printf("All tests completed.\n");
    return 0;
}
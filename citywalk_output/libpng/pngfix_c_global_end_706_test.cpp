// C++ unit test suite for the focal method global_end in pngfix.c
// This test suite does not rely on GTest. It uses a lightweight, non-terminating assertion approach.
// It assumes the project provides a C header (pngfix.h) that exposes the necessary types
// (struct global, struct IDAT_list, and prototypes for global_end, global_init, and IDAT_list_init).

#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <pngfix.h>
#include <stdio.h>
#include <ctype.h>


// Bring C declarations into the C++ test code.
// We assume the project provides pngfix.h with the required declarations.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void test_fail(const char* test_name, const char* message) {
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
}

static void test_pass(const char* test_name) {
    ++g_total_tests;
    std::cout << "[PASS] " << test_name << std::endl;
}

// Test 1: Verify that global_end returns the current status_code value before CLEAR mutates it.
// Rationale: CLEAR(*global) happens after capturing the return code; the function should return
// the value that was in global->status_code prior to clearing.
static bool test_global_end_returns_own_status_code() {
    const char* test_name = "test_global_end_returns_own_status_code";

    // Prepare a fresh global object using library-provided initializer.
    struct global g;
    global_init(&g);

    // Initialize internal state that the function relies on.
    // The idat_cache must be in a valid but empty state for IDAT_list_end to operate safely.
    IDAT_list_init(&g.idat_cache);

    // Choose a representative status code value and assign it.
    const int test_value = 1234;
    g.status_code = test_value;

    // Call the focal method
    int rc = global_end(&g);

    // Basic assertion: the return value should equal the pre-existing status_code value.
    if (rc != test_value) {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "expected rc == %d, got rc == %d",
                 test_value, rc);
        test_fail(test_name, msg);
        return false;
    }

    test_pass(test_name);
    return true;
}

// Test 2: Verify that global_end correctly handles multiple status_code values across calls.
// This exercises the function with different inputs to ensure consistent behavior.
static bool test_global_end_returns_value_for_various_statuses() {
    const char* test_name = "test_global_end_returns_value_for_various_statuses";

    // Prepare a fresh global object
    struct global g;
    global_init(&g);
    IDAT_list_init(&g.idat_cache);

    // Test a small set of representative values, including edge-like and negative numbers.
    const int test_values[] = {0, -1, 7, -12345, 99999};
    const size_t count = sizeof(test_values) / sizeof(test_values[0]);
    bool all_pass = true;

    for (size_t i = 0; i < count; ++i) {
        int val = test_values[i];

        // Reinitialize for each iteration to simulate separate runs
        global_init(&g);
        IDAT_list_init(&g.idat_cache);
        g.status_code = val;

        int rc = global_end(&g);
        if (rc != val) {
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "on iteration %zu: expected rc == %d, got rc == %d",
                     i, val, rc);
            test_fail(test_name, msg);
            all_pass = false;
            // Continue to test remaining values to maximize coverage
        }
    }

    if (all_pass) {
        test_pass(test_name);
    }

    return all_pass;
}

// Main function to drive tests
int main(int argc, char** argv) {
    (void)argc; (void)argv;

    // Run tests
    test_global_end_returns_own_status_code();
    test_global_end_returns_value_for_various_statuses();

    // Summary
    std::cout << "\nTest Summary: "
              << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;

    // Non-terminating test approach means we simply return 0 if all tests pass,
    // otherwise return a non-zero value to indicate failure.
    return (g_failed_tests == 0) ? 0 : 1;
}
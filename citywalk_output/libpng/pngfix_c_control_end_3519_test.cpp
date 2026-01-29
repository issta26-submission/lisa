// Self-contained unit tests for the focal method: control_end
// This test suite is designed to be compiled with a C++11 capable compiler
// without any external testing framework (no GTest). It mocks the minimal
// environment required by control_end and its dependent function file_end
// to validate the interaction between these components.
// The tests are non-terminating: failures are reported but do not abort
// subsequent tests, allowing full coverage across all tests.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// ------------------------------------------------------------------------------------
// Minimal mock environment to emulate the C structures and functions used by
// the focal method control_end in pngfix.c, without pulling in the full PNG library.
// This is a self-contained replica solely for unit testing purposes.
// ------------------------------------------------------------------------------------

// Mock definitions (mirroring the likely shape of the real code, but simplified)
extern "C" {

// Forward declarations to resemble the real API
typedef struct file {
    // Contents are irrelevant for the test; we merely need a distinct type.
    int dummy;
} file;

typedef struct control {
    file file; // The focal method expects a 'control' with a 'file' member
} control;

// The focal method under test (redeclared to mirror the real signature)
int control_end(struct control *control);

// The function invoked by control_end; in the real project this would perform
// file closing logic. For tests, we provide a mock that allows controlled outputs.
int file_end(struct file *file);
}

// ------------------------------------------------------------------------------------
// Test scaffolding and mocks

// Control the mock behavior of file_end via this global switch.
// The test can set this value to determine the return code of file_end.
static int file_end_return_value = 0;

// Capture the exact pointer passed to file_end to verify that control_end passes
// the address of control->file, not some other location.
static void* last_file_ptr = nullptr;

// Implementations of the mock functions (C linkage to mirror the real code)
extern "C" {

// Mock implementation of file_end: returns a value controlled by file_end_return_value
// and records the address received to validate correct pointer passing.
int file_end(struct file *file) {
    last_file_ptr = static_cast<void*>(file);
    return file_end_return_value;
}

// Focal method under test: it simply forwards to file_end(&control->file)
int control_end(struct control *control) {
    // The real code returns the result of file_end(&control->file)
    return file_end(&control->file);
}
}

// ------------------------------------------------------------------------------------
// Lightweight test framework (non-terminating)
// We implement a tiny test harness that uses plain C++ I/O and does not abort
// on assertion failures, allowing us to cover multiple scenarios in one run.

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Macro to perform an assertion without terminating the test run
#define ASSERT_TRUE(cond, msg) do {                               \
    ++tests_run;                                                    \
    if (cond) {                                                     \
        ++tests_passed;                                             \
    } else {                                                        \
        ++tests_failed;                                             \
        std::cerr << "Assertion failed: " << (msg) << std::endl;  \
    }                                                               \
} while (0)

// Helper macro to clearly separate test sections in output
#define TEST_SECTION(name) std::cout << "\n=== " << (name) << " ===" << std::endl;

// Test 1: Ensure control_end passes the address of control->file to file_end
// and returns the same value as file_end.
static void test_control_end_passes_file_address() {
    TEST_SECTION("control_end passes the address of control->file to file_end");

    // Arrange
    control c; // zero-initialized (default-constructed in C, here in C++ it's zeroed)
    file_end_return_value = 0;
    last_file_ptr = nullptr;

    // Act
    int rc = control_end(&c);

    // Assert
    ASSERT_TRUE(last_file_ptr == static_cast<void*>(&c.file),
                "control_end should pass the address of control->file to file_end");
    ASSERT_TRUE(rc == 0,
                "control_end should return the value provided by file_end (0) in this scenario");
}

// Test 2: Ensure control_end correctly propagates non-zero/negative values
// from file_end back to the caller.
static void test_control_end_propagates_return_value() {
    TEST_SECTION("control_end propagates file_end's return value");

    // Test with a non-zero value
    control c1;
    file_end_return_value = -17;
    last_file_ptr = nullptr;

    int rc1 = control_end(&c1);
    ASSERT_TRUE(rc1 == -17,
                "control_end should propagate non-zero return value from file_end");

    // Additionally verify that the address passed is still the internal file
    ASSERT_TRUE(last_file_ptr == static_cast<void*>(&c1.file),
                "control_end should pass the address of control->file to file_end (second case)");
}

// Test 3: Verify that multiple calls do not interfere with each other's state
static void test_control_end_multiple_calls_is_isolated() {
    TEST_SECTION("control_end maintains isolation across multiple calls");

    // First call with value A
    control cA;
    file_end_return_value = 42;
    last_file_ptr = nullptr;
    int rA = control_end(&cA);

    ASSERT_TRUE(rA == 42, "First call should return 42 when file_end_return_value = 42");
    ASSERT_TRUE(last_file_ptr == static_cast<void*>(&cA.file),
                "First call should pass address of cA.file to file_end");

    // Second call with value B
    control cB;
    file_end_return_value = -99;
    last_file_ptr = nullptr;
    int rB = control_end(&cB);

    ASSERT_TRUE(rB == -99, "Second call should return -99 when file_end_return_value = -99");
    ASSERT_TRUE(last_file_ptr == static_cast<void*>(&cB.file),
                "Second call should pass address of cB.file to file_end");
}

// ------------------------------------------------------------------------------------
// Main entry point to run all tests

int main() {
    std::cout << "Starting unit tests for focal method: control_end" << std::endl;

    test_control_end_passes_file_address();
    test_control_end_propagates_return_value();
    test_control_end_multiple_calls_is_isolated();

    // Summary
    std::cout << "\nTest Run Summary: "
              << "Total=" << tests_run
              << ", Passed=" << tests_passed
              << ", Failed=" << tests_failed
              << std::endl;

    // Return non-zero if any test failed to aid integration with CI systems
    return (tests_failed == 0) ? 0 : 1;
}
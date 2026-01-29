// Test suite for UnityOutputCharSpy_OutputChar using a minimal C++11 test harness
// No Google Test; tests run from main(). The goal is to exercise the focal method
// UnityOutputCharSpy_OutputChar(int c) and its dependent functions without GMock.
//
// Assumptions about the target (per provided code):
// - There are global/static variables: spy_enable, count, size, buffer (within unity_output_Spy.c).
// - The API exposed in unity_output_Spy.h includes:
//     void UnityOutputCharSpy_Create(int s);
//     void UnityOutputCharSpy_Destroy(void);
//     void UnityOutputCharSpy_OutputChar(int c);
//     const char * UnityOutputCharSpy_Get(void);
//     void UnityOutputCharSpy_Enable(int enable);
//
// The tests focus on: true/false branches of spy_enable and inner condition
// (count < (size-1)) and verify buffer content via UnityOutputCharSpy_Get().
// We do not rely on actual stdout output (putchar) to keep tests portable.

#include <cstring>
#include <unity_output_Spy.h>
#include <cstdio>
#include <string.h>
#include <unity_fixture.h>
#include <stdio.h>


// Import C functions (as C linkage) from the focal module
extern "C" {
}

// Simple testing harness (non-terminating assertions)
static int g_failures = 0;

// Helper to report a failure without aborting tests
static void report_failure(const char* test_name, const char* message) {
    fprintf(stderr, "[%s] FAILURE: %s\n", test_name, message);
    ++g_failures;
}

// Helper to perform a character equality assertion on a buffer element
static void expect_char_eq(const char* test_name, const char* buffer, int idx, char expected) {
    if (buffer[idx] != expected) {
        char msg[128];
        snprintf(msg, sizeof(msg), "buffer[%d] expected '%c', got '%c'", idx, expected, buffer[idx]);
        report_failure(test_name, msg);
    }
}

// Test 1: Appends characters when spy is enabled and buffer is not full
// - Purpose: cover true branch of spy_enable and inner condition (count < size-1)
static void test_Appends_When_Enabled_And_Not_Full() {
    const char* test_name = "Test_Appends_When_Enabled__Not_Full";
    UnityOutputCharSpy_Create(5);      // size = 5, so (size-1) = 4
    UnityOutputCharSpy_Enable(1);      // Enable spying to buffer path

    UnityOutputCharSpy_OutputChar('A');
    UnityOutputCharSpy_OutputChar('B');

    const char* buf = UnityOutputCharSpy_Get();

    // Expect the first two elements to be the written characters
    expect_char_eq(test_name, buf, 0, 'A');
    expect_char_eq(test_name, buf, 1, 'B');

    UnityOutputCharSpy_Destroy();
}

// Test 2: Does not write to buffer when buffer is full (count >= size-1)
// - Purpose: cover false branch of inner condition (count < size-1) and ensure no more writes occur
static void test_Does_Not_Write_When_Full() {
    const char* test_name = "Test_Does_Not_Write_When_Full";
    UnityOutputCharSpy_Create(3);      // size = 3, so (size-1) = 2
    UnityOutputCharSpy_Enable(1);      // Enable spying to buffer path

    // Fill buffer to size-1 (indexes 0 and 1 will be written)
    UnityOutputCharSpy_OutputChar('X');
    UnityOutputCharSpy_OutputChar('Y');

    // Attempt to write beyond the allowed limit
    UnityOutputCharSpy_OutputChar('Z');

    const char* buf = UnityOutputCharSpy_Get();

    // Validate the first two characters are written
    expect_char_eq(test_name, buf, 0, 'X');
    expect_char_eq(test_name, buf, 1, 'Y');
    // The third position should not contain 'Z' since it should not have been written
    if (buf[2] == 'Z') {
        report_failure(test_name, "buffer[2] should not contain 'Z' after full condition");
    }

    UnityOutputCharSpy_Destroy();
}

// Test 3: When spy is disabled, buffer should not be modified by OutputChar
// - Purpose: cover outer else branch (spy_enable == 0)
static void test_Disabled_Does_Not_Modify_Buffer() {
    const char* test_name = "Test_Disabled_Does_Not_Modify_Buffer";
    UnityOutputCharSpy_Create(4);      // size = 4
    UnityOutputCharSpy_Enable(1);      // Start with enabled

    // Write a character to populate the buffer
    UnityOutputCharSpy_OutputChar('A');

    // Disable spying; subsequent writes should not modify buffer
    UnityOutputCharSpy_Enable(0);
    UnityOutputCharSpy_OutputChar('B');

    const char* buf = UnityOutputCharSpy_Get();

    // The first character should remain 'A'
    expect_char_eq(test_name, buf, 0, 'A');
    // The second position should not be overwritten by 'B'
    if (buf[1] == 'B') {
        report_failure(test_name, "buffer[1] was unexpectedly written while spying was disabled");
    }

    UnityOutputCharSpy_Destroy();
}

// Test 4: Verify behavior near the boundary when enabling true path and buffer becomes full
// - Purpose: additional coverage of the boundary condition with size=2
static void test_Enable_Branch_BoundaryBehavior() {
    const char* test_name = "Test_Enable_Branch_BoundaryBehavior";
    UnityOutputCharSpy_Create(2);      // size = 2, (size-1) = 1
    UnityOutputCharSpy_Enable(1);      // Enable

    UnityOutputCharSpy_OutputChar('C'); // Should be written to buffer[0]
    const char* buf = UnityOutputCharSpy_Get();
    expect_char_eq(test_name, buf, 0, 'C');

    // Next write should not be appended since count == (size-1)
    UnityOutputCharSpy_OutputChar('D');
    buf = UnityOutputCharSpy_Get();

    // If still not appended, buffer[1] should not be 'D'
    if (buf[1] == 'D') {
        report_failure(test_name, "buffer[1] unexpectedly contains 'D' when full");
    }

    UnityOutputCharSpy_Destroy();
}

int main() {
    // Run all tests in sequence
    test_Appends_When_Enabled_And_Not_Full();
    test_Does_Not_Write_When_Full();
    test_Disabled_Does_Not_Modify_Buffer();
    test_Enable_Branch_BoundaryBehavior();

    if (g_failures) {
        fprintf(stderr, "Unit tests finished with %d failure(s).\n", g_failures);
        return 1;
    } else {
        printf("All unit tests passed successfully.\n");
        return 0;
    }
}
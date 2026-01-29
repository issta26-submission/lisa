// Test suite for focal method: string_clear(string_t *s)
// Focus: verify string_clear sets first character to zero and resets length to zero.

// Note: We attempt to compile this as a single translation unit by including the focal code (enough.c).
// To avoid multiple mains, we rename the focal file's main to main_included via macro tricks.
// We also define 'local' to nothing so that the included C code can be parsed by the C++ compiler.
// This harness uses a lightweight, non-terminating assertion approach to maximize coverage.

#include <string.h>
#include <enough.c>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>


// Treat 'local' as empty to align with the provided code when included.
// This helps the included C code compile cleanly under C++11.
#define local

// Rename the focal file's main to avoid duplicate main in the final binary.
// After inclusion, we will provide our own main() to run tests.
#define main main_included

// Include the focal class dependencies and the focal method.
// The macro trick ensures we don't conflict with our test's main().

#undef main
#undef local

// Lightweight test harness (non-terminating assertions) to maximize execution.
// We avoid GTest; use simple counters and console output.

static int test_failures = 0;

// Simple assertion macro that does not terminate the test on failure.
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        ++test_failures; \
        std::cerr << "TEST FAIL: " << (msg) << std::endl; \
    } \
} while(0)

// Test 1: Basic behavior - string_clear should set s.str[0] to 0 and s.len to 0
void test_string_clear_basic() {
    // Acquire the string_t type from the included focal code.
    string_t s;

    // Initialize with non-zero first character and a non-zero length.
    s.str[0] = 'X';
    s.len = 123; // arbitrary non-zero length

    // Call the focal function under test.
    string_clear(&s);

    // Validate post-conditions without terminating on failure.
    TEST_ASSERT(s.str[0] == 0, "string_clear should set s.str[0] to 0");
    TEST_ASSERT(s.len == 0, "string_clear should set s.len to 0");
}

// Test 2: Edge case - when string is already empty, ensure it remains consistent after clear.
void test_string_clear_when_already_empty() {
    string_t s;

    // Initialize with empty string.
    s.str[0] = 0;
    s.len = 0;

    string_clear(&s);

    TEST_ASSERT(s.str[0] == 0, "string_clear should keep s.str[0] == 0 when already empty");
    TEST_ASSERT(s.len == 0, "string_clear should keep s.len == 0 when already empty");
}

// Test 3: Large content scenario - ensure operation does not crash with unusual values.
// We only verify that the first character is reset and length is zero to confirm behavior.
void test_string_clear_large_content() {
    string_t s;

    // Simulate a large string scenario.
    s.str[0] = 'L';
    s.str[1] = 'A';
    s.str[2] = 'R';
    s.str[3] = 'G';
    s.len = 1000;

    string_clear(&s);

    TEST_ASSERT(s.str[0] == 0, "string_clear should reset first character even after large content");
    TEST_ASSERT(s.len == 0, "string_clear should reset length to 0 regardless of prior value");
}

// Run all tests and report summary.
int main(int argc, char **argv) {
    (void)argc; (void)argv; // unused

    // Execute tests. We deliberately do not exit on first failure to maximize coverage.
    test_string_clear_basic();
    test_string_clear_when_already_empty();
    test_string_clear_large_content();

    if(test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "Total TEST FAILURES: " << test_failures << std::endl;
        return 1;
    }
}

// End of test suite. Comments:
// - The test suite assumes that the included enough.c provides a type named string_t
//   with at least members: char str[] (or char str[...]) and size_t len.
// - We use direct access s.str[0] and s.len to exercise the focal method's effects.
// - The macro renaming (main -> main_included) allows a clean test main alongside the focal code's main.
// - All tests are designed to be executable under C++11 without GTest or other test frameworks.
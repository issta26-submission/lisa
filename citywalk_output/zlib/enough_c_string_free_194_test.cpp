// Note: This test suite is crafted to exercise the focal method string_free
// as defined in the provided focal code. The test harness is written in
// C++11 (no GTest) and uses only the standard library. It relies on the
// assumption that the macro 'local' expands to an appropriate storage
// duration (e.g., static) in the included focal code. To allow the focal
// function to be tested within a single translation unit, we prepend a
// macro to map 'local' to 'static' and include the focal implementation.
// The tests focus on the observable effects of string_free: freeing the
// internal buffer and resetting the state fields.

#include <cstring>
#include <string.h>
#include <enough.c>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


// Public structure used by the focal method.
// We mirror the layout used in the focal code to ensure compatibility.
typedef struct string_t {
    char *str;
    size_t size;
    size_t len;
} string_t;

// Map the local qualifier in the focal code to static for testability.
// This assumes that the focal file uses: local void ..., etc.
#define local static

// Include the focal implementation in the test translation unit.
// The focal method: local void string_free(string_t *s) { free(s->str); s->str = NULL; s->size = 0; s->len = 0; }

// Simple non-terminating assertion helper.
// It prints a message but does not abort the test, allowing multiple tests to run.
static int g_failed = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "ASSERT FAILED: " << (msg) << "\n"; \
    } else { \
        std::cout << "ASSERT PASSED: " << (msg) << "\n"; \
    } \
} while(0)


// Test 1: Normal usage - string is allocated and populated.
// Expectations:
// - After string_free(&s), s.str == NULL
// - s.size == 0 and s.len == 0
static void test_string_free_normal_case() {
    // Arrange
    string_t s;
    s.str = (char*)std::malloc(6); // allocate space for "hello" + null terminator
    if (s.str == nullptr) {
        std::cerr << "Memory allocation failed in test_string_free_normal_case\n";
        g_failed++;
        return;
    }
    std::memcpy(s.str, "hello", 6); // copy including terminator
    s.size = 6;
    s.len = 5;

    // Act
    string_free(&s);

    // Assert
    ASSERT_TRUE(s.str == nullptr, "After string_free, s.str should be NULL");
    ASSERT_TRUE(s.size == 0, "After string_free, s.size should be 0");
    ASSERT_TRUE(s.len == 0, "After string_free, s.len should be 0");
}

// Test 2: NULL string pointer - function should handle NULL gracefully.
// Expectations:
// - No crash
// - s.str remains NULL; s.size and s.len remain 0
static void test_string_free_null_pointer() {
    // Arrange
    string_t s;
    s.str = nullptr;
    s.size = 0;
    s.len = 0;

    // Act
    string_free(&s);

    // Assert
    ASSERT_TRUE(s.str == nullptr, "After string_free on NULL, s.str should remain NULL");
    ASSERT_TRUE(s.size == 0, "After string_free on NULL, s.size should remain 0");
    ASSERT_TRUE(s.len == 0, "After string_free on NULL, s.len should remain 0");
}

// Test 3: Double-free scenario - ensure no crash on consecutive frees.
// Expectations:
// - First call frees and resets fields; second call should be safe and idempotent
static void test_string_free_double_free() {
    // Arrange
    string_t s;
    s.str = (char*)std::malloc(4);
    if (s.str == nullptr) {
        std::cerr << "Memory allocation failed in test_string_free_double_free\n";
        ++g_failed;
        return;
    }
    std::memset(s.str, 0, 4);
    s.size = 4;
    s.len = 1;

    // Act & Assert: First call
    string_free(&s);
    ASSERT_TRUE(s.str == nullptr, "After first string_free, s.str should be NULL");

    // Act: Second call (should be safe)
    string_free(&s);
    // Assert: State should remain reset
    ASSERT_TRUE(s.str == nullptr, "After second string_free, s.str should remain NULL");
    ASSERT_TRUE(s.size == 0, "After second string_free, s.size should remain 0");
    ASSERT_TRUE(s.len == 0, "After second string_free, s.len should remain 0");
}

// Entry point to run all tests.
// Note: In a real test environment, you might want to integrate with a test
// runner. Here, we provide a simple main to execute all tests sequentially.
int main() {
    std::cout << "Starting test suite for focal method: string_free\n";

    test_string_free_normal_case();
    test_string_free_null_pointer();
    test_string_free_double_free();

    if (g_failed == 0) {
        std::cout << "All tests PASSED. string_free behaves as expected.\n";
        return 0;
    } else {
        std::cerr << g_failed << " test(s) FAILED.\n";
        return 1;
    }
}
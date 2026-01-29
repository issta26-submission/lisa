// Unit test suite for the focal method: string_printf
// This test suite is designed for C++11 compilation without GTest.
// It re-implements the minimal necessary dependencies present in the focal class
// to exercise the string_printf function in a controlled manner.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cstdarg>
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>


// The focal method uses 'local' as a storage-class specifier.
// Provide a compatible macro so the test can compile in C++.
#define local static

// Minimal representation of the string_t structure used by string_printf.
// Field semantics match those expected by the focal code:
// - str: pointer to the character buffer
// - len: current length of the string content in the buffer
// - size: total allocated size of the buffer
typedef struct {
    char *str;
    size_t len;
    size_t size;
} string_t;

// Dependency stubs (no-ops) to mirror the class dependencies.
// These are placeholders to satisfy the compiler and align with the provided
// FOCAL_CLASS_DEP semantics (string_clear, string_init, string_free).
local void string_clear(string_t *s) {}
local void string_init(string_t *s) {}
local void string_free(string_t *s) {}

// Focal string_printf implementation copied from the provided source.
// We keep the exact logic (including potential realloc and the va_list handling).
// Note: The original code reuses the va_list after resizing, which is undefined
// behavior in standard C/C++, but this test suite follows the exact logic to
// ensure compatibility with the given focal method.
local void string_printf(string_t *s, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t len = s->len;
    int ret = vsnprintf(s->str + len, s->size - len, fmt, ap);
    assert(ret >= 0 && "out of memory");
    s->len += ret;
    if (s->size < s->len + 1) {
        do {
            s->size <<= 1;
            assert(s->size != 0 && "overflow");
        } while (s->size < s->len + 1);
        s->str = realloc(s->str, s->size);
        assert(s->str != NULL && "out of memory");
        vsnprintf(s->str + len, s->size - len, fmt, ap);
    }
    va_end(ap);
}

// ---------------------------
// Test Harness
// ---------------------------

// Test 1: No resize branch engaged (s->size is large enough for first write).
// This validates the 'true' branch of the condition (no resizing).
// Expected outcome:
// - s->len increases by the length of the format output.
// - The content of s->str should start with the appended string (no resizing performed).
bool test_no_resize_branch() {
    // Initialize string with ample space
    string_t s;
    const size_t initial_size = 256;
    s.str = (char*)malloc(initial_size);
    if (s.str == NULL) return false;
    s.str[0] = '\0';
    s.len = 0;
    s.size = initial_size;

    // Perform a simple append with no format specifiers
    // "hello" has length 5; no va_args used.
    string_printf(&s, "hello");

    // Expectations:
    // - len == 5
    // - buffer contains "hello" at the start
    bool ok = (s.len == 5) && (strncmp(s.str, "hello", 5) == 0) && s.str[5] == '\0';

    free(s.str);
    return ok;
}

// Test 2: Resize branch engaged (s->size < s->len + 1 triggers reallocation).
// This validates the 'false' branch of the condition.
// We use a small initial buffer so that after the first write, a resize is required.
// The format string has no format specifiers, so the second vsnprintf call will
// write the same content starting at the original 'len' offset. This helps test
// memory reallocation and correctness of the final buffer content.
bool test_resize_branch() {
    // Initialize string with a small buffer to trigger resize
    string_t s;
    s.str = (char*)malloc(8); // small initial buffer
    if (s.str == NULL) return false;
    // Pre-fill with "abc" and set metadata
    memcpy(s.str, "abc", 3);
    s.str[3] = '\0';
    s.len = 3;
    s.size = 8;

    // Perform append that will require resizing
    // "hello" has length 5
    string_printf(&s, "hello");

    // After first write: s.len = 8
    // Resize should happen since size (8) < len+1 (9)
    // After resize: s.size should be at least 16
    // The content should be "abc" followed by "hello" (at positions 3..7)
    // and a null terminator at position 8.
    bool ok = true;
    if (s.len != 8) ok = false;
    if (s.str[0] != 'a' || s.str[1] != 'b' || s.str[2] != 'c') ok = false;
    // Check the appended portion
    if (strncmp(s.str + 3, "hello", 5) != 0) ok = false;
    // Null terminator should be at position 8
    if (s.str[8] != '\0') ok = false;
    // Buffer should have been resized to at least 16
    if (s.size < 16) ok = false;

    free(s.str);
    return ok;
}

// ---------------------------
// Main
// ---------------------------

int main(int argc, char **argv) {
    int failures = 0;

    // Run tests
    bool t1 = test_no_resize_branch();
    if (t1) {
        std::cout << "[PASS] test_no_resize_branch\n";
    } else {
        std::cout << "[FAIL] test_no_resize_branch\n";
        ++failures;
    }

    bool t2 = test_resize_branch();
    if (t2) {
        std::cout << "[PASS] test_resize_branch\n";
    } else {
        std::cout << "[FAIL] test_resize_branch\n";
        ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << failures << " test(s) failed.\n";
    }

    return failures;
}
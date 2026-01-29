// Unit test suite for the focal method buffer_init in pngimage.c
// Target language: C++11 (no GTest). Uses a lightweight, non-terminating assertion approach.
// The tests are designed to be standalone and compilable alongside the C file containing buffer_init.

// Include standard headers for IO and null definitions
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstddef>


// Provide C declarations for the portion of the C API we are testing.
// We mirror the minimal structure layout that the focal function touches to enable testing.
// Note: We assume the real pngimage.c defines compatible types; this test uses a compatible
// subset sufficient for verifying buffer_init behavior.

extern "C" {
    // Forward-declare the structures with the field layout used by buffer_init.
    typedef struct buffer_node {
        struct buffer_node *next;
    } buffer_node;

    typedef struct buffer {
        buffer_node first;      // 'first' contains a 'next' pointer (as used by buffer_init)
        buffer_node *last;        // last pointer of the buffer
        buffer_node *current;     // current pointer of the buffer
    } buffer;

    // The focal function under test (assumed to be defined in pngimage.c with C linkage)
    void buffer_init(buffer *buffer);
}

// Lightweight namespace-scoped test harness
namespace unit_tests {
    // Global test failure counter for non-terminating tests
    static int g_failures = 0;

    // Simple assertion macro: records failure but does not abort the test execution.
    #define ASSERT(condition, message) do { \
        if(!(condition)) { \
            ++g_failures; \
            std::fprintf(stderr, "Assertion failed: %s (in %s:%d)\n", \
                         message, __FILE__, __LINE__); \
        } \
    } while(0)

    // Test 1: Ensure buffer_init resets buffer fields as specified
    // Precondition: buffer has non-null values in first.next, last and current
    // Action: call buffer_init
    // Expectation: buffer.first.next == NULL, buffer.last == NULL, buffer.current == NULL
    void test_buffer_init_resets_all_fields() {
        // Arrange
        buffer buf;
        buf.first.next = (buffer_node*)0x1001;  // non-null placeholder
        buf.last = (buffer_node*)0x2002;        // non-null placeholder
        buf.current = (buffer_node*)0x3003;     // non-null placeholder

        // Act
        buffer_init(&buf);

        // Assert
        ASSERT(buf.first.next == NULL,
               "buffer.first.next should be NULL after buffer_init");
        ASSERT(buf.last == NULL,
               "buffer.last should be NULL after buffer_init");
        ASSERT(buf.current == NULL,
               "buffer.current should be NULL after buffer_init");
    }

    // Test 2: Ensure buffer_init is safe when some fields start as NULL
    // This checks idempotency and that the function does not write to unintended fields.
    // Case A: All fields already NULL -> after init, remain NULL
    // Case B: Some fields non-NULL -> after init, they become NULL as specified
    void test_buffer_init_handles_various_initial_states() {
        // Case A: All fields NULL
        buffer buf_all_null;
        buf_all_null.first.next = NULL;
        buf_all_null.last = NULL;
        buf_all_null.current = NULL;

        buffer_init(&buf_all_null);

        ASSERT(buf_all_null.first.next == NULL,
               "buffer_init should keep buffer.first.next as NULL when it is already NULL");
        ASSERT(buf_all_null.last == NULL,
               "buffer_init should keep buffer.last as NULL when it is already NULL");
        ASSERT(buf_all_null.current == NULL,
               "buffer_init should keep buffer.current as NULL when it is already NULL");

        // Case B: Some fields non-NULL initially
        buffer buf_some_nonnull;
        buf_some_nonnull.first.next = (buffer_node*)0xDEADBEEF;
        buf_some_nonnull.last = (buffer_node*)0xFEEDBEEF;
        buf_some_nonnull.current = (buffer_node*)0xBAADF00D;

        buffer_init(&buf_some_nonnull);

        ASSERT(buf_some_nonnull.first.next == NULL,
               "buffer.first.next should be NULL after buffer_init (Case B)");
        ASSERT(buf_some_nonnull.last == NULL,
               "buffer.last should be NULL after buffer_init (Case B)");
        ASSERT(buf_some_nonnull.current == NULL,
               "buffer.current should be NULL after buffer_init (Case B)");
    }

    // Run all tests
    void run_all() {
        test_buffer_init_resets_all_fields();
        test_buffer_init_handles_various_initial_states();
    }
} // namespace unit_tests

// Entry point for the test executable
int main(void) {
    // Run the test suite (under the specified namespace)
    unit_tests::run_all();

    // Report summary
    if (unit_tests::g_failures == 0) {
        std::fprintf(stderr, "pngimage_test: ALL tests PASSED.\n");
        return 0;
    } else {
        std::fprintf(stderr, "pngimage_test: %d test(s) FAILED.\n", unit_tests::g_failures);
        return 1;
    }
}
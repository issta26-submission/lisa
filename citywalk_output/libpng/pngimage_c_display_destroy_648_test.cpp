// File: test_display_destroy_with_png_write.cpp
// Purpose: Test the focal method display_destroy when PNG_WRITE_PNG_SUPPORTED is defined.
// - We override buffer_destroy with a mock to count invocations.
// - We verify that both dp->written_file and dp->original_file are destroyed.

#define PNG_WRITE_PNG_SUPPORTED 1
#define buffer_destroy(buffer) buffer_destroy_mock(buffer)

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <pngimage.c>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstddef>


// Forward declare types used by the production code (from pngimage.c)
struct buffer;
struct display;

// Provide a prototype for the production function we are testing (C linkage)
extern "C" void display_destroy(struct display *dp);

// The mock that will replace buffer_destroy in the production code.
// We declare it here so the preprocessor macro can map to it.
extern "C" void buffer_destroy_mock(struct buffer *buffer);

// Global counters to verify correct destruction behavior
static int destroyed_written = 0;
static int destroyed_original = 0;

// Include the production code. The macro above will redirect
// calls to buffer_destroy(...) to buffer_destroy_mock(...).

// Mock implementation: determine which buffer was destroyed by inspecting
// the containing struct display and which member was passed.
extern "C" void buffer_destroy_mock(struct buffer *buffer)
{
    if (buffer == nullptr) return;

    // Determine if this buffer belongs to a 'written_file' inside a display
    size_t off_written = offsetof(struct display, written_file);
    struct display *dp_written = (struct display *)((char*)buffer - off_written);
    if (buffer == &dp_written->written_file) {
        ++destroyed_written;
        return;
    }

    // Determine if this buffer belongs to an 'original_file' inside a display
    size_t off_original = offsetof(struct display, original_file);
    struct display *dp_original = (struct display *)((char*)buffer - off_original);
    if (buffer == &dp_original->original_file) {
        ++destroyed_original;
        return;
    }

    // If it doesn't belong to either, do nothing.
}

// RESET helpers for test clarity
static void reset_counters() {
    destroyed_written = 0;
    destroyed_original = 0;
}

// Test: display_destroy should destroy both buffers when PNG_WRITE_PNG_SUPPORTED is defined
static int test_display_destroy_with_written_and_original()
{
    reset_counters();

    struct display dp = {}; // zero-initialized; buffers are members of the struct

    display_destroy(&dp);

    // Expect both buffers to have been "destroyed" exactly once
    if (destroyed_written == 1 && destroyed_original == 1) {
        return 1; // pass
    } else {
        fprintf(stderr, "Test failed: expected both buffers destroyed. wrote=%d orig=%d\n",
                destroyed_written, destroyed_original);
        return 0; // fail
    }
}

int main()
{
    // Run the test
    int ok = test_display_destroy_with_written_and_original();
    if (ok) {
        printf("PASS: display_destroy destroyed both buffers when PNG_WRITE_PNG_SUPPORTED is defined.\n");
        return 0;
    } else {
        printf("FAIL: display_destroy did not destroy buffers as expected when PNG_WRITE_PNG_SUPPORTED is defined.\n");
        return 1;
    }
}
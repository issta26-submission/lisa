// This test suite targets the buffer_write function in pngimage.c.
// It uses the project’s existing C API (buffer_init, buffer_extend, buffer_write, etc.)
// to validate the behavior of writing data into a multi-node buffer.
// The tests are written in C++11 (no GTest) and call into the C code via headers.
// Step 1 (Understanding) contributed the Candidate Keywords and dependencies used below.
// Step 2/3 (Test generation and refinement) implemented through focused test cases.
// The tests aim to cover true branches of the write logic by manipulating the buffer state.

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <pngimage.h>


// Candidate Keywords derived from the focal method and dependencies:
// dp (display), buffer, buffer_list, last, end_count, sizeof last->buffer, buffer_extend,
// memcpy, data, size, last->next, display_log, APP_ERROR, write callback, png_bytep,
// buffer_init, buffer_destroy, display_init, etc.

// Include the project's C header to obtain proper type definitions and function declarations.
// The header should declare: struct display, struct buffer, struct buffer_list, png_bytep, and
// functions like display_init, buffer_init, buffer_extend, buffer_write, etc.
extern "C" {
}

// Simple non-terminating test assertions to maximize code coverage execution.
static int g_failures = 0;
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        fprintf(stderr, "EXPECT_TRUE failed: %s\n", msg);
        ++g_failures;
    }
}

// Test 1: Small write that fits within the initial buffer node.
// Verifies that end_count increases correctly and the data is stored in the first node.
void test_buffer_write_small_data() {
    fprintf(stderr, "Test 1: Small write within initial buffer node\n");

    struct display dp;
    display_init(&dp);

    struct buffer buf;
    buffer_init(&buf);

    // Capacity of a single buffer node
    size_t cap = sizeof buf.last->buffer;
    if (cap == 0) {
        fprintf(stderr, "Buffer capacity reported as 0; cannot run Test 1 reliably.\n");
        ++g_failures;
        return;
    }

    // Prepare input that fits into the first node (cap - 1 to ensure non-full write)
    size_t write_size = cap - 1;
    unsigned char* input = new unsigned char[write_size];
    memset(input, 'A', write_size);

    // Execute the operation
    buffer_write(&dp, &buf, input, write_size);

    // Validate end_count and data integrity in the first node
    expect_true(buf.end_count == write_size, "end_count should equal written size after small write");
    expect_true(memcmp(buf.last->buffer, input, write_size) == 0,
                "Buffer contents should match input after small write");

    delete[] input;
    fprintf(stderr, "Test 1 completed.\n");
}

// Test 2: Force a transition to a new buffer node (extend) when the current node is full.
// Validates that buffer_extend is invoked and the data is written to the new node.
void test_buffer_write_trigger_extend_new_node() {
    fprintf(stderr, "Test 2: Trigger extend to a new node and write there\n");

    struct display dp;
    display_init(&dp);

    struct buffer buf;
    buffer_init(&buf);

    // Force the condition to create a new node on write
    size_t cap = sizeof buf.last->buffer;
    buf.end_count = cap; // current node is full

    unsigned char val = 'Z';
    buffer_write(&dp, &buf, &val, 1);

    // After extend, end_count should reflect the single byte written in the new node
    expect_true(buf.end_count == 1, "After extend, end_count should be 1 in the new node");
    expect_true(buf.last != NULL, "There should be a non-NULL last pointer after extend");
    expect_true(buf.last->buffer[0] == val,
                "New node's first byte should contain the written value");

    fprintf(stderr, "Test 2 completed.\n");
}

// Test 3: Existing next node path (else-branch) where last->next is already non-NULL.
// Verifies that the code navigates to the next node and writes there.
void test_buffer_write_else_branch_existing_next() {
    fprintf(stderr, "Test 3: Else-branch when next node already exists\n");

    struct display dp;
    display_init(&dp);

    struct buffer buf;
    buffer_init(&buf);

    // Create and link a second node manually to simulate existing chain
    struct buffer_list* first = buf.last;
    struct buffer_list* second = buffer_extend(first);
    if (second == NULL) {
        fprintf(stderr, "buffer_extend returned NULL; cannot run Test 3 reliably.\n");
        ++g_failures;
        return;
    }
    first->next = second; // link explicitly to simulate pre-existing chain

    // Force the code path to move from first to second by setting end_count to capacity
    size_t cap = sizeof buf.last->buffer;
    buf.end_count = cap;

    unsigned char val = 'X';
    buffer_write(&dp, &buf, &val, 1);

    // After write, the last should be advanced to 'second' and contain the byte
    expect_true(buf.last == second, "Last pointer should advance to existing next node");
    expect_true(buf.end_count == 1, "End count in second node should be updated to 1");
    expect_true(second->buffer[0] == val, "Second node's first byte should hold the written value");

    fprintf(stderr, "Test 3 completed.\n");
}

// Entry point to run tests from main (as per domain guidance for non-GTest environments)
int main(void) {
    fprintf(stderr, "Starting tests for buffer_write in pngimage.c (C++11 test harness)\n");

    test_buffer_write_small_data();
    test_buffer_write_trigger_extend_new_node();
    test_buffer_write_else_branch_existing_next();

    if (g_failures == 0) {
        fprintf(stderr, "All tests passed.\n");
        return 0;
    } else {
        fprintf(stderr, "Tests completed with %d failure(s).\n", g_failures);
        return 1;
    }
}
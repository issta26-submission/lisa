/*
Unit test suite for the focal method: buffer_read in pngimage.c
- Target: buffer_read(struct display *dp, struct buffer *bp, png_bytep data, size_t size)
- Environment: C++11, no GoogleTest. A lightweight, non-terminating assertion framework is implemented.
- Strategy:
  1) Use the project's header to obtain real struct definitions (struct display, struct buffer, struct buffer_list) and the function prototype for buffer_read.
  2) Create test scenarios that exercise:
    - Branch where current is NULL (file truncated).
    - Branch where current == bp->last and read_count >= end_count (file truncated).
    - Branch that moves to a next buffer (read_count >= sizeof last->buffer) and then copies from the next buffer.
    - Normal copy within a single buffer (no boundary crossing).
  3) Validate behavior by inspecting bp state and the data copied to the output buffer. Logs emitted by display_log are not strictly asserted (capturing logs would require a more invasive hook; instead, we rely on state transitions and data content).
- Notes:
  - This test suite relies on the real type definitions from the project (struct display, struct buffer, struct buffer_list, etc.). It uses those types directly from the project's headers.
  - The tests are designed to be executable from main() without terminating the process on assertion failures.
  - All tests are implemented in one file (tests.cpp) for simplicity and portability.

Compile instructions (conceptual):
  - Compile along with pngimage.c and the headers that define the types and buffer_read.
  - Example (conceptual, adjust to your build system):
      g++ -std=c++11 -I<path_to_headers> tests.cpp pngimage.c -o tests
  - Run: ./tests

This file contains four unit tests with explanatory comments above each test.
*/

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


// Include the project's header to obtain the real types and prototypes.
// The header path may vary in your project setup. Adjust accordingly.
extern "C" {
}

// Lightweight non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper macro: non-terminating assertion
#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILURE: %s (line %d): %s\n", __FILE__, __LINE__, (msg)); \
        ++g_tests_failed; \
    } \
} while (0)


// Helper: compare two memory regions
static bool memeq(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}


// Test 1: current == NULL => "file truncated" path is taken.
// Expectation: No data is copied, bp->read_count remains as initialized.
bool test_buffer_read_truncated_when_current_null() {
    // Prepare dp (display) with default state
    struct display dp;
    std::memset(&dp, 0, sizeof(dp));  // Initialize to zero; specific fields not relied upon for this test.

    // Prepare bp (buffer) with current == NULL
    struct buffer bp;
    std::memset(&bp, 0, sizeof(bp));
    bp.current = NULL;       // triggers the "file truncated" branch
    bp.last = NULL;
    bp.read_count = 0;
    bp.end_count = 0;          // not used due to current == NULL

    // Prepare output buffer
    unsigned char out[16];
    std::memset(out, 0xAA, sizeof(out)); // pre-fill to detect changes

    // Call focal method
    buffer_read(&dp, &bp, out, sizeof(out));

    // Validate: read_count should remain 0, and out should remain unchanged (all 0xAA)
    bool data_unchanged = memeq(out, (unsigned char[16]){0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA}, 16);
    ASSERT_TRUE(bp.read_count == 0, "read_count should remain 0 when current is NULL");
    ASSERT_TRUE(data_unchanged, "output buffer should remain unchanged when current is NULL");

    return g_tests_failed == 0;
}


// Test 2: current == bp->last and read_count >= end_count => "file truncated" path (true branch).
// Expectation: No data is copied, bp->read_count remains as initialized.
bool test_buffer_read_truncated_at_eof_condition() {
    // Prepare two buffer_list nodes (A -> B)
    struct buffer_list nodeA;
    struct buffer_list nodeB;

    // Initialize nodeA
    std::memset(&nodeA, 0, sizeof(nodeA));
    // Fill nodeA.buffer with some data (not strictly required for this test)
    const unsigned char a_bytes[] = { 'A', 'B', 'C', 'D' };
    size_t a_len = sizeof(a_bytes);
    // We can't rely on exact array length; copy into buffer if it exists
    if (sizeof nodeA.buffer >= a_len) {
        std::memcpy(nodeA.buffer, a_bytes, a_len);
    }

    // Setup nodeA -> nodeB
    nodeA.next = &nodeB;

    // Initialize nodeB
    std::memset(&nodeB, 0, sizeof(nodeB));
    const unsigned char b_bytes[] = { 'X', 'Y', 'Z' };
    size_t b_len = sizeof(b_bytes);
    if (sizeof nodeB.buffer >= b_len) {
        std::memcpy(nodeB.buffer, b_bytes, b_len);
    }

    // BP setup: current == last == nodeA; read_count >= end_count to trigger first-if
    struct display dp;
    std::memset(&dp, 0, sizeof(dp));

    struct buffer bp;
    std::memset(&bp, 0, sizeof(bp));
    bp.current = &nodeA;
    bp.last = &nodeA;
    bp.read_count = 2;      // >= end_count will be true if end_count <= 2
    bp.end_count = 2;       // equal to read_count -> triggers the USER_ERROR branch

    unsigned char out[16];
    std::memset(out, 0x55, sizeof(out));

    buffer_read(&dp, &bp, out, sizeof(out));

    // Validate: no data copied
    bool data_unchanged = memeq(out, (unsigned char[16]){0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55}, 16);
    ASSERT_TRUE(bp.read_count == 2, "read_count should remain unchanged when truncation condition is met");
    ASSERT_TRUE(data_unchanged, "output buffer should remain unchanged when truncation occurs at EOF condition");

    return g_tests_failed == 0;
}


// Test 3: Move to the next buffer (read_count >= sizeof last->buffer) and copy from next buffer.
// Expectation: bp.current moves to nodeB, copies up to size from nodeB.buffer, and bp.read_count reflects copied bytes.
bool test_buffer_read_move_to_next_and_copy_from_next() {
    // Build two buffers: nodeA (current/last) and nodeB (next)
    struct buffer_list nodeA;
    struct buffer_list nodeB;

    std::memset(&nodeA, 0, sizeof(nodeA));
    std::memset(&nodeB, 0, sizeof(nodeB));

    // Fill nodeA with some data (for completeness)
    const unsigned char a_b[] = { 'H', 'e', 'l', 'l', 'o' };
    size_t a_len = sizeof(a_b);
    if (sizeof nodeA.buffer >= a_len) {
        std::memcpy(nodeA.buffer, a_b, a_len);
    }

    // Link A -> B
    nodeA.next = &nodeB;

    // Fill nodeB with test data
    const unsigned char b_b[] = { 'W', 'o', 'r', 'l', 'd' };
    size_t b_len = sizeof(b_b);
    if (sizeof nodeB.buffer >= b_len) {
        std::memcpy(nodeB.buffer, b_b, b_len);
    }

    // BP setup
    struct display dp;
    std::memset(&dp, 0, sizeof(dp));

    struct buffer bp;
    std::memset(&bp, 0, sizeof(bp));
    bp.current = &nodeA;
    bp.last = &nodeA;
    // Determine actual buffer size for test -> use sizeof(last->buffer)
    size_t bufsize = sizeof(nodeA.buffer);
    bp.read_count = bufsize;      // triggers move to next buffer
    bp.end_count = 1000;            // not reached in this test

    unsigned char out[16];
    std::memset(out, 0, sizeof(out));

    // We'll request 3 bytes to copy from nodeB
    buffer_read(&dp, &bp, out, 3);

    // Validate: current should now be nodeB, read_count should be 3, and first 3 bytes from nodeB.buffer should be copied
    bool moved_to_next = (bp.current == &nodeB);
    bool copied_exact = memeq(out, (unsigned char[]){ 'W', 'o', 'r' }, 3);
    bool read_count_correct = (bp.read_count == 3);

    ASSERT_TRUE(moved_to_next, "buffer_read should move to the next buffer when read_count >= sizeof buffer");
    ASSERT_TRUE(copied_exact, "three bytes copied should match the beginning of nodeB.buffer");
    ASSERT_TRUE(read_count_correct, "read_count should reflect the number of bytes copied from next buffer");

    return g_tests_failed == 0;
}


// Test 4: Copy within a single buffer without switching buffers.
// Expectation: bp.current remains the same, bp.read_count increases by size, data matches the first bytes.
bool test_buffer_read_copy_within_same_buffer() {
    // Setup single buffer nodeA
    struct buffer_list nodeA;
    std::memset(&nodeA, 0, sizeof(nodeA));

    // Fill nodeA with known data
    const unsigned char payload[] = { 'A', 'B', 'C', 'D', 'E' };
    size_t plen = sizeof(payload);
    if (sizeof nodeA.buffer >= plen) {
        std::memcpy(nodeA.buffer, payload, plen);
    }

    // BP setup
    struct display dp;
    std::memset(&dp, 0, sizeof(dp));

    struct buffer bp;
    std::memset(&bp, 0, sizeof(bp));
    bp.current = &nodeA;
    bp.last = &nodeA;
    bp.read_count = 0;
    bp.end_count = 1000;

    unsigned char out[4];
    std::memset(out, 0, sizeof(out));

    // Copy first two bytes
    buffer_read(&dp, &bp, out, 2);

    bool copied = memeq(out, (unsigned char[]){ 'A', 'B' }, 2);
    bool readcount_ok = (bp.read_count == 2);
    bool current_unchanged = (bp.current == &nodeA);

    ASSERT_TRUE(copied, "two bytes should be copied from the first buffer");
    ASSERT_TRUE(readcount_ok, "read_count should reflect the number of bytes copied");
    ASSERT_TRUE(current_unchanged, "current should remain pointing to the same buffer after partial read");

    return g_tests_failed == 0;
}


int main(void) {
    printf("Running buffer_read unit tests...\n");

    bool all_passed =
        test_buffer_read_truncated_when_current_null() &&
        test_buffer_read_truncated_at_eof_condition() &&
        test_buffer_read_move_to_next_and_copy_from_next() &&
        test_buffer_read_copy_within_same_buffer();

    if (g_tests_failed == 0) {
        printf("ALL TESTS PASSED (%d tests).\n", g_tests_run);
        return 0;
    } else {
        fprintf(stderr, "SOME TESTS FAILED: %d run, %d failed.\n", g_tests_run, g_tests_failed);
        return 1;
    }
}
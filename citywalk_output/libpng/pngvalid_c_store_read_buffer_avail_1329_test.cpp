#include <cstddef>
#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// This test suite is a self-contained, minimal C++11 harness
// that re-implements the focal method's relevant data structures
// and logic in order to exercise store_read_buffer_avail without
// requiring the full external project. The goal is to validate
// the control flow and key branch behavior (true/false branches),
// while keeping the test self-contained and portable.

// Domain knowledge notes included as comments near test cases.

// ------------------------------
// Mocked definitions (minimal)
// ------------------------------

#define STORE_BUFFER_SIZE 1024

// Minimal representation of a store buffer node.
// The real project uses a more complex structure; we only
// need the 'prev' pointer to walk the chain and the implicit
// layout compatible with the test harness.
struct png_store_buffer {
    png_store_buffer *prev;
};

// Minimal representation of a store chunk containing one buffer
// and a count of bytes currently available in this chunk.
struct png_store_chunk {
    png_store_buffer data; // first buffer in this chunk
    size_t datacount;      // number of bytes currently available in this chunk
};

// Minimal representation of the png_store structure used by
// the focal method. It references a current chunk, a next buffer
// in the chain, a read position, and a pread pointer (for error handling).
struct png_store {
    png_store_chunk *current;
    png_store_buffer *next;
    size_t readpos;
    void *pread;
};

// Forward declaration of the focal method under test.
// In the real project, this is provided by pngvalid.c and related headers.
// Here we implement a faithful (self-contained) replica to enable testing.
size_t store_read_buffer_avail(png_store *ps);

// Local helper to mimic the libpng error handler in tests.
// The real function would longjmp or terminate; here we just record the call.
static bool png_error_called = false;
static void png_error(void* /*pread*/, const char* /*message*/) {
    // Mark that an error path was exercised (for observability in tests).
    png_error_called = true;
}

// Implementation of the focal method (self-contained version for testing).
size_t store_read_buffer_avail(png_store *ps)
{
    if (ps->current != NULL && ps->next != NULL)
    {
        png_store_buffer *next = &ps->current->data;
        size_t cbAvail = ps->current->datacount;
        while (next != ps->next && next != NULL)
        {
            next = next->prev;
            cbAvail += STORE_BUFFER_SIZE;
        }
        if (next != ps->next)
            png_error(ps->pread, "buffer read error");
        if (cbAvail > ps->readpos)
            return cbAvail - ps->readpos;
    }
    return 0;
}

// ------------------------------
// Lightweight test framework
// ------------------------------

static int test_count = 0;
static int fail_count = 0;

#define LOG(msg) do { std::cout << msg << std::endl; } while(0)

template <typename T, typename U>
static void expect_eq(const T& a, const U& b, const char* msg) {
    if (!(a == b)) {
        std::cout << "TEST FAIL: " << msg
                  << " | expected: " << b << ", actual: " << a << "\n";
        ++fail_count;
    } else {
        // Optional success trace (can be disabled for noise reduction)
        // std::cout << "TEST PASS: " << msg << "\n";
    }
}

template <typename T>
static void expect_true(const T& a, const char* msg) {
    expect_eq(a, true, msg);
}

static void run_all_tests();


// ------------------------------
// Unit Tests for store_read_buffer_avail
// Domain: true/false branches, boundary conditions
// Notes:
//  - We build a minimal in-memory chain of png_store_buffer elements.
//  - We exercise scenarios to cover both the loop accumulation
//    path and the simple one-buffer path.
//  - We also test cases where current or next is NULL to cover
//    the early-return branch.
//  - We avoid triggering the error path (png_error) to keep tests
//    deterministic in this harness; the error path would require
//    a more complex longjmp-like setup or a stubbed png_error
//    that simulates termination.
// ------------------------------

/*
Test 1: Multiple buffers in chain
- Setup:
  - current chunk datacount = 50
  - current.data.prev -> B (ps.next)
  - B.prev = NULL (end of chain)
  - ps.next points to B
  - readpos = 100
- Expected: cbAvail = 50 + STORE_BUFFER_SIZE (1024) = 1074
  -> avail = 1074 - 100 = 974
*/
static void test_store_read_buffer_avail_multiple_buffers() {
    // Reset error flag
    png_error_called = false;

    // Create chain: A (current) -> B (ps.next)
    png_store_buffer B;
    B.prev = NULL;

    png_store_chunk A;
    A.datacount = 50;
    A.data.prev = &B;

    png_store ps;
    ps.current = &A;
    ps.next = &B;
    ps.readpos = 100;
    ps.pread = nullptr;

    size_t avail = store_read_buffer_avail(&ps);
    expect_eq(avail, static_cast<size_t>(974), "Multiple buffers: available bytes should be 974");
    expect_true(png_error_called == false, "Multiple buffers: error path should not be triggered");
}

/*
Test 2: Multiple buffers but readpos larger than total available
- Setup identical to Test 1 but readpos = 1100
- Expected: cbAvail (1074) <= readpos => 0
*/
static void test_store_read_buffer_avail_multiple_buffers_insufficient() {
    png_error_called = false;

    png_store_buffer B;
    B.prev = NULL;

    png_store_chunk A;
    A.datacount = 50;
    A.data.prev = &B;

    png_store ps;
    ps.current = &A;
    ps.next = &B;
    ps.readpos = 1100;
    ps.pread = nullptr;

    size_t avail = store_read_buffer_avail(&ps);
    expect_eq(avail, static_cast<size_t>(0), "Multiple buffers: insufficient bytes should yield 0");
    expect_true(png_error_called == false, "Multiple buffers: error path should not be triggered");
}

/*
Test 3: Single buffer in chain (no accumulation)
- Setup:
  - Current datacount = 200
  - ps.next points to same data (single-buffer case)
  - readpos = 50
- Expected: avail = 200 - 50 = 150
*/
static void test_store_read_buffer_avail_single_buffer() {
    png_error_called = false;

    // Single buffer in the chain
    png_store_buffer Abuffer;
    Abuffer.prev = NULL;

    png_store_chunk A;
    A.data = Abuffer;
    A.datacount = 200;

    png_store ps;
    ps.current = &A;
    ps.next = &A.data; // next points to the same buffer
    ps.readpos = 50;
    ps.pread = nullptr;

    size_t avail = store_read_buffer_avail(&ps);
    expect_eq(avail, static_cast<size_t>(150), "Single buffer: available bytes should be 150");
    expect_true(png_error_called == false, "Single buffer: error path should not be triggered");
}

/*
Test 4: ps.current is NULL => immediate 0
- Setup: current = NULL, next non-NULL
- Expected: 0
*/
static void test_store_read_buffer_avail_current_null() {
    png_error_called = false;

    png_store ps;
    ps.current = nullptr;
    // next must be non-NULL to reach the check, but since current is NULL, function returns 0
    png_store_buffer dummy;
    dummy.prev = NULL;
    ps.next = &dummy;
    ps.readpos = 0;
    ps.pread = nullptr;

    size_t avail = store_read_buffer_avail(&ps);
    expect_eq(avail, static_cast<size_t>(0), "Current NULL: should return 0");
    expect_true(png_error_called == false, "Current NULL: error path should not be triggered");
}

/*
Test 5: ps.next is NULL => immediate 0
- Setup: current non-NULL, next NULL
- Expected: 0
*/
static void test_store_read_buffer_avail_next_null() {
    png_error_called = false;

    png_store_buffer Abuffer;
    Abuffer.prev = NULL;

    png_store_chunk A;
    A.data = Abuffer;
    A.datacount = 128;

    png_store ps;
    ps.current = &A;
    ps.next = nullptr; // next is NULL
    ps.readpos = 10;
    ps.pread = nullptr;

    size_t avail = store_read_buffer_avail(&ps);
    expect_eq(avail, static_cast<size_t>(0), "Next NULL: should return 0");
    expect_true(png_error_called == false, "Next NULL: error path should not be triggered");
}

// ---------------------------------
// Test harness entrypoint
// ---------------------------------

static void run_all_tests() {
    LOG("Starting store_read_buffer_avail unit tests (self-contained harness)...");
    test_count = 0;
    fail_count = 0;

    test_store_read_buffer_avail_multiple_buffers();
    test_store_read_buffer_avail_multiple_buffers_insufficient();
    test_store_read_buffer_avail_single_buffer();
    test_store_read_buffer_avail_current_null();
    test_store_read_buffer_avail_next_null();

   test_count += 5; // five tests defined above

    if (fail_count == 0) {
        LOG("All tests passed.");
    } else {
        std::cout << fail_count << " / " << test_count << " tests failed." << std::endl;
    }
}

// ------------------------------
// Main: run tests
// ------------------------------

int main() {
    run_all_tests();
    return (fail_count == 0) ? 0 : 1;
}
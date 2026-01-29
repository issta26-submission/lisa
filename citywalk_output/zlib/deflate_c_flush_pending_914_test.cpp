// Unit test harness for the focal method: flush_pending
// This test re-implements the minimal environment necessary to exercise
// the logic of flush_pending as described in the provided snippet.
// The real project may expose flush_pending via internal linkage; for the
// purposes of unit testing in this isolated environment, we reproduce the
// function logic here to verify its correctness across edge cases.
//
// The test suite does not rely on any external testing framework (GTest not used).
// It uses a lightweight assertion mechanism that records failures and reports
// them at the end of execution.
//
// Key dependencies (Candidate Keywords) reflected in tests:
// - state management: deflate_state, pending, pending_out, pending_buf
// - I/O state: z_streamp (next_out, avail_out, total_out)
// - helper hooks: _tr_flush_bits(s)
// - memory copy: zmemcpy
// - control flow branches: len == 0, len > avail_out, len <= avail_out, s->pending == 0
//
// Domain knowledge considerations:
// - Static/internal helpers are mocked locally (non-terminating tests allowed).
// - Test cases cover true/false branches and boundary conditions.
// - All data structures are lightweight and self-contained for C++11 compilation.

#include <iostream>
#include <deflate.h>
#include <cstring>


// Candidate Keywords (as comments above) are reflected below in the test harness.

// Minimal type definitions to mimic the structures used by flush_pending.
typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;

// Forward declaration to mimic the external deflate_state used by flush_pending.
struct deflate_state {
    unsigned pending;           // number of pending bytes to flush
    Bytef*   pending_out;         // current output pointer for pending data
    Bytef*   pending_buf;         // start of the pending buffer
};

// The stream outline that flush_pending consumes.
struct z_stream_s {
    Bytef* next_out;
    uInt   avail_out;
    uLong  total_out;
    deflate_state* state;
};

// Typedef to match z_streamp usage in the original code.
typedef z_stream_s* z_streamp;

// Helper function prototypes (mocked for test environment)
static void _tr_flush_bits(deflate_state* s) {
    // In the real library, this would flush internal bit buffer.
    // For testing, it's a no-op.
    (void)s;
}

// Mocked memory copy function used by the focal method.
static void zmemcpy(Bytef* dest, const Bytef* src, unsigned len) {
    std::memmove(dest, src, len);
}

// The focal method under test (reproduced here for unit testing).
// Note: In the real project, this would be a static function defined in deflate.c.
// Here we expose it to allow direct testing of its semantics.
void flush_pending(z_streamp strm) {
    unsigned len;
    deflate_state *s = strm->state;
    _tr_flush_bits(s);
    len = s->pending;
    if (len > strm->avail_out) len = strm->avail_out;
    if (len == 0) return;
    zmemcpy(strm->next_out, s->pending_out, len);
    strm->next_out  += len;
    s->pending_out  += len;
    strm->total_out += len;
    strm->avail_out -= len;
    s->pending      -= len;
    if (s->pending == 0) {
        s->pending_out = s->pending_buf;
    }
}

// Lightweight test framework (non-terminating assertions)
static int test_failures = 0;
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        ++test_failures; \
        std::cerr << "Test failure: " << msg << std::endl; \
    } \
} while(0)

// Helper to create a fresh buffer filled with a byte pattern for verification
static void fill_pattern(Bytef* buf, unsigned len, Bytef pattern) {
    for (unsigned i = 0; i < len; ++i) buf[i] = pattern;
}

// Test 1: Basic flush when pending <= avail_out
// Expect: all pending bytes are copied to output, pointers advanced,
// totals updated, and pending resets when zero.
static void test_flush_basic() {
    // Prepare source data to be flushed
    Bytef src[8] = { 1,2,3,4,5,6,7,8 };
    // Destination buffer
    Bytef dst[8];
    std::memset(dst, 0, sizeof(dst));

    // Set up stream and state
    z_streamp strm = new z_stream_s;
    strm->next_out = dst;
    strm->avail_out = 5;           // room for 5 bytes
    strm->total_out = 0;

    deflate_state* s = new deflate_state;
    s->pending = 5;                  // we have 5 bytes to flush
    s->pending_out = src;            // points to source data
    s->pending_buf = src;            // pending_buf points to same buffer for reset

    strm->state = s;

    // Call function under test
    flush_pending(strm);

    // Validate post-conditions
    // 1) The first 5 bytes [1,2,3,4,5] should be copied to dst
    Bytef expected[5] = {1,2,3,4,5};
    ASSERT(std::memcmp(dst, expected, 5) == 0, "Basic flush: copied bytes mismatch");

    // 2) next_out advanced by 5
    Bytef* expected_next_out = dst + 5;
    ASSERT(strm->next_out == expected_next_out, "Basic flush: next_out not advanced correctly");

    // 3) total_out increased by 5
    ASSERT(strm->total_out == 5, "Basic flush: total_out not updated");

    // 4) avail_out decreased by 5
    ASSERT(strm->avail_out == 0, "Basic flush: avail_out not decremented");

    // 5) pending decreased by 5 and is now zero
    ASSERT(s->pending == 0, "Basic flush: pending not decremented to zero");

    // 6) pending_out reset to pending_buf when pending == 0
    ASSERT(s->pending_out == s->pending_buf, "Basic flush: pending_out not reset after zero pending");

    // Cleanup
    delete s;
    delete strm;
}

// Test 2: Partial flush when pending > avail_out
// Expect: only avail_out bytes are flushed, pointers updated accordingly,
// and pending remains non-zero with pending_out advanced.
static void test_flush_partial() {
    Bytef src[16] = { 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };
    Bytef dst[8];
    std::memset(dst, 0, sizeof(dst));

    z_streamp strm = new z_stream_s;
    strm->next_out = dst;
    strm->avail_out = 4;          // only room for 4 bytes
    strm->total_out = 0;

    deflate_state* s = new deflate_state;
    s->pending = 10;                 // 10 bytes to flush
    s->pending_out = src;            // pending data located at src
    s->pending_buf = src;            // buffer start

    strm->state = s;

    flush_pending(strm);

    // Validate
    // 1) First 4 bytes should be copied
    Bytef expected[4] = {10,11,12,13};
    ASSERT(std::memcmp(dst, expected, 4) == 0, "Partial flush: copied bytes mismatch");

    // 2) next_out advanced by 4
    Bytef* expected_next_out = dst + 4;
    ASSERT(strm->next_out == expected_next_out, "Partial flush: next_out not advanced correctly");

    // 3) total_out increased by 4
    ASSERT(strm->total_out == 4, "Partial flush: total_out not updated");

    // 4) avail_out decreased to 0
    ASSERT(strm->avail_out == 0, "Partial flush: avail_out not decremented to zero");

    // 5) pending decreased by 4, non-zero (6 remaining)
    ASSERT(s->pending == 6, "Partial flush: pending should be reduced by flushed amount");

    // 6) pending_out advanced but not reset (since pending != 0)
    ASSERT(s->pending_out == src + 4, "Partial flush: pending_out should advance with partial copy");

    // Cleanup
    delete s;
    delete strm;
}

// Test 3: No-op when there is no pending data (len == 0)
static void test_flush_zero_pending() {
    Bytef dst[4] = {0,0,0,0};
    Bytef src[4] = {9,9,9,9};

    z_streamp strm = new z_stream_s;
    strm->next_out = dst;
    strm->avail_out = 4;
    strm->total_out = 0;

    deflate_state* s = new deflate_state;
    s->pending = 0;
    s->pending_out = src;
    s->pending_buf = src;
    strm->state = s;

    flush_pending(strm);

    // Expect no changes
    Bytef expected_dst[4] = {0,0,0,0};
    ASSERT(std::memcmp(dst, expected_dst, 4) == 0, "Zero-pending: destination should remain unchanged");

    ASSERT(strm->total_out == 0, "Zero-pending: total_out should remain zero");
    ASSERT(strm->avail_out == 4, "Zero-pending: avail_out should remain unchanged");
    ASSERT(s->pending == 0, "Zero-pending: pending should remain zero");
    ASSERT(s->pending_out == src, "Zero-pending: pending_out should remain unchanged");

    delete s;
    delete strm;
}

// Test 4: Exact fit when pending == avail_out
// Expect: flush all, same as Test 1, but checks branch where len == avail_out
static void test_flush_exact_fit() {
    Bytef src[3] = { 5,6,7 };
    Bytef dst[3];
    std::memset(dst, 0, sizeof(dst));

    z_streamp strm = new z_stream_s;
    strm->next_out = dst;
    strm->avail_out = 3;
    strm->total_out = 0;

    deflate_state* s = new deflate_state;
    s->pending = 3;
    s->pending_out = src;
    s->pending_buf = src;
    strm->state = s;

    flush_pending(strm);

    Bytef expected[3] = {5,6,7};
    ASSERT(std::memcmp(dst, expected, 3) == 0, "Exact-fit flush: copied bytes mismatch");

    ASSERT(strm->total_out == 3, "Exact-fit flush: total_out should be 3");
    ASSERT(strm->avail_out == 0, "Exact-fit flush: avail_out should be 0");
    ASSERT(s->pending == 0, "Exact-fit flush: pending should be 0");
    ASSERT(s->pending_out == s->pending_buf, "Exact-fit flush: pending_out should reset to pending_buf");

    delete s;
    delete strm;
}

// Entry point for running all tests
int main() {
    // Delegate to individual tests
    test_flush_basic();
    test_flush_partial();
    test_flush_zero_pending();
    test_flush_exact_fit();

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
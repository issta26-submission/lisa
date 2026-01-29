/*
Step 1: Program Understanding and Candidate Keywords
- Core dependencies used by gzfwrite:
  - gz_statep / gzFile (state object representing a gz stream)
  - state->mode (needs to be GZ_WRITE to proceed)
  - state->err (must be Z_OK to proceed)
  - Z_OK, GZ_WRITE, Z_STREAM_ERROR (error signaling)
  - size, nitems, len calculation and overflow check: len = nitems * size
  - gz_error to report overflow with Z_STREAM_ERROR
  - gz_write to perform the actual write when valid
  - Behavior when file is NULL (returns 0)
  - Returns number of full items written: (gz_write(...) / size)
  - Overflow detection path triggers gz_error and returns 0

- Candidate Keywords to drive test design:
  - file == NULL
  - state->mode == GZ_WRITE
  - state->err == Z_OK
  - size == 0
  - overflow detection: len / size != nitems
  - gz_error with Z_STREAM_ERROR
  - gz_write invocation and its return value
  - return value equals written_items = gz_write(state, buf, len) / size

- This test harness implements a minimal, self-contained environment to exercise gzfwrite without external dependencies.
*/

#include <cstring>
#include <stdarg.h>
#include <limits>
#include <cstdint>
#include <iostream>
#include <gzguts.h>


// Typedefs and constants to mirror the simplified zlib-like wiring in the focal code.
typedef const void* voidpc;
typedef uint64_t z_size_t;  // domain uses unsigned size types

// Minimal gz state representation (as used by gzfwrite)
struct gz_state_s {
    int mode;
    int err;
    // Other members from the real library are not required for gzfwrite tests.
};
typedef struct gz_state_s* gz_statep;
typedef void* gzFile;

// Constants used by gzfwrite
const int GZ_WRITE = 1;
const int Z_OK = 0;
const int Z_STREAM_ERROR = -2;
const int Z_NO_FLUSH = 0;
const int Z_FINISH = 2;
const int Z_ERRNO = -1;

// Testing harness globals to simulate internal behaviors
static z_size_t g_write_len_to_write = 0; // how many bytes the mock gz_write should pretend to write
static z_size_t g_last_written_len = 0;   // last requested write length passed to gz_write
static int g_error_called = 0;            // how many times gz_error was invoked
static int g_last_error_code = 0;         // last error code reported via gz_error
static const char* g_last_error_msg = nullptr; // last error message

// Mocked dependencies (minimal, self-contained)
z_size_t gz_write(gz_statep /*state*/, voidpc buf, z_size_t len) {
    // Record what's "attempted" to be written
    g_last_written_len = len;
    // Simulate partial/full writes based on configured global
    if (g_write_len_to_write == (z_size_t)(-1)) {
        // Simulate a failure by reporting 0 bytes written
        return 0;
    }
    if (g_write_len_to_write < len) {
        return g_write_len_to_write; // partial write
    }
    return len; // full write
}

void gz_error(gz_statep /*state*/, int err, const char* /*msg*/) {
    // Track error details for assertions
    g_error_called++;
    g_last_error_code = err;
    g_last_error_msg = /*msg*/ nullptr; // message is not used in assertions, but kept for completeness
}

// The focal function under test (reproduced in-test for self-containment)
z_size_t gzfwrite(voidpc buf, z_size_t size, z_size_t nitems, gzFile file) {
    z_size_t len;
    gz_statep state;
    if (file == NULL)
        return 0;
    state = (gz_statep)file;
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;
    len = nitems * size;
    if (size && len / size != nitems) {
        gz_error(state, Z_STREAM_ERROR, "request does not fit in a size_t");
        return 0;
    }
    return len ? gz_write(state, buf, len) / size : 0;
}

// Simple non-terminating assertion helpers (to maximize code execution)
static int g_failures = 0;
static void expect_eq_uint64(uint64_t a, uint64_t b, const char* msg) {
    if (a != b) {
        std::cerr << "FAIL: " << msg << " | got: " << a << " expected: " << b << "\n";
        ++g_failures;
    } else {
        std::cout << "PASS: " << msg << "\n";
    }
}
static void expect_eq_int(int a, int b, const char* msg) {
    if (a != b) {
        std::cerr << "FAIL: " << msg << " | got: " << a << " expected: " << b << "\n";
        ++g_failures;
    } else {
        std::cout << "PASS: " << msg << "\n";
    }
}

// Test 1: file == NULL should return 0
void test_null_file_returns_zero() {
    char dummy[1] = {0};
    gzFile f = nullptr;
    z_size_t res = gzfwrite(dummy, 1, 1, f);
    expect_eq_uint64(res, 0, "gzfwrite returns 0 when file is NULL");
}

// Test 2a: state->mode != GZ_WRITE should return 0
void test_wrong_mode_returns_zero() {
    gz_state state;
    state.mode = 0; // not GZ_WRITE
    state.err = Z_OK;
    gzFile f = &state;
    char dummy[1] = {0};
    z_size_t res = gzfwrite(dummy, 1, 1, f);
    expect_eq_uint64(res, 0, "gzfwrite returns 0 when mode != GZ_WRITE");
}

// Test 2b: state->err != Z_OK should return 0
void test_err_not_OK_returns_zero() {
    gz_state state;
    state.mode = GZ_WRITE;
    state.err = 1; // not Z_OK
    gzFile f = &state;
    char dummy[1] = {0};
    z_size_t res = gzfwrite(dummy, 1, 1, f);
    expect_eq_uint64(res, 0, "gzfwrite returns 0 when err != Z_OK");
}

// Test 3: Overflow detection triggers gz_error and returns 0
void test_overflow_detection() {
    gz_state state;
    state.mode = GZ_WRITE;
    state.err = Z_OK;
    gzFile f = &state;
    char dummy[1] = {0};
    // Use size != 0 and nitems = max value to force overflow in len = nitems * size
    z_size_t size = 2;
    z_size_t nitems = (z_size_t)(-1); // max uint64
    g_error_called = 0;
    g_last_error_code = 0;
    g_last_error_msg = nullptr;
    z_size_t res = gzfwrite(dummy, size, nitems, f);
    expect_eq_uint64(res, 0, "gzfwrite should return 0 on overflow and trigger Z_STREAM_ERROR");
    expect_eq_int(g_error_called, 1, "gz_error should be called on overflow");
    expect_eq_int(g_last_error_code, Z_STREAM_ERROR, "gz_error should be called with Z_STREAM_ERROR");
}

// Test 4: Full write path (valid size and count, gz_write returns full length)
void test_full_write_path() {
    gz_state state;
    state.mode = GZ_WRITE;
    state.err = Z_OK;
    gzFile f = &state;
    char buffer[12] = {0}; // 12 bytes
    // len = 4 * 3 = 12; gz_write should be called with 12 and return 12
    g_write_len_to_write = 12;
    z_size_t res = gzfwrite(buffer, 4, 3, f);
    expect_eq_uint64(res, 3, "gzfwrite returns 3 when 12 bytes are written in 4-byte items");
}

// Test 5: size == 0 should result in 0 (no operation performed)
void test_size_zero_path() {
    gz_state state;
    state.mode = GZ_WRITE;
    state.err = Z_OK;
    gzFile f = &state;
    char dummy[10] = {0};
    z_size_t res = gzfwrite(dummy, 0, 5, f);
    expect_eq_uint64(res, 0, "gzfwrite returns 0 when size == 0 (no operation)");
}

// Test 6: Partial write path (gz_write returns less than requested)
void test_partial_write_path() {
    gz_state state;
    state.mode = GZ_WRITE;
    state.err = Z_OK;
    gzFile f = &state;
    char buffer[12] = {0};
    g_write_len_to_write = 6; // simulate partial write
    z_size_t res = gzfwrite(buffer, 4, 3, f); // len = 12, expect 6 written -> 6/4 = 1 item
    expect_eq_uint64(res, 1, "gzfwrite should return 1 when gz_write writes 6 bytes of 12 (partial write)");
}

int main() {
    // Run all tests
    test_null_file_returns_zero();
    test_wrong_mode_returns_zero();
    test_err_not_OK_returns_zero();
    test_overflow_detection();
    test_full_write_path();
    test_size_zero_path();
    test_partial_write_path();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
// Unit test suite for the focal method inflateEnd (inflate.c) using C++11.
// This test suite does not rely on GTest and uses a lightweight, non-terminating
// assertion mechanism to maximize code coverage.
// The tests initialize a z_stream (z_streamp) and its internal inflate_state to
// exercise different execution paths inside inflateEnd, including handling of
// NULL pointers and proper freeing of allocated resources via the provided ZFREE
// and zfree hooks.

// Note: This test is designed to be compiled together with inflate.c and its
// associated headers (inflate.h, zutil.h, inftrees.h, inffast.h). The tests
// instantiate the structures and function pointers in a way compatible with
// zlib's API as used by inflateEnd.

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include the C headers that define the zlib-like types and inflateEnd signature.
// Wrapped in extern "C" to ensure correct linkage from C++.
extern "C" {
}

// Lightweight test harness: non-terminating expectations
static int gFailures = 0;
static int gFreedCount = 0;

// Custom memory management hooks to intercept frees allocated by inflateEnd.
// These mimick the zlib allocator/deallocator usage embedded in inflate.c.
static voidpf test_alloc(voidpf opaque, uInt items, uInt size) {
    // Allocate using malloc to be compatible with free() in test_free.
    (void)opaque; // unused in test harness
    return (voidpf)malloc(items * size);
}

static void test_free(voidpf opaque, voidpf address) {
    // Free using the standard free() function and track frees.
    (void)opaque; // unused in test
    if (address) {
        free(address);
        ++gFreedCount;
    }
}

// Simple assertion helpers (non-terminating)
#define EXPECT_EQ(a, b, desc) do { \
    if ((a) != (b)) { \
        std::cerr << "FAIL: " << desc << " | Expected: " << (b) << " Got: " << (a) << std::endl; \
        ++gFailures; \
    } else { \
        std::cout << "PASS: " << desc << std::endl; \
    } \
} while (0)

#define EXPECT_PTR_EQ(a, b, desc) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "FAIL: " << desc << " | Expected pointer: " << b << " Got: " << a << std::endl; \
        ++gFailures; \
    } else { \
        std::cout << "PASS: " << desc << std::endl; \
    } \
} while (0)

/////////////////////////////
// Test 1: inflateEnd(nullptr) should return Z_STREAM_ERROR
/////////////////////////////
void test_inflateEnd_nullptr() {
    // Ensure behavior when NULL stream is passed.
    int ret = inflateEnd(nullptr);
    EXPECT_EQ(ret, Z_STREAM_ERROR, "inflateEnd(nullptr) should return Z_STREAM_ERROR");
}

/////////////////////////////
// Test 2: inflateEnd with a valid z_stream but invalid internal state (state != strm)
/////////////////////////////
void test_inflateEnd_invalid_state() {
    // Allocate a z_stream and a fake inflate_state where state->strm != strm
    z_streamp strm = (z_streamp)std::malloc(sizeof(z_stream));
    if (!strm) {
        std::cerr << "ERROR: Failed to allocate z_stream in test_inflateEnd_invalid_state" << std::endl;
        ++gFailures;
        return;
    }
    // Setup non-null allocators to satisfy inflateStateCheck
    strm->zalloc = test_alloc;
    strm->zfree = test_free;
    strm->opaque = NULL;
    strm->state = NULL;

    // Allocate a state object but intentionally mismatch the strm pointer inside
    struct inflate_state *state = (struct inflate_state*)std::malloc(sizeof(struct inflate_state));
    if (!state) {
        std::cerr << "ERROR: Failed to allocate inflate_state in test_inflateEnd_invalid_state" << std::endl;
        std::free(strm);
        ++gFailures;
        return;
    }
    state->strm = (z_streamp)0xDEADBEEF; // invalid, not equal to strm
    state->mode = HEAD; // ensure within range, but state->strm mismatch should cause failure
    state->window = NULL;
    strm->state = state;

    int ret = inflateEnd(strm);
    EXPECT_EQ(ret, Z_STREAM_ERROR, "inflateEnd should return Z_STREAM_ERROR when state->strm != strm");
    // Clean up (state memory likely not freed due to early return)
    // The state memory may or may not be freed by inflateEnd depending on path;
    // to be safe, only free strm here; if inflateEnd freed state, double-free should be avoided.
    std::free(strm);
    // Do not free 'state' here since inflateEnd may free it on failure path (defensive)
}

/////////////////////////////
// Test 3: inflateEnd with valid state and non-null window pointer should return Z_OK
// and free both window and state; strm->state should become NULL.
/////////////////////////////
void test_inflateEnd_valid_state_with_window() {
    // Prepare stream and allocator hooks
    z_streamp strm = (z_streamp)std::malloc(sizeof(z_stream));
    if (!strm) {
        std::cerr << "ERROR: Failed to allocate z_stream in test_inflateEnd_valid_state_with_window" << std::endl;
        ++gFailures;
        return;
    }
    strm->zalloc = test_alloc;
    strm->zfree = test_free;
    strm->opaque = NULL;

    // Allocate and initialize inflate_state
    struct inflate_state *state = (struct inflate_state*)std::malloc(sizeof(struct inflate_state));
    if (!state) {
        std::cerr << "ERROR: Failed to allocate inflate_state in test_inflateEnd_valid_state_with_window" << std::endl;
        std::free(strm);
        ++gFailures;
        return;
    }
    state->strm = strm;
    state->mode = HEAD; // within valid range [HEAD, SYNC]
    // Allocate a non-null window buffer to exercise the ZFREE branch
    state->window = (unsigned char*)std::malloc(64);
    if (!state->window) {
        std::cerr << "ERROR: Failed to allocate window buffer in test_inflateEnd_valid_state_with_window" << std::endl;
        std::free(state);
        std::free(strm);
        ++gFailures;
        return;
    }
    // Attach the state to the stream
    strm->state = state;

    // Initialize the freed counter
    gFreedCount = 0;

    int ret = inflateEnd(strm);
    EXPECT_EQ(ret, Z_OK, "inflateEnd should return Z_OK for valid state with non-null window");
    // After end, both window and state should have been freed via test_free
    EXPECT_EQ(gFreedCount, 2, "inflateEnd should free both window and state (2 frees)");
    // strm->state should be NULL
    EXPECT_PTR_EQ(strm->state, (void*)nullptr, "strm->state should be NULL after inflateEnd");

    // Clean up the stream struct itself
    std::free(strm);
    // Note: The 'state' memory and 'window' memory have been freed by inflateEnd,
    // so we should not double-free them here.
}

/////////////////////////////
// Test 4: inflateEnd with valid state but NULL window should free only the state.
// Expect Z_OK and exactly 1 free.
/////////////////////////////
void test_inflateEnd_valid_state_without_window() {
    // Prepare stream and allocator hooks
    z_streamp strm = (z_streamp)std::malloc(sizeof(z_stream));
    if (!strm) {
        std::cerr << "ERROR: Failed to allocate z_stream in test_inflateEnd_valid_state_without_window" << std::endl;
        ++gFailures;
        return;
    }
    strm->zalloc = test_alloc;
    strm->zfree = test_free;
    strm->opaque = NULL;

    // Allocate and initialize inflate_state
    struct inflate_state *state = (struct inflate_state*)std::malloc(sizeof(struct inflate_state));
    if (!state) {
        std::cerr << "ERROR: Failed to allocate inflate_state in test_inflateEnd_valid_state_without_window" << std::endl;
        std::free(strm);
        ++gFailures;
        return;
    }
    state->strm = strm;
    state->mode = HEAD; // within valid range
    state->window = NULL; // NULL window to skip first free

    strm->state = state;

    gFreedCount = 0;

    int ret = inflateEnd(strm);
    EXPECT_EQ(ret, Z_OK, "inflateEnd should return Z_OK for valid state with NULL window");
    EXPECT_EQ(gFreedCount, 1, "inflateEnd should free only the state (1 free)");
    EXPECT_PTR_EQ(strm->state, (void*)nullptr, "strm->state should be NULL after inflateEnd");

    // Clean up the stream struct itself
    std::free(strm);
    // The 'state' memory should have been freed by inflateEnd; avoid double-free.
}

/////////////////////////////
// Driver: run all tests
/////////////////////////////
int main() {
    std::cout << "Starting inflateEnd unit tests (C++11, no GTest)" << std::endl;

    test_inflateEnd_nullptr();
    test_inflateEnd_invalid_state();
    test_inflateEnd_valid_state_with_window();
    test_inflateEnd_valid_state_without_window();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " TEST(S) FAILED" << std::endl;
        return -1;
    }
}
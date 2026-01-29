// High-quality C++11 unit tests for the inflateReset function (inflate.c)
// No GTest is used. A lightweight test harness is implemented.
// The tests cover true/false branches of conditions, correct reset behavior,
// and error cases as exercised by inflateStateCheck and inflateResetKeep.
// The tests rely only on the project's provided headers and the C library headers.

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Bring in the C-style zlib types and functions.
// These headers define z_streamp, Z_STREAM_ERROR, Z_OK, etc., and the inflate* APIs.
extern "C" {
}

// Lightweight test harness
static int g_failures = 0;

#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " | Expected: " << (expected) \
                  << "  Actual: " << (actual) << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    if ((ptr) == nullptr) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " | Pointer is NULL" << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define ASSERT_TRUE(expr, msg) do { \
    if (!(expr)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while (0)

// Dummy allocation helpers to satisfy inflateStateCheck in tests
// Signatures come from zlib: alloc_func and free_func
static voidpf dummyAlloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return (void*)malloc(items * size);
}
static void dummyFree(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Test 1: inflateReset should return Z_STREAM_ERROR when input is NULL
static int test_inflateReset_null_input() {
    int failures = 0;
    int res = inflateReset(nullptr);
    ASSERT_EQ(Z_STREAM_ERROR, res, "inflateReset(NULL) should return Z_STREAM_ERROR");
    if (res != Z_STREAM_ERROR) ++failures;
    return failures;
}

// Test 2: inflateReset should return Z_STREAM_ERROR when zalloc or zfree is NULL
static int test_inflateReset_bad_alloc_functions() {
    int failures = 0;
    z_streamp strm = (z_streamp)malloc(sizeof(z_stream));
    if (!strm) { std::cerr << "Failed to allocate z_stream for Test 2\n"; ++failures; return failures; }
    std::memset(strm, 0, sizeof(z_stream));

    // Case: zalloc is non-null but zfree is NULL
    strm->zalloc = dummyAlloc;
    strm->zfree = (free_func)0; // intentionally NULL
    strm->state = nullptr;

    int res = inflateReset(strm);
    ASSERT_EQ(Z_STREAM_ERROR, res, "inflateReset with NULL zfree should return Z_STREAM_ERROR");
    std::free(strm);

    return failures;
}

// Test 3: inflateReset should return Z_STREAM_ERROR when the state pointer is NULL
static int test_inflateReset_null_state() {
    int failures = 0;
    z_streamp strm = (z_streamp)malloc(sizeof(z_stream));
    if (!strm) { std::cerr << "Failed to allocate z_stream for Test 3\n"; ++failures; return failures; }
    std::memset(strm, 0, sizeof(z_stream));

    strm->zalloc = dummyAlloc;
    strm->zfree = dummyFree;
    strm->state = nullptr; // invalid

    int res = inflateReset(strm);
    ASSERT_EQ(Z_STREAM_ERROR, res, "inflateReset with NULL state should return Z_STREAM_ERROR");
    std::free(strm);
    return failures;
}

// Test 4: Valid reset path - ensure several fields are reset by inflateResetKeep
static int test_inflateReset_valid_basic() {
    int failures = 0;

    z_streamp strm = (z_streamp)malloc(sizeof(z_stream));
    if (!strm) { std::cerr << "Failed to allocate z_stream for Test 4\n"; ++failures; return failures; }
    std::memset(strm, 0, sizeof(z_stream));

    // Provide valid allocator/free functions
    strm->zalloc = dummyAlloc;
    strm->zfree = dummyFree;

    // Allocate a realistic inflate_state and attach to stream
    struct inflate_state *state = (struct inflate_state*)malloc(sizeof(struct inflate_state));
    if (!state) { std::cerr << "Failed to allocate inflate_state for Test 4\n"; std::free(strm); ++failures; return failures; }

    // Initialize a plausible non-empty state
    std::memset(state, 0, sizeof(struct inflate_state));
    state->strm = strm;
    state->mode = HEAD;       // valid mode
    state->wrap = 1;          // test adler behavior
    state->total = 123;
    state->havedict = 0;
    state->flags = -1;
    state->dmax = 32768U;
    state->head = nullptr;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    state->sane = 1;
    state->back = -1;

    strm->total_in = 999;
    strm->total_out = 888;
    strm->state = (void*)state;
    // adler will be set if wrap is true during reset; start with some non-zero
    strm->adler = 0;

    int res = inflateReset(strm);
    ASSERT_EQ(Z_OK, res, "inflateReset should return Z_OK for valid state");

    // Verify that core fields have been reset
    ASSERT_EQ(0, state->wsize, "state->wsize should be reset to 0");
    ASSERT_EQ(0, state->whave, "state->whave should be reset to 0");
    ASSERT_EQ(0, state->wnext, "state->wnext should be reset to 0");

    ASSERT_EQ(0, strm->total_in, "strm->total_in should be reset to 0");
    ASSERT_EQ(0, strm->total_out, "strm->total_out should be reset to 0");
    ASSERT_TRUE(strm->msg == nullptr, "strm->msg should be NULL after reset");

    // adler should reflect wrap value
    ASSERT_EQ((unsigned)state->wrap & 1, (unsigned)strm->adler, "strm->adler should reflect wrap flag after reset");

    // mode should be HEAD after reset
    ASSERT_EQ(HEAD, state->mode, "state->mode should be HEAD after reset");

    // Cleanup
    std::free(state);
    std::free(strm);
    return failures;
}

// Test 5: inflateReset should fail if the state's strm pointer does not match the input strm
static int test_inflateReset_invalid_state_pointer() {
    int failures = 0;

    z_streamp strm = (z_streamp)malloc(sizeof(z_stream));
    if (!strm) { std::cerr << "Failed to allocate z_stream for Test 5\n"; ++failures; return failures; }
    std::memset(strm, 0, sizeof(z_stream));

    strm->zalloc = dummyAlloc;
    strm->zfree = dummyFree;

    // Correct state object but we will intentionally mismatch the strm pointer it points to
    struct inflate_state *stateGood = (struct inflate_state*)malloc(sizeof(struct inflate_state));
    struct inflate_state *stateBad = (struct inflate_state*)malloc(sizeof(struct inflate_state));
    if (!stateGood || !stateBad) { std::cerr << "Failed to allocate inflate_state for Test 5\n"; ++failures; return failures; }

    std::memset(stateGood, 0, sizeof(struct inflate_state));
    std::memset(stateBad, 0, sizeof(struct inflate_state));

    stateGood->strm = strm;
    stateBad->strm = nullptr; // wrong association

    // Put the bad state into the stream
    strm->state = (void*)stateBad;

    int res = inflateReset(strm);
    ASSERT_EQ(Z_STREAM_ERROR, res, "inflateReset with mismatched state->strm should return Z_STREAM_ERROR");

    // Cleanup
    std::free(stateGood);
    std::free(stateBad);
    std::free(strm);
    return failures;
}

// Test 6: inflateReset should fail if the state's mode is invalid (outside HEAD..SYNC)
static int test_inflateReset_invalid_mode() {
    int failures = 0;

    z_streamp strm = (z_streamp)malloc(sizeof(z_stream));
    if (!strm) { std::cerr << "Failed to allocate z_stream for Test 6\n"; ++failures; return failures; }
    std::memset(strm, 0, sizeof(z_stream));

    strm->zalloc = dummyAlloc;
    strm->zfree = dummyFree;

    struct inflate_state *state = (struct inflate_state*)malloc(sizeof(struct inflate_state));
    if (!state) { std::cerr << "Failed to allocate inflate_state for Test 6\n"; ++failures; std::free(strm); return failures; }

    std::memset(state, 0, sizeof(struct inflate_state));
    state->strm = strm;
    state->mode = HEAD - 1; // explicitly invalid (below HEAD)

    strm->state = (void*)state;

    int res = inflateReset(strm);
    ASSERT_EQ(Z_STREAM_ERROR, res, "inflateReset with invalid mode should return Z_STREAM_ERROR");

    // Cleanup
    std::free(state);
    std::free(strm);
    return failures;
}

int main() {
    // Run all tests
    int totalFailures = 0;

    totalFailures += test_inflateReset_null_input();
    totalFailures += test_inflateReset_bad_alloc_functions();
    totalFailures += test_inflateReset_null_state();
    totalFailures += test_inflateReset_valid_basic();
    totalFailures += test_inflateReset_invalid_state_pointer();
    totalFailures += test_inflateReset_invalid_mode();

    if (g_failures == 0) {
        std::cout << "All inflateReset tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " inflateReset tests FAILED." << std::endl;
        return g_failures;
    }
}
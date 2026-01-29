// InflatePrime_test.cpp
// A lightweight C++11 unit test suite for the focal method inflatePrime.
// The tests rely on the public zlib interface exposed via zlib.h.
// They do not use Google Test; instead, a tiny custom test harness prints PASS/FAIL
// and a final summary. The tests exercise true/false branches and key state mutations
// by driving the internal inflate_state through the public inflatePrime API.
//
// Note: This test assumes zlib is available in the include/library path as <zlib.h>.
// Compile with something like: g++ -std=c++11 InflatePrime_test.cpp -lz -o InflatePrime_test

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include zlib public API
extern "C" {
}

// Forward declaration of the internal inflate_state structure and related fields.
// We rely on the public headers to provide the necessary internal layout for testing.
// If required, a project setup with the appropriate private headers may be needed.
// We access state->hold and state->bits to validate behavior, and state->strm, state->mode.
struct inflate_state;

// Dummy alloc/free functions to satisfy z_stream's zalloc/zfree requirements.
static voidpf dummyAlloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}
static void dummyFree(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Helper: create a test z_stream with an associated inflate_state.
// The state is zero-initialized to start from a clean baseline, with mode set to HEAD (0 assumed).
static bool create_test_stream(z_stream** outStrm) {
    if (!outStrm) return false;
    z_stream* strm = (z_stream*)malloc(sizeof(z_stream));
    if (!strm) return false;
    memset(strm, 0, sizeof(z_stream));

    // Initialize zalloc/zfree so inflatePrime can call inflateStateCheck safely.
    strm->zalloc = dummyAlloc;
    strm->zfree = dummyFree;
    strm->opaque = NULL;

    // Allocate and zero inflate_state; inflatePrime will cast to this struct internally.
    strm->state = malloc(sizeof(struct inflate_state));
    if (!strm->state) {
        free(strm);
        return false;
    }
    memset(strm->state, 0, sizeof(struct inflate_state));

    // Set up minimal, consistent fields expected by inflateStateCheck
    // The exact fields depend on the zlib version; we set commonly required ones.
    struct inflate_state* state = (struct inflate_state*)strm->state;
    state->strm = strm;
    // Initialize mode to a value within HEAD..SYNC range. We assume HEAD == 0.
    state->mode = 0;
    state->hold = 0;
    state->bits = 0;

    *outStrm = strm;
    return true;
}

// Helper: destroy test z_stream and clean up allocations.
static void destroy_test_stream(z_stream* strm) {
    if (!strm) return;
    if (strm->state) free(strm->state);
    free(strm);
}

// Simple test harness helpers
static int g_total = 0;
static int g_passed = 0;

static void expect_true(bool cond, const char* desc) {
    g_total++;
    if (cond) {
        g_passed++;
        printf("PASS: %s\n", desc);
    } else {
        printf("FAIL: %s\n", desc);
    }
}

// Test 1: bits == 0 should return Z_OK immediately and not mutate state
static void test_inflatePrime_bits_zero() {
    // Setup
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 1: setup failed\n");
        return;
    }

    // Action
    int ret = inflatePrime(strm, 0, 0);

    // Verification
    // Should be Z_OK and state unchanged (hold=0, bits=0)
    struct inflate_state* st = (struct inflate_state*)strm->state;
    bool ok = (ret == Z_OK) && (st->hold == 0) && (st->bits == 0);
    expect_true(ok, "inflatePrime with bits=0 returns Z_OK and keeps state unchanged");

    // Cleanup
    destroy_test_stream(strm);
}

// Test 2: bits < 0 should reset hold and bits to 0 and return Z_OK
static void test_inflatePrime_negative_bits_resets() {
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 2: setup failed\n");
        return;
    }

    // First, perform a call that mutates the state: bits=2, value=1
    inflatePrime(strm, 2, 1);

    // Now call with negative bits; expect reset
    int ret = inflatePrime(strm, -1, 0);

    struct inflate_state* st = (struct inflate_state*)strm->state;
    bool ok = (ret == Z_OK) && (st->hold == 0) && (st->bits == 0);
    expect_true(ok, "inflatePrime with bits<0 resets hold and bits and returns Z_OK");

    destroy_test_stream(strm);
}

// Test 3: bits > 16 should yield Z_STREAM_ERROR
static void test_inflatePrime_bits_exceed_16_error() {
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 3: setup failed\n");
        return;
    }

    int ret = inflatePrime(strm, 17, 0);
    bool ok = (ret == Z_STREAM_ERROR);
    expect_true(ok, "inflatePrime with bits>16 returns Z_STREAM_ERROR");

    destroy_test_stream(strm);
}

// Test 4: Overflow of state.bits when adding bits would exceed 32 (state.bits + bits > 32)
static void test_inflatePrime_bits_overflow_error() {
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 4: setup failed\n");
        return;
    }

    // First call: add 16 bits to reach 16
    inflatePrime(strm, 16, 0xFFFF);
    // Second call: add another 16 bits to reach 32
    inflatePrime(strm, 16, 0xFFFF);
    // Third call: attempting to add 1 bit would overflow (33 > 32)
    int ret = inflatePrime(strm, 1, 0);

    bool ok = (ret == Z_STREAM_ERROR);
    expect_true(ok, "inflatePrime detects state.bits overflow (>32) and returns Z_STREAM_ERROR");

    destroy_test_stream(strm);
}

// Test 5: Two-step normal operation: ensure cumulative hold/bits update correctly
static void test_inflatePrime_two_step_normal() {
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 5: setup failed\n");
        return;
    }

    // Step 1: bits=2, value=3 -> hold = 3, bits = 2
    inflatePrime(strm, 2, 3);

    // Step 2: bits=3, value=5 -> hold += 5 << 2 = 20; total hold = 23; bits = 5
    inflatePrime(strm, 3, 5);

    struct inflate_state* st = (struct inflate_state*)strm->state;
    bool ok = (st->hold == 23) && (st->bits == 5);
    expect_true(ok, "inflatePrime two-step operation updates hold and bits cumulatively to expected values");

    destroy_test_stream(strm);
}

// Test 6: Masking behavior: value masked to bits
static void test_inflatePrime_value_masking() {
    z_stream* strm = NULL;
    if (!create_test_stream(&strm)) {
        printf("Test 6: setup failed\n");
        return;
    }

    // bits=2, value=0b1111 -> masked to 0b11 (3); hold = 3
    inflatePrime(strm, 2, 0xF);

    struct inflate_state* st = (struct inflate_state*)strm->state;
    bool ok = (st->hold == 3) && (st->bits == 2);
    expect_true(ok, "inflatePrime masks value according to bits (value & ((1<<bits)-1))");

    destroy_test_stream(strm);
}

int main() {
    printf("Starting inflatePrime unit tests (no GTest, simple harness)...\n");

    test_inflatePrime_bits_zero();
    test_inflatePrime_negative_bits_resets();
    test_inflatePrime_bits_exceed_16_error();
    test_inflatePrime_bits_overflow_error();
    test_inflatePrime_two_step_normal();
    test_inflatePrime_value_masking();

    printf("Tests completed. Passed %d/%d tests.\n", g_passed, g_total);
    return (g_total == g_passed) ? 0 : 1;
}
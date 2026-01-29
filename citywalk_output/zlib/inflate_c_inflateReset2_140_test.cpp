// Unit tests for inflateReset2 (C function) using a lightweight C++11 test harness.
// The tests exercise various branches within inflateReset2 by driving a real zlib-like
// environment provided by the project's inflate.c and headers.
// This file uses the library's public API (inflateInit2_, inflateReset2, inflateEnd, etc.)
// to initialize and manipulate the internal inflate_state.
// No GoogleTest/GMock; a small custom test harness is implemented.

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Candidate Keywords (Step 1): Core components and dependencies observed in inflateReset2
// - inflateStateCheck, struct inflate_state, z_streamp (z_stream)
//
// - windowBits handling:
//     negative values: windowBits < 0, with -15 bound
//     non-negative: wrap calculation, valid range 8..15 (0 allowed as a special case)
//
// - state->window and state->wbits management (free window if different wbits)
// - wrap and wbits assignment
// - inflateReset(strm) invocation after state updates
//
// - Macros: Z_NULL, ZFREE, ZSTREAM_ERROR, Z_OK, Z_STREAM_ERROR
// - Optional GUNZIP path (ifdef GUNZIP) altering windowBits behavior
//
// - Test goals: 
//   1) Negative out-of-range (e.g., -16) => Z_STREAM_ERROR
//   2) Negative within range (e.g., -8) => wrap=0, windowBits=8
//   3) Positive valid windowBits (e.g., 15) => wrap computed, wbits set
//   4) windowBits == 0 => wbits 0, wrap computed
//   5) Existing non-null state->window freed when wbits changes
//
// Step 2/3: Test Suite (inflateReset2) implemented with a minimal C/C++ harness
// - Uses the library's inflateInit2_, inflateReset2, inflateEnd
// - Verifies internal state after inflation reset requests
// - Tracks memory frees to verify ZFREE behavior
// - Maintains non-terminating assertions: counts passes/failures, prints results
// - Accesses internal fields (inflate_state) via the provided headers; assumes public exposure

extern "C" {
}

// Simple test context to accumulate results without terminating on failures
struct TestContext {
    int passes = 0;
    int failures = 0;

    void report(bool ok, const char* msg) {
        if (ok) {
            ++passes;
        } else {
            ++failures;
            std::fprintf(stderr, "Test Failure: %s\n", msg);
        }
    }

    void summary() const {
        std::printf("Tests passed: %d, failed: %d\n", passes, failures);
    }

    bool ok() const { return failures == 0; }
};

// Simple global to capture which pointer was freed (for test 5)
static void* g_last_freed_ptr = nullptr;

// Mocked allocator/free wrappers to track frees without altering library behavior
static voidpf test_alloc(voidpf opaque, unsigned int items, unsigned int size) {
    (void)opaque; // unused
    void* p = std::malloc(items * size);
    if (p) std::memset(p, 0, items * size);
    return p;
}

static void test_free(voidpf opaque, voidpf address) {
    (void)opaque; // unused
    g_last_freed_ptr = address;
    if (address != nullptr)
        std::free(address);
}

// Test 1: Negative windowBits less than -15 should yield Z_STREAM_ERROR
void test_inflateReset2_negative_too_small(TestContext& ctx) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    strm.zalloc = (alloc_func)test_alloc;
    strm.zfree  = (free_func)test_free;
    strm.opaque = nullptr;

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    ctx.report(ret == Z_OK, "inflateInit2_ should succeed with windowBits=15");

    // Call with an out-of-range negative value
    ret = inflateReset2(&strm, -16);
    ctx.report(ret == Z_STREAM_ERROR, "inflateReset2(-16) should return Z_STREAM_ERROR");

    inflateEnd(&strm);
}

// Test 2: Negative within range (-8) should set wrap=0 and wbits=8
void test_inflateReset2_negative_within_range(TestContext& ctx) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    strm.zalloc = (alloc_func)test_alloc;
    strm.zfree  = (free_func)test_free;
    strm.opaque = nullptr;

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    ctx.report(ret == Z_OK, "inflateInit2_ should succeed");

    ret = inflateReset2(&strm, -8);
    ctx.report(ret == Z_OK, "inflateReset2(-8) should return Z_OK");

    struct inflate_state* state = (struct inflate_state*)strm.state;
    ctx.report(state != nullptr && state->wrap == 0, "state->wrap should be 0 after -8");
    ctx.report(state != nullptr && state->wbits == 8, "state->wbits should be 8 after -8");

    inflateEnd(&strm);
}

// Test 3: Positive valid windowBits (15) should compute wrap and set wbits
void test_inflateReset2_positive_windowbits(TestContext& ctx) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    strm.zalloc = (alloc_func)test_alloc;
    strm.zfree  = (free_func)test_free;
    strm.opaque = nullptr;

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    ctx.report(ret == Z_OK, "inflateInit2_ should succeed for 15");

    ret = inflateReset2(&strm, 15);
    ctx.report(ret == Z_OK, "inflateReset2(15) should return Z_OK");

    struct inflate_state* state = (struct inflate_state*)strm.state;
    ctx.report(state != nullptr && state->wrap == 5, "state->wrap should be 5 for windowBits=15");
    ctx.report(state != nullptr && state->wbits == 15, "state->wbits should be 15 for windowBits=15");

    inflateEnd(&strm);
}

// Test 4: windowBits = 0 should be accepted and set wbits=0, wrap computed
void test_inflateReset2_zero_windowbits(TestContext& ctx) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    strm.zalloc = (alloc_func)test_alloc;
    strm.zfree  = (free_func)test_free;
    strm.opaque = nullptr;

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    ctx.report(ret == Z_OK, "inflateInit2_ should succeed");

    ret = inflateReset2(&strm, 0);
    ctx.report(ret == Z_OK, "inflateReset2(0) should return Z_OK");

    struct inflate_state* state = (struct inflate_state*)strm.state;
    ctx.report(state != nullptr && state->wbits == 0, "state->wbits should be 0 after windowBits=0");
    ctx.report(state != nullptr && state->wrap == 5, "state->wrap should be 5 when windowBits=0");

    inflateEnd(&strm);
}

// Test 5: If a non-null window exists with a different wbits, it should be freed
void test_inflateReset2_free_existing_window(TestContext& ctx) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    strm.zalloc = (alloc_func)test_alloc;
    strm.zfree  = (free_func)test_free;
    strm.opaque = nullptr;

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    ctx.report(ret == Z_OK, "inflateInit2_ should succeed");

    struct inflate_state* state = (struct inflate_state*)strm.state;
    // Manually simulate an existing window allocation
    void* dummy = std::malloc(128);
    state->window = dummy;
    state->wbits = 15; // different from the forthcoming windowBits=8
    g_last_freed_ptr = nullptr;

    ret = inflateReset2(&strm, 8);
    ctx.report(ret == Z_OK, "inflateReset2(8) should succeed and free existing window");

    // The library should have freed the previous window
    ctx.report(g_last_freed_ptr == dummy, "previous window should have been freed via ZFREE");
    ctx.report(state->window == nullptr, "state->window should be NULL after freeing old window");

    // Note: Do not manually free dummy here; test_free/freeing is handled by the mock
    inflateEnd(&strm);
}

int main() {
    TestContext ctx;

    // Grouped tests to broaden coverage of inflateReset2 behavior
    test_inflateReset2_negative_too_small(ctx);
    test_inflateReset2_negative_within_range(ctx);
    test_inflateReset2_positive_windowbits(ctx);
    test_inflateReset2_zero_windowbits(ctx);
    test_inflateReset2_free_existing_window(ctx);

    ctx.summary();
    // Return non-zero on failure to allow CI scripts to detect issues
    return ctx.failures == 0 ? 0 : 1;
}
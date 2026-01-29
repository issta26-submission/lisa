// Test suite for inflateGetHeader in inflate.c (C++11, no GTest)
// This test harness uses a lightweight, non-terminating test runner.
// It exercises both true and false branches of inflateGetHeader by constructing
// a minimal z_streamp environment compatible with the internal expectations of inflateStateCheck.
//
// Notes:
// - The test depends on internal zlib headers (inflate.h) to access the internal
//   inflate_state structure and constants like HEAD.
// - Tests allocate and initialize the internal state to satisfy inflateStateCheck.
// - The tests avoid terminating on failure; they log results and continue.
//
// To run: Compile this file together with inflate.c (the implementation under test) and link normally.

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include internal zlib headers used by inflate.c
extern "C" {
}

// Ensure C linkage for the FFI function under test
extern "C" int inflateGetHeader(z_streamp strm, gz_headerp head);

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

static void log_pass(const char* msg) {
    std::cout << "[PASS] " << msg << std::endl;
}
static void log_fail(const char* msg) {
    std::cerr << "[FAIL] " << msg << std::endl;
}
static void expect(bool cond, const char* desc) {
    ++g_total;
    if (cond) {
        log_pass(desc);
    } else {
        ++g_failed;
        log_fail(desc);
    }
}

// Simple non-terminating tests that exercise inflateGetHeader

// Test 1: Success path - wrap bit set, valid state, head provided
static void test_inflateGetHeader_success() {
    // Prepare a z_streamp (z_stream) instance
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));

    // Implement non-null allocation/free callbacks
    auto my_alloc = [](voidpf opaque, uInt items, uInt size) -> voidpf {
        return (voidpf)std::malloc((size_t)items * (size_t)size);
    };
    auto my_free = [](voidpf opaque, voidpf address) -> void {
        (void)opaque; // unused
        std::free(address);
    };

    zstrm.zalloc = my_alloc;
    zstrm.zfree = my_free;
    zstrm.opaque = nullptr;

    // Allocate and initialize internal inflate_state
    struct inflate_state *state = new struct inflate_state;
    // Initialize minimal fields required by inflateStateCheck
    state->strm = &zstrm;
    state->mode = HEAD;       // within [HEAD, SYNC]
    state->wrap = 2;            // ensure (wrap & 2) != 0
    state->head = nullptr;

    zstrm.state = state;

    // Prepare a header structure
    gz_header header;
    std::memset(&header, 0, sizeof(gz_header));
    gz_headerp headp = &header;

    // Call the function under test
    int ret = inflateGetHeader(&zstrm, headp);

    // Validate: should return Z_OK and set head->done to 0
    expect(ret == Z_OK, "inflateGetHeader should return Z_OK on valid state");
    expect(headp->done == 0, "inflateGetHeader should initialize head->done to 0");
    expect(state->head == headp, "inflateGetHeader should store head in state->head");

    // Cleanup
    delete state;
}

// Test 2: Failure path - NULL strm should yield Z_STREAM_ERROR
static void test_inflateGetHeader_null_strm() {
    // Prepare a header object
    gz_header header;
    std::memset(&header, 0, sizeof(gz_header));
    gz_headerp headp = &header;

    int ret = inflateGetHeader(nullptr, headp);
    expect(ret == Z_STREAM_ERROR, "inflateGetHeader should return Z_STREAM_ERROR when strm is NULL");
}

// Test 3: Failure path - missing wrap flag (bit 1 << 1) causing wrap check to fail
static void test_inflateGetHeader_missing_wrap() {
    // Prepare a z_streamp (z_stream) instance
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));

    // Allocation/free callbacks
    auto my_alloc = [](voidpf opaque, uInt items, uInt size) -> voidpf {
        return (voidpf)std::malloc((size_t)items * (size_t)size);
    };
    auto my_free = [](voidpf opaque, voidpf address) -> void {
        (void)opaque;
        std::free(address);
    };

    zstrm.zalloc = my_alloc;
    zstrm.zfree = my_free;
    zstrm.opaque = nullptr;

    // Allocate internal state, but set wrap to 0 to fail the (wrap & 2) check
    struct inflate_state *state = new struct inflate_state;
    state->strm = &zstrm;
    state->mode = HEAD;
    state->wrap = 0; // lacking bit 2
    state->head = nullptr;

    zstrm.state = state;

    // Prepare header
    gz_header header;
    std::memset(&header, 0, sizeof(gz_header));
    gz_headerp headp = &header;

    int ret = inflateGetHeader(&zstrm, headp);

    // Validate: should return Z_STREAM_ERROR due to wrap condition
    expect(ret == Z_STREAM_ERROR, "inflateGetHeader should fail when (state->wrap & 2) == 0");
    
    // Cleanup
    delete state;
}

// Test 4: Verification that head->done is set to 0 (again) when state is valid
static void test_inflateGetHeader_headDoneReset() {
    // Prepare a z_streamp (z_stream) instance
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));

    auto my_alloc = [](voidpf opaque, uInt items, uInt size) -> voidpf {
        return (voidpf)std::malloc((size_t)items * (size_t)size);
    };
    auto my_free = [](voidpf opaque, voidpf address) -> void {
        (void)opaque;
        std::free(address);
    };

    zstrm.zalloc = my_alloc;
    zstrm.zfree = my_free;
    zstrm.opaque = nullptr;

    struct inflate_state *state = new struct inflate_state;
    state->strm = &zstrm;
    state->mode = HEAD;
    state->wrap = 2;
    state->head = nullptr;

    zstrm.state = state;

    gz_header header;
    header.done = 123; // ensure non-zero before call
    gz_headerp headp = &header;

    int ret = inflateGetHeader(&zstrm, headp);

    // Validate: should be Z_OK and head->done should be reset to 0
    expect(ret == Z_OK, "inflateGetHeader should return Z_OK and reset head->done");
    expect(headp->done == 0, "head->done should be set to 0 by inflateGetHeader");

    delete state;
}

// Driver
int main() {
    std::cout << "Starting inflateGetHeader tests..." << std::endl;

    test_inflateGetHeader_success();
    test_inflateGetHeader_null_strm();
    test_inflateGetHeader_missing_wrap();
    test_inflateGetHeader_headDoneReset();

    std::cout << "Tests completed. Total: " << g_total << ", Passed: "
              << (g_total - g_failed) << ", Failed: " << g_failed << std::endl;
    return (g_failed == 0) ? 0 : 1;
}
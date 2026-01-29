// Unit test suite for inflateStateCheck (via public wrappers) in inflate.c
// This test is written for C++11, uses the project-provided headers, and avoids
// any external test framework (GTest). It exercises true/false branches of the
// condition predicates within inflateStateCheck by invoking inflateResetKeep
// (the public function that calls the focal static helper).

#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include the project headers that declare zlib-like types and the inflate API.
// These headers are expected to be available in the project under test.
extern "C" {
}

// Provide dummy implementations for alloc/free function pointers used by the
// zlib-like API. These allow us to construct z_streamp objects that pass
// through inflateStateCheck without performing real memory management.
extern "C" void* dummy_alloc(void* opaque, unsigned int items, unsigned int size);
extern "C" void  dummy_free(void* opaque, void* address);

extern "C" void* dummy_alloc(void* opaque, unsigned int items, unsigned int size) {
    (void)opaque; (void)items; (void)size;
    return NULL;
}

extern "C" void dummy_free(void* opaque, void* address) {
    (void)opaque; (void)address;
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_passed = 0;
#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_total; \
    if ((expected) != (actual)) { \
        std::cerr << "FAIL: " << msg << " | Expected " << (expected) \
                  << ", got " << (actual) << std::endl; \
    } else { \
        ++g_passed; \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while(0)

static void test_inflateStateCheck_null_ptr() {
    // Case: strm is Z_NULL
    z_streamp s = Z_NULL;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR for null strm");
}

static void test_inflateStateCheck_null_zalloc() {
    // Case: strm non-null but zalloc is NULL
    z_stream z;
    z_streamp s = &z;
    s->zalloc = (alloc_func)0; // NULL cast to alloc_func
    s->zfree  = dummy_free;
    s->state  = NULL; // Not relevant due to early check, but keep consistent
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR when zalloc is NULL");
}

static void test_inflateStateCheck_null_state() {
    // Case: zalloc/zfree valid but state == NULL
    z_stream z;
    z_streamp s = &z;
    s->zalloc = dummy_alloc;
    s->zfree  = dummy_free;
    s->state  = NULL;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR when internal state is NULL");
}

static void test_inflateStateCheck_state_strm_mismatch() {
    // Case: state->strm does not point to the provided strm
    z_stream z;
    z_streamp s = &z;
    s->zalloc = dummy_alloc;
    s->zfree  = dummy_free;
    inflate_state* st = new inflate_state;
    st->strm = reinterpret_cast<z_streamp>(0xDEADBEEF); // wrong pointer
    st->mode = HEAD;
    s->state = st;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR when state->strm != strm");
    delete st;
}

static void test_inflateStateCheck_state_mode_out_of_range() {
    // Case: state->mode is out of allowed range
    z_stream z;
    z_streamp s = &z;
    s->zalloc = dummy_alloc;
    s->zfree  = dummy_free;
    inflate_state* st = new inflate_state;
    st->strm = s;
    st->mode = HEAD - 1; // below HEAD
    s->state = st;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR when mode < HEAD");
    delete st;
}

static void test_inflateStateCheck_state_mode_out_of_range_upper() {
    // Case: state->mode above SYNC
    z_stream z;
    z_streamp s = &z;
    s->zalloc = dummy_alloc;
    s->zfree  = dummy_free;
    inflate_state* st = new inflate_state;
    st->strm = s;
    st->mode = SYNC + 1;
    s->state = st;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_STREAM_ERROR, ret, "inflateResetKeep returns Z_STREAM_ERROR when mode > SYNC");
    delete st;
}

static void test_inflateStateCheck_valid_path() {
    // Case: All conditions satisfied -> should return Z_OK
    z_stream z;
    z_streamp s = &z;
    s->zalloc = dummy_alloc;
    s->zfree  = dummy_free;
    inflate_state* st = new inflate_state;
    st->strm = s;
    st->mode = HEAD; // in-range
    s->state = st;
    int ret = inflateResetKeep(s);
    EXPECT_EQ(Z_OK, ret, "inflateResetKeep returns Z_OK for valid state");
    delete st;
}

// Entry point
int main() {
    // Run tests
    test_inflateStateCheck_null_ptr();
    test_inflateStateCheck_null_zalloc();
    test_inflateStateCheck_null_state();
    test_inflateStateCheck_state_strm_mismatch();
    test_inflateStateCheck_state_mode_out_of_range();
    test_inflateStateCheck_state_mode_out_of_range_upper();
    test_inflateStateCheck_valid_path();

    // Summary
    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << std::endl;
    return (g_total == g_passed) ? 0 : 1;
}
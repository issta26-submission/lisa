// Unit test suite for inflateResetKeep in inflate.c
// This test suite is self-contained, uses a minimal test harness (no GTest),
// and exercises multiple paths of inflateResetKeep as described in the task.
// It relies on the existing zlib-like types and headers provided by the project.

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include the project's internal headers to access the exact structures and
// function prototypes used by inflateResetKeep.
extern "C" {
}

// Dummy memory management functions to satisfy inflateStateCheck requirements.
static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return (voidpf)malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Simple, non-terminating assertion helper
static bool assert_true(bool cond, const char* msg) {
    if (!cond) {
        fprintf(stderr, "Assertion failed: %s\n", msg);
    }
    return cond;
}

// Test 1: inflateResetKeep should return Z_STREAM_ERROR for a null/invalid stream pointer
static bool test_inflateResetKeep_null_ptr_returns_error() {
    int rc = inflateResetKeep(nullptr);
    bool ok = (rc == Z_STREAM_ERROR);
    if (!ok) {
        fprintf(stderr, "Test 1 failed: expected Z_STREAM_ERROR, got %d\n", rc);
    } else {
        printf("Test 1 passed: inflateResetKeep(nullptr) returned Z_STREAM_ERROR as expected.\n");
    }
    return ok;
}

// Test 2: inflateResetKeep with wrap == 0 resets various fields and preserves adler
//          when wrap is 0, adler should be left untouched.
static bool test_inflateResetKeep_wrap_zero_resets_fields() {
    z_stream s;
    memset(&s, 0, sizeof(s));

    // Provide non-null allocators to satisfy inflateStateCheck
    s.zalloc = dummy_alloc;
    s.zfree  = dummy_free;

    // Allocate and initialize internal inflate_state
    struct inflate_state state;
    memset(&state, 0, sizeof(state));
    state.strm = &s;

    // Prepare the field values before reset
    s.total_in  = 1234;
    s.total_out = 5678;
    state.total = 42;           // will be reset to 0
    s.msg = (Z_NULL ? NULL : (char*)0x1); // non-null to ensure it's reset to Z_NULL

    state.wrap = 0;               // false branch: adler should not be updated
    s.adler = 0xDEADBEEF;          // should remain unchanged when wrap == 0

    state.mode    = HEAD;           // valid range
    state.last    = 1;              // will be reset to 0
    state.havedict = 1;              // will be reset to 0
    state.flags   = -1;             // will be reset to -1
    state.dmax    = 999999;         // will be reset to 32768U
    state.head    = (void*)0xABCDE;   // will be reset to nullptr
    state.hold    = 0xFFFFFFFF;       // will be reset to 0
    state.bits    = 16;               // will be reset to 0

    // Pointers: set to NULL so that reset assigns codes (NULL) as well
    state.codes = nullptr;
    state.lencode = state.distcode = state.next = state.codes;

    state.sane = 1;
    state.back = -1;

    // Link stream and state
    s.state = &state;

    // Call the function under test
    int ret = inflateResetKeep(&s);

    // Validate return code
    bool ok = (ret == Z_OK);
    if (!ok) {
        fprintf(stderr, "Test 2 failed: expected Z_OK, got %d\n", ret);
        return false;
    }

    // Validate core resets
    if (!assert_true(s.total_in  == 0, "total_in should be reset to 0") ) return false;
    if (!assert_true(s.total_out == 0, "total_out should be reset to 0") ) return false;
    if (!assert_true(state.total == 0,     "state.total should be reset to 0") ) return false;

    // msg should be reset to Z_NULL
    if (!assert_true(s.msg == Z_NULL, "msg should be reset to Z_NULL")) return false;

    // wrap == 0 should not change adler
    if (!assert_true(s.adler == 0xDEADBEEF, "adler should remain unchanged when wrap == 0")) return false;

    // Critical state resets
    if (!assert_true(state.mode == HEAD, "state.mode should be HEAD")) return false;
    if (!assert_true(state.last == 0, "state.last should be reset to 0")) return false;
    if (!assert_true(state.havedict == 0, "state.havedict should be 0")) return false;
    if (!assert_true(state.flags == -1, "state.flags should be -1")) return false;
    if (!assert_true(state.dmax == 32768U, "state.dmax should be 32768U")) return false;
    if (!assert_true(state.head == nullptr, "state.head should be nullptr")) return false;
    if (!assert_true(state.hold == 0, "state.hold should be 0")) return false;
    if (!assert_true(state.bits == 0, "state.bits should be 0")) return false;

    // lencode/distcode/next should all point to codes
    if (!assert_true(state.lencode == state.codes, "state.lencode should equal state.codes")) return false;
    if (!assert_true(state.distcode == state.codes, "state.distcode should equal state.codes")) return false;
    if (!assert_true(state.next == state.codes, "state.next should equal state.codes")) return false;

    if (!assert_true(state.sane == 1, "state.sane should be 1")) return false;
    if (!assert_true(state.back == -1, "state.back should be -1")) return false;

    printf("Test 2 passed: inflateResetKeep with wrap=0 resets fields correctly.\n");
    return true;
}

// Test 3: inflateResetKeep with wrap == 1 updates adler to (wrap & 1) and resets fields.
// This confirms the true branch of the adler update.
static bool test_inflateResetKeep_wrap_one_updates_adler() {
    z_stream s;
    memset(&s, 0, sizeof(s));

    s.zalloc = dummy_alloc;
    s.zfree  = dummy_free;

    struct inflate_state state;
    memset(&state, 0, sizeof(state));
    state.strm = &s;

    // Set up a different adler value to observe change
    s.adler = 0x1234;
    state.wrap = 1;  // non-zero => adler should be updated to (wrap & 1) => 1

    // Pre-reset stream values
    s.total_in  = 10;
    s.total_out = 20;
    state.total = 5;
    s.msg = (char*)0x1; // non-null pre-reset

    state.mode  = HEAD;
    state.last  = 7;
    state.havedict = 1;
    state.flags = -2;
    state.dmax = 9999;
    state.head = (void*)0xBEEF;
    state.hold = 0xAA55;
    state.bits = 12;

    state.codes = nullptr;
    state.lencode = state.distcode = state.next = state.codes;

    state.sane = 0;
    state.back = 2;

    s.state = &state;

    int ret = inflateResetKeep(&s);
    if (ret != Z_OK) {
        fprintf(stderr, "Test 3 failed: expected Z_OK, got %d\n", ret);
        return false;
    }

    // adler should now be (state.wrap & 1) == 1
    if (!assert_true(s.adler == 1, "adler should be updated to 1 when wrap is non-zero")) return false;

    // core resets (same checks as Test 2)
    if (!assert_true(s.total_in  == 0, "total_in should be reset to 0")) return false;
    if (!assert_true(s.total_out == 0, "total_out should be reset to 0")) return false;
    if (!assert_true(state.total == 0, "state.total should be reset to 0")) return false;
    if (!assert_true(s.msg == Z_NULL, "msg should be reset to Z_NULL")) return false;

    // state resets
    if (!assert_true(state.mode == HEAD, "state.mode should be HEAD")) return false;
    if (!assert_true(state.last == 0, "state.last should be reset to 0")) return false;
    if (!assert_true(state.havedict == 0, "state.havedict should be 0")) return false;
    if (!assert_true(state.flags == -1, "state.flags should be -1")) return false;
    if (!assert_true(state.dmax == 32768U, "state.dmax should be 32768U")) return false;
    if (!assert_true(state.head == nullptr, "state.head should be nullptr")) return false;
    if (!assert_true(state.hold == 0, "state.hold should be 0")) return false;
    if (!assert_true(state.bits == 0, "state.bits should be 0")) return false;
    if (!assert_true(state.sane == 1, "state.sane should be 1")) return false;
    if (!assert_true(state.back == -1, "state.back should be -1")) return false;
    if (!assert_true(state.lencode == state.codes, "state.lencode should equal state.codes")) return false;
    if (!assert_true(state.distcode == state.codes, "state.distcode should equal state.codes")) return false;
    if (!assert_true(state.next == state.codes, "state.next should equal state.codes")) return false;

    printf("Test 3 passed: inflateResetKeep with wrap=1 updates adler and resets fields correctly.\n");
    return true;
}

// Test 4: inflateResetKeep should return Z_STREAM_ERROR when internal state is invalid
static bool test_inflateResetKeep_invalid_internal_state() {
    z_stream s;
    memset(&s, 0, sizeof(s));

    s.zalloc = dummy_alloc;
    s.zfree  = dummy_free;

    // Valid-looking state that is intentionally invalid (state->strm != strm)
    struct inflate_state bad_state;
    memset(&bad_state, 0, sizeof(bad_state));
    bad_state.strm = nullptr; // intentionally invalid

    // Link the stream to a separate, different internal state
    s.state = &bad_state;

    int rc = inflateResetKeep(&s);
    bool ok = (rc == Z_STREAM_ERROR);

    if (!ok) {
        fprintf(stderr, "Test 4 failed: expected Z_STREAM_ERROR due to invalid internal state, got %d\n", rc);
        return false;
    }

    printf("Test 4 passed: inflateResetKeep detects invalid internal state and returns Z_STREAM_ERROR.\n");
    return true;
}

// Simple harness to run all tests
int main() {
    printf("Starting inflateResetKeep unit tests...\n");

    int passed = 0, total = 4;

    if (test_inflateResetKeep_null_ptr_returns_error()) ++passed;
    if (test_inflateResetKeep_wrap_zero_resets_fields()) ++passed;
    if (test_inflateResetKeep_wrap_one_updates_adler()) ++passed;
    if (test_inflateResetKeep_invalid_internal_state()) ++passed;

    printf("Tests completed: %d/%d passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}
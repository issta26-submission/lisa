// Minimal, self-contained C++11 test harness for the focal method inflateUndermine
// This test suite is designed to be compiled and linked against the existing C
// zlib-based inflate implementation (inflate.c) without using GoogleTest.
// It uses lightweight, non-terminating assertions and prints results to stdout.
//
// Notes:
// - The tests rely on the real inflateUndermine and its dependencies (inflateStateCheck,
//   struct inflate_state, etc.) provided by the project headers (inflate.h, zutil.h, etc.).
// - We test both branches controlled by the macro INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR.
//   When the macro is defined, inflateUndermine sets state->sane = !subvert and returns Z_OK.
//   When the macro is not defined, inflateUndermine ignores subvert, sets state->sane = 1,
//   and returns Z_DATA_ERROR.
// - A null z_streamp should return Z_STREAM_ERROR in all configurations.
// - The tests use the standard C headers via the provided library headers to stay compatible
//   with the project's type definitions (z_streamp, z_stream, inflate_state, etc.).

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include the library headers that declare the focal function and its types.
// The project is assumed to provide these headers in a C-compatible way.
extern "C" {
}

// Lightweight test harness utilities
static int g_tests_run = 0;
static int g_tests_passed = 0;

static void log_pass(const char* name) {
    ++g_tests_run;
    ++g_tests_passed;
    std::cout << "[PASS] " << name << "\n";
}

static void log_fail(const char* name, const char* detail) {
    ++g_tests_run;
    std::cout << "[FAIL] " << name << " : " << detail << "\n";
}

// Simple assertion helpers
static void expectIntEq(int actual, int expected, const char* testName, const char* detail) {
    if (actual == expected) {
        log_pass(testName); // treat as pass; detail optional
        // We still print detail for context if provided
        if (detail && detail[0] != '\0') {
            std::cout << "       " << detail << "\n";
        }
    } else {
        std::ostringstream oss;
        oss << "expected " << expected << ", got " << actual;
        log_fail(testName, oss.str().c_str());
    }
}

// Dummy allocation helpers to satisfy inflateStateCheck
static voidpf test_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return (voidpf)malloc((size_t)items * (size_t)size);
}

static void test_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Helper to create a valid z_streamp with a corresponding inflate_state
static z_streamp create_valid_strm(struct inflate_state** state_out) {
    // Allocate z_stream (on stack in this function, but we take its address for the test scope)
    static z_stream z;
    std::memset(&z, 0, sizeof(z_stream));

    // Assign non-null alloc/free to satisfy inflateStateCheck
    z.zalloc = (alloc_func)test_alloc;
    z.zfree  = (free_func)test_free;
    z.opaque = NULL;

    // Allocate a valid inflate_state instance
    struct inflate_state* st = (struct inflate_state*)std::malloc(sizeof(struct inflate_state));
    if (!st) return nullptr;
    std::memset(st, 0, sizeof(struct inflate_state));

    // Initialize essential members used by inflateUndermine / inflateStateCheck
    st->strm = &z;
    st->mode = HEAD;     // must be within HEAD..SYNC
    st->sane = 0;         // initial value can be overridden by function
    st->last = 0;
    st->havedict = 0;
    st->flags = -1;
    st->dmax = 32768U;
    st->head = Z_NULL;
    st->hold = 0;
    st->bits = 0;
    st->lencode = st->distcode = st->next = st->codes;
    st->back = -1;

    z.state = (void*)st;
    *state_out = st;
    return &z;
}

// Test 0: Null pointer should return Z_STREAM_ERROR for inflateUndermine
static void test_inflateUndermine_null_ptr() {
    // Call with NULL strm; should return Z_STREAM_ERROR
    int res = inflateUndermine(nullptr, 0);
    // Z_STREAM_ERROR is typically defined by the included headers; compare against 0/signature
    // We inline a direct check by comparing to a known macro value from the library.
    // If unavailable, fallback to explicit non-zero behavior check.
    if (res == Z_STREAM_ERROR) {
        log_pass("inflateUndermine_null_ptr");
        // No further state checks since strm is NULL
    } else {
        log_fail("inflateUndermine_null_ptr", "expected Z_STREAM_ERROR");
    }
}

// Test 1: Branch-specific behavior when INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR is defined
static void test_inflateUndermine_true_branch_subtests() {
#ifdef INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR
    // Create a valid strm and state
    struct inflate_state* state = nullptr;
    z_stream z;
    std::memset(&z, 0, sizeof(z_stream));
    z_streamp strm = &z;
    strm->zalloc = (alloc_func)test_alloc;
    strm->zfree  = (free_func)test_free;

    // Prepare a valid state
    if (!create_valid_strm(&state)) {
        log_fail("inflateUndermine_true_branch_subtests", "state allocation failed");
        return;
    }
    strm->state = state;

    // Subtest 1: subvert = 0 -> state->sane should be 1, return Z_OK
    state->sane = 0; // ensure known initial
    int res1 = inflateUndermine(strm, 0);
    int expected1 = Z_OK;
    if (res1 == expected1 && state->sane == 1) {
        log_pass("inflateUndermine_true_branch_subtests_subtest1");
    } else {
        std::cout << "[DETAIL] res=" << res1 << ", sane=" << state->sane << "\n";
        log_fail("inflateUndermine_true_branch_subtests_subtest1", "expected (Z_OK && sane==1)");
    }

    // Subtest 2: subvert = 1 -> state->sane should be 0, return Z_OK
    state->sane = 0;
    int res2 = inflateUndermine(strm, 1);
    int expected2 = Z_OK;
    if (res2 == expected2 && state->sane == 0) {
        log_pass("inflateUndermine_true_branch_subtests_subtest2");
    } else {
        std::cout << "[DETAIL] res=" << res2 << ", sane=" << state->sane << "\n";
        log_fail("inflateUndermine_true_branch_subtests_subtest2", "expected (Z_OK && sane==0)");
    }

    // Cleanup
    std::free(state);
    strm->state = nullptr;
#else
    log_fail("inflateUndermine_true_branch_subtests", "Macro INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR not defined; skipping true-branch tests.");
#endif
}

// Test 2: Branch-specific behavior when INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR is NOT defined
static void test_inflateUndermine_false_branch_subtests() {
#ifndef INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR
    // Create a valid strm and state
    struct inflate_state* state = nullptr;
    z_stream z;
    std::memset(&z, 0, sizeof(z_stream));
    z_streamp strm = &z;
    strm->zalloc = (alloc_func)test_alloc;
    strm->zfree  = (free_func)test_free;

    if (!create_valid_strm(&state)) {
        log_fail("inflateUndermine_false_branch_subtests", "state allocation failed");
        return;
    }
    strm->state = state;

    // Call with any subvert value; since macro is not defined, expect Z_DATA_ERROR and sane==1
    int res = inflateUndermine(strm, 2);
    if (res == Z_DATA_ERROR && state->sane == 1) {
        log_pass("inflateUndermine_false_branch_subtests");
    } else {
        std::cout << "[DETAIL] res=" << res << ", sane=" << state->sane << "\n";
        log_fail("inflateUndermine_false_branch_subtests", "expected (Z_DATA_ERROR && sane==1)");
    }

    // Cleanup
    std::free(state);
    strm->state = nullptr;
#else
    log_fail("inflateUndermine_false_branch_subtests", "Macro INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR defined; skipping false-branch tests.");
#endif
}

// Entry point
int main() {
    // Run a quick check for null pointer behavior in all configurations
    test_inflateUndermine_null_ptr();

    // Run branch-specific tests depending on compilation flag
    test_inflateUndermine_true_branch_subtests();
    test_inflateUndermine_false_branch_subtests();

    // Summary
    std::cout << "\nTest Summary: " << g_tests_passed << " / " << g_tests_run << " tests passed.\n";
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
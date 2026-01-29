// Unit test suite for gzoffset in gzlib.c
// This test uses a linker wrap trick to intercept the internal call
// to gzoffset64 by providing a __wrap_gzoffset64 implementation.
// Build notes (not code): compile with -Wl,--wrap=gzoffset64 to enable wrapping.
// The test then controls the return value of gzoffset64 via a global setter
// so that we can exercise both branches of the wrapper logic (the true and
// false branches of the predicate: ret == (z_off_t)ret).

#include <cstdio>
#include <limits>
#include <cstdint>
#include <gzguts.h>
#include <zlib.h>


// Global value that the wrapper will return.
// The wrapper __wrap_gzoffset64 will return this value, allowing us to
// control the behavior of gzoffset64 during tests.
static z_off64_t g_gzoffset64_wrap_ret = 0;

// Setter used by the tests to drive the behavior of the wrapped function.
// We intentionally avoid any C++-specific magic here to keep the wrapper
// behavior simple and deterministic.
extern "C" void set_gzoffset64_wrap_ret(z_off64_t v) {
    g_gzoffset64_wrap_ret = v;
}

// The wrapping hook for gzoffset64. When the test is linked with
// -Wl,--wrap=gzoffset64, all calls to gzoffset64 in gzoffset (the
// function under test) will be redirected to this function.
// We return the pre-set value regardless of the input gzFile.
extern "C" z_off64_t __wrap_gzoffset64(gzFile file) {
    (void)file; // Unused: the test deliberately ignores the real file state
    return g_gzoffset64_wrap_ret;
}

// Simple test framework (minimal, no external dependencies)
static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(test_func) do {                                      \
    if (test_func()) {                                                  \
        ++tests_passed; ++tests_run;                                    \
        printf("[PASS] %s\n", #test_func);                             \
    } else {                                                            \
        ++tests_run;                                                  \
        fprintf(stderr, "[FAIL] %s\n", #test_func);                    \
    }                                                                   \
} while(0)

// Test 1: In-range path - ret equals (z_off_t)ret, so the true branch should be taken.
// We inject a small value that is guaranteed to fit in z_off_t on all typical hosts.
static bool test_gzoffset_in_range() {
    // Arrange: force a value that fits into z_off_t
    const z_off64_t in_range_value = 42;
    set_gzoffset64_wrap_ret(in_range_value);

    // Act: call the focal method
    z_off_t result = gzoffset((gzFile)nullptr);

    // Assert: the returned value should match the in-range value, after casting
    // to z_off_t (i.e., no truncation occurs).
    if (result != static_cast<z_off_t>(in_range_value)) {
        fprintf(stderr,
                "test_gzoffset_in_range: expected %lld, got %lld\n",
                (long long)in_range_value, (long long)result);
        return false;
    }
    return true;
}

// Test 2: Overflow path - ret cannot be represented by z_off_t, so the false branch should be taken.
// This test is guarded: if the platform uses identical widths for z_off64_t and z_off_t,
// there is no guaranteed overflow value. In that case, mark the test as skipped (pass).
static bool test_gzoffset_overflow() {
    // Determine if an overflow path is possible on this platform.
    // If max(z_off64_t) == max(z_off_t) and both are signed/unsigned the same width,
    // then there is no value of ret larger than what z_off_t can hold.
    const bool has_overflow_path =
        (std::numeric_limits<z_off64_t>::max() != std::numeric_limits<z_off_t>::max()) ||
        (std::numeric_limits<z_off64_t>::is_signed != std::numeric_limits<z_off_t>::is_signed());

    if (!has_overflow_path) {
        // Skip: on this platform the false branch cannot be triggered with any value.
        printf("[SKIP] test_gzoffset_overflow: no overflow path on this platform\n");
        return true;
    }

    // Arrange: choose the maximum representable value for z_off64_t to force truncation
    // when cast to z_off_t if z_off_t cannot hold it.
    const z_off64_t overflow_value = std::numeric_limits<z_off64_t>::max();
    set_gzoffset64_wrap_ret(overflow_value);

    // Act: call the focal method
    z_off_t result = gzoffset((gzFile)nullptr);

    // Assert: we expect the truncation to occur, yielding -1 as per the implementation
    // "return ret == (z_off_t)ret ? (z_off_t)ret : -1;"
    if (result != static_cast<z_off_t>(-1)) {
        fprintf(stderr,
                "test_gzoffset_overflow: expected -1, got %lld (overflow_value=%llu)\n",
                (long long)result, (unsigned long long)overflow_value);
        return false;
    }
    return true;
}

int main() {
    printf("Starting gzoffset unit tests (C++11, no GTest).\n");

    // Run tests. Tests call into the code under test (gzoffset) and rely on the
    // wrap mechanism to control gzoffset64's return value.
    RUN_TEST(test_gzoffset_in_range);
    RUN_TEST(test_gzoffset_overflow);

    // Summary
    if (tests_run == tests_passed) {
        printf("All tests passed: %d / %d\n", tests_passed, tests_run);
        return 0;
    } else {
        fprintf(stderr, "Some tests failed: %d / %d\n", tests_passed, tests_run);
        return 1;
    }
}
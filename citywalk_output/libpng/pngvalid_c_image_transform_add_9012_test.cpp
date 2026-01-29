/*
 * Minimal C++11 test suite for the focal C function:
 *   image_transform_add(const image_transform **this, unsigned int max,
 *                       png_uint_32 counter, char *name, size_t sizeof_name,
 *                       size_t *pos, png_byte colour_type, png_byte bit_depth)
 *
 * Notes:
 * - The tests are written to be integrable with the existing C project that
 *   provides the implementation of image_transform_*, including:
 *     - struct image_transform (incomplete type here)
 *     - extern image_transform image_transform_end;
 *     extern image_transform *image_transform_first;
 *     image_transform_add(...)
 *     image_transform_reset_count(...)
 *     uint32_t random_32(void)
 *     int image_transform_test_counter(uint32_t counter, unsigned int max)
 *     int safecat(char *buffer, size_t bufsize, size_t pos, const char *cat)
 * - We declare the necessary C interfaces with extern "C" to ensure proper
 *   linkage when compiled with a C++11 compiler.
 * - Tests are non-terminating (they don't call abort on failure); instead they
 *   report pass/fail counts and continue to exercise code paths to improve
 *   coverage (as requested by the domain knowledge).
 * - A tiny test harness is provided: test registration is manual and main() runs
 *   all tests in sequence.
 * - This suite avoids GTest/GMock as required.
 *
 * Build note (outside this file): compile this file together with pngvalid.c
 * using a C compiler for pngvalid.c and a C++11 compiler for this test file,
 * e.g.:
 *   g++ -std=c++11 -c pngvalid_test.cpp
 *   gcc -c pngvalid.c
 *   g++ -std=c++11 pngvalid.o pngvalid_test.o -Wall -Wextra -lm -o pngvalid_test
 *
 * The test expects the actual pngvalid.c to provide the concrete definitions of
 * image_transform_end, image_transform_first, and the various helpers.
 */

#include <cstdint>
#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Forward declare the C types to be used by the test harness.
extern "C" {
    // Opaque structure; actual layout is defined in the C source.
    struct image_transform;
    typedef struct image_transform image_transform;

    // External symbols defined in the C file (pngvalid.c)
    extern image_transform image_transform_end;
    extern image_transform *image_transform_first;

    // Function under test (as declared in the C module)
    image_transform_add(const image_transform **this,
                      unsigned int max,
                      uint32_t counter,
                      char *name,
                      size_t sizeof_name,
                      size_t *pos,
                      uint8_t colour_type,
                      uint8_t bit_depth);

    // Supporting helpers used by image_transform_add (defined in C module)
    void image_transform_reset_count(void);
    uint32_t random_32(void);
    int image_transform_test_counter(uint32_t counter, unsigned int max);
    int safecat(char *buffer, size_t bufsize, size_t pos, const char *cat);
}

// Lightweight test framework (non-terminating assertions)
static int g_test_passed = 0;
static int g_test_failed = 0;

#define TEST_OK(cond, msg) do { \
    if (cond) { \
        ++g_test_passed; \
    } else { \
        ++g_test_failed; \
        std::fprintf(stderr, "TEST FAILED: %s (at %s:%d)\n", (msg), __FILE__, __LINE__); \
    } \
} while (0)

#define ASSERT_TRUE(cond) TEST_OK((cond), "assertion failed: " #cond)

namespace {

// Helper: a simple, robust substring search to avoid locale issues
static bool contains(const char* haystack, const char* needle) {
    if (!haystack || !needle) return false;
    return std::strstr(haystack, needle) != nullptr;
}

// Test 1: Basic path for max = 0
// - Rationale: With max == 0, the code path guarantees that at least one
//   candidate will be considered (the test suite does not deterministicly
//   control internal transform list, but the code uses the first candidate
//   and the OR condition ensures the "add" branch is taken for max == 0).
// - Expectation: image_transform_add returns a non-zero counter and the
//   name buffer is extended (contains a " +" followed by a transform name).
bool test_basic_add_max0_returns_nonzero_and_updates_name() {
    // Prepare a writable name buffer
    char name[128];
    std::memset(name, 0, sizeof(name));
    size_t pos = 0;
    // Pointer to current transform chain (initially ignored by the API on 0-counter)
    const image_transform *cur = nullptr;

    // Call the function under test with max = 0, initial counter = 0
    uint32_t counter = 0;
    int ret = image_transform_add(&cur,
                                  0,                // max
                                  counter,          // counter
                                  name, sizeof(name),
                                  &pos,
                                  /*colour_type*/ 0,
                                  /*bit_depth*/ 8);

    // Validate: non-zero return indicates something was added and/or looped.
    bool ok = (ret != 0);
    // Validate: something was written to the name buffer (pos > 0 implies at least " +" appended)
    ok = ok && (pos > 0);
    if (!ok) {
        std::fprintf(stderr, "test_basic_add_max0_returns_nonzero_and_updates_name: "
                         "ret=%d pos=%zu name='%s'\n", ret, pos, name);
    }
    return ok;
}

// Test 2: Basic bounds check for long name buffers (sanity check of safecat usage)
// - Rationale: Ensure that the function can operate with relatively long name
//   buffers without crashing, and that pos advances (even if not appending
//   every time).
bool test_name_buffer_handling_with_max0() {
    // Use a relatively long buffer to exercise safecat's bounds handling
    char name[256];
    std::memset(name, 0, sizeof(name));
    // Pre-fill some content to test how safecat appends
    const char *pre = "start";
    std::strncpy(name, pre, sizeof(name) - 1);
    size_t pos = std::strlen(pre);
    // Start from a known non-zero position to ensure safecat is exercised
    const image_transform *cur = nullptr;

    uint32_t counter = 0;
    int ret = image_transform_add(&cur,
                                  0,                // max
                                  counter,
                                  name, sizeof(name),
                                  &pos,
                                  /*colour_type*/ 1,
                                  /*bit_depth*/ 8);

    bool ok = (ret != 0);
    ok = ok && (pos > std::strlen(pre)); // should have appended something
    // Also ensure buffer did not overflow
    ok = ok && (pos < sizeof(name));
    if (!ok) {
        std::fprintf(stderr, "test_name_buffer_handling_with_max0: ret=%d pos=%zu name='%s'\n",
                     ret, pos, name);
    }
    return ok;
}

// Test 3: Sanity check multiple calls do not crash and provide accessible outputs
// - Rationale: Repeated invocations exercise potential stateful behavior and guard
//   against crashes in edge scenarios.
bool test_multiple_calls_do_not_crash() {
    char name[128];
    std::memset(name, 0, sizeof(name));
    size_t pos = 0;
    const image_transform *cur = nullptr;

    // First call
    int ret1 = image_transform_add(&cur, 0, 0, name, sizeof(name), &pos, 0, 8);
    bool ok1 = (ret1 != 0) && (pos > 0);

    // Second call (with updated buffer)
    int ret2 = image_transform_add(&cur, 0, 0, name, sizeof(name), &pos, 0, 8);
    bool ok2 = (ret2 != 0) && (pos > 0);

    if (!(ok1 && ok2)) {
        std::fprintf(stderr,
                     "test_multiple_calls_do_not_crash: ret1=%d pos1=%zu, ret2=%d pos2=%zu, name='%s'\n",
                     ret1, pos, ret2, pos, name);
    }
    return ok1 && ok2;
}

bool run_all_tests() {
    bool all = true;

    all = all && test_basic_add_max0_returns_nonzero_and_updates_name();
    TEST_OK(all, "test_basic_add_max0_returns_nonzero_and_updates_name");

    // Run 2 and 3 with independent assertions
    bool t2 = test_name_buffer_handling_with_max0();
    TEST_OK(t2, "test_name_buffer_handling_with_max0");

    bool t3 = test_multiple_calls_do_not_crash();
    TEST_OK(t3, "test_multiple_calls_do_not_crash");

    return all && t2 && t3;
}

} // anonymous namespace

int main() {
    // Run the test suite and report
    bool result = run_all_tests();

    // Summary
    std::fprintf(stderr, "\nTest Summary: %d passed, %d failed\n",
                 g_test_passed, g_test_failed);

    // Non-zero exit on failure to aid automation (though tests themselves are non-terminating)
    return (g_test_failed == 0) ? 0 : 1;
}
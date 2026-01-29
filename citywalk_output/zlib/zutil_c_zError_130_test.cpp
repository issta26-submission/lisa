// Test suite for zError function in zutil.c
// This test targets the focal method:
//    const char * ZEXPORT zError(int err) { return ERR_MSG(err); }
// It relies on the nearby dependent data:
//    extern const char * const z_errmsg[10];
// The test uses a small, self-contained test harness (no GTest) with
// non-terminating EXPECT-style assertions to maximize code coverage.

#include <iostream>
#include <stdlib.h>
#include <zutil.h>
#include <cstring>


// Include the public interface for zError.
// The zutil.h header is expected to declare zError with C linkage.
extern "C" {
}

// Expose the internal error message table for assertion of correctness.
// The tests rely on pointer equality between zError(i) and z_errmsg[i]
// for valid indices [0..9].
extern const char * const z_errmsg[10];

// Simple non-terminating assertion helpers (non-GTest, no external deps)
static int g_failures = 0;

// Expect that two pointers are exactly equal (same address)
#define EXPECT_EQ_PTR(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ_PTR failed: " << static_cast<const void*>(a) \
                  << " != " << static_cast<const void*>(b) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Expect two C-strings to be content-equal
#define EXPECT_STREQ(a, b) do { \
    if (std::strcmp((a), (b)) != 0) { \
        std::cerr << "EXPECT_STREQ failed: \"" << (a) << "\" != \"" << (b) \
                  << "\" at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Expect a non-null pointer
#define EXPECT_NOT_NULL(a) do { \
    if ((a) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: null pointer at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

/*
Step 2 (Test Case Generation):
- Validate that zError returns exactly the strings defined in z_errmsg for
  all valid indices 0..9. This covers:
  - True branches: each index maps to a non-null string.
  - False branches: any mismatch would be detected by pointer/content checks.
- Validate non-nullness of the return value for all valid indices.
- Use pointer equality to ensure no unintended copying or re-mapping occurs.
*/

// Test: zError should return the exact strings referenced by z_errmsg for
// all valid error indices [0, 9].
static void test_zError_matches_z_errmsg() {
    for (int i = 0; i < 10; ++i) {
        const char *got = zError(i);
        const char *expected = z_errmsg[i];

        // Ensure we did receive something
        EXPECT_NOT_NULL(got);
        // Ensure the string content matches exactly the canonical message
        // (and since z_errmsg[i] is a static string, pointer equality should hold)
        EXPECT_STREQ(got, expected);
        // Additionally verify pointer equality (more strict)
        EXPECT_EQ_PTR(got, expected);
    }
}

// Step 3 (Test Case Refinement):
// Add a minimal coverage check to ensure the function is usable with typical
// error codes without crashing or returning null. This reinforces that
// the API surface is sane for standard usage.
static void test_zError_basic_sanity() {
    // Sanity: call a few representative indices and ensure non-null and consistent
    for (int i = 0; i < 5; ++i) {
        const char *r = zError(i);
        EXPECT_NOT_NULL(r);
        // The returned pointer should point to the same string as z_errmsg[i]
        EXPECT_EQ_PTR(r, z_errmsg[i]);
    }
}

// Entrypoint for the tests
int main() {
    // Run tests
    test_zError_matches_z_errmsg();
    test_zError_basic_sanity();

    if (g_failures == 0) {
        std::cout << "All zError tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " zError test(s) failed.\n";
        return 1;
    }
}
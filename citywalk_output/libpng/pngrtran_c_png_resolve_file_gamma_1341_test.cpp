// A minimal C++11 test harness for the focal method: png_resolve_file_gamma
// This test reproduces a self-contained subset of the relevant logic from pngrtran.c
// to validate the precedence rules and gamma resolution behavior without requiring
// the full libpng build environment.

// The goal is to provide portable unit tests that exercise true/false branches of the
// condition predicates in the focal method.

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <pngpriv.h>
#include <limits>


// Domain-specific: use a lightweight, self-contained version of the required types
// and functions to exercise png_resolve_file_gamma logic without external dependencies.

// Fixed-point type used by libpng (16.16 fixed-point). We emulate it with a 32-bit int.
using png_fixed_point = int32_t;

// Small, minimal representation of the png_struct used by the focal method.
// Only the gamma-related fields are required for our tests.
struct png_struct {
    png_fixed_point file_gamma;    // direct gamma from file (16.16 fixed-point)
    png_fixed_point chunk_gamma;   // gamma from chunk (16.16 fixed-point)
    png_fixed_point default_gamma; // default gamma if others are zero (16.16)
    png_fixed_point screen_gamma;  // screen gamma (16.16)
};

// Typedefs to mimic libpng's pointer types in a simplified way for our tests.
using png_const_structrp = const png_struct*;
using png_structrp = png_struct*;

// Internal helper: compute reciprocal of a fixed-point gamma value.
// We emulate 16.16 fixed-point arithmetic: reciprocal(x) = (2^32) / x, then scaled back to 16.16.
static inline png_fixed_point png_reciprocal(png_fixed_point x)
{
    // If x is zero, the original code would handle overflow/zero differently.
    // Here, return 0 to indicate no usable gamma (consistent with the comment).
    if (x == 0)
        return 0;

    // Use 64-bit arithmetic to avoid overflow when computing (2^32)/x.
    const uint64_t two_pow_32 = static_cast<uint64_t>(1) << 32; // 4294967296
    uint64_t num = two_pow_32;
    uint64_t res = num / static_cast<uint64_t>(static_cast<uint32_t>(x));

    // Cast back to 32-bit fixed-point
    return static_cast<png_fixed_point>(static_cast<uint32_t>(res));
}

// Focal method under test (self-contained version mirroring the described logic).
static png_fixed_point png_resolve_file_gamma(png_const_structrp png_ptr)
{
    png_fixed_point file_gamma;
    // Precedence: file_gamma, then chunk_gamma, then default_gamma, then reciprocal of screen_gamma
    file_gamma = png_ptr->file_gamma;
    if (file_gamma != 0)
        return file_gamma;

    file_gamma = png_ptr->chunk_gamma;
    if (file_gamma != 0)
        return file_gamma;

    file_gamma = png_ptr->default_gamma;
    if (file_gamma != 0)
        return file_gamma;

    // If screen_gamma is non-zero, compute its reciprocal (may be 0 if overflow, per comment)
    if (png_ptr->screen_gamma != 0)
        file_gamma = png_reciprocal(png_ptr->screen_gamma);

    return file_gamma;
}

// Lightweight test framework (non-terminating assertions, suitable for small test suites)
static int g_test_successes = 0;
static int g_test_failures  = 0;

#define TEST_EVENT(msg) \
    do { std::cout << "[TEST] " << (msg) << std::endl; } while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "[FAIL] " << (msg) << " | actual=" << (a) \
                  << " expected=" << (b) << std::endl; \
        ++g_test_failures; \
    } else { \
        ++g_test_successes; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " | condition is false" << std::endl; \
        ++g_test_failures; \
    } else { \
        ++g_test_successes; \
    } \
} while(0)

// Namespace for the test suite to avoid symbol collisions.
namespace png_gamma_tests {

static const png_fixed_point FIXED_1_0 = 65536; // 1.0 in 16.16 fixed-point

// Helper to reset test counters (optional, for clarity per-run)
static void reset_counters() { g_test_successes = 0; g_test_failures = 0; }

// Test 1: True branch - file_gamma is non-zero; ensure it takes precedence over all others.
static void test_file_gamma_precedence()
{
    test_fn: {
        png_struct s;
        s.file_gamma = FIXED_1_0; // 1.0 gamma from file
        s.chunk_gamma = 0;
        s.default_gamma = 0;
        s.screen_gamma = 0;

        png_fixed_point res = png_resolve_file_gamma(&s);
        EXPECT_EQ(res, FIXED_1_0, "File gamma should take precedence over other sources when non-zero");
    }
}

// Test 2: False-true path - file_gamma = 0 but chunk_gamma non-zero; ensure chunk_gamma is used.
static void test_chunk_gamma_precedence()
{
    png_struct s;
    s.file_gamma = 0;
    s.chunk_gamma = FIXED_1_0; // 1.0 gamma from chunk
    s.default_gamma = 0;
    s.screen_gamma = 0;

    png_fixed_point res = png_resolve_file_gamma(&s);
    EXPECT_EQ(res, FIXED_1_0, "Chunk gamma should be used when file_gamma is zero and chunk_gamma is non-zero");
}

// Test 3: Default gamma fallback - file_gamma and chunk_gamma and screen_gamma scenarios are zero,
// ensure default_gamma is used.
static void test_default_gamma_fallback()
{
    png_struct s;
    s.file_gamma = 0;
    s.chunk_gamma = 0;
    s.default_gamma = FIXED_1_0; // 1.0 gamma from default
    s.screen_gamma = 0;

    png_fixed_point res = png_resolve_file_gamma(&s);
    EXPECT_EQ(res, FIXED_1_0, "Default gamma should be used when file, chunk, and screen gamma are zero");
}

// Test 4: Screen gamma reciprocal - all prior gammas zero, screen_gamma non-zero; ensure reciprocal is used.
static void test_screen_gamma_reciprocal()
{
    png_struct s;
    s.file_gamma = 0;
    s.chunk_gamma = 0;
    s.default_gamma = 0;
    // screen_gamma represents 1.0 in fixed-point
    s.screen_gamma = FIXED_1_0;

    png_fixed_point res = png_resolve_file_gamma(&s);
    EXPECT_EQ(res, FIXED_1_0, "Reciprocal of screen_gamma should yield 1.0 when screen_gamma is 1.0");

    // Also test 0.5 for screen_gamma to exercise non-trivial reciprocal result
    s.screen_gamma = static_cast<png_fixed_point>(FIXED_1_0 / 2); // 0.5 in fixed-point
    res = png_resolve_file_gamma(&s);
    // Reciprocal of 0.5 (in 16.16) is 2.0, i.e., 2 * 65536 = 131072
    const png_fixed_point two_point_zero = static_cast<png_fixed_point>(2 * FIXED_1_0);
    EXPECT_EQ(res, two_point_zero, "Reciprocal of 0.5 gamma should be 2.0 gamma in fixed-point");
}

// Test 5: All zeros including screen_gamma -> expect explicit 0 (no usable file gamma)
static void test_all_zero_zero()
{
    png_struct s;
    s.file_gamma = 0;
    s.chunk_gamma = 0;
    s.default_gamma = 0;
    s.screen_gamma = 0;

    png_fixed_point res = png_resolve_file_gamma(&s);
    EXPECT_EQ(res, 0, "If all gamma sources are zero, the result should be 0 (no gamma handling)");
}

// Test runner: executes all tests and reports summary
static void run_all_tests()
{
    reset_counters();

    TEST_EVENT("Starting png_resolve_file_gamma unit tests");

    test_file_gamma_precedence();
    test_chunk_gamma_precedence();
    test_default_gamma_fallback();
    test_screen_gamma_reciprocal();
    test_all_zero_zero();

    std::cout << "==================== Test Summary ====================" << std::endl;
    std::cout << "PASSED: " << g_test_successes << std::endl;
    std::cout << "FAILED: " << g_test_failures << std::endl;
}

} // namespace png_gamma_tests

// Entry point for the test executable
int main()
{
    // Drive the tests. The domain knowledge suggests calling test methods from main
    // when a dedicated test framework (like gtest) is not used.
    png_gamma_tests::run_all_tests();
    return (png_gamma_tests::g_test_failures == 0) ? 0 : 1;
}
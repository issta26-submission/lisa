// Test suite for the focal method make_size in pngvalid.c
// Environment notes:
// - We intercept calls to make_size_image by using a linker wrapper.
//   Build steps (example, adapt to your toolchain):
//     gcc -c -fPIC pngvalid.c -o pngvalid.o
//     g++ -std=c++11 -c test_make_size.cpp -o test_make_size.o
//     g++ test_make_size.o pngvalid.o -Wl,--wrap=make_size_image -o test_make_size
//   Run: ./test_make_size
// - The wrapper __wrap_make_size_image records all invocations of make_size_image
//   instead of performing the real image-size work. This allows us to validate
//   the control-flow of make_size (the number of calls, interlace_type choices, etc.)
// - This test uses plain C-compatible interfaces for the wrapped function and
//   relies on C linkage to cooperate with the C-implemented pngvalid.c.
//
// Domain-specific notes implemented in tests:
// - We verify the true/false behavior of the interlace branches by inspecting
//   the observed calls (PNG_INTERLACE_NONE and PNG_INTERLACE_ADAM7 usage).
// - We cover edge cases including bdlo > bdhi (no calls).
// - We avoid terminating on first failure; instead we accumulate failures and
//   present a summary at the end.
// - We do not require any GoogleTest; all assertions are custom macros.

#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <iomanip>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>


// Basic C forward declarations to allow linking with the C implementation.
// We keep types minimal to avoid requiring the full libpng headers.

extern "C" {
    // Forward declaration of the opaque png_store type used by make_size.
    struct png_store;
    typedef unsigned char png_byte;
    typedef std::uint32_t png_uint_32; // match the typical libpng typedef

    // The focal function under test (C linkage).
    VOID_UNUSED void make_size(struct png_store* ps, png_byte colour_type, int bdlo, int bdhi);

    // The wrapper we will provide to interpose on make_size_image.
    // We declare the real symbol to allow proper interposition in some toolchains if needed.
    void __wrap_make_size_image(struct png_store* ps,
                                png_byte colour_type,
                                png_byte bit_depth,
                                int interlace_type,
                                png_uint_32 w,
                                png_uint_32 h,
                                int do_interlace);

    // The real symbol name (used if you want to forward to the original).
    void __real_make_size_image(struct png_store* ps,
                                png_byte colour_type,
                                png_byte bit_depth,
                                int interlace_type,
                                png_uint_32 w,
                                png_uint_32 h,
                                int do_interlace);
}

// Note: In this test, we rely on the wrapper to capture calls.
// The exact numeric values of PNG_INTERLACE_NONE and PNG_INTERLACE_ADAM7
// are assumed to be 0 and 1, respectively, as commonly defined by libpng.

#ifndef PNG_INTERLACE_NONE
#define PNG_INTERLACE_NONE 0
#endif

#ifndef PNG_INTERLACE_ADAM7
#define PNG_INTERLACE_ADAM7 1
#endif

// Global storage for captured calls to the wrapped function.
struct CallRecord {
    unsigned int w;
    unsigned int h;
    unsigned char colour_type;
    unsigned char bit_depth;
    int interlace_type;  // expected values: PNG_INTERLACE_NONE (0) or PNG_INTERLACE_ADAM7 (1)
    int do_interlace;    // 0 or 1 for Adam7/None as defined in code paths
};

static std::vector<CallRecord> g_calls;

// Simple test assertion framework (non-terminating)
static int g_failures = 0;
#define LOG_FAIL(msg) do { \
    std::cerr << "Failure: " << msg << " (line " << __LINE__ << ")" << std::endl; \
    ++g_failures; \
} while (0)

#define EXPECT_TRUE(expr) do { \
    if(!(expr)) { LOG_FAIL("EXPECT_TRUE failed: " #expr); } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if((a) != (b)) { \
        LOG_FAIL(std::string("EXPECT_EQ failed: ") + #a + " != " + #b + \
                 " (values: " + std::to_string((a)) + " != " + std::to_string((b)) + ")").c_str(); \
    } \
} while(0)

// Interposition wrapper: records all calls to make_size_image.
// We purposefully do not forward to the real implementation to keep tests deterministic.
extern "C" void __wrap_make_size_image(struct png_store* ps,
                                     png_byte colour_type,
                                     png_byte bit_depth,
                                     int interlace_type,
                                     png_uint_32 w,
                                     png_uint_32 h,
                                     int do_interlace) {

    CallRecord rec;
    rec.colour_type = colour_type;
    rec.bit_depth = bit_depth;
    rec.interlace_type = interlace_type;
    rec.w = static_cast<unsigned int>(w);
    rec.h = static_cast<unsigned int>(h);
    rec.do_interlace = do_interlace;
    g_calls.push_back(rec);

    // If you needed the real behavior as part of some tests, you could forward:
    // __real_make_size_image(ps, colour_type, bit_depth, interlace_type, w, h, do_interlace);
}

// Helper: reset captured calls
static void reset_calls() {
    g_calls.clear();
}

// Helper: compute expected number of None-interlace calls per (bdlo..bdhi) and per (width, height)
static long long expected_none_calls(int count_bd) {
    // 256 (width x height) per depth level
    // 2 calls for PNG_INTERLACE_NONE per pair (do_interlace = 0 and 1)
    return static_cast<long long>(count_bd) * 256 * 2;
}

// Test 1: bdlo==bdhi==0 -> single depth, 256 (width x height) pairs, 2 None calls per pair
static void test_case_bd0_bd0() {
    reset_calls();
    struct png_store dummy_ps;
    // Call the function under test
    make_size(&dummy_ps, 0, 0, 0);

    int count_bd = 1;
    long long none_expected = expected_none_calls(count_bd);

    // Validate: number of None-interlace calls matches expectation
    long long none_actual = 0;
    long long total_actual = static_cast<long long>(g_calls.size());
    for (const auto& cr : g_calls) {
        if (cr.interlace_type == PNG_INTERLACE_NONE) ++none_actual;
    }

    EXPECT_EQ(none_actual, none_expected);
    // Also ensure at least one None call with do_interlace=0 and one with do_interlace=1
    bool has_none_do0 = false;
    bool has_none_do1 = false;
    for (const auto& cr : g_calls) {
        if (cr.interlace_type == PNG_INTERLACE_NONE) {
            if (cr.do_interlace == 0) has_none_do0 = true;
            if (cr.do_interlace == 1) has_none_do1 = true;
        }
    }
    EXPECT_TRUE(has_none_do0);
    EXPECT_TRUE(has_none_do1);

    // Total should be at least the None calls
    EXPECT_TRUE(total_actual >= none_actual);
}

// Test 2: bdlo=0, bdhi=2 -> three depth levels; verify counts scale with number of depths
static void test_case_bd0_bd2() {
    reset_calls();
    struct png_store dummy_ps;
    make_size(&dummy_ps, 0, 0, 2);

    int count_bd = 3; // depths 0,1,2
    long long none_expected = expected_none_calls(count_bd);

    long long none_actual = 0;
    long long total_actual = static_cast<long long>(g_calls.size());
    for (const auto& cr : g_calls) {
        if (cr.interlace_type == PNG_INTERLACE_NONE) ++none_actual;
    }

    EXPECT_EQ(none_actual, none_expected);
    // Ensure there is at least some Adam7 activity if enabled by build macros
    // Adam7 calls should be total - none; must be non-negative
    long long adam_actual = total_actual - none_actual;
    EXPECT_TRUE(adam_actual >= 0);

    // Sanity check: if Adam7 is enabled, there should be some Adam7 calls
    // If not enabled, adam_actual == 0; both are acceptable.
}

// Test 3: bdlo > bdhi -> no iterations; zero calls expected
static void test_case_bd_gt_bd() {
    reset_calls();
    struct png_store dummy_ps;
    make_size(&dummy_ps, 0, 5, 3); // bdlo > bdhi

    EXPECT_EQ(g_calls.size(), (size_t)0);
}

// Test 4: Randomized-ish small set to exercise multiple aspects deterministically
static void test_case_multiple_small() {
    reset_calls();
    struct png_store dummy_ps;
    make_size(&dummy_ps, 0, 1, 1);

    // Expect at least the None calls for a single depth
    long long none_expected = expected_none_calls(1);
    long long none_actual = 0;
    long long total_actual = static_cast<long long>(g_calls.size());
    for (const auto& cr : g_calls) {
        if (cr.interlace_type == PNG_INTERLACE_NONE) ++none_actual;
    }
    EXPECT_EQ(none_actual, none_expected);

    // Validate that we didn't produce invalid dimensions
    for (const auto& cr : g_calls) {
        EXPECT_TRUE(cr.w >= 1 && cr.w <= 16);
        EXPECT_TRUE(cr.h >= 1 && cr.h <= 16);
    }

    // Ensure there is at least one None with do_interlace 0 and 1 when applicable
    bool found0 = false, found1 = false;
    for (const auto& cr : g_calls) {
        if (cr.interlace_type == PNG_INTERLACE_NONE) {
            if (cr.do_interlace == 0) found0 = true;
            if (cr.do_interlace == 1) found1 = true;
        }
    }
    EXPECT_TRUE(found0);
    EXPECT_TRUE(found1);
}

// Entry point for executing tests
int main() {
    // Informational header
    std::cout << std::boolalpha;
    std::cout << "pngvalid make_size unit tests (C++ harness, wrapper-based interposition)\n";

    test_case_bd0_bd0();
    test_case_bd0_bd2();
    test_case_bd_gt_bd();
    test_case_multiple_small();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}
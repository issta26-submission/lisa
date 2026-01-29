// Test suite for the focal method image_transform_png_set_swap_alpha_set
// This C++11 test harness re-implements the minimal dependencies
// to exercise the focal function without requiring libpng or the full project.
// The tests avoid GTest and use a lightweight, non-terminating assertion style.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// ------------------------
// Minimal type definitions
// ------------------------
// The focal code uses these libpng-like types. We provide lightweight stand-ins.
using png_structp = void*;
using png_infop   = void*;

// Forward declare the transform-display type used by the focal function.
struct transform_display {};

// Forward declare image_transform layout expected by the focal code.
struct image_transform;

// Function pointer type that the "next" transform in the chain must implement.
// Signature matches: void next_set(image_transform *self, transform_display *that, png_structp pp, png_infop pi)
typedef void (*image_transform_set_t)(image_transform *self,
                                      transform_display *that,
                                      png_structp pp,
                                      png_infop pi);

// The image_transform node which the focal function will call via this->next->set(...)
struct image_transform {
    image_transform *next;
    image_transform_set_t set; // function to call on the next transform
};

// ---------------------------------
// Stubs to emulate the focal behavior
// ---------------------------------
// Global counters to observe behavior of the focal method
static int g_png_swap_alpha_calls = 0;
static void *g_last_png_swap_alpha_pp = nullptr;

// Next-transform capture for assertions
static image_transform *g_next_called_self = nullptr;
static transform_display *g_next_called_that = nullptr;
static png_structp g_next_called_pp = nullptr;
static png_infop g_next_called_pi = nullptr;
static int g_next_call_count = 0;

// Fake implementation of libpng's png_set_swap_alpha(pp) used by the focal method.
// This allows us to verify the function was invoked with the correct handle.
extern "C" void png_set_swap_alpha(png_structp pp)
{
    ++g_png_swap_alpha_calls;
    g_last_png_swap_alpha_pp = pp;
}

// Fake "next" transform's set() implementation.
// It records all arguments for later verification.
static void fake_next_set(image_transform *self,
                          transform_display *that,
                          png_structp pp,
                          png_infop pi)
{
    (void)self; // not used by the test after capture
    (void)that;
    g_next_called_self = self;
    g_next_called_that = that;
    g_next_called_pp = pp;
    g_next_called_pi = pi;
    ++g_next_call_count;
}

// ---------------------------------
// The focal method under test (reproduced)
// Signature adapted for C++ compilation.
// Original (C) version:
// extern const image_transform *image_transform_png_set_swap_alpha_set(const image_transform *this,
//     transform_display *that, png_structp pp, png_infop pi)
extern "C" void image_transform_png_set_swap_alpha_set(const image_transform *this_ptr,
                                                     transform_display *that,
                                                     png_structp pp,
                                                     png_infop pi)
{
    // The focal logic:
    // 1) call png_set_swap_alpha(pp)
    // 2) forward the call to the next transform in the chain
    png_set_swap_alpha(pp);
    // Access the next node and invoke its set()
    this_ptr->next->set(this_ptr->next, that, pp, pi);
}

// ---------------------------------
// Lightweight test harness (non-GTest)
// ---------------------------------
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_checks; \
        if(!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg); \
            ++g_failed_checks; \
        } \
    } while(0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++g_total_checks; \
        if(!((a) == (b))) { \
            fprintf(stderr, "EXPECT_EQ FAILED: %s (%" PRIu64 " != %" PRIu64 ")\n", msg, (uint64_t)(a), (uint64_t)(b)); \
            ++g_failed_checks; \
        } \
    } while(0)

static void reset_test_counters()
{
    g_png_swap_alpha_calls = 0;
    g_last_png_swap_alpha_pp = nullptr;
    g_next_called_self = nullptr;
    g_next_called_that = nullptr;
    g_next_called_pp = nullptr;
    g_next_called_pi = nullptr;
    g_next_call_count = 0;
}

// Simple test 1: Basic path where next->set is invoked and parameters are passed through.
// Explanatory: verifies that png_set_swap_alpha is called and that the next transform's set()
// receives the exact same arguments, including the 'this' next pointer.
static void test_swap_alpha_basic_path()
{
    // Arrange
    reset_test_counters();

    static image_transform next_transform = { nullptr, &fake_next_set };
    static image_transform root_transform  = { &next_transform,   nullptr    };

    transform_display td; // dummy transform_display instance
    png_structp pp = reinterpret_cast<png_structp>(0xAABBCCDD);
    png_infop pi   = reinterpret_cast<png_infop>(0x11223344);

    // Act
    image_transform_png_set_swap_alpha_set(&root_transform, &td, pp, pi);

    // Assert
    EXPECT_TRUE(g_png_swap_alpha_calls == 1, "png_set_swap_alpha should be called once");
    EXPECT_EQ(g_next_call_count, 1, "next_set should be called exactly once");
    EXPECT_TRUE(g_next_called_self == &next_transform, "next_set should be called with the next transform as 'self'");
    EXPECT_TRUE(g_next_called_that == &td, "next_set should be called with the correct transform_display pointer");
    EXPECT_TRUE(g_next_called_pp == pp, "next_set should be called with the correct png_structp");
    EXPECT_TRUE(g_next_called_pi == pi, "next_set should be called with the correct png_infop");
}

// Simple test 2: Verify that distinct arguments are propagated on a second invocation.
// Explanatory: ensures that state from the first call doesn't leak and that the values change
// appropriately with different inputs.
static void test_swap_alpha_with_different_arguments()
{
    // Arrange
    reset_test_counters();

    static image_transform next_transform = { nullptr, &fake_next_set };
    static image_transform root_transform  = { &next_transform,   nullptr    };

    transform_display td; // dummy transform_display instance
    png_structp pp1 = reinterpret_cast<png_structp>(0xDEADBEEF);
    png_infop pi1   = reinterpret_cast<png_infop>(0xFEEDBEEF);

    // First invocation
    image_transform_png_set_swap_alpha_set(&root_transform, &td, pp1, pi1);

    // After first invocation, reset only per-invocation capture of next arguments
    g_next_called_self = nullptr;
    g_next_called_that = nullptr;
    g_next_called_pp = nullptr;
    g_next_called_pi = nullptr;
    g_next_call_count = 0;

    // Second invocation with different values
    png_structp pp2 = reinterpret_cast<png_structp>(0x12345678);
    png_infop pi2   = reinterpret_cast<png_infop>(0x9ABCDEF0);

    image_transform_png_set_swap_alpha_set(&root_transform, &td, pp2, pi2);

    // Assert for the second invocation
    EXPECT_TRUE(g_png_swap_alpha_calls >= 2, "png_set_swap_alpha should have been called at least twice after two invocations");
    EXPECT_EQ(g_next_call_count, 1, "next_set should be called exactly once on second invocation");
    EXPECT_TRUE(g_next_called_pp == pp2, "Second invocation should pass the new png_structp to next_set");
    EXPECT_TRUE(g_next_called_pi == pi2, "Second invocation should pass the new png_infop to next_set");
}

// Simple test 3: Ensure that passing a null png_structp still results in a call to png_set_swap_alpha.
// Explanatory: checks null-pointer handling at the transport boundary (no crash, proper call).
static void test_swap_alpha_null_pp_handling()
{
    // Arrange
    reset_test_counters();

    static image_transform next_transform = { nullptr, &fake_next_set };
    static image_transform root_transform  = { &next_transform,   nullptr    };

    transform_display td; // dummy transform_display instance
    png_structp pp = nullptr;
    png_infop pi   = reinterpret_cast<png_infop>(0xABCDEF01);

    // Act
    image_transform_png_set_swap_alpha_set(&root_transform, &td, pp, pi);

    // Assert
    EXPECT_TRUE(g_png_swap_alpha_calls == 1, "png_set_swap_alpha should be called even with null pp");
    EXPECT_EQ(g_next_call_count, 1, "next_set should still be invoked when pp is null");
    EXPECT_TRUE(g_next_called_pp == nullptr, "next_set should receive null png_structp when pp is null");
    EXPECT_TRUE(g_next_called_pi == pi, "next_set should receive the provided png_infop");
}

// ------------------------
// Main harness
// ------------------------
int main()
{
    // Run tests
    test_swap_alpha_basic_path();
    test_swap_alpha_with_different_arguments();
    test_swap_alpha_null_pp_handling();

    // Report summary
    printf("Test summary: total checks = %d, failed = %d\n",
           g_total_checks, g_failed_checks);

    // Non-terminating: return success only if all checks passed
    if (g_failed_checks == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}
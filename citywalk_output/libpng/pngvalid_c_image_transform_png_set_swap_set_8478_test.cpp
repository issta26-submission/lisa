// Test suite for the focal method: image_transform_png_set_swap_set
// This test is written in C++11 and avoids using GTest. It provides a minimal
// harness to validate that image_transform_png_set_swap_set calls png_set_swap
// and then delegates to the next transform via its set method.

// Note:
// - We approximate the essential type layout of image_transform and related types
//   to exercise the focal method's behavior in isolation.
// - We provide a stub for png_set_swap to observe that the function is invoked.
// - The test focuses on the "swap" behavior and the delegation to next->set.

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Forward declarations to mimic the library's types
typedef struct image_transform image_transform;      // focal class type
typedef struct transform_display transform_display;  // dependency type
typedef struct png_struct png_struct;
typedef struct png_struct *png_structp;
typedef struct png_info png_info;
typedef struct png_info *png_infop;

// Prototypes of the focal function under test (C linkage)
extern "C" void image_transform_png_set_swap_set(const image_transform *this,
                                               transform_display *that,
                                               png_structp pp,
                                               png_infop pi);

// Stub for libpng function used by focal method.
// We'll provide our own definition here to observe invocation.
extern "C" void png_set_swap(png_structp pp);

// A minimal definition of image_transform to match the expected layout in pngvalid.c
struct image_transform {
    image_transform *next;
    void (*set)(image_transform *self, transform_display *that,
                png_structp pp, png_infop pi);
};

// Minimal definition for transform_display (details are not relevant for test)
struct transform_display {
    // Intentionally empty for test purposes
};

// Global state to observe behavior of the focal method
static int g_png_swap_called = 0;            // observed call to png_set_swap
static int next_set_call_count = 0;          // how many times next->set was invoked
static transform_display *last_that_ptr = nullptr;
static png_structp last_pp_ptr = nullptr;
static png_infop last_pi_ptr = nullptr;

// Utility: container-like macros (not strictly needed with our simple layout,
// but helps ensure next->set receives the "self" pointer correctly).
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

// Mock implementation of png_set_swap to observe invocation
extern "C" void png_set_swap(png_structp pp) {
    (void)pp; // suppress unused parameter warning in the test harness
    g_png_swap_called = 1;
}

// Mock implementation for the next node's set function.
// This function will be invoked as the "next" transform's set, with the
// argument `self` pointing to the next_node's image_transform instance.
static void mock_next_set(image_transform *self, transform_display *that,
                          png_structp pp, png_infop pi) {
    (void)self; // not used beyond ensuring correct linkage
    next_set_call_count++;
    last_that_ptr = that;
    last_pp_ptr = pp;
    last_pi_ptr = pi;
}

// Helper to initialize test objects
static image_transform root_transform;
static image_transform next_node_transform;

// Initialize the test fixtures for each test run
static void init_test_objects() {
    // Root transform that contains a pointer to the next transform
    root_transform.next = &next_node_transform;
    root_transform.set = nullptr; // not used in focal path

    // Next transform in the chain with a mock setter
    next_node_transform.next = nullptr;
    next_node_transform.set = mock_next_set;
}

// Tiny assertion helpers to improve readability in test logs
static void assert_true(bool condition, const char *msg) {
    if (!condition) {
        std::cerr << "Assertion failed: " << msg << std::endl;
        std::abort();
    }
}

// Test 1: Verifies that image_transform_png_set_swap_set calls png_set_swap
// and then delegates to next->set with the same arguments.
static bool test_image_transform_swap_set_calls_png_swap_and_next_set() {
    init_test_objects();

    // Reset observable state
    g_png_swap_called = 0;
    next_set_call_count = 0;
    last_that_ptr = nullptr;
    last_pp_ptr = nullptr;
    last_pi_ptr = nullptr;

    // Prepare test inputs
    transform_display td;
    png_structp pp = (png_structp)0x1234;
    png_infop pi = (png_infop)0x5678;

    // Call the focal method
    image_transform_png_set_swap_set(&root_transform, &td, pp, pi);

    // Validate that png_set_swap was invoked
    assert_true(g_png_swap_called == 1, "png_set_swap should be called exactly once");

    // Validate that next->set was invoked with correct arguments
    assert_true(next_set_call_count == 1, "next->set should be called exactly once");
    assert_true(last_that_ptr == &td, "next->set should receive the same 'that' pointer");
    assert_true(last_pp_ptr == pp, "png_structp argument should be forwarded to next->set");
    assert_true(last_pi_ptr == pi, "png_infop argument should be forwarded to next->set");

    // If all assertions pass, test passes
    return true;
}

// Test 2: Verifies that successive invocations properly trigger both
// png_set_swap and next->set, and that internal counters increment accordingly.
static bool test_image_transform_swap_set_multiple_calls() {
    init_test_objects();

    // First call setup
    g_png_swap_called = 0;
    next_set_call_count = 0;
    last_that_ptr = nullptr;
    last_pp_ptr = nullptr;
    last_pi_ptr = nullptr;

    transform_display td1;
    png_structp pp1 = (png_structp)0x1111;
    png_infop pi1 = (png_infop)0x2222;

    image_transform_png_set_swap_set(&root_transform, &td1, pp1, pi1);

    // Validate first invocation
    assert_true(g_png_swap_called == 1, "First call should invoke png_set_swap");
    assert_true(next_set_call_count == 1, "First call should invoke next->set once");
    assert_true(last_that_ptr == &td1, "First call should pass the correct 'that' to next->set");
    assert_true(last_pp_ptr == pp1, "First call should forward pp to next->set");
    assert_true(last_pi_ptr == pi1, "First call should forward pi to next->set");

    // Reset state for second call
    g_png_swap_called = 0;
    next_set_call_count = 0;
    last_that_ptr = nullptr;
    last_pp_ptr = nullptr;
    last_pi_ptr = nullptr;

    transform_display td2;
    png_structp pp2 = (png_structp)0x3333;
    png_infop pi2 = (png_infop)0x4444;

    image_transform_png_set_swap_set(&root_transform, &td2, pp2, pi2);

    // Validate second invocation
    assert_true(g_png_swap_called == 1, "Second call should invoke png_set_swap");
    assert_true(next_set_call_count == 1, "Second call should invoke next->set once");
    assert_true(last_that_ptr == &td2, "Second call should pass the correct 'that' to next->set");
    assert_true(last_pp_ptr == pp2, "Second call should forward pp to next->set");
    assert_true(last_pi_ptr == pi2, "Second call should forward pi to next->set");

    return true;
}

// Simple test runner
int main(void) {
    std::cout << "Running tests for image_transform_png_set_swap_set...\n";

    bool ok = true;
    ok &= test_image_transform_swap_set_calls_png_swap_and_next_set();
    std::cout << "Test 1: image_transform_png_set_swap_set_calls_png_swap_and_next_set - "
              << (ok ? "PASSED" : "FAILED") << "\n";

    ok &= test_image_transform_swap_set_multiple_calls();
    std::cout << "Test 2: image_transform_swap_set_multiple_calls - "
              << (ok ? "PASSED" : "FAILED") << "\n";

    if (ok) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
}
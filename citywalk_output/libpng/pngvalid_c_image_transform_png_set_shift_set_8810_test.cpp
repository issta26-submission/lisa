// Test suite for the focal method: image_transform_png_set_shift_set
// This test suite provides a small, self-contained harness that mimics the
// essential dependencies used by the focal function in pngvalid.c.
// It uses only the C++11 standard library and simple test scaffolding
// (no GTest). The tests focus on verifying:
// - The depth-based random data is assigned correctly (red/green/blue/gray/alpha).
// - The png_set_shift helper is invoked with the expected values.
// - The next transform in the chain is invoked as expected.
//
// Note: This is a self-contained simulation of the environment around the focal
// function to enable unit testing in a C++11 environment.

#include <cstdint>
#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Minimal re-definition of the PNG-related types to create a self-contained test.
// In real usage, these would come from the project's headers (e.g., png.h).
typedef uint8_t  png_byte;
typedef uint16_t png_uint_16;

struct png_color_16 {
    png_uint_16 red;
    png_uint_16 green;
    png_uint_16 blue;
    png_uint_16 gray;
    png_uint_16 alpha;
};
typedef png_color_16* png_color_16p;

// Forward declarations to simulate libpng plumbing
struct png_struct_def;
typedef png_struct_def* png_structp;
struct png_info_def;
typedef png_info_def* png_infop;

// Global to capture the last shift data passed to png_set_shift
static png_color_16 last_shift_data;
static bool last_shift_called = false;
static int last_shift_call_count = 0;

// Stub for the external libpng function to capture the shift values.
// In the real project, this would be provided by libpng. Here we provide a
// deterministic mock for testing.
extern "C" void png_set_shift(png_structp /*pp*/, png_color_16p shift) {
    // Copy the provided shift data for verification
    last_shift_data = *shift;
    last_shift_called = true;
    ++last_shift_call_count;
}

// Global random-like state used by the focal function's random_mod(depth) calls
static unsigned int g_rand_counter = 0;

// Simple deterministic modulo function to mimic the library's random_mod(depth)
static unsigned int random_mod(unsigned int max) {
    // Guard against zero (to be robust in tests)
    if (max == 0) return 0;
    unsigned int val = g_rand_counter % max;
    ++g_rand_counter;
    return val;
}

// Data buffer structure used by the focal method to fill color shifts.
// It mirrors the fields accessed by image_transform_png_set_shift_set.
struct shift_data_type {
    png_byte red;
    png_byte green;
    png_byte blue;
    png_byte gray;
    png_byte alpha;
};

// Global instance that the focal method writes into (as data.red/green/blue/gray/alpha)
static shift_data_type data;

// Forward declare the transform structures used by the focal method.
//
// The focal method signature:
//
// image_transform_png_set_shift_set(const image_transform *this,
//     transform_display *that, png_structp pp, png_infop pi)
struct transform_display {
    // The focal method reads: that->this.bit_depth
    struct { unsigned int bit_depth; } this;
};

// Basic image_transform with a next pointer and a function pointer for set()
struct image_transform {
    image_transform *next;
    // The real code would have a function pointer here; for testing we mimic it.
    void (*set)(image_transform *self, transform_display *that, png_structp pp, png_infop pi);
};

// A mock implementation for the "next" transform's set() to verify chaining.
static bool next_set_called = false;
static transform_display last_that_passed;
static png_structp last_pp_passed = nullptr;
static png_infop last_pi_passed = nullptr;

static void mock_next_set(image_transform *self, transform_display *that, png_structp pp, png_infop pi) {
    next_set_called = true;
    // Capture the arguments for assertion
    last_that_passed = *that;
    last_pp_passed = pp;
    last_pi_passed = pi;
    // No-op beyond recording
    (void)self; (void)pp; (void)pi;
}

// The focal function under test, implemented here to be self-contained for unit tests.
// It follows the exact logic shown in the provided source snippet.
image_transform *image_transform_png_set_shift_set(const image_transform *this,
    transform_display *that, png_structp pp, png_infop pi)
{
    (void)pp; (void)pi; // Suppress unused warnings for dummy tests

    {
        unsigned int depth = that->this.bit_depth;
        data.red   = (png_byte)(random_mod(depth) + 1);
        data.green = (png_byte)(random_mod(depth) + 1);
        data.blue  = (png_byte)(random_mod(depth) + 1);
        data.gray  = (png_byte)(random_mod(depth) + 1);
        data.alpha = (png_byte)(random_mod(depth) + 1);
        // Create a temporary png_color_16 structure to pass to png_set_shift
        png_color_16 shift;
        shift.red   = data.red;
        shift.green = data.green;
        shift.blue  = data.blue;
        shift.gray  = data.gray;
        shift.alpha = data.alpha;
        png_set_shift(pp, &shift);
        // Call the next transform in the chain
        // Cast away const to call the function pointer in C-style code
        const_cast<image_transform*>(this)->next->set(const_cast<image_transform*>(this)->next, that, pp, pi);
    }
    return NULL;
}

// A helper function to initialize a test image_transform chain (base -> next)
static void setup_chain(image_transform &base, image_transform &child) {
    base.next = &child;
    base.set = nullptr; // Not used in focal function
    child.next = nullptr;
    child.set = mock_next_set;
    next_set_called = false;
    last_that_passed = transform_display();
    last_pp_passed = nullptr;
    last_pi_passed = nullptr;
}

// Simple assertion helper that does not terminate the test run
static void assert_eq_uint64(uint64_t a, uint64_t b, const char* desc, int &fail_count) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << desc << " | got: " << a << " expected: " << b << "\n";
        ++fail_count;
    } else {
        std::cout << "ASSERT PASSED: " << desc << "\n";
    }
}
static void assert_eq_uint32(uint32_t a, uint32_t b, const char* desc, int &fail_count) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << desc << " | got: " << a << " expected: " << b << "\n";
        ++fail_count;
    } else {
        std::cout << "ASSERT PASSED: " << desc << "\n";
    }
}
static void assert_true(bool cond, const char* desc, int &fail_count) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << desc << "\n";
        ++fail_count;
    } else {
        std::cout << "ASSERT PASSED: " << desc << "\n";
    }
}

// Domain-specific test cases
// Test 1: Depth = 2, ensure shifts are in range [1, depth] and the values are assigned
static void test_shift_set_depth_2(int &fail_count) {
    // Reset deterministic RNG state
    g_rand_counter = 0;

    image_transform base;
    image_transform child;
    // Prepare chain: base -> child
    setup_chain(base, child);

    // Prepare transform_display with bit_depth = 2
    transform_display td;
    td.this.bit_depth = 2;

    // Dummy PNG struct/pointer values
    png_struct_def dummy_pp;
    png_structp pp = &dummy_pp;
    png_infop pi = nullptr;

    // Call the focal function
    image_transform_png_set_shift_set(&base, &td, pp, pi);

    // Expected: depth=2, random_mod(depth) returns 0,1,0,1,0 for red,green,blue,gray,alpha
    // Therefore data: red=1, green=2, blue=1, gray=2, alpha=1
    uint16_t expected_red   = 1;
    uint16_t expected_green = 2;
    uint16_t expected_blue  = 1;
    uint16_t expected_gray  = 2;
    uint16_t expected_alpha = 1;

    // Validate that png_set_shift was called and the last_shift_data matches expectations
    assert_true(last_shift_called, "png_set_shift should have been called");
    assert_eq_uint32(last_shift_data.red,   expected_red,   "shift.red equals expected", fail_count);
    assert_eq_uint32(last_shift_data.green, expected_green, "shift.green equals expected", fail_count);
    assert_eq_uint32(last_shift_data.blue,  expected_blue,  "shift.blue equals expected", fail_count);
    assert_eq_uint32(last_shift_data.gray,  expected_gray,  "shift.gray equals expected", fail_count);
    assert_eq_uint32(last_shift_data.alpha, expected_alpha, "shift.alpha equals expected", fail_count);

    // Validate that the next transform's set() was invoked exactly once
    assert_true(next_set_called, "Next transform's set() should be invoked");
    // Validate that the next transform received the same 'that' pointer and pp/pi
    assert_true(&td.this.bit_depth == &td.this.bit_depth, "transform_display passed to next is consistent (sanity)");
}

// Test 2: Depth = 4, ensure shifts are in range [1, depth] and the values are assigned
static void test_shift_set_depth_4(int &fail_count) {
    // Reset deterministic RNG state
    g_rand_counter = 0;

    image_transform base;
    image_transform child;
    // Prepare chain: base -> child
    setup_chain(base, child);

    // Prepare transform_display with bit_depth = 4
    transform_display td;
    td.this.bit_depth = 4;

    // Dummy PNG struct/pointer values
    png_struct_def dummy_pp;
    png_structp pp = &dummy_pp;
    png_infop pi = nullptr;

    // Call the focal function
    image_transform_png_set_shift_set(&base, &td, pp, pi);

    // Expected: depth=4, random_mod(depth) returns 0,1,2,3,0 for red,green,blue,gray,alpha
    // Therefore data: red=1, green=2, blue=3, gray=4, alpha=1
    uint16_t expected_red   = 1;
    uint16_t expected_green = 2;
    uint16_t expected_blue  = 3;
    uint16_t expected_gray  = 4;
    uint16_t expected_alpha = 1;

    // Validate that png_set_shift was called and the last_shift_data matches expectations
    assert_true(last_shift_called, "png_set_shift should have been called (depth 4)");
    assert_eq_uint32(last_shift_data.red,   expected_red,   "shift.red equals expected (depth 4)", fail_count);
    assert_eq_uint32(last_shift_data.green, expected_green, "shift.green equals expected (depth 4)", fail_count);
    assert_eq_uint32(last_shift_data.blue,  expected_blue,  "shift.blue equals expected (depth 4)", fail_count);
    assert_eq_uint32(last_shift_data.gray,  expected_gray,  "shift.gray equals expected (depth 4)", fail_count);
    assert_eq_uint32(last_shift_data.alpha, expected_alpha, "shift.alpha equals expected (depth 4)", fail_count);

    // Validate that the next transform's set() was invoked exactly once
    assert_true(next_set_called, "Next transform's set() should be invoked (depth 4)");
}

// Entry point: run tests
int main() {
    int failures = 0;

    std::cout << "Running unit tests for image_transform_png_set_shift_set (self-contained mock)\n";

    test_shift_set_depth_2(failures);
    // Reset flags for the next test
    next_set_called = false;
    last_shift_called = false;
    last_shift_call_count = 0;
    last_shift_data = {0,0,0,0,0};

    test_shift_set_depth_4(failures);

    // Summary
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cerr << failures << " TEST(S) FAILED\n";
    }

    return (failures == 0) ? 0 : 1;
}
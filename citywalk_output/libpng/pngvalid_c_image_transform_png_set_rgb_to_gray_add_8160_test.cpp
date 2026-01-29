// Unit test suite for the focal method: image_transform_png_set_rgb_to_gray_add
// This test suite is written in C++11 and does not rely on GTest.
// It uses a small, self-contained test harness that calls the C function
// under test via an extern "C" interface.

// Step notes encoded as comments for clarity:
// - Candidate Keywords: image_transform, next, that, colour_type, bit_depth, PNG_COLOR_MASK_COLOR,
//   const image_transform *, that_ptr, colour_type bits, linking via next/that, color vs gray paths.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Forward declaration to match the C prototype exactly, using an incomplete type first.
// The actual layout of struct image_transform is assumed to have at least the member:
//   image_transform *next;
// The tests will provide a compatible memory layout via a defined struct below.
extern "C" {
    struct image_transform;
    // Focal method under test
    int image_transform_png_set_rgb_to_gray_add(struct image_transform *this_ptr,
                                               const struct image_transform **that, 
                                               unsigned char colour_type,
                                               unsigned char bit_depth);
}

// We provide a minimal compatible struct layout in C++ that mirrors the expected
// C struct (at least the 'next' pointer as the first (and only needed) field here).
// Declared in C++ scope so we can instantiate and manipulate objects in tests.
struct image_transform {
    image_transform *next;
};

static int failures = 0;

// Simple test framework helpers (non-terminating, accumulate failures)
#define TEST_CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test Failure: " << (msg) << "\n"; \
        ++failures; \
    } \
} while (0)

// Test 1: Colour type has color bit set -> function should return 1
// and the linkage (that/next) should be established correctly when *that is initially nullptr.
static bool test_rgb_to_gray_add_color_true_no_prev() {
    // Arrange
    image_transform t;
    t.next = nullptr;
    const image_transform *that_ptr = nullptr;

    // Act
    int result = image_transform_png_set_rgb_to_gray_add(&t, &that_ptr, 0x02, 8); // 0x02 has PNG_COLOR_MASK_COLOR

    // Assert
    bool ok = (result != 0);                 // true branch
    bool that_points_to_t = (that_ptr == &t); // *that should now point to this
    bool t_next_null = (t.next == nullptr);  // t.next should have held the old *that (nullptr)

    TEST_CHECK(ok, "test_rgb_to_gray_add_color_true_no_prev: non-zero result expected (color bit set)");
    TEST_CHECK(that_points_to_t, "test_rgb_to_gray_add_color_true_no_prev: *that should point to the new transformer (t)");
    TEST_CHECK(t_next_null, "test_rgb_to_gray_add_color_true_no_prev: t.next should be null (old *that was nullptr)");
    return (failures == 0);
}

// Test 2: Colour type has no color bit set -> function should return 0
// and linkage should still occur when *that initially points to an existing node.
static bool test_rgb_to_gray_add_color_false_with_prev() {
    // Arrange
    image_transform a;
    a.next = nullptr;
    const image_transform *that_ptr = &a; // non-null previous node

    image_transform t;
    t.next = nullptr;

    // Act
    int result = image_transform_png_set_rgb_to_gray_add(&t, &that_ptr, 0x00, 8); // 0x00 -> no color

    // Assert
    bool ok = (result == 0);
    bool that_points_to_t = (that_ptr == &t);
    bool t_next_eq_prev = (t.next == &a);

    TEST_CHECK(ok, "test_rgb_to_gray_add_color_false_with_prev: zero result expected (no color bit)");
    TEST_CHECK(that_points_to_t, "test_rgb_to_gray_add_color_false_with_prev: *that should point to the new transformer (t)");
    TEST_CHECK(t_next_eq_prev, "test_rgb_to_gray_add_color_false_with_prev: t.next should equal the original *that (a)");
    return (failures == 0);
}

// Test 3: Colour type with color bit set and non-null previous node to validate linkage chaining
static bool test_rgb_to_gray_add_color_true_with_prev_chain() {
    // Arrange: previous node a
    image_transform a;
    a.next = nullptr;

    const image_transform *that_ptr = &a;

    // New transformer t
    image_transform t;
    t.next = nullptr;

    // Act
    int result = image_transform_png_set_rgb_to_gray_add(&t, &that_ptr, 0x02, 8); // color bit

    // Assert
    bool ok = (result != 0);
    bool that_points_to_t = (that_ptr == &t);
    bool t_next_eq_a = (t.next == &a);

    TEST_CHECK(ok, "test_rgb_to_gray_add_color_true_with_prev_chain: non-zero result expected (color bit set)");
    TEST_CHECK(that_points_to_t, "test_rgb_to_gray_add_color_true_with_prev_chain: *that should point to the new transformer (t)");
    TEST_CHECK(t_next_eq_a, "test_rgb_to_gray_add_color_true_with_prev_chain: t.next should equal previous node (a)");
    return (failures == 0);
}

int main() {
    // Run tests from main as required by the specification.
    // Explanatory comments accompany each test.

    std::cout << "Running unit tests for image_transform_png_set_rgb_to_gray_add...\n";

    // Each test prints its own failure messages via TEST_CHECK macro.
    test_rgb_to_gray_add_color_true_no_prev();
    test_rgb_to_gray_add_color_false_with_prev();
    test_rgb_to_gray_add_color_true_with_prev_chain();

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Total failures: " << failures << "\n";
        return 1;
    }
}
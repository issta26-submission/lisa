// High-quality C++11 unit tests for the focal C function:
// image_transform_png_set_gray_to_rgb_add
// This test suite is designed to verify the pointer manipulations and the
// boolean return value of the function under test without terminating on
// first failure (non-terminating assertions style).

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <png.h>
#include <math.h>


extern "C" {
    // Forward declaration of the C type as used by the focal method.
    // We rely on the project providing a proper libpng-compatible
    // struct image_transform; the test does not access private fields
    // directly except for the 'next' member which is used by the function.
    struct image_transform;

    // Focal method under test (C linkage)
    int image_transform_png_set_gray_to_rgb_add(struct image_transform *this_ptr,
                                                const struct image_transform **that,
                                                png_byte colour_type,
                                                png_byte bit_depth);
}

// Simple non-terminating assertion framework
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond)  do {                                    \
    ++tests_run;                                                     \
    if (!(cond)) {                                                   \
        ++tests_failed;                                              \
        std::cerr << "[TEST " << tests_run << "] FAILURE: " #cond  \
                  << " at line " << __LINE__ << "\n";                \
    }                                                                \
} while (0)

#define EXPECT_FALSE(cond) do {                                    \
    ++tests_run;                                                     \
    if (cond) {                                                      \
        ++tests_failed;                                              \
        std::cerr << "[TEST " << tests_run << "] FAILURE: " #cond  \
                  << " (expected false) at line " << __LINE__ << "\n"; \
    }                                                                \
} while (0)

#define EXPECT_EQ(a, b) do {                                         \
    ++tests_run;                                                       \
    if (!((a) == (b))) {                                             \
        ++tests_failed;                                                \
        std::cerr << "[TEST " << tests_run << "] FAILURE: " #a " == " #b    \
                  << " (actual: " << (a) << ", expected: " << (b)       \
                  << ") at line " << __LINE__ << "\n";                  \
    }                                                                \
} while (0)

static void run_test_true_branch_grayscale_updates_pointers_and_returns_true() {
    // Test that when colour_type has no color mask, the function returns true,
    // and that it links the new transformer as the head of the chain while
    // the previous head is preserved in 'next'.

    // Allocate a fresh image_transform object to act as "this" (the new node)
    image_transform *node = new image_transform();

    // Previous head (pointer to const image_transform)
    const image_transform *head = nullptr; // initial head

    // Call the focal function with a grayscale colour type (no color mask)
    // We pass colour_type = 0 which should yield (0 & PNG_COLOR_MASK_COLOR) == 0
    const png_byte grayscale_type = 0;
    const png_byte bit_depth = 8; // unused by the function, but provided for completeness

    int ret = image_transform_png_set_gray_to_rgb_add(node, &head, grayscale_type, bit_depth);

    // Assertions
    EXPECT_TRUE(ret != 0);        // true branch
    EXPECT_TRUE(head == node);    // head updated to new node
    // The new node's 'next' should point to previous head (nullptr in this test)
    // Access to the 'next' member is assumed to be present in the struct.
    EXPECT_TRUE(node->next == nullptr);

    delete node;
}

static void run_test_false_branch_color_mask_set_returns_false() {
    // Test that when colour_type includes the color mask, the function returns false
    // and updates the chain similarly to the true branch.

    image_transform *node = new image_transform();

    const image_transform *head = nullptr; // initial head
    // Use a colour_type with the color mask bit set.
    // PNG_COLOR_MASK_COLOR is typically 0x02; using it directly ensures the
    // predicate is non-zero.
    const png_byte color_mask_type = PNG_COLOR_MASK_COLOR;
    const png_byte bit_depth = 8; // unused by the function

    int ret = image_transform_png_set_gray_to_rgb_add(node, &head, color_mask_type, bit_depth);

    // Assertions
    EXPECT_FALSE(ret != 0);       // false branch
    EXPECT_TRUE(head == node);    // head updated to new node
    EXPECT_TRUE(node->next == nullptr);

    delete node;
}

static void run_test_linked_chain_updates_previous_head() {
    // Build a small chain of two transforms and verify that the second
    // transformation correctly links to the first one and that pointers are updated.

    // First node
    image_transform *first = new image_transform();
    const image_transform *head1 = nullptr;
    int ret1 = image_transform_png_set_gray_to_rgb_add(first, &head1, 0, 8);
    EXPECT_TRUE(ret1 != 0);
    EXPECT_TRUE(head1 == first);
    EXPECT_TRUE(first->next == nullptr);

    // Second node
    image_transform *second = new image_transform();
    const image_transform *head2 = head1; // initialize head2 to current head
    int ret2 = image_transform_png_set_gray_to_rgb_add(second, &head2, 0, 8);

    // Assertions after second insert
    EXPECT_TRUE(ret2 != 0);
    EXPECT_TRUE(head2 == second);       // head updated to second
    // second->next should point to the previous head (first)
    EXPECT_TRUE(second->next == first);
    // First node should remain intact as the tail (its next was nullptr)
    EXPECT_TRUE(first->next == nullptr);

    delete second;
    delete first;
}

int main(void) {
    // Run all tests in a deterministic order
    run_test_true_branch_grayscale_updates_pointers_and_returns_true();
    run_test_false_branch_color_mask_set_returns_false();
    run_test_linked_chain_updates_previous_head();

    // Summary
    std::cout << "Tests run: " << tests_run
              << ", Failures: " << tests_failed << std::endl;

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}
// Lightweight C++11 unit test suite for the focal C function image_transform_png_set_expand_add
// This test suite is designed to run without Google Test.
// It uses a minimal in-source test harness with non-terminating assertions to maximize coverage.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// Domain-specific minimal type definitions to mirror the focal API for testing.
// We avoid depending on libpng headers to keep the test self-contained and compilable
// in a typical environment where the real library is linked in separately.

typedef unsigned char png_byte;

// Minimal representation of the image_transform struct used by the focal function.
// Based on usage in the focal code, next is a pointer to the next transform in a chain.
// The function assigns and forwards pointers, so we mirror a compatible layout.
typedef struct image_transform {
    const struct image_transform *next;
} image_transform;

// Declaration of the focal function from the production code (C linkage to avoid name mangling).
// Note: We intentionally avoid using the name "this" for the parameter in C++ code.
// The signature matches the production code conventions (parameter types inferred from the focal snippet).
extern "C" int image_transform_png_set_expand_add(image_transform *pThis,
    const image_transform **that, png_byte colour_type, png_byte bit_depth);

// Simple non-terminating test harness
static int g_failures = 0;

// Helper macro to report pointer equality mismatches clearly.
// Cast pointers to void* for portable address printing.
#define EXPECT_PTR_EQ(a, b) do { \
    if ((const void*)(a) != (const void*)(b)) { \
        ++g_failures; \
        std::fprintf(stderr, "FAIL: %s:%d: Pointer mismatch: %s (%p) != %s (%p)\n", __FILE__, __LINE__, #a, (const void*)(a), #b, (const void*)(b)); \
    } \
} while (0)

// Helper macro to assert boolean conditions without terminating the test run.
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        ++g_failures; \
        std::fprintf(stderr, "FAIL: %s:%d: Condition failed: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while (0)

// Test 1: When colour_type has no alpha, image_transform_png_set_expand_add should return true,
// and it should update the chain pointers accordingly: new transform becomes the head and
// its next points to the previous head.
static void test_expand_add_no_alpha_updates_chain_correctly() {
    // Setup: old head in the chain
    image_transform old_head;
    old_head.next = nullptr;

    // New transform to insert at head
    image_transform new_head;
    new_head.next = nullptr; // initialize

    // the pointer to the previous head (pointer to a const image_transform)
    const image_transform *that = &old_head;

    // Call the focal function with no alpha in the colour_type (e.g., 0x01 has no alpha bit set)
    // bit_depth is unused by the function; pass an arbitrary value (e.g., 8)
    int result = image_transform_png_set_expand_add(&new_head, &that, 0x01, 8);

    // Validate: function returns true
    EXPECT_TRUE(result != 0);

    // Validate pointer update: that should now point to the newly inserted transform
    EXPECT_PTR_EQ(that, &new_head);

    // Validate that new_head.next points to the previously head (old_head)
    EXPECT_PTR_EQ(new_head.next, &old_head);

    // Sanity: old head should remain unchanged otherwise
    EXPECT_PTR_EQ(old_head.next, nullptr);
}

// Test 2: When colour_type has the alpha bit set, image_transform_png_set_expand_add should return false,
// but the pointer-linking side-effect should still occur (new head inserted at head of the chain).
static void test_expand_add_with_alpha_returns_false_and_updates_chain() {
    // Setup: old head in the chain
    image_transform old_head;
    old_head.next = nullptr;

    // New transform to insert at head
    image_transform new_head;
    new_head.next = nullptr; // initialize

    // the pointer to the previous head (pointer to a const image_transform)
    const image_transform *that = &old_head;

    // Call the focal function with an alpha-bearing colour_type (e.g., 0x08 is commonly
    // used as PNG_COLOR_MASK_ALPHA in libpng)
    int result = image_transform_png_set_expand_add(&new_head, &that, 0x08, 8);

    // Validate: function returns false
    EXPECT_TRUE(result == 0);

    // Validate pointer update: that should now point to the newly inserted transform
    EXPECT_PTR_EQ(that, &new_head);

    // Validate that new_head.next points to the previously head (old_head)
    EXPECT_PTR_EQ(new_head.next, &old_head);

    // Sanity: old head should remain unchanged otherwise
    EXPECT_PTR_EQ(old_head.next, nullptr);
}

// Main function to drive tests.
// According to the domain knowledge, tests should be invoked from main when not using a test framework.
int main(void) {
    // Run tests
    test_expand_add_no_alpha_updates_chain_correctly();
    test_expand_add_with_alpha_returns_false_and_updates_chain();

    if (g_failures == 0) {
        std::puts("ALL TESTS PASSED");
        return 0;
    } else {
        std::fprintf(stderr, "%d TEST(S) FAILED\n", g_failures);
        return 1;
    }
}

// Note on dependencies and linking:
// - This test provides minimal type definitions and re-declares the focal function prototype
//   to allow linking with the actual production code (shared library or object file) that
//   defines image_transform_png_set_expand_add following the same signature.
// - If building in an environment without the real function, supply a compatible mock implementation
//   of image_transform_png_set_expand_add for compilation and run-time behavior checks.
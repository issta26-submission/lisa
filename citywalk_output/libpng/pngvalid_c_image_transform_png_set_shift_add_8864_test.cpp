// Unit test suite for the focal method: image_transform_png_set_shift_add
// This test suite is written in C++11 without any testing framework (GTest not allowed).
// It directly calls the C function from the provided focal source.
// The tests focus on the function's insertion into a singly-linked list and its return predicate.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Minimal stand-in type declarations to match the focal function's signature.
// We purposely keep these minimal and compatible with the actual implementation
// used in the pngvalid.c file, which relies on an image_transform struct with a
// 'next' pointer.

typedef struct image_transform image_transform;
struct image_transform {
    image_transform *next;
};

// png_byte is typically unsigned char in libpng.
// We mirror that here for compatibility with the focal function's signature.
typedef unsigned char png_byte;
#define PNG_COLOR_TYPE_PALETTE 3  // Palette color type constant as used by the focal code.

// Forward declaration of the focal function from the C source (linker will provide the definition).
// Note: The parameter name 'this' in the implementation conflicts with C++'s keyword
// 'this'. We provide an alternate name here (this_ptr) for the declaration.
extern "C" int image_transform_png_set_shift_add(image_transform *this_ptr,
                                               const image_transform **that,
                                               png_byte colour_type,
                                               png_byte bit_depth);

// Simple test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Basic assertion macro that records failures without terminating the test run.
#define TEST_ASSERT(cond, msg) do {                                     \
    ++g_tests_run;                                                        \
    if(!(cond)) {                                                         \
        ++g_tests_failed;                                                 \
        std::cerr << "Test failed: " << (msg)                           \
                  << " (line " << __LINE__ << ")" << std::endl;        \
    }                                                                   \
} while(0)

// Test 1: Palette color type should not be inserted as a next color (return false)
 // and the new transform should become the head of the list.
// Verifies: head points to the newly inserted 'this', and previous head becomes next.
static void test_shift_add_palette_sets_head_and_returns_false() {
    // Arrange
    image_transform a;
    a.next = nullptr;

    const image_transform *head = nullptr; // initial list is empty
    // Act
    int ret = image_transform_png_set_shift_add(&a, &head, PNG_COLOR_TYPE_PALETTE, 0);
    // Assert
    TEST_ASSERT(head == &a, "Head should point to the newly inserted transform after palette add");
    TEST_ASSERT(a.next == nullptr, "New transform's next should be the previous head (nullptr)");
    TEST_ASSERT(ret == 0, "Return value should be false (0) for PNG_COLOR_TYPE_PALETTE");
}

// Test 2: Non-palette color type should be inserted before existing head (true branch)
// and the previous head becomes the 'next' of the new head.
static void test_shift_add_non_palette_inserts_before_existing_head() {
    // Arrange: existing head
    image_transform existing;
    existing.next = nullptr;
    const image_transform *head = &existing;

    // New transform to insert
    image_transform newt;
    newt.next = nullptr;

    // Act
    int ret = image_transform_png_set_shift_add(&newt, &head, 1 /* not PALETTE */, 42);

    // Assert
    TEST_ASSERT(head == &newt, "Head should be updated to the newly inserted transform");
    TEST_ASSERT(newt.next == &existing, "New transform's next should point to the previous head");
    TEST_ASSERT(ret == 1, "Return value should be true (non-zero) for non-palette colour_type");
}

// Test 3: Bit depth should be ignored by the function (true for non-palette path)
// This test ensures bit_depth does not affect linkage behavior or return value.
static void test_shift_add_bit_depth_ignored() {
    // Arrange: empty list
    image_transform first;
    first.next = nullptr;
    const image_transform *head = nullptr;

    // Act: first insertion with non-palette colour_type
    int ret1 = image_transform_png_set_shift_add(&first, &head, 2 /* not PALETTE */, 0);
    TEST_ASSERT(head == &first, "Head should point to first transform after non-palette insert");
    TEST_ASSERT(first.next == nullptr, "First transform's next should be nullptr (empty list)");
    TEST_ASSERT(ret1 == 1, "Return value should be true for non-palette colour_type");

    // Arrange: second insertion with different bit_depth
    image_transform second;
    second.next = nullptr;

    int ret2 = image_transform_png_set_shift_add(&second, &head, 2 /* still non-palette */, 255);
    TEST_ASSERT(head == &second, "Head should now be the second inserted transform");
    TEST_ASSERT(second.next == &first, "Second transform's next should point to the previous head (first)");
    TEST_ASSERT(ret2 == 1, "Return value should remain true regardless of bit_depth (non-palette)");
}

// Entry point: run all tests and report summary
int main(void) {
    test_shift_add_palette_sets_head_and_returns_false();
    test_shift_add_non_palette_inserts_before_existing_head();
    test_shift_add_bit_depth_ignored();

    std::cerr << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    if (g_tests_failed > 0) {
        return 1;
    }
    return 0;
}
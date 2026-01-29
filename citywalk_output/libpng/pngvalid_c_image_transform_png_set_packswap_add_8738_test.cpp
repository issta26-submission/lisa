// C++11 unit test suite for the focal C function:
// image_transform_png_set_packswap_add
// This test suite assumes the function is provided by the project (pngvalid.c)
// and linked with this test binary. We reproduce minimal supporting types here
// to exercise the function behavior.
// The tests are written without any testing framework (no GTest) per requirements.

#include <cassert>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Domain knowledge note: We are testing the following behavior of
// image_transform_png_set_packswap_add:
// - It inserts the given node at the head of a singly-linked list (that is pointed to by *that).
// - It returns 1 if bit_depth < 8, otherwise 0.
// - The colour_type parameter is unused (ignored) by the function (observed via UNUSED macro
//   in the original code). Our tests verify that varying colour_type does not affect behavior.

// Minimal type compatibility shim to match expected C interface
typedef unsigned char png_byte;

// Forward declaration of the focal function with C linkage to ensure proper linking.
extern "C" int image_transform_png_set_packswap_add(struct image_transform *this_ptr,
                                                   const struct image_transform **that,
                                                   png_byte colour_type, png_byte bit_depth);

// Minimal representation of the dependency type used by the focal function.
struct image_transform {
    image_transform* next;
};

// Helper to clean up a singly linked list of image_transform nodes
static void delete_chain(image_transform* head) {
    while (head != nullptr) {
        image_transform* n = head;
        head = head->next;
        delete n;
    }
}

// Test 1: Basic insertion at head with bit_depth = 7 (true branch)
static void test_packswap_add_inserts_head_bitdepth7() {
    // We model an empty list and insert a single new node at the head.
    image_transform* head = nullptr;

    image_transform* new_node = new image_transform();
    new_node->next = nullptr;

    int result = image_transform_png_set_packswap_add(new_node, &head, 0, 7);

    // Expected: head points to new_node, new_node->next is nullptr, result == 1
    assert(head == new_node);
    assert(new_node->next == nullptr);
    assert(result == 1);

    // Cleanup
    delete_chain(head);
    // Test passed
    std::cout << "Test 1 passed: insertion at head with bit_depth 7 (true branch).\n";
}

// Test 2: Basic insertion at head with bit_depth = 8 (false branch)
static void test_packswap_add_inserts_head_bitdepth8() {
    image_transform* head = nullptr;

    image_transform* new_node = new image_transform();
    new_node->next = nullptr;

    int result = image_transform_png_set_packswap_add(new_node, &head, 0, 8);

    // Expected: head points to new_node, new_node->next is nullptr, result == 0
    assert(head == new_node);
    assert(new_node->next == nullptr);
    assert(result == 0);

    // Cleanup
    delete_chain(head);
    std::cout << "Test 2 passed: insertion at head with bit_depth 8 (false branch).\n";
}

// Test 3: Insertion when a non-empty head already exists
// Verifies that the new node becomes the head and its next points to the old head.
static void test_packswap_add_inserts_head_over_existing() {
    // Create an existing head node
    image_transform* old_head = new image_transform();
    old_head->next = nullptr;

    // Create new node to insert at head
    image_transform* new_node = new image_transform();
    new_node->next = nullptr;

    image_transform* head = old_head;

    int result = image_transform_png_set_packswap_add(new_node, &head, 0, 7);

    // Expected: head == new_node; new_node->next == old_head; old_head remains as tail
    assert(head == new_node);
    assert(new_node->next == old_head);
    assert(old_head->next == nullptr);
    assert(result == 1);

    // Cleanup
    delete_chain(head); // this will delete new_node and old_head in chain
    std::cout << "Test 3 passed: insertion at head when a non-empty list exists (true branch).\n";
}

// Test 4: Colour_type is ignored by the function
// Verify that changing colour_type does not affect insertion behavior or return value.
static void test_packswap_add_ignores_colour_type() {
    // Case A: colour_type = 0
    image_transform* head_a = nullptr;
    image_transform* node_a = new image_transform();
    node_a->next = nullptr;

    int res_a = image_transform_png_set_packswap_add(node_a, &head_a, 0, 7);
    assert(head_a == node_a);
    assert(node_a->next == nullptr);
    assert(res_a == 1);
    delete_chain(head_a);

    // Case B: colour_type = 255
    image_transform* head_b = nullptr;
    image_transform* node_b = new image_transform();
    node_b->next = nullptr;

    int res_b = image_transform_png_set_packswap_add(node_b, &head_b, 255, 7);
    assert(head_b == node_b);
    assert(node_b->next == nullptr);
    assert(res_b == 1);
    delete_chain(head_b);

    // If both results and structures are identical, colour_type is ignored.
    std::cout << "Test 4 passed: colour_type is ignored by image_transform_png_set_packswap_add.\n";
}

// Entry point to run all tests
int main() {
    // Step 2/3: Generate and refine tests to cover both branches and list manipulation
    // Domain knowledge: Use only standard library, simple asserts, and a single main
    // function to drive tests (no GTest/GMock). Tests rely on the public API of the focal
    // method and its simple behaviour (head insertion and  bit-depth based return).

    // Run tests
    test_packswap_add_inserts_head_bitdepth7();
    test_packswap_add_inserts_head_bitdepth8();
    test_packswap_add_inserts_head_over_existing();
    test_packswap_add_ignores_colour_type();

    std::cout << "All tests passed.\n";
    return 0;
}
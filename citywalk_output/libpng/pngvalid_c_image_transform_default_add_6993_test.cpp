// C++11 test suite for the focal method: image_transform_default_add
// NOTE: This test is designed as a lightweight, framework-free unit test
// harness (no Google Test, etc.). It relies on the presence of the corresponding
// C function image_transform_default_add with C linkage.
// The test uses a minimal, compatible layout for image_transform to access the
// 'next' pointer as a means to verify the linked-list insertion semantics.
//
// Candidate Keywords (Step 1):
// - image_transform, next, that (head of the list), colour_type, bit_depth
// - The behavior: this->next gets old *that, *that becomes this, returns 1
// - Edge case: *that is NULL (inserting at head of an empty list)
// - Verification of return value and pointer updates
//
// Important caveat (Step 3 - Domain Knowledge): We assume that the real
// image_transform type used by the library has a member 'image_transform *next'
// as its first field. For testing purposes here, we provide a compatible
// layout and interact with the function through that layout. If your build embeds
// a different internal layout, ensure the test is adapted accordingly.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// Forward declaration to avoid depending on private headers.
// The real library defines a struct image_transform; we declare a compatible
// layout here for testing the linkage semantics of image_transform_default_add.
struct image_transform;

// Public API from the focal C code (C linkage)
extern "C" int image_transform_default_add(image_transform *this_ptr,
                                          const image_transform **that,
                                          unsigned char colour_type,
                                          unsigned char bit_depth);

// Minimal compatible layout for testing. We only rely on the 'next' pointer.
struct image_transform {
    image_transform *next;
    // Other fields are intentionally omitted for the test harness.
};

// Lightweight test harness
static int g_test_count = 0;
static int g_fail_count = 0;

static void report_pass(const char* name) {
    ++g_test_count;
    std::printf("PASS: %s\n", name);
}
static void report_fail(const char* name, const char* detail) {
    ++g_test_count;
    ++g_fail_count;
    std::fprintf(stderr, "FAIL: %s - %s\n", name, detail ? detail : "");
}

// Helper assertion macro (non-terminating): records failure but continues
#define ASSERT(cond, name, detail) do { if(!(cond)) { report_fail(name, detail); return; } } while(0)


// Test 1: Add to an empty list (when *that is NULL). Expect head to point to 'this'
// and that->next to be NULL. Return value should be 1.
static void test_add_to_empty_list() {
    const char* test_name = "test_add_to_empty_list";

    image_transform node;
    node.next = nullptr;

    image_transform *head = nullptr;
    int ret = image_transform_default_add(&node, (const image_transform **)&head, 0, 0);

    ASSERT(ret == 1, test_name, "return value == 1");
    ASSERT(head == &node, test_name, "head points to the newly added node");
    ASSERT(node.next == nullptr, test_name, "new node's next is NULL when inserting into empty list");
    report_pass(test_name);
}

// Test 2: Add to a non-empty list. Ensure insertion happens at head and old head becomes next.
static void test_add_to_non_empty_list() {
    const char* test_name = "test_add_to_non_empty_list";

    image_transform first;
    first.next = nullptr;

    image_transform second;
    second.next = nullptr;

    image_transform *head = &first;
    int ret = image_transform_default_add(&second, (const image_transform **)&head, 0, 0);

    ASSERT(ret == 1, test_name, "return value == 1");
    ASSERT(head == &second, test_name, "head updated to new node (second)");
    ASSERT(second.next == &first, test_name, "new node's next points to old head (first)");
    ASSERT(first.next == nullptr, test_name, "old head remains unchanged (next == NULL)");
    report_pass(test_name);
}

// Test 3: Insert another node when there is an existing chain (head points to a chain).
static void test_insert_into_existing_chain() {
    const char* test_name = "test_insert_into_existing_chain";

    image_transform tail;
    tail.next = nullptr;

    image_transform middle;
    middle.next = &tail;

    image_transform head_node;
    head_node.next = &middle;

    image_transform new_node;
    new_node.next = nullptr;

    image_transform *head = &head_node;
    int ret = image_transform_default_add(&new_node, (const image_transform **)&head, 0, 0);

    ASSERT(ret == 1, test_name, "return value == 1");
    ASSERT(head == &new_node, test_name, "head updated to new_node");
    ASSERT(new_node.next == &head_node, test_name, "new_node.next points to old head (head_node)");
    ASSERT(head_node.next == &middle, test_name, "existing chain remains intact (middle after head_node)");
    ASSERT(middle.next == &tail, test_name, "existing chain tail intact (tail after middle)");
    report_pass(test_name);
}

// Test 4: Verify colour_type and bit_depth are ignored (no side effects). Use non-zero values.
static void test_ignored_parameters() {
    const char* test_name = "test_ignored_parameters";

    image_transform node;
    node.next = nullptr;

    image_transform *head = nullptr;
    int ret = image_transform_default_add(&node, (const image_transform **)&head, 7, 13);

    ASSERT(ret == 1, test_name, "return value == 1");
    ASSERT(head == &node, test_name, "head points to the newly added node");
    ASSERT(node.next == nullptr, test_name, "next is NULL when inserting into empty list with nonzero colour/bit-depth");
    report_pass(test_name);
}


// Entry point
int main(void) {
    // Run tests (Step 2 / Step 3 consolidation)
    test_add_to_empty_list();
    test_add_to_non_empty_list();
    test_insert_into_existing_chain();
    test_ignored_parameters();

    // Summary
    if (g_fail_count == 0) {
        std::printf("ALL TESTS PASSED (%d tests).\n", g_test_count);
        return 0;
    } else {
        std::fprintf(stderr, "SUMMARY: %d failures out of %d tests.\n", g_fail_count, g_test_count);
        return 1;
    }
}
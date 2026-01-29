// Unit test suite for the focal method: buffer_extend
// Purpose: verify that buffer_extend correctly extends a singly linked list node
//          by creating and linking a new node after the provided current node.
//
// Assumptions and environment notes:
// - The test is written in C++11 and links against the C implementation containing
//   the function: struct buffer_list * buffer_extend(struct buffer_list *current);
// - The buffer_list struct is defined in the library; for testing purposes we mirror
//   a compatible layout with the first member being 'struct buffer_list *next'.
// - We rely on the library's NEW macro for allocation inside buffer_extend.
// - We only exercise true-branch behavior where current->next is expected to be NULL.
// - Static analysis and static memory management considerations are kept simple.
// - This test harness uses a minimal, non-terminating check mechanism suitable when gtest is not allowed.

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>


extern "C" {
    // Forward declare the focal function with C linkage.
    // Do not redefine the struct layout here to avoid conflicting layouts across translation units.
    struct buffer_list;
    struct buffer_list * buffer_extend(struct buffer_list *current);
}

// Lightweight test scaffolding: a simple, non-terminating assertion helper
// that records failures but allows tests to proceed for higher coverage.
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_CHECK(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        std::cerr << "[FAIL] " << msg << "\n"; \
    } \
} while (0)

// A minimal definition of buffer_list used by the test as a compatible layout.
// We assume the library's buffer_list has at least a 'next' pointer as its first member.
// This mirrors a common singly-linked list node pattern.
// If the library's actual structure differs, this layout should still be compatible
// for the purposes of pointer-based linking and the observable behavior tested here.
struct buffer_list {
    buffer_list *next;
};

// Test 1: Basic single extension
// - Given a current node with current.next == NULL
// - buffer_extend(current) should allocate a new node, assign it to current.next,
// - and return a pointer to that new node whose next is initialized to NULL.
static void test_buffer_extend_single() {
    // Setup: current node with next == NULL
    buffer_list current;
    current.next = NULL;

    // Action: extend from current
    buffer_list *new_node = buffer_extend(&current);

    // Assertions:
    // 1) The function should return a non-NULL pointer representing the newly created node.
    TEST_CHECK(new_node != NULL, "buffer_extend should return a non-NULL pointer when current.next is NULL");

    // 2) The current node's next should now point to the newly created node.
    TEST_CHECK(current.next == new_node, "buffer_extend should set current.next to the newly created node");

    // 3) The newly created node's next should be NULL (initial state).
    TEST_CHECK(new_node != NULL && new_node->next == NULL, "New node's next should be initialized to NULL");

    // Cleanup: free the allocated node
    if (new_node) {
        free(new_node);
        // Note: 'current' is on stack; no need to free it
    }
}

// Test 2: Double extension (chain building)
// - After the first extension, extend again from the newly created node.
// - Validate the chain: current -> a -> b, with a.next == b and b.next == NULL.
static void test_buffer_extend_chain() {
    // Setup: current node with next == NULL
    buffer_list current;
    current.next = NULL;

    // First extension
    buffer_list *a = buffer_extend(&current);
    TEST_CHECK(a != NULL, "First extension should return a non-NULL node (a)");
    TEST_CHECK(current.next == a, "After first extension, current.next should point to node a");

    // Second extension from 'a'
    buffer_list *b = buffer_extend(a);
    TEST_CHECK(b != NULL, "Second extension should return a non-NULL node (b)");
    TEST_CHECK(a->next == b, "After second extension, a.next should point to node b");
    TEST_CHECK(b->next == NULL, "Second extension should initialize b.next to NULL");

    // Cleanup: free allocated nodes a and b
    if (b) {
        free(b);
    }
    if (a) {
        free(a);
    }
    // current is on stack; nothing to free
}

// Main: run all tests and report summary
int main(void) {
    std::cout << "Running buffer_extend test suite (C++11, no GTest):\n";

    test_buffer_extend_single();
    test_buffer_extend_chain();

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << "\n";

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
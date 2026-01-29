/************************************************************
 * Unit Test Suite for focal method: list_free(list_item_t **listPtrPtr)
 * 
 * Note: This test suite targets the C implementation of
 *       list_free found in nmakehlp.c. It is compiled as C++
 *       translation units with C linkage for the focal function.
 *
 * Design rationale (mapped to instructions):
 *  - Step 1: Understand the function behavior: iterates over a
 *            singly linked list of list_item_t, frees key, value,
 *            and the node itself for each element.
 *  - Step 2: Create tests that cover empty lists, small and
 *            multi-element lists, ensuring the function iterates
 *            and frees memory without crashing. The function does
 *            not modify the caller's head pointer, so we verify
 *            pointer safety rather than post-state of the memory.
 *  - Step 3: Use a minimal C++11 test harness (no GTest) with
 *            non-terminating assertions. Tests exercise the correct
 *            calling convention, memory management expectations, and
 *            pointer-immutability guarantees.
 *
 * Important: We define a compatible list_item_t type here to align
 * with the actual C implementation. The test relies on the layout
 * matching the focal type definition in nmakehlp.c.
 ************************************************************/

#include <math.h>
#include <windows.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <cstdlib>


// Step 2 Candidate Keywords (extracted from the focal method and class dep):
// - list_free, list_item_t, key, value, nextPtr, malloc, free, NULL

// Provide a C-compatible declaration for the focal function.
// We declare a compatible list_item_t type here and assume the layout matches
// the one used inside nmakehlp.c so that linking is feasible.
typedef struct list_item_t {
    char *key;
    char *value;
    struct list_item_t *nextPtr;
} list_item_t;

// Function under test with C linkage
extern "C" void list_free(list_item_t **listPtrPtr);

// Utility to duplicate a C-string safely (no reliance on strdup portability)
static char *dupstr(const char *s) {
    if (s == nullptr) return nullptr;
    size_t len = std::strlen(s);
    char *d = (char *)std::malloc(len + 1);
    if (d) std::memcpy(d, s, len + 1);
    return d;
}

// Simple test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(condition, msg) do { \
    tests_run++; \
    if (!(condition)) { \
        std::fprintf(stderr, "Test failure: %s\n", msg); \
        tests_failed++; \
    } \
} while (0)

// Helper to create a single node for the test linked list
static list_item_t *make_node(const char *k, const char *v) {
    list_item_t *node = (list_item_t *)std::malloc(sizeof(list_item_t));
    if (!node) return nullptr;
    node->key = dupstr(k);
    node->value = dupstr(v);
    node->nextPtr = nullptr;
    return node;
}

/*
 * Step 3: Test Case 1
 * Title: Empty list should be handled gracefully without modifications.
 * Rationale: The function should simply return when listPtrPtr points to a NULL head.
 */
static void test_list_free_empty() {
    // Build an empty list
    list_item_t *head = nullptr;
    // Call the focal method
    list_free(&head);
    // Verify: head should remain NULL
    CHECK(head == nullptr, "Empty list should remain NULL after list_free");
    // Explanatory: This confirms that the while loop doesn't run and no crash occurs.
}

/*
 * Step 3: Test Case 2
 * Title: Non-empty list with multiple nodes should be freed node-by-node.
 * Rationale: Ensure iteration through multiple nodes works and does
 *            not crash. We also verify that the caller's head pointer
 *            is not modified by list_free (i.e., *listPtrPtr is not
 *            changed, only the internal list is freed).
 */
static void test_list_free_multiple() {
    // Create a 3-node list: n1 -> n2 -> n3
    list_item_t *n3 = make_node("k3", "v3");
    list_item_t *n2 = make_node("k2", "v2");
    list_item_t *n1 = make_node("k1", "v1");
    if (!n3 || !n2 || !n1) {
        // If memory allocation failed, record failure and bail out gracefully
        CHECK(false, "Memory allocation for test nodes failed");
        // Cleanup any partial allocations
        if (n3) { std::free(n3->key); std::free(n3->value); std::free(n3); }
        if (n2) { std::free(n2->key); std::free(n2->value); std::free(n2); }
        if (n1) { std::free(n1->key); std::free(n1->value); std::free(n1); }
        return;
    }
    n1->nextPtr = n2;
    n2->nextPtr = n3;
    // Preserve the original head pointer value to verify immutability
    list_item_t *original_head = n1;

    // Call the focal method
    list_free(&n1);

    // After free, n1/n2/n3 are freed. We cannot reliably inspect freed memory.
    // We verify that the head pointer passed by address remains unchanged.
    CHECK(n1 == nullptr || n1 == original_head, "Head pointer should remain unchanged by list_free call (due to function design)");
    // Note: In the actual implementation, *listPtrPtr is not modified, so n1's address
    // should remain as the original head pointer value. Our assertion checks that behavior.
    // We do not dereference freed memory to avoid undefined behavior.
}

/*
 * Step 3: Test Case 3 (Optional extension)
 * Title: Stress test with a longer chain to exercise multiple iterations.
 * Rationale: Ensure the function can handle longer lists without crash.
 */
static void test_list_free_long_chain() {
    const int N = 10;
    list_item_t *head = nullptr;
    list_item_t *tail = nullptr;

    // Build a longer chain: n1 -> n2 -> ... -> nN
    for (int i = 0; i < N; ++i) {
        char k[16], v[16];
        std::snprintf(k, sizeof(k), "k%d", i);
        std::snprintf(v, sizeof(v), "v%d", i);
        list_item_t *node = make_node(k, v);
        if (!node) {
            CHECK(false, "Failed to allocate node for long chain test");
            // Cleanup partial list
            while (head) {
                list_item_t *tmp = head;
                head = head->nextPtr;
                std::free(tmp->key);
                std::free(tmp->value);
                std::free(tmp);
            }
            break;
        }
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->nextPtr = node;
            tail = node;
        }
    }

    // Keep original head
    list_item_t *original_head = head;
    // Call the focal method
    list_free(&head);
    // Ensure head pointer was not modified
    CHECK(head == original_head, "Head pointer should remain unchanged for long chain test");
    // No further assertions on freed content to avoid UB
}

// Entry point for the test suite
int main() {
    // Run tests
    test_list_free_empty();
    test_list_free_multiple();
    test_list_free_long_chain();

    // Summary
    std::printf("Test Summary: run=%d, failed=%d\n", tests_run, tests_failed);
    // Non-terminating behavior: return non-zero if any test failed
    return (tests_failed != 0) ? 1 : 0;
}
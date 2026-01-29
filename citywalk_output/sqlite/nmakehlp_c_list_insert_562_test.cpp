/*
Unit test suite for the focal method:
list_insert(list_item_t **listPtrPtr, const char *key, const char *value)

Assumptions:
- The actual implementation is provided in nmakehlp.c and uses a struct defined as:
  typedef struct list_item_t {
      char *key;
      char *value;
      struct list_item_t *nextPtr;
  } list_item_t;
- The function allocates a new node, duplicates key/value with strdup, sets nextPtr to NULL,
  and appends the node at the end of the list pointed to by *listPtrPtr.
- This test suite is written in C++11, uses a C interface to the function under test, and does not rely on Google Test.
- Memory management is performed by freeing the allocated nodes and the duplicated strings.

Notes for reviewers:
- Tests cover: insertion into an empty list, insertion at the end of a non-empty list, and verification that key/value are duplicated (modifying original inputs does not affect stored copies).
- Each test includes explanatory comments and uses non-terminating assertions (they print failures and continue).
*/

#include <math.h>
#include <windows.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <cstdlib>


// Expose C API for the function under test
extern "C" {

    // Definition of the list node as expected by the focal method.
    // This must match the actual definition in the implementation for safe access in tests.
    typedef struct list_item_t {
        char *key;
        char *value;
        struct list_item_t *nextPtr;
    } list_item_t;

    // Prototype of the focal method under test
    list_item_t *list_insert(list_item_t **listPtrPtr, const char *key, const char *value);
}

// Simple string equality helper
static bool streq(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) return a == b;
    return std::strcmp(a, b) == 0;
}

// Free a singly linked list created by list_insert to avoid memory leaks
static void freeList(list_item_t** head) {
    while (*head) {
        list_item_t* cur = *head;
        *head = cur->nextPtr;
        if (cur->key) free(cur->key);
        if (cur->value) free(cur->value);
        free(cur);
    }
}

// Global test failure counter (non-terminating, to maximize coverage)
static int g_failures = 0;

// Non-terminating assertions (print on failure but continue)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_STR(a,b) do { \
    if(((a)==nullptr && (b)!=nullptr) || ((a)!=nullptr && (b)==nullptr) || std::strcmp((a),(b)) != 0) { \
        std::fprintf(stderr, "EXPECT_EQ_STR failed: \"%s\" != \"%s\" at %s:%d\n", (a)?(a):"NULL", (b)?(b):"NULL", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Test 1: Inserting into an empty list should set head to the new node and store correct key/value
static void test_insert_into_empty_list() {
    // Explanation:
    // - Start with an empty list (head == NULL)
    // - Insert one item
    // - Verify the returned node is non-NULL, head points to this node, and key/value are correct
    // - The nextPtr of the head should be NULL
    list_item_t *head = NULL;
    list_item_t *node = list_insert(&head, "ABC", "123");

    EXPECT_TRUE(node != NULL);              // Insertion should succeed
    EXPECT_TRUE(head == node);              // Empty list should return the new node as head
    EXPECT_TRUE(head != NULL && streq(head->key, "ABC"));
    EXPECT_TRUE(head != NULL && streq(head->value, "123"));
    EXPECT_TRUE(head != NULL && head->nextPtr == NULL);

    // Cleanup
    freeList(&head);
}

// Test 2: Inserting into a non-empty list should append at the end
static void test_append_to_non_empty_list() {
    // Explanation:
    // - Create a list with one node
    // - Insert a second node; the head should remain the first node, and head->nextPtr should point to the second
    // - Validate both nodes' key/value and linkage
    list_item_t *head = NULL;
    list_item_t *first = list_insert(&head, "First", "one");
    EXPECT_TRUE(first != NULL);
    EXPECT_TRUE(head == first);

    list_item_t *second = list_insert(&head, "Second", "two");
    EXPECT_TRUE(second != NULL);
    // Head should still be the first node
    EXPECT_TRUE(head == first);
    // The second node should be appended at the end
    EXPECT_TRUE(head->nextPtr == second);

    // Validate contents of second node
    EXPECT_TRUE(streq(second->key, "Second"));
    EXPECT_TRUE(streq(second->value, "two"));
    EXPECT_TRUE(second->nextPtr == NULL);

    // Cleanup
    freeList(&head);
}

// Test 3: Ensure that key/value are duplicated (modifying original inputs should not affect stored copies)
static void test_duplicate_strings_independence() {
    // Explanation:
    // - Pass in buffers for key/value
    // - Modify the original buffers after insertion
    // - Ensure stored copies remain unchanged
    list_item_t *head = NULL;
    char keybuf[8] = "GammaX";
    char valbuf[4] = "7";

    list_item_t *node = list_insert(&head, keybuf, valbuf);
    EXPECT_TRUE(node != NULL);

    // Mutate original buffers after insertion
    keybuf[0] = 'g';
    valbuf[0] = '8';

    // Stored copies should remain as originally provided
    EXPECT_TRUE(head != NULL);
    EXPECT_TRUE(streq(head->key, "GammaX"));
    EXPECT_TRUE(streq(head->value, "7"));

    // Cleanup
    freeList(&head);
}

int main() {
    // Run tests
    test_insert_into_empty_list();
    test_append_to_non_empty_list();
    test_duplicate_strings_independence();

    // Summary
    if (g_failures == 0) {
        std::printf("All tests passed.\n");
    } else {
        std::fprintf(stderr, "%d test(s) failed.\n", g_failures);
    }
    return g_failures;
}
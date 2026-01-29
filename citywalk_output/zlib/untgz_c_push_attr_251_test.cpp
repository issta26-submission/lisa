// Unit test suite for the focal method push_attr in untgz.c
// This test suite is written for C++11 without any external test framework.
// It uses a small, self-contained test harness with explicit test functions
// and a main() that executes tests and reports results.
//
// Notes:
// - The tests assume the following struct layout present in the production file:
//     struct attr_item {
//       char *fname;
//       int mode;
//       time_t time;
//       struct attr_item *next;
//     };
// - The function under test is assumed to have the signature:
//     void push_attr(struct attr_item **list, char *fname, int mode, time_t time);
// - We declare a matching struct layout in the test to inspect the linked list.
// - Memory is allocated via malloc/strdup in the production code; tests free allocated memory.

/* Include standard library headers for test utilities */
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// C linkage for the production code symbols
extern "C" {
    // Re-declare the struct layout used by the production code to inspect fields.
    struct attr_item {
        char *fname;
        int mode;
        time_t time;
        struct attr_item *next;
    };

    // Prototype of the function under test
    void push_attr(struct attr_item **list, char *fname, int mode, time_t time);
}

// Helper: free a linked list created by push_attr
static void free_list(struct attr_item *lst) {
    while (lst) {
        struct attr_item *n = lst->next;
        free(lst->fname);
        free(lst);
        lst = n;
    }
}

/*
Test 1: push_attr adds the first item correctly to an empty list.
- Expected:
  - head is non-NULL
  - head->fname is equal to the provided fname content ("test1.txt")
  - head->mode equals the provided mode (0644)
  - head->time equals the provided time
  - head->next is NULL
- Rationale: Verifies the basic allocation, field initialization, and list head insertion.
*/
static bool test_push_attr_adds_first_item() {
    struct attr_item *head = NULL;
    time_t t1 = 123456; // deterministic timestamp
    // Cast to non-const to match the function signature
    push_attr(&head, (char*)"test1.txt", 0644, t1);

    bool ok = true;
    if (head == NULL) {
        ok = false;
    } else {
        if (strcmp(head->fname, "test1.txt") != 0) ok = false;
        if (head->mode != 0644) ok = false;
        if (head->time != t1) ok = false;
        if (head->next != NULL) ok = false;
    }

    free_list(head);
    return ok;
}

/*
Test 2: push_attr correctly updates the head when a second item is added.
- Steps:
  - Push first item ("first.txt", 0644, t1)
  - Push second item ("second.txt", 0600, t2)
- Expected:
  - New head corresponds to "second.txt" with mode 0600 and time t2
  - New head's next points to the first item
  - The first item retains its values ("first.txt", 0644, t1) and is intact
- Rationale: Ensures the list is correctly linked in LIFO fashion.
*/
static bool test_push_attr_second_item_links() {
    struct attr_item *head = NULL;
    time_t t1 = 1111;
    push_attr(&head, (char*)"first.txt", 0644, t1);

    struct attr_item *first = head; // reference to the first node

    time_t t2 = 2222;
    push_attr(&head, (char*)"second.txt", 0600, t2);

    bool ok = true;
    if (head == NULL) ok = false;
    else {
        if (strcmp(head->fname, "second.txt") != 0) ok = false;
        if (head->mode != 0600) ok = false;
        if (head->time != t2) ok = false;
        if (head->next != first) ok = false;

        // Verify the first item remains intact
        if (first == NULL) ok = false;
        else {
            if (strcmp(first->fname, "first.txt") != 0) ok = false;
            if (first->mode != 0644) ok = false;
            if (first->time != t1) ok = false;
        }
    }

    free_list(head);
    return ok;
}

/*
Test 3: push_attr copies the input fname string (not merely storing the pointer).
- Steps:
  - Prepare a local modifiable buffer fname_in = "orig.txt"
  - Call push_attr with fname_in
  - Modify fname_in after the call (e.g., fname_in[0] = 'X')
- Expected:
  - head->fname content remains "orig.txt"
  - head->fname pointer is different from fname_in (i.e., a copy was made via strdup)
- Rationale: Validates that the function duplicates the string, avoiding aliasing with the input buffer.
*/
static bool test_push_attr_copies_input_string() {
    struct attr_item *head = NULL;
    char fname_in[] = "orig.txt"; // buffer we can modify after the call
    time_t t = 3333;

    push_attr(&head, fname_in, 0644, t);

    bool ok = true;
    if (head == NULL) ok = false;
    else {
        // The stored name should be a copy of "orig.txt"
        if (strcmp(head->fname, "orig.txt") != 0) ok = false;
        // The stored fname pointer should not be the same as the input buffer
        if (head->fname == fname_in) ok = false;

        // Modify the input buffer and ensure the copy remains unchanged
        fname_in[0] = 'X';
        if (strcmp(head->fname, "orig.txt") != 0) ok = false;
    }

    free_list(head);
    return ok;
}

/*
Test 4: push_attr with three items creates a correctly linked chain in proper order.
- Steps:
  - Push "a.txt" with t1
  - Push "b.txt" with t2
  - Push "c.txt" with t3
- Expected order (head to tail):
  - head -> "c.txt" (t3)
  - head->next -> "b.txt" (t2)
  - head->next->next -> "a.txt" (t1)
- Rationale: Ensures multiple insertions maintain correct order and data integrity across the chain.
*/
static bool test_push_attr_three_item_list_order() {
    struct attr_item *head = NULL;
    time_t t1 = 1;
    time_t t2 = 2;
    time_t t3 = 3;

    push_attr(&head, (char*)"a.txt", 0700, t1);
    push_attr(&head, (char*)"b.txt", 0644, t2);
    push_attr(&head, (char*)"c.txt", 0555, t3);

    bool ok = true;
    // Validate entire chain
    if (head == NULL || head->next == NULL || head->next->next == NULL) ok = false;
    if (strcmp(head->fname, "c.txt") != 0) ok = false;
    if (head->time != t3 || head->mode != 0555) ok = false;
    if (strcmp(head->next->fname, "b.txt") != 0) ok = false;
    if (head->next->time != t2 || head->next->mode != 0644) ok = false;
    if (strcmp(head->next->next->fname, "a.txt") != 0) ok = false;
    if (head->next->next->time != t1 || head->next->next->mode != 0700) ok = false;

    free_list(head);
    return ok;
}

/* Minimal test runner and report */
int main() {
    int total = 4;
    int passed = 0;

    std::cout << "Running unit tests for push_attr (untgz.c) with C++11 test harness...\n";

    if (test_push_attr_adds_first_item()) {
        std::cout << "[PASS] test_push_attr_adds_first_item\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_push_attr_adds_first_item\n";
    }

    if (test_push_attr_second_item_links()) {
        std::cout << "[PASS] test_push_attr_second_item_links\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_push_attr_second_item_links\n";
    }

    if (test_push_attr_copies_input_string()) {
        std::cout << "[PASS] test_push_attr_copies_input_string\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_push_attr_copies_input_string\n";
    }

    if (test_push_attr_three_item_list_order()) {
        std::cout << "[PASS] test_push_attr_three_item_list_order\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_push_attr_three_item_list_order\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed to aid automated checking
    return (passed == total) ? 0 : 1;
}
// Unit test suite for the focal IDAT_list_extend function (reproduced in-test)
// This test is written for a C-like environment adapted to C++11.
// It does not rely on GoogleTest; instead a small in-house testing harness is used.

#include <setjmp.h>
#include <string.h>
#include <climits>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Minimal in-test reproduction of the dependency surface used by IDAT_list_extend.
// This mirrors the relevant parts of the original chroma where IDAT_list is defined.

// Structure representing a node in the IDAT list.
struct IDAT_list {
    IDAT_list *next;
    unsigned int length;
};

// Forward declaration of helper used by the function under test.
unsigned int IDAT_list_size(struct IDAT_list *list, unsigned int length);

// Macros reproduced from the original file to match behavior.
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define voidcast(t, v) ((t)(v))

// Local in-test implementation of IDAT_list_size.
// In the real project, this would compute the allocation size for a list node
// (potentially including trailing buffers). For testing purposes, we only
// need to allocate space for the IDAT_list struct itself.
unsigned int IDAT_list_size(struct IDAT_list */*list*/, unsigned int /*length*/) {
    return sizeof(struct IDAT_list);
}

// In-test reproduction of the focal method under test, copied from the prompt.
// It uses the same logic and side effects as the original function.
struct IDAT_list* IDAT_list_extend(struct IDAT_list *tail) {
{
   struct IDAT_list *next = tail->next;
   if (next == NULL)
   {
      unsigned int length = 2 * tail->length;
      if (length < tail->length) /* arithmetic overflow */
         length = tail->length;
      next = voidcast(IDAT_list*, malloc(IDAT_list_size(NULL, length)));
      CLEAR(*next);
      /* The caller must handle this: */
      if (next == NULL)
         return NULL;
      next->next = NULL;
      next->length = length;
      tail->next = next;
   }
   return next;
}
}

// Small, lightweight test harness (non-terminating assertions).
static int tests_run = 0;
static int tests_failed = 0;

// ASSERT-like macro that records failures but does not abort execution.
#define EXPECT_TRUE(cond, msg) do {                          \
    ++tests_run;                                             \
    if (!(cond)) {                                           \
        ++tests_failed;                                      \
        fprintf(stderr, "Test failure: %s\n", msg);          \
    }                                                        \
} while (0)

// Test 1: When tail->next is NULL, IDAT_list_extend should allocate a new node
// with length equal to 2 * tail->length (unless overflow occurs, in which case it caps
// to tail->length). The new node's next should be NULL and its length should reflect
// the doubling logic.
static void test_extend_allocates_new_node_and_doubles_length() {
    // Arrange
    struct IDAT_list tail;
    tail.next = NULL;
    tail.length = 8; // normal case to double to 16

    // Act
    struct IDAT_list *result = IDAT_list_extend(&tail);

    // Assert
    EXPECT_TRUE(result != NULL, "Allocation of new IDAT_list node failed when tail->next was NULL.");
    EXPECT_TRUE(tail.next == result, "tail->next should point to the newly allocated node.");
    EXPECT_TRUE(result->length == 16, "New node length should be double the tail length (16).");
    EXPECT_TRUE(result->next == NULL, "New node's next should be NULL.");
    EXPECT_TRUE(tail.next == result, "tail->next should reference the result node (confirming linkage).");

    // Cleanup
    free(result);
    tail.next = NULL;
}

// Test 2: When tail->next is already non-NULL, IDAT_list_extend should return tail->next
// without performing any allocation or modification to the existing chain.
static void test_extend_reuses_existing_next_without_allocation() {
    // Arrange
    struct IDAT_list tail;
    tail.next = (struct IDAT_list*)malloc(sizeof(struct IDAT_list));
    tail.next->next = NULL;
    tail.next->length = 5;
    tail.length = 3;

    // Act
    struct IDAT_list *result = IDAT_list_extend(&tail);

    // Assert
    EXPECT_TRUE(result == tail.next, "Should return existing tail->next when it's already non-NULL.");

    // Cleanup
    free(tail.next);
    tail.next = NULL;
}

// Test 3: Overflow handling path where tail->length is UINT_MAX.
// In this case, 2 * tail->length overflows; the code should cap length to tail->length.
static void test_extend_handles_overflow_guard() {
    // Arrange
    struct IDAT_list tail;
    tail.next = NULL;
    tail.length = UINT_MAX;

    // Act
    struct IDAT_list *result = IDAT_list_extend(&tail);

    // Assert
    EXPECT_TRUE(result != NULL, "Allocation should succeed even in overflow-safe path.");
    EXPECT_TRUE(result->length == UINT_MAX, "Length should be capped to UINT_MAX on overflow.");

    // Cleanup
    free(result);
    tail.next = NULL;
}

// Main function to execute tests.
int main(void) {
    test_extend_allocates_new_node_and_doubles_length();
    test_extend_reuses_existing_next_without_allocation();
    test_extend_handles_overflow_guard();

    printf("Tests run: %d, Failures: %d\n", tests_run, tests_failed);
    return (tests_failed == 0) ? 0 : 1;
}
// Comprehensive C++11 unit tests for the focal function rechunk_length
// The tests implement minimal C-structs and a C-style rechunk_length
// to exercise the logic of rechunk_length in isolation without depending on libpng.
//
// The test suite uses a tiny non-terminating assertion approach:
// each test reports pass/fail and continues execution to maximize coverage.

#include <cstdint>
#include <cassert>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


// Domain-aligned typedefs to mirror the original C code
typedef unsigned int png_uint_32;

// Forward declare to allow C linkage for the focal function
extern "C" {
   struct global;
   struct IDAT_list;
   struct IDAT;
   png_uint_32 rechunk_length(struct IDAT *idat, int start);
}

// Minimal C-structs mirroring the dependencies of rechunk_length

struct IDAT_list {
    unsigned int count;
    png_uint_32 *lengths; // array of length values, size = count
    struct IDAT_list *next;
};

struct global {
    png_uint_32 idat_max;
};

struct IDAT {
    struct global *global;
    png_uint_32 idat_length;
    png_uint_32 idat_max;
    struct IDAT_list *idat_cur;
    unsigned int idat_count;
    unsigned int idat_index;
    struct IDAT_list *idat_list_tail;
};

// The focal function copied from the provided source, adapted for C++11 linkage.
extern "C" png_uint_32 rechunk_length(struct IDAT *idat, int start)
{
{
   png_uint_32 len = idat->global->idat_max;
   if (len == 0) /* use original chunk lengths */
   {
      const struct IDAT_list *cur;
      unsigned int count;
      if (start)
         return idat->idat_length; /* use the cache */
      /* Otherwise rechunk_length is called at the end of a chunk for the length
       * of the next one.
       */
      cur = idat->idat_cur;
      count = idat->idat_count;
      assert(idat->idat_index == idat->idat_length &&
         idat->idat_length == cur->lengths[count]);
      /* Return length of the *next* chunk */
      if (++count < cur->count)
         return cur->lengths[count];
      /* End of this list */
      assert(cur != idat->idat_list_tail);
      cur = cur->next;
      assert(cur != NULL && cur->count > 0);
      return cur->lengths[0];
   }
   else /* rechunking */
   {
      /* The chunk size is the lesser of file->idat_max and the number
       * of remaining bytes.
       */
      png_uint_32 have = idat->idat_length - idat->idat_index;
      if (len > have)
      {
         struct IDAT_list *cur = idat->idat_cur;
         unsigned int j = idat->idat_count+1; /* the next IDAT in the list */
         do
         {
            /* Add up the remaining bytes.  This can't overflow because the
             * individual lengths are always <= 0x7fffffff, so when we add two
             * of them overflow is not possible.
             */
            assert(cur != NULL);
            for (;;)
            {
               /* NOTE: IDAT_list::count here, not IDAT_list::length */
               for (; j < cur->count; ++j)
               {
                  have += cur->lengths[j];
                  if (len <= have)
                     return len;
               }
               /* If this was the end return the count of the available bytes */
               if (cur == idat->idat_list_tail)
                  return have;
               cur = cur->next;
               j = 0;
            }
         }
         while (len > have);
      }
      return len;
   }
}
}

// Tiny test harness utilities for non-terminating assertions
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        printf("TEST FAIL: %s\n", msg); \
    } \
} while (0)

///////////////////// Test Helpers /////////////////////

static IDAT_list* make_list(unsigned int count, const png_uint_32* lengths) {
    IDAT_list* node = (IDAT_list*)std::malloc(sizeof(IDAT_list));
    node->count = count;
    if (count > 0) {
        node->lengths = (png_uint_32*)std::malloc(count * sizeof(png_uint_32));
        for (unsigned int i = 0; i < count; ++i) {
            node->lengths[i] = lengths[i];
        }
    } else {
        node->lengths = NULL;
    }
    node->next = NULL;
    return node;
}

static void free_list(IDAT_list* list) {
    if (!list) return;
    if (list->lengths) std::free(list->lengths);
    std::free(list);
}

static global* make_global(png_uint_32 max) {
    global* g = (global*)std::malloc(sizeof(global));
    g->idat_max = max;
    return g;
}

static IDAT* make_idat(global* g) {
    IDAT* id = (IDAT*)std::malloc(sizeof(IDAT));
    id->global = g;
    id->idat_length = 0;
    id->idat_max = 0;
    id->idat_cur = NULL;
    id->idat_count = 0;
    id->idat_index = 0;
    id->idat_list_tail = NULL;
    return id;
}

static void free_idat(IDAT* idat) {
    if (!idat) return;
    // Free the current list chain
    IDAT_list* cur = idat->idat_cur;
    while (cur) {
        IDAT_list* nxt = cur->next;
        free_list(cur);
        cur = nxt;
    }
    std::free(idat);
}

// Helper to attach a chain: idat_cur points to head; idat_list_tail points to last
// Requires that the head's next points to the subsequent list, etc.
static void attach_chain(IDAT* idat, IDAT_list* head, IDAT_list* tail) {
    idat->idat_cur = head;
    idat->idat_list_tail = tail;
    // For the invariant used by tests (when used in len==0 branch):
    // There must be at least one list in the chain if tail != NULL
}

///////////////////// Test Cases /////////////////////

static void test_case_end_of_current_list() {
    // Case: len == 0 (idat_max == 0) using original chunk lengths
    // Build first list with 3 elements: [10, 20, 30]
    // Build second (tail) list with 1 element: [5]
    // idat_length must equal cur->lengths[count] where count = idat_count
    // We set idat_count = 2 so cur->lengths[2] == 30
    global* g = make_global(0); // idat_max = 0
    unsigned png_lengths1[3] = {10, 20, 30};
    IDAT_list* head = make_list(3, png_lengths1);
    unsigned png_lengths2[1] = {5};
    IDAT_list* tail = make_list(1, png_lengths2);
    head->next = tail;

    IDAT* idat = make_idat(g);
    idat->idat_length = 30;
    idat->idat_max = 0;
    idat->idat_cur = head;
    idat->idat_count = 2;   // so that cur->lengths[count] == idat_length
    idat->idat_index = 30;  // equality check: idat_index == idat_length
    idat->idat_list_tail = tail;

    png_uint_32 res = rechunk_length(idat, 0);
    TEST_ASSERT(res == 5, "End of current list: expected next list's first length (5)");

    // Cleanup
    free_list(head);
    free_list(tail);
    free_idat(idat);
    free(g);
}

static void test_case_cache_start_true() {
    // Case: start == 1 uses cache (idat_length)
    global* g = make_global(0);
    IDAT* idat = make_idat(g);
    idat->idat_length = 1234;
    idat->idat_max = 0;
    idat->idat_cur = NULL;
    idat->idat_list_tail = NULL;
    idat->idat_length = 1234;
    idat->idat_index = 0;
    idat->idat_count = 0;

    png_uint_32 res = rechunk_length(idat, 1);
    TEST_ASSERT(res == 1234, "Cache path: start==1 should return idat_length (1234)");

    free_idat(idat);
    free(g);
}

static void test_case_len_le_have_branch() {
    // Case: rechunking when len <= have -> return len immediately
    // Set idat_max = 8, idat_length - idat_index = 12 (have)
    // Expect rechunk_length to return len (8)
    global* g = make_global(8);
    IDAT* idat = make_idat(g);
    idat->idat_length = 12;
    idat->idat_index = 0;
    idat->idat_cur = NULL;
    idat->idat_list_tail = NULL;
    idat->idat_count = 0;
    idat->idat_max = 8;

    png_uint_32 res = rechunk_length(idat, 0);
    TEST_ASSERT(res == 8, "Len <= Have path: should return idat_max (8)");

    free_idat(idat);
    free(g);
}

static void test_case_len_gt_have_two_lists() {
    // Case: rechunking across multiple lists; len > have triggers traversal into list elements
    // idat_max = 50; idat_length = 12; idat_index = 0 -> have = 12
    // First list: lengths [0, 20, 20, 20] (count=4)
    // Expect after adding lengths[1] = 20, then lengths[2] = 20 to exceed 50 -> return 50
    global* g = make_global(50);
    IDAT* idat = make_idat(g);
    idat->idat_length = 12;
    idat->idat_index = 0;
    idat->idat_cur = NULL;
    idat->idat_list_tail = NULL;
    idat->idat_max = 50;
    idat->idat_count = 0; // j starts at 1
    // Build first list
    png_uint_32 l1[] = {0, 20, 20, 20}; // count = 4
    IDAT_list* cur1 = make_list(4, l1);
    // No tail in this test (we won't reach tail)
    idat->idat_cur = cur1;
    idat->idat_list_tail = cur1;
    // Call
    png_uint_32 res = rechunk_length(idat, 0);
    TEST_ASSERT(res == 50, "Len > Have with multiple lengths: should return 50");

    // Cleanup
    free_list(cur1);
    free_idat(idat);
    free(g);
}

static void test_case_tail_have_return() {
    // Case: reach end of current list and return cumulative "have" from tail
    // Build two lists: first (cur) with a small value, tail with two values
    // idat_max = 70; idat_length = 30; idat_index = 0 -> have=30
    // First list: [0, 2] -> adds 2 -> have=32
    // Tail: [5,5] -> adds 5 then 5 -> have=42; since len(70) > have, and cur==tail, return 42
    global* g = make_global(70);
    IDAT* idat = make_idat(g);
    idat->idat_length = 30;
    idat->idat_index = 0;
    idat->idat_cur = NULL;
    idat->idat_list_tail = NULL;
    idat->idat_max = 70;
    idat->idat_count = 0;

    // First list
    unsigned l1[] = {0, 2}; // count 2
    IDAT_list* cur = make_list(2, l1);

    // Tail list
    unsigned l2[] = {5, 5}; // count 2
    IDAT_list* tail = make_list(2, l2);
    // Link lists
    cur->next = tail;

    idat->idat_cur = cur;
    idat->idat_list_tail = tail;
    idat->idat_length = 30;

    png_uint_32 res = rechunk_length(idat, 0);
    TEST_ASSERT(res == 42, "Tail path: should return cumulative have value (42)");

    // Cleanup
    free_list(cur);
    free_list(tail);
    free_idat(idat);
    free(g);
}

///////////////////// Runner /////////////////////

static void run_all_tests() {
    test_case_end_of_current_list();
    test_case_cache_start_true();
    test_case_len_le_have_branch();
    test_case_len_gt_have_two_lists();
    test_case_tail_have_return();

    // Summary
    printf("Tests run: %d, Passed: %d, Failed: %d\n",
           g_tests_passed + g_tests_failed,
           g_tests_passed,
           g_tests_failed);
}

int main(void) {
    run_all_tests();
    return (g_tests_failed == 0) ? 0 : 1;
}
// Unit test suite for sqlite3VdbeLinkSubProgram
// Purpose: verify linking a SubProgram into the Vdbe's program list (a simple stack/linked-list insertion).
// Approach: use the real type definitions from the project's headers to ensure correct layout.
// Note: This test uses non-terminating expectations (they print failures but do not abort).

#include <cstdlib>
#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>


// Lightweight non-terminating assertion helpers.
// They report failures but do not stop test execution, enabling higher coverage.
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
  if((void*)(a) != (void*)(b)) { \
    std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << (void*)(a) << ") != " #b " (" << (void*)(b) << ") at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_failures; \
  } \
} while(0)

// Import necessary dependencies with correct C linkage so that the
// sqlite3VdbeLinkSubProgram function and the involved types are visible.
// This relies on the project providing vdbeInt.h/sqliteInt.h as in the focal class dependencies.
extern "C" {
}

// The test suite exercises the following scenarios for sqlite3VdbeLinkSubProgram:
//   1) Linking into an empty program list (pProgram is NULL).
//   2) Linking into a non-empty program list (head insertion semantics).
//   3) Multiple successive insertions to ensure the list grows correctly (LIFO order).

// Test 1: Linking into an empty program list.
//   Expectation: after linking, pVdbe->pProgram points to the newly linked SubProgram
//   and the new SubProgram's pNext is NULL.
static void test_link_subprogram_basic_null_head() {
  Vdbe v;
  v.pProgram = NULL; // empty list

  SubProgram *p = (SubProgram*)malloc(sizeof(SubProgram));
  // Initialize the node; only pNext is required by the function under test.
  p->pNext = NULL;

  sqlite3VdbeLinkSubProgram(&v, p);

  // Verify that the new sub-program is now the head and has no next.
  EXPECT_PTR_EQ(v.pProgram, p);
  EXPECT_PTR_EQ(p->pNext, NULL);

  free(p);
}

// Test 2: Linking into a non-empty program list (head insertion).
//   Before: head points to an existing SubProgram 'head'.
//   After: the new SubProgram 'p' becomes the head and its pNext points to the old head.
static void test_link_subprogram_head_overwrites() {
  Vdbe v;

  SubProgram *head = (SubProgram*)malloc(sizeof(SubProgram));
  head->pNext = NULL;

  v.pProgram = head; // existing list with a single head node

  SubProgram *p = (SubProgram*)malloc(sizeof(SubProgram));
  p->pNext = NULL;

  sqlite3VdbeLinkSubProgram(&v, p);

  // Assertions: new head is 'p', p->pNext points to old head, old head untouched otherwise.
  EXPECT_PTR_EQ(v.pProgram, p);
  EXPECT_PTR_EQ(p->pNext, head);
  EXPECT_PTR_EQ(head->pNext, NULL);

  free(head);
  free(p);
}

// Test 3: Multiple successive insertions to verify full chain integrity.
//   Sequence: insert a, then b, then c. Final chain should be c -> b -> a -> NULL.
static void test_link_subprogram_triple_chain() {
  Vdbe v;
  v.pProgram = NULL; // start with empty

  SubProgram *a = (SubProgram*)malloc(sizeof(SubProgram));
  a->pNext = NULL;

  SubProgram *b = (SubProgram*)malloc(sizeof(SubProgram));
  b->pNext = NULL;

  SubProgram *c = (SubProgram*)malloc(sizeof(SubProgram));
  c->pNext = NULL;

  // First insert 'a'
  sqlite3VdbeLinkSubProgram(&v, a);
  // Then insert 'b'
  sqlite3VdbeLinkSubProgram(&v, b);
  // Finally insert 'c'
  sqlite3VdbeLinkSubProgram(&v, c);

  // Expect chain: c -> b -> a -> NULL
  EXPECT_PTR_EQ(v.pProgram, c);
  EXPECT_PTR_EQ(c->pNext, b);
  EXPECT_PTR_EQ(b->pNext, a);
  EXPECT_PTR_EQ(a->pNext, NULL);

  free(a);
  free(b);
  free(c);
}

// Entry point for the test suite.
// Runs all tests and reports a final summary.
// The DOMAIN_KNOWLEDGE requests non-terminating assertions; we implement that behavior.
// This main function is invoked directly from the environment (no gtest/gmock).
int main() {
  test_link_subprogram_basic_null_head();
  test_link_subprogram_head_overwrites();
  test_link_subprogram_triple_chain();

  if (g_failures == 0) {
    std::cout << "All tests passed\n";
    return 0;
  } else {
    std::cerr << g_failures << " test(s) failed\n";
    return 1;
  }
}
/*
  Unit test suite for the focal method:
  gen_ncmp(compiler_state_t *cstate, enum e_offrel offrel, u_int offset,
           u_int size, bpf_u_int32 mask, int jtype, int reverse,
           bpf_u_int32 v)

  This test harness uses GCC's linker wrap facility (--wrap) to provide
  test doubles for the dependent public functions:
    - gen_load_a
    - new_stmt
    - sappend
    - gen_jmp_k
    - gen_not

  The wrappers record important state so we can assert:
    - The load parameters (offrel, offset, size) were passed through.
    - When mask != 0xffffffff a new statement with s.k == mask was created.
    - gen_jmp_k was invoked with the expected jtype and v and the statement list.
    - Reverse mode triggers a call to gen_not.

  Build notes:
  Compile with something like:
    g++ -std=c++11 -O0 -Wall -Wextra -Wno-sign-compare \
        test_gen_ncmp.cpp \
        -o test_gen_ncmp \
        -Wl,--wrap=gen_load_a \
        -Wl,--wrap=new_stmt \
        -Wl,--wrap=sappend \
        -Wl,--wrap=gen_jmp_k \
        -Wl,--wrap=gen_not
  This ensures the tests hit the test doubles defined here.
  If your environment uses a different naming convention for wraps, adjust accordingly.

  Important: This test suite relies on the following simplified type stubs
  that mirror the local expectations of the focal method and its usage
  inside gencode.c. They do not attempt to recreate the entire original
  environment; they only provide enough structure for unit testing the
  behavior of gen_ncmp.

  Note: We avoid terminating assertions; instead, we count failures and report
  a final summary to satisfy "non-terminating assertions" guidance.
*/

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Step A: Minimal type stubs to align with gen_ncmp signature expectations.
// We intentionally keep these as forward declarations to allow testing the
// interaction via wrapper functions (when building with --wrap).

// Public types used by gen_ncmp (as seen in the focal method)
typedef unsigned int u_int;
typedef unsigned int bpf_u_int32;
typedef struct compiler_state_t compiler_state_t;

// Forward declare the block and slist types used by gen_ncmp and wrappers.
typedef struct block block;
typedef struct slist slist;

// Enum placeholder for offrel (actual values are not important for the tests)
enum e_offrel { OFFREL_ABS = 0, OFFREL_REL = 1 };

// Conservative forward declaration of the focal function under test.
// The actual implementation lives in gencode.c; we rely on wraps to provide
// test doubles for its dependencies.
extern "C" struct block* gen_ncmp(compiler_state_t *cstate,
                                  enum e_offrel offrel,
                                  u_int offset,
                                  u_int size,
                                  bpf_u_int32 mask,
                                  int jtype,
                                  int reverse,
                                  bpf_u_int32 v);

/*
  Test doubles (wrappers) for the dependent functions used by gen_ncmp.

  We rely on GCC's linker wrap feature (--wrap=NAME). The real calls inside
  gen_ncmp will be redirected to __wrap_NAME. Our wrappers provide
  controlled behavior and capture internal state for assertions.
*/

// Allocation helpers to implement a minimal slist and block
struct slist {
  slist *next;
  struct { unsigned int k; } s;
};

struct block {
  int dummy;
};

// Global test state for assertions in each test
static enum e_offrel g_last_load_offrel = OFFREL_ABS;
static unsigned int g_last_load_offset = 0;
static unsigned int g_last_load_size = 0;

static slist *g_last_new_stmt = nullptr;
static slist *g_last_sappend_left = nullptr;
static slist *g_last_sappend_right = nullptr;

static block *g_last_jmp_k_block = nullptr;
static int g_last_jmp_k_jtype = -1;
static unsigned int g_last_jmp_k_v = 0;
static slist *g_last_jmp_k_stmts = nullptr;

static int g_not_called_count = 0;

// Prototypes for wrappers (as needed by the test harness)
extern "C" {
  // protos for wrap hooks (actual wraps will rename these to __wrap_*)
  slist* __wrap_gen_load_a(compiler_state_t *cstate,
                           enum e_offrel offrel, unsigned int offset, unsigned int size);

  slist* __wrap_new_stmt(compiler_state_t *cstate, int code);

  void __wrap_sappend(slist *s0, slist *s1);

  block* __wrap_gen_jmp_k(compiler_state_t *cstate,
                          int jtype, unsigned int v,
                          slist *stmts);

  block* __wrap_gen_not(block *b);
}

// Implementations of wrappers to emulate the dependencies
extern "C" {

slist* __wrap_gen_load_a(compiler_state_t *cstate,
                         enum e_offrel offrel, unsigned int offset, unsigned int size)
{
  // Capture arguments for verification
  g_last_load_offrel = offrel;
  g_last_load_offset = offset;
  g_last_load_size = size;

  // Create a dummy slist node to represent the loaded A
  slist *s = (slist *)std::malloc(sizeof(slist));
  if (!s) {
    std::cerr << "Memory allocation failed in __wrap_gen_load_a" << std::endl;
    std::exit(1);
  }
  s->next = nullptr;
  s->s.k = 0;
  return s;
}

slist* __wrap_new_stmt(compiler_state_t *cstate, int code)
{
  // Create a new statement node; this will be modified by gen_ncmp to set k
  slist *s2 = (slist *)std::malloc(sizeof(slist));
  if (!s2) {
    std::cerr << "Memory allocation failed in __wrap_new_stmt" << std::endl;
    std::exit(1);
  }
  s2->next = nullptr;
  s2->s.k = 0;
  g_last_new_stmt = s2; // record the created statement for inspection
  return s2;
}

void __wrap_sappend(slist *s0, slist *s1)
{
  if (!s0) return;
  s0->next = s1;
  g_last_sappend_left = s0;
  g_last_sappend_right = s1;
}

block* __wrap_gen_jmp_k(compiler_state_t *cstate,
                        int jtype, unsigned int v,
                        slist *stmts)
{
  // Record the jump target and arguments
  g_last_jmp_k_block = (block *)std::malloc(sizeof(block));
  g_last_jmp_k_block->dummy = 1;
  g_last_jmp_k_jtype = jtype;
  g_last_jmp_k_v = v;
  g_last_jmp_k_stmts = stmts;
  return g_last_jmp_k_block;
}

block* __wrap_gen_not(block *b)
{
  // Indicate that a NOT operation was applied
  g_not_called_count++;
  // For simplicity, return the same block to mimic NOT behavior
  return b;
}
} // extern "C"

// Utility to reset test state between tests
static void reset_test_state()
{
  g_last_load_offrel = OFFREL_ABS;
  g_last_load_offset = 0;
  g_last_load_size = 0;

  g_last_new_stmt = nullptr;
  g_last_sappend_left = nullptr;
  g_last_sappend_right = nullptr;

  g_last_jmp_k_block = nullptr;
  g_last_jmp_k_jtype = -1;
  g_last_jmp_k_v = 0;
  g_last_jmp_k_stmts = nullptr;

  g_not_called_count = 0;
}

/* Lightweight assertion macro (non-terminating) */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
  if (cond) { \
    ++tests_passed; \
  } else { \
    ++tests_failed; \
    std::cerr << "Test failed: " << msg << " (line " << __LINE__ << ")" << std::endl; \
  } \
} while(0)

/* Test 1: When mask != 0xffffffff, a new statement is created and its k is set to mask,
   and the jmp_k is invoked with given jtype and v. */
static void test_mask_nonmax_creates_stmt_and_invokes_jmp()
{
  reset_test_state();

  // Prepare a dummy compiler_state_t instance (opaque to the test)
  static struct compiler_state_t dummy_state;
  compiler_state_t *cstate = &dummy_state;

  // Call the focal method with a non-default mask and known jtype/v
  const unsigned int offset = 4;
  const unsigned int size = 8;
  const bpf_u_int32 mask = 0x12345678;
  const int jtype = 2;
  const int reverse = 0;
  const bpf_u_int32 v = 0xdeadbeef;

  struct block *ret = gen_ncmp(cstate, OFFREL_ABS, offset, size, mask, jtype, reverse, v);

  // Assertions
  TEST_ASSERT(g_last_load_offrel == OFFREL_ABS, "load_offrel should be passed through");
  TEST_ASSERT(g_last_load_offset == offset, "load offset should match");
  TEST_ASSERT(g_last_load_size == size, "load size should match");

  TEST_ASSERT(g_last_new_stmt != nullptr, "new_stmt should be invoked when mask != 0xffffffff");
  TEST_ASSERT(g_last_new_stmt->s.k == mask, "new_stmt's k field should be set to mask");

  TEST_ASSERT(g_last_jmp_k_block != nullptr, "gen_jmp_k should be invoked and return a block");
  TEST_ASSERT(g_last_jmp_k_jtype == jtype, "gen_jmp_k should receive the correct jtype");
  TEST_ASSERT(g_last_jmp_k_v == v, "gen_jmp_k should receive the correct v");
  TEST_ASSERT(ret == g_last_jmp_k_block, "gen_ncmp should return the NOT if reverse is false (not applied) via gen_jmp_k return");
}

/* Test 2: When mask == 0xffffffff, no new statement is created. */
static void test_mask_max_no_new_stmt()
{
  reset_test_state();

  static struct compiler_state_t dummy_state;
  compiler_state_t *cstate = &dummy_state;

  const unsigned int offset = 1;
  const unsigned int size = 2;
  const bpf_u_int32 mask = 0xffffffff; // special value: no AND mask should be applied
  const int jtype = 1;
  const int reverse = 0;
  const bpf_u_int32 v = 0xabcdef;

  struct block *ret = gen_ncmp(cstate, OFFREL_REL, offset, size, mask, jtype, reverse, v);

  TEST_ASSERT(g_last_load_offrel == OFFREL_REL, "load_offrel should be passed through (REL variant)");
  TEST_ASSERT(g_last_load_offset == offset, "load offset should match");
  TEST_ASSERT(g_last_load_size == size, "load size should match");

  TEST_ASSERT(g_last_new_stmt == nullptr, "new_stmt should NOT be invoked when mask == 0xffffffff");
  TEST_ASSERT(g_last_jmp_k_block != nullptr, "gen_jmp_k should still be invoked even if mask is max");
  TEST_ASSERT(g_last_jmp_k_jtype == jtype, "gen_jmp_k should receive the correct jtype");
  TEST_ASSERT(g_last_jmp_k_v == v, "gen_jmp_k should receive the correct v");
  TEST_ASSERT(ret == g_last_jmp_k_block, "Return value should be the jmp_k block pointer (NOT path depends on reverse)");
}

/* Test 3: Reverse path triggers gen_not call. We verify gen_not was invoked. */
static void test_reverse_triggers_not()
{
  reset_test_state();

  static struct compiler_state_t dummy_state;
  compiler_state_t *cstate = &dummy_state;

  const unsigned int offset = 10;
  const unsigned int size = 16;
  const bpf_u_int32 mask = 0x0A0A0A0A;
  const int jtype = 3;
  const int reverse = 1;
  const bpf_u_int32 v = 0x1234;

  struct block *ret = gen_ncmp(cstate, OFFREL_ABS, offset, size, mask, jtype, reverse, v);

  TEST_ASSERT(g_last_jmp_k_block != nullptr, "gen_jmp_k should have produced a block before NOT");
  TEST_ASSERT(g_not_called_count > 0, "gen_not should have been invoked due to reverse == 1");
}

/* Test 4: Validate that offset, size and offrel pass-through do not alter return type
   and that the call chain is consistent for multiple parameter combinations. */
static void test_param_passthrough_various()
{
  reset_test_state();

  static struct compiler_state_t dummy_state;
  compiler_state_t *cstate = &dummy_state;

  // Different parameter combination
  const unsigned int offset = 7;
  const unsigned int size = 4;
  const bpf_u_int32 mask = 0x0; // zero mask (treated as non-maximum; still should create s with k=0)
  const int jtype = 4;
  const int reverse = 0;
  const bpf_u_int32 v = 0xBEEF;

  struct block *ret = gen_ncmp(cstate, OFFREL_REL, offset, size, mask, jtype, reverse, v);

  // Even with mask = 0, we expect a new_stmt to be created if mask != 0xffffffff
  // Here, mask is 0, so we expect a statement with k == 0 to be created.
  TEST_ASSERT(g_last_load_offrel == OFFREL_REL, "load_offrel should reflect OFFREL_REL");
  TEST_ASSERT(g_last_load_offset == offset, "load offset should reflect offset");
  TEST_ASSERT(g_last_load_size == size, "load size should reflect size");

  TEST_ASSERT(g_last_new_stmt != nullptr, "new_stmt should be invoked for mask (0 is not max val)");
  TEST_ASSERT(g_last_new_stmt->s.k == mask, "new_stmt's k field should be set to 0 as provided");

  TEST_ASSERT(ret == g_last_jmp_k_block, "Return value should be the jmp_k block pointer");
}
int main()
{
  // Run all tests
  test_mask_nonmax_creates_stmt_and_invokes_jmp();
  test_mask_max_no_new_stmt();
  test_reverse_triggers_not();
  test_param_passthrough_various();

  // Summary
  std::cout << "Gen_ncmp unit tests completed." << std::endl;
  std::cout << "Passed: " << tests_passed << ", Failed: " << tests_failed << std::endl;
  return (tests_failed ? 1 : 0);
}

/*
  Step 3 notes (domain knowledge):
  - These tests exercise the true/false branches of the conditional in gen_ncmp
    (whether mask equals 0xffffffff or not).
  - They verify that side effects on the dependent helpers (gen_load_a, new_stmt,
    sappend, gen_jmp_k, gen_not) are observable via captured global state.
  - Static members (if any) in the focal library would be accessed via the class name;
    here we demonstrate access through wrappers and static test state.
  - We rely on C-style behavior for memory management in test doubles (malloc/free),
    keeping test doubles isolated from the actual library's internal memory management.
  - We do not mock only private details; instead, we exercise public entry (gen_ncmp) and
    observe through the wrappers.
  - This test environment is designed for C++11, without GTest; a simple main() drives tests
    and prints a summary.
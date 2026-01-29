/*
  Self-contained unit tests for the focal method sdup, adapted to C++11 without
  using Google Test. The tests include minimal mock implementations of the
  external dependencies expected by sdup (notably newchunk_nolongjmp and
  pcapint_strlcpy) and verify both success and failure paths, as well as
  proper handling of multiple calls.

  Test execution model:
  - Non-terminating assertions: tests continue executing after a failed assertion,
    and a global counter summarizes failures at the end.
  - All tests are invoked from main as plain function calls.
  - Static-like helpers and state are kept within this single translation unit.

  Notes:
  - The tests are self-contained and do not rely on the original gencode.c or any
    external project build system.
  - The code mirrors the logic of the provided sdup function:
      sdup(cstate, s) -> allocate n = strlen(s)+1, copy via pcapint_strlcpy, return cp
*/

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
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


// Minimal representations of the dependencies needed by sdup

struct compiler_state_t {
    bool fail_alloc;   // when true, newchunk_nolongjmp should fail (simulate OOM)
    int allocated;      // how many times a chunk has been allocated
};

// Forward declarations to mimic the original environment
char* newchunk_nolongjmp(compiler_state_t *cstate, size_t n);
size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize);

// The focal method under test
char* sdup(compiler_state_t *cstate, const char *s)
{
{
    size_t n = strlen(s) + 1;
    char *cp = newchunk_nolongjmp(cstate, n);
    if (cp == NULL)
        return (NULL);
    pcapint_strlcpy(cp, s, n);
    return (cp);
}
}

// Mocked allocation routine (simulates a non-throwing allocator or NULL on failure)
char* newchunk_nolongjmp(compiler_state_t *cstate, size_t n)
{
    if (cstate == nullptr)
        return nullptr;
    if (cstate->fail_alloc)
        return nullptr; // Simulate allocation failure
    cstate->allocated += 1;
    return (char*)malloc(n);
}

// Safe string copy helper (emulates pcapint_strlcpy behavior)
size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize)
{
    if (dstsize == 0)
        return 0;
    size_t srclen = strlen(src);
    size_t copylen = (srclen < dstsize - 1) ? srclen : dstsize - 1;
    // Use memcpy to ensure deterministic behavior for tests
    memcpy(dst, src, copylen);
    dst[copylen] = '\0';
    return copylen;
}

// Lightweight test framework (non-terminating assertions)

static int test_failures = 0;

#define ASSERT_TRUE(x) do { \
    if (!(x)) { \
        printf("Assertion failed: %s at %s:%d\n", #x, __FILE__, __LINE__); \
        ++test_failures; \
    } \
} while (0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf("Assertion failed: %s == %s (%lld != %lld) at %s:%d\n", #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
        ++test_failures; \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b) do { \
    if ((a) == NULL || (b) == NULL) { \
        printf("Assertion failed: NULL string comparison for %s vs %s at %s:%d\n", (a ? a : "NULL"), (b ? b : "NULL"), __FILE__, __LINE__); \
        ++test_failures; \
    } else if (strcmp((a), (b)) != 0) { \
        printf("Assertion failed: strings differ: \"%s\" vs \"%s\" at %s:%d\n", (a), (b), __FILE__, __LINE__); \
        ++test_failures; \
    } \
} while (0)

// Test cases

// Test 1: Basic successful duplication of a non-empty string
// - Expect a non-NULL result
// - The content should match the input string
// - Allocation should have occurred exactly once
void test_sdup_basic()
{
    compiler_state_t cs = {false, 0};
    const char *input = "hello, world";
    char *out = sdup(&cs, input);
    ASSERT_TRUE(out != NULL);
    if (out != NULL) {
        ASSERT_STR_EQ(out, input);
        free(out);
    }
    ASSERT_EQ(cs.allocated, 1);
}

// Test 2: Empty string handling
// - Should allocate space for the empty string (null terminator)
// - Content should be an empty string
// - Allocation should occur exactly once
void test_sdup_empty()
{
    compiler_state_t cs = {false, 0};
    const char *input = "";
    char *out = sdup(&cs, input);
    ASSERT_TRUE(out != NULL);
    if (out != NULL) {
        ASSERT_STR_EQ(out, input);
        free(out);
    }
    ASSERT_EQ(cs.allocated, 1);
}

// Test 3: Allocation failure path
// - Simulate allocation failure via the cstate flag
// - Expect NULL return and no allocation recorded
void test_sdup_alloc_failure()
{
    compiler_state_t cs = {true, 0}; // Simulate allocation failure
    const char *input = "failure case";
    char *out = sdup(&cs, input);
    ASSERT_TRUE(out == NULL);
    ASSERT_EQ(cs.allocated, 0);
}

// Test 4: Multiple allocations across successive calls
// - Ensure consecutive sdup calls allocate independent chunks
// - Verify contents and allocation count reflects multiple allocations
void test_sdup_multiple_calls()
{
    compiler_state_t cs = {false, 0};
    const char *a = "a";
    const char *b = "bb";

    char *out1 = sdup(&cs, a);
    char *out2 = sdup(&cs, b);

    ASSERT_TRUE(out1 != NULL && out2 != NULL);
    if (out1 != NULL) {
        ASSERT_STR_EQ(out1, a);
        free(out1);
    }
    if (out2 != NULL) {
        ASSERT_STR_EQ(out2, b);
        free(out2);
    }
    ASSERT_EQ(cs.allocated, 2);
}

// Runner
void run_all_tests()
{
    test_sdup_basic();
    test_sdup_empty();
    test_sdup_alloc_failure();
    test_sdup_multiple_calls();
}

// Entry point
int main()
{
    run_all_tests();
    if (test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", test_failures);
        return 1;
    }
}
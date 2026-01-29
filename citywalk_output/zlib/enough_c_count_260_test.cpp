/*
  Unit test suite for the focal method: big_t count(int syms, int left, int len)
  This test harness provides a minimal, self-contained environment to exercise
  the function's behavior, including base cases, caching, and an overflow path.

  Notes:
  - We implement a lightweight emulation of the global state 'g', the type aliases
    big_t and code_t, and a deterministic map() to index into g.num.
  - The tests are written without Google Test (as required). A tiny EXPECT/ASSERT
    style framework is provided to drive and report tests.
  - Tests cover:
    1) base case (syms == left)
    2) caching path (use of g.num[index])
    3) overflow path triggered by a deeper short-circuit (inner -1)
    4) an additional base-case with len==0 to ensure len boundary is respected
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cassert>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <stdlib.h>


// Domain-specific types replicated for testing
using big_t = long long;
using code_t = unsigned long long;

// Global-like state (emulating 'g' from the focal file)
struct GlobalState {
    int max;                 // maximum code length allowed
    std::vector<big_t> num;  // memoization array (initialized to 0)
} g;

// Simple, deterministic mapping from (syms,left,len) to an index in g.num
static inline size_t map(int syms, int left, int len) {
    // Deterministic, repeatable indexing suitable for tests
    // Assumes tests only explore a small, bounded domain
    return static_cast<size_t>(syms) * 1000u + static_cast<size_t>(left) * 31u + static_cast<size_t>(len);
}

// The focal function under test (reproduced in the test harness)
static big_t count(int syms, int left, int len) {
    if (syms == left)
        return 1;
    // In real code, this is an assertion guard; tests use inputs to satisfy it
    assert(syms > left && left > 0 && len < g.max);
    size_t index = map(syms, left, len);
    big_t got = g.num[index];
    if (got)
        return got;         // cached result

    int least = (left << 1) - syms;
    if (least < 0)
        least = 0;

    int most = (((code_t)left << (g.max - len)) - syms) /
               (((code_t)1 << (g.max - len)) - 1);

    big_t sum = 0;
    for (int use = least; use <= most; use++) {
        got = count(syms - use, (left - use) << 1, len + 1);
        sum += got;
        if (got == (big_t)-1 || sum < got)      // overflow
            return (big_t)-1;
    }
    // verify that all recursive calls are productive
    assert(sum != 0);
    // save the result and return it
    g.num[index] = sum;
    return sum;
}

// Lightweight test framework (non-terminating)
static int tests_run = 0;
static int tests_pass = 0;

#define EXPECT_TRUE(cond, msg) do { \
    tests_run++; \
    if (cond) { tests_pass++; } \
    else { fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg); } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    tests_run++; \
    if (static_cast<long long>(a) == static_cast<long long>(b)) { tests_pass++; } \
    else { fprintf(stderr, "EXPECT_EQ FAILED: %s (expected %lld, got %lld)\n", msg, (long long)(b), (long long)(a)); } \
} while (0)

static void reset_g(int max_value) {
    g.max = max_value;
    // allocate a generous memo table; tests only touch a few indices
    g.num.assign(100000, 0);
}

int main() {
    // Test 1: base case where syms == left should return 1 (no recursion)
    {
        reset_g(4);
        big_t r = count(3, 3, 1);  // syms == left
        EXPECT_EQ(r, (big_t)1, "base case: count(3,3,1) should return 1 when syms==left");
    }

    // Test 2: caching path - first computation fills g.num; second call uses cache
    {
        reset_g(4);
        big_t r1 = count(3, 2, 1);  // not base case; computes a value
        size_t idx = map(3, 2, 1);
        big_t cached = g.num[idx];
        EXPECT_TRUE(cached != 0, "cache should be populated after first count(3,2,1)");
        big_t r2 = count(3, 2, 1); // should hit cache
        EXPECT_EQ(r2, r1, "second call should return the cached value, matching first result");
    }

    // Test 3: overflow path propagation via inner -1 result
    // Setup: ensure inner call (syms=3,left=2,len=2) yields -1 by pre-setting g.num[index] = -1
    // Then calling count(4,2,1) will first recurse to (4,4,2)=1, then to (3,2,2) returning -1,
    // triggering the overflow condition and returning -1.
    {
        reset_g(4);
        // Pre-fill inner state to force -1 when queried
        size_t inner_idx = map(3, 2, 2);
        g.num[inner_idx] = -1;

        big_t r = count(4, 2, 1);
        EXPECT_EQ(r, (big_t)-1, "overflow path should return -1 when a deeper call yields -1");
    }

    // Test 4: additional base case with len boundary (syms == left and len can be 0)
    {
        reset_g(4);
        big_t r = count(5, 5, 0);  // syms == left, len can be 0
        EXPECT_EQ(r, (big_t)1, "base case with len==0 should return 1 when syms==left");
    }

    // Summary
    printf("Tests run: %d, Passed: %d, Failed: %d\n", tests_run, tests_pass, tests_run - tests_pass);
    if (tests_run == tests_pass) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}
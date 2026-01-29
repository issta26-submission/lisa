#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cstdarg>
#include <cassert>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>


// Lightweight, self-contained test harness for the focal method "examine"
// The goal is to exercise the function in a C++11 environment without GTest.

// Domain-specific: provide minimal re-implementation of the dependencies used by examine

// Minimal string wrapper to mimic string_t in the original code
struct string_t {
    std::string s;
};

// Global structure to hold code/tables and output, mirroring the expected external g
struct Global {
    int root;
    int max;
    int large;
    int code[64];
    string_t out;
} g;

// Typedef used in original code for large bit-shift computations
typedef unsigned long long code_t;

// Stubbed utility functions (placeholders matching the dependency section)
static void string_clear(string_t *s) { s->s.clear(); }
static void string_init(string_t *s) { s->s.clear(); }
static void string_free(string_t *s) { /* no-op for test harness */ }

static void string_printf(string_t *s, const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    s->s += buf;
}

// Minimal been_here implementation: always false for focused test exploration
static int been_here(int syms, int left, int len, int mem, int rem)
{
    (void)syms; (void)left; (void)len; (void)mem; (void)rem;
    return 0;
}

// Forward declare examine to mirror the tested function's scope
static void examine(int syms, int left, int len, int mem, int rem);

// The focal method under test, ported to C++11 style static function
static void examine(int syms, int left, int len, int mem, int rem)
{
    if (syms == left) {
        // base case: complete code entry at this level
        g.code[len] = left;
        // complete computation of memory used by this code (optional in test paths)
        while (rem < left) {
            left -= rem;
            rem = 1 << (len - g.root);
            mem += rem;
        }
        assert(rem == left);
        if (mem >= g.large) {
            if (mem > g.large) {
                g.large = mem;
                string_clear(&g.out);
            }
            int syms2 = 0;
            // reuse 'syms' variable name inside scope as in original logic
            syms = 0;
            int left2 = 1 << g.max;
            for (int bits = g.max; bits > g.root; bits--) {
                syms += g.code[bits];
                left2 -= g.code[bits];
                assert((left2 & 1) == 0);
                left2 >>= 1;
            }
            string_printf(&g.out, "<%u, %u, %u>:", (unsigned)syms, (unsigned)(g.root + 1),
                          (unsigned)(((1 << g.root) - left2) << 1));
            for (int bits = g.root + 1; bits <= g.max; bits++)
                if (g.code[bits])
                    string_printf(&g.out, " %d[%d]", g.code[bits], bits);
            string_printf(&g.out, "\n");
        }
        g.code[len] = 0;
        return;
    }
    if (been_here(syms, left, len, mem, rem))
        return;
    int least = (left << 1) - syms;
    if (least < 0)
        least = 0;
    int most = (((code_t)left << (g.max - len)) - syms) /
               (((code_t)1 << (g.max - len)) - 1);
    int use = least;
    while (rem < use) {
        use -= rem;
        rem = 1 << (len - g.root);
        mem += rem;
    }
    rem -= use;
    for (use = least; use <= most; use++) {
        g.code[len] = use;
        examine(syms - use, (left - use) << 1, len + 1,
                mem + (rem ? 1 << (len - g.root) : 0), rem << 1);
        if (rem == 0) {
            rem = 1 << (len - g.root);
            mem += rem;
        }
        rem--;
    }
    g.code[len] = 0;
}

// Simple test harness scaffolding
static int failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << "\n"; \
        ++failures; \
    } \
} while (0)

// Helper to reset global testing environment
static void reset_globals()
{
    g.root = 0;
    g.max = 3;
    g.large = 0;
    std::memset(g.code, 0, sizeof(g.code));
    g.out.s.clear();
}

// Test case 1: Ensure that when syms == left and mem >= large, the function
// prints the computed sub-code to g.out and resets the temporary code entry.
static void test_case_print_subcode()
{
    reset_globals();
    // Prepare a scenario that triggers printing:
    // - root=0, max=3 so we compute a small tree
    g.root = 0;
    g.max = 3;
    g.large = 0;       // trigger print on mem >= large
    // Pre-fill code entries for bits [1..3] used in printing
    g.code[1] = 1;
    g.code[2] = 1;
    g.code[3] = 2;
    // Also provide a last code entry at len to satisfy g.code[len] = left
    // Choose len=4 to not overwrite the above indices
    int syms = 8;
    int left = 8;
    int len = 4;
    g.code[len] = left;  // will be overwritten in examine, but used for completeness
    int mem = 0;
    int rem = 8;           // ensure the while (rem < left) is skipped (rem >= left)

    examine(syms, left, len, mem, rem);

    // Expected output constructed from the test setup
    std::string expected = "<4, 1, 2>: 1[1] 1[2] 2[3]\n";
    TEST_ASSERT(g.out.s == expected, "Output should match expected sub-code print");

    // The base-case entry should be reset (g.code[len] becomes 0)
    TEST_ASSERT(g.code[len] == 0, "Len code entry should be reset to 0 after return");
}

// Test case 2: Ensure that when mem < large, no printing occurs and code entry is cleared.
// This covers the false branch of the mem >= large condition.
static void test_case_no_print()
{
    reset_globals();
    g.root = 0;
    g.max = 3;
    g.large = 10;          // larger than mem to prevent printing
    // Prepare a base-case path: syms == left
    int syms = 5;
    int left = 5;
    int len = 2;
    g.code[len] = left;      // will be overwritten in examine
    int mem = 0;
    int rem = 5;               // rem == left ensures the inner while won't execute

    examine(syms, left, len, mem, rem);

    // Expect no output to g.out
    TEST_ASSERT(g.out.s.empty(), "No output should be produced when mem < large");

    // Len code entry should be cleared at the end
    TEST_ASSERT(g.code[len] == 0, "Len code entry should be reset to 0 after return (no print path)");
}

// Simple test runner: call tests from main to align with domain knowledge guidance
int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    test_case_print_subcode();
    test_case_no_print();

    if (failures > 0) {
        std::cerr << "Total failures: " << failures << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}
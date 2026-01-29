// Test suite for the focal method gpc_g8b located in pngstest.c
// This test suite is written in C++11 without using GTest.
// It exercises the grayscale/gamma blending behavior of gpc_g8b by providing
// low-level pixel buffers (r, g, b, a) and a simple Background with ig/dg.
// The tests are designed to cover true/false branches of the focal predicates.
// Note: This test suite must be linked with the production PNG test file (pngstest.c)
// that defines gpc_g8b and the related data/types (Pixel, Background, sRGB*, etc.).
// We avoid depending on private/internal methods and rely only on the public
// function under test through a C linkage interface.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// We declare the focal function with C linkage and a generic pointer interface.
// This allows us to pass plain byte buffers without needing to precisely
// re-declare the internal C structs Pixel/Background in C++.
// The actual C implementation in pngstest.c will interpret these buffers as
// Pixel and Background structures and operate accordingly.
extern "C" void gpc_g8b(void* out, const void* in, const void* back);

// Helper macro for simple test assertions with informative output.
#define TEST_ASSERT(cond, name, msg)                                 \
    do {                                                             \
        if (cond) {                                                  \
            printf("[PASS] %s\n", name);                            \
        } else {                                                     \
            printf("[FAIL] %s: %s\n", name, msg);                   \
        }                                                            \
    } while (0)

// Utility to print a small 4-byte buffer for debugging (RGBA order).
static void print_pixel4(const unsigned char *p) {
    printf("(%u, %u, %u, %u)", p[0], p[1], p[2], p[3]);
}

// Test 1: when in->a <= 0, out channels must all equal back->ig and alpha becomes 255
// This covers the "a <= 0" branch.
static bool test_zero_alpha() {
    // Input pixel: a = 0, arbitrary RGB values
    unsigned char in[4] = {10, 20, 30, 0}; // r, g, b, a
    // Background with ig = 123 (expected grayscale result)
    unsigned char back[2] = {123, 0}; // ig, dg (dg not used in this branch)
    unsigned char out[4] = {0, 0, 0, 0};

    gpc_g8b(out, in, back);

    bool ok = (out[0] == back[0]) && (out[1] == back[0]) &&
              (out[2] == back[0]) && (out[3] == 255);

    if (!ok) {
        printf("  Input a=0: in=");
        print_pixel4(in);
        printf(" out=");
        print_pixel4(out);
        printf(" expected r=g=b=%u, a=255\n", back[0]);
    }
    return ok;
}

// Test 2: when in->a >= 255 and in->r == in->g == in->b (grayscale with full alpha)
// This covers the "in->a >= 255" branch and the grayscale sub-branch.
static bool test_alpha255_grayscale() {
    unsigned char in[4] = {100, 100, 100, 255};
    unsigned char back[2] = {0, 0};
    unsigned char out[4] = {0, 0, 0, 0};

    gpc_g8b(out, in, back);

    bool ok = (out[0] == 100) && (out[1] == 100) && (out[2] == 100) && (out[3] == 255);
    if (!ok) {
        printf("  Input a=255 grayscale: in=");
        print_pixel4(in);
        printf(" out=");
        print_pixel4(out);
        printf(" expected r=g=b=100, a=255\n");
    }
    return ok;
}

// Test 3: when in->a >= 255 but not grayscale (color -> sRGB(YfromRGB(...)) path)
// This verifies the non-grayscale code path and that output is a single grayscale value with a=255.
static bool test_alpha255_color_not_grayscale() {
    unsigned char in[4] = {10, 60, 200, 255};
    unsigned char back[2] = {0, 0};
    unsigned char out[4] = {0, 0, 0, 0};

    gpc_g8b(out, in, back);

    // We expect r=g=b (grayscale) and alpha = 255
    bool all_equal = (out[0] == out[1]) && (out[1] == out[2]);
    bool alpha_ok = (out[3] == 255);
    if (!all_equal || !alpha_ok) {
        printf("  Input a=255 color: in=");
        print_pixel4(in);
        printf(" out=");
        print_pixel4(out);
        printf(" expected r=g=b value (unknown exact), a=255\n");
        return false;
    }
    // Additional sanity: value should be within [0, 255]
    if (out[0] > 255) {
        printf("  Unexpected value overflow: out=%u\n", out[0]);
        return false;
    }
    // If we reach here, the branch was exercised and produced a grayscale output.
    return true;
}

// Test 4: when 0 < in->a < 255 (partial transparency path)
// Ensure r=g=b (grayscale) and alpha is 255; exercise the blending with back->dg
static bool test_mid_alpha_with_backdg() {
    unsigned char in[4] = {50, 200, 100, 128}; // a = 128
    unsigned char back[2] = {0, 25};           // dg = 25
    unsigned char out[4] = {0, 0, 0, 0};

    gpc_g8b(out, in, back);

    bool all_equal = (out[0] == out[1]) && (out[1] == out[2]);
    bool alpha_ok = (out[3] == 255);
    if (!all_equal || !alpha_ok) {
        printf("  Input a=128 mid-alpha: in=");
        print_pixel4(in);
        printf(" back=(ig=%u,dg=%u) out=", back[0], back[1]);
        print_pixel4(out);
        printf(" expected r=g=b grayscale, a=255\n");
        return false;
    }

    // Basic sanity: grayscale value should be in [0,255]
    if (out[0] > 255) {
        printf("  Unexpected value overflow: out=%u\n", out[0]);
        return false;
    }

    return true;
}

// Runner: execute all tests and report a summary
int main(void) {
    printf("Running gpc_g8b unit tests (pngstest.c focal method) with non-GTest C++ harness...\n");

    int total = 0;
    int passed = 0;

    // Step 2: Candidate keyword-driven coverage (as comments, but tests implement actual coverage)
    // Candidate Keywords considered:
    // - in->a, in->r, in->g, in->b
    // - back->ig, back->dg
    // - Branch: in->a <= 0
    // - Branch: in->a >= 255
    // - Branch: else (0 < a < 255)
    // - Output channel normalization to grayscale (r=g=b)
    // - Final alpha set to 255
    // - sRGB(YfromRGB(...)) path for non-grayscale 255-alpha case (indirectly tested by
    //   test_alpha255_color_not_grayscale)
    // - Back-dg influence on mid-alpha path (test_mid_alpha_with_backdg)

    // Test 1
    total++;
    if (test_zero_alpha()) {
        passed++;
    }

    // Test 2
    total++;
    if (test_alpha255_grayscale()) {
        passed++;
    }

    // Test 3
    total++;
    if (test_alpha255_color_not_grayscale()) {
        passed++;
    }

    // Test 4
    total++;
    if (test_mid_alpha_with_backdg()) {
        passed++;
    }

    printf("Summary: %d/%d tests passed.\n", passed, total);

    // Non-terminating assertion style: return 0 only if all tests pass, non-zero otherwise
    return (passed == total) ? 0 : 1;
}
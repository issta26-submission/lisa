// C++11 test suite for the focal method skip_chunk_type (located in pngfix.c)
// This test harness invokes the C function skip_chunk_type through an extern declaration.
// It uses a minimal struct global with a single member 'skip' to drive decision branches.
// The tests aim to cover true/false branches for each condition predicate and
// use libpng style chunk-type construction to generate 32-bit chunk type codes.

// Notes for test environment:
// - The test assumes libpng headers are available to provide png_uint_32 and chunk-type macros.
// - The actual numeric values for chunk types are derived by packing four ASCII characters
//   in the standard PNG chunk-code fashion: (a<<24) | (b<<16) | (c<<8) | d.
// - This approach relies on the canonical representation of chunk types in libpng-compatible code.

#include <cstdint>
#include <cassert>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


extern "C" {
}

// Minimal replica of the library's 'struct global' used by skip_chunk_type.
// It must align with the member the function reads (global->skip).
// The full layout of the real library struct is not required for this focused test.
struct global {
    int skip;
};

// Declaration of the focal function from pngfix.c
// We rely on the actual implementation linked during build.
// Use C linkage to call from C++ test code.
extern "C" int skip_chunk_type(const struct global *global, png_uint_32 type);

// Helper to construct a 32-bit chunk type code from four ASCII characters.
// This mirrors the canonical PNG chunk type encoding: (a<<24) | (b<<16) | (c<<8) | d
static inline png_uint_32 make_type(char a, char b, char c, char d) {
    return (static_cast<png_uint_32>(static_cast<unsigned char>(a)) << 24) |
           (static_cast<png_uint_32>(static_cast<unsigned char>(b)) << 16) |
           (static_cast<png_uint_32>(static_cast<unsigned char>(c)) << 8)  |
           (static_cast<png_uint_32>(static_cast<unsigned char>(d)));
}

// Lightweight test harness primitives
static int g_failures = 0;
static int g_tests_run = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if (static_cast<long long>(expected) != static_cast<long long>(actual)) { \
        std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                  << ", actual: " << (actual) << "\n"; \
        ++g_failures; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
    } \
} while (false)

int main() {
    // Ensure required libpng symbols/macros are available
    // Basic sanity check: ensure SKIP_* macros exist
#if !defined(SKIP_ALL) || !defined(SKIP_COLOR) || !defined(SKIP_TRANSFORM) || !defined(SKIP_UNUSED) || !defined(SKIP_UNSAFE)
    std::cerr << "Missing SKIP_* macros from libpng headers. Aborting tests.\n";
    return 1;
#endif

    // Test 1: tRNS chunk should not be skipped (always critical)
    {
        global g{0};
        png_uint_32 tRNS = make_type('t','R','N','S');
        int res = skip_chunk_type(&g, tRNS);
        EXPECT_EQ(0, res, "skip_chunk_type should not skip png_tRNS (critical-like).");
    }

    // Test 2: sBIT chunk should not be skipped (always critical-like)
    {
        global g{0};
        png_uint_32 sBIT = make_type('s','B','I','T');
        int res = skip_chunk_type(&g, sBIT);
        EXPECT_EQ(0, res, "skip_chunk_type should not skip png_sBIT (critical-like).");
    }

    // Test 3: gAMA with SKIP_ALL should be skipped
    {
        global g{0};
        g.skip = SKIP_ALL;
        png_uint_32 gAMA = make_type('g','A','M','A');
        int res = skip_chunk_type(&g, gAMA);
        EXPECT_EQ(1, res, "skip_chunk_type should skip png_gAMA when SKIP_ALL is set.");
    }

    // Test 4: sRGB with SKIP_ALL should be skipped
    {
        global g{0};
        g.skip = SKIP_ALL;
        png_uint_32 sRGB = make_type('s','R','G','B');
        int res = skip_chunk_type(&g, sRGB);
        EXPECT_EQ(1, res, "skip_chunk_type should skip png_sRGB when SKIP_ALL is set.");
    }

    // Test 5: cHRM with SKIP_COLOR should be skipped
    {
        global g{0};
        g.skip = SKIP_COLOR;
        png_uint_32 cHRM = make_type('c','H','R','M');
        int res = skip_chunk_type(&g, cHRM);
        EXPECT_EQ(1, res, "skip_chunk_type should skip png_cHRM when SKIP_COLOR is set.");
    }

    // Test 6: iCCP with SKIP_COLOR should be skipped
    {
        global g{0};
        g.skip = SKIP_COLOR;
        png_uint_32 iCCP = make_type('i','C','C','P');
        int res = skip_chunk_type(&g, iCCP);
        EXPECT_EQ(1, res, "skip_chunk_type should skip png_iCCP when SKIP_COLOR is set.");
    }

    // Test 7: bKGD with SKIP_TRANSFORM should be skipped
    {
        global g{0};
        g.skip = SKIP_TRANSFORM;
        png_uint_32 bKGD = make_type('b','K','G','D');
        int res = skip_chunk_type(&g, bKGD);
        EXPECT_EQ(1, res, "skip_chunk_type should skip png_bKGD when SKIP_TRANSFORM is set.");
    }

    // Test 8: Unknown chunk with SAFE_TO_COPY = true should be skipped when SKIP_UNUSED
    // SAFE_TO_COPY is typically true for first letter lowercase (e.g., 'a')
    {
        global g{0};
        g.skip = SKIP_UNUSED;
        png_uint_32 unk_lo = make_type('a','b','c','d'); // likely SAFE_TO_COPY = true
        int res = skip_chunk_type(&g, unk_lo);
        EXPECT_EQ(1, res, "skip_chunk_type should skip unknown chunk with SAFE_TO_COPY and SKIP_UNUSED.");
    }

    // Test 9: Unknown chunk with SAFE_TO_COPY = false should be skipped when SKIP_UNSAFE
    {
        global g{0};
        g.skip = SKIP_UNSAFE;
        png_uint_32 unk_up = make_type('A','B','C','D'); // SAFE_TO_COPY = false
        int res = skip_chunk_type(&g, unk_up);
        EXPECT_EQ(1, res, "skip_chunk_type should skip unknown chunk with SAFE_TO_COPY=false and SKIP_UNSAFE.");
    }

    // Test 10: Unknown chunk with SAFE_TO_COPY = false and skip below SKIP_UNSAFE should NOT be skipped
    {
        global g{0};
        g.skip = 0; // definitely below SKIP_UNSAFE
        png_uint_32 unk_up = make_type('A','B','C','D');
        int res = skip_chunk_type(&g, unk_up);
        EXPECT_EQ(0, res, "skip_chunk_type should NOT skip unknown chunk when SKIP levels are low and SAFE_TO_COPY=false.");
    }

    // Summary
    std::cout << "\nTest summary: "
              << g_tests_run << " tests run, "
              << g_failures << " failures.\n";

    return (g_failures == 0) ? 0 : 1;
}
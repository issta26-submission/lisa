/*
  Lightweight C++11 test suite for the focal function:
  png_chunk_index_from_name(png_uint_32 chunk_name)

  Goals:
  - Validate mapping for known PNG chunks to their PNG_INDEX_... counterparts.
  - Validate default/unknown mapping returns PNG_INDEX_unknown.
  - Use a minimal, non-terminating test approach (non-GTest) suitable for C++11.
  - Demonstrate coverage for at least IHDR, PLTE, IDAT, IEND and unknown cases.

  Notes:
  - This test relies on libpng headers (png.h) providing:
    - chunk name constants like png_IHDR, png_PLTE, png_IDAT, png_IEND, etc.
    - index constants like PNG_INDEX_IHDR, PNG_INDEX_PLTE, PNG_INDEX_IDAT, PNG_INDEX_IEND, PNG_INDEX_unknown.
  - The test is designed to link against libpng (e.g., -lpng) and call the focal function
    defined in pngrutil.c (as per the provided code snippet).
*/

#include <cstdint>
#include <png.h>
#include <pngpriv.h>
#include <cstdio>


// Declaration of the focal function (provided by the library in real usage)
extern "C" png_uint_32 png_chunk_index_from_name(png_uint_32 chunk_name);

// Simple non-terminating test harness
static int g_failures = 0;

// Lightweight assertion macro for integral values
#define TEST_CHECK_EQ(actual, expected, description)                          \
    do {                                                                       \
        if ((actual) != (expected)) {                                        \
            fprintf(stderr, "TEST FAILED: %s\n", (description));              \
            fprintf(stderr, "  Actual  : %llu\n",                             \
                    (unsigned long long)(actual));                           \
            fprintf(stderr, "  Expected: %llu\n",                            \
                    (unsigned long long)(expected));                         \
            ++g_failures;                                                     \
        } else {                                                             \
            /* Test passed for this assertion; continue execution. */         \
        }                                                                    \
    } while (0)

int main() {
    // Test 1: IHDR should map to PNG_INDEX_IHDR
    {
        // Given the chunk name for IHDR
        png_uint_32 ihdr_chunk = png_IHDR;
        // When passed to the focal function
        png_uint_32 ihdr_index = png_chunk_index_from_name(ihdr_chunk);
        // Then expect the corresponding index constant
        TEST_CHECK_EQ(ihdr_index, PNG_INDEX_IHDR,
            "IHDR chunk maps to PNG_INDEX_IHDR");
    }

    // Test 2: PLTE should map to PNG_INDEX_PLTE
    {
        png_uint_32 plte_chunk = png_PLTE;
        png_uint_32 plte_index = png_chunk_index_from_name(plte_chunk);
        TEST_CHECK_EQ(plte_index, PNG_INDEX_PLTE,
            "PLTE chunk maps to PNG_INDEX_PLTE");
    }

    // Test 3: IDAT should map to PNG_INDEX_IDAT
    {
        png_uint_32 idat_chunk = png_IDAT;
        png_uint_32 idat_index = png_chunk_index_from_name(idat_chunk);
        TEST_CHECK_EQ(idat_index, PNG_INDEX_IDAT,
            "IDAT chunk maps to PNG_INDEX_IDAT");
    }

    // Test 4: IEND should map to PNG_INDEX_IEND
    {
        png_uint_32 iend_chunk = png_IEND;
        png_uint_32 iend_index = png_chunk_index_from_name(iend_chunk);
        TEST_CHECK_EQ(iend_index, PNG_INDEX_IEND,
            "IEND chunk maps to PNG_INDEX_IEND");
    }

    // Test 5: Unknown chunk should map to PNG_INDEX_unknown
    {
        png_uint_32 unknown_chunk = 0xFFFFFFFF; // value not expected to be a known chunk
        png_uint_32 unknown_index = png_chunk_index_from_name(unknown_chunk);
        TEST_CHECK_EQ(unknown_index, PNG_INDEX_unknown,
            "Unknown chunk maps to PNG_INDEX_unknown");
    }

    // Test 6: Boundary/zero value should map to PNG_INDEX_unknown
    {
        png_uint_32 zero_chunk = 0;
        png_uint_32 zero_index = png_chunk_index_from_name(zero_chunk);
        TEST_CHECK_EQ(zero_index, PNG_INDEX_unknown,
            "Chunk value 0 maps to PNG_INDEX_unknown");
    }

    // Summary result
    if (g_failures != 0) {
        fprintf(stderr, "Total failures: %d\n", g_failures);
        return 1;
    }

    printf("All tests passed\n");
    return 0;
}
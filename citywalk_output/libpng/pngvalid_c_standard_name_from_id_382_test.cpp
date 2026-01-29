// Unit test suite for the focal method: standard_name_from_id
// Targeted for C++11 compilation without GTest.
// The tests rely on the external C function standard_name_from_id and
// the C function standard_name (provided in the project under test).
// We compare the direct call to standard_name_from_id against a
// recomputed call to standard_name with fields derived from the same id
// using the macros COL_FROM_ID, DEPTH_FROM_ID, PALETTE_FROM_ID, etc.

#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Include libpng types/macros

// Declare external C functions from the focal code base.
// We mirror the signatures used in the project so we can invoke them from C++.
extern "C" int standard_name(char *buffer, size_t bufsize, size_t pos,
                           png_byte colour_type, int bit_depth,
                           unsigned int npalette, int interlace_type,
                           png_uint_32 w, png_uint_32 h, int do_interlace);

extern "C" int standard_name_from_id(char *buffer, size_t bufsize, size_t pos,
                                    png_uint_32 id);

// Simple non-terminating test harness
// Tracks number of failed checks but continues execution to maximize coverage.
static int g_failures = 0;
#define CHECK(cond, msg)                          \
    do {                                            \
        if (!(cond)) {                              \
            ++g_failures;                            \
            std::cerr << "TEST_FAIL: " << msg << "\n"; \
        }                                           \
    } while (0)

static void run_case(png_uint_32 id) {
    // Buffers for outputs
    char buf_from_id[256];
    char buf_from_expanded[256];
    std::memset(buf_from_id, 0, sizeof(buf_from_id));
    std::memset(buf_from_expanded, 0, sizeof(buf_from_expanded));

    // Call the focal function directly
    int ret_from_id = standard_name_from_id(buf_from_id, sizeof(buf_from_id), 0, id);

    // Compute expected values by deriving fields from id and calling standard_name
    // Use the same pos value (0) to ensure consistent formatting
    png_byte colour_type      = COL_FROM_ID(id);
    int     bit_depth          = DEPTH_FROM_ID(id);
    unsigned int npalette        = PALETTE_FROM_ID(id);
    int     interlace_type     = INTERLACE_FROM_ID(id);
    png_uint_32 w                 = WIDTH_FROM_ID(id);
    png_uint_32 h                 = HEIGHT_FROM_ID(id);
    int     do_interlace         = DO_INTERLACE_FROM_ID(id);

    int ret_expected = standard_name(buf_from_expanded, sizeof(buf_from_expanded), 0,
                                   colour_type, bit_depth, npalette,
                                   interlace_type, w, h, do_interlace);

    // Compare return codes
    CHECK(ret_from_id == ret_expected,
          "Return value mismatch for id " << std::hex << id);

    // Compare produced strings
    if (std::strcmp(buf_from_id, buf_from_expanded) != 0) {
        ++g_failures;
        std::cerr << "BUFFER_MISMATCH for id 0x" << std::hex << id << std::dec << "\n";
        std::cerr << "standard_name_from_id: \"" << buf_from_id << "\"\n";
        std::cerr << "standard_name_expanded: \"" << buf_from_expanded << "\"\n";
    }
}

int main() {
    // Run a set of representative test ids to cover multiple branches
    // including boundary and varied bit fields.
    std::vector<png_uint_32> test_ids = {
        0x00000000U,      // all zero fields
        0x00000001U,      // minimal non-zero
        0x12345678U,      // mixed field values
        0x89ABCDEFU,      // another mixed value
        0xFFFFFFFFU       // all bits set (boundary)
    };

    for (png_uint_32 id : test_ids) {
        run_case(id);
    }

    // Additional targeted case: ensure non-zero palette path is exercised
    // Construct an id that would typically result in a non-zero palette flag
    // by setting a hypothetical PALETTE bits (via macro usage).
    // The exact palette bit position depends on the libpng internal encoding.
    // We attempt a few variations around the previous id values.
    run_case(0x01020304U);
    run_case(0x0F0F0F0FU);

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL_TESTS_PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}
// Test suite for zlib_flevel in pngfix.c
// This test targets the focal method zlib_flevel and exercises all
// reachable branches of the switch over (zlib->header[1] >> 6).
// It relies on the project's existing png.h (or libpng headers) to
// provide the actual struct zlib type used by pngfix.c.
// The tests are implemented in C++11 (no Google Test) and use a lightweight
// harness that continues on failures to maximize coverage.

#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


#ifdef PNG_FREESTANDING_TESTS
#else
#endif

// zlib_flevel is defined in pngfix.c with C linkage.
// We declare it here with C linkage to avoid name mangling when compiled as C++.
extern "C" const char *zlib_flevel(struct zlib *zlib);

// Helper: run a single test case.
// - header1_value: value assigned to zlib->header[1]
// - expected: expected string result from zlib_flevel
// - test_name: descriptive name for logging
// Returns 1 on failure, 0 on success.
static int run_case(int header1_value, const char *expected, const char *test_name) {
    // Build a zlib instance. The actual libpng/pngfix project should provide
    // the complete definition of 'struct zlib' from png.h.
    struct zlib z;
    // Zero initialize to sane defaults; we only set header[1] for the test.
    std::memset(&z, 0, sizeof(z));
    // zlib_flevel only inspects header[1] (specifically its top 2 bits after >>6).
    z.header[1] = (unsigned char)header1_value;

    const char *actual = zlib_flevel(&z);
    if (std::strcmp(actual, expected) != 0) {
        std::fprintf(stderr,
                     "TEST FAILED: %s\n"
                     "  Input header[1]=%d => got \"%s\", expected \"%s\"\n",
                     test_name, header1_value, actual, expected);
        return 1;
    } else {
        std::printf("TEST PASSED: %s\n", test_name);
        return 0;
    }
}

int main() {
    int failures = 0;

    // Step 2, Test Case Generation:
    // Branch coverage for zlib_flevel:
    // - Case 0: header[1] in [0..63] => (header[1] >> 6) == 0 => "supfast"
    // - Case 1: header[1] in [64..127] => (header[1] >> 6) == 1 => "stdfast"
    // - Case 2: header[1] in [128..191] => (header[1] >> 6) == 2 => "default"
    // - Case 3: header[1] in [192..255] => (header[1] >> 6) == 3 => "maximum"
    //
    // The following tests exercise boundary values and representative points.

    // True branch: supfast (case 0)
    // Using exact boundary and near-boundary values to ensure 0 <= (header[1]>>6) <= 0
    failures += run_case(0, "supfast", "zlib_flevel_supfast_zero");
    failures += run_case(63, "supfast", "zlib_flevel_supfast_boundary63");

    // True branch: stdfast (case 1)
    // header[1] with top two bits = 01
    failures += run_case(64, "stdfast", "zlib_flevel_stdfast_64");
    failures += run_case(127, "stdfast", "zlib_flevel_stdfast_boundary127");

    // True branch: default (case 2)
    // header[1] with top two bits = 10
    failures += run_case(128, "default", "zlib_flevel_default_128");
    failures += run_case(191, "default", "zlib_flevel_default_boundary191");

    // True branch: maximum (case 3)
    // header[1] with top two bits = 11
    failures += run_case(192, "maximum", "zlib_flevel_maximum_192");
    failures += run_case(255, "maximum", "zlib_flevel_maximum_255");

    if (failures > 0) {
        std::fprintf(stderr, "Total failures: %d\n", failures);
        return 2;
    }

    std::printf("ALL TESTS PASSED\n");
    return 0;
}
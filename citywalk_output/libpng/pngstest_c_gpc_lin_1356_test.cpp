// C++11 unit test harness for the focal method gpc_lin from pngstest.c
// - This test exercises the branching logic of gpc_lin by comparing the
//   actual output against a deterministic expected value derived from ilinear.
// - We rely on the external ilinear implementation provided by the tested project.
// - No GTest is used; instead, simple asserts and printable diagnostics are used.

#include <cstdint>
#include <cassert>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <string>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Forward declarations of the C components we are testing.
// We assume the Pixel structure is 4 x uint16_t: r, g, b, a.
// The Background type is opaque for this test (function ignores it via (void)back;).


extern "C" {

// Pixel layout assumed by the pngstest.c in the focal code path.
typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t a;
} Pixel;

// An opaque Background type; only its pointer is used in gpc_lin.
typedef struct {
    // Intentionally empty; the focal function ignores this value.
} Background;

// External functions from the tested file.
// ilinear maps a fixed_srgb value to a linear value (implementation provided by the project).
uint16_t ilinear(uint16_t fixed_srgb);

// Focal function under test. It writes into 'out' based on 'in' and 'back'.
void gpc_lin(Pixel *out, const Pixel *in, const Background *back);

} // extern "C"


// Helper: run a single test case comparing actual output to expected values
static bool run_case(const std::string &name, Pixel input)
{
    Pixel output;
    Background back; // dummy background; the function ignores it
    gpc_lin(&output, &input, &back);

    // Compute expected values using the same ilinear transformation as the focal method.
    uint16_t expected_r = ilinear(input.r);
    uint16_t expected_g = (input.g == input.r) ? expected_r : ilinear(input.g);

    uint16_t expected_b;
    if (input.g == input.r) {
        // Branch: g == r
        if (input.b == input.r)
            expected_b = expected_r;
        else
            expected_b = ilinear(input.b);
    } else {
        // Branch: g != r
        if (input.b == input.r)
            expected_b = expected_r;
        else if (input.b == input.g)
            expected_b = expected_g;
        else
            expected_b = ilinear(input.b);
    }

    // The alpha channel is set to 65535 by gpc_lin in all cases.
    const uint16_t expected_a = 65535;

    bool ok = (output.r == expected_r) &&
              (output.g == expected_g) &&
              (output.b == expected_b) &&
              (output.a == expected_a);

    if (!ok) {
        std::cerr << "Test '" << name << "' failed.\n"
                  << "  Input:  (" << input.r << ", " << input.g << ", " << input.b << ", " << input.a << ")\n"
                  << "  Output: (" << output.r << ", " << output.g << ", " << output.b << ", " << output.a << ")\n"
                  << "  Expected: (" << expected_r << ", " << expected_g << ", " << expected_b << ", " << expected_a << ")\n";
    } else {
        std::cout << "Test '" << name << "' passed.\n";
    }

    return ok;
}


int main(void)
{
    // Step 1: True equality branch - all channels equal
    // Input where r == g == b; expect r==g==b after ilinear, a=65535
    Pixel t1_in = { 100, 100, 100, 0 };
    bool t1 = run_case("all_equal_r_g_b", t1_in);

    // Step 2: r == g but b is different
    // Expect out.r == out.g, and out.b == ilinear(b)
    Pixel t2_in = { 50, 50, 200, 0 };
    bool t2 = run_case("r_eq_g_b_diff", t2_in);

    // Step 3: r != g and b == r
    // Expect out.r == ilinear(r), out.g == ilinear(g), out.b == out.r
    Pixel t3_in = { 10, 20, 10, 0 };
    bool t3 = run_case("r_diff_g_and_b_eq_r", t3_in);

    // Step 4: r != g and b == g
    // Expect out.r == ilinear(r), out.g == ilinear(g), out.b == out.g
    Pixel t4_in = { 5, 15, 15, 0 };
    bool t4 = run_case("r_diff_g_and_b_eq_g", t4_in);

    // Step 5: all channels distinct
    // Expect general ilinear mapping for each channel with b not equal to r or g
    Pixel t5_in = { 1, 4, 2, 0 };
    bool t5 = run_case("all_diff_r_g_b", t5_in);

    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;
    if (!t3) ++failures;
    if (!t4) ++failures;
    if (!t5) ++failures;

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
}
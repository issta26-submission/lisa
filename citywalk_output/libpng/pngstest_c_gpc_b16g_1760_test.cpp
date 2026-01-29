// Minimal C++11 test suite for the focal function gpc_b16g from pngstest.c
// This test suite avoids any external testing framework (no GTest) and uses a
// lightweight main() based test runner. It focuses on exercising the two branches
// of the conditional inside gpc_b16g and validates key outputs (RGB channel handling
// and alpha propagation) as per the function's logic.

// The tests assume the following compatibility with the C implementation in pngstest.c:
// - Pixel is a struct with 16-bit color channels: r, g, b, a (uint16_t).
// - Background is a struct with 16-bit fields: ig (gray value for opaque) and dg (delta gray).
// - gpc_b16g has C linkage: void gpc_b16g(Pixel *out, const Pixel *in, const Background *back);

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Forward declaration of the function under test (C linkage)
extern "C" {
    // Pixel and Background must match the definitions expected by pngstest.c
    struct Pixel {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    };

    struct Background {
        uint16_t ig;
        uint16_t dg;
    };

    void gpc_b16g(Pixel *out, const Pixel *in, const Background *back);
}

// Lightweight test helpers
static void log_test(const char* name, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << std::endl;
}

// Test 1: Branch when in->a <= 0 should copy the background grey (ig) to RGB and set alpha to 255
static bool test_gpc_b16g_inA_zero_branch() {
    // Arrange
    Pixel in;
    in.r = in.g = in.b = 0;
    in.a = 0; // triggers the "in->a <= 0" branch

    Background back;
    back.ig = 0xABCD;  // some arbitrary grey value
    back.dg = 0x1234;  // arbitrary delta-grey (not used in this branch)

    Pixel out;

    // Act
    gpc_b16g(&out, &in, &back);

    // Assert
    bool ok = true;
    if (out.a != 255) {
        ok = false;
    }
    if (out.r != back.ig || out.g != back.ig || out.b != back.ig) {
        ok = false;
    }

    // Optionally print details on failure
    if (!ok) {
        std::cout << "  Details: in.a=" << in.a
                  << " back.ig=" << back.ig
                  << " out=(" << out.r << "," << out.g << "," << out.b << "," << out.a << ")\n";
    }

    log_test("gpc_b16g_inA_zero_branch: RGB equals back.ig and alpha=255", ok);
    return ok;
}

// Test 2: Branch when in->a > 0 should produce RGB equal across channels (r==g==b)
// and alpha should be set to 255. We purposefully pick in.g=0 and back.dg=0
// so the internal computation yields sRGB(0) which should assign 0 to RGB.
// This validates the else-branch's common-path outcome (rgb channels equal and alpha).
static bool test_gpc_b16g_inA_positive_else_branch_zero_inputs() {
    // Arrange
    Pixel in;
    in.r = in.g = in.b = 0;
    in.a = 1; // triggers the "else" branch since a > 0

    // Use values that drive the internal linear combination to zero
    in.g = 0;
    // The test focuses on the RGB channels and alpha, not the exact numeric conversion
    Background back;
    back.ig = 0x0000; // not used in this branch, but kept valid
    back.dg = 0x0000; // ensures the combination can yield 0

    Pixel out;

    // Act
    gpc_b16g(&out, &in, &back);

    // Assert
    bool ok = true;
    if (out.a != 255) {
        ok = false;
    }
    if (!(out.r == out.g && out.g == out.b)) {
        ok = false;
    }
    // We expect sRGB(in->g * a + back->dg * a1) with in.g=0 and back.dg=0 to be 0
    // If sRGB(0) yields 0, then RGB should be 0.
    if (out.r != 0) {
        ok = false;
    }

    // Optionally print details on failure
    if (!ok) {
        std::cout << "  Details: in.a=" << in.a
                  << " in.g=" << static_cast<unsigned>(in.g)
                  << " back.dg=" << back.dg
                  << " out=(" << out.r << "," << out.g << "," << out.b << "," << out.a << ")\n";
    }

    log_test("gpc_b16g_inA_positive_else_branch_rgb_all_zero_when_inputs_zero", ok);
    return ok;
}

// Simple runner
int main(void) {
    // Run tests
    bool t1 = test_gpc_b16g_inA_zero_branch();
    bool t2 = test_gpc_b16g_inA_positive_else_branch_zero_inputs();

    int total = 2;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0);

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
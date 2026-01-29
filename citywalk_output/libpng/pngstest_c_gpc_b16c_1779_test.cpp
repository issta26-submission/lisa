// Minimal C++11 test suite for the C function gpc_b16c from pngstest.c
// This test targets the focal method and its core dependencies.
// It does not rely on Google Test; a lightweight test harness is implemented.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Candidate Keywords (Step 1):
// - gpc_b16c: 16-bit channel blending with background
// - Pixel: input/output pixel structure (r,g,b,a)
// - Background: background color data with separate channels (ir,ig,ib,dr,dg,db)
// - sRGB: color space conversion function used inside gpc_b16c
// - Branch coverage: in->a <= 0 (false/true branches)
// - 16-bit color depth, alpha handling, back-color referencing
// - Test scaffolding: non-terminating assertions, result logging

// Step 2: Provide C declarations matching the focal class dependencies
// These declarations mirror the layout of the structures used by gpc_b16c.
// We assume the overall layout consistent with the usage in the focal method.

extern "C" {

// Pixel structure used by gpc_b16c (16-bit per channel, 0..65535)
typedef struct Pixel {
    unsigned short r;
    unsigned short g;
    unsigned short b;
    unsigned short a;
} Pixel;

// Background structure used by gpc_b16c
// Contains both input color channels (ir, ig, ib) and destination channels (dr, dg, db)
typedef struct Background {
    unsigned short ir;
    unsigned short ig;
    unsigned short ib;
    unsigned short dr;
    unsigned short dg;
    unsigned short db;
} Background;

// gpc_b16c function under test
void gpc_b16c(Pixel *out, const Pixel *in, const Background *back);

// sRGB helper function used within gpc_b16c
// Converts a linear value to a gamma-corrected sRGB value.
// It is defined in the same library as gpc_b16c; we declare it for test calculations.
double sRGB(double linear);

} // extern "C"

// Simple test harness (non-terminating assertions) suitable for C++11

static int g_total = 0;
static int g_passed = 0;

static void log_result(const char* test_name, bool passed) {
    g_total++;
    if (passed) {
        g_passed++;
        // Non-terminating, only log success quietly
        // printf("PASS: %s\n", test_name);
    } else {
        fprintf(stderr, "FAIL: %s\n", test_name);
    }
}

// Helper: compute expected output for the else-branch (in->a > 0) using the same formula as gpc_b16c
static Pixel compute_expected_else(const Pixel* in, const Background* back) {
    Pixel exp;
    double a = static_cast<double>(in->a) / 65535.0; // a in [0,1]
    double a1 = 1.0 - a;
    a /= 65535.0; // as done in gpc_b16c
    double r_lin = static_cast<double>(in->r) * a + static_cast<double>(back->dr) * a1;
    double g_lin = static_cast<double>(in->g) * a + static_cast<double>(back->dg) * a1;
    double b_lin = static_cast<double>(in->b) * a + static_cast<double>(back->db) * a1;
    exp.r = static_cast<unsigned short>( sRGB(r_lin) );
    exp.g = static_cast<unsigned short>( sRGB(g_lin) );
    exp.b = static_cast<unsigned short>( sRGB(b_lin) );
    exp.a = 255;
    return exp;
}

// Test Case 1: in->a == 0; should take background color ir/ig/ib and set alpha to 255
static bool test_case_zero_alpha() {
    Pixel in  = { 10, 20, 30, 0 };       // a <= 0 path
    Background back = { 11, 22, 33, 100, 110, 120 }; // ir, ig, ib, dr, dg, db

    Pixel out = {0, 0, 0, 0};
    gpc_b16c(&out, &in, &back);

    bool pass = (out.r == back.ir) && (out.g == back.ig) && (out.b == back.ib) && (out.a == 255);
    return pass;
}

// Test Case 2: in->a > 0 with small alpha value; verify computation matches formula
static bool test_case_small_alpha() {
    Pixel in  = { 1000, 2000, 3000, 1 }; // a > 0
    Background back = { 0, 0, 0, 4000, 5000, 6000 }; // dr/dg/db set to non-zero values

    Pixel out;
    std::memset(&out, 0, sizeof(out));

    gpc_b16c(&out, &in, &back);

    Pixel expected = compute_expected_else(&in, &back);
    bool pass = (out.r == expected.r) && (out.g == expected.g) && (out.b == expected.b) && (out.a == 255);
    return pass;
}

// Test Case 3: in->a at maximum (65535); ensure else-branch with a close to 0 result for the r,g,b channels
static bool test_case_full_alpha() {
    Pixel in  = { 65535, 65535, 65535, 65535 }; // a = 65535
    Background back = { 0, 0, 0, 0, 0, 0 }; // dr/dg/db = 0

    Pixel out;
    std::memset(&out, 0, sizeof(out));

    gpc_b16c(&out, &in, &back);

    // Compute expected: a = 1.0; a1 = 0; a /= 65535 => 1/65535
    double a = 1.0;            // a = in.a/65535.0
    double a1 = 0.0;           // 1 - a
    a /= 65535.0;              // a becomes 1/65535
    double r_lin = static_cast<double>(in.r) * a + static_cast<double>(back.dr) * a1;
    double g_lin = static_cast<double>(in.g) * a + static_cast<double>(back.dg) * a1;
    double b_lin = static_cast<double>(in.b) * a + static_cast<double>(back.db) * a1;
    Pixel expected;
    expected.r = static_cast<unsigned short>( sRGB(r_lin) );
    expected.g = static_cast<unsigned short>( sRGB(g_lin) );
    expected.b = static_cast<unsigned short>( sRGB(b_lin) );
    expected.a = 255;

    bool pass = (out.r == expected.r) && (out.g == expected.g) && (out.b == expected.b) && (out.a == 255);
    return pass;
}

// Main test runner
int main(void) {
    // Run tests and log results
    bool t1 = test_case_zero_alpha();
    log_result("gpc_b16c_case_zero_alpha", t1);

    bool t2 = test_case_small_alpha();
    log_result("gpc_b16c_case_small_alpha", t2);

    bool t3 = test_case_full_alpha();
    log_result("gpc_b16c_case_full_alpha", t3);

    // Summary
    printf("Tests executed: %d, Passed: %d, Failed: %d\n",
           g_total, g_passed, g_total - g_passed);

    return (g_total == g_passed) ? 0 : 1;
}
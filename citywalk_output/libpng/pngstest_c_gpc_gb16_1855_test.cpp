// C++11 test harness for gpc_gb16 (no GTest, self-contained assertions)
// This test suite assumes the Pixel and Background structures and the
// functions isRGB, sRGB, and gpc_gb16 are available with C linkage.
// It focuses on the 3 branches inside gpc_gb16 as described in the focal method.

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <iomanip>
#include <string>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <ctype.h>


// Provide C-linkage declarations for the tested symbols.
// These declarations mirror the minimal interface used by the focal code.
// The actual project may define these in headers; if not, this keeps tests
// decoupled and compilable with the project environment.

extern "C" {
    // Minimal Pixel and Background type shapes inferred from usage in focal method
    // Note: Field names should align with the implementation to ensure proper access.
    typedef struct Pixel {
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;
    } Pixel;

    typedef struct Background {
        unsigned int ig;  // background gray level for fully transparent path (intended)
        unsigned int dg;  // mixed gray level contribution for mid-alpha path
    } Background;

    // Function prototypes used by gpc_gb16
    void gpc_gb16(Pixel *out, const Pixel *in, const Background *back);
    unsigned int isRGB(int fixed_linear);
    double sRGB(double linear);
}

// Simple non-terminating test harness
static int g_failures = 0;

#define TEST_NAME_BUFFER_SIZE 256

// Helper: Print a failure message with test name and details
static void fail(const std::string &test_name, const std::string &details) {
    std::cerr << "[FAIL] " << test_name << ": " << details << std::endl;
    ++g_failures;
}

// Test 1: in->a <= 0 should copy back->ig to RGB channels and set alpha to 255
// This exercises the first branch of gpc_gb16.
static void test_gb16_alpha_zero_branch() {
    const char test_name[] = "gb16_alpha_zero_branch";

    Pixel in = {0, 0, 0, 0};      // a = 0 triggers first branch
    Background back = { .ig = 128, .dg = 0 }; // arbitrary background gray
    Pixel out = {0, 0, 0, 0};

    gpc_gb16(&out, &in, &back);

    // Expected: r=g=b=back.ig, a=255
    if (out.r != back.ig || out.g != back.ig || out.b != back.ig) {
        fail(test_name, "RGB channels do not match back->ig when in.a <= 0");
    } else if (out.a != 255) {
        fail(test_name, "Alpha channel is not set to 255 in alpha-zero branch");
    } else {
        // Passed
        std::cout << "[PASS] " << test_name << std::endl;
    }
}

// Test 2: in->a >= 65535 should set RGB channels to isRGB(in->g) and alpha to 255
// This exercises the second branch of gpc_gb16.
static void test_gb16_alpha_full_branch() {
    const char test_name[] = "gb16_alpha_full_branch";

    Pixel in = {0, 200, 0, 65535}; // a = 65535 triggers second branch
    // in.g used as input to isRGB
    Background back = { .ig = 0, .dg = 0 };
    Pixel out = {0, 0, 0, 0};

    unsigned int expected = isRGB((int)in.g);

    gpc_gb16(&out, &in, &back);

    // Expected: r=g=b=isRGB(in.g), a=255
    if (out.r != expected || out.g != expected || out.b != expected) {
        fail(test_name, "RGB channels do not match isRGB(in.g) for alpha-full branch");
    } else if (out.a != 255) {
        fail(test_name, "Alpha channel is not set to 255 in alpha-full branch");
    } else {
        std::cout << "[PASS] " << test_name << std::endl;
    }
}

// Test 3: 0 < in->a < 65535 should blend via sRGB(as described)
// This exercises the else branch with mid-alpha blending
static void test_gb16_alpha_mid_branch() {
    const char test_name[] = "gb16_alpha_mid_branch";

    // Choose mid-alpha value
    Pixel in = {0, 100, 0, 32768};  // a ~ 0.5 (half of 65535)
    Background back = { .ig = 0, .dg = 30 }; // some background gray/dark value
    Pixel out = {0, 0, 0, 0};

    // Compute expected using the same mathematical steps as the focal code
    double a = in.a / 65535.0; // ~0.5
    double a1 = 1.0 - a;       // ~0.5
    a /= 65535.0;              // very small factor
    double blended = in.g * a + back.dg * a1;
    unsigned int expected = static_cast<unsigned int>(sRGB(blended));

    gpc_gb16(&out, &in, &back);

    // Expected: r=g=b=expected, a=255
    if (out.r != expected || out.g != expected || out.b != expected) {
        fail(test_name, "RGB channels do not match expected sRGB blend in mid-alpha branch");
    } else if (out.a != 255) {
        fail(test_name, "Alpha channel is not set to 255 in mid-alpha branch");
    } else {
        std::cout << "[PASS] " << test_name << std::endl;
    }
}

// Test 4: Ensure static-like behavior is respected (static helpers not directly tested here)
// This test ensures that gpc_gb16 continues to set alpha to 255 in all branches.
// It acts as a lightweight regression check for the alpha behavior.
static void test_gb16_alpha_always_255() {
    const char test_name[] = "gb16_alpha_all_branches_alpha_255";

    // Case 1: alpha-zero branch
    Pixel in1 = {10, 0, 0, 0};
    Background back1 = { .ig = 7, .dg = 0 };
    Pixel out1 = {0,0,0,0};
    gpc_gb16(&out1, &in1, &back1);
    if (out1.a != 255) { fail(test_name, "Alpha not 255 in branch 1"); }

    // Case 2: alpha-full branch
    Pixel in2 = {0, 10, 0, 65535};
    Background back2 = { .ig = 0, .dg = 0 };
    Pixel out2 = {0,0,0,0};
    gpc_gb16(&out2, &in2, &back2);
    if (out2.a != 255) { fail(test_name, "Alpha not 255 in branch 2"); }

    // Case 3: mid-alpha branch
    Pixel in3 = {0, 50, 0, 32768};
    Background back3 = { .ig = 0, .dg = 10 };
    Pixel out3 = {0,0,0,0};
    gpc_gb16(&out3, &in3, &back3);
    if (out3.a != 255) { fail(test_name, "Alpha not 255 in branch 3"); }

    // If reached here, alpha-255 holds for all tested branches
    std::cout << "[PASS] " << test_name << std::endl;
}

// Entry point: run all tests and report summary
int main(void) {
    // Run individual test cases
    test_gb16_alpha_zero_branch();
    test_gb16_alpha_full_branch();
    test_gb16_alpha_mid_branch();
    test_gb16_alpha_always_255();

    // Summary
    if (g_failures == 0) {
        std::cout << "\nAll tests passed. Summary: "
                  << g_failures << " failures." << std::endl;
        return 0;
    } else {
        std::cerr << "\nTest suite completed with " << g_failures
                  << " failure(s)." << std::endl;
        return 1;
    }
}
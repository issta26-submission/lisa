/*
Unit Test Suite for gamma_transform_test (C API interop with libpng-like shim)

Notes and design decisions:
- This test suite is designed to be compiled as a C++11 unit test harness that links against
  the C source containing gamma_transform_test (the focal method) and its static helpers.
- To observe the behavior of gamma_transform_test without pulling the full libpng implementation,
  we provide a C linkage stub for gamma_test that captures the generated 'name' string and
  some relevant parameters. Our test asserts on this captured data to verify:
  - Branch selection for the sbit vs bit_depth condition
  - Presence of "16to8" when scale16 is true
  - Correct formatting of the file and screen gamma values to 3 decimals
  - Propagation of pm->test_gamma_expand16 through to gamma_test
- We also provide a minimal fake png_modifier type to satisfy the function signatures.
- The tests do not terminate the program on failure; instead they use simple assertions and
  print a summary at the end. This aligns with the instruction to use non-terminating assertions
  to maximize code execution.
- This harness assumes the focal functions are available at link time:
  - extern "C" void gamma_transform_test(png_modifier *pm,
      png_byte colour_type, png_byte bit_depth, int palette_number,
      int interlace_type, const double file_gamma, const double screen_gamma,
      png_byte sbit, int use_input_precision, int scale16);
  - extern "C" void gamma_test(png_modifier *pm, png_byte colour_type, png_byte bit_depth,
      int palette_number, int interlace_type, double file_gamma, double screen_gamma,
      png_byte sbit, int unused, const char *name,
      int use_input_precision, int scale16, int expand16, int do_background,
      int a, int b, int c);
- For the purposes of this test harness, we redefine the necessary types here (png_byte and png_modifier)
  to avoid pulling the entire libpng headers. The gamma_transform_test in the C source will interoperate
  with these definitions when linked together.

Usage:
- Compile this file together with the C source that defines gamma_transform_test and the real
  gamma_test (or with a compatible gamma_test stub as provided by this harness).
- The harness runs several targeted tests and prints a summary.

Author: OpenAI's ChatGPT
*/

#include <cassert>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Forward declarations to mirror the C API used by gamma_transform_test.
// We provide minimal type definitions sufficient for compilation and linking with the C code.

typedef unsigned char png_byte;

// Minimal png_modifier structure to satisfy gamma_transform_test's usage.
// The real libpng type has many fields; we only need the ones referenced in gamma_transform_test.
typedef struct png_modifier {
    int test_gamma_expand16; // observed by tests via gamma_test parameter propagation
} png_modifier;

// Forward declarations of the focal C functions (to be provided by the library under test).
extern "C" {
    // The function under test (gamma_transform_test)
    void gamma_transform_test(png_modifier *pm,
                            png_byte colour_type, png_byte bit_depth,
                            int palette_number, int interlace_type,
                            const double file_gamma, const double screen_gamma,
                            png_byte sbit, int use_input_precision, int scale16);

    // A stubbed callback that gamma_transform_test will call to report its internal name string
    // and related parameters. We provide a concrete implementation in this test harness below
    // with C linkage so it can be linked against the C unit under test.
    void gamma_test(png_modifier *pm,
                    png_byte colour_type, png_byte bit_depth,
                    int palette_number, int interlace_type,
                    double file_gamma, double screen_gamma,
                    png_byte sbit, int unused, const char *name,
                    int use_input_precision, int scale16,
                    int expand16, int do_background, int a, int b, int c);
}

// Data structure to capture gamma_test invocations for assertions.
struct GammaTestCall {
    std::string name;
    unsigned int colour_type;
    unsigned int bit_depth;
    int palette_number;
    int interlace_type;
    double file_gamma;
    double screen_gamma;
    unsigned int sbit;
    int unused;
    int use_input_precision;
    int scale16;
    int expand16;
    int do_background;
    int a;
    int b;
    int c;
};

// Global storage for captured gamma_test invocations
static std::vector<GammaTestCall> g_captured_calls;

// C linkage implementation of gamma_test to intercept calls from gamma_transform_test
extern "C" void gamma_test(png_modifier *pm,
                         png_byte colour_type, png_byte bit_depth,
                         int palette_number, int interlace_type,
                         double file_gamma, double screen_gamma,
                         png_byte sbit, int unused, const char *name,
                         int use_input_precision, int scale16,
                         int expand16, int do_background, int a, int b, int c)
{
    GammaTestCall call;
    call.name = name ? std::string(name) : std::string();
    call.colour_type = static_cast<unsigned int>(colour_type);
    call.bit_depth = static_cast<unsigned int>(bit_depth);
    call.palette_number = palette_number;
    call.interlace_type = interlace_type;
    call.file_gamma = file_gamma;
    call.screen_gamma = screen_gamma;
    call.sbit = static_cast<unsigned int>(sbit);
    call.unused = unused;
    call.use_input_precision = use_input_precision;
    call.scale16 = scale16;
    call.expand16 = expand16;
    call.do_background = do_background;
    call.a = a;
    call.b = b;
    call.c = c;

    g_captured_calls.push_back(call);
}

// Lightweight test helper to reset captured data
static void reset_captured_calls() {
    g_captured_calls.clear();
}

// Helper to print test results
static void print_summary(const std::string &title, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << title << "\n";
}

// Domain-specific helpers and tests
// We implement a few unit tests to cover critical branches and formatting of the name string
// produced by gamma_transform_test. Each test is documented with the keywords it targets.

static bool test_case_sbit_branch_true() {
    // Candidate Keywords: sbit, bit_depth, "sbit(n) ", 3-decimal gamma formatting, no scale16
    reset_captured_calls();

    png_modifier pm;
    pm.test_gamma_expand16 = 7; // arbitrary to verify propagation

    // true branch: sbit != bit_depth and sbit != 0
    // colour_type=2 (truecolor), bit_depth=8, sbit=4
    gamma_transform_test(&pm, 2, 8, 0, 0, 0.454, 1.234, 4, 0, 0);

    if (g_captured_calls.size() != 1) {
        print_summary("test_case_sbit_branch_true: expected 1 gamma_test call", false);
        return false;
    }

    const std::string &name = g_captured_calls[0].name;
    // Expect name to start with sbit(4) followed by a space
    if (name.find("sbit(4) ") != 0) {
        print_summary("test_case_sbit_branch_true: name prefix should be 'sbit(4) '", false);
        return false;
    }

    // Expect 0.454->1.234 formatting
    if (name.find("0.454->1.234") == std::string::npos) {
        print_summary("test_case_sbit_branch_true: name should contain '0.454->1.234'", false);
        return false;
    }

    // No 16to8 expected in this case
    if (name.find("16to8") != std::string::npos) {
        print_summary("test_case_sbit_branch_true: unexpected '16to8' in name when scale16 is 0", false);
        return false;
    }

    // Expand16 parameter should reflect pm.test_gamma_expand16
    if (g_captured_calls[0].expand16 != 7) {
        print_summary("test_case_sbit_branch_true: expand16 should propagate pm->test_gamma_expand16", false);
        return false;
    }

    print_summary("test_case_sbit_branch_true", true);
    return true;
}

static bool test_case_sbit_branch_false_with_scale16() {
    // Candidate Keywords: gamma, scale16, "16to8 ", 3-decimal gamma formatting
    reset_captured_calls();

    png_modifier pm;
    pm.test_gamma_expand16 = 3;

    // false branch: e.g., sbit == 0, scale16 = 1
    gamma_transform_test(&pm, 2, 8, 0, 0, 0.500, 0.750, 0, 0, 0);

    if (g_captured_calls.size() != 1) {
        print_summary("test_case_sbit_branch_false_with_scale16: expected 1 gamma_test call", false);
        return false;
    }

    const std::string &name = g_captured_calls[0].name;
    // Expect name to start with 'gamma '
    if (name.find("gamma ") != 0) {
        print_summary("test_case_sbit_branch_false_with_scale16: name should start with 'gamma '", false);
        return false;
    }

    // Expect presence of 16to8 after gamma
    if (name.find("16to8 ") == std::string::npos) {
        print_summary("test_case_sbit_branch_false_with_scale16: expected '16to8 ' in name when scale16 is true", false);
        return false;
    }

    // Expect 0.500->0.750 formatting
    if (name.find("0.500->0.750") == std::string::npos) {
        print_summary("test_case_sbit_branch_false_with_scale16: name should contain '0.500->0.750'", false);
        return false;
    }

    // Expand16 should propagate
    if (g_captured_calls[0].expand16 != 3) {
        print_summary("test_case_sbit_branch_false_with_scale16: expand16 should propagate", false);
        return false;
    }

    print_summary("test_case_sbit_branch_false_with_scale16", true);
    return true;
}

static bool test_case_gamma_formatting_consistent() {
    // Additional check: identical gamma formatting for distinct inputs
    reset_captured_calls();

    png_modifier pm;
    pm.test_gamma_expand16 = 9;

    gamma_transform_test(&pm, 2, 8, 0, 0, 0.123, 0.999, 8, 1, 0);

    if (g_captured_calls.size() != 1) {
        print_summary("test_case_gamma_formatting_consistent: expected 1 gamma_test call", false);
        return false;
    }

    const std::string &name = g_captured_calls[0].name;
    if (name.find("gamma ") != 0) {
        print_summary("test_case_gamma_formatting_consistent: expected 'gamma ' prefix", false);
        return false;
    }

    if (name.find("0.123->0.999") == std::string::npos) {
        print_summary("test_case_gamma_formatting_consistent: expected '0.123->0.999' formatting", false);
        return false;
    }

    print_summary("test_case_gamma_formatting_consistent", true);
    return true;
}

static bool test_case_expand16_propagation() {
    // Verification that pm->test_gamma_expand16 is carried through
    reset_captured_calls();

    png_modifier pm;
    pm.test_gamma_expand16 = 42;

    gamma_transform_test(&pm, 2, 8, 0, 0, 0.111, 0.222, 4, 0, 0);

    if (g_captured_calls.size() != 1) {
        print_summary("test_case_expand16_propagation: expected 1 gamma_test call", false);
        return false;
    }

    if (g_captured_calls[0].expand16 != 42) {
        print_summary("test_case_expand16_propagation: expand16 did not propagate from pm", false);
        return false;
    }

    print_summary("test_case_expand16_propagation", true);
    return true;
}

// Main test runner
int main() {
    int total = 0;
    int passed = 0;

    // Run individual test cases with descriptive comments.
    total++; if (test_case_sbit_branch_true()) passed++;

    total++; if (test_case_sbit_branch_false_with_scale16()) passed++;

    total++; if (test_case_gamma_formatting_consistent()) passed++;

    total++; if (test_case_expand16_propagation()) passed++;

    // Summary
    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed to aid integration with build systems that
    // treat non-zero as test failure.
    return (passed == total) ? 0 : 1;
}
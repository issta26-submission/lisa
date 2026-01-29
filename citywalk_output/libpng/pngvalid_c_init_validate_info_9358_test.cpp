// Automated unit tests for init_validate_info (pngvalid.c) using a lightweight, non-GTest approach.
// This test suite is written in C++11 and links against the libpng-based pngvalid.c test harness.
// It exercises key branches in init_validate_info by constructing gamma_display and validate_info
// structures and invoking init_validate_info. The tests use non-terminating assertions so that
// all tests can run and report a summary.
//
// Notes:
// - The tests rely on the public interfaces declared in pngvalid.h and libpng headers (png.h).
// - We initialize the internal structures with minimal, valid data (via modifier_init) to avoid
//   hidden null-pointer dereferences in the helper functions (abserr, pcerr, etc.).
// - The test suite avoids GTest and uses a small internal assertion mechanism.
// - The test code is designed to compile under C++11 with a C linkage for the PNG/libpng headers.

#include <pngvalid.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Forward declarations of C APIs (from libpng/pngvalid.h). We compile this as C++11 and link with the C code.
extern "C" {
}

// Lightweight test harness
static int g_pass = 0;
static int g_fail = 0;

// Simple approximate equality for doubles
static bool approx_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Non-terminating assertion helpers
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << std::endl;
        ++g_fail;
    } else {
        ++g_pass;
    }
}

// Helper to initialize a valid png_modifier using provided API
static void init_modifier(png_modifier *pm) {
    // modifier_init is declared in pngvalid.h; it sets up internal state to a sane default.
    // If the function isn't available in the given environment, this will fail to link.
    modifier_init(pm);
}

// Test 1: SBIT branch - dp->sbit == 0 should set vi->sbit to in_depth and isbit_shift to 0
static bool test_init_validate_info_sbit_branch_else() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;
    // Case: sbit not set (0) => use in_depth
    dp.sbit = 0;
    // Minimal non-branching fields
    dp.screen_gamma = 0.5;   // not used for this branch
    dp.file_gamma = 1.0;
    dp.use_input_precision = 0;
    dp.this.colour_type = 0;
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.background_gamma = 1.0;
    dp.do_background = 0;
    dp.background_color.red = 0;
    dp.background_color.green = 0;
    dp.background_color.blue = 0;
    dp.scale16 = 0;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    const int in_depth = 8;
    const int out_depth = 8;

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    bool ok = (vi.sbit == in_depth) && (vi.isbit_shift == 0) && (vi.sbit_max == ((1U << in_depth) - 1));
    return ok;
}

// Test 2: SBIT branch - dp->sbit > 0 and dp->sbit < in_depth should set vi->sbit and isbit_shift
static bool test_init_validate_info_sbit_branch_true() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    // Case: sbit explicitly smaller than in_depth
    dp.sbit = 4;
    dp.this.colour_type = 0;
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.screen_gamma = 0.5;
    dp.file_gamma = 1.0;
    dp.use_input_precision = 0;
    dp.do_background = 0;
    dp.background_gamma = 1.0;
    dp.background_color.red = 0;
    dp.background_color.green = 0;
    dp.background_color.blue = 0;
    dp.scale16 = 0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    bool ok = (vi.sbit == dp.sbit) && (vi.isbit_shift == (in_depth - dp.sbit)) && (vi.sbit_max == ((1U << dp.sbit) - 1));
    return ok;
}

// Test 3: Screen gamma near 1 threshold should zero screen_gamma and screen_inverse, and gamma_correction near 0
static bool test_init_validate_info_screen_gamma_threshold() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    // Set screen_gamma exactly 1.0 and file_gamma to 1.0 to trigger threshold behavior
    dp.screen_gamma = 1.0;
    dp.file_gamma = 1.0;

    // Other fields to satisfy function flow
    dp.this.colour_type = 0;
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.sbit = 8; // arbitrary valid value
    dp.use_input_precision = 0;
    dp.do_background = 0;
    dp.background_gamma = 1.0;
    dp.background_color.red = 0;
    dp.background_color.green = 0;
    dp.background_color.blue = 0;
    dp.scale16 = 0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    bool ok = approx_equal(vi.screen_gamma, 0.0) &&
              approx_equal(vi.screen_inverse, 0.0) &&
              approx_equal(vi.gamma_correction, 0.0) &&
              approx_equal(vi.file_inverse, 0.0); // file_inverse should also be zero when file_gamma == 1.0
    return ok;
}

// Test 4: Background processing when colour_type indicates alpha/has_tRNS and do_background != 0
static bool test_init_validate_info_background_processing() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    // Trigger background processing path
    dp.this.colour_type = 0; // uses has_tRNS path
    dp.this.has_tRNS = 1;
    dp.this.is_transparent = 0;
    dp.do_background = 1;
    dp.background_gamma = 2.0;

    // Background color (red component)
    int outmax = 255; // for in_depth=8, out_depth=8
    dp.background_color.red = 128;
    dp.background_color.green = 0;
    dp.background_color.blue = 0;

    // Gamma inputs
    dp.screen_gamma = 1.0;
    dp.file_gamma = 1.0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    // Expect do_background to be carried through and a non-zero background_red computed
    double r_expected = 128.0 / static_cast<double>(outmax);
    double bg_inverse = 1.0 / dp.background_gamma; // 0.5
    double r_expected_after = std::pow(r_expected, bg_inverse);

    bool ok_background = std::fabs(vi.background_red - r_expected_after) < 1e-2;
    bool ok_do_background = (vi.do_background != 0);
    // Green and blue should be computed as well but remain near 0
    bool ok_green = approx_equal(vi.background_green, 0.0, 1e-6);
    bool ok_blue  = approx_equal(vi.background_blue, 0.0, 1e-6);

    return ok_do_background && ok_background && ok_green && ok_blue;
}

// Test 5: No-background path when colour_type does not imply background support
static bool test_init_validate_info_no_background_path() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    // Colour type that should not trigger background processing
    dp.this.colour_type = 1; // e.g., PALETTE type
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.do_background = 1; // even if requested, should be ignored
    dp.background_gamma = 1.0;
    dp.background_color.red = 100;
    dp.background_color.green = 150;
    dp.background_color.blue = 200;

    dp.screen_gamma = 1.0;
    dp.file_gamma = 1.0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    // Since the condition is not met, do_background must be 0 and colors reset to 0
    bool ok_do_background_zero = (vi.do_background == 0);
    bool ok_bg_zero = (approx_equal(vi.background_red, 0.0) && approx_equal(vi.background_green, 0.0) && approx_equal(vi.background_blue, 0.0));

    return ok_do_background_zero && ok_bg_zero;
}

// Test 6: File gamma inverse threshold (fabs(file_inverse - 1) near 0) should reset to 0
static bool test_init_validate_info_file_inverse_threshold() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    // file_gamma == 1.0 should trigger file_inverse to 0 due to threshold
    dp.file_gamma = 1.0;
    dp.screen_gamma = 0.5;
    dp.this.colour_type = 0;
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.do_background = 0;
    dp.background_gamma = 1.0;
    dp.background_color.red = 0;
    dp.background_color.green = 0;
    dp.background_color.blue = 0;
    dp.sbit = 0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    // file_inverse should be 0 due to threshold
    bool ok = approx_equal(vi.file_inverse, 0.0, 1e-6);
    return ok;
}

// Test 7: Scale16 propagation
static bool test_init_validate_info_scale16_propagation() {
    png_modifier pm;
    init_modifier(&pm);

    gamma_display dp;
    std::memset(&dp, 0, sizeof(dp));
    dp.pm = &pm;

    dp.scale16 = 1;
    dp.file_gamma = 1.0;
    dp.screen_gamma = 1.0;
    dp.this.colour_type = 0;
    dp.this.has_tRNS = 0;
    dp.this.is_transparent = 0;
    dp.do_background = 0;
    dp.background_gamma = 1.0;

    const int in_depth = 8;
    const int out_depth = 8;

    validate_info vi;
    std::memset(&vi, 0, sizeof(vi));

    init_validate_info(&vi, &dp, (png_const_structp)nullptr, in_depth, out_depth);

    bool ok = (vi.scale16 == dp.scale16);
    return ok;
}

// Runner
int main() {
    std::cout << "pngvalid init_validate_info unit tests (C++11) starting...\n";

    auto run_bool = [](const char* testname, bool ok) {
        if (ok) {
            ++g_pass;
            std::cout << "[PASS] " << testname << "\n";
        } else {
            ++g_fail;
            std::cout << "[FAIL] " << testname << "\n";
        }
    };

    run_bool("test_init_validate_info_sbit_branch_else",
             test_init_validate_info_sbit_branch_else());
    run_bool("test_init_validate_info_sbit_branch_true",
             test_init_validate_info_sbit_branch_true());
    run_bool("test_init_validate_info_screen_gamma_threshold",
             test_init_validate_info_screen_gamma_threshold());
    run_bool("test_init_validate_info_background_processing",
             test_init_validate_info_background_processing());
    run_bool("test_init_validate_info_no_background_path",
             test_init_validate_info_no_background_path());
    run_bool("test_init_validate_info_file_inverse_threshold",
             test_init_validate_info_file_inverse_threshold());
    run_bool("test_init_validate_info_scale16_propagation",
             test_init_validate_info_scale16_propagation());

    std::cout << "Tests run: " << (g_pass + g_fail)
              << " | Passed: " << g_pass
              << " | Failed: " << g_fail << "\n";

    // Return non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}
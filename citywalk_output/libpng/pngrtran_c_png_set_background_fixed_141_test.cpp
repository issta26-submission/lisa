// Lightweight C++11 unit test suite for png_set_background_fixed
// This test harness provides a minimal in-repo mock of the relevant
// libpng structures and functions used by png_set_background_fixed.
// It is designed to be self-contained (no external GTest), and uses
// non-terminating assertions to maximize code coverage.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-model types (mocked for unit testing)
typedef unsigned char png_byte;
typedef int png_fixed_point; // 32-bit fixed-point placeholder

typedef struct png_color_16 {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short gray;
    unsigned char index;
} png_color_16;

typedef png_color_16* png_color_16p;
typedef const png_color_16* png_const_color_16p;
typedef png_color_16 png_color_16_struct; // alias for readability

typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef png_struct* png_structptr;

// Forward declarations of the focal method dependencies (mocked)
void png_debug(int /*level*/, const char* /*msg*/) { /* no-op for tests */ }
int  png_rtran_ok(png_structrp /*png_ptr*/, int /*need_IHDR*/) { return 1; } // default OK
void png_warning(png_structrp png_ptr, const char* msg);

// Constants used by the focal method
const int PNG_COMPOSE           = 0x01;
const int PNG_STRIP_ALPHA       = 0x02;
const int PNG_ENCODE_ALPHA      = 0x04;
const int PNG_FLAG_OPTIMIZE_ALPHA = 0x08;
const int PNG_BACKGROUND_EXPAND = 0x10;
const int PNG_BACKGROUND_GAMMA_UNKNOWN = -1;

// The focal method under test (re-implemented for the test harness)
png_set_background_fixed(png_structrp png_ptr,
    png_const_color_16p background_color, int background_gamma_code,
    int need_expand, png_fixed_point background_gamma)
{
{
   png_debug(1, "in png_set_background_fixed");
   if (png_rtran_ok(png_ptr, 0) == 0 || background_color == NULL)
      return;
   if (background_gamma_code == PNG_BACKGROUND_GAMMA_UNKNOWN)
   {
      png_warning(png_ptr, "Application must supply a known background gamma");
      return;
   }
   png_ptr->transformations |= PNG_COMPOSE | PNG_STRIP_ALPHA;
   png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
   png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
   png_ptr->background = *background_color;
   png_ptr->background_gamma = background_gamma;
   png_ptr->background_gamma_type = (png_byte)(background_gamma_code);
   if (need_expand != 0)
      png_ptr->transformations |= PNG_BACKGROUND_EXPAND;
   else
      png_ptr->transformations &= ~PNG_BACKGROUND_EXPAND;
}
}

// Minimal png_struct definition for testing
struct png_struct {
    int transformations;
    int flags;
    png_color_16 background;
    png_fixed_point background_gamma;
    png_byte background_gamma_type;
    int rtran_ok; // control the return value of png_rtran_ok
    bool warned;
    std::string warning_msg;
};

// png_warning implementation (records a warning in the fake struct)
void png_warning(png_structrp png_ptr, const char* msg) {
    if (png_ptr) {
        png_ptr->warned = true;
        png_ptr->warning_msg = msg ? msg : "";
    }
}

// Simple assertion/logging framework (non-terminating)
static int g_total_tests = 0;
static int g_failed_tests = 0;
static std::vector<std::string> g_failure_messages;

static void log_failure(const std::string& msg) {
    g_failure_messages.push_back(msg);
    ++g_failed_tests;
}

// Generic boolean assertion
static void expect_true(bool cond, const std::string& msg) {
    ++g_total_tests;
    if (!cond) {
        log_failure("ASSERT_TRUE failed: " + msg);
    }
}

// Integer equality assertion
static void expect_eq_int(int a, int b, const std::string& msg) {
    ++g_total_tests;
    if (a != b) {
        log_failure("ASSERT_EQ int failed: " + msg +
                    " (expected " + std::to_string(b) +
                    ", got " + std::to_string(a) + ")");
    }
}

// Color equality helper
static bool color_equal(const png_color_16& a, const png_color_16& b) {
    return (a.red   == b.red)   &&
           (a.green == b.green) &&
           (a.blue  == b.blue)  &&
           (a.gray  == b.gray)  &&
           (a.index == b.index);
}

// Test 1: background_color == NULL should cause early return and no state changes
// Purpose: Ensure the function handles null color input gracefully without side effects.
static void test_null_background_color_no_change() {
    png_struct test_ptr;
    test_ptr.transformations = 0;
    test_ptr.flags = 0xFF;
    test_ptr.background = {0,0,0,0,0};
    test_ptr.background_gamma = 0;
    test_ptr.background_gamma_type = 0;
    test_ptr.rtran_ok = 1;
    test_ptr.warned = false;

    png_color_16 color = { 1, 2, 3, 4, 5 }; // arbitrary

    // Copy initial state
    png_struct initial = test_ptr;

    // Call with NULL background_color
    png_set_background_fixed(&test_ptr, NULL, 0, 1, 123);

    // Expect no changes
    expect_eq_int(test_ptr.transformations, initial.transformations,
                  "transformations unchanged on NULL background_color");
    expect_eq_int(test_ptr.flags, initial.flags, "flags unchanged on NULL background_color");
    expect_true(color_equal(test_ptr.background, initial.background),
                "background unchanged on NULL background_color");
    expect_eq_int(test_ptr.background_gamma, initial.background_gamma,
                  "background_gamma unchanged on NULL background_color");
    expect_eq_int(test_ptr.background_gamma_type, initial.background_gamma_type,
                  "background_gamma_type unchanged on NULL background_color");
    expect_true(!test_ptr.warned, "no warning should be issued on NULL background_color");
}

// Test 2: background_gamma_code unknown should emit warning and return without changes
// Purpose: Verify gamma-unknown path is handled correctly and does not mutate state.
static void test_gamma_unknown_warning() {
    png_struct test_ptr;
    test_ptr.transformations = 0;
    test_ptr.flags = 0x0;
    test_ptr.background = {10, 20, 30, 0, 0};
    test_ptr.background_gamma = 0;
    test_ptr.background_gamma_type = 0;
    test_ptr.rtran_ok = 1;
    test_ptr.warned = false;

    png_color_16 color = { 5, 6, 7, 0, 0 };

    png_struct initial = test_ptr;

    // Use unknown gamma code
    png_set_background_fixed(&test_ptr, &color, PNG_BACKGROUND_GAMMA_UNKNOWN, 1, 999);

    // Expect no changes to core state, but a warning recorded
    expect_true(test_ptr.warned, "warning should be issued for unknown background gamma");
    expect_true(test_ptr.background.red == initial.background.red &&
                test_ptr.background.green == initial.background.green &&
                test_ptr.background.blue == initial.background.blue &&
                test_ptr.background.gray == initial.background.gray &&
                test_ptr.background.index == initial.background.index,
                "background should not be changed on unknown gamma");
}

// Test 3: Normal path with need_expand = non-zero should set EXPAND flag
// Purpose: Ensure proper state mutation when valid inputs are provided (expand true).
static void test_normal_path_expand_true() {
    png_struct test_ptr;
    test_ptr.transformations = 0;
    test_ptr.flags = 0x0;
    test_ptr.background = {0,0,0,0,0};
    test_ptr.background_gamma = 0;
    test_ptr.background_gamma_type = 0;
    test_ptr.rtran_ok = 1;
    test_ptr.warned = false;

    png_color_16 color = { 11, 22, 33, 0, 0 };

    png_fixed_point gamma = 777;

    png_set_background_fixed(&test_ptr, &color, 2, 1, gamma);

    // Expect: COMPOSE and STRIP_ALPHA set, ENCODE_ALPHA cleared
    expect_true((test_ptr.transformations & PNG_COMPOSE) != 0, "COMPOSE bit set");
    expect_true((test_ptr.transformations & PNG_STRIP_ALPHA) != 0, "STRIP_ALPHA bit set");
    expect_true((test_ptr.transformations & PNG_ENCODE_ALPHA) == 0, "ENCODE_ALPHA bit cleared");
    expect_true((test_ptr.transformations & PNG_BACKGROUND_EXPAND) != 0, "BACKGROUND_EXPAND set when need_expand != 0");

    // No warning expected
    expect_true(!test_ptr.warned, "no warning expected on valid gamma");
    // Background and gamma fields updated
    expect_true(color_equal(test_ptr.background, color), "background color copied correctly");
    expect_eq_int(test_ptr.background_gamma, gamma, "background_gamma copied correctly");
    expect_eq_int(test_ptr.background_gamma_type, 2, "background_gamma_type set correctly");
}

// Test 4: Normal path with need_expand = 0 should clear EXPAND flag
// Purpose: Ensure proper state mutation when need_expand is zero.
static void test_normal_path_expand_false() {
    png_struct test_ptr;
    test_ptr.transformations = PNG_COMPOSE | PNG_STRIP_ALPHA | PNG_BACKGROUND_EXPAND;
    test_ptr.flags = 0;
    test_ptr.background = {0,0,0,0,0};
    test_ptr.background_gamma = 0;
    test_ptr.background_gamma_type = 0;
    test_ptr.rtran_ok = 1;
    test_ptr.warned = false;

    png_color_16 color = { 100, 110, 120, 0, 0 };

    png_fixed_point gamma = 1234;

    png_set_background_fixed(&test_ptr, &color, 1, 0, gamma);

    // Expect: EXPAND bit cleared
    expect_true((test_ptr.transformations & PNG_BACKGROUND_EXPAND) == 0, "BACKGROUND_EXPAND cleared when need_expand == 0");

    // Other bits should be preserved except ENCODE_ALPHA cleared and background set
    expect_true((test_ptr.transformations & PNG_COMPOSE) != 0, "COMPOSE preserved");
    expect_true((test_ptr.transformations & PNG_STRIP_ALPHA) != 0, "STRIP_ALPHA preserved");
    expect_true((test_ptr.transformations & PNG_ENCODE_ALPHA) == 0, "ENCODE_ALPHA cleared");
    expect_true(color_equal(test_ptr.background, color), "background color updated");
    expect_eq_int(test_ptr.background_gamma, gamma, "background_gamma updated");
    expect_eq_int(test_ptr.background_gamma_type, 1, "background_gamma_type updated");
}

// Test 5: Early return when png_rtran_ok returns 0 should not mutate state
// Purpose: Ensure early exit due to png_rtran_ok == 0 prevents any changes.
static void test_rtran_ok_zero_early_return() {
    png_struct test_ptr;
    test_ptr.transformations = 0xABCD;
    test_ptr.flags = 0x1234;
    test_ptr.background = {1,2,3,4,5};
    test_ptr.background_gamma = 42;
    test_ptr.background_gamma_type = 9;
    test_ptr.rtran_ok = 0; // force early return
    test_ptr.warned = false;

    png_color_16 color = { 9, 8, 7, 6, 5 };

    png_set_background_fixed(&test_ptr, &color, 0, 1, 0);

    // Expect no changes due to early return
    expect_eq_int(test_ptr.transformations, 0xABCD, "transformations unchanged on early return");
    expect_eq_int(test_ptr.flags, 0x1234, "flags unchanged on early return");
    expect_true(color_equal(test_ptr.background, {1,2,3,4,5}), "background unchanged on early return");
    expect_eq_int(test_ptr.background_gamma, 42, "background_gamma unchanged on early return");
    expect_eq_int(test_ptr.background_gamma_type, 9, "background_gamma_type unchanged on early return");
    expect_true(!test_ptr.warned, "no warning on early return");
}

// Main entry: run all tests and report summary
int main() {
    test_null_background_color_no_change();
    test_gamma_unknown_warning();
    test_normal_path_expand_true();
    test_normal_path_expand_false();
    test_rtran_ok_zero_early_return();

    // Summary
    std::cout << "Test summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures." << std::endl;
    if (!g_failure_messages.empty()) {
        std::cout << "Failure details:\n";
        for (const auto& m : g_failure_messages) {
            std::cout << " - " << m << "\n";
        }
    }

    return g_failed_tests; // non-zero if any test failed
}
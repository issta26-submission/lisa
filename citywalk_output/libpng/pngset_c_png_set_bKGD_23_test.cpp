// Lightweight C++11 unit test suite for the focal function png_set_bKGD
// This test suite is self-contained (no GTest) and uses a small
// custom test harness with non-terminating checks.
// The goal is to validate the behavior of png_set_bKGD under various inputs.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// -------------------------------------------------------------
// Minimal stand-in types and function to emulate the focal API
// (This is a simplified, self-contained recreation for tests of
// the provided function logic. It is not a full libpng implementation.)
// ------------------------------------------------------------

// Representation of the 16-bit color structure used by libpng
struct png_color_16_def {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t gray;
    uint16_t index;
};
typedef png_color_16_def png_color_16;
typedef png_color_16_def* png_color_16p;
typedef const png_color_16_def* png_const_color_16p;

// Forward declaration of the private info structure (minimal subset)
struct png_info_def {
    unsigned int valid;
    png_color_16 background;
};
// Pointers used by the API
typedef struct png_struct_def { int dummy; } png_struct_def;
typedef png_struct_def* png_const_structrp;  // non-const pointer alias for test ease
typedef png_struct_def* png_structrp;        // non-const
typedef png_info_def* png_inforp;

// A tiny placeholder for the debugging hook used by the function
static void png_debug1(int /*level*/, const char* /*fmt*/, const char* /*arg*/)
{
    // Intentionally empty for tests to avoid noisy output
}

// A macro-like constant used by the API (bit mask for the valid field)
#define PNG_INFO_bKGD 0x01U

// The focal function under test (stand-in implementation matching the provided code)
void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr,
    png_const_color_16p background)
{
{
   png_debug1(1, "in %s storage function", "bKGD");
   if (png_ptr == NULL || info_ptr == NULL || background == NULL)
      return;
   info_ptr->background = *background;
   info_ptr->valid |= PNG_INFO_bKGD;
}
}

// Helper to compare two png_color_16 structures (content comparison)
static bool equal_color(const png_color_16& a, const png_color_16& b)
{
    return a.red == b.red && a.green == b.green && a.blue == b.blue &&
           a.gray == b.gray && a.index == b.index;
}

// Simple test harness state
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Helper to report test result (non-terminating)
static void report_test(const char* name, bool result)
{
    ++g_tests_run;
    if (result) {
        ++g_tests_passed;
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << "\n";
    }
}

// -------------------------------------------------------------
// Test 1: Null png_ptr should cause no modifications to info_ptr
// This validates the true-branch is skipped when png_ptr is NULL.
// -------------------------------------------------------------
static bool test_null_png_ptr()
{
    // Arrange
    png_struct_def s = {0};
    png_const_structrp png_ptr = nullptr; // NULL input
    png_info_def info = {0};
    info.valid = 0;
    info.background.red = 0; info.background.green = 0; info.background.blue = 0;
    info.background.gray = 0; info.background.index = 0;

    png_color_16 back;
    back.red = 10; back.green = 20; back.blue = 30; back.gray = 40; back.index = 5;

    // Act
    png_set_bKGD(png_ptr, &info, &back);

    // Assert: info_ptr should be unchanged
    bool ok = (info.valid == 0) &&
              equal_color(info.background, back) == false; // since we expect no change, the background should still be zero
    // We compare to initial zeros
    ok = (info.valid == 0) && (info.background.red == 0) && (info.background.green == 0) &&
         (info.background.blue == 0) && (info.background.gray == 0) && (info.background.index == 0);

    report_test("test_null_png_ptr", ok);
    return ok;
}

// -------------------------------------------------------------
// Test 2: Null info_ptr should be a no-op (no crash, no modification happens)
// -------------------------------------------------------------
static bool test_null_info_ptr()
{
    // Arrange
    png_struct_def s = {0};
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(&s);
    png_inforp info_ptr = nullptr; // NULL
    png_color_16 back;
    back.red = 1; back.green = 2; back.blue = 3; back.gray = 4; back.index = 5;

    // Act
    png_set_bKGD(png_ptr, info_ptr, &back);

    // Since info_ptr is NULL, there is nothing to assert about,
    // but we ensure the function did not crash and nothing to dereference occurred.
    // The test passes if this point is reached without crash.
    report_test("test_null_info_ptr", true);
    return true;
}

// -------------------------------------------------------------
// Test 3: Null background pointer should be a no-op
// -------------------------------------------------------------
static bool test_null_background_ptr()
{
    // Arrange
    png_struct_def s = {0};
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(&s);
    png_inforp info_ptr = &((png_info_def){0});
    info_ptr->valid = 0;
    info_ptr->background.red = 9; info_ptr->background.green = 9;
    info_ptr->background.blue = 9; info_ptr->background.gray = 9;
    info_ptr->background.index = 9;

    // Act
    png_set_bKGD(png_ptr, info_ptr, nullptr);

    // Assert: info_ptr should be unchanged
    bool ok = (info_ptr->valid == 0) &&
              equal_color(info_ptr->background, (png_color_16){0,0,0,0,0});
    // Since info_ptr background should be unchanged from initialization, it should still be 9 in all fields.
    ok = (info_ptr->valid == 0) &&
         (info_ptr->background.red == 9) && (info_ptr->background.green == 9) &&
         (info_ptr->background.blue == 9) && (info_ptr->background.gray == 9) &&
         (info_ptr->background.index == 9);

    report_test("test_null_background_ptr", ok);
    return ok;
}

// -------------------------------------------------------------
// Test 4: All inputs valid; verify copy and valid flag set
// -------------------------------------------------------------
static bool test_copy_background_sets_valid_and_copies()
{
    // Arrange
    png_struct_def s = {0};
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(&s);
    png_inforp info_ptr = new png_info_def();
    info_ptr->valid = 0;
    info_ptr->background.red = 0; info_ptr->background.green = 0;
    info_ptr->background.blue = 0; info_ptr->background.gray = 0;
    info_ptr->background.index = 0;

    png_color_16 back;
    back.red = 11; back.green = 22; back.blue = 33; back.gray = 44; back.index = 7;

    // Act
    png_set_bKGD(png_ptr, info_ptr, &back);

    // Assert: background should be copied and PNG_INFO_bKGD bit set
    bool ok = equal_color(info_ptr->background, back) &&
              ((info_ptr->valid & PNG_INFO_bKGD) != 0);

    report_test("test_copy_background_sets_valid_and_copies", ok);
    delete info_ptr; // cleanup
    return ok;
}

// -------------------------------------------------------------
// Test 5: Existing non-zero valid flags should be preserved and OR-ed with bKGD flag
// -------------------------------------------------------------
static bool test_existing_valid_preserved_and_ored()
{
    // Arrange
    png_struct_def s = {0};
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(&s);
    png_inforp info_ptr = new png_info_def();
    info_ptr->valid = 0xAA; // some pre-existing flags
    info_ptr->background.red = 0; info_ptr->background.green = 0;
    info_ptr->background.blue = 0; info_ptr->background.gray = 0;
    info_ptr->background.index = 0;

    png_color_16 back;
    back.red = 5; back.green = 6; back.blue = 7; back.gray = 8; back.index = 9;

    // Act
    png_set_bKGD(png_ptr, info_ptr, &back);

    // Assert: existing flags preserved and OR-ed with bKGD
    bool ok = (info_ptr->valid & 0xAA) == 0xAA;
    ok = ok && ((info_ptr->valid & PNG_INFO_bKGD) != 0);

    report_test("test_existing_valid_preserved_and_ored", ok);
    delete info_ptr;
    return ok;
}

// -------------------------------------------------------------
// Main: run all tests
// -------------------------------------------------------------
int main()
{
    // Run tests and print summary
    test_null_png_ptr();
    test_null_info_ptr();
    test_null_background_ptr();
    test_copy_background_sets_valid_and_copies();
    test_existing_valid_preserved_and_ored();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_passed << " / " << g_tests_run << " tests passed.\n";

    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
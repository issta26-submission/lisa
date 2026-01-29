/*
Unit test suite for the focal method: png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask)

Step-by-step alignment with the provided instructions:
- Step 1 (Program Understanding): The focal method clears bits in info_ptr->valid when both png_ptr and info_ptr are non-null.
  Core candidate keywords: png_ptr, info_ptr, valid, mask, bitwise NOT (~mask), NULL, conditional check.

- Step 2 (Unit Test Generation): Tests focus on:
  - Both pointers non-null: valid bits are cleared by mask.
  - One or both pointers NULL: no modification to info_ptr->valid (guard clause).
  - Edge cases for mask (e.g., high-bit masks, zero mask).
  - Access path uses PNG's public API (png_create_read_struct, png_create_info_struct) to obtain valid info_ptr structures.

- Step 3 (Test Case Refinement): Tests assembled to maximize coverage of true/false branches and boundary conditions; non-terminating assertions are used (test continues after a failure).

Notes:
- This test uses libpng public API to create a real png_info structure with a valid bitfield, ensuring compatibility with the real png_set_invalid implementation.
- Tests run from main() without a testing framework (no GTest/GMock), and report failures to stdout while continuing execution.
- Each test includes comments describing purpose, approach, and expectations.

Code begins here. Ensure libpng development headers and library are available in the include/library search paths when compiling.
*/

#include <png.h>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Global counters to summarize test results.
// Following the domain knowledge, we use non-terminating assertions by collecting failures.
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to report test results without aborting the test suite.
static void report_result(const std::string& test_name, bool passed, const std::string& details = "") {
    ++g_total_tests;
    if (passed) {
        std::cout << "[OK] " << test_name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << test_name;
        if (!details.empty()) {
            std::cout << " - " << details;
        }
        std::cout << std::endl;
    }
}

/*
Test 1: test_png_set_invalid_clears_bits
Purpose:
  Verify that when both png_ptr and info_ptr are non-null, the function clears the bits in info_ptr->valid according to mask.
Approach:
  - Create a real PNG read struct and PNG info struct via libpng APIs.
  - Initialize info_ptr->valid to a known value (e.g., 0xFFFF'FFFF).
  - Call png_set_invalid(png_ptr, info_ptr, mask) with a specific mask.
  - Expect info_ptr->valid == (0xFFFF'FFFF & ~mask).
*/
static void test_png_set_invalid_clears_bits() {
    const char* test_name = "test_png_set_invalid_clears_bits";
    // Setup: create png_ptr and info_ptr
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        report_result(test_name, false, "Failed to create png_ptr");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        report_result(test_name, false, "Failed to create info_ptr");
        return;
    }

    // Initialize info_ptr's valid bitfield
    info_ptr->valid = 0xFFFFFFFFu; // All bits set initially

    // Define a non-zero mask to clear specific bits
    int mask = 0x0F0F0F0F;
    png_set_invalid(png_ptr, info_ptr, mask);

    unsigned int expected = 0xFFFFFFFFu & (unsigned int)(~mask);
    if (info_ptr->valid != expected) {
        report_result(test_name, false,
            "info_ptr->valid was not cleared correctly for non-null pointers");
    } else {
        report_result(test_name, true);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

/*
Test 2: test_png_set_invalid_png_ptr_null
Purpose:
  Ensure that when png_ptr is NULL and info_ptr is non-null, the function does nothing (no modification).
Approach:
  - Create a valid info_ptr and set a known value for valid.
  - Call png_set_invalid with png_ptr = NULL.
  - Expect info_ptr->valid to remain unchanged.
*/
static void test_png_set_invalid_png_ptr_null() {
    const char* test_name = "test_png_set_invalid_png_ptr_null";
    // Setup: create png_ptr as NULL and a valid info_ptr
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // To obtain a valid info_ptr, create a temporary PNG object
    png_structp tmp_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (tmp_png == NULL) {
        report_result(test_name, false, "Failed to create temporary png_ptr for info_ptr allocation");
        return;
    }
    info_ptr = png_create_info_struct(tmp_png);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&tmp_png, NULL, NULL);
        report_result(test_name, false, "Failed to create temporary info_ptr for test");
        return;
    }

    info_ptr->valid = 0xA5A5A5A5u;

    int mask = 0x0F0F0F0F;
    // png_ptr is NULL; function should not modify info_ptr->valid
    png_set_invalid(png_ptr, info_ptr, mask);

    if (info_ptr->valid != 0xA5A5A5A5u) {
        report_result(test_name, false, "info_ptr->valid changed when png_ptr was NULL");
    } else {
        report_result(test_name, true);
    }

    // Cleanup
    png_destroy_read_struct(&tmp_png, &info_ptr, NULL);
    // png_ptr is NULL; nothing to destroy for it
}

/*
Test 3: test_png_set_invalid_info_ptr_null
Purpose:
  Ensure that when info_ptr is NULL and png_ptr is non-null, the function does nothing (no crash).
Approach:
  - Create a valid png_ptr via libpng API.
  - Call png_set_invalid with info_ptr = NULL.
  - No assertion on memory, but the call should complete safely without modifying anything or crashing.
*/
static void test_png_set_invalid_info_ptr_null() {
    const char* test_name = "test_png_set_invalid_info_ptr_null";
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        report_result(test_name, false, "Failed to create png_ptr");
        return;
    }

    int mask = 0x12345678;
    // info_ptr is NULL; function should simply return without dereferencing
    png_set_invalid(png_ptr, NULL, mask);

    // If we reach here without crash, test passes logically (no change to a non-existent info_ptr)
    report_result(test_name, true);

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

/*
Test 4: test_png_set_invalid_zero_mask_no_change
Purpose:
  Ensure that a zero mask does not modify info_ptr->valid (since ~0 is all-ones, but AND with existing bits should leave unchanged).
Approach:
  - Create a valid png_ptr and info_ptr; set info_ptr->valid to an arbitrary value.
  - Call with mask = 0.
  - Expect info_ptr->valid to remain unchanged.
*/
static void test_png_set_invalid_zero_mask_no_change() {
    const char* test_name = "test_png_set_invalid_zero_mask_no_change";
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        report_result(test_name, false, "Failed to create png_ptr");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        report_result(test_name, false, "Failed to create info_ptr");
        return;
    }

    info_ptr->valid = 0x1A2B3C4Du;
    int mask = 0; // zero mask should leave valid unchanged
    png_set_invalid(png_ptr, info_ptr, mask);

    if (info_ptr->valid != 0x1A2B3C4Du) {
        report_result(test_name, false, "info_ptr->valid changed with zero mask");
    } else {
        report_result(test_name, true);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

int main() {
    std::cout << "Starting tests for png_set_invalid (non-GTest style)\n";

    test_png_set_invalid_clears_bits();
    test_png_set_invalid_png_ptr_null();
    test_png_set_invalid_info_ptr_null();
    test_png_set_invalid_zero_mask_no_change();

    std::cout << "Test Summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures.\n";

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}
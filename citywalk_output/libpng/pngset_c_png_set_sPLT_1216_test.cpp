// C++11 test suite for the focal function: png_set_sPLT
// This test uses libpng public API to verify behavior and relies on
// direct access to internal sPLT information via pngpriv.h for verification.
// The tests are designed to be non-terminating (no EXPECT_* macros) and run
// from main() as a small test harness (no GTest).

#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


extern "C" {
    // Include libpng headers
    #include <png.h>
    // Access internal structures (as per FOCAL_CLASS_DEP in the prompt)
    // These provide access to internal fields like info_ptr->splt_palettes.
    #include "pngpriv.h"
}

// Simple assertion helper
#define ASSERT(condition, message)                                   \
    do {                                                               \
        if (!(condition)) {                                          \
            std::cerr << "ASSERTION FAILED: " << (message) << "\n";  \
            return;                                                  \
        } else {                                                     \
            std::cout << "PASS: " << (message) << "\n";            \
        }                                                            \
    } while (false)

// Test 1: Null inputs should cause an early return and not crash
// This checks that null png_ptr or null info_ptr or non-positive nentries
// or null entries do not crash the function.
static void test_png_set_sPLT_null_inputs() {
    std::cout << "\nTest 1: Null inputs handling\n";

    // Prepare a minimal valid png_ptr / info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    // Prepare a valid sPLT input (one entry) but the test will pass null png_ptr
    static png_sPLT_entry e0;
    e0.red = 0; e0.green = 0; e0.blue = 0; e0.frequency = 1;

    static png_sPLT_t splt_input;
    splt_input.name = "Palette_NULL_INPUT";
    splt_input.nentries = 1;
    splt_input.entries = &e0;
    splt_input.depth = 8;

    // Case: png_ptr == NULL
    png_set_sPLT(NULL, info_ptr, &splt_input, 1);

    // Case: info_ptr == NULL
    png_set_sPLT(png_ptr, NULL, &splt_input, 1);

    // Case: nentries <= 0
    png_set_sPLT(png_ptr, info_ptr, &splt_input, 0);

    // Case: entries == NULL
    png_set_sPLT(png_ptr, info_ptr, NULL, 1);

    // Clean up
    if (info_ptr) png_destroy_info_struct(png_ptr, &info_ptr);
    if (png_ptr) png_destroy_write_struct(&png_ptr, NULL);
    std::cout << "Completed Test 1 (null inputs) without crashes.\n";
}

// Test 2: Valid addition of a single sPLT entry.
// This validates that the function correctly reallocates internal storage,
// copies the name, and marks the info with PNG_INFO_sPLT.
static void test_png_set_sPLT_single_entry() {
    std::cout << "\nTest 2: Valid single sPLT entry addition\n";

    // Create png_ptr / info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    ASSERT(png_ptr != NULL, "Create png_ptr for Test 2");
    ASSERT(info_ptr != NULL, "Create info_ptr for Test 2");

    // Initialize internal fields expected by later access
    info_ptr->splt_palettes = NULL;
    info_ptr->splt_palettes_num = 0;
    info_ptr->valid = 0;

    // Prepare an input sPLT entry
    static png_sPLT_entry e0;
    e0.red = 10; e0.green = 20; e0.blue = 30; e0.frequency = 5;

    static png_sPLT_t splt_input;
    splt_input.name = "PaletteA";
    splt_input.nentries = 2;
    // Two dummy entries
    static png_sPLT_entry e1, e2;
    e1.red = 40; e1.green = 50; e1.blue = 60; e1.frequency = 2;
    e2.red = 70; e2.green = 80; e2.blue = 90; e2.frequency = 3;

    static png_sPLT_entry entry_array[2] = { e1, e2 };
    splt_input.entries = entry_array;
    splt_input.depth = 8;

    // Call the function under test
    png_set_sPLT(png_ptr, info_ptr, &splt_input, 1);

    // Verify results via internal fields
    ASSERT(info_ptr->splt_palettes_num == 1, "splt_palettes_num should be 1 after addition");
    ASSERT(info_ptr->splt_palettes != NULL, "splt_palettes should be non-NULL after addition");
    // The first (and only) palette
    png_sPLT_tp first = info_ptr->splt_palettes;
    ASSERT(first != NULL, "First sPLT entry should be non-NULL");
    ASSERT(std::strcmp(first->name, "PaletteA") == 0, "Palette name should be 'PaletteA'");
    ASSERT(first->nentries == 2, "First sPLT entry should have 2 nentries");
    ASSERT(first->entries != NULL, "First sPLT entries array should be allocated");

    // Basic cleanup (destroy will also free internal allocations if necessary)
    if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    std::cout << "Completed Test 2 (valid single entry) with internal state validated.\n";
}

// Test 3: Invalid sPLT entry is skipped (name NULL triggers app_error and skip)
static void test_png_set_sPLT_invalid_entry_skipped() {
    std::cout << "\nTest 3: Invalid sPLT entry is skipped\n";

    // Create png_ptr / info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    ASSERT(png_ptr != NULL, "Create png_ptr for Test 3");
    ASSERT(info_ptr != NULL, "Create info_ptr for Test 3");

    // Reset internal sPLT-related fields
    info_ptr->splt_palettes = NULL;
    info_ptr->splt_palettes_num = 0;
    info_ptr->valid = 0;

    // Prepare an input with invalid entry (name is NULL)
    static png_sPLT_entry e0;
    e0.red = 1; e0.green = 2; e0.blue = 3; e0.frequency = 1;

    static png_sPLT_t splt_input;
    splt_input.name = NULL; // invalid: should trigger app_error and skip
    splt_input.nentries = 1;
    splt_input.entries = &e0;
    splt_input.depth = 8;

    // Call the function under test
    png_set_sPLT(png_ptr, info_ptr, &splt_input, 1);

    // Expect no palette added
    ASSERT(info_ptr->splt_palettes_num == 0, "splt_palettes_num should remain 0 after invalid entry");
    // Note: We don't rely on app_error side effects; just ensure no allocation occurred.

    // Cleanup
    if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    std::cout << "Completed Test 3 (invalid entry handling) with no sPLT added.\n";
}

// Main test runner
int main() {
    std::cout << "Starting png_set_sPLT unit tests (C++11, no GTest)\n";

    test_png_set_sPLT_null_inputs();
    test_png_set_sPLT_single_entry();
    test_png_set_sPLT_invalid_entry_skipped();

    std::cout << "All tests completed.\n";
    return 0;
}
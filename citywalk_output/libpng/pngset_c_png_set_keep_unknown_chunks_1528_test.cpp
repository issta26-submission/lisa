// Unit test suite for the focal method: png_set_keep_unknown_chunks
// This test suite is crafted for a C++11 environment without Google Test.
// It relies on libpng public API to construct a png_ptr and exercise the function.
// The tests focus on verifying key branches of the function:
//  - NULL pointer handling
//  - Handling of num_chunks_in <= 0 (unknown_default path)
//  - Handling of missing chunk_list when num_chunks_in > 0
//  - Adding a single unknown chunk and updating internal list
//  - Invalid keep values gracefully produce no changes
//
// The tests use non-terminating checks; they report failures but continue execution
// to maximize code coverage (as per domain guidelines).
//
// {Candidate Keywords} used for test design: png_ptr, unknown_default, num_chunk_list, chunk_list,
// add_one_chunk, chunks_to_ignore, num_chunks_in, keep, NULL-pointer, memory-allocation, error-path.

#include <iostream>
#include <string>
#include <png.h>
#include <pngpriv.h>


// Simple test harness state
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASS(msg) do { ++g_tests_run; std::cout << "[PASS] " << msg << "\n"; } while(0)
#define TEST_FAIL(msg) do { ++g_tests_run; ++g_tests_failed; std::cerr << "[FAIL] " << msg << "\n"; } while(0)

// Test 1: Null pointer should cause no crash and return gracefully
bool test_null_pointer_handling() {
    // Call with NULL png_ptr; function should return without performing any action
    png_set_keep_unknown_chunks(nullptr, 1, nullptr, 1);
    // If we reach here, the function handled NULL gracefully
    TEST_PASS("png_set_keep_unknown_chunks handles NULL png_ptr without crashing");
    return true;
}

// Test 2: num_chunks_in <= 0 path should set unknown_default to 'keep' and return (no chunk-list changes)
bool test_unknown_default_set_on_zero_len() {
    // Create a valid png_ptr using libpng public API
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        TEST_FAIL("Failed to create png_struct for test_unknown_default_set_on_zero_len");
        return false;
    }

    // Drive the branch: num_chunks_in <= 0; ensure that unknown_default is set to 'keep'
    // Pre-condition: set unknown_default to a sentinel value to verify change
    png_ptr->unknown_default = 999;

    // Call with num_chunks_in = 0 and some 'keep' value (e.g., 5)
    png_set_keep_unknown_chunks(png_ptr, 5, nullptr, 0);

    // Post-condition: png_ptr->unknown_default should now be 5
    bool ok = (png_ptr->unknown_default == 5);

    png_destroy_write_struct(&png_ptr, nullptr);
    if (!ok) {
        TEST_FAIL("test_unknown_default_set_on_zero_len: unknown_default not set to keep when num_chunks_in == 0");
        return false;
    }

    TEST_PASS("test_unknown_default_set_on_zero_len: updated unknown_default when num_chunks_in == 0");
    return true;
}

// Test 3: num_chunks_in > 0 with NULL chunk_list should trigger error path and leave state unchanged
bool test_error_path_missing_chunk_list() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        TEST_FAIL("Failed to create png_struct for test_error_path_missing_chunk_list");
        return false;
    }

    // Ensure initial state
    png_ptr->chunk_list = NULL;
    png_ptr->num_chunk_list = 0;

    // Call with a non-zero num_chunks_in but NULL chunk_list
    png_set_keep_unknown_chunks(png_ptr, 1, nullptr, 1);

    // Post-condition: no changes to internal state
    bool ok = (png_ptr->chunk_list == NULL) && (png_ptr->num_chunk_list == 0);

    png_destroy_write_struct(&png_ptr, nullptr);
    if (!ok) {
        TEST_FAIL("test_error_path_missing_chunk_list: function modified state despite NULL chunk_list");
        return false;
    }

    TEST_PASS("test_error_path_missing_chunk_list: correctly handled missing chunk_list when num_chunks_in > 0");
    return true;
}

// Test 4: Adding a single unknown chunk updates the internal list and count
bool test_add_single_unknown_chunk() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        TEST_FAIL("Failed to create png_struct for test_add_single_unknown_chunk");
        return false;
    }

    // Start with empty list
    png_ptr->chunk_list = NULL;
    png_ptr->num_chunk_list = 0;

    // Prepare a single 5-byte entry: 4-char chunk name + 1-byte flag (non-zero)
    // Use chunk name "tEXt" as a representative unknown chunk
    unsigned char entry[5] = { 't', 'E', 'X', 't', 1 }; // last byte indicates 'keep'
    png_const_bytep inlist = entry;

    // Call to keep unknown chunks: one chunk, keep flag = 1
    png_set_keep_unknown_chunks(png_ptr, 1, inlist, 1);

    // Post-condition: there should be at least one chunk in the list
    bool ok = (png_ptr->chunk_list != NULL) && (png_ptr->num_chunk_list > 0);

    if (ok) {
        png_bytep list = png_ptr->chunk_list;
        // Validate the first 4 bytes of the stored chunk name
        ok = (list[0] == 't' && list[1] == 'E' && list[2] == 'X' && list[3] == 't');
        // Ensure the 5th byte (flag) is non-zero (indicating enabled/kept)
        ok = ok && (list[4] != 0);
    }

    png_destroy_write_struct(&png_ptr, nullptr);
    if (!ok) {
        TEST_FAIL("test_add_single_unknown_chunk: failed to add single unknown chunk or verify stored entry");
        return false;
    }

    TEST_PASS("test_add_single_unknown_chunk: added one unknown chunk and validated stored entry");
    return true;
}

// Test 5: Invalid keep value should not modify internal state
bool test_invalid_keep_value_no_change() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        TEST_FAIL("Failed to create png_struct for test_invalid_keep_value_no_change");
        return false;
    }

    // Prepare a valid chunk_list entry
    unsigned char entry[5] = { 't', 'E', 'X', 't', 1 };

    // Ensure initial state
    png_ptr->chunk_list = NULL;
    png_ptr->num_chunk_list = 0;

    // Use an invalid keep value (negative)
    const int invalid_keep = -1;

    png_set_keep_unknown_chunks(png_ptr, invalid_keep, entry, 1);

    // Post-condition: no changes to internal state
    bool ok = (png_ptr->chunk_list == NULL) && (png_ptr->num_chunk_list == 0);

    png_destroy_write_struct(&png_ptr, nullptr);
    if (!ok) {
        TEST_FAIL("test_invalid_keep_value_no_change: internal state was modified for invalid keep value");
        return false;
    }

    TEST_PASS("test_invalid_keep_value_no_change: invalid 'keep' value did not modify internal state");
    return true;
}

// Main runner
int main() {
    std::cout << "Starting tests for: png_set_keep_unknown_chunks\n";

    bool r1 = test_null_pointer_handling();
    bool r2 = test_unknown_default_set_on_zero_len();
    bool r3 = test_error_path_missing_chunk_list();
    bool r4 = test_add_single_unknown_chunk();
    bool r5 = test_invalid_keep_value_no_change();

    // Aggregate results
    int total = g_tests_run;
    int failed = g_tests_failed;
    int passed = total - failed;

    std::cout << "\nTest Summary: "
              << passed << " passed, "
              << failed << " failed, "
              << total << " total\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}
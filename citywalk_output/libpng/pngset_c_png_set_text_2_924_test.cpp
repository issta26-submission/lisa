// Minimal C++11 unit test suite for png_set_text_2 (pngset.c) using libpng
// This test harness uses a lightweight EXPECT-style approach (non-terminating asserts)
// and does not rely on GoogleTest or GMock as requested.

#include <iostream>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// libpng headers (C API)
extern "C" {
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAILED] " << msg << " (Line " << __LINE__ << ")" << std::endl; \
    } \
} while (0)

#define TEST_CASE(name) void name()

// Helper to safely free memory allocated by libpng if non-null
static void safe_free(png_structp png_ptr, void* ptr) {
    if (ptr != NULL) {
        // Use the library's allocator if available; otherwise standard free.
        // PNG provides png_free which handles NULL checks gracefully.
        if (png_ptr != NULL) {
            png_free(png_ptr, ptr);
        } else {
            free(ptr);
        }
    }
}

// Test 1: png_set_text_2 should return 0 if png_ptr is NULL
TEST_CASE(test_png_set_text_2_null_png_ptr) {
    // Prepare a dummy info_ptr (will be ignored due to early return)
    png_structp dummy_png = NULL;
    png_inforp info_ptr = NULL;

    // Create a minimal text entry
    png_text text_entry;
    text_entry.key = (char*)"Comment";
    text_entry.text = (char*)"Hello";
    text_entry.compression = PNG_TEXT_COMPRESSION_NONE;
    text_entry.lang = NULL;
    text_entry.lang_key = NULL;

    // Call with NULL png_ptr
    int result = png_set_text_2(dummy_png, info_ptr, &text_entry, 1);
    EXPECT_TRUE(result == 0, "png_set_text_2 should return 0 when png_ptr is NULL");
}

// Test 2: png_set_text_2 should return 0 if info_ptr is NULL
TEST_CASE(test_png_set_text_2_null_info_ptr) {
    // Create a real png_ptr and a dummy info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = NULL;
    if (png_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create png_ptr for test_png_set_text_2_null_info_ptr");
        return;
    }

    // Prepare input text
    png_text text_entry;
    text_entry.key = (char*)"Comment";
    text_entry.text = (char*)"Hello";
    text_entry.compression = PNG_TEXT_COMPRESSION_NONE;
    text_entry.lang = NULL;
    text_entry.lang_key = NULL;

    int result = png_set_text_2(png_ptr, info_ptr, &text_entry, 1);
    EXPECT_TRUE(result == 0, "png_set_text_2 should return 0 when info_ptr is NULL");

    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

// Test 3: png_set_text_2 should return 0 when num_text <= 0
TEST_CASE(test_png_set_text_2_zero_num_text) {
    // Create png_ptr and info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = NULL;
    if (png_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create png_ptr for test_png_set_text_2_zero_num_text");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create info_ptr for test_png_set_text_2_zero_num_text");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    // Prepare a minimal text array (not used since num_text = 0)
    png_text text_entry;
    text_entry.key = (char*)"Comment";
    text_entry.text = (char*)"Hello";
    text_entry.compression = PNG_TEXT_COMPRESSION_NONE;
    text_entry.lang = NULL;
    text_entry.lang_key = NULL;

    int result = png_set_text_2(png_ptr, info_ptr, &text_entry, 0);
    EXPECT_TRUE(result == 0, "png_set_text_2 should return 0 when num_text <= 0");

    png_destroy_write_struct(&png_ptr, &info_ptr);
}

// Test 4: Basic path: store a single text chunk without iTXt
TEST_CASE(test_png_set_text_2_basic_store) {
    // Create png_ptr and info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = NULL;
    if (png_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create png_ptr for test_png_set_text_2_basic_store");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create info_ptr for test_png_set_text_2_basic_store");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    // Allocate an initial text array capacity for 1 element
    info_ptr->text = (png_textp)malloc(sizeof(png_text) * 1);
    if (info_ptr->text == NULL) {
        EXPECT_TRUE(false, "Memory allocation for info_ptr->text failed");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    info_ptr->max_text = 1;
    info_ptr->num_text = 0;

    // Prepare input text
    png_text input_text;
    input_text.key = (char*)"Comment";
    input_text.text = (char*)"Hello world";
    input_text.compression = PNG_TEXT_COMPRESSION_NONE;
    input_text.lang = NULL;
    input_text.lang_key = NULL;

    int result = png_set_text_2(png_ptr, info_ptr, &input_text, 1);
    EXPECT_TRUE(result == 0, "png_set_text_2_basic_store should succeed");

    // Validate storage
    EXPECT_TRUE(info_ptr->num_text == 1, "info_ptr->num_text should be 1 after storage");
    if (info_ptr->num_text == 1) {
        png_textp stored = &(info_ptr->text[0]);
        EXPECT_TRUE(stored->key != NULL && strcmp(stored->key, "Comment") == 0,
                    "Stored key should be 'Comment'");
        EXPECT_TRUE(stored->text != NULL && strcmp(stored->text, "Hello world") == 0,
                    "Stored text should be 'Hello world'");
        // Text length fields for non-iTXt path
        // Depending on libpng version, text_length may reflect the string length
        // Here we simply ensure the string content is preserved.
    }

    // Cleanup
    if (info_ptr->text) {
        // Free the allocated array (inner strings are allocated within this buffer)
        png_free(png_ptr, info_ptr->text);
        info_ptr->text = NULL;
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

// Test 5: Reallocation path: trigger growth of text array when more entries are provided
TEST_CASE(test_png_set_text_2_reallocation) {
    // Create png_ptr and info_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_inforp info_ptr = NULL;
    if (png_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create png_ptr for test_png_set_text_2_reallocation");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        EXPECT_TRUE(false, "Failed to create info_ptr for test_png_set_text_2_reallocation");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    // Initial small capacity to force reallocation
    info_ptr->text = (png_textp)malloc(sizeof(png_text) * 1);
    if (info_ptr->text == NULL) {
        EXPECT_TRUE(false, "Memory allocation for info_ptr->text failed");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    info_ptr->max_text = 1;
    info_ptr->num_text = 0;

    // Prepare two input text entries to trigger reallocation
    png_text input_text[2];
    input_text[0].key = (char*)"K0";
    input_text[0].text = (char*)"T0";
    input_text[0].compression = PNG_TEXT_COMPRESSION_NONE;
    input_text[0].lang = NULL;
    input_text[0].lang_key = NULL;

    input_text[1].key = (char*)"K1";
    input_text[1].text = (char*)"T1";
    input_text[1].compression = PNG_TEXT_COMPRESSION_NONE;
    input_text[1].lang = NULL;
    input_text[1].lang_key = NULL;

    int result = png_set_text_2(png_ptr, info_ptr, input_text, 2);
    EXPECT_TRUE(result == 0, "png_set_text_2_reallocation should succeed");

    // Validate growth
    EXPECT_TRUE(info_ptr->num_text == 2, "info_ptr->num_text should be 2 after reallocation");
    if (info_ptr->num_text == 2) {
        EXPECT_TRUE(info_ptr->text[0].key != NULL &&
                    strcmp(info_ptr->text[0].key, "K0") == 0,
                    "First stored key should be 'K0'");
        EXPECT_TRUE(info_ptr->text[1].key != NULL &&
                    strcmp(info_ptr->text[1].key, "K1") == 0,
                    "Second stored key should be 'K1'");
    }

    // Cleanup
    if (info_ptr->text) {
        png_free(png_ptr, info_ptr->text);
        info_ptr->text = NULL;
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

// Runner
int main() {
    std::cout << "Running tests for png_set_text_2 (pngset.c) with libpng integration" << std::endl;

    // Execute test cases
    test_png_set_text_2_null_png_ptr();
    test_png_set_text_2_null_info_ptr();
    test_png_set_text_2_zero_num_text();
    test_png_set_text_2_basic_store();
    test_png_set_text_2_reallocation();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
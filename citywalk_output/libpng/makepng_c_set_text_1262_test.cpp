// Unit test suite for the focal method: set_text
// This test harness provides a self-contained C++11 test environment
// that replicates the essential behavior of the original C code path
// (set_text) and its dependencies (load_file, load_fake, png_set_text).
// We do not rely on GTest; instead, a small in-file test runner is used.

#include <cassert>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Domain-specific note:
// We implement minimal stubs and the function under test to exercise
// the control flow of set_text with true/false branches.

// ---------------------------------------------------------------------------
// Type definitions to mimic the libpng types used by set_text

typedef struct png_struct_tag* png_structp;
typedef struct png_info_tag* png_infop;
typedef char* png_charp;
typedef unsigned char* png_bytep;

typedef struct png_text {
    png_charp text;
    size_t text_length;
    // In real libpng there are more fields, but we only need these for tests.
} png_text;
typedef png_text* png_textp;

// ---------------------------------------------------------------------------
// Global state used by the test harness to verify behavior

static png_text g_test_text;               // Text object passed to set_text
static int      g_png_set_text_called = 0; // Tracks number of times png_set_text is invoked

// Captured copy of text data as written by the mock png_set_text
typedef struct CapturedText {
    unsigned char* data; // dynamically allocated copy for content verification
    size_t        length;
} CapturedText;
static CapturedText g_captured_text = { nullptr, 0 };

// Pointers returned by the loaders for verification
static unsigned char* g_last_file_ptr = nullptr; // memory allocated by load_file
static unsigned char* g_last_fake_ptr = nullptr; // memory allocated by load_fake

// ---------------------------------------------------------------------------
// Forward declarations of the functions under test and helpers

extern "C" void set_text(png_structp png_ptr, png_infop info_ptr, png_textp text, png_charp param);

// Mocked dependencies (filesystem/param-based injections)

static size_t load_file(const char* name, png_bytep* result);
static size_t load_fake(png_charp param, png_bytep* result);

// Mocked libpng function to capture the set_text call results
static void png_set_text(png_structp png_ptr, png_infop info_ptr, png_textp text, int num)
{
    (void)png_ptr; (void)info_ptr; (void)num;
    g_png_set_text_called++;

    // Copy the text payload to a separate buffer so tests can inspect the content
    if (text && text->text_length > 0 && text->text != nullptr) {
        g_captured_text.length = text->text_length;
        g_captured_text.data = (unsigned char*)malloc(text->text_length);
        if (g_captured_text.data) {
            memcpy(g_captured_text.data, text->text, text->text_length);
        }
    } else {
        g_captured_text.length = 0;
        g_captured_text.data = nullptr;
    }
}

// The focal method under test, replicated here to enable isolated unit testing.
// The implementation mirrors the provided source exactly as far as needed for tests.
extern "C" void set_text(png_structp png_ptr, png_infop info_ptr, png_textp text, png_charp param)
{
{
   switch (param[0])
   {
      case '<':
         {
            png_bytep file = NULL;
            text->text_length = load_file(param+1, &file);
            text->text = (png_charp)file;
         }
         break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
         {
            png_bytep data = NULL;
            size_t fake_len = load_fake(param, &data);
            if (fake_len > 0) /* else a simple parameter */
            {
               text->text_length = fake_len;
               text->text = (png_charp)data;
               break;
            }
         }
      default:
         text->text = param;
         break;
   }
   // In real code, this would register the text entry into the PNG info
   // struct via libpng. Here we simulate by invoking the mock function.
   png_set_text(png_ptr, info_ptr, text, 1);
   if (text->text != param)
      free(text->text);
}
}

// ---------------------------------------------------------------------------
// Helpers to manage and verify test results

static void reset_state_for_test(png_textp t)
{
    t->text = nullptr;
    t->text_length = 0;
    if (g_captured_text.data) {
        free(g_captured_text.data);
        g_captured_text.data = nullptr;
    }
    g_captured_text.length = 0;
    g_png_set_text_called = 0;

    // reset last pointers (if needed)
    g_last_file_ptr = nullptr;
    g_last_fake_ptr = nullptr;
}

// Helper to compare captured text content with expected string
static bool captured_text_equals(const char* expected, size_t expected_len)
{
    if (g_captured_text.length != expected_len) return false;
    if (expected_len == 0) return true;
    if (g_captured_text.data == nullptr) return false;
    return (memcmp(g_captured_text.data, expected, expected_len) == 0);
}

// ---------------------------------------------------------------------------
// Implementations of the mocked dependencies

static size_t load_file(const char* name, png_bytep* result)
{
    // For the purpose of tests, we only support a single controlled path
    // when name == "path" to verify "<" handler.
    if (name && strcmp(name, "path") == 0) {
        const char* data = "FILEDATA"; // 8 bytes
        size_t len = strlen(data);
        unsigned char* p = (unsigned char*)malloc(len);
        if (p) memcpy(p, data, len);
        *result = p;
        g_last_file_ptr = p;
        return len;
    }
    *result = nullptr;
    return 0;
}

static size_t load_fake(png_charp param, png_bytep* result)
{
    // Provide a fake payload only for a single-digit single-parameter
    // (e.g., "0") to exercise the fake-path in the switch-case.
    if (param && param[0] >= '0' && param[0] <= '9' && param[1] == '\0') {
        const char* data = "FAKE";
        size_t len = strlen(data);
        unsigned char* p = (unsigned char*)malloc(len);
        if (p) memcpy(p, data, len);
        *result = p;
        g_last_fake_ptr = p;
        return len;
    }
    *result = nullptr;
    return 0;
}

// ---------------------------------------------------------------------------
// Simple test harness (no external frameworks)

static bool test_A_param_angle_brace_path() {
    // Test the '<' path: param = "<path"
    reset_state_for_test(&g_test_text);

    const char* param = "<path";
    // Ensure structure is clean
    g_test_text.text = nullptr;
    g_test_text.text_length = 0;

    // Call the focal method
    set_text(nullptr, nullptr, &g_test_text, (png_charp)param);

    // Expectations:
    // 1) text points to allocated file data (not to the parameter)
    // 2) text_length equals the length returned by load_file (8)
    // 3) png_set_text was invoked exactly once
    bool ok = true;
    ok &= (g_test_text.text == g_last_file_ptr);
    ok &= (g_test_text.text_length == 8);
    ok &= (g_png_set_text_called == 1);

    // 4) Captured text content matches "FILEDATA"
    ok &= captured_text_equals("FILEDATA", 8);

    // Cleanup allocated file data (should already be freed by set_text)
    // But the captured copy is separately allocated; free it here.
    if (g_captured_text.data) { free(g_captured_text.data); g_captured_text.data = nullptr; }

    // Report
    if (!ok) {
        fprintf(stderr, "Test A (param '<path') FAILED\n");
    } else {
        printf("Test A (param '<path') PASSED\n");
    }
    return ok;
}

static bool test_B_param_single_digit_fake() {
    // Test the single-digit "0" path: param = "0"
    reset_state_for_test(&g_test_text);

    const char* param = "0";
    g_test_text.text = nullptr;
    g_test_text.text_length = 0;

    set_text(nullptr, nullptr, &g_test_text, (png_charp)param);

    bool ok = true;
    ok &= (g_test_text.text == g_last_fake_ptr);
    ok &= (g_test_text.text_length == 4);
    ok &= (g_png_set_text_called == 1);
    ok &= captured_text_equals("FAKE", 4);

    if (!ok) {
        fprintf(stderr, "Test B (param '0') FAILED\n");
    } else {
        printf("Test B (param '0') PASSED\n");
    }

    // Cleanup
    if (g_captured_text.data) { free(g_captured_text.data); g_captured_text.data = nullptr; }

    return ok;
}

static bool test_C_param_single_digit_escaping_default() {
    // Test the default path: param = "9" (single digit but not handled as fake)
    reset_state_for_test(&g_test_text);

    // Use a mutable buffer so the code can compare pointers reliably
    char param[] = "9";
    g_test_text.text = nullptr;
    g_test_text.text_length = 0;

    set_text(nullptr, nullptr, &g_test_text, param);

    bool ok = true;
    // Should be the same pointer as param (no allocation/freedom)
    ok &= (g_test_text.text == param);
    ok &= (g_png_set_text_called == 1);

    // The captured copy should be of length 0 since text_length was not set
    // in default path; ensure function was invoked
    if (!ok) {
        fprintf(stderr, "Test C (param '9') FAILED\n");
    } else {
        printf("Test C (param '9') PASSED\n");
    }

    // Cleanup
    if (g_captured_text.data) { free(g_captured_text.data); g_captured_text.data = nullptr; }

    return ok;
}

// ---------------------------------------------------------------------------
// Main: run tests

int main() {
    printf("Starting unit tests for set_text focal method...\n");

    bool all_ok = true;
    all_ok &= test_A_param_angle_brace_path();
    all_ok &= test_B_param_single_digit_fake();
    all_ok &= test_C_param_single_digit_escaping_default();

    if (all_ok) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Some tests FAILED.\n");
        return 1;
    }
}
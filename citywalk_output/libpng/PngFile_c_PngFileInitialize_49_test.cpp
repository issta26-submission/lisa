/*
   Unit Test Suite for PngFileInitialize in PngFile.c
   - Framework: Lightweight in-house test harness (no GTest)
   - Platform: Windows (requires Windows headers)
   - Language: C++11 compatible test runner invoking C functions from PngFile.c
   - Purpose: Validate the behavior and state of the OPENFILENAME object (ofn)
     after calling PngFileInitialize(HWND hwnd)

   Notes:
   - We access the global ofn and verify key fields are initialized as expected.
   - We avoid private methods/fields; we only rely on public/global state visible to the function.
   - We perform non-terminating assertions to maximize code execution and coverage.
   - All tests are self-contained in this single file.
*/

#include <commdlg.h>
#include <windows.h>
#include <cstring>
#include <zlib.h>
#include <tchar.h>
#include <stdlib.h>
#include <cstdio>
#include <pngfile.h>
#include <stdio.h>
#include <cexcept.h>
#include <png.h>


extern "C" void PngFileInitialize (HWND hwnd);
extern OPENFILENAME ofn;

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

static inline void expect_true(bool cond, const char* msg) {
    g_tests_run++;
    if (!cond) {
        g_tests_failed++;
        fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg);
    }
}

static inline void expect_eq_int(int a, int b, const char* msg) {
    g_tests_run++;
    if (a != b) {
        g_tests_failed++;
        fprintf(stderr, "EXPECT_EQ_INT FAILED: %s. Got %d, expected %d\n", msg, a, b);
    }
}

static inline void expect_null_ptr(const void* p, const char* msg) {
    g_tests_run++;
    if (p != NULL) {
        g_tests_failed++;
        fprintf(stderr, "EXPECT_NULL_PTR FAILED: %s. Pointer is not NULL (0x%p)\n", msg, p);
    }
}

static inline void expect_non_null_ptr(const void* p, const char* msg) {
    g_tests_run++;
    if (p == NULL) {
        g_tests_failed++;
        fprintf(stderr, "EXPECT_NON_NULL_PTR FAILED: %s. Pointer is NULL\n", msg);
    }
}

// Test 1: Initialize with NULL hwnd and verify core OPENFILENAME fields
// - ofn.lStructSize should equal sizeof(OPENFILENAME)
// - ofn.hwndOwner should be NULL
// - ofn.lpstrFilter should be set (non-NULL) and its first string should be "PNG Files (*.PNG)"
// - ofn.nMaxFile should be MAX_PATH
// - ofn.lpstrDefExt should be "png"
// - Optional: a few related fields should be NULL as set in the function
static void test_PngFileInitialize_null_hwnd() {
    // Arrange/Act
    PngFileInitialize(NULL);

    // Assert
    expect_true(ofn.lStructSize == sizeof(OPENFILENAME), "lStructSize must be sizeof(OPENFILENAME)");
    expect_true(ofn.hwndOwner == NULL, "hwndOwner must be NULL when initialized with NULL");

    expect_true(ofn.lpstrFilter != NULL, "lpstrFilter must be non-NULL after initialization");

    // Validate the first string of the multi-string filter
    // The buffer format is: "PNG Files (*.PNG)\0*.png\0All Files (*.*)\0*.*\0\0"
    // Reading as TCHAR* will stop at the first '\0', so we compare the first substring.
    const TCHAR* firstSubstring = ofn.lpstrFilter;
    const TCHAR* expectedFirst = _T("PNG Files (*.PNG)");
    if (_tcsncmp(firstSubstring, expectedFirst, _tcslen(expectedFirst)) != 0) {
        fprintf(stderr, "FILTER FIRST SUBSTRING MISMATCH: got \"%s\", expected \"%s\"\n",
                firstSubstring, expectedFirst);
        g_tests_failed++;
        g_tests_run++; // accounting for this assertion as well
    }

    expect_eq_int(ofn.nMaxFile, MAX_PATH, "nMaxFile must be MAX_PATH");
    if (_tcsncmp(ofn.lpstrDefExt, _T("png"), 3) != 0 || _tcslen(ofn.lpstrDefExt) != 3) {
        fprintf(stderr, "DEFEXT MISMATCH: got \"%s\", expected \"png\"\n", ofn.lpstrDefExt);
        g_tests_failed++;
        g_tests_run++;
    }

    expect_null_ptr(ofn.lpstrFile, "lpstrFile should be NULL");
    expect_null_ptr(ofn.lpstrFileTitle, "lpstrFileTitle should be NULL");
    expect_null_ptr(ofn.lpstrCustomFilter, "lpstrCustomFilter should be NULL");
    expect_eq_int(ofn.nFileOffset, 0, "nFileOffset should be 0");
    expect_eq_int(ofn.nFileExtension, 0, "nFileExtension should be 0");
    expect_true(ofn.lpfnHook == NULL, "lpfnHook should be NULL");
    expect_true(ofn.lpTemplateName == NULL, "lpTemplateName should be NULL");
    expect_true(ofn.hInstance == NULL, "hInstance should be NULL");
}

// Test 2: Initialize with a non-NULL hwnd and verify that hwndOwner is updated
// - The rest of the fields should remain consistent with Test 1 behavior
static void test_PngFileInitialize_non_null_hwnd() {
    HWND test_hwnd = (HWND)0x1234;

    PngFileInitialize(test_hwnd);

    // Verify that hwndOwner is updated
    expect_true(ofn.hwndOwner == test_hwnd, "hwndOwner must be updated to the provided hwnd");

    // Basic consistency checks identical to Test 1
    expect_true(ofn.lStructSize == sizeof(OPENFILENAME), "lStructSize must stay as sizeof(OPENFILENAME)");
    expect_true(ofn.lpstrFilter != NULL, "lpstrFilter must remain non-NULL after initialization");

    // Validate first substring again to ensure stability
    const TCHAR* firstSubstring = ofn.lpstrFilter;
    const TCHAR* expectedFirst = _T("PNG Files (*.PNG)");
    if (_tcsncmp(firstSubstring, expectedFirst, _tcslen(expectedFirst)) != 0) {
        fprintf(stderr, "FILTER FIRST SUBSTRING MISMATCH (NON-NULL HWND): got \"%s\", expected \"%s\"\n",
                firstSubstring, expectedFirst);
        g_tests_failed++;
        g_tests_run++;
    }

    expect_eq_int(ofn.nMaxFile, MAX_PATH, "nMaxFile must be MAX_PATH (non-null hwnd)");
    if (_tcsncmp(ofn.lpstrDefExt, _T("png"), 3) != 0 || _tcslen(ofn.lpstrDefExt) != 3) {
        fprintf(stderr, "DEFEXT MISMATCH (NON-NULL HWND): got \"%s\", expected \"png\"\n", ofn.lpstrDefExt);
        g_tests_failed++;
        g_tests_run++;
    }

    expect_null_ptr(ofn.lpstrFile, "lpstrFile should be NULL after initialization");
    expect_null_ptr(ofn.lpstrFileTitle, "lpstrFileTitle should be NULL after initialization");
    expect_null_ptr(ofn.lpstrCustomFilter, "lpstrCustomFilter should be NULL after initialization");
    expect_eq_int(ofn.nFileOffset, 0, "nFileOffset should be 0");
    expect_eq_int(ofn.nFileExtension, 0, "nFileExtension should be 0");
    expect_true(ofn.lpfnHook == NULL, "lpfnHook should be NULL");
    expect_true(ofn.lpTemplateName == NULL, "lpTemplateName should be NULL");
    expect_true(ofn.hInstance == NULL, "hInstance should be NULL");
}

static int run_all_tests() {
    fprintf(stderr, "Starting PngFileInitialize tests...\n");

    test_PngFileInitialize_null_hwnd();
    test_PngFileInitialize_non_null_hwnd();

    fprintf(stderr, "Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    return g_tests_failed;
}

int main() {
    // Initialize the test suite and run all tests
    int failures = run_all_tests();

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TESTS FAILED: %d failure(s)\n", failures);
    }

    return failures ? 1 : 0;
}
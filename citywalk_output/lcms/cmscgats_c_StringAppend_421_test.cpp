/*
  Lightweight C++ test suite for the focal C function:
  cmsBool StringAppend(string* s, char c)

  Notes:
  - No GoogleTest is used. A small in-file test harness is implemented to
    perform non-terminating assertions and report a summary at the end.
  - We re-declare the minimal surface of types used by the focal function so
    tests can create and manipulate a string instance suitable for testing.
  - We rely on the existing AllocChunk implemented in the project (as used by
    StringAppend) and link against it. We provide careful dummy values for it8
    where needed to avoid NULL-pointer related issues in realloc paths.
  - The tests target:
      1) No reallocation path
      2) Reallocation path where begin is non-NULL (ensures memcpy of old data)
      3) Reallocation path where begin is NULL (ensures memory allocation and write)
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Lightweight, in-file type declarations to mirror the real project surface.
// These declarations are sufficient for compiling and linking the focal function
// StringAppend and for constructing test scenarios.

extern "C" {

typedef unsigned int cmsUInt32Number;
typedef int cmsBool;

// Forward declare cmsIT8 type (as used by the real code)
typedef struct cmsIT8_tag cmsIT8;
struct cmsIT8_tag {
    int dummy; // minimal placeholder
};

// Forward declare and define the 'string' type layout expected by StringAppend.
// We re-create only the fields necessary for tests (layout mirrors real code).
typedef struct string string;
struct string {
    cmsUInt32Number len;
    cmsUInt32Number max;
    char* begin;
    cmsIT8* it8;
};

// Prototype of focal function under test
cmsBool StringAppend(string* s, char c);

// AllocChunk is used by StringAppend during reallocation.
// We declare it here to satisfy the call in the focal function during tests.
void* AllocChunk(cmsIT8* it8, cmsUInt32Number size);

} // extern "C"

// Simple in-file test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Convenience macro for non-terminating assertions
#define TEST_ASSERT(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        printf("TEST FAILED: %s\n", msg); \
    } \
} while(0)

// Test 1: No reallocation path
// Setup: len = 3, max = 10, begin holds "ABC". Append 'D'.
// Expect: len becomes 4, buffer contains "ABCD" and is null-terminated.
// This covers the "if (s->len + 1 >= s->max)" false branch.
void test_StringAppend_no_realloc()
{
    printf("Running test_StringAppend_no_realloc...\n");

    // Allocate a test string object matching the expected layout
    string* s = (string*)malloc(sizeof(string));
    s->len = 3;
    s->max = 10;
    s->begin = (char*)malloc(s->max);
    if (s->begin == NULL) {
        printf("Memory allocation failed for test 1 setup\n");
        g_failed_tests++;
        g_total_tests++;
        free(s);
        return;
    }
    // Initialize with "ABC"
    memcpy(s->begin, "ABC", 3);
    s->begin[3] = '\0';
    s->it8 = NULL;

    // Call the focal function
    cmsBool res = StringAppend(s, 'D');
    TEST_ASSERT(res != 0, "StringAppend should return TRUE when no realloc is performed");
    TEST_ASSERT(s->len == 4, "Length should be 4 after append without realloc");
    TEST_ASSERT(strncmp(s->begin, "ABCD", 4) == 0 && s->begin[4] == '\0',
                "Buffer should contain 'ABCD' after append");

    // Cleanup
    free(s->begin);
    free(s);
    printf("Completed test_StringAppend_no_realloc\n\n");
}

// Test 2: Reallocation path with begin non-NULL
// Setup: len = 9, max = 10, begin contains '1'..'9'.
// Append 'X'. Expect: realloc occurs, old data preserved, new char appended,
// resulting in "123456789X".
void test_StringAppend_with_realloc_begin_not_null()
{
    printf("Running test_StringAppend_with_realloc_begin_not_null...\n");

    string* s = (string*)malloc(sizeof(string));
    s->len = 9;
    s->max = 10;
    s->begin = (char*)malloc(10);
    if (s->begin == NULL) {
        printf("Memory allocation failed for test 2 setup\n");
        g_failed_tests++;
        g_total_tests++;
        free(s);
        return;
    }
    for (int i = 0; i < 9; ++i) {
        s->begin[i] = '1' + i; // '1','2',...,'9'
    }
    s->begin[9] = '\0';
    s->it8 = (cmsIT8*)(&((int){0})); // non-NULL dummy pointer to avoid NULL checks

    cmsBool dummy_alloc_ok; // not used, but ensures we keep style similar to real code
    (void)dummy_alloc_ok;

    cmsBool res = StringAppend(s, 'X');
    TEST_ASSERT(res != 0, "StringAppend should succeed when realloc is performed and begin is non-NULL");
    TEST_ASSERT(s->len == 10, "Length should be 10 after realloc append");
    // Build expected string: "123456789X"
    char expected[12];
    for (int i = 0; i < 9; ++i) expected[i] = '1' + i;
    expected[9] = 'X';
    expected[10] = '\0';
    TEST_ASSERT(strncmp(s->begin, expected, 10) == 0 && s->begin[10] == '\0',
                "Buffer should contain '123456789X' after realloc and append");

    // Cleanup
    free(s->begin);
    free(s);
    printf("Completed test_StringAppend_with_realloc_begin_not_null\n\n");
}

// Test 3: Reallocation path with begin NULL
// Setup: len = 0, max = 1, begin NULL.
// Append 'Z'. Expect: AllocChunk is called to allocate memory, then 'Z' is stored.
// Result: len == 1, begin != NULL, buffer contains 'Z'.
void test_StringAppend_with_realloc_begin_null()
{
    printf("Running test_StringAppend_with_realloc_begin_null...\n");

    string* s = (string*)malloc(sizeof(string));
    s->len = 0;
    s->max = 1;
    s->begin = NULL;
    s->it8 = NULL; // could be NULL for this test

    cmsBool res = StringAppend(s, 'Z');
    TEST_ASSERT(res != 0, "StringAppend should succeed when reallocating from NULL begin");
    TEST_ASSERT(s->len == 1, "Length should be 1 after append with NULL begin and realloc");
    TEST_ASSERT(s->begin != NULL, "Begin should be allocated after realloc");
    TEST_ASSERT(s->begin[0] == 'Z' && s->begin[1] == '\0',
                "Buffer should contain 'Z' after append");

    // Cleanup
    if (s->begin) free(s->begin);
    free(s);
    printf("Completed test_StringAppend_with_realloc_begin_null\n\n");
}

// Main entry: run all tests and print a concise summary.
int main(void)
{
    printf("Starting StringAppend test suite...\n\n");

    test_StringAppend_no_realloc();
    test_StringAppend_with_realloc_begin_not_null();
    test_StringAppend_with_realloc_begin_null();

    printf("Test suite finished. Total: %d, Failed: %d\n",
           g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
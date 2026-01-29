/*
  Candidate Keywords (derived from Step 1):
  - cJSON_SetValuestring, object, valuestring, cJSON_String, cJSON_IsReference
  - memory management: strcpy, cJSON_strdup, global_hooks, cJSON_free
  - edge predicates: NULL object, NULL valuestring, non-string type, is reference
  - memory overlap check: overlapping source and destination condition
  - in-place copy vs allocate-new-copy (v1_len <= v2_len vs v1_len > v2_len)
  - test coverage: true/false branches of each predicate
  - domain knowledge constraints: use C++11, no GTest, main() driven tests
*/

/* Unit tests for cJSON_SetValuestring in C using C++11 test harness.
   Note: We link against the C library (cJSON.c/.h). Tests are written in C++
   and invoke the C API directly. No GTest is used per requirements.
*/

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

/* Simple non-terminating test harness */
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void log_result(const char* test_name, bool passed) {
    ++g_tests_run;
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        ++g_tests_failed;
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

/*  Test 1: NULL object should return NULL */
static void test_SetValuestring_NULLObject_ReturnsNULL() {
    // Object is NULL; expect NULL return
    char* res = cJSON_SetValuestring(NULL, "hello");
    log_result("test_SetValuestring_NULLObject_ReturnsNULL", res == NULL);
}

/*  Test 2: Non-string object should return NULL */
static void test_SetValuestring_ObjectNotString_ReturnsNULL() {
    // Create a numeric cJSON object which is not a string
    cJSON* obj = cJSON_CreateNumber(42);
    char* res = cJSON_SetValuestring(obj, "newvalue");
    log_result("test_SetValuestring_ObjectNotString_ReturnsNULL", res == NULL);
    cJSON_Delete(obj);
}

/*  Test 3: Object marked as IsReference should return NULL */
static void test_SetValuestring_ObjectIsReference_ReturnsNULL() {
    cJSON* obj = cJSON_CreateString("orig");
    // Mark as reference (should bypass setting valuestring)
    obj->type |= cJSON_IsReference;
    char* res = cJSON_SetValuestring(obj, "newvalue");
    log_result("test_SetValuestring_ObjectIsReference_ReturnsNULL", res == NULL);
    cJSON_Delete(obj);
}

/*  Test 4: object->valuestring NULL should cause NULL return */
static void test_SetValuestring_ObjectValuestringNull_ReturnsNULL() {
    cJSON* obj = cJSON_CreateString("orig");
    // Force valuestring to NULL to simulate corrupted or cleared string
    obj->valuestring = NULL;
    char* res = cJSON_SetValuestring(obj, "newvalue");
    log_result("test_SetValuestring_ObjectValuestringNull_ReturnsNULL", res == NULL);
    cJSON_Delete(obj);
}

/*  Test 4.5: NULL valuestring parameter should return NULL */
static void test_SetValuestring_ValuestringNULL_ReturnsNULL() {
    cJSON* obj = cJSON_CreateString("orig");
    char* res = cJSON_SetValuestring(obj, NULL);
    log_result("test_SetValuestring_ValuestringNULL_ReturnsNULL", res == NULL);
    cJSON_Delete(obj);
}

/*  Test 5: In-place copy when v1_len <= v2_len and no overlap */
static void test_SetValuestring_InPlaceCopy_NoOverlap() {
    cJSON* obj = cJSON_CreateString("abcdef"); // v2_len = 6
    const char* newstr = "xyz";               // v1_len = 3
    char* res = cJSON_SetValuestring(obj, newstr);
    bool ok = (res == obj->valuestring) && (strcmp(obj->valuestring, newstr) == 0);
    log_result("test_SetValuestring_InPlaceCopy_NoOverlap", ok);
    cJSON_Delete(obj);
}

/*  Test 6: Allocate new copy when v1_len > v2_len */
static void test_SetValuestring_AllocatesNewCopy_WhenLonger() {
    cJSON* obj = cJSON_CreateString("ab"); // v2_len = 2
    char* old_ptr = obj->valuestring;
    const char* newstr = "abcdef";        // v1_len = 6
    char* res = cJSON_SetValuestring(obj, newstr);
    bool ok = (res == obj->valuestring) && (obj->valuestring != old_ptr) &&
              (strcmp(obj->valuestring, newstr) == 0);
    log_result("test_SetValuestring_AllocatesNewCopy_WhenLonger", ok);
    cJSON_Delete(obj);
}

/*  Test 7: Overlapping memory regions should return NULL (safety check) */
static void test_SetValuestring_Overlapping_ReturnsNULL() {
    // Allocate a buffer to simulate overlapping src/dst regions
    const size_t BUFSZ = 128;
    char* buffer = (char*)malloc(BUFSZ);
    if (!buffer) {
        log_result("test_SetValuestring_Overlapping_ReturnsNULL", false);
        return;
    }
    // Prepare memory: dest at buffer+20 with "OldStr" (length 6)
    memset(buffer, 0, BUFSZ);
    memcpy(buffer + 20, "OldStr", 6); // includes 'O','l','d','S','t','r'
    buffer[26] = '\0';
    // Source at buffer+22 with "AB" (length 2) -> overlaps with dest region [20,26)
    memcpy(buffer + 22, "AB", 3); // "AB\0" at 22..24
    // Create a minimal cJSON object pointing to dest
    cJSON* obj = (cJSON*)malloc(sizeof(cJSON));
    obj->type = cJSON_String;
    obj->valuestring = buffer + 20;

    char* res = cJSON_SetValuestring(obj, buffer + 22); // overlaps
    bool ok = (res == NULL);
    log_result("test_SetValuestring_Overlapping_ReturnsNULL", ok);

    // Cleanup
    free(buffer);
    free(obj);
}

/*  Test 8: Verify that true/false branches for basic cases are covered by previous tests.
   Additional quick sanity: setting equal length string should work and not crash.
*/
static void test_SetValuestring_EqualLength_Copy() {
    cJSON* obj = cJSON_CreateString("abc"); // length 3
    const char* newstr = "def";            // length 3
    char* res = cJSON_SetValuestring(obj, newstr);
    bool ok = (res == obj->valuestring) && (strcmp(obj->valuestring, newstr) == 0);
    log_result("test_SetValuestring_EqualLength_Copy", ok);
    cJSON_Delete(obj);
}

/* Entry point for tests */
int main() {
    // Run tests
    test_SetValuestring_NULLObject_ReturnsNULL();
    test_SetValuestring_ObjectNotString_ReturnsNULL();
    test_SetValuestring_ObjectIsReference_ReturnsNULL();
    test_SetValuestring_ObjectValuestringNull_ReturnsNULL();
    test_SetValuestring_ValuestringNULL_ReturnsNULL();
    test_SetValuestring_InPlaceCopy_NoOverlap();
    test_SetValuestring_AllocatesNewCopy_WhenLonger();
    test_SetValuestring_Overlapping_ReturnsNULL();
    test_SetValuestring_EqualLength_Copy();

    std::cout << "\nTest summary: " << g_tests_run << " run, "
              << g_tests_failed << " failed.\n";
    return (g_tests_failed == 0) ? 0 : 1;
}
/*
 * misc_utils_tests.c
 *
 * Purpose:
 *   Provide a focused Unity-based test suite for the focal main() under test
 *   (which registers and runs cjson_utils related tests). The suite focuses on
 *   ensuring that null pointer scenarios do not crash the cJSON utilities
 *   (as exercised by the existing cjson_utils_functions_shouldnt_crash_with_null_pointers
 *   path and related API surfaces if present).
 *
 * Notes:
 *   - Uses Unity framework (as indicated by the project dependencies in the focal class dep).
 *   - Guards against API presence using preprocessor checks (e.g., cJSON_Print, cJSON_PrintUnformatted,
 *     cJSON_GetObjectItem) so the tests remain safe across different builds where some APIs may be
 *     absent in the linked cJSON_utils variant.
 *   - The tests intentionally avoid private/internal methods; they exercise public API surfaces that
 *     are designed to be NULL-safe (e.g., cJSON_Delete(NULL)).
 *   - The tests are non-terminating in the sense of not causing a hard crash; they rely on Unity's
 *     assertion macros to validate expected behaviors without terminating the entire test suite prematurely.
 *
 * This file is intended to compile with C++11 compliant toolchains (as part of a C project compiled
 * under C++11) and does not rely on GTest/GMock.
 */

#include <unity/src/unity.h>
#include <../cJSON_Utils.h>
#include <unity/examples/unity_config.h>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*
 * Test: cjson_utils_functions_shouldnt_crash_with_null_pointers
 * Purpose:
 *   Validate that a set of commonly used cJSON safe operations do not crash when provided NULL pointers.
 *   This specifically exercises null-pointer interactions that are documented to be safe in cJSON
 *   (e.g., cJSON_Delete(NULL)).
 * Notes:
 *   - If optional APIs (like cJSON_Print) exist in the linked library, verify they return NULL for NULL input.
 *   - The test avoids directly dereferencing NULL pointers and uses Unity assertions to verify return values.
 */
static void cjson_utils_functions_shouldnt_crash_with_null_pointers(void)
{
    // 1) NULL input to cJSON_Delete should be a no-op and must not crash
    cJSON_Delete(NULL);

    // 2) If available, cJSON_Print(NULL) should return NULL (no crash)
#ifdef cJSON_Print
    char *printed = cJSON_Print(NULL);
    TEST_ASSERT_NULL((void*)printed);
    if (printed)
    {
        free(printed);
    }
#endif

    // 3) If available, cJSON_PrintUnformatted(NULL) should return NULL (no crash)
#ifdef cJSON_PrintUnformatted
    char *unprinted = cJSON_PrintUnformatted(NULL);
    TEST_ASSERT_NULL((void*)unprinted);
    if (unprinted)
    {
        free(unprinted);
    }
#endif
}

/*
 * Test: cjson_utils_print_null_object_returns_null
 * Purpose:
 *   Specifically re-assert that printing a NULL cJSON object is safe and returns NULL.
 *   This helps cover the true/false branches of the NULL-check predicate for Print APIs.
 */
static void cjson_utils_print_null_object_returns_null(void)
{
#ifdef cJSON_Print
    // Verify that printing a NULL object yields NULL (and does not crash)
    char *printed = cJSON_Print(NULL);
    TEST_ASSERT_NULL((void*)printed);
    if (printed)
    {
        free(printed);
    }
#endif
}

/*
 * Test: cjson_utils_get_object_item_with_null_object
 * Purpose:
 *   Ensure that requesting an object item from a NULL parent object returns NULL.
 *   This exercises the NULL-branch of the GetObjectItem API if it exists.
 */
static void cjson_utils_get_object_item_with_null_object(void)
{
#ifdef cJSON_GetObjectItem
    // Attempt to retrieve an item from a NULL object; expect NULL result
    cJSON *item = cJSON_GetObjectItem(NULL, "key");
    TEST_ASSERT_NULL((void*)item);
#endif
}

/* Main entry point for the Unity-based test runner. The suite includes multiple
 * test cases to improve coverage around null-pointer handling in the cJSON utilities. */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(cjson_utils_functions_shouldnt_crash_with_null_pointers);
    RUN_TEST(cjson_utils_print_null_object_returns_null);
    RUN_TEST(cjson_utils_get_object_item_with_null_object);
    return UNITY_END();
}
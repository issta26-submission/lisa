/*
Step 0: Context and Candidate Keywords
- Focal method: cJSON_AddBoolToObject
- Core dependent components: cJSON_CreateBool, add_item_to_object, global_hooks, cJSON_Delete, NULL, cJSON_bool
- Key interactions: object (cJSON*), name (const char*), boolean flag, return value (cJSON*)
- Expected behavior: create a bool item, attempt to insert into object, return the item on success, otherwise clean up and return NULL

Step 1: Candidate Keywords extracted from the focal class/file
- cJSON_AddBoolToObject
- cJSON_CreateBool
- add_item_to_object
- global_hooks
- cJSON_Delete
- NULL
- cJSON_bool
- object (cJSON*)
- name (const char*)
- boolean (cJSON_bool)

Step 2: Test goals based on the above
- Validate successful addition: when object and name are valid, the function returns a non-NULL item that is present in the object and has the expected boolean value.
- Validate that the returned item matches the object’s stored item (pointer identity).
- Validate failure path when object is NULL: function returns NULL and does not crash.
- Validate failure path when name is NULL: function returns NULL and does not crash.
- Cover true and false branches of the boolean value.
Notes:
- We will implement a lightweight C++11 test harness (no GTest) with non-terminating assertions to maximize coverage.
- We will use the public cJSON API only (no access to private static helpers).
- Tests will be executable by a standard C++11 compiler and link against the cJSON.c implementation.

Step 3: Domain knowledge guidance applied
- Use only standard library + cJSON public API
- Access static/private implementation details should be avoided
- Non-terminating assertions: log failures but continue
- Use main() to drive tests (no GTest/GMock)
- Distinguish pointer identity vs content by comparing pointers and values explicitly
*/

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Include C header for cJSON with C linkage
extern "C" {
}

// Lightweight, non-terminating test macros
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if(cond) { \
        std::cerr << "EXPECT_FALSE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_NULL(ptr) do { \
    if((ptr) != nullptr) { \
        std::cerr << "EXPECT_NULL failed: " #ptr " is non-null at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    if((ptr) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: " #ptr " is null at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " #a " != " #b " (" << (void*)(a) << " != " << (void*)(b) << ") at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_BOOL_EQ(a,b) do { \
    if(((a) ? true : false) != ((b) ? true : false)) { \
        std::cerr << "EXPECT_BOOL_EQ failed: " #a " != " #b " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Test 1: Successful addition with true value
// - Create an object
// - Add a bool with name "flag_true" and value true
// - Expect non-NULL result, and that the object contains the item with the correct value
void test_add_bool_to_object_true() {
    cJSON *obj = cJSON_CreateObject();
    EXPECT_NOT_NULL(obj);

    cJSON *ret = cJSON_AddBoolToObject(obj, "flag_true", (cJSON_bool)true);
    EXPECT_NOT_NULL(ret);

    // Verify pointer identity: returned item should be the same as the object item retrieved by name
    cJSON *from_obj = cJSON_GetObjectItemCaseSensitive(obj, "flag_true");
    EXPECT_NOT_NULL(from_obj);
    EXPECT_PTR_EQ(ret, from_obj);

    // Verify content: it is a boolean and true
    EXPECT_TRUE(cJSON_IsBool(ret));
    EXPECT_TRUE(cJSON_IsTrue(ret));

    // Cleanup
    cJSON_Delete(obj);
}

// Test 2: Successful addition with false value
// - Similar to Test 1 but with value false
void test_add_bool_to_object_false() {
    cJSON *obj = cJSON_CreateObject();
    EXPECT_NOT_NULL(obj);

    cJSON *ret = cJSON_AddBoolToObject(obj, "flag_false", (cJSON_bool)false);
    EXPECT_NOT_NULL(ret);

    cJSON *from_obj = cJSON_GetObjectItemCaseSensitive(obj, "flag_false");
    EXPECT_NOT_NULL(from_obj);
    EXPECT_PTR_EQ(ret, from_obj);

    // Verify content: boolean and false
    EXPECT_TRUE(cJSON_IsBool(ret));
    EXPECT_TRUE(cJSON_IsFalse(ret));

    cJSON_Delete(obj);
}

// Test 3: Failure path when object is NULL
// - The function should return NULL and not crash, regardless of boolean value
void test_add_bool_to_object_null_object() {
    cJSON *ret = cJSON_AddBoolToObject(nullptr, "flag_null_obj", (cJSON_bool)true);
    EXPECT_NULL(ret);
}

// Test 4: Failure path when name is NULL
// - The function should return NULL and not crash, and not modify the object
void test_add_bool_to_object_null_name() {
    cJSON *obj = cJSON_CreateObject();
    EXPECT_NOT_NULL(obj);

    cJSON *ret = cJSON_AddBoolToObject(obj, nullptr, (cJSON_bool)true);
    EXPECT_NULL(ret);

    // Ensure object remains unchanged: there should be no item with a NULL name.
    cJSON *check = cJSON_GetObjectItemCaseSensitive(obj, (const char*)nullptr);
    EXPECT_NULL(check);

    cJSON_Delete(obj);
}

// Helper to run all tests and report summary
int main() {
    // Step 2/3: Execute the test suite
    test_add_bool_to_object_true();
    test_add_bool_to_object_false();
    test_add_bool_to_object_null_object();
    test_add_bool_to_object_null_name();

    // Summary
    std::cout << "Test suite finished. Failures: " << g_test_failures << std::endl;

    // Return non-zero if any test failed
    return g_test_failures ? 1 : 0;
}
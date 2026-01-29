/*
Step 1 - Candidate Keywords (derived from the focal method and its file-level dependencies):
- cJSON_AddTrueToObject
- cJSON_CreateTrue
- add_item_to_object
- global_hooks
- cJSON_AddItemToObject
- cJSON_GetObjectItem
- cJSON_IsTrue
- cJSON_CreateObject
- cJSON_Delete
- cJSON_Public (as part of the API exposure)
Notes:
- The tests focus on the public API surface used by cJSON_AddTrueToObject and observable
  object state after the operation.
- We exercise both the success path and failure paths (object NULL or name NULL).
- We verify that the returned pointer is the same as the item stored in the object and that
  the item is truly a JSON true value.
*/

/* Domain Knowledge:
- Use only standard C++11 library and provided C API (cJSON) to build the test.
- Do not rely on private static helpers; use only public APIs.
- Non-terminating assertions are preferred to maximize code coverage across tests.
- We implement a small, self-contained test harness (no GTest) with a simple pass/fail report.
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
#include <cstdlib>
#include <limits.h>


extern "C" {
}

/* Lightweight test harness (non-terminating, continues after failures) */
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define T_EXPECT_TRUE(cond) do {                                  \
    ++g_tests_run;                                                  \
    if(!(cond)) {                                                   \
        ++g_tests_failed;                                           \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__       \
                  << " - Expected true: " << #cond << std::endl;   \
    }                                                               \
} while(0)

#define T_EXPECT_PTR_EQ(lhs, rhs) do {                             \
    ++g_tests_run;                                                  \
    if((lhs) != (rhs)) {                                           \
        ++g_tests_failed;                                           \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__       \
                  << " - Pointers differ: " << (void*)(lhs)           \
                  << " != " << (void*)(rhs) << std::endl;          \
    }                                                               \
} while(0)

#define T_EXPECT_NOT_NULL(ptr) do {                                \
    ++g_tests_run;                                                  \
    if((ptr) == nullptr) {                                        \
        ++g_tests_failed;                                           \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__       \
                  << " - Pointer is null: " << #ptr << std::endl;  \
    }                                                               \
} while(0)

static void test_AddTrueToObject_Normal()
{
    // Objective: Validate the successful path of cJSON_AddTrueToObject
    // 1) Creates a new object.
    // 2) Adds a true value with a given name.
    // 3) Ensures the returned item is non-null.
    // 4) Ensures the returned item is stored in the object under the same name.
    // 5) Ensures the stored item represents a true value.
    cJSON *object = cJSON_CreateObject();
    T_EXPECT_NOT_NULL(object);

    cJSON *ret = cJSON_AddTrueToObject(object, "active");
    T_EXPECT_TRUE(ret != nullptr);
    T_EXPECT_PTR_EQ(ret, cJSON_GetObjectItem(object, "active"));
    if (ret != nullptr) {
        T_EXPECT_TRUE(cJSON_IsTrue(ret));
        // Double-check the object also reflects the same item
        cJSON *item = cJSON_GetObjectItem(object, "active");
        T_EXPECT_NOT_NULL(item);
        T_EXPECT_TRUE(cJSON_IsTrue(item));
    }

    cJSON_Delete(object);
}

static void test_AddTrueToObject_ObjectNull()
{
    // Objective: Verify behavior when the target object is NULL.
    // Expected: NULL is returned and no crash occurs.
    cJSON *ret = cJSON_AddTrueToObject(nullptr, "active");
    T_EXPECT_TRUE(ret == nullptr);
}

static void test_AddTrueToObject_NameNull()
{
    // Objective: Verify behavior when the name parameter is NULL.
    // Expected: NULL is returned and no crash occurs.
    cJSON *object = cJSON_CreateObject();
    T_EXPECT_NOT_NULL(object);

    cJSON *ret = cJSON_AddTrueToObject(object, nullptr);
    T_EXPECT_TRUE(ret == nullptr);

    cJSON_Delete(object);
}

static void run_all_tests()
{
    test_AddTrueToObject_Normal();
    test_AddTrueToObject_ObjectNull();
    test_AddTrueToObject_NameNull();
}

int main()
{
    run_all_tests();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}
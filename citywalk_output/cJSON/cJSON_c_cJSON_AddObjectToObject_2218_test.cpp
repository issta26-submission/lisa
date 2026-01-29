// Unit test suite for cJSON_AddObjectToObject in cJSON.c
// This test is designed to be compiled as C++11 code, without GoogleTest.
// It uses a lightweight, non-terminating assertion approach and a small test harness.
// The tests focus on verifying the behavior of cJSON_AddObjectToObject as described in the focal method.
//
// Key dependencies considered (Candidate Keywords):
// - cJSON_CreateObject, add_item_to_object, cJSON_GetObjectItem, cJSON_IsObject
// - cJSON_Delete to ensure proper memory management
// - global_hooks as used by cJSON_AddObjectToObject
// - The positive path (true branch) when insertion succeeds
// - The negative path (false branch) when insertion fails (simulated by non-object parent)
// - Coverage of multiple child insertions on the same parent
//
// Notes:
// - This test suite assumes the existence of the cJSON.c implementation linked with the test binary.
// - The tests use only the C++ standard library and the provided cJSON interface.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Lightweight test framework (non-terminating, suitable for C++11)
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAILED] " << __FUNCTION__ << ": " << msg << "\n"; \
            return false; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "[FAILED] " << __FUNCTION__ << ": " << msg \
                      << " (expected: " << (a) << " == " << (b) << ")\n"; \
            return false; \
        } \
    } while (0)

#define RUN_TEST(test_fn) \
    do { \
        ++g_total_tests; \
        if ((test_fn)()) { \
            ++g_passed_tests; \
        } else { \
            ++g_failed_tests; \
        } \
    } while (0)

// Test 1: Positive path - ensuring that cJSON_AddObjectToObject successfully creates an inner object
// and attaches it to the parent under the provided name.
// This verifies the true branch of the conditional and basic object integration.
static bool test_AddObjectToObject_Succeeds_AttachesChild() {
    // Candidate Keywords: cJSON_CreateObject, add_item_to_object, cJSON_GetObjectItem, cJSON_IsObject
    cJSON *parent = cJSON_CreateObject();
    EXPECT_TRUE(parent != NULL, "Parent object creation should succeed");

    // Act: add a new object under key "child"
    cJSON *child = cJSON_AddObjectToObject(parent, "child");

    // Assert: the insertion should succeed and return the newly created object
    EXPECT_TRUE(child != NULL, "Returned child should not be NULL on success");

    if (child != NULL) {
        // The child should be of type object
        EXPECT_TRUE(cJSON_IsObject(child) != 0, "Returned child should be an object");

        // The parent should now contain the item under key "child"
        cJSON *retrieved = cJSON_GetObjectItem(parent, "child");
        EXPECT_TRUE(retrieved == child, "Retrieved item should match the returned child object");

        // Extra sanity: ensure the child is empty (no members) initially
        // There is no direct GetObjectSize, but we can attempt to retrieve a non-existent key
        cJSON *nonexistent = cJSON_GetObjectItem(child, "not_there");
        EXPECT_TRUE(nonexistent == NULL, "Newly created object should not have members yet");
    }

    // Cleanup
    cJSON_Delete(parent);

    return true;
}

// Test 2: Negative path - simulate failure path by providing a non-object parent.
// The function should return NULL and clean up any allocated resources for the inner object.
// This exercises the false branch of the conditional.
static bool test_AddObjectToObject_Fails_When_ParentNotObject() {
    // Candidate Keywords: cJSON_CreateString, cJSON_AddObjectToObject, cJSON_Delete
    cJSON *non_object_parent = cJSON_CreateString("not_an_object");
    EXPECT_TRUE(non_object_parent != NULL, "Non-object parent creation should succeed");

    // Act: attempt to add an inner object to a non-object parent
    cJSON *child = cJSON_AddObjectToObject(non_object_parent, "child");

    // Assert: the operation should fail and return NULL
    EXPECT_TRUE(child == NULL, "Expected NULL when parent is not an object");

    // Cleanup
    cJSON_Delete(non_object_parent);

    return true;
}

// Test 3: Multiple children - verify that multiple successful insertions can coexist under the same parent.
// This further validates the true-branch behavior for successive calls.
static bool test_AddObjectToObject_MultipleChildren() {
    // Candidate Keywords: cJSON_CreateObject, cJSON_AddObjectToObject, cJSON_GetObjectItem
    cJSON *parent = cJSON_CreateObject();
    EXPECT_TRUE(parent != NULL, "Parent object creation should succeed");

    cJSON *childA = cJSON_AddObjectToObject(parent, "alpha");
    cJSON *childB = cJSON_AddObjectToObject(parent, "beta");

    EXPECT_TRUE(childA != NULL, "First child (alpha) should be created successfully");
    EXPECT_TRUE(childB != NULL, "Second child (beta) should be created successfully");

    if (childA != NULL && childB != NULL) {
        // Verify that both children are retrievable by their keys and are distinct
        cJSON *retrievedA = cJSON_GetObjectItem(parent, "alpha");
        cJSON *retrievedB = cJSON_GetObjectItem(parent, "beta");
        EXPECT_TRUE(retrievedA == childA, "Retrieved alpha should match the returned childA");
        EXPECT_TRUE(retrievedB == childB, "Retrieved beta should match the returned childB");
        EXPECT_TRUE(retrievedA != retrievedB, "alpha and beta should be distinct objects");
        // Optional: ensure both are objects
        EXPECT_TRUE(cJSON_IsObject(childA) != 0, "alpha should be an object");
        EXPECT_TRUE(cJSON_IsObject(childB) != 0, "beta should be an object");
    }

    // Cleanup
    cJSON_Delete(parent);

    return true;
}

// Main entry to run tests
int main() {
    std::cout << "Running cJSON_AddObjectToObject unit tests (C++11)..." << std::endl;

    RUN_TEST(test_AddObjectToObject_Succeeds_AttachesChild);
    RUN_TEST(test_AddObjectToObject_Fails_When_ParentNotObject);
    RUN_TEST(test_AddObjectToObject_MultipleChildren);

    std::cout << "Tests run: " << g_total_tests
              << ", Passed: " << g_passed_tests
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    if (g_failed_tests > 0) {
        return 1;
    }
    return 0;
}
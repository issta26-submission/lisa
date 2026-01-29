// Comprehensive unit test suite for cJSON_CreateObjectReference (CJSON.c) using C++11 (no GTest).
// This test focuses on ensuring that creating an object reference correctly marks the new item
// as an object reference and points to the provided child (or NULL when given NULL).

// Notes on Candidate Keywords and dependencies (Step 1):
// - Candidate Keywords: cJSON_CreateObjectReference, cJSON_New_Item, global_hooks, cJSON_Object,
//   cJSON_IsReference, cast_away_const, internal_hooks, child, NULL, reference semantics.
// - Primary behavior under test: creating a new cJSON item that represents an object reference to
//   the provided child (or NULL), with the type field containing both cJSON_Object and cJSON_IsReference.
// - Static helpers (like cJSON_New_Item and cast_away_const) are used internally in the focal function.
// - The tests rely on the public API exposed by cJSON.h (cJSON_CreateObjectReference, cJSON_CreateObject, cJSON_Delete, etc.)
// - We verify: non-null return, correct type flags, and correct child pointer (pointer identity, not content).
// - We also exercise the NULL-child case to ensure item->child is NULL and the type flags are set accordingly.
// - We avoid touching private/static internals and use provided public APIs only.
// - We use a light-weight test harness (no external testing framework) and non-terminating checks.

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


// Include the CJSON public API header. This provides declarations for cJSON_CreateObjectReference
// and other related API functions used in the tests.

// Simple non-terminating test harness
static int test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE failed: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) \
                  << " | expected: " << (b) << " actual: " << (a) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " << (msg) \
                  << " | expected: " << b << " actual: " << a \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++test_failures; \
    } \
} while (0)

// Test 1: Non-null child creates a proper object reference to an existing object
void test_CreateObjectReference_with_nonnull_child() {
    // Candidate Keywords mapping:
    // - child is a real cJSON object to be referenced
    // - The function should set type to cJSON_Object | cJSON_IsReference
    // - The returned item should point to the original child via item->child
    // - Ownership semantics: the child should remain valid after creating the reference
    cJSON *child = cJSON_CreateObject();
    EXPECT_TRUE(child != NULL, "Child object for reference creation should be non-null");

    cJSON *ref = cJSON_CreateObjectReference(child);
    EXPECT_TRUE(ref != NULL, "cJSON_CreateObjectReference should return a non-null item when child is non-null");

    // The type should combine OBJECT and IS_REFERENCE flags
    int expected_type = cJSON_Object | cJSON_IsReference;
    EXPECT_EQ(ref->type, expected_type, "Reference item type should be OBJECT | IS_REFERENCE");

    // The reference's child should point to the same memory as 'child'
    EXPECT_PTR_EQ(ref->child, child, "The reference's child pointer should equal the original child pointer");

    // Ensure that the reference is distinct from the original child
    EXPECT_FALSE(ref == child, "The reference item should be a distinct object from the original child");

    // Cleanup
    cJSON_Delete(ref);
    cJSON_Delete(child);

    // This test validates the successful path (true branch) of cJSON_CreateObjectReference.
}

// Test 2: Null child should still create a valid object-reference item with NULL child
void test_CreateObjectReference_with_null_child() {
    // Candidate Keywords mapping:
    // - child = NULL should result in an item with item->child == NULL
    // - The type should still be OBJECT | IS_REFERENCE
    cJSON *ref = cJSON_CreateObjectReference(nullptr);
    EXPECT_TRUE(ref != NULL, "cJSON_CreateObjectReference should return non-null when child is NULL");

    int expected_type = cJSON_Object | cJSON_IsReference;
    EXPECT_EQ(ref->type, expected_type, "Reference item type should be OBJECT | IS_REFERENCE even when child is NULL");

    EXPECT_TRUE(ref->child == NULL, "The reference's child should be NULL when input child is NULL");

    // If available, verify semantics via helper
    bool isObject = cJSON_IsObject(ref);
    EXPECT_TRUE(isObject, "The reference item should still be recognized as an object by cJSON_IsObject");

    // Cleanup
    cJSON_Delete(ref);
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Running tests for cJSON_CreateObjectReference (Step 2/3)" << std::endl;

    test_CreateObjectReference_with_nonnull_child();
    test_CreateObjectReference_with_null_child();

    if (test_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Tests finished with " << test_failures << " failure(s)." << std::endl;
        return test_failures;
    }
}
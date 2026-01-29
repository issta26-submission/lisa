/*
Unit test suite for the focal method:
CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child)

Step 1 (Program Understanding) - Candidate Keywords:
- global_hooks
- cJSON_New_Item
- cJSON_Array
- cJSON_IsReference
- cast_away_const
- child (const cJSON *)
- item (cJSON *)
- item->type (bitwise flags)
- item->child (pointer to child)
These keywords indicate the core dependencies and behaviors we will validate:
- Creation path via cJSON_New_Item
- Setting type flags: cJSON_Array | cJSON_IsReference
- Referencing the input child via cast_away_const
- Behavior when child is NULL vs non-NULL

Step 2 (Unit Test Generation)
We will exercise:
- Non-NULL child: ensure returned item is created, has correct type flags, and item->child points to the exact input child.
- NULL child: ensure returned item is created, has correct type flags, and item->child is NULL.
We will not rely on GTest; a small, non-terminating assertion framework will be implemented inline.

Step 3 (Test Case Refinement)
- Use C++11-compliant code that includes the C library header (cJSON.h) and links with cJSON.c at build time.
- Use non-terminating EXPECT_* style assertions to maximize coverage.
- Access static/internal details only through the public API and documented behavior (as per the contract of cJSON_CreateArrayReference).
- Provide explanatory comments for each test case.
- Implement a minimal test harness in main() to run tests and summarize results.

Note: The tests are crafted to compile with a C++11 compiler alongside the provided cJSON.c/h sources. We avoid private/internal calls beyond what’s exposed via the public API and documented behavior. Memory management in tests is conservative to avoid double-free risks with reference items; tests will delete the created reference item and, for safety, will not explicitly delete the original child to avoid potential double free depending on library ownership semantics.

Below is the test code.

*/

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Import C declarations for the cJSON library in a C++ compatible way.
extern "C" {
}

// Simple non-terminating test harness (no GTest). We accumulate failures and continue.
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_NONNULL(p, msg) do { \
    if((p) == NULL) { \
        std::cerr << "EXPECT_NONNULL FAILED: " << (msg) << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_PTR_EQ FAILED: " << (msg) \
                  << " (expected " << static_cast<void*>(const_cast<cJSON*>(b)) \
                  << ", got " << static_cast<void*>(const_cast<cJSON*>(a)) << ")" \
                  << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

// Test 1: Non-NULL child should be wrapped as an Array reference pointing to the child.
void test_CreateArrayReference_WithNonNullChild() {
    // Create a non-NULL child (a simple string node)
    cJSON* child = cJSON_CreateString("child");
    EXPECT_NONNULL(child, "child should be created successfully for test 1");

    // Create an array reference to the child
    cJSON* ref = cJSON_CreateArrayReference(child);
    EXPECT_NONNULL(ref, "array reference should be created for non-NULL child");

    // Validate type flags: should be an Array and marked as a reference
    EXPECT_TRUE((ref->type & cJSON_Array) != 0, "ref should have cJSON_Array type flag set");
    EXPECT_TRUE((ref->type & cJSON_IsReference) != 0, "ref should have cJSON_IsReference flag set");

    // Validate the reference points to the exact input child
    EXPECT_PTR_EQ(ref->child, child, "ref->child should point to the original child exactly");

    // Clean up: delete the reference item. Do not delete the child to avoid potential double-free
    // semantics between reference and original item (ownership semantics of cJSON_IsReference).
    cJSON_Delete(ref);
    // Note: We intentionally avoid cJSON_Delete(child) to prevent possible double-free
    // if the reference does not own the child. Leaks here are acceptable for unit testing purposes.
}

// Test 2: NULL child should still yield a valid array reference with NULL child pointer.
void test_CreateArrayReference_WithNullChild() {
    // Create an array reference to NULL
    cJSON* ref = cJSON_CreateArrayReference(NULL);
    EXPECT_NONNULL(ref, "array reference should be created for NULL child");

    // Validate type flags: should be an Array and marked as a reference
    EXPECT_TRUE((ref->type & cJSON_Array) != 0, "ref should have cJSON_Array type flag set (NULL child case)");
    EXPECT_TRUE((ref->type & cJSON_IsReference) != 0, "ref should have cJSON_IsReference flag set (NULL child case)");

    // The child pointer should be NULL
    EXPECT_NONNULL(ref, "ref should be non-NULL in NULL child test");
    EXPECT_TRUE(ref->child == NULL, "ref->child should be NULL when input child is NULL");

    // Clean up
    cJSON_Delete(ref);
}

// Simple test runner
int main() {
    std::cout << "Running test suite for cJSON_CreateArrayReference...\n";

    test_CreateArrayReference_WithNonNullChild();
    test_CreateArrayReference_WithNullChild();

    if (g_tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_tests_failed << " test(s) FAILED." << std::endl;
        return 1;
    }
}

/*
Notes for maintainers:
- This test suite targets the focal method cJSON_CreateArrayReference and validates:
  - Correct creation path (non-NULL child)
  - Correct type flags (cJSON_Array | cJSON_IsReference)
  - Correct referencing behavior (item->child points to the input child)
  - Behavior when child is NULL (ref->child == NULL)
- We rely on the public API and the library's documented semantics of IsReference.
- We avoid using GTest/GMock as requested; test results are produced via a small harness in main().
- Static/internal helper functions in cJSON.c are not mocked; we exercise the public surface only.
- This test assumes the presence of cJSON.h and a linked build with cJSON.c.
*/
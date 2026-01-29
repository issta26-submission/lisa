/*
Step 1: Program Understanding (cJSON_GetNumberValue)
- Functionality: Returns the numeric value from a cJSON item if the item is a number; otherwise returns NAN.
- Core dependency: cJSON_IsNumber(item) check; on true, returns item->valuedouble; on false, returns NAN.
- Related elements to cover in tests:
  - Positive case: a proper number item yields its valuedouble.
  - Negative case: non-number items yield NAN (e.g., string, boolean, object, array).
  - Edge cases could involve different numeric representations (though the API exposes valuedouble directly).

Candidate Keywords to guide test generation:
- cJSON_IsNumber
- valuedouble
- NAN
- cJSON_CreateNumber
- cJSON_CreateString / cJSON_CreateTrue / cJSON_CreateFalse / cJSON_CreateObject / cJSON_CreateArray
- cJSON_GetNumberValue
- cJSON_Delete
*/

#include <cmath>
#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

/* Simple, non-terminating test harness (GTest-like macros but standalone)
   - Keeps running all tests, collects pass/fail counts, and reports at the end.
   - This adheres to the constraint: "Use non-terminating assertions (e.g., EXPECT_*)".
*/

static int g_tests_total = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_tests_total; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        fprintf(stderr, "Test failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
    } \
} while (0)

#define EXPECT_DOUBLE_EQ(a, b) do { \
    ++g_tests_total; \
    if ((a) == (b)) { \
        ++g_tests_passed; \
    } else { \
        fprintf(stderr, "Test failed: expected %f, got %f (%s:%d)\n", (b), (a), __FILE__, __LINE__); \
    } \
} while (0)

#define EXPECT_NAN(x) do { \
    ++g_tests_total; \
    if (std::isnan((x))) { \
        ++g_tests_passed; \
    } else { \
        fprintf(stderr, "Test failed: expected NaN, got %f (%s:%d)\n", (x), __FILE__, __LINE__); \
    } \
} while (0)

/* Step 2: Unit Test Generation
   - Tests to cover both true and false branches of the predicate in cJSON_GetNumberValue.
   - True branch: item is a number -> returns item->valuedouble.
   - False branch: item is not a number -> returns NAN.
*/
static void test_GetNumberValue_NumberItem()
{
    // Basic positive cases
    {
        cJSON *n = cJSON_CreateNumber(0.0);
        double v = cJSON_GetNumberValue(n);
        EXPECT_DOUBLE_EQ(v, 0.0);
        cJSON_Delete(n);
    }
    {
        cJSON *n = cJSON_CreateNumber(1.5);
        double v = cJSON_GetNumberValue(n);
        EXPECT_DOUBLE_EQ(v, 1.5);
        cJSON_Delete(n);
    }
    {
        cJSON *n = cJSON_CreateNumber(-42.25);
        double v = cJSON_GetNumberValue(n);
        EXPECT_DOUBLE_EQ(v, -42.25);
        cJSON_Delete(n);
    }
    {
        // Large number
        cJSON *n = cJSON_CreateNumber(1e308);
        double v = cJSON_GetNumberValue(n);
        EXPECT_DOUBLE_EQ(v, 1e308);
        cJSON_Delete(n);
    }
}

static void test_GetNumberValue_NonNumberReturnsNaN()
{
    // String item
    {
        cJSON *s = cJSON_CreateString("abc");
        double v = cJSON_GetNumberValue(s);
        EXPECT_NAN(v);
        cJSON_Delete(s);
    }
    // Boolean true
    {
        cJSON *t = cJSON_CreateTrue();
        double v = cJSON_GetNumberValue(t);
        EXPECT_NAN(v);
        cJSON_Delete(t);
    }
    // Boolean false
    {
        cJSON *f = cJSON_CreateFalse();
        double v = cJSON_GetNumberValue(f);
        EXPECT_NAN(v);
        cJSON_Delete(f);
    }
    // Empty object
    {
        cJSON *obj = cJSON_CreateObject();
        double v = cJSON_GetNumberValue(obj);
        EXPECT_NAN(v);
        cJSON_Delete(obj);
    }
    // Array
    {
        cJSON *arr = cJSON_CreateArray();
        double v = cJSON_GetNumberValue(arr);
        EXPECT_NAN(v);
        cJSON_Delete(arr);
    }
}

// Additional test: ensure mixed items do not affect internal behavior of other types
static void test_GetNumberValue_MixedItems()
{
    // Create multiple items and ensure non-number items still yield NaN when queried individually
    {
        cJSON *num = cJSON_CreateNumber(3.14);
        cJSON *str = cJSON_CreateString("xyz");
        cJSON *arr = cJSON_CreateArray();
        cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.72));

        // proprie tests on non-number
        double v1 = cJSON_GetNumberValue(str);
        double v2 = cJSON_GetNumberValue(arr);
        EXPECT_NAN(v1);
        EXPECT_NAN(v2);

        // cleanup
        cJSON_Delete(num);
        cJSON_Delete(str);
        cJSON_Delete(arr);
    }
}

/* Step 3: Test Case Refinement
   - The tests above exercise the required paths with a focus on NAN semantics and exact value equality for numbers.
   - We used a minimal, self-contained C++11 test harness to satisfy the requirement of not using GTest.
   - Static/private-like behavior is not accessed directly; we interact with the public API only (cJSON_CreateNumber, cJSON_GetNumberValue, etc.).
*/

int main()
{
    // Run tests
    test_GetNumberValue_NumberItem();
    test_GetNumberValue_NonNumberReturnsNaN();
    test_GetNumberValue_MixedItems();

    // Summary
    printf("CJSON GetNumberValue tests: passed %d / %d tests.\n", g_tests_passed, g_tests_total);

    // Return non-zero if any test failed
    return (g_tests_passed == g_tests_total) ? 0 : 1;
}
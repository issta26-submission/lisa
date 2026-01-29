/*
Step 1: Program Understanding (Summary for test design)
- Focused method: cJSON_IsObject(const cJSON * const item)
- Behavior:
  - If item is NULL, returns false.
  - Otherwise, checks the low 8 bits of item->type and compares to cJSON_Object.
  - Uses a bitmask (item->type & 0xFF) to ignore higher bits.
- Key candidate components (used to drive tests):
  - NULL pointer handling
  - item->type equals cJSON_Object
  - item->type contains extra high bits (e.g., 0x100) in addition to cJSON_Object in the low 8 bits
  - item->type equals other types (e.g., cJSON_Array) should fail
- Test constraints:
  - Use only standard C++11 facilities and the provided C API.
  - Do not rely on GTest; implement a small non-terminating assertion mechanism.
  - Keep tests executable and cover both true and false branches of the predicate.
  - Access macros/constants from cJSON.h (e.g., cJSON_Object, cJSON_Array).
*/

/* Step 3/Final: C++11 test suite for cJSON_IsObject without GTest
   Explanations are provided as comments above each test function.
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


// Include cJSON headers with C linkage when compiling as C++
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness (collects failures but continues)
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::cerr << "EXPECT_TRUE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if((cond)) { \
        ++g_test_failures; \
        std::cerr << "EXPECT_FALSE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

/* Step 2: Unit Test Generation
   Tests for cJSON_IsObject:
   - NULL input -> false
   - item with type == cJSON_Object -> true
   - item::type has high bits set but low byte == cJSON_Object -> true
   - item with type == cJSON_Array -> false
*/

/* Test 1: NULL input should return false */
static void test_CJSON_IsObject_NULLInput(void)
{
    // Candidate Keywords: item == NULL path
    cJSON_bool result = cJSON_IsObject(NULL);
    EXPECT_FALSE(result);
}

/* Test 2: Exact object type should return true */
static void test_CJSON_IsObject_ExactObject(void)
{
    // Allocate a minimal cJSON object and set its type to cJSON_Object
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item == NULL) {
        std::cerr << "Memory allocation failed in test_CJSON_IsObject_ExactObject\n";
        ++g_test_failures;
        return;
    }
    item->type = cJSON_Object;
    cJSON_bool result = cJSON_IsObject(item);
    EXPECT_TRUE(result);
    free(item);
}

/* Test 3: High bits set but low byte == cJSON_Object should still be true
   (ensures masking with 0xFF works as intended)
*/
static void test_CJSON_IsObject_ObjectWithHighBits(void)
{
    // Set high bits (e.g., 0x100) and low byte to cJSON_Object (1)
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item == NULL) {
        std::cerr << "Memory allocation failed in test_CJSON_IsObject_ObjectWithHighBits\n";
        ++g_test_failures;
        return;
    }
    item->type = (cJSON_Object | 0x100); // low byte == 1 => cJSON_Object
    cJSON_bool result = cJSON_IsObject(item);
    EXPECT_TRUE(result);
    free(item);
}

/* Test 4: Different type (Array) should return false */
static void test_CJSON_IsObject_ArrayType(void)
{
    cJSON *item = (cJSON *)malloc(sizeof(cJSON));
    if (item == NULL) {
        std::cerr << "Memory allocation failed in test_CJSON_IsObject_ArrayType\n";
        ++g_test_failures;
        return;
    }
    item->type = cJSON_Array;
    cJSON_bool result = cJSON_IsObject(item);
    EXPECT_FALSE(result);
    free(item);
}

/* Step 3: Test Case Refinement
   - All tests are self-contained and do not rely on static helpers beyond cJSON.h.
   - Tests execute in main(); non-terminating assertions ensure full coverage.
   - Static functions within the C file are not invoked directly; only the public API is tested.
*/

static void run_all_tests(void)
{
    test_CJSON_IsObject_NULLInput();
    test_CJSON_IsObject_ExactObject();
    test_CJSON_IsObject_ObjectWithHighBits();
    test_CJSON_IsObject_ArrayType();
}

int main(void)
{
    run_all_tests();

    if (g_test_failures == 0) {
        std::cout << "All tests passed\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed\n";
        return 1;
    }
}
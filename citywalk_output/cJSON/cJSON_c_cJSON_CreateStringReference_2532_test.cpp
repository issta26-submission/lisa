// Test suite for cJSON_CreateStringReference using C++11 (no Google Test)
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


// Simple non-terminating test harness
static int g_failures = 0;

#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic sanity check for a normal non-NULL input string
// - The function should return a non-NULL cJSON item.
// - The item type should have cJSON_String and cJSON_IsReference bits set.
// - valuestring should point to the exact input string (pointer equality).
void test_CreateStringReference_basic() {
    const char *input = "hello_world";
    cJSON *item = cJSON_CreateStringReference(input);

    TEST_ASSERT(item != NULL, "cJSON_CreateStringReference should not return NULL for non-NULL input");

    if (item != NULL) {
        TEST_ASSERT((item->type & cJSON_String) != 0, "Item type should include cJSON_String flag");
        TEST_ASSERT((item->type & cJSON_IsReference) != 0, "Item type should include cJSON_IsReference flag");
        TEST_ASSERT(item->valuestring == input, "valuestring should be the same pointer as the input string (pointer equality)");
        cJSON_Delete(item);
    }
}

// Test 2: NULL input handling
// - The function should still return a non-NULL item.
// - valuestring should be NULL.
// - Type flags should still indicate a string reference.
void test_CreateStringReference_null_input() {
    cJSON *item = cJSON_CreateStringReference(NULL);

    TEST_ASSERT(item != NULL, "cJSON_CreateStringReference(NULL) should return a valid item");

    if (item != NULL) {
        TEST_ASSERT(item->valuestring == NULL, "valuestring should be NULL when input is NULL");
        TEST_ASSERT((item->type & cJSON_String) != 0, "Item type should include cJSON_String flag");
        TEST_ASSERT((item->type & cJSON_IsReference) != 0, "Item type should include cJSON_IsReference flag");
        cJSON_Delete(item);
    }
}

// Test 3: Pointer stability with dynamically allocated string
// - Ensure that a dynamically allocated string passed in as input remains accessible
//   via the item, and that the pointer is preserved (no copying of string data).
void test_CreateStringReference_dynamic_input_pointer_preserved() {
    char *dynamic_str = (char*)malloc(64);
    std::strcpy(dynamic_str, "dynamic_input");
    cJSON *item = cJSON_CreateStringReference(dynamic_str);

    TEST_ASSERT(item != NULL, "cJSON_CreateStringReference should not return NULL for dynamic input");
    if (item != NULL) {
        TEST_ASSERT(item->valuestring == dynamic_str, "valuestring should be the same pointer as the dynamic input");
        // Change the original buffer and verify that the pointer remains the same (contents may change)
        std::strcpy(dynamic_str, "changed_content");
        TEST_ASSERT(std::strcmp(item->valuestring, "changed_content") == 0, "valuestring should reflect changes in the underlying buffer");
        cJSON_Delete(item);
        free(dynamic_str);
    }
}

int main() {
    // Run tests
    test_CreateStringReference_basic();
    test_CreateStringReference_null_input();
    test_CreateStringReference_dynamic_input_pointer_preserved();

    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    }
}
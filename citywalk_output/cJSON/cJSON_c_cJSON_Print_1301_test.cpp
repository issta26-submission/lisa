// C++11 test suite for the focal method: cJSON_Print in cJSON.c
// This test suite uses a lightweight, non-terminating assertion mechanism
// (EXPECT_* macros) and runs from main() without any external testing framework.
// It exercises cJSON_Print by performing round-trip verification: construct a
// JSON object using the public C API, print it, re-parse the string, and compare
// the parsed structure to the original using cJSON_Compare.

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
#include <string>
#include <limits.h>


extern "C" {
}

// Lightweight non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << (msg) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    ++g_total; \
    if((ptr) == NULL) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << (msg) \
                  << " (pointer is NULL) (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)


// Helper: perform a round-trip test for a given cJSON item
static void test_roundtrip(const cJSON *item, const char *desc)
{
    // Printing the item (formatted JSON)
    char *printed = cJSON_Print(item);
    EXPECT_NOT_NULL(printed, std::string(desc) + " - cJSON_Print returned NULL");

    if (printed != NULL) {
        // Parse the printed JSON back
        cJSON *parsed = cJSON_Parse(printed);
        if (parsed == NULL) {
            EXPECT_TRUE(false, std::string(desc) + " - cJSON_Parse of printed output failed");
        } else {
            // Compare the original item with the parsed item (structure/content equality)
            cJSON_bool eq = cJSON_Compare(item, parsed, true);
            EXPECT_TRUE(eq != 0, std::string(desc) + " - round-trip structures do not match");
            cJSON_Delete(parsed);
        }
        // Free the printed string (memory allocated by cJSON_Print)
        cJSON_free(printed);
    }
}


// Test 1: Simple object with a string and a number
static void test_Print_SimpleObject()
{
    // Purpose: Ensure that a basic object round-trips correctly through cJSON_Print.
    // - Object: { "name": "ChatGPT", "version": 3.5 }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        EXPECT_TRUE(false, "test_Print_SimpleObject - failed to create root object");
        return;
    }

    cJSON_AddStringToObject(root, "name", "ChatGPT");
    cJSON_AddNumberToObject(root, "version", 3.5);

    test_roundtrip(root, "Simple object round-trip");

    cJSON_Delete(root);
}


// Test 2: Nested structure with arrays and booleans
static void test_Print_NestedStructure()
{
    // Purpose: Validate cJSON_Print handles complex nesting:
    // {
    //   "title": "Example",
    //   "tags": ["alpha", "beta"],
    //   "stats": { "count": 10, "valid": true }
    // }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        EXPECT_TRUE(false, "test_Print_NestedStructure - failed to create root object");
        return;
    }

    cJSON_AddStringToObject(root, "title", "Example");

    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(array, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "tags", array);

    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "count", 10);
    cJSON_AddBoolToObject(stats, "valid", 1);

    cJSON_AddItemToObject(root, "stats", stats);

    test_roundtrip(root, "Nested structure round-trip");

    cJSON_Delete(root);
}


// Test 3: Empty object
static void test_Print_EmptyObject()
{
    // Purpose: Ensure that an empty object prints and round-trips correctly.
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        EXPECT_TRUE(false, "test_Print_EmptyObject - failed to create root object");
        return;
    }

    test_roundtrip(root, "Empty object round-trip");

    cJSON_Delete(root);
}


// Test 4: Unformatted vs Formatted consistency
static void test_Print_Formatted_vs_Unformatted()
{
    // Purpose: Ensure that both cJSON_Print (formatted) and cJSON_PrintUnformatted
    // produce consistent content when parsed back.

    // Create an example object
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddBoolToObject(root, "flag", 1);
    cJSON_AddNumberToObject(root, "pi", 3.14159);

    // Formatted
    char *formatted = cJSON_Print(root);
    // Unformatted
    char *unformatted = cJSON_PrintUnformatted(root);

    EXPECT_NOT_NULL(formatted, "test_Print_Formatted_vs_Unformatted - Formatted print NULL");
    EXPECT_NOT_NULL(unformatted, "test_Print_Formatted_vs_Unformatted - Unformatted print NULL");

    if (formatted && unformatted) {
        // Parse both and compare with the original
        cJSON *pf = cJSON_Parse(formatted);
        cJSON *pu = cJSON_Parse(unformatted);
        if (pf && pu) {
            cJSON_bool eqf = cJSON_Compare(root, pf, true);
            cJSON_bool equ = cJSON_Compare(root, pu, true);
            EXPECT_TRUE(eqf != 0, "Formatted print round-trip should match original");
            EXPECT_TRUE(equ != 0, "Unformatted print round-trip should match original");
            cJSON_Delete(pf);
            cJSON_Delete(pu);
        } else {
            if (pf == NULL) {
                EXPECT_TRUE(false, "test_Print_Formatted_vs_Unformatted - cannot parse formatted output");
            }
            if (pu == NULL) {
                EXPECT_TRUE(false, "test_Print_Formatted_vs_Unformatted - cannot parse unformatted output");
            }
        }
    }

    // Cleanup
    cJSON_free(formatted);
    cJSON_free(unformatted);
    cJSON_Delete(root);
}


// Test 5: Deeply nested object to stress recursion in print
static void test_Print_DeeplyNested()
{
    // Build: { "level0": { "level1": { "level2": { "level3": "deep" } } } }
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        EXPECT_TRUE(false, "test_Print_DeeplyNested - failed to create root");
        return;
    }

    cJSON *l0 = cJSON_CreateObject();
    cJSON *l1 = cJSON_CreateObject();
    cJSON *l2 = cJSON_CreateObject();
    cJSON *l3 = cJSON_CreateString("deep");

    cJSON_AddItemToObject(l2, "level3", l3);
    cJSON_AddItemToObject(l1, "level2", l2);
    cJSON_AddItemToObject(l0, "level1", l1);
    cJSON_AddItemToObject(root, "level0", l0);

    test_roundtrip(root, "Deeply nested object round-trip");

    cJSON_Delete(root);
}


// Entry point: run all tests
int main(void)
{
    std::cout << "Starting cJSON_Print test suite (C++11)" << std::endl;

    test_Print_SimpleObject();
    test_Print_NestedStructure();
    test_Print_EmptyObject();
    test_Print_Formatted_vs_Unformatted();
    test_Print_DeeplyNested();

    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;

    // Return non-zero if there were failures to aid CI integration
    return (g_failed == 0) ? 0 : 1;
}
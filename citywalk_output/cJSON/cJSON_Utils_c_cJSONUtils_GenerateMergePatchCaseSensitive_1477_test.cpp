// Test suite for cJSONUtils_GenerateMergePatchCaseSensitive
// Target: C++11, no Google Test, a simple non-terminating assertion style
// The tests exercise the merge-patch patch generation and application logic
// by comparing the resulting JSON against expected outcomes.

#include <cstring>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cJSON_Utils.h>
#include <limits.h>


extern "C" {
}

// Helper to print JSON objects in a deterministic, unformatted form
static char* print_unformatted(const cJSON* item) {
    if (!item) return NULL;
    return cJSON_PrintUnformatted(item);
}

// Helper to compare two JSON strings for equality
static bool json_strings_equal(const char* a, const char* b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    return std::strcmp(a, b) == 0;
}

// Simple non-terminating assertion logger
static void log_failure(const char* test_name, const char* message) {
    fprintf(stderr, "Test %s FAILED: %s\n", test_name, message);
}

// Test 1: Basic addition
// from: {}
// to:   {"a": 1}
// Expectation: generated patch when applied to 'from' yields 'to'
static int test_generate_merge_patch_case_sensitive_addition() {
    int failures = 0;
    const char* test_name = "GenerateMergePatchCaseSensitive_Addition";

    // Build 'from' and 'to'
    cJSON* from = cJSON_CreateObject();
    cJSON* to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 1);

    // Generate patch and apply to a copy of 'from'
    cJSON* patch = cJSONUtils_GenerateMergePatchCaseSensitive(from, to);
    if (patch == NULL) {
        log_failure(test_name, "Patch generation returned NULL");
        ++failures;
        // Cleanup but continue
        cJSON_Delete(from);
        cJSON_Delete(to);
        return failures;
    }

    cJSON* target = cJSON_Duplicate(from, 1);
    cJSON* result = cJSONUtils_MergePatchCaseSensitive(target, patch);

    char* to_str = print_unformatted(to);
    char* res_str = print_unformatted(result);

    if (!json_strings_equal(to_str, res_str)) {
        log_failure(test_name, "Resulting JSON does not match expected 'to' JSON for addition.");
        fprintf(stderr, "Expected: %s\n", to_str ? to_str : "NULL");
        fprintf(stderr, "Got     : %s\n", res_str ? res_str : "NULL");
        ++failures;
    }

    // Cleanup
    free(to_str);
    free(res_str);
    cJSON_Delete(patch);
    // 'result' shares ownership with 'target'; deleting either is enough
    cJSON_Delete(result);
    cJSON_Delete(from);
    cJSON_Delete(to);

    return failures;
}

// Test 2: Basic replacement
// from: {"a":1}
// to:   {"a":2}
// Expectation: patch modifies existing key 'a'
static int test_generate_merge_patch_case_sensitive_replacement() {
    int failures = 0;
    const char* test_name = "GenerateMergePatchCaseSensitive_Replacement";

    cJSON* from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);

    cJSON* to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 2);

    cJSON* patch = cJSONUtils_GenerateMergePatchCaseSensitive(from, to);
    if (patch == NULL) {
        log_failure(test_name, "Patch generation returned NULL");
        ++failures;
        cJSON_Delete(from);
        cJSON_Delete(to);
        return failures;
    }

    cJSON* target = cJSON_Duplicate(from, 1);
    cJSON* result = cJSONUtils_MergePatchCaseSensitive(target, patch);

    char* to_str = print_unformatted(to);
    char* res_str = print_unformatted(result);

    if (!json_strings_equal(to_str, res_str)) {
        log_failure(test_name, "Resulting JSON does not match expected 'to' JSON for replacement.");
        fprintf(stderr, "Expected: %s\n", to_str ? to_str : "NULL");
        fprintf(stderr, "Got     : %s\n", res_str ? res_str : "NULL");
        ++failures;
    }

    free(to_str);
    free(res_str);
    cJSON_Delete(patch);
    cJSON_Delete(result);
    cJSON_Delete(from);
    cJSON_Delete(to);

    return failures;
}

// Test 3: Deletion
// from: {"a":1,"b":2}
// to:   {"a":1}
// Expectation: patch removes key 'b'
static int test_generate_merge_patch_case_sensitive_deletion() {
    int failures = 0;
    const char* test_name = "GenerateMergePatchCaseSensitive_Deletion";

    cJSON* from = cJSON_CreateObject();
    cJSON_AddNumberToObject(from, "a", 1);
    cJSON_AddNumberToObject(from, "b", 2);

    cJSON* to = cJSON_CreateObject();
    cJSON_AddNumberToObject(to, "a", 1);

    cJSON* patch = cJSONUtils_GenerateMergePatchCaseSensitive(from, to);
    if (patch == NULL) {
        log_failure(test_name, "Patch generation returned NULL");
        ++failures;
        cJSON_Delete(from);
        cJSON_Delete(to);
        return failures;
    }

    cJSON* target = cJSON_Duplicate(from, 1);
    cJSON* result = cJSONUtils_MergePatchCaseSensitive(target, patch);

    char* to_str = print_unformatted(to);
    char* res_str = print_unformatted(result);

    if (!json_strings_equal(to_str, res_str)) {
        log_failure(test_name, "Resulting JSON does not match expected 'to' JSON for deletion.");
        fprintf(stderr, "Expected: %s\n", to_str ? to_str : "NULL");
        fprintf(stderr, "Got     : %s\n", res_str ? res_str : "NULL");
        ++failures;
    }

    free(to_str);
    free(res_str);
    cJSON_Delete(patch);
    cJSON_Delete(result);
    cJSON_Delete(from);
    cJSON_Delete(to);

    return failures;
}

// Test 4: Nested object changes
// from: {"a":{"b":1}}
// to:   {"a":{"b":2,"c":3}}
// Expectation: patch updates nested 'a.b' and adds 'a.c'
static int test_generate_merge_patch_case_sensitive_nested() {
    int failures = 0;
    const char* test_name = "GenerateMergePatchCaseSensitive_Nested";

    cJSON* from = cJSON_CreateObject();
    cJSON* fromA = cJSON_CreateObject();
    cJSON_AddNumberToObject(fromA, "b", 1);
    cJSON_AddItemToObject(from, "a", fromA);

    cJSON* to = cJSON_CreateObject();
    cJSON* toA = cJSON_CreateObject();
    cJSON_AddNumberToObject(toA, "b", 2);
    cJSON_AddNumberToObject(toA, "c", 3);
    cJSON_AddItemToObject(to, "a", toA);

    cJSON* patch = cJSONUtils_GenerateMergePatchCaseSensitive(from, to);
    if (patch == NULL) {
        log_failure(test_name, "Patch generation returned NULL");
        ++failures;
        cJSON_Delete(from);
        cJSON_Delete(to);
        return failures;
    }

    cJSON* target = cJSON_Duplicate(from, 1);
    cJSON* result = cJSONUtils_MergePatchCaseSensitive(target, patch);

    char* to_str = print_unformatted(to);
    char* res_str = print_unformatted(result);

    if (!json_strings_equal(to_str, res_str)) {
        log_failure(test_name, "Resulting JSON does not match expected nested 'to' JSON.");
        fprintf(stderr, "Expected: %s\n", to_str ? to_str : "NULL");
        fprintf(stderr, "Got     : %s\n", res_str ? res_str : "NULL");
        ++failures;
    }

    free(to_str);
    free(res_str);
    cJSON_Delete(patch);
    cJSON_Delete(result);
    cJSON_Delete(from);
    cJSON_Delete(to);

    return failures;
}

// Test 5: Array modification
// from: {"a":[1,2,3]}
// to:   {"a":[1,4,3]}
// Expectation: patch updates index 1 from 2 to 4
static int test_generate_merge_patch_case_sensitive_array_modification() {
    int failures = 0;
    const char* test_name = "GenerateMergePatchCaseSensitive_ArrayModification";

    cJSON* from = cJSON_CreateObject();
    cJSON* arrFrom = cJSON_CreateArray();
    cJSON_AddItemToArray(arrFrom, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arrFrom, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arrFrom, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(from, "a", arrFrom);

    cJSON* to = cJSON_CreateObject();
    cJSON* arrTo = cJSON_CreateArray();
    cJSON_AddItemToArray(arrTo, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arrTo, cJSON_CreateNumber(4));
    cJSON_AddItemToArray(arrTo, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(to, "a", arrTo);

    cJSON* patch = cJSONUtils_GenerateMergePatchCaseSensitive(from, to);
    if (patch == NULL) {
        log_failure(test_name, "Patch generation returned NULL");
        ++failures;
        cJSON_Delete(from);
        cJSON_Delete(to);
        return failures;
    }

    cJSON* target = cJSON_Duplicate(from, 1);
    cJSON* result = cJSONUtils_MergePatchCaseSensitive(target, patch);

    char* to_str = print_unformatted(to);
    char* res_str = print_unformatted(result);

    if (!json_strings_equal(to_str, res_str)) {
        log_failure(test_name, "Resulting JSON does not match expected array modification in 'to'.");
        fprintf(stderr, "Expected: %s\n", to_str ? to_str : "NULL");
        fprintf(stderr, "Got     : %s\n", res_str ? res_str : "NULL");
        ++failures;
    }

    free(to_str);
    free(res_str);
    cJSON_Delete(patch);
    cJSON_Delete(result);
    cJSON_Delete(from);
    cJSON_Delete(to);

    return failures;
}

int main() {
    int total_failures = 0;

    int f1 = test_generate_merge_patch_case_sensitive_addition();
    if (f1 > 0) total_failures += f1;
    else printf("PASS: GenerateMergePatchCaseSensitive_Addition\n");

    int f2 = test_generate_merge_patch_case_sensitive_replacement();
    if (f2 > 0) total_failures += f2;
    else printf("PASS: GenerateMergePatchCaseSensitive_Replacement\n");

    int f3 = test_generate_merge_patch_case_sensitive_deletion();
    if (f3 > 0) total_failures += f3;
    else printf("PASS: GenerateMergePatchCaseSensitive_Deletion\n");

    int f4 = test_generate_merge_patch_case_sensitive_nested();
    if (f4 > 0) total_failures += f4;
    else printf("PASS: GenerateMergePatchCaseSensitive_Nested\n");

    int f5 = test_generate_merge_patch_case_sensitive_array_modification();
    if (f5 > 0) total_failures += f5;
    else printf("PASS: GenerateMergePatchCaseSensitive_ArrayModification\n");

    if (total_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILED TESTS: %d\n", total_failures);
        return 1;
    }
}
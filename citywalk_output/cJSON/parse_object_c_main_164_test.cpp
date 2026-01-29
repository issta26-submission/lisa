// This is a C++11 test harness intended to validate the behavior of the focal
// parse_object.c tests that exercise parsing of cJSON objects via the main test
// runner. The original project uses the Unity framework in C for the tests
// (parse_object_should_parse_empty_objects, parse_object_should_not_parse_non_objects,
// parse_object_should_parse_objects_with_multiple_elements, parse_object_should_parse_objects_with_one_element).
// Since the instruction prohibits GTest and requires a test suite executable from C++11,
// this file provides a standalone C++ test harness that mimics those scenarios
// using the same underlying cJSON parsing library (cJSON_Parse). It does not rely
// on Unity and instead uses a lightweight, non-terminating assertion approach
// that accumulates failures and reports a final summary.
//
// Step 1 (Conceptual): Candidate Keywords (core components used by the focal method)
// - item (cJSON item memory), cJSON (data structure), parse_object, object, empty object,
//   non-objects (arrays, strings, numbers, booleans, null), multiple elements, one element,
//   keys (string names), types (cJSON_Number, cJSON_String, etc.), COUNT of children.
//
// Step 2 (Test targets aligned with focal test suite):
// - parse_empty_object -> {} should yield an object with 0 members
// - parse_non_object_input -> non-object inputs should not produce an object
// - parse_object_with_multiple_elements -> {"a":1,"b":2} has exactly two object members
// - parse_object_with_one_element -> {"name":"value"} has exactly one member named "name" with a string value
//
// Step 3 (Test refinement): Use non-terminating assertions; cover true/false branches;
// test harness writes verbose output but continues after failures; uses only the C
// standard library and the provided cJSON library.

#include <unity/src/unity.h>
#include <cstring>
#include <cJSON.h>
#include <unity/examples/unity_config.h>
#include <cstdio>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <stdio.h>


// Include the cJSON header from the project (C library). Compile as C++ with extern "C"
// to avoid name mangling issues.
extern "C" {
}

// Global counter for test failures (non-terminating assertions).
static int g_failures = 0;

// Lightweight failure reporter (non-terminating; continues testing).
static void report_failure(const char* json, const char* reason) {
    std::fprintf(stderr, "Test failure: %s | JSON: %s\n", reason, json);
    ++g_failures;
}

// Helper: Count the number of direct children of a cJSON object.
static int count_object_children(const cJSON* object_item) {
    int count = 0;
    for (const cJSON* child = object_item->child; child != nullptr; child = child->next) {
        ++count;
    }
    return count;
}

// Helper: Verify that the given JSON string parses to an object with the expected
// number of immediate children. Used for testing empty vs. non-empty objects.
static void assert_object_with_expected_count(const char* json, int expected_count) {
    cJSON* root = cJSON_Parse(json);
    if (root == nullptr) {
        report_failure(json, "Parse failed: expected a valid JSON object");
        return;
    }
    if ((root->type & cJSON_Object) == 0) {
        report_failure(json, "Parsed root is not a JSON object");
        cJSON_Delete(root);
        return;
    }
    int actual = count_object_children(root);
    if (actual != expected_count) {
        std::fprintf(stderr,
                     "Object member count mismatch: expected %d, got %d | JSON: %s\n",
                     expected_count, actual, json);
        ++g_failures;
    }
    cJSON_Delete(root);
}

// Helper: Verify that the object has exactly one member named "name" with a string value.
static void assert_object_with_one_element_name_string() {
    const char* json = "{\"name\":\"value\"}";
    cJSON* root = cJSON_Parse(json);
    if (root == nullptr) {
        report_failure(json, "Parse failed: expected an object with one string member");
        return;
    }
    if ((root->type & cJSON_Object) == 0) {
        report_failure(json, "Parsed root is not a JSON object");
        cJSON_Delete(root);
        return;
    }
    // Count and inspect the single member
    int count = 0;
    bool ok = true;
    for (const cJSON* child = root->child; child != nullptr; child = child->next) {
        ++count;
        if (child->string == nullptr || std::strcmp(child->string, "name") != 0) {
            ok = false;
            report_failure(json, "Expected a single member key named 'name'");
        }
        if ((child->type & cJSON_String) == 0) {
            ok = false;
            report_failure(json, "Expected the value of 'name' to be a string");
        }
    }
    if (count != 1) {
        report_failure(json, "Expected exactly 1 member in object");
        ok = false;
    }
    if (!ok) {
        // detailed failures already reported
    }
    cJSON_Delete(root);
}

// Helper: Verify that the object has exactly two members: "a" and "b", both numbers.
static void assert_object_with_two_number_elements() {
    const char* json = "{\"a\":1, \"b\":2}";
    cJSON* root = cJSON_Parse(json);
    if (root == nullptr) {
        report_failure(json, "Parse failed: expected an object with two numeric members");
        return;
    }
    if ((root->type & cJSON_Object) == 0) {
        report_failure(json, "Parsed root is not a JSON object");
        cJSON_Delete(root);
        return;
    }

    bool found_a = false;
    bool found_b = false;
    int count = 0;

    for (const cJSON* child = root->child; child != nullptr; child = child->next) {
        ++count;
        if (child->string == nullptr) {
            report_failure(json, "Member with missing key name");
            continue;
        }
        if (std::strcmp(child->string, "a") == 0) {
            found_a = true;
            if ((child->type & cJSON_Number) == 0) {
                report_failure(json, "Value for key 'a' is not a number");
            }
        } else if (std::strcmp(child->string, "b") == 0) {
            found_b = true;
            if ((child->type & cJSON_Number) == 0) {
                report_failure(json, "Value for key 'b' is not a number");
            }
        } else {
            char msg[256];
            std::snprintf(msg, sizeof(msg),
                          "Unexpected key '%s' found in object", child->string);
            report_failure(json, msg);
        }
    }

    if (count != 2) {
        std::fprintf(stderr, "Expected 2 members, got %d | JSON: %s\n", count, json);
        ++g_failures;
    }
    if (!found_a || !found_b) {
        std::fprintf(stderr, "Missing required keys 'a' and 'b' in object | JSON: %s\n", json);
        ++g_failures;
    }

    cJSON_Delete(root);
}

// Test 1: parse_object_should_parse_empty_objects
// Purpose: Ensure that parsing an empty object "{}" yields a valid cJSON object
// with zero members.
static void parse_object_should_parse_empty_objects(void) {
    // Empty object should parse and have 0 members
    assert_object_with_expected_count("{}", 0);
}

// Test 2: parse_object_should_parse_objects_with_one_element
// Purpose: Ensure that an object with a single element is parsed correctly, with
// the correct key and value type (string in this case).
static void parse_object_should_parse_objects_with_one_element(void) {
    // Reuse a focused assertion about a single expected key/value pair
    assert_object_with_one_element_name_string();
}

// Test 3: parse_object_should_parse_objects_with_multiple_elements
// Purpose: Ensure that an object with multiple elements is parsed correctly and
// that expected keys exist with correct value types.
static void parse_object_should_parse_objects_with_multiple_elements(void) {
    // Expect keys "a" and "b" both with numeric values
    assert_object_with_two_number_elements();
}

// Test 4: parse_object_should_not_parse_non_objects
// Purpose: Ensure that non-object JSON values (arrays, strings, numbers, booleans, null)
// do not get treated as objects by the parser.
static void parse_object_should_not_parse_non_objects(void) {
    const char* non_objects[] = {"[]", "\"string\"", "123", "true", "null"};
    const int total = sizeof(non_objects) / sizeof(non_objects[0]);
    for (int i = 0; i < total; ++i) {
        const char* json = non_objects[i];
        cJSON* root = cJSON_Parse(json);
        if (root != nullptr && (root->type & cJSON_Object)) {
            std::fprintf(stderr, "Non-object input erroneously parsed as object: %s\n", json);
            ++g_failures;
            cJSON_Delete(root);
        } else {
            if (root != nullptr) cJSON_Delete(root); // cleanup if parse returned non-object
        }
    }
}

// Step 3: Test Case Refinement
// - Tests are designed to be non-terminating: all tests run even if some fail.
// - They exercise both true and false branches: object vs. non-object, 0 vs. >0 members,
//   expected key names, and value types.
// - Static details are kept internal to each test; this harness does not depend on Unity/GTest.
// - Only the C standard library and cJSON are used, satisfying the provided dependencies.

int main(void) {
    // Run tests in the same order as the focal Unity RUN_TEST invocations.
    parse_object_should_parse_empty_objects();
    parse_object_should_not_parse_non_objects();
    parse_object_should_parse_objects_with_multiple_elements();
    parse_object_should_parse_objects_with_one_element();

    if (g_failures == 0) {
        std::printf("All parse_object tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "%d parse_object test(s) failed. See logs above for details.\n", g_failures);
        return 1;
    }
}
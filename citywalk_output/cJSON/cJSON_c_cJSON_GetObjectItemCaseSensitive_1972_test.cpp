/*
  Test Suite for cJSON_GetObjectItemCaseSensitive

  Step 1 (Program Understanding) summary:
  - Focal method: cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string)
    - It delegates to get_object_item(object, string, true) to fetch an object member by name
      with case-sensitive matching.
  - Core dependencies (from <FOCAL_CLASS_DEP> block):
    - cJSON object API: cJSON_CreateObject, cJSON_AddStringToObject, cJSON_GetStringValue, cJSON_Delete
    - Accessors: cJSON_GetObjectItemCaseSensitive, cJSON_GetObjectItem (non-case-sensitive)
    - Utility: NULL handling, case-sensitive comparison behavior via get_object_item
  - Key concepts to test:
    - Successful retrieval with exact case (true path)
    - Failure when key does not exist or case does not match (false path)
    - NULL object handling
    - Distinction between case-sensitive and case-insensitive retrieval

  Step 2 & 3 (Test Generation and Refinement):
  - Implement a small C++11 test harness (no GTest) targeting the C API of cJSON.
  - Provide 4 tests:
    1) Basic case-sensitive match succeeds
    2) Case-sensitive mismatch returns NULL
    3) NULL object returns NULL
    4) Case-insensitive retrieval matches while case-sensitive would fail (to contrast GetObjectItem vs GetObjectItemCaseSensitive)
  - Use non-terminating assertions: each test returns bool; main aggregates results.
  - Use only C++ standard library for test harness and explanatory comments; memory managed via cJSON_Delete.
  - Access public C API; do not rely on private/static internals.

  Notes:
  - This test suite assumes cJSON.h/cJSON.c are available in the build environment.
  - The tests are designed to be executable under a C++11 compiler environment.
  - To build: compile cJSON.c with a C compiler and link the resulting object with the C++ test file, or compile both as C++ if the environment supports C linkage for cJSON.c.
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
#include <string>
#include <limits.h>


// Bring in C API for cJSON with C linkage
extern "C" {
}

// Candidate Keywords (from Step 1) represented in tests
// - get_object_item, cJSON_GetObjectItemCaseSensitive, cJSON_GetObjectItem
// - cJSON_CreateObject, cJSON_AddStringToObject, cJSON_GetStringValue
// - NULL handling, case sensitivity semantics

// Simple test harness (non-GTest)
// - Each test returns true on success, false on failure
// - main aggregates results and prints a concise report

// Test 1: Basic case-sensitive match succeeds
bool test_GetObjectItemCaseSensitive_BasicMatch() {
    // Arrange
    cJSON *object = cJSON_CreateObject();
    if (object == nullptr) {
        std::cerr << "[Test1] Failed to create JSON object.\n";
        return false;
    }

    // Add a couple of keys to ensure basic retrieval works
    if (cJSON_AddStringToObject(object, "KeyOne", "value1") == nullptr) {
        std::cerr << "[Test1] Failed to add KeyOne string to object.\n";
        cJSON_Delete(object);
        return false;
    }
    if (cJSON_AddStringToObject(object, "AnotherKey", "value2") == nullptr) {
        std::cerr << "[Test1] Failed to add AnotherKey string to object.\n";
        cJSON_Delete(object);
        return false;
    }

    // Act
    cJSON *item = cJSON_GetObjectItemCaseSensitive(object, "KeyOne");

    // Assert
    bool ok = true;
    if (item == nullptr) {
        std::cerr << "[Test1] cJSON_GetObjectItemCaseSensitive returned NULL for existing key with exact case.\n";
        ok = false;
    } else {
        const char *value = cJSON_GetStringValue(item);
        if (value == nullptr || std::string(value) != "value1") {
            std::cerr << "[Test1] Retrieved value mismatch. Expected 'value1', got '"
                      << (value ? value : "NULL") << "'.\n";
            ok = false;
        }
    }

    // Cleanup
    cJSON_Delete(object);
    return ok;
}

// Test 2: Case-sensitive mismatch returns NULL
bool test_GetObjectItemCaseSensitive_MismatchReturnsNull() {
    // Arrange
    cJSON *object = cJSON_CreateObject();
    if (object == nullptr) {
        std::cerr << "[Test2] Failed to create JSON object.\n";
        return false;
    }

    if (cJSON_AddStringToObject(object, "KeyTwo", "value2") == nullptr) {
        std::cerr << "[Test2] Failed to add KeyTwo string to object.\n";
        cJSON_Delete(object);
        return false;
    }

    // Act
    cJSON *item = cJSON_GetObjectItemCaseSensitive(object, "keytwo"); // different case

    // Assert
    bool ok = (item == nullptr);
    if (!ok) {
        std::cerr << "[Test2] Expected NULL for case-sensitive mismatch, got non-NULL.\n";
    }

    // Cleanup
    cJSON_Delete(object);
    return ok;
}

// Test 3: NULL object handling
bool test_GetObjectItemCaseSensitive_NullObject() {
    // Act
    cJSON *item = cJSON_GetObjectItemCaseSensitive(nullptr, "KeyAny");

    // Assert
    bool ok = (item == nullptr);
    if (!ok) {
        std::cerr << "[Test3] Expected NULL when object is NULL.\n";
    }
    return ok;
}

// Test 4: Case-insensitive retrieval for contrast with case-sensitive
bool test_GetObjectItemCaseSensitive_ContrastWithCaseInsensitive() {
    // Arrange
    cJSON *object = cJSON_CreateObject();
    if (object == nullptr) {
        std::cerr << "[Test4] Failed to create JSON object.\n";
        return false;
    }
    // Add a key with a specific case
    if (cJSON_AddStringToObject(object, "CaseSensitiveKey", "sv") == nullptr) {
        std::cerr << "[Test4] Failed to add CaseSensitiveKey string.\n";
        cJSON_Delete(object);
        return false;
    }

    // Act: case-insensitive retrieval (should succeed)
    cJSON *item_insensitive = cJSON_GetObjectItem(object, "casesensitivekey"); // lowercased version
    // Act: case-sensitive retrieval (should fail)
    cJSON *item_sensitive = cJSON_GetObjectItemCaseSensitive(object, "casesensitivekey");

    bool ok = true;
    if (item_insensitive == nullptr) {
        std::cerr << "[Test4] Expected non-NULL for case-insensitive retrieval, got NULL.\n";
        ok = false;
    } else {
        const char *value = cJSON_GetStringValue(item_insensitive);
        if (value == nullptr || std::string(value) != "sv") {
            std::cerr << "[Test4] Insensitive retrieval value mismatch. Expected 'sv', got '"
                      << (value ? value : "NULL") << "'.\n";
            ok = false;
        }
    }

    if (item_sensitive != nullptr) {
        std::cerr << "[Test4] Expected NULL for case-sensitive retrieval of mismatching key, got non-NULL.\n";
        ok = false;
    }

    // Cleanup
    cJSON_Delete(object);
    return ok;
}

// Main test runner
int main() {
    int total = 0;
    int failed = 0;

    auto run = [&](const std::string& name, bool (*testFunc)()) {
        ++total;
        bool result = false;
        try {
            result = testFunc();
        } catch (...) {
            result = false;
            std::cerr << "[Test] Exception occurred in " << name << "\n";
        }
        if (!result) {
            ++failed;
            std::cerr << "[Test] FAILED: " << name << "\n";
        } else {
            std::cout << "[Test] PASSED: " << name << "\n";
        }
    };

    run("GetObjectItemCaseSensitive_BasicMatch", test_GetObjectItemCaseSensitive_BasicMatch);
    run("GetObjectItemCaseSensitive_MismatchReturnsNull", test_GetObjectItemCaseSensitive_MismatchReturnsNull);
    run("GetObjectItemCaseSensitive_NullObject", test_GetObjectItemCaseSensitive_NullObject);
    run("GetObjectItemCaseSensitive_ContrastWithCaseInsensitive", test_GetObjectItemCaseSensitive_ContrastWithCaseInsensitive);

    std::cout << "Tests run: " << total << ", Failures: " << failed << "\n";
    return failed ? 1 : 0;
}
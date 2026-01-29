/*
Step 1: Program Understanding (Candidate Keywords)
- Core function under test: cJSON_GetObjectItem(const cJSON * const object, const char * const string)
- Behavior: delegates to internal get_object_item(object, string, false) -> non-case-sensitive search for a key in a JSON object.
- Key dependencies (public API surface for tests): cJSON_CreateObject, cJSON_AddNumberToObject, cJSON_AddStringToObject, cJSON_GetObjectItem, cJSON_GetObjectItemCaseSensitive, cJSON_GetNumberValue, cJSON_GetStringValue, cJSON_IsNumber, cJSON_Delete, cJSON_CreateString, cJSON_AddBoolToObject, etc. All tests should rely on public APIs and not internal static helpers.
- Coverage focus: true/false branches around key lookup (existing vs. missing keys, case-insensitive behavior, case-sensitive variant, order of items when duplicates with different cases exist), null object handling.

Step 2: Unit Test Generation (comprehensive test suite for cJSON_GetObjectItem)
The test suite below uses a small C++11 test harness (no Google Test) to exercise cJSON_GetObjectItem and related public APIs. It validates:
- Return value for non-existent object returns NULL
- Case-insensitive search finds keys regardless of case
- Case-sensitive search only finds exact-case matches via cJSON_GetObjectItemCaseSensitive
- Behavior when multiple keys differ only by case returns the first encountered item for non-case-sensitive search
- Retrieval of numeric and string values via the returned item
- NULL object handling

Step 3: Test Case Refinement
- Tests cover true/false branches of key presence, absence, case-insensitive vs case-sensitive searches
- Use of static/static-like behavior not required; tests rely on public APIs
- Tests are designed to be non-terminating: failures are reported but do not abort execution
- A minimal test harness prints failures and a final summary; the main function calls all tests

Code (C++11 test suite for cJSON_GetObjectItem)
*/

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
#include <limits.h>


// Include C API for cJSON
extern "C" {
}

// Global counters for test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Lightweight non-terminating assertions
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

/*
Step 2, 3: Unit tests for cJSON_GetObjectItem
Test 1: Basic lookup and value retrieval (numeric value)
Test 2: Case-insensitive lookup (string key with differing case)
Test 3: Case-sensitive lookup behavior (exact case required)
Test 4: Duplicate keys differing by case; verify first match for non-case-sensitive lookup
Test 5: NULL object pointer returns NULL
Test 6: Lookup of a non-existent key returns NULL
Test 7: Retrieval of string value from found item
*/

static void test_GetObjectItem_BasicFinds()
{
    // Basic object with a numeric key
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "num", 123);
    cJSON *item = cJSON_GetObjectItem(obj, "num");
    EXPECT_TRUE(item != nullptr, "GetObjectItem should locate existing numeric key 'num'");
    if (item) {
        double v = cJSON_GetNumberValue(item);
        EXPECT_TRUE(v == 123.0, "Value of 'num' should be 123");
    }

    // Add a string value and verify retrieval
    cJSON_AddStringToObject(obj, "greet", "hello");
    cJSON *greetItem = cJSON_GetObjectItem(obj, "greet");
    EXPECT_TRUE(greetItem != nullptr, "GetObjectItem should locate existing string key 'greet'");
    if (greetItem) {
        const char *s = cJSON_GetStringValue(greetItem);
        EXPECT_TRUE(s != nullptr && std::strcmp(s, "hello") == 0, "Value of 'greet' should be 'hello'");
    }

    cJSON_Delete(obj);
}

static void test_GetObjectItem_CaseInsensitive()
{
    // Object with a key whose case will be compared insensitively
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "KeyName", 42);

    // Non-case-sensitive lookup should succeed regardless of case
    cJSON *item = cJSON_GetObjectItem(obj, "keyname");
    EXPECT_TRUE(item != nullptr, "Case-insensitive search should find 'KeyName' when querying 'keyname'");
    if (item) {
        EXPECT_TRUE(cJSON_GetNumberValue(item) == 42, "Value should be 42 for case-insensitive match");
    }

    // Case-sensitive lookup should fail for different case
    cJSON *cs = cJSON_GetObjectItemCaseSensitive(obj, "keyname");
    EXPECT_TRUE(cs == nullptr, "Case-sensitive search should not find 'keyname' when key is 'KeyName'");

    cJSON_Delete(obj);
}

static void test_GetObjectItem_CaseSensitive()
{
    // Object with a key that will be looked up case-sensitively
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "CaseKey", "value");

    // Exact match should succeed
    cJSON *csItem = cJSON_GetObjectItemCaseSensitive(obj, "CaseKey");
    EXPECT_TRUE(csItem != nullptr, "Case-sensitive lookup should find exact key 'CaseKey'");
    if (csItem) {
        const char *s = cJSON_GetStringValue(csItem);
        EXPECT_TRUE(s != nullptr && std::strcmp(s, "value") == 0, "Value should be 'value' for key 'CaseKey'");
    }

    // Different case should fail for case-sensitive lookup
    cJSON *csNotFound = cJSON_GetObjectItemCaseSensitive(obj, "casekey");
    EXPECT_TRUE(csNotFound == nullptr, "Case-sensitive lookup should not match when case differs");

    cJSON_Delete(obj);
}

static void test_GetObjectItem_DuplicateCaseInsensitiveOrder()
{
    // Two keys that differ only by case; ensure first match is returned for non-case-sensitive lookup
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "a", 1);
    cJSON_AddNumberToObject(obj, "A", 2);

    cJSON *found = cJSON_GetObjectItem(obj, "a");
    EXPECT_TRUE(found != nullptr, "Non-case-sensitive lookup should return first matching item for 'a'");
    if (found) {
        EXPECT_TRUE(cJSON_GetNumberValue(found) == 1, "First match should have value 1");
    }

    cJSON_Delete(obj);
}

static void test_GetObjectItem_NullObject()
{
    // NULL object pointer should yield NULL without crashing
    cJSON *found = cJSON_GetObjectItem(nullptr, "anything");
    EXPECT_TRUE(found == nullptr, "GetObjectItem with NULL object should return NULL");
}

static void test_GetObjectItem_NotFound()
{
    // Key not present should return NULL
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "present", 1);
    cJSON *notFound = cJSON_GetObjectItem(obj, "absent");
    EXPECT_TRUE(notFound == nullptr, "Lookup for non-existent key should return NULL");
    cJSON_Delete(obj);
}

int main()
{
    // Execute test cases
    test_GetObjectItem_BasicFinds();
    test_GetObjectItem_CaseInsensitive();
    test_GetObjectItem_CaseSensitive();
    test_GetObjectItem_DuplicateCaseInsensitiveOrder();
    test_GetObjectItem_NullObject();
    test_GetObjectItem_NotFound();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    return g_tests_failed ? 1 : 0;
}
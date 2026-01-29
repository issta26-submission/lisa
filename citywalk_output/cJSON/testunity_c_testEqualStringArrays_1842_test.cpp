// Test suite for the focal method testEqualStringArrays from the provided Unity-based test file.
// This C++11 test harness reproduces the core behavior of Unity's TEST_ASSERT_EQUAL_STRING_ARRAY
// and exercises both true and false branches to achieve good coverage.
// Candidate Keywords (Step 1): testStrings, expStrings, TEST_ASSERT_EQUAL_STRING_ARRAY, equalStringArray,
// NULL pointers, length (count), string comparison (strcmp), array-of-strings, non-terminating tests.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Core utility: compare two arrays of C strings for the first 'count' elements.
// Special handling to mimic Unity's behavior observed in the focal tests:
// - If count == 0: return true only if both arrays are NULL; false otherwise.
// - If both arrays are NULL and count > 0: return true.
// - If one array is NULL and the other is not: return false.
// - For non-NULL elements, use strcmp and treat NULL elements as unequal to non-NULL elements.
static bool equalStringArray(const char** a, const char** b, std::size_t count) {
    if (count == 0) {
        // Mimic Unity's behavior observed in testNotEqualStringArrayLengthZero:
        // when count is zero, the test should fail if only one side is NULL.
        if (a == NULL && b == NULL) return true;
        return false;
    }
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;

    for (std::size_t i = 0; i < count; ++i) {
        const char* va = a[i];
        const char* vb = b[i];
        if (va == NULL && vb == NULL) continue;
        if (va == NULL || vb == NULL) return false;
        if (std::strcmp(va, vb) != 0) return false;
    }
    return true;
}

// Mimic Unity's macro TEST_ASSERT_EQUAL_STRING_ARRAY(exp, act, count);
static void TEST_ASSERT_EQUAL_STRING_ARRAY(const char** expected, const char** actual, std::size_t count) {
    if (!equalStringArray(expected, actual, count)) {
        throw std::runtime_error("TEST_ASSERT_EQUAL_STRING_ARRAY failed");
    }
}

// Helpers for "each equal" style tests (assert all elements equal to a given string)
static bool equalStringToAll(const char* expected, const char** array, std::size_t count) {
    if (array == nullptr && count > 0) return false;
    if (count == 0) {
        // As with TEST_ASSERT_EACH_EQUAL_STRING in Unity, zero-length checks can be treated as pass
        // unless Unity's semantics specify otherwise; based on typical usage, consider as pass.
        return true;
    }
    for (std::size_t i = 0; i < count; ++i) {
        if (array[i] == nullptr) return false;
        if (std::strcmp(array[i], expected) != 0) return false;
    }
    return true;
}
static void TEST_ASSERT_EACH_EQUAL_STRING(const char* expected, const char** array, std::size_t count) {
    if (!equalStringToAll(expected, array, count)) {
        throw std::runtime_error("TEST_ASSERT_EACH_EQUAL_STRING failed");
    }
}

// Simple test harness
struct TestCase {
    std::string name;
    std::function<void()> func;
};

static void runTests(const std::vector<TestCase>& tests) {
    int failures = 0;
    for (const auto& t : tests) {
        try {
            t.func();
            std::cout << "[PASS] " << t.name << "\n";
        } catch (const std::exception& ex) {
            ++failures;
            std::cout << "[FAIL] " << t.name << " - " << ex.what() << "\n";
        } catch (...) {
            ++failures;
            std::cout << "[FAIL] " << t.name << " - Unknown exception\n";
        }
    }
    std::cout << "Total failures: " << failures << "\n";
}

// Step 2: Unit Test Generation for testEqualStringArrays
// We replicate the focal test cases from testunity.c focusing on string array equality.

static void testEqualStringArrays_cpp() {
    // Reference arrays
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    // 1) expStrings vs expStrings, length 3 -> should pass
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, expStrings, 3);
    // 2) expStrings vs testStrings, length 3 -> should pass
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 3);
    // 3) expStrings vs testStrings, length 2 -> should pass
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 2);
    // 4) expStrings vs testStrings, length 1 -> should pass
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 1);
}

static void testNotEqualStringArray1_cpp() {
    // 4-element mismatch at last element zoo vs moo
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testNotEqualStringArray2_cpp() {
    // First element differs
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "moo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testNotEqualStringArray3_cpp() {
    // Fourth element NULL vs "zoo"
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testNotEqualStringArray4_cpp() {
    // Second element NULL in expected
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", NULL, "woo", "moo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testNotEqualStringArray5_cpp() {
    // Actual testStrings is NULL, expected is non-NULL
    const char **testStrings = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testNotEqualStringArray6_cpp() {
    // Expected is NULL, actual is non-NULL
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char **expStrings = NULL;
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4); // should fail
}

static void testEqualStringArrayIfBothNulls_cpp() {
    // Both arrays NULL should be considered equal
    const char **testStrings = NULL;
    const char **expStrings  = NULL;
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
}

static void testNotEqualStringArrayLengthZero_cpp() {
    // count == 0, one side non-NULL should fail (mirrors Unity behavior)
    const char *testStrings[] = { NULL };
    const char **expStrings = NULL;
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 0); // should fail
}

static void testEqualStringEachEqual_cpp() {
    // Recreate Unity's TEST_ASSERT_EACH_EQUAL_STRING scenarios
    const char *testStrings1[] = { "foo", "foo", "foo", "foo" };
    const char *testStrings2[] = { "boo", "boo", "boo", "zoo" };
    const char *testStrings3[] = { "", "", "", "" };

    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings1, 4);
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings1, 1);
    TEST_ASSERT_EACH_EQUAL_STRING("boo", testStrings2, 3);
    TEST_ASSERT_EACH_EQUAL_STRING("", testStrings3, 4);
}

static void testNotEqualStringEachEqual1_cpp() {
    const char *testStrings[] = { "foo", "foo", "foo", "moo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4); // should fail
}

static void testNotEqualStringEachEqual2_cpp() {
    const char *testStrings[] = { "boo", "foo", "foo", "foo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4); // should fail
}

static void testNotEqualStringEachEqual3_cpp() {
    const char *testStrings[] = { "foo", "foo", "foo", NULL };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4); // should fail
}

static void testNotEqualStringEachEqual4_cpp() {
    const char *testStrings[] = { "foo", "foo", "woo", "foo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4); // should fail
}

static void testNotEqualStringEachEqual5_cpp() {
    TEST_ASSERT_EACH_EQUAL_STRING("foo", NULL, 1); // should fail
}

// Step 3: Test Case Refinement - ensure coverage and realistic behavior
// Additional scenarios for robustness
static void testEqualMemory_like_behavior_not_required_for focal_strings_cpp() {
    // Intentionally left minimal; the focal method only tests string arrays.
    // This placeholder demonstrates how one could add memory-field tests if needed.
}

// Main entry
int main() {
    // Register test cases
    std::vector<TestCase> tests = {
        {"testEqualStringArrays_cpp", testEqualStringArrays_cpp},
        {"testNotEqualStringArray1_cpp", testNotEqualStringArray1_cpp},
        {"testNotEqualStringArray2_cpp", testNotEqualStringArray2_cpp},
        {"testNotEqualStringArray3_cpp", testNotEqualStringArray3_cpp},
        {"testNotEqualStringArray4_cpp", testNotEqualStringArray4_cpp},
        {"testNotEqualStringArray5_cpp", testNotEqualStringArray5_cpp},
        {"testNotEqualStringArray6_cpp", testNotEqualStringArray6_cpp},
        {"testEqualStringArrayIfBothNulls_cpp", testEqualStringArrayIfBothNulls_cpp},
        {"testNotEqualStringArrayLengthZero_cpp", testNotEqualStringArrayLengthZero_cpp},
        {"testEqualStringEachEqual_cpp", testEqualStringEachEqual_cpp},
        {"testNotEqualStringEachEqual1_cpp", testNotEqualStringEachEqual1_cpp},
        {"testNotEqualStringEachEqual2_cpp", testNotEqualStringEachEqual2_cpp},
        {"testNotEqualStringEachEqual3_cpp", testNotEqualStringEachEqual3_cpp},
        {"testNotEqualStringEachEqual4_cpp", testNotEqualStringEachEqual4_cpp},
        {"testNotEqualStringEachEqual5_cpp", testNotEqualStringEachEqual5_cpp}
    };

    runTests(tests);
    return 0;
}
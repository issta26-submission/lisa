// Lightweight C++11 unit test suite for cJSON_GetArraySize (from cJSON.c)
// - No Google Test or external test frameworks used
// - Uses a tiny, self-contained test harness with non-terminating assertions
// - Focuses on core behavior: NULL input, empty array, arrays with items, and object-like scenarios
// - Relies on the public cJSON API only
// - Explanatory comments provided for each test

#include <sstream>
#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <memory>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <functional>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Import C library cJSON
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple assertion logger (non-fatal by design; tests continue on failure)
class AssertionLogger {
public:
    static AssertionLogger& instance() {
        static AssertionLogger inst;
        return inst;
    }

    void reset() { messages.clear(); }
    void add(const std::string& m) { messages.push_back(m); }
    bool hasFailures() const { return !messages.empty(); }
    const std::vector<std::string>& get() const { return messages; }

private:
    std::vector<std::string> messages;
    AssertionLogger() {}
    ~AssertionLogger() {}
    AssertionLogger(const AssertionLogger&) = delete;
    AssertionLogger& operator=(const AssertionLogger&) = delete;
};

#define EXPECT_TRUE(cond) do { \
    bool _cond = (cond); \
    if(!_cond) { \
        std::stringstream ss; \
        ss << "EXPECT_TRUE failed: " #cond; \
        AssertionLogger::instance().add(ss.str()); \
    } \
} while(0)

#define EXPECT_EQ_INT(expected, actual) do { \
    int _exp = (expected); \
    int _act = (actual); \
    if(_exp != _act) { \
        std::stringstream ss; \
        ss << "EXPECT_EQ_INT failed: actual(" << _act << ") != expected(" << _exp << ")"; \
        AssertionLogger::instance().add(ss.str()); \
    } \
} while(0)

#define TEST_PASS(name) \
    std::cout << "[PASS] " name "\n";

#define TEST_FAIL(name) \
    std::cout << "[FAIL] " name "\n";

// Test 1: Null pointer input should yield 0
// - Verifies API handles NULL gracefully and does not crash
// - Important edge-case covered by early return in implementation
bool test_GetArraySize_NullInput_ReturnsZero() {
    AssertionLogger::instance().reset();

    int size = cJSON_GetArraySize(nullptr);

    EXPECT_EQ_INT(0, size);
    // Return true if no assertion failures were recorded
    return !AssertionLogger::instance().hasFailures();
}

// Test 2: Empty array should have size 0
// - Checks behavior for a newly created array with no elements
bool test_GetArraySize_EmptyArray_ReturnsZero() {
    AssertionLogger::instance().reset();

    cJSON *array = cJSON_CreateArray();
    int size = cJSON_GetArraySize(array);
    EXPECT_EQ_INT(0, size);

    cJSON_Delete(array);
    return !AssertionLogger::instance().hasFailures();
}

// Test 3: Array with a single item should return 1
// - Validates counting of one element
bool test_GetArraySize_ArrayWithOneItem_ReturnsOne() {
    AssertionLogger::instance().reset();

    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(42));
    int size = cJSON_GetArraySize(array);
    EXPECT_EQ_INT(1, size);

    cJSON_Delete(array);
    return !AssertionLogger::instance().hasFailures();
}

// Test 4: Array with multiple items should return correct count
// - Validates counting for several elements (0..4)
bool test_GetArraySize_ArrayWithMultipleItems_ReturnsCorrectCount() {
    AssertionLogger::instance().reset();

    const int N = 5;
    cJSON *array = cJSON_CreateArray();
    for (int i = 0; i < N; ++i) {
        cJSON_AddItemToArray(array, cJSON_CreateNumber(i));
    }
    int size = cJSON_GetArraySize(array);
    EXPECT_EQ_INT(N, size);

    cJSON_Delete(array);
    return !AssertionLogger::instance().hasFailures();
}

// Test 5: GetArraySize on an object with members should count members
// - Because cJSON uses a single linked list via 'child' for both arrays and objects,
//   this test ensures the function counts the number of children even when used on an object.
// - This is important given the implementation does not check the JSON type.
bool test_GetArraySize_OnObjectWithMembers_ReturnsMemberCount() {
    AssertionLogger::instance().reset();

    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "first", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(obj, "second", cJSON_CreateNumber(2));
    cJSON_AddItemToObject(obj, "third", cJSON_CreateNumber(3));

    int size = cJSON_GetArraySize(obj);
    EXPECT_EQ_INT(3, size);

    cJSON_Delete(obj);
    return !AssertionLogger::instance().hasFailures();
}

// Main test runner
int main() {
    // Informational header
    std::cout << "Running cJSON_GetArraySize test suite (C++11, no GTest)\n";

    int total = 0;
    int passed = 0;

    // Execute tests one by one; each test uses non-fatal assertions
    total++;
    if (test_GetArraySize_NullInput_ReturnsZero()) {
        TEST_PASS("test_GetArraySize_NullInput_ReturnsZero");
        passed++;
    } else {
        TEST_FAIL("test_GetArraySize_NullInput_ReturnsZero");
    }

    total++;
    if (test_GetArraySize_EmptyArray_ReturnsZero()) {
        TEST_PASS("test_GetArraySize_EmptyArray_ReturnsZero");
        passed++;
    } else {
        TEST_FAIL("test_GetArraySize_EmptyArray_ReturnsZero");
    }

    total++;
    if (test_GetArraySize_ArrayWithOneItem_ReturnsOne()) {
        TEST_PASS("test_GetArraySize_ArrayWithOneItem_ReturnsOne");
        passed++;
    } else {
        TEST_FAIL("test_GetArraySize_ArrayWithOneItem_ReturnsOne");
    }

    total++;
    if (test_GetArraySize_ArrayWithMultipleItems_ReturnsCorrectCount()) {
        TEST_PASS("test_GetArraySize_ArrayWithMultipleItems_ReturnsCorrectCount");
        passed++;
    } else {
        TEST_FAIL("test_GetArraySize_ArrayWithMultipleItems_ReturnsCorrectCount");
    }

    total++;
    if (test_GetArraySize_OnObjectWithMembers_ReturnsMemberCount()) {
        TEST_PASS("test_GetArraySize_OnObjectWithMembers_ReturnsMemberCount");
        passed++;
    } else {
        TEST_FAIL("test_GetArraySize_OnObjectWithMembers_ReturnsMemberCount");
    }

    // Summary
    std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}

/*
Notes on Candidate Keywords (from Step 1):
- cJSON: core JSON object representation
- array / child / next: traversal pointers used by cJSON_GetArraySize
- Null input handling: early return path
- Memory management: cJSON_CreateArray, cJSON_AddItemToArray, cJSON_Delete
- Object vs Array: GetArraySize counts number of children regardless of type (as per implementation)
- Potential overflow: not exercised due to practical limits in unit tests
*/
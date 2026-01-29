// Unit test suite for cJSON_PrintPreallocated function (C library tested from C++11).
// This test suite does not use Google Test. It provides a lightweight test harness
// with non-terminating assertions to maximize code coverage.
// The tests rely on linking against cJSON.c/.a and include cJSON.h.
// Explanatory comments are added for each test case.

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


extern "C" {
}

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility: report a failed assertion without terminating tests
static void report_failure(const char* test_name, const char* message, const char* file, int line) {
    std::cerr << "[TEST FAILURE] " << test_name << " - " << message
              << " (" << file << ":" << line << ")\n";
    ++g_failed_tests;
}

// Macros for non-terminating assertions
#define ASSERT_TRUE(cond, test_name, message) do { \
    ++g_total_tests; \
    if(!(cond)) { report_failure(test_name, message, __FILE__, __LINE__); } \
} while(0)

#define ASSERT_STR_EQ(expected, actual, test_name, message) do { \
    ++g_total_tests; \
    if(std::strcmp((expected), (actual)) != 0) { \
        report_failure(test_name, message, __FILE__, __LINE__); \
    } \
} while(0)

#define ASSERT_SUBSTR_EXIST(substr, str, test_name, message) do { \
    ++g_total_tests; \
    if((str).find(substr) == std::string::npos) { \
        report_failure(test_name, message, __FILE__, __LINE__); \
    } \
} while(0)


// Test 1: Negative length should return false (tests the first branch of the predicate)
bool test_PrintPreallocated_NegativeLength() {
    const char* test_name = "test_PrintPreallocated_NegativeLength";

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        report_failure(test_name, "Failed to create cJSON object", __FILE__, __LINE__);
        return false;
    }

    char buffer[256] = {0};
    int negative_length = -1;
    cJSON_bool result = cJSON_PrintPreallocated(root, buffer, negative_length, /*format*/false);

    // Expect false due to negative length
    ASSERT_TRUE(result == false, test_name, "Expected false when length < 0");

    cJSON_Delete(root);
    return (g_failed_tests == 0); // return true if no failure so far in this test
}

// Test 2: NULL buffer should return false (tests the second branch of the predicate)
bool test_PrintPreallocated_NullBuffer() {
    const char* test_name = "test_PrintPreallocated_NullBuffer";

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        report_failure(test_name, "Failed to create cJSON object", __FILE__, __LINE__);
        return false;
    }

    int length = 32;
    cJSON_bool result = cJSON_PrintPreallocated(root, /*buffer*/ NULL, length, /*format*/false);

    // Expect false due to NULL buffer
    ASSERT_TRUE(result == false, test_name, "Expected false when buffer is NULL");

    cJSON_Delete(root);
    return (g_failed_tests == 0);
}

// Test 3: Unformatted print should produce deterministic compact JSON string
bool test_PrintPreallocated_UnformattedProducesDeterministicOutput() {
    const char* test_name = "test_PrintPreallocated_UnformattedProducesDeterministicOutput";

    // Build a small object: {"name":"ChatGPT","version":3.5}
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        report_failure(test_name, "Failed to create cJSON object", __FILE__, __LINE__);
        return false;
    }
    cJSON_AddStringToObject(root, "name", "ChatGPT");
    cJSON_AddNumberToObject(root, "version", 3.5);

    // Expected string via standard (unformatted) print
    char* expected = cJSON_Print(root);
    if (!expected) {
        report_failure(test_name, "Failed to generate expected string via cJSON_Print", __FILE__, __LINE__);
        cJSON_Delete(root);
        return false;
    }
    std::string expected_str(expected);
    cJSON_free(expected);

    // Now print to preallocated buffer
    char buffer[256];
    cJSON_bool result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), /*format*/false);

    // Validate result and content
    ASSERT_TRUE(result == true, test_name, "Expected true for valid preallocated print");
    if (result == true) {
        ASSERT_STR_EQ(expected_str.c_str(), buffer, test_name, "Buffer content should match compact JSON");
    }

    cJSON_Delete(root);
    return (g_failed_tests == 0);
}

// Test 4: Formatted (pretty) print should include quotes and version info; verify presence
bool test_PrintPreallocated_FormattedContainsExpectedFragments() {
    const char* test_name = "test_PrintPreallocated_FormattedContainsExpectedFragments";

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        report_failure(test_name, "Failed to create cJSON object", __FILE__, __LINE__);
        return false;
    }

    cJSON_AddStringToObject(root, "name", "ChatGPT");
    cJSON_AddNumberToObject(root, "version", 3.5);

    // Print with formatting enabled
    char buffer[1024];
    cJSON_bool result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), /*format*/true);

    ASSERT_TRUE(result == true, test_name, "Expected true for valid preallocated pretty print");

    if (result == true) {
        std::string s(buffer);
        // Check presence of key substrings without enforcing exact formatting
        ASSERT_SUBSTR_EXIST("\"name\"", s, test_name, "Pretty JSON should contain the \"name\" key");
        ASSERT_SUBSTR_EXIST("\"ChatGPT\"", s, test_name, "Pretty JSON should contain the value ChatGPT");
        ASSERT_SUBSTR_EXIST("\"version\"", s, test_name, "Pretty JSON should contain the \"version\" key");
        ASSERT_SUBSTR_EXIST("3.5", s, test_name, "Pretty JSON should contain the value 3.5");
        // Basic structural checks
        ASSERT_TRUE(!s.empty() && s.front() == '{' && s.back() != '\0', test_name, "Pretty JSON should start with '{' and end with a character");
    }

    cJSON_Delete(root);
    return (g_failed_tests == 0);
}

// Test 5: Length exactly equal to the required content length (including NUL)
bool test_PrintPreallocated_LengthExactlyFits() {
    const char* test_name = "test_PrintPreallocated_LengthExactlyFits";

    // Build an object to predict its exact unformatted length
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        report_failure(test_name, "Failed to create cJSON object", __FILE__, __LINE__);
        return false;
    }
    cJSON_AddStringToObject(root, "name", "ChatGPT");
    cJSON_AddNumberToObject(root, "version", 3.5);

    // Generate expected string via standard print to know exact length
    char* expected = cJSON_Print(root);
    if (!expected) {
        report_failure(test_name, "Failed to generate expected string via cJSON_Print", __FILE__, __LINE__);
        cJSON_Delete(root);
        return false;
    }
    std::string expected_str(expected);
    cJSON_free(expected);

    // Include space for the terminating NUL
    size_t required_length = expected_str.size() + 1;
    std::vector<char> buffer(required_length);

    cJSON_bool result = cJSON_PrintPreallocated(root, buffer.data(), (int)required_length, /*format*/false);
    ASSERT_TRUE(result == true, test_name, "Expected true when buffer length exactly fits the content");

    if (result == true) {
        std::string produced(buffer.data(), expected_str.size()); // Compare only actual characters
        ASSERT_STR_EQ(expected_str.c_str(), produced.c_str(), test_name, "Output should exactly match the expected compact JSON when length exactly fits");
    }

    cJSON_Delete(root);
    return (g_failed_tests == 0);
}


// Main: run all tests and report summary
int main() {
    // Run tests. Each test uses non-terminating assertions; we report failures but continue.
    bool t1 = test_PrintPreallocated_NegativeLength();
    bool t2 = test_PrintPreallocated_NullBuffer();
    bool t3 = test_PrintPreallocated_UnformattedProducesDeterministicOutput();
    bool t4 = test_PrintPreallocated_FormattedContainsExpectedFragments();
    bool t5 = test_PrintPreallocated_LengthExactlyFits();

    int total = 5;
    // We can't reliably count per-test successes from return values if tests fail in some sub-assertions;
    // Here we assume return value reflects overall test health.
    int passed = 0;
    if (t1) ++passed;
    if (t2) ++passed;
    if (t3) ++passed;
    if (t4) ++passed;
    if (t5) ++passed;

    std::cout << "Test suite: " << passed << " / " << total << " tests passed.\n";
    if (g_failed_tests > 0) {
        std::cerr << g_failed_tests << " failure(s) detected during tests.\n";
        return 1;
    }
    return 0;
}
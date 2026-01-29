// Test suite for the focal C function: cJSON_Version
// - No Google Test is used. A small in-house test harness is provided.
// - The tests focus on verifying correctness, stability, and boundaries
//   of the cJSON_Version() function as described in the problem statement.
//
// Notes aligned with Domain Knowledge:
// - Access the version components via macros: CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH
// - The function returns a pointer to a static buffer; verify pointer stability across calls.
// - Validate the "major.minor.patch" format and that the parsed numbers match the macros.
// - Ensure the string fits within the internal 15-byte buffer (static char version[15]).

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Include the cJSON header to access version macros

#ifdef __cplusplus
extern "C" {
#endif
// Function under test declared with C linkage
const char* cJSON_Version(void);
#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_CHECK(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << msg << std::endl; \
    } \
} while(0)


// Test 1: Basic format and macro consistency
// - Parse the version string returned by cJSON_Version() using sscanf.
// - Compare parsed major/minor/patch against CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH.
static void test_version_format_and_macros() {
    const char* ver = cJSON_Version();
    TEST_CHECK(ver != nullptr, "cJSON_Version() returned null");
    int maj = 0, min = 0, patch = 0;
    int parsed = std::sscanf(ver, "%d.%d.%d", &maj, &min, &patch);
    TEST_CHECK(parsed == 3, "Version string should have format 'major.minor.patch' with three integers");
    TEST_CHECK(maj == CJSON_VERSION_MAJOR, "Major version should match CJSON_VERSION_MAJOR");
    TEST_CHECK(min == CJSON_VERSION_MINOR, "Minor version should match CJSON_VERSION_MINOR");
    TEST_CHECK(patch == CJSON_VERSION_PATCH, "Patch version should match CJSON_VERSION_PATCH");
}

// Test 2: Pointer stability across calls
// - The function uses a static buffer inside the function scope; the pointer should remain the same across calls.
static void test_pointer_stability() {
    const char* v1 = cJSON_Version();
    const char* v2 = cJSON_Version();
    TEST_CHECK(v1 != nullptr, "First version call should not return null");
    TEST_CHECK(v2 != nullptr, "Second version call should not return null");
    TEST_CHECK(v1 == v2, "cJSON_Version() should return the same pointer across consecutive calls");
}

// Test 3: Content stability after multiple invocations
// - Repeatedly calling cJSON_Version should not mutate the content of the returned string.
static void test_content_stability() {
    const char* v1 = cJSON_Version();
    // Perform several additional calls
    for (int i = 0; i < 5; ++i) {
        (void)cJSON_Version();
    }
    const char* v2 = cJSON_Version();
    TEST_CHECK(v1 != nullptr && v2 != nullptr, "Version pointers should be non-null");
    TEST_CHECK(std::strcmp(v1, v2) == 0, "Version string content should remain stable across calls");
}

// Test 4: Buffer length bound
// - The internal buffer is 15 bytes; ensure the string length is strictly less than 15.
static void test_buffer_length_bound() {
    const char* ver = cJSON_Version();
    TEST_CHECK(ver != nullptr, "Version string should be non-null");
    size_t len = std::strlen(ver);
    TEST_CHECK(len < 15, "Version string length must fit within the 15-byte internal buffer");
}

// Test 5: Version string format integrity (two dots present)
// - Verify that exactly two dot separators exist to form three numeric components.
static void test_dot_count() {
    const char* ver = cJSON_Version();
    TEST_CHECK(ver != nullptr, "Version string should be non-null");
    int dot_count = 0;
    for (const char* p = ver; *p != '\0'; ++p) {
        if (*p == '.') ++dot_count;
    }
    TEST_CHECK(dot_count == 2, "Version string should contain exactly two dots (major.minor.patch)");
}

// Test 6: Ensure non-null terminator after length
// - Sanity check that the string is properly null-terminated at strlen(ver).
static void test_null_terminator_after_length() {
    const char* ver = cJSON_Version();
    TEST_CHECK(ver != nullptr, "Version string should be non-null");
    size_t len = std::strlen(ver);
    TEST_CHECK(ver[len] == '\0', "Version string should be null-terminated after strlen length");
}

// Runner to execute all tests
static void run_all_tests() {
    std::cout << "Starting cJSON_Version unit tests (C++11)..." << std::endl;

    test_version_format_and_macros();
    test_pointer_stability();
    test_content_stability();
    test_buffer_length_bound();
    test_dot_count();
    test_null_terminator_after_length();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;
}

// Entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed to indicate test suite result
    return g_failed_tests;
}
// Minimal C++11 unit tests for AllocString in cmscgats.c without using GTest.
// This test suite uses a lightweight, non-terminating assertion approach to
// maximize code coverage while allowing tests to continue after failures.

// Notes about the tested function:
// - AllocString(it8, str) returns NULL if str is NULL.
// - It computes Size = strlen(str) + 1 and calls AllocChunk(it8, Size).
// - If allocation succeeds, it copies 'Size-1' bytes from str into the allocated
//   buffer. Therefore, the resulting buffer is not guaranteed to be NUL-terminated
//   (only the first strlen(str) bytes contain the original string data).

#include <lcms2_internal.h>
#include <string>
#include <cstdio>
#include <cstring>


// Forward declare the cmsIT8 type and the AllocString function with C linkage,
// assuming cmscgats.c is compiled as C and linked with this C++ test.
extern "C" {
    struct cmsIT8; // incomplete type, enough for pointer usage in tests
    char* AllocString(struct cmsIT8* it8, const char* str);
}

// Lightweight test harness (non-terminating assertions)
static int gTests = 0;
static int gFailures = 0;

// Simple failure logger
static void log_failure(const char* file, int line, const char* msg) {
    std::fprintf(stderr, "Failure in %s:%d: %s\n", file, line, msg);
}

// Assert that a condition is true; continue on failure
#define EXPECT_TRUE(cond) \
    do { \
        ++gTests; \
        if (!(cond)) { \
            ++gFailures; \
            log_failure(__FILE__, __LINE__, "EXPECTED TRUE FAILED: " #cond); \
        } \
    } while (0)

// Assert that a condition is false; continue on failure
#define EXPECT_FALSE(cond) \
    do { \
        ++gTests; \
        if (cond) { \
            ++gFailures; \
            log_failure(__FILE__, __LINE__, "EXPECTED FALSE FAILED: " #cond); \
        } \
    } while (0)

// Helper: compare two std::string values and log if different
static void assert_str_eq(const std::string& actual, const std::string& expected, const char* file, int line) {
    ++gTests;
    if (actual != expected) {
        ++gFailures;
        std::fprintf(stderr, "Test %s:%d: string mismatch. Expected '%s', got '%s'\n",
                     file, line, expected.c_str(), actual.c_str());
    }
}

int main() {
    // Candidate Keywords / Core dependencies observed:
    // - it8: cmsIT8 context object (passed through to memory allocator)
    // - AllocChunk: underlying allocator used by AllocString
    // - str: input C-string to copy into allocated buffer
    // - Size: total allocation size (strlen(str) + 1)
    // - memcpy: copy only Size-1 bytes (i.e., the string content, without a terminator)

    // Prepare a dummy it8 context (pointer value is what AllocString will forward to AllocChunk)
    struct cmsIT8 dummyContext;

    // Test 1: NULL input should return NULL and not attempt allocation
    {
        char* result = AllocString(&dummyContext, NULL);
        EXPECT_TRUE(result == NULL);
        // No memory to free in this test case (allocation should not occur)
    }

    // Test 2: Non-empty string should allocate memory and copy exactly strlen(str) bytes
    // Verify that the first N bytes equal the original string, where N = strlen(input)
    {
        const char* input = "HelloWorld"; // 10 characters
        char* result = AllocString(&dummyContext, input);
        EXPECT_TRUE(result != NULL);

        if (result != NULL) {
            // Compare the first strlen(input) bytes with the input string
            size_t n = std::strlen(input);
            std::string actual(result, n); // construct string from first n bytes
            std::string expected(input, n);
            assert_str_eq(actual, expected, __FILE__, __LINE__);
        }
        // We do not free result here to avoid potential mismatches with AllocChunk's
        // memory management in the test environment. The program will exit shortly.
    }

    // Test 3: Another non-empty string to exercise different lengths (edge case)
    {
        const char* input = "ABCDEF"; // 6 characters
        char* result = AllocString(&dummyContext, input);
        EXPECT_TRUE(result != NULL);
        if (result != NULL) {
            size_t n = std::strlen(input);
            std::string actual(result, n);
            std::string expected(input, n);
            assert_str_eq(actual, expected, __FILE__, __LINE__);
        }
    }

    // Summary
    std::printf("AllocString tests completed: %d tests, %d failures.\n", gTests, gFailures);

    // Return non-zero if any test failed to indicate failure status to test harnesses
    return (gFailures == 0) ? 0 : 1;
}
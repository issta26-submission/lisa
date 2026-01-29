// Minimal C++11 unit tests for sqlite3_snprintf (no GoogleTest, no GMock).
// This test suite is designed to be compiled together with the provided
// FOCAL_METHOD (sqlite3_snprintf) and its dependencies.
// The tests use a lightweight, non-terminating assertion approach and
// report results from main().
//
// Key notes:
// - Tests focus on core dependent behavior extracted from the focal method:
//   - Return value is zBuf
//   - Early return when n <= 0
//   - Basic formatting with va_list flow (via sqlite3_snprintf)
//   - Buffer overflow/ truncation behavior is exercised
// - API_ARMOR branch is not explicitly toggled here; tests cover normal usage.
// - Static/internal helpers from the real implementation are not accessed directly.

#include <string>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Declaration of the focal function (C linkage since it's a C API)
extern "C" char *sqlite3_snprintf(int n, char *zBuf, const char *zFormat, ...);

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

// Simple assertion helpers
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        printf("ASSERT FAILED: %s\n", msg); \
        ++g_failures; \
    } else { \
        printf("ASSERT PASSED: %s\n", msg); \
    } \
} while(0)

#define ASSERT_STREQ(expected, actual, msg) do { \
    if(std::strcmp((expected), (actual)) != 0) { \
        printf("ASSERT STRNEQ FAILED: %s (expected \"%s\", got \"%s\")\n", msg, (expected), (actual)); \
        ++g_failures; \
    } else { \
        printf("ASSERT STR_EQ PASSED: %s\n", msg); \
    } \
} while(0)

#define ASSERT_PTR_EQ(expected, actual, msg) do { \
    if((expected) != (actual)) { \
        printf("ASSERT PTR FAILED: %s (expected %p, got %p)\n", msg, (void*)(expected), (void*)(actual)); \
        ++g_failures; \
    } else { \
        printf("ASSERT PTR PASSED: %s\n", msg); \
    } \
} while(0)

#define ASSERT_INT_EQ(expected, actual, msg) do { \
    if((long)(expected) != (long)(actual)) { \
        printf("ASSERT INT FAILED: %s (expected %ld, got %ld)\n", msg, (long)(expected), (long)(actual)); \
        ++g_failures; \
    } else { \
        printf("ASSERT INT PASSED: %s\n", msg); \
    } \
} while(0)


// Test 1: n <= 0 should return zBuf unchanged
// Purpose: Validate early return path when buffer size is non-positive.
static void test_n_nonpositive() {
    const char* testName = "n <= 0 returns zBuf unchanged";

    char buf[32];
    std::strcpy(buf, "XYZ"); // initial content to verify unchanged

    char* ret = sqlite3_snprintf(0, buf, "format %d", 999);

    // Expect return pointer equal to input buffer
    ASSERT_PTR_EQ(buf, ret, std::string(testName) + " (return value is zBuf)");

    // Expect content unchanged
    ASSERT_STREQ("XYZ", buf, std::string(testName) + " (buf unchanged)");
}

// Test 2: Basic formatting with normal parameters
// Purpose: Ensure typical usage writes correctly formatted string into zBuf
static void test_basic_format() {
    const char* testName = "basic formatting writes expected string";

    char buf[64] = {0};
    char* ret = sqlite3_snprintf(64, buf, "Hello %s %d", "World", 123);

    ASSERT_PTR_EQ(buf, ret, std::string(testName) + " (return value is zBuf)");
    ASSERT_STREQ("Hello World 123", buf, std::string(testName) + " (content matches)");
}

// Test 3: Overflow/truncation behavior
// Purpose: Ensure when n is small, output is truncated and terminated properly
static void test_overflow_truncation() {
    const char* testName = "overflow truncation writes at most n-1 chars";

    char buf[5]; // n = 5 => up to 4 characters + terminator
    char* ret = sqlite3_snprintf(5, buf, "abcdef"); // 6 chars, no placeholders

    // Expect return pointer equal to input buffer
    ASSERT_PTR_EQ(buf, ret, std::string(testName) + " (return value is zBuf)");

    // Expect truncation to 4 chars + terminator -> "abcd"
    ASSERT_STREQ("abcd", buf, std::string(testName) + " (truncated content)");
}

// Test 4: Empty format string
// Purpose: Ensure empty format yields empty string and terminator is placed
static void test_empty_format() {
    const char* testName = "empty format yields empty string";

    char buf[16] = {0};
    // Use non-zero n to ensure path executes and writes terminator
    char* ret = sqlite3_snprintf(16, buf, "");

    ASSERT_PTR_EQ(buf, ret, std::string(testName) + " (return value is zBuf)");
    ASSERT_STREQ("", buf, std::string(testName) + " (empty string wrote terminator)");
}

// Test 5: Exact capacity handling (n == 1 should yield empty string)
static void test_exact_capacity() {
    const char* testName = "n == 1 yields empty string (only terminator)";

    char buf[2] = {0};
    char* ret = sqlite3_snprintf(1, buf, "abc"); // no room for any chars

    ASSERT_PTR_EQ(buf, ret, std::string(testName) + " (return value is zBuf)");
    ASSERT_STREQ("", buf, std::string(testName) + " (terminator only)");
}

// Entry point: run all tests and report summary
int main() {
    printf("sqlite3_snprintf unit test suite (C++11, no GTest)\n");

    test_n_nonpositive();
    test_basic_format();
    test_overflow_truncation();
    test_empty_format();
    test_exact_capacity();

    if(g_failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILURES: %d\n", g_failures);
    }

    return g_failures ? 1 : 0;
}
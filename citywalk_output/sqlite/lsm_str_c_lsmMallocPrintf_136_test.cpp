// This test suite targets the focal method: lsmMallocPrintf
// It exercises typical formatting paths and verifies memory content.
// The tests are written in C++11 without any external test framework (GTest).
// Explanatory comments accompany each unit test case.

// NOTE: The test relies on the real implementations of lsmStringInit,
// lsmStringVAppendf, lsmReallocOrFree, etc., being available in the linked project.
// The test uses a minimal C-linkage interface to call the focal C function.

#include <lsmInt.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Bring in the C declarations required to call lsmMallocPrintf.
extern "C" {
char *lsmMallocPrintf(lsm_env *pEnv, const char *zFormat, ...);
}

// Lightweight non-terminating test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

static void testReportFailure(const std::string &msg) {
    std::cerr << "[TEST FAILED] " << msg << std::endl;
    ++g_failedTests;
}

#define EXPECT_TRUE(cond, msg) do { \
    ++g_totalTests; \
    if(!(cond)) { testReportFailure(msg); } \
} while(0)

#define EXPECT_STR_EQ(actual, expected, msg) do { \
    ++g_totalTests; \
    if((actual) == nullptr) { \
        testReportFailure(msg + " | got null pointer, expected: \"" + std::string(expected) + "\""); \
    } else if(std::strcmp(actual, expected) != 0) { \
        testReportFailure(msg + " | Expected: \"" + std::string(expected) + \
                          "\"; Got: \"" + std::string(actual) + "\""); \
    } \
} while(0)

// Test 1: Basic formatting with no additional arguments
// Purpose: Validate that a simple string is produced exactly as formatted.
static void test_basic_no_args() {
    // The focal function should format "Hello" and return a pointer to a memory region
    // containing "Hello" followed by a null terminator.
    char *p = lsmMallocPrintf(nullptr, "Hello");
    EXPECT_TRUE(p != nullptr, "lsmMallocPrintf(nullptr, \"Hello\") should return non-null");
    if(p) {
        EXPECT_STR_EQ(p, "Hello", "Basic formatting should yield 'Hello'");
        free(p); // Best-effort cleanup; memory is allocated by the library using malloc-like semantics
    }
}

// Test 2: Formatting with a single integer placeholder
// Purpose: Ensure correct insertion of integer values into the formatted string.
static void test_with_integer() {
    char *p = lsmMallocPrintf(nullptr, "Value=%d", 42);
    EXPECT_TRUE(p != nullptr, "lsmMallocPrintf(nullptr, \"Value=%d\", 42) should return non-null");
    if(p) {
        EXPECT_STR_EQ(p, "Value=42", "Integer formatting should yield 'Value=42'");
        free(p);
    }
}

// Test 3: Formatting with multiple placeholders (integer and string)
// Purpose: Validate proper handling of multiple va_list arguments through the double va_start path.
static void test_multiple_placeholders() {
    char *p = lsmMallocPrintf(nullptr, "A=%d, B=%s", 7, "foo");
    EXPECT_TRUE(p != nullptr, "lsmMallocPrintf(nullptr, \"A=%d, B=%s\", 7, \"foo\") should return non-null");
    if(p) {
        EXPECT_STR_EQ(p, "A=7, B=foo", "Multiple placeholders should yield 'A=7, B=foo'");
        free(p);
    }
}

// Test 4: Escaped percent sign in format string
// Purpose: Ensure that literal '%' characters are handled correctly (printf behavior: '%%' -> '%').
static void test_escaped_percent() {
    char *p = lsmMallocPrintf(nullptr, "Percent %%d");
    EXPECT_TRUE(p != nullptr, "lsmMallocPrintf(nullptr, \"Percent %%d\") should return non-null");
    if(p) {
        EXPECT_STR_EQ(p, "Percent %d", "Escaped percent should yield 'Percent %d'");
        free(p);
    }
}

// Test 5: Long string argument to verify dynamic memory growth
// Purpose: Check that the function can handle longer formatted outputs without truncation.
static void test_long_string_argument() {
    std::string longArg(50, 'x'); // 50-character string of 'x'
    char *p = lsmMallocPrintf(nullptr, "Long=%s", longArg.c_str());
    EXPECT_TRUE(p != nullptr, "lsmMallocPrintf(nullptr, \"Long=%s\", longArg) should return non-null");
    if(p) {
        std::string expected = "Long=" + longArg;
        EXPECT_STR_EQ(p, expected.c_str(), "Long string argument should be preserved in output");
        free(p);
    }
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Starting tests for lsmMallocPrintf...\n";

    test_basic_no_args();
    test_with_integer();
    test_multiple_placeholders();
    test_escaped_percent();
    test_long_string_argument();

    std::cout << "Test run complete. Total tests: " << g_totalTests
              << ", Failures: " << g_failedTests << std::endl;

    // Return non-zero if any test failed, to signal issues in CI.
    return (g_failedTests == 0) ? 0 : 1;
}
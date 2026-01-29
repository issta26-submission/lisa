// test_subtest_suite.cpp
// A C++11 test suite for the focal function SubTest defined in testcms2.c
// This suite does not rely on GTest; it uses a lightweight in-house assertion framework.
// It exercises various variadic calls to SubTest and validates the content written
// to the global SubTestBuffer via its extern symbol.

// Note: This test assumes SubTestBuffer is a global char array defined in testcms2.c
// and that SubTest has C linkage. We declare them here accordingly.

#include <cstring>
#include <iostream>
#include <testcms2.h>


// Declarations for the focal function under test and its shared buffer.
// These are expected to be provided by the test CMS22 framework (testcms2.c).
extern "C" void SubTest(const char* frm, ...);
extern "C" char SubTestBuffer[];

static int g_totalTests = 0;
static int g_failedTests = 0;

// Lightweight assertion macro: compares SubTestBuffer content with expected string.
// It intentionally does not abort on failure to maximize test coverage.
#define EXPECT_EQ_STR(expected) do {                                  \
    ++g_totalTests;                                                     \
    if (std::strcmp(SubTestBuffer, (expected)) != 0) {                \
        std::cerr << "FAIL: " __FILE__ ":" << __LINE__             \
                  << "  In " << __func__                     \
                  << ", expected \"" << (expected)              \
                  << "\" but got \"" << SubTestBuffer << "\"\n";  \
        ++g_failedTests;                                              \
    }                                                                 \
} while (0)

// Test 1: Basic string without any format specifiers.
// Purpose: Ensure SubTest correctly copies a plain string into SubTestBuffer.
static void test_SubTest_BasicString() {
    // Explanation: No variadic arguments are provided.
    SubTest("HelloWorld");
    EXPECT_EQ_STR("HelloWorld");
}

// Test 2: Integer formatting.
// Purpose: Verify that an integer argument is formatted correctly.
static void test_SubTest_IntFormatting() {
    SubTest("Value=%d", 42);
    EXPECT_EQ_STR("Value=42");
}

// Test 3: String formatting with %s.
// Purpose: Verify that a string argument is formatted correctly.
static void test_SubTest_StringFormatting() {
    SubTest("Name=%s", "Alice");
    EXPECT_EQ_STR("Name=Alice");
}

// Test 4: Floating point formatting with a precision specifier.
// Purpose: Ensure double argument is formatted properly (uses default %f behavior).
static void test_SubTest_FloatFormatting() {
    SubTest("Pi=%.2f", 3.14159);
    EXPECT_EQ_STR("Pi=3.14");
}

// Test 5: Mixed formatting with multiple specifiers, including integer, string, and float.
// Purpose: Validate handling of multiple specifiers in a single format string.
static void test_SubTest_MixedFormatting() {
    SubTest("Mixed=%d-%s-%.1f", 7, "ok", 2.5);
    EXPECT_EQ_STR("Mixed=7-ok-2.5");
}

// Test 6: Literal percent sign using %%.
// Purpose: Ensure that literal % is produced correctly without consuming an argument.
static void test_SubTest_PercentLiteral() {
    SubTest("Percent=%%");
    EXPECT_EQ_STR("Percent=%");
}

// Test 7: Multiple specifiers with default %f formatting (no precision specifier).
// Purpose: Confirm default 6 decimal places are used for floating point values.
static void test_SubTest_MultiSpecifiers_DefaultFloat() {
    SubTest("A=%d B=%s C=%f", 1, "two", 3.0);
    EXPECT_EQ_STR("A=1 B=two C=3.000000");
}

// Run all tests and summarize results.
int main() {
    // Run individual tests
    test_SubTest_BasicString();
    test_SubTest_IntFormatting();
    test_SubTest_StringFormatting();
    test_SubTest_FloatFormatting();
    test_SubTest_MixedFormatting();
    test_SubTest_PercentLiteral();
    test_SubTest_MultiSpecifiers_DefaultFloat();

    // Summary
    std::cout << "SubTest test suite completed. "
              << "Passed: " << (g_totalTests - g_failedTests)
              << " / " << g_totalTests << " tests.\n";

    // Return non-zero if any test failed
    return (g_failedTests == 0) ? 0 : 1;
}
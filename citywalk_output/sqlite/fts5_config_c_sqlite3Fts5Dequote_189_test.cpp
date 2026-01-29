// This test suite targets the focal method sqlite3Fts5Dequote found in fts5_config.c.
// It exercises the dequoting behavior by providing strings that either do or do not begin
// with a quote-like character. The static helper fts5Dequote is invoked internally by
// sqlite3Fts5Dequote when the first character is '[', ''', '"', or '`'. Since static
// helpers are internal to the file, this test only relies on the observable effect: the
// input string should be dequoted when started with a quoting character, otherwise it should
// remain unchanged.
//
// Candidate Keywords (Step 1): fts5Dequote, sqlite3Fts5Dequote, fts5_iswhitespace, quotes,
// dequote behavior, bracket quoting, single quote, double quote, backtick, observable state

#include <iostream>
#include <string>
#include <sstream>
#include <fts5Int.h>


// Step 2: Declare focal function from the C file under test.
// The function is defined in fts5_config.c and exposed with C linkage.
extern "C" void sqlite3Fts5Dequote(char *z);

// Global test counters
static int g_testsRun = 0;
static int g_testsFail = 0;

// Helper: convert any value that can be streamed to string
template <typename T>
static std::string to_string_safe(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Report a non-terminating failure (does not abort the test run)
static void reportFailure(const char* testName, int line, const std::string& msg) {
    std::cerr << "[FAIL] " << testName << ":" << line << " - " << msg << "\n";
    ++g_testsFail;
}

// EXPECT macros that do not terminate the test run
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { reportFailure(__FUNCTION__, __LINE__, "EXPECT_TRUE failed: " #cond); } } while(0)

#define EXPECT_EQ(expected, actual) \
    do { auto _e = to_string_safe((expected)); auto _a = to_string_safe((actual)); \
         if(_e != _a) { std::ostringstream oss; oss << "EXPECT_EQ failed: " << _e << " != " << _a; \
         reportFailure(__FUNCTION__, __LINE__, oss.str()); } } while(0)

// Test 1: When the first character is not a quoting character, the input should remain unchanged.
static void test_no_change_when_no_quote() {
    ++g_testsRun;
    // First character 'b' is not a whitespace, not a quote. Expect no dequoting performed.
    char z[8] = "bar";
    sqlite3Fts5Dequote(z);
    std::string out(z);
    EXPECT_EQ(std::string("bar"), out);
}

// Test 2: Double-quote wrapping should be removed by the dequote operation.
static void test_dequote_double_quotes() {
    ++g_testsRun;
    char z[8] = "\"foo\""; // "foo" wrapped in double quotes
    sqlite3Fts5Dequote(z);
    std::string out(z);
    EXPECT_EQ(std::string("foo"), out);
}

// Test 3: Single-quote wrapping should be removed by the dequote operation.
static void test_dequote_single_quotes() {
    ++g_testsRun;
    char z[8] = "'bar'"; // 'bar' wrapped in single quotes
    sqlite3Fts5Dequote(z);
    std::string out(z);
    EXPECT_EQ(std::string("bar"), out);
}

// Test 4: Bracket quoting should be removed by the dequote operation.
static void test_dequote_brackets() {
    ++g_testsRun;
    char z[8] = "[baz]"; // [baz] wrapped in brackets
    sqlite3Fts5Dequote(z);
    std::string out(z);
    EXPECT_EQ(std::string("baz"), out);
}

// Test 5: Backtick quoting should be removed by the dequote operation.
static void test_dequote_backtick() {
    ++g_testsRun;
    char z[8] = "`qux`"; // `qux` wrapped in backticks
    sqlite3Fts5Dequote(z);
    std::string out(z);
    EXPECT_EQ(std::string("qux"), out);
}

// Main: Run all tests and report summary (non-terminating assertions ensure full run)
int main() {
    test_no_change_when_no_quote();
    test_dequote_double_quotes();
    test_dequote_single_quotes();
    test_dequote_brackets();
    test_dequote_backtick();

    std::cout << "Tests run: " << g_testsRun << ", Failures: " << g_testsFail << std::endl;
    // Non-zero return code if any test failed
    return (g_testsFail == 0) ? 0 : 1;
}
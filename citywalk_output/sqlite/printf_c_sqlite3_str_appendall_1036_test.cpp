// Test suite for sqlite3_str_appendall (wrapper around sqlite3_str_append with length from sqlite3Strlen30)
// This test assumes the focal function sqlite3_str_appendall is provided by the project under test (printf.c)
// and that the dependencies sqlite3_str_append and sqlite3Strlen30 can be provided here for linking.
// The tests are written in C++11 and do not rely on any external testing framework (GTest not used).

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Forward declaration of the sqlite3_str type as used by the focal code.
// We provide a local definition to serve as an opaque handle for test purposes.
// The actual project may have a different internal layout; since sqlite3_str_appendall
// only passes this pointer to sqlite3_str_append, this lightweight definition suffices for testing.
struct sqlite3_str;

// Provide C linkage for the dependency functions so the linker can resolve symbols
// from the focal C file (printf.c) that calls these functions.
extern "C" {

// Minimal sqlite3_str structure used by the test to accumulate appended text.
struct sqlite3_str {
    std::string content;
};

// Prototype of the function under test (should be provided by the project under test).
void sqlite3_str_appendall(sqlite3_str *p, const char *z);

// Dependency: computes the length of z, up to a maximum of 30 characters.
// In the real SQLite code, sqlite3Strlen30(z) returns at most 30.
// We implement a compatible stub here for tests.
int sqlite3Strlen30(const char *z);

// Dependency: appends N characters from z to the accumulator p.
void sqlite3_str_append(sqlite3_str *p, const char *z, int N);
} // extern "C"

// Provide the test implementations of the dependencies.
// Ensure these symbols are linked with the focal method during test linking.

extern "C" int sqlite3Strlen30(const char *z) {
    // Simulate SQLite's 30-character limit for the string length function
    int n = 0;
    while (n < 30 && z[n] != '\0') ++n;
    return n;
}

extern "C" void sqlite3_str_append(sqlite3_str *p, const char *z, int N) {
    // Append exactly N characters from z into the accumulator's content
    if (p == nullptr) return;
    if (z == nullptr) return;
    p->content.append(z, N);
}

// The focal method is expected to be provided by the project (printf.c):
// void sqlite3_str_appendall(sqlite3_str *p, const char *z);
// We declare it here to call it from tests (linker must resolve to the real implementation).
extern "C" void sqlite3_str_appendall(sqlite3_str *p, const char *z);

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "Test failure: " << (msg) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while (0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            ++g_failed_tests; \
            std::cerr << "Test failure: " << (msg) \
                      << " Expected: " << (b) << " Actual: " << (a) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while (0)

// Test 1: Append empty string to an empty accumulator
// Expected: no change to the accumulator content
void test_appendall_empty_to_empty() {
    ++g_total_tests;
    sqlite3_str p;
    p.content.clear();
    sqlite3_str_appendall(&p, "");
    ASSERT_EQ(p.content, std::string(""), "Appending empty string to empty accumulator should remain empty");
}

// Test 2: Append a short string that fits within 30 characters
void test_appendall_short_string() {
    ++g_total_tests;
    sqlite3_str p;
    p.content.clear();
    sqlite3_str_appendall(&p, "hello");
    ASSERT_EQ(p.content, std::string("hello"), "Appending 'hello' should result in 'hello'");
}

// Test 3: Append a long string longer than 30 characters
// Only the first 30 characters should be appended due to sqlite3Strlen30 behavior
void test_appendall_long_string_limits_30() {
    ++g_total_tests;
    sqlite3_str p;
    p.content.clear();
    // Create a string longer than 30 characters
    std::string longStr(60, 'A'); // 60 'A's
    sqlite3_str_appendall(&p, longStr.c_str());
    // Expect exactly 30 'A's appended
    ASSERT_EQ(p.content, std::string(30, 'A'),
              "Appending a string longer than 30 chars should append only first 30 chars");
}

// Test 4: Append multiple times to accumulate text
void test_appendall_multiple_appends() {
    ++g_total_tests;
    sqlite3_str p;
    p.content.clear();
    sqlite3_str_appendall(&p, "12345");
    sqlite3_str_appendall(&p, "ABCDE"); // This is the focal function, but test can call appendall again
    ASSERT_EQ(p.content, std::string("12345ABCDE"), "Multiple appends should accumulate text correctly");
}

// Test 5: Append empty string after existing content should not alter content
void test_appendall_empty_after_content() {
    ++g_total_tests;
    sqlite3_str p;
    p.content = "initial";
    sqlite3_str_appendall(&p, "");
    ASSERT_EQ(p.content, std::string("initial"), "Appending empty string should not modify existing content");
}

// Main function to run tests
int main() {
    // Run individual tests
    test_appendall_empty_to_empty();
    test_appendall_short_string();
    test_appendall_long_string_limits_30();
    test_appendall_multiple_appends();
    test_appendall_empty_after_content();

    // Summary
    if (g_failed_tests == 0) {
        std::cout << "All tests passed (" << g_total_tests << " tests).\n";
        return 0;
    } else {
        std::cerr << g_failed_tests << " of " << g_total_tests
                  << " tests failed.\n";
        return 1;
    }
}
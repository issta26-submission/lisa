// Test suite for the focal method: sqlite3MPrintf
// This test suite provides minimal C++11-compatibile unit tests without Google Test.
// It mocks sqlite3VMPrintf (via C linkage) to simulate formatting behavior and validates
// the output of sqlite3MPrintf for various scenarios.
// NOTE: This file should be compiled and linked with a C translation unit that contains
// the implementation of sqlite3MPrintf (as in printf.c). We provide a test-side
// implementation of sqlite3VMPrintf to drive predictable outputs.

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>
#include <cstdlib>


// Forward declare SQLite's opaque type to match the real code usage.
typedef struct sqlite3 sqlite3;

// Provide C linkage for the test-scoped mock of sqlite3VMPrintf (overrides the real
// implementation symbol during linking).
extern "C" {

// Mock implementation of sqlite3VMPrintf used by sqlite3MPrintf during tests.
// It formats the given zFormat with the provided va_list and returns a malloc'ed string.
char* sqlite3VMPrintf(sqlite3* db, const char* zFormat, va_list ap) {
    (void)db; // db is unused by our test mock, kept for compatibility.

    // Copy va_list so we can reuse it for length calculation
    va_list ap_copy;
#if defined(__ANDROID__) || defined(__APPLE__) || defined(__linux__)
    va_copy(ap_copy, ap);
#else
    // Some compilers may not require va_copy, but for portability we try to provide a fallback.
    ap_copy = ap;
#endif

    // Determine required length
    int n = vsnprintf(nullptr, 0, zFormat, ap_copy);
    va_end(ap_copy);
    if (n < 0) {
        return nullptr;
    }

    // Allocate exact size (+1 for NUL terminator)
    char* z = (char*)std::malloc((size_t)n + 1);
    if (!z) {
        return nullptr;
    }

    // Produce the formatted string
    vsnprintf(z, (size_t)n + 1, zFormat, ap);
    return z;
}

// Prototype of the real function to allow linking with the focal method (printf.c)
char* sqlite3MPrintf(sqlite3* db, const char* zFormat, ...);

} // extern "C"

// Simple, non-terminating test harness (non-GTest)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << __FILE__ << ":" << __LINE__ \
                  << " (" << #cond << ")" << std::endl; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total; \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << __FILE__ << ":" << __LINE__ \
                  << " (actual: " << (a) << "  expected: " << (b) << ")" \
                  << std::endl; \
        ++g_failed; \
    } \
} while(0)

static void test_sqlite3MPrintf_basic_int_string() {
    // Test basic formatting with int and string arguments.
    // Expected: "Value=7 and test"
    char* res = sqlite3MPrintf(nullptr, "Value=%d and %s", 7, "test");
    std::string actual = res ? res : "";
    free(res);
    EXPECT_EQ(actual, std::string("Value=7 and test"));
}

static void test_sqlite3MPrintf_percent_literal() {
    // Test literal percent character handling (%% in format).
    // Expected: "100% sure"
    char* res = sqlite3MPrintf(nullptr, "100%% sure");
    std::string actual = res ? res : "";
    free(res);
    EXPECT_EQ(actual, std::string("100% sure"));
}

static void test_sqlite3MPrintf_width_and_precision() {
    // Test width and precision for floating-point formatting.
    // Expected: "Num=  3.14"
    char* res = sqlite3MPrintf(nullptr, "Num=%6.2f", 3.14159);
    std::string actual = res ? res : "";
    free(res);
    EXPECT_EQ(actual, std::string("Num=  3.14"));
}

static void test_sqlite3MPrintf_string_and_more_args() {
    // Test multiple arguments including a string and an integer sequence.
    // Expected: "Hello 1234 World"
    char* res = sqlite3MPrintf(nullptr, "Hello %d%s %s", 1234, "", "World");
    std::string actual = res ? res : "";
    free(res);
    EXPECT_EQ(actual, std::string("Hello 1234 World"));
}

static void run_all_tests() {
    test_sqlite3MPrintf_basic_int_string();
    test_sqlite3MPrintf_percent_literal();
    test_sqlite3MPrintf_width_and_precision();
    test_sqlite3MPrintf_string_and_more_args();

    // Summary
    std::cout << "Test summary: total=" << g_total << ", failed=" << g_failed << std::endl;
    if (g_failed == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << "Some tests failed." << std::endl;
    }
}

int main() {
    run_all_tests();
    return (g_failed == 0) ? 0 : 1;
}
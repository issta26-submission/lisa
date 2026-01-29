// Unit test suite for sqlite3IsTrueOrFalse (located in expr.c)
// This test suite does not depend on GTest; it uses a small custom framework.
// It mocks necessary external dependencies (sqlite3StrICmp, EP_IsTrue, EP_IsFalse) to enable
// in-tree compilation with the focal method.
// The tests cover true/false branches (case-insensitive) and the non-matching case.

#include <iostream>
#include <sqliteInt.h>
#include <cctype>


// Domain knowledge: provide necessary types/prototypes to link with the focal method.
// We assume the production code defines `u32` and uses EP_IsTrue/EP_IsFalse.
// We provide minimal mocks here to allow compilation/linking in this test harness.

extern "C" {
    // Minimal typedef to match sqlite3IsTrueOrFalse signature
    typedef unsigned int u32;

    // Expose the constants used by sqlite3IsTrueOrFalse
    // These values are chosen to be distinct and non-zero for true/false branches.
    extern const u32 EP_IsTrue;
    extern const u32 EP_IsFalse;

    // Prototype of the focal function under test (defined in expr.c)
    u32 sqlite3IsTrueOrFalse(const char *zIn);

    // Mock of the sqlite3StrICmp used by sqlite3IsTrueOrFalse
    // We implement a simple case-insensitive comparison: returns 0 if equal.
    int sqlite3StrICmp(const char*, const char*);
}

// Define the EP_IsTrue / EP_IsFalse values
extern "C" const u32 EP_IsTrue = 1;
extern "C" const u32 EP_IsFalse = 2;

// Local, simple, self-contained implementation of sqlite3StrICmp for testing.
// This mirrors a case-insensitive string comparison (like SQLite's function) but scoped to tests.
extern "C" int sqlite3StrICmp(const char* zA, const char* zB) {
    if (zA == nullptr && zB == nullptr) return 0;
    if (zA == nullptr) return -1;
    if (zB == nullptr) return 1;

    while (*zA && *zB) {
        unsigned char ca = static_cast<unsigned char>(*zA);
        unsigned char cb = static_cast<unsigned char>(*zB);
        ca = static_cast<unsigned char>(std::tolower(ca));
        cb = static_cast<unsigned char>(std::tolower(cb));
        if (ca != cb) return (int)ca - (int)cb;
        ++zA;
        ++zB;
    }
    return (unsigned char)*zA - (unsigned char)*zB;
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;

static void log_fail(const char* testName, unsigned int expected, unsigned int actual) {
    std::cerr << "TEST FAILED: " << testName
              << " | expected: " << expected
              << " | actual: " << actual << std::endl;
}

static bool expect_eq(const char* testName, unsigned int expected, unsigned int actual) {
    ++g_tests_run;
    if (expected == actual) {
        ++g_tests_passed;
        return true;
    } else {
        log_fail(testName, expected, actual);
        return false;
    }
}

/*
Test 1: true (lowercase)
- Input: "true"
- Expect: EP_IsTrue
*/
static bool test_true_lowercase() {
    const char* name = "test_true_lowercase";
    unsigned int res = sqlite3IsTrueOrFalse("true");
    return expect_eq(name, EP_IsTrue, res);
}

/*
Test 2: TRUE (uppercase)
- Input: "TRUE"
- Expect: EP_IsTrue
*/
static bool test_true_uppercase() {
    const char* name = "test_true_uppercase";
    unsigned int res = sqlite3IsTrueOrFalse("TRUE");
    return expect_eq(name, EP_IsTrue, res);
}

/*
Test 3: false (lowercase)
- Input: "false"
- Expect: EP_IsFalse
*/
static bool test_false_lowercase() {
    const char* name = "test_false_lowercase";
    unsigned int res = sqlite3IsTrueOrFalse("false");
    return expect_eq(name, EP_IsFalse, res);
}

/*
Test 4: FALSE (uppercase)
- Input: "FALSE"
- Expect: EP_IsFalse
*/
static bool test_false_uppercase() {
    const char* name = "test_false_uppercase";
    unsigned int res = sqlite3IsTrueOrFalse("FALSE");
    return expect_eq(name, EP_IsFalse, res);
}

/*
Test 5: Non-matching input with spaces
- Input: "true " (trailing space)
- Expect: 0
*/
static bool test_non_match_with_space() {
    const char* name = "test_non_match_with_space";
    unsigned int res = sqlite3IsTrueOrFalse("true ");
    return expect_eq(name, 0, res);
}

/*
Test 6: Non-matching input completely
- Input: "truth"
- Expect: 0
*/
static bool test_non_match_truncated() {
    const char* name = "test_non_match_truncated";
    unsigned int res = sqlite3IsTrueOrFalse("truth");
    return expect_eq(name, 0, res);
}

// Main harness: execute all tests and provide a summary.
// Note: We do not terminate on first failure to maximize coverage (non-terminating assertions).
int main() {
    bool t1 = test_true_lowercase();
    bool t2 = test_true_uppercase();
    bool t3 = test_false_lowercase();
    bool t4 = test_false_uppercase();
    bool t5 = test_non_match_with_space();
    bool t6 = test_non_match_truncated();

    // Print a concise summary
    std::cout << "Unit tests executed: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << std::endl;

    // Return success only if all tests passed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}
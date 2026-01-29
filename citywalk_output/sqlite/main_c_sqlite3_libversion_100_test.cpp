// Test suite for the focal method: sqlite3_libversion
// This test is written for C++11 and does not rely on GoogleTest.
// It uses a lightweight, non-terminating assertion mechanism to
// maximize code coverage across tests.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Step 1: Candidate Keywords extracted from the focal method and its dependencies
// - sqlite3_libversion: the function under test
// - sqlite3_version: the source/version string it returns
// - The string content and pointer identity of the version string
// These keywords guide the tests to focus on the core dependency: the version string.

extern "C" {
    // Declaration of the focal function under test
    const char *sqlite3_libversion(void);

    // Declaration of the dependent global data used by sqlite3_libversion
    // sqlite3_version is defined in the sqlite3 implementation (likely in sqlite3.c)
    // We declare it here to compare pointer/reference and content at runtime.
    extern const char sqlite3_version[];
}

// Simple non-terminating assertion mechanism
// It records failures but continues execution to maximize coverage.
static int g_test_failed = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << msg << " (in " << __func__ << ")\n"; \
        g_test_failed = 1; \
    } \
} while(0)

static void test_libversion_returns_non_null() {
    // Verifies that sqlite3_libversion returns a non-null C-string
    // Candidate Keywords: sqlite3_libversion, sqlite3_version
    const char *ver = sqlite3_libversion();
    CHECK(ver != nullptr, "sqlite3_libversion() should not return nullptr");
    // Explains what this test covers: ensures basic contract of the API.
}

static void test_libversion_matches_version_pointer() {
    // Verifies that the pointer returned by sqlite3_libversion is the same
    // as the global sqlite3_version symbol.
    // Candidate Keywords: pointer identity of sqlite3_version
    const char *ver_from_api = sqlite3_libversion();
    CHECK(ver_from_api == sqlite3_version, "sqlite3_libversion() should return the sqlite3_version symbol (pointer identity)");
}

static void test_libversion_matches_version_string_content() {
    // Verifies that the string content returned by sqlite3_libversion matches
    // the content of sqlite3_version.
    // Candidate Keywords: string content equality of the version
    const char *ver_from_api = sqlite3_libversion();
    CHECK(ver_from_api != nullptr, "sqlite3_libversion() must be non-null before content comparison");
    CHECK(std::strcmp(ver_from_api, sqlite3_version) == 0,
          "sqlite3_libversion() content should match sqlite3_version string");
}

static void test_libversion_stability_across_calls() {
    // Verifies that multiple calls produce consistent results
    // including both pointer identity and string content.
    // Candidate Keywords: stability of return value
    const char *v1 = sqlite3_libversion();
    const char *v2 = sqlite3_libversion();
    CHECK(v1 == v2, "sqlite3_libversion() should return a stable pointer across calls");
    CHECK(v1 != nullptr && v2 != nullptr, "Returned pointers should be non-null");
    CHECK(std::strcmp(v1, v2) == 0, "Returned strings should be identical across calls");
}

int main() {
    // Run all tests
    test_libversion_returns_non_null();
    test_libversion_matches_version_pointer();
    test_libversion_matches_version_string_content();
    test_libversion_stability_across_calls();

    if(g_test_failed) {
        std::cerr << "SUMMARY: FAIL (one or more tests failed)\n";
        return 1;
    } else {
        std::cout << "SUMMARY: PASS (all tests succeeded)\n";
        return 0;
    }
}
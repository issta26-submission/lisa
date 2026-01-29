// Unit test suite for cre2_opt_new and related CRE2 wrappers
// This test harness is written without Google Test (GTest) and uses a lightweight
// in-file test framework that allows multiple test cases to run in a single main().
// The tests exercise cre2_opt_new, cre2_opt_delete, cre2_new, cre2_delete, and
// cre2_easy_match to cover true/false branches where feasible in a C++11 environment.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the project wrapper headers and RE2 dependencies

// Lightweight test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void report_test_result(const char* test_name, bool passed, const char* message = nullptr) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[OK] " << test_name << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name;
        if (message) std::cerr << " - " << message;
        std::cerr << "\n";
    }
}

// Test 1: cre2_opt_new should allocate and return a non-null options object (on success).
static bool test_cre2_opt_new_returns_non_null() {
    void* raw = cre2_opt_new();
    if (!raw) {
        report_test_result("test_cre2_opt_new_returns_non_null", false,
                           "cre2_opt_new returned NULL (allocation failure in this environment)");
        return false;
    }
    // Best-effort sanity check: cast to RE2::Options* and ensure it's a plausible object.
    RE2::Options* opt = reinterpret_cast<RE2::Options*>(raw);
    if (!opt) {
        report_test_result("test_cre2_opt_new_returns_non_null", false,
                           " reinterpret_cast produced NULL for RE2::Options*");
        cre2_opt_delete((cre2_options_t*)raw);
        return false;
    }

    // Clean up using provided API (cre2_opt_delete is a no-op by design in this codebase,
    // but we still call it to simulate proper lifecycle).
    cre2_opt_delete((cre2_options_t*)raw);
    report_test_result("test_cre2_opt_new_returns_non_null", true);
    return true;
}

// Test 2: cre2_new with a valid pattern should succeed (non-null regexp handle).
static bool test_cre2_new_with_valid_pattern_succeeds() {
    void* opt_raw = cre2_opt_new();
    if (!opt_raw) {
        report_test_result("test_cre2_new_with_valid_pattern_succeeds", false,
                           "cre2_opt_new returned NULL");
        return false;
    }

    cre2_options_t* opt = reinterpret_cast<cre2_options_t*>(opt_raw);
    cre2_regexp_t* rx = cre2_new("a", 1, opt);
    if (!rx) {
        cre2_opt_delete(opt);
        report_test_result("test_cre2_new_with_valid_pattern_succeeds", false,
                           "cre2_new failed to create regexp for valid pattern");
        return false;
    }

    cre2_delete(rx);
    cre2_opt_delete(opt);
    report_test_result("test_cre2_new_with_valid_pattern_succeeds", true);
    return true;
}

// Test 3: cre2_new with an invalid pattern should return NULL (simulate false branch).
static bool test_cre2_new_with_invalid_pattern_returns_null() {
    void* opt_raw = cre2_opt_new();
    if (!opt_raw) {
        report_test_result("test_cre2_new_with_invalid_pattern_returns_null", false,
                           "cre2_opt_new returned NULL");
        return false;
    }

    cre2_regexp_t* rx = cre2_new("(", 1, reinterpret_cast<cre2_options_t*>(opt_raw));
    if (rx != nullptr) {
        cre2_delete(rx);
        cre2_opt_delete(reinterpret_cast<cre2_options_t*>(opt_raw));
        report_test_result("test_cre2_new_with_invalid_pattern_returns_null", false,
                           "cre2_new unexpectedly succeeded with invalid pattern");
        return false;
    }

    // Expected NULL; ensure we still clean up the options
    cre2_opt_delete(reinterpret_cast<cre2_options_t*>(opt_raw));
    report_test_result("test_cre2_new_with_invalid_pattern_returns_null", true);
    return true;
}

// Test 4: cre2_easy_match should return 1 on a successful match.
static bool test_cre2_easy_match_valid_match() {
    cre2_string_t match; // output placeholder
    int result = cre2_easy_match("a", 1, "a", 1, &match, 1);
    if (result != 1) {
        std::ostringstream oss;
        oss << "expected 1 (match), got " << result;
        report_test_result("test_cre2_easy_match_valid_match", false, oss.str().c_str());
        return false;
    }
    report_test_result("test_cre2_easy_match_valid_match", true);
    return true;
}

// Test 5: cre2_easy_match should return 2 when the pattern is invalid.
static bool test_cre2_easy_match_invalid_pattern_returns_2() {
    cre2_string_t match;
    int result = cre2_easy_match("(", 1, "a", 1, &match, 1);
    if (result != 2) {
        std::ostringstream oss;
        oss << "expected 2 (wrong regexp), got " << result;
        report_test_result("test_cre2_easy_match_invalid_pattern_returns_2", false, oss.str().c_str());
        return false;
    }
    report_test_result("test_cre2_easy_match_invalid_pattern_returns_2", true);
    return true;
}

// Entry point for running all tests
int main() {
    std::cout << "Running CRE2 wrapper test suite (cre2_opt_new and related wrappers)..." << std::endl;

    // Run tests
    bool t1 = test_cre2_opt_new_returns_non_null();
    bool t2 = test_cre2_new_with_valid_pattern_succeeds();
    bool t3 = test_cre2_new_with_invalid_pattern_returns_null();
    bool t4 = test_cre2_easy_match_valid_match();
    bool t5 = test_cre2_easy_match_invalid_pattern_returns_2();

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}
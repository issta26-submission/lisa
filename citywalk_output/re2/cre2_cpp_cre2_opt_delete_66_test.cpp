// Unit test suite for cre2_opt_delete (cre2.cpp) using a lightweight, non-GTest C++11 test harness.
// This suite focuses on the lifecycle of cre2_options_t objects and their interaction
// with cre2_new/cre2_delete as demonstrated in typical usage patterns.
// The tests are designed to be executable without additional test frameworks.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Simple non-terminating assertion helper.
// Prints PASS/FAIL and returns the boolean result to allow test continuation.
static bool assert_true(bool cond, const char* msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << msg << std::endl;
        return false;
    }
}

// Test 1: cre2_opt_delete on a valid options object should succeed without crashing.
// Rationale: cre2_opt_delete simply deletes the object via TO_OPT(opt). This test
// ensures that a normal lifecycle path (allocate -> delete) is safe.
static bool test_cre2_opt_delete_basic() {
    cre2_options_t *opt = cre2_opt_new();
    bool ok = (opt != nullptr);
    assert_true(ok, "cre2_opt_new should return a non-null pointer");

    if (ok) {
        cre2_opt_delete(opt);
        // If delete crashes, the test process would terminate here.
        assert_true(true, "cre2_opt_delete should complete without crash on valid opt");
    }
    return ok;
}

// Test 2: Integration path - create a pattern with options, ensure no error, then clean up.
// Rationale: Validate that cre2_opt_delete can be safely paired with cre2_new/cre2_delete
// in a realistic usage scenario (opt is passed to cre2_new to configure the regex engine).
static bool test_cre2_opt_delete_integration() {
    cre2_options_t *opt = cre2_opt_new();
    bool ok = (opt != nullptr);
    assert_true(ok, "Integration: cre2_opt_new should return non-null");

    cre2_regexp_t *rex = nullptr;
    if (ok) {
        // Use a simple valid pattern to ensure a successful cre2_new call.
        rex = cre2_new("abc", 3, opt);
        ok = (rex != nullptr) && (cre2_error_code(rex) == 0);
        assert_true(ok, "Integration: cre2_new with a valid pattern should succeed (error_code == 0)");
    }

    if (rex) {
        cre2_delete(rex);
        assert_true(true, "Integration: cre2_delete should succeed for valid regexp");
    }
    if (opt) {
        cre2_opt_delete(opt);
        assert_true(true, "Integration: cre2_opt_delete should succeed after using opt");
    }

    return ok;
}

// Test 3: cre2_opt_delete(nullptr) should be a safe no-op (null pointer handling).
// Rationale: While the focal implementation does not explicitly check for null,
// C++'s delete on a null pointer is a no-op. This test ensures stability when
// a null pointer is passed to cre2_opt_delete.
static bool test_cre2_opt_delete_nullptr() {
    // This should not crash (no-op behavior).
    cre2_opt_delete(nullptr);
    assert_true(true, "cre2_opt_delete(nullptr) should be a safe no-op");
    return true;
}

// Entry point for the test suite.
// Executes all tests and prints a concise summary.
int main() {
    std::cout << "Starting tests for cre2_opt_delete and related lifecycle..." << std::endl;

    bool t1 = test_cre2_opt_delete_basic();
    bool t2 = test_cre2_opt_delete_integration();
    bool t3 = test_cre2_opt_delete_nullptr();

    int total = 3;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);
    int failed = total - passed;

    std::cout << "\nTest summary: " << passed << " passed, " << failed << " failed." << std::endl;

    // Return non-zero if any test failed to facilitate simple CI checks.
    return (failed == 0) ? 0 : 1;
}
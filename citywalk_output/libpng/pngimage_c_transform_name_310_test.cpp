// Test suite for the focal method: transform_name(int t)
// This test suite is designed to be compiled with a C/C library
// that provides the C function:
//    extern "C" const char* transform_name(int t);
// The function returns the name corresponding to the lowest-set-bit
// in t, or "invalid transform" if there is no matching transform.
// The tests are written in C++11 without any external testing framework.

// NOTE: The implementation relies on the actual library's transform_info table.
// To exercise the code paths, we test key properties of the function rather
// than assuming any specific transform mapping. This keeps tests robust
// against changes in the transform table while ensuring meaningful coverage.

#include <stdarg.h>
#include <vector>
#include <string.h>
#include <string>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>


// Declare the C function (no name mangling)
extern "C" const char* transform_name(int t);

// Simple non-GTest test harness (non-terminating assertions)
// Collects failures and reports at the end. This keeps test execution
// flowing to maximize code coverage.
static std::vector<std::string> g_failures;

// Record a failure message for a given test
static void record_failure(const std::string& testName, const std::string& message) {
    g_failures.push_back(testName + " - " + message);
}

// Test 1: transform_name(0) must return "invalid transform"
// Rationale: t &= -t yields 0 for t == 0, so no transform matches.
// The expected behavior is to return the literal "invalid transform".
static void test_transform_name_zero_invalid() {
    // Test purpose explanation:
    // - Verify the boundary behavior when no bits are set.
    // - Ensure deterministic output for the 'no transform' case.
    const char* res = transform_name(0);
    const char* expected = "invalid transform";

    if (res == nullptr) {
        record_failure("test_transform_name_zero_invalid",
                       "transform_name(0) returned nullptr; expected string literal.");
        return;
    }

    if (std::strcmp(res, expected) != 0) {
        record_failure("test_transform_name_zero_invalid",
                       std::string("expected '") + expected + "', got '" +
                       std::string(res) + "'");
    }
}

// Test 2: Lowest-set-bit consistency property
// Rationale: For any t with multiple bits set, transform_name(t) should
// produce the same result as transform_name(t & -t) since both isolate
// the lowest set bit before the lookup loop.
// This tests the core logic path of isolating the first set bit and
// iterating the transform_info table (the loop and conditional checks).
static void test_transform_name_lowbit_consistency() {
    // A representative set of test values, including single bits, combinations,
    // and large values to exercise various code paths in the loop.
    const int test_values[] = {
        1, 2, 3, 4, 5, 6, 7,
        9, 10, 12, 15, 16, 17, 33,
        0x40, 0x80, 0x100, 0x200, 0x400,
        0x800, 0x1000, 0x2000, 0x4000, 0x8000,
        0x10000, 0x20000, 0x40000, 0x80000, 0x100000,
        0x80000000  // 32-bit wide coverage; may be truncated on some platforms
    };

    const size_t count = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t idx = 0; idx < count; ++idx) {
        int t = test_values[idx];
        // Skip negative values if any (though int is used, safety)
        if (t < 0) continue;

        const char* res_t = transform_name(t);
        int lowbit = t & -t;
        const char* res_low = transform_name(lowbit);

        if (res_t == nullptr) {
            record_failure("test_transform_name_lowbit_consistency",
                           "transform_name(" + std::to_string(t) +
                           ") returned nullptr; expected a string.");
            continue;
        }
        if (res_low == nullptr) {
            record_failure("test_transform_name_lowbit_consistency",
                           "transform_name(" + std::to_string(lowbit) +
                           ") returned nullptr; expected a string.");
            continue;
        }

        // If both are non-null, they should be equal by the function's contract.
        if (std::strcmp(res_t, res_low) != 0) {
            record_failure("test_transform_name_lowbit_consistency",
                           "Mismatch for t=" + std::to_string(t) +
                           ": transform_name(t)='" + std::string(res_t) +
                           "', transform_name(lowbit)=" + std::string(res_low));
        }
    }
}

// Additional sanity test: ensure transform_name returns a non-empty string for
// a variety of inputs. This helps catch accidental nulls or empty results.
static void test_transform_name_non_empty_for_various_inputs() {
    const int test_values[] = { 1, 2, 3, 5, 8, 13, 21, 34 };
    const size_t count = sizeof(test_values) / sizeof(test_values[0]);
    for (size_t i = 0; i < count; ++i) {
        int t = test_values[i];
        const char* res = transform_name(t);
        if (res == nullptr) {
            record_failure("test_transform_name_non_empty_for_various_inputs",
                           "transform_name(" + std::to_string(t) + ") returned nullptr.");
        } else if (std::strlen(res) == 0) {
            record_failure("test_transform_name_non_empty_for_various_inputs",
                           "transform_name(" + std::to_string(t) +
                           ") returned an empty string.");
        }
    }
}

// Entry point for running all tests
int main(void) {
    // Run individual tests
    test_transform_name_zero_invalid();
    test_transform_name_lowbit_consistency();
    test_transform_name_non_empty_for_various_inputs();

    // Report results
    if (g_failures.empty()) {
        std::cout << "All tests passed for transform_name. Coverage focused on core behavior.\n";
        return 0;
    } else {
        std::cout << g_failures.size() << " test failure(s) detected:\n";
        for (const auto& f : g_failures) {
            std::cout << " - " << f << "\n";
        }
        return 1;
    }
}
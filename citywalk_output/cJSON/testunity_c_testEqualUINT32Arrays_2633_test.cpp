/*
  Generated test suite for the focal method: testEqualUINT32Arrays
  Step 1 - Program Understanding (candidates)
  - Candidate Keywords: UNITY_UINT32, TEST_ASSERT_EQUAL_UINT32_ARRAY (represented here in plain C++), UINT32/uint32_t arrays
  - Focus: verify equality of uint32_t arrays with varying lengths using non-terminating checks in a test harness
  Step 2 - Unit Test Generation
  - Dependencies: minimal subset of the focal class dependencies (types and array comparison logic)
  - Objective: cover true paths (equal arrays) and a path that would fail if compared incorrectly
  Step 3 - Test Case Refinement
  - Use a lightweight C++11 test harness (no GTest) with non-terminating expectations
  - Provide explanations (comments) for each test
  Note: This test suite is designed to be compiled with a C++11 compiler and does not rely on GTest.
*/

#include <cstdint>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight, non-terminating test harness (non-GTest)
namespace SimpleTestHarness {
    // Public interface to record failures
    static std::vector<std::string> g_failures;
    static int g_total_tests = 0;

    // Record a failure message without aborting the test
    inline void record_failure(const std::string& msg) {
        g_failures.push_back(msg);
    }

    // Expectation helper for uint32_t arrays
    inline void expect_uint32_array_equal(const uint32_t* a, const uint32_t* b, size_t len, const std::string& desc) {
        if (std::memcmp(a, b, len * sizeof(uint32_t)) != 0) {
            char buf[256];
            std::snprintf(buf, sizeof(buf),
                          "Expectation failed: %s. Arrays differ for length %zu.",
                          desc.c_str(), len);
            record_failure(std::string(buf));
        }
    }

} // namespace SimpleTestHarness

using namespace SimpleTestHarness;

// Typedef matching candidate keyword from the focal method context
typedef uint32_t UNITY_UINT32;

// Global helper to simplify test case naming in output
static void print_test_header(const std::string& name) {
    std::cout << "Running test: " << name << std::endl;
}

// Test 1: Validate that the focal testEqualUINT32Arrays logic succeeds for a variety of lengths
// This mirrors the original Unity test scenarios where arrays are equal under certain length constraints.
void testEqualUINT32Arrays_Focal_Scenario1(void) {
    ++g_total_tests;

    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p2[] = {1, 8, 987, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    // Equal to itself with length 1
    // Expected to pass
    uint32_t* a0 = p0;
    uint32_t* b0 = p0;
    expect_uint32_array_equal(a0, b0, 1, "p0 vs p0 with len 1");

    // Equal to itself with full length (4)
    // Expected to pass
    expect_uint32_array_equal(p0, p0, 4, "p0 vs p0 with len 4");

    // Equal to another identical array with full length (4)
    // Expected to pass
    expect_uint32_array_equal(p0, p1, 4, "p0 vs p1 (identical) with len 4");

    // Equal for the first 3 elements when the 4th differs
    // Expected to pass for first 3 elements
    expect_uint32_array_equal(p0, p2, 3, "p0 vs p2 (first 3 elements equal) with len 3");

    // Equal for the first element when others differ
    // Expected to pass for first 1 element
    expect_uint32_array_equal(p0, p3, 1, "p0 vs p3 (first element equal) with len 1");
}

// Test 2: Validate that a deliberate mismatch would be detected (non-terminating assertion)
// This ensures the harness can capture failures without aborting the test suite.
void testEqualUINT32Arrays_Focal_Scenario2_FailCase(void) {
    ++g_total_tests;

    UNITY_UINT32 p0[] = {1, 2, 3, 4};
    UNITY_UINT32 p1[] = {1, 2, 3, 5}; // last element differs

    // This should fail, but the harness must not abort
    expect_uint32_array_equal(p0, p1, 4, "p0 vs p1 (deliberate mismatch) with len 4");
}

// Additional negative path: unequal lengths but same starting values should still be evaluated
void testEqualUINT32Arrays_Focal_Scenario3_PartialMatch(void) {
    ++g_total_tests;

    UNITY_UINT32 p0[] = {10, 20, 30, 40};
    UNITY_UINT32 p1[] = {10, 20, 30, 99}; // 4th element differs

    // For length 3, should pass
    expect_uint32_array_equal(p0, p1, 3, "p0 vs p1 (first 3 elements equal) with len 3");
}

// Main runner to execute tests and report summary
int main() {
    // Optional: print a header for readability
    std::cout << "===== Focal Method Test Suite: testEqualUINT32Arrays (non-GTest) =====" << std::endl;

    // Run tests
    testEqualUINT32Arrays_Focal_Scenario1();
    testEqualUINT32Arrays_Focal_Scenario2_FailCase();
    testEqualUINT32Arrays_Focal_Scenario3_PartialMatch();

    // Report results
    if (g_failures.empty()) {
        std::cout << "All tests passed. Total tests: " << g_total_tests << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with failures. Total tests: " << g_total_tests
                  << ", Failures: " << g_failures.size() << std::endl;
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "Failure " << (i + 1) << ": " << g_failures[i] << std::endl;
        }
        return 1;
    }
}
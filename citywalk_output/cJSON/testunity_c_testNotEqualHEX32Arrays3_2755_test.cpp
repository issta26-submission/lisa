// test_suite.cpp
// A lightweight C++11 test harness to exercise the focal scenario
// of testNotEqualHEX32Arrays3 and related index-differing variants.
// This is a standalone, non-GTest test suite designed to compile with
// a C++11 toolchain and not rely on the Unity framework directly.
// It mirrors the intent of the focal test: verify that two HEX32 arrays
// are not equal when a single element differs (index 2 in the focal case).

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test framework
static std::string g_currentTestName;
static bool g_currentTestFailed = false;

struct TestCase {
    const char* name;
    std::function<void(void)> func;
};

static std::vector<TestCase> g_tests;

// Register a test with the framework
static void registerTest(const char* name, std::function<void(void)> f) {
    g_tests.push_back({name, f});
}

// Emit a failure message for the currently executing test
static void fail(const std::string& msg) {
    std::cerr << "FAIL: " << g_currentTestName << " - " << msg << std::endl;
    g_currentTestFailed = true;
}

// Minimal assertion: two HEX32 arrays are equal?
static bool TEST_ASSERT_EQUAL_HEX32_ARRAY(const uint32_t* a, const uint32_t* b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Candidate Keywords (core dependents) summary for test planning
// - HEX32 arrays, 4-length, p0 vs p1, element-wise comparison
// - Difference at index 2 (as in focal testNotEqualHEX32Arrays3) and at other indices
// - Non-terminating verification pattern (tests should not crash the harness)
/////////////////////////////////////////////////////////////

// Test 1: Focal scenario reproduced - difference at index 2 (p0[2] = 987, p1[2] = 986)
static void testNotEqualHEX32Arrays3_diffIndex2(void) {
    // Focal data from the original testNotEqualHEX32Arrays3
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {1, 8, 986, 65132u};
    // Expect inequality; arrays should not be reported as equal
    bool equal = TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    if (equal) {
        fail("Expected arrays to be not equal, but they were reported equal.");
    }
}

// Test 2: Diversity - difference at index 0
static void testNotEqualHEX32Arrays3_diffIndex0(void) {
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {999, 8, 987, 65132u}; // differ at index 0
    bool equal = TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    if (equal) {
        fail("Expected arrays to be not equal (diff at index 0), but reported equal.");
    }
}

// Test 3: Diversity - difference at index 1
static void testNotEqualHEX32Arrays3_diffIndex1(void) {
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {1, 9, 987, 65132u}; // differ at index 1
    bool equal = TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    if (equal) {
        fail("Expected arrays to be not equal (diff at index 1), but reported equal.");
    }
}

// Test 4: Diversity - difference at index 3
static void testNotEqualHEX32Arrays3_diffIndex3(void) {
    const uint32_t p0[] = {1, 8, 987, 65132u};
    const uint32_t p1[] = {1, 8, 987, 65133u}; // differ at index 3
    bool equal = TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    if (equal) {
        fail("Expected arrays to be not equal (diff at index 3), but reported equal.");
    }
}

// Wrapper test registrations
static void registerAllTests() {
    registerTest("testNotEqualHEX32Arrays3_diffIndex2", testNotEqualHEX32Arrays3_diffIndex2);
    registerTest("testNotEqualHEX32Arrays3_diffIndex0", testNotEqualHEX32Arrays3_diffIndex0);
    registerTest("testNotEqualHEX32Arrays3_diffIndex1", testNotEqualHEX32Arrays3_diffIndex1);
    registerTest("testNotEqualHEX32Arrays3_diffIndex3", testNotEqualHEX32Arrays3_diffIndex3);
}

// Main test runner
int main() {
    // Register tests
    registerAllTests();

    int total = 0;
    int failed = 0;

    for (const auto& test : g_tests) {
        g_currentTestName = test.name;
        g_currentTestFailed = false;
        // Execute test
        test.func();
        ++total;
        if (g_currentTestFailed) {
            ++failed;
        } else {
            // Print a light-weight pass message per test to aid feedback
            std::cout << "PASS: " << g_currentTestName << std::endl;
        }
    }

    // Summary
    std::cout << "Test Summary: " << (total - failed) << " / " << total << " tests passed." << std::endl;
    if (failed > 0) {
        std::cerr << failed << " test(s) failed." << std::endl;
        return 1;
    }
    return 0;
}
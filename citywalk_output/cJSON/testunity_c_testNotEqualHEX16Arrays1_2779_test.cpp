// Minimal C++11 unit test harness to cover the focal testNotEqualHEX16Arrays1
// This reproduces the essence of the Unity-based test in testunity.c
// Goals:
// - Provide a small, self-contained test suite without external frameworks
// - Focus on the focal behavior: arrays differ -> TEST_ASSERT_EQUAL_HEX16_ARRAY should fail
// - Use non-terminating style: tests return a boolean indicating pass/fail
// - Include explanatory comments for each test

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight Unity-like shim to expose a minimal dependency surface
class UnityLike {
public:
    // Indicators (static members mimic static state in the original Unity file)
    static int CurrentTestFailed;
    static int CurrentTestIgnored;
    static const char* CurrentTestName;
    static unsigned int CurrentTestLineNumber;

    // Initialize current test context
    static void BeginTest(const std::string& name) {
        CurrentTestFailed = 0;
        CurrentTestIgnored = 0;
        CurrentTestName = name.c_str();
        CurrentTestLineNumber = 0;
    }

    // EndTest is a placeholder here
    static void EndTest() {
        // No-op for this minimal harness
    }
};

// Initialize static members
int UnityLike::CurrentTestFailed = 0;
int UnityLike::CurrentTestIgnored = 0;
const char* UnityLike::CurrentTestName = nullptr;
unsigned int UnityLike::CurrentTestLineNumber = 0;

// Core dependency: a minimal HEX16 array comparison function
// Returns true if all elements are equal, false otherwise
bool TEST_ASSERT_EQUAL_HEX16_ARRAY(const unsigned short* expected,
                                 const unsigned short* actual,
                                 size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        if (expected[i] != actual[i])
            return false;
    }
    return true;
}

// Wrapper for each test: the focal testNotEqualHEX16Arrays1 expects the two arrays
// to be different. The test passes if the comparison returns false.
bool testNotEqualHEX16Arrays1_impl()
{
    // Step 2: Candidate data from the focal method
    // p0 and p1 differ at the last element, mirroring the original testunity.c
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 987, 65131u};

    // The focal assertion should fail (i.e., arrays are not equal)
    // Our test harness treats "not equal" as a passing condition for this test
    bool equal = TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);

    // If equal is true, the original test would fail; we fail the test.
    if (equal)
    {
        std::cerr << "testNotEqualHEX16Arrays1_impl: unexpected equality (arrays are equal).\n";
        return false;
    }

    // Expected path: not equal -> test passes
    return true;
}

// Additional variants (to exercise more coverage around Not Equal HEX16 Arrays)
// These mirror the patterns seen in the provided Unity file, but are adapted for this harness.
bool testNotEqualHEX16Arrays2_impl()
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {2, 8, 987, 65132u}; // first element differs
    bool equal = TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    if (equal)
    {
        std::cerr << "testNotEqualHEX16Arrays2_impl: unexpected equality (arrays are equal).\n";
        return false;
    }
    return true;
}

bool testNotEqualHEX16Arrays3_impl()
{
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u}; // third element differs
    bool equal = TEST_ASSERT_EQUAL_HEX16_ARRAY(p0, p1, 4);
    if (equal)
    {
        std::cerr << "testNotEqualHEX16Arrays3_impl: unexpected equality (arrays are equal).\n";
        return false;
    }
    return true;
}

// Simple test harness to collect and report results
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

// Entry point: run all tests and print a summary
int main() {
    // Prepare test cases
    std::vector<TestCase> tests = {
        {"testNotEqualHEX16Arrays1_impl", testNotEqualHEX16Arrays1_impl},
        {"testNotEqualHEX16Arrays2_impl", testNotEqualHEX16Arrays2_impl},
        {"testNotEqualHEX16Arrays3_impl", testNotEqualHEX16Arrays3_impl}
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Starting test suite for focal method: testNotEqualHEX16Arrays1 (adapted)\n";

    for (auto& tc : tests) {
        UnityLike::BeginTest(tc.name);
        bool result = false;
        try {
            // Execute test
            result = tc.func();
        } catch (const std::exception& ex) {
            std::cerr << "Test " << tc.name << " threw exception: " << ex.what() << "\n";
            result = false;
        } catch (...) {
            std::cerr << "Test " << tc.name << " threw an unknown exception.\n";
            result = false;
        }
        UnityLike::EndTest();

        if (result) {
            ++passed;
            std::cout << "[PASS]  " << tc.name << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL]  " << tc.name << "\n";
        }
    }

    // Summary
    std::cout << "Test results: " << passed << " passed, " << failed << " failed.\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}
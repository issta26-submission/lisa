// Test Suite: NotEqual HEX Arrays - Focal Method
// Context: This suite targets the focal testNotEqualHEXArrays3 used in testunity.c.
// The original test relies on Unity's test framework macros (EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_HEX_ARRAY, VERIFY_FAILS_END).
// To provide a portable C++11 test harness (without GTest), we re-create the core assertion behavior
// and verify that the expected "abort/fail" path is triggered when two HEX arrays differ.
// Notes:
// - Candidate Keywords extracted from the focal method: HEX arrays, p0, p1, array length (4), mismatch, abort path.
// - We also demonstrate a simple static dependency handling as an auxiliary test (static member usage).

#include <exception>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Minimal exception to simulate Unity's abort/fail scenario for a hex-array comparison.
struct TestAbort : public std::exception {};

// Core helper: asserts that two HEX arrays are equal; if any element differs, it aborts.
void assert_equal_hex_array_abort(const uint32_t* a, const uint32_t* b, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
        {
            // Simulate Unity's abort on assertion failure
            throw TestAbort();
        }
    }
    // If all elements are equal, function would return normally (success path).
}

// Auxiliary static-dependency demonstration
struct Dependency
{
    static int s_counter;
    static int inc_and_get(void) { return ++s_counter; }
};
int Dependency::s_counter = 0;

// Focal test case adapted to C++11 test harness:
// This mirrors testNotEqualHEXArrays3 from testunity.c, expecting the test to abort due to a mismatch in the 3rd element.
bool testNotEqualHEXArrays3_focal(void)
{
    // Candidate HEX arrays where the 3rd element differs: 987 vs 986
    uint32_t p0[] = {1, 8, 987, 65132u};
    uint32_t p1[] = {1, 8, 986, 65132u};

    try
    {
        // This should abort (throw) due to mismatch
        assert_equal_hex_array_abort(p0, p1, 4);
        // If we reach here, the abort did not occur as expected
        return false; // fail
    }
    catch (const TestAbort&)
    {
        // Expected abort occurred
        return true; // pass
    }
    catch (...)
    {
        // Unexpected exception type
        return false; // fail
    }
}

// Auxiliary test: verify a scenario where the static dependency is accessible and increments correctly.
// This ensures class dependencies (static members) are usable in tests, matching Step 1's guidance regarding static members.
bool test_static_dependency_usage(void)
{
    int a = Dependency::inc_and_get(); // should be 1
    int b = Dependency::inc_and_get(); // should be 2
    // True only if static behavior is as expected
    return (a == 1 && b == 2);
}

// A small test harness to run tests without GTest/Unity
int main()
{
    // Register tests (name, function)
    typedef std::function<bool(void)> TestFn;
    struct TestCase {
        std::string name;
        TestFn    func;
    };

    std::vector<TestCase> tests = {
        { "NotEqualHEXArrays3 focal (p0 vs p1 differs on 3rd element)", testNotEqualHEXArrays3_focal },
        { "Static dependency usage (increment counter)", test_static_dependency_usage }
    };

    int passed = 0;
    for (const auto& tc : tests)
    {
        bool result = tc.func();
        if (result)
        {
            std::cout << "[PASS] " << tc.name << std::endl;
            ++passed;
        }
        else
        {
            std::cout << "[FAIL] " << tc.name << std::endl;
        }
    }

    std::cout << "Summary: " << passed << " / " << tests.size() << " tests passed." << std::endl;
    return (passed == static_cast<int>(tests.size())) ? 0 : 1;
}
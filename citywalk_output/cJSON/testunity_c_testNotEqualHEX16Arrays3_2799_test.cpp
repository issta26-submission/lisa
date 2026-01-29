/*
  Lightweight C++11 test harness translating the focal Unity test
  testNotEqualHEX16Arrays3 into standalone C++ tests.

  Step 1 (conceptual): The focal test asserts that two HEX16 arrays are not equal
  by triggering a failure when they differ at index 2. We implement a small
  hex16ArrayEquals helper and two tests:
    - testNotEqualHEX16Arrays3: should pass because arrays are not equal
    - testNotEqualHEX16Arrays3_EqualCase: should fail (arrays are equal)

  Step 2: Class/Dependency knowledge is reflected by providing a minimal
  "TEST_ASSERT_EQUAL_HEX16_ARRAY"-like comparison (hex16ArrayEquals) in a
  static scope (analogous to file-scope helper in the original test file).

  Step 3: Coverage refinement is achieved by exercising both the not-equal case
  and an equal-case scenario to ensure true/false branches are covered.
*/

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords mapped:
// - HEX16: unsigned short arrays (16-bit hex values)
// - ARRAY: array comparisons
// - NOT EQUAL: expectation of inequality
// - TEST_ASSERT_EQUAL_HEX16_ARRAY: equivalent to hex16ArrayEquals in this harness

// Static helper simulating Unity's TEST_ASSERT_EQUAL_HEX16_ARRAY behavior.
// Returns true if arrays are equal, false otherwise.
static bool hex16ArrayEquals(const unsigned short* a, const unsigned short* b, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Global flag to indicate overall test success/failure
static bool g_testsPassed = true;

/*
  Test: testNotEqualHEX16Arrays3
  Purpose:
    Mirrors the Unity testNotEqualHEX16Arrays3(), which builds two HEX16 arrays
    that differ at index 2 and asserts they are not equal. In this harness
    we verify that hex16ArrayEquals returns false for differing arrays and
    report PASS when inequality is detected.

  This covers the "not equal" branch (true/false decision) for 4-element arrays.
*/
static void testNotEqualHEX16Arrays3(void)
{
    // FOCAL TEST DATA (as in the provided testunity.c)
    unsigned short p0[] = {1, 8, 987, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    bool eq = hex16ArrayEquals(p0, p1, 4);

    if (eq)
    {
        std::cout << "testNotEqualHEX16Arrays3: FAIL - arrays are unexpectedly equal\n";
        g_testsPassed = false;
    }
    else
    {
        // Expected path: arrays are not equal
        std::cout << "testNotEqualHEX16Arrays3: PASS\n";
    }
}

/*
  Test: testNotEqualHEX16Arrays3_EqualCase
  Purpose:
    Additional coverage to exercise the equal-case branch.
    Ensures that when arrays are equal, the equality check returns true
    and the test should fail if we were asserting inequality.
*/
static void testNotEqualHEX16Arrays3_EqualCase(void)
{
    unsigned short p0[] = {1, 8, 986, 65132u};
    unsigned short p1[] = {1, 8, 986, 65132u};

    bool eq = hex16ArrayEquals(p0, p1, 4);

    if (!eq)
    {
        std::cout << "testNotEqualHEX16Arrays3_EqualCase: FAIL - arrays should be equal\n";
        g_testsPassed = false;
    }
    else
    {
        // This is the expected outcome for the equal-case
        std::cout << "testNotEqualHEX16Arrays3_EqualCase: PASS\n";
    }
}

int main()
{
    // Run the tests
    testNotEqualHEX16Arrays3();
    testNotEqualHEX16Arrays3_EqualCase();

    if (g_testsPassed)
    {
        std::cout << "All tests completed. Result: PASS\n";
        return 0;
    }
    else
    {
        std::cerr << "One or more tests failed. Result: FAIL\n";
        return 1;
    }
}
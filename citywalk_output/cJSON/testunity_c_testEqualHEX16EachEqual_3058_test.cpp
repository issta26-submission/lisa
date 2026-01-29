// A lightweight C++11 unit test harness focused specifically on the focal method
// testEqualHEX16EachEqual (as implemented in testunity.c). The goal is to
// exercise the same core logic (TEST_ASSERT_EACH_EQUAL_HEX16-like checks) without
// relying on the Unity framework. This provides a standalone, compilable test
// suite in C++11 that covers both passing and failing scenarios for this focal
// behavior, and demonstrates true/false branches.
//
// Notes:
// - This test suite is intended to be compiled with a C++11 compiler.
// - It does not rely on GTest or other heavy test frameworks per instructions.
// - The tests mimic the semantics of TEST_ASSERT_EACH_EQUAL_HEX16:
//   ensure every element in an array equals the expected value for a given count.
//
// Candidate Keywords (derived from the focal method and its dependencies):
// - UNITY_UINT16, TEST_ASSERT_EACH_EQUAL_HEX16, p0, p1, p2, p3, 65132u, 987, 8, 1, 500, 600, 700
// - The core operation is element-wise equality checking across arrays.

#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight helper to run a named test and report PASS/FAIL
static bool runTest(const std::string& name, const std::function<bool()> &fn) {
    bool ok = fn();
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
    return ok;
}

// Reusable checker that mimics TEST_ASSERT_EACH_EQUAL_HEX16(expected, array, count)
// It returns true if all elements equal 'expected' for the first 'count' items.
static bool assertEachEqualHEX16(uint16_t expected, const uint16_t* array, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        if (array[i] != expected) {
            return false;
        }
    }
    return true;
}

// Focal test 1: Basic passing case for testEqualHEX16EachEqual
// Mirrors: TEST_ASSERT_EACH_EQUAL_HEX16(65132u, p0, 1);
//           TEST_ASSERT_EACH_EQUAL_HEX16(65132u, p0, 4);
//           TEST_ASSERT_EACH_EQUAL_HEX16(987, p1, 4);
//           TEST_ASSERT_EACH_EQUAL_HEX16(8, p2, 3);
//           TEST_ASSERT_EACH_EQUAL_HEX16(1, p3, 1);
static bool testEqualHEX16EachEqual_cpp(void) {
    // Candidate data as in focal method
    const uint16_t p0[] = {65132u, 65132u, 65132u, 65132u};
    const uint16_t p1[] = {987, 987, 987, 987};
    const uint16_t p2[] = {8, 8, 8, 2};
    const uint16_t p3[] = {1, 500, 600, 700};

    bool ok = true;
    ok &= assertEachEqualHEX16(65132u, p0, 1);
    ok &= assertEachEqualHEX16(65132u, p0, 4);
    ok &= assertEachEqualHEX16(987,     p1, 4);
    ok &= assertEachEqualHEX16(8,       p2, 3);
    ok &= assertEachEqualHEX16(1,       p3, 1);

    return ok;
}

// Focal test 2: Not-equal scenario #1 for testEqualHEX16EachEqual
// Intent: the third element mismatches when expecting 65132
// This should fail the "each equal" check.
static bool testNotEqualHEX16EachEqual1_cpp(void) {
    const uint16_t p0[] = {65132u, 65132u, 987u, 65132u};

    // All values must be 65132; element 2 (index 2) is 987 -> mismatch
    return !assertEachEqualHEX16(65132u, p0, 4);
}

// Focal test 3: Not-equal scenario #2 for testEqualHEX16EachEqual
// Intent: the first element mismatches when expecting 987
static bool testNotEqualHEX16EachEqual2_cpp(void) {
    const uint16_t p0[] = {1u, 987u, 987u, 987u};

    // All values must be 987; first element is 1 -> mismatch
    return !assertEachEqualHEX16(987u, p0, 4);
}

// Focal test 4: Not-equal scenario #3 for testEqualHEX16EachEqual
// Intent: the last element mismatches when expecting 8
static bool testNotEqualHEX16EachEqual3_cpp(void) {
    const uint16_t p0[] = {8u, 8u, 8u, 65132u};

    // All values must be 8; last element is 65132 -> mismatch
    return !assertEachEqualHEX16(8u, p0, 4);
}

// Simple test driver
int main() {
    std::vector<std::pair<std::string, std::function<bool()>>> tests = {
        { "testEqualHEX16EachEqual_cpp (passing case)", testEqualHEX16EachEqual_cpp },
        { "testNotEqualHEX16EachEqual1_cpp (should fail)", testNotEqualHEX16EachEqual1_cpp },
        { "testNotEqualHEX16EachEqual2_cpp (should fail)", testNotEqualHEX16EachEqual2_cpp },
        { "testNotEqualHEX16EachEqual3_cpp (should fail)", testNotEqualHEX16EachEqual3_cpp }
    };

    int passed = 0, failed = 0;

    for (auto &t : tests) {
        bool result = runTest(t.first, t.second);
        if (result) ++passed; else ++failed;
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed." << std::endl;

    // Return non-zero if any test failed to signal issues to build systems
    return failed == 0 ? 0 : 1;
}
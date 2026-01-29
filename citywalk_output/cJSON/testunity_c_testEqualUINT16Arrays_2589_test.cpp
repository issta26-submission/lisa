// test_equal_uint16_arrays.cpp
// Purpose: Provide a C++11 based unit test suite for the focal concept
// of comparing equality of UINT16 arrays, emulating the behavior of
// testEqualUINT16Arrays from testunity.c using a lightweight, non-GTest framework.
// This test suite focuses on the core dependent component "equal_uint16_arrays"
// and exercises both true and false branches as shown in the original Unity tests.

// Domain knowledge notes (mapped to Candidate Keywords):
// - Candidate Keywords: UINT16 arrays, p0/p1/p2/p3 arrays, equality predicate for UINT16 arrays,
//   length parameter, boundary cases (len=1, len=4, len=3, len=0).
// - The focal method under test is conceptually: bool equal_uint16_arrays(const uint16_t*, const uint16_t*, size_t).
// - The tests exercise true paths (arrays equal for given length) and false paths (differences within the length).

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Candidate implementation of the focal method under test.
// Compares the first 'len' elements of two UINT16 arrays and returns true if all
// corresponding elements are equal, false otherwise. If len == 0, returns true
// (empty range is trivially equal).
bool equal_uint16_arrays(const uint16_t* a, const uint16_t* b, size_t len)
{
    if (len == 0) return true;
    if (a == nullptr || b == nullptr) return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Lightweight test framework (non-GTest, suitable for C++11)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple assertion helpers
#define ASSERT_TRUE(cond, msg) do {                               \
    ++g_tests_run;                                                 \
    if (!(cond)) {                                                 \
        ++g_tests_failed;                                          \
        std::cerr << "TEST FAILED (ASSERT_TRUE): "                  \
                  << __FUNCTION__ << " - " << (msg) << "\n";     \
    }                                                            \
} while (false)

#define ASSERT_FALSE(cond, msg) do {                              \
    ++g_tests_run;                                                 \
    if (cond) {                                                    \
        ++g_tests_failed;                                          \
        std::cerr << "TEST FAILED (ASSERT_FALSE): "                 \
                  << __FUNCTION__ << " - " << (msg) << "\n";     \
    }                                                            \
} while (false)


// Test 1: Basic equality scenarios mirroring focal test:
// p0 == {1,8,987,65132}, p1 == {1,8,987,65132}, p2 == {1,8,987,2}, p3 == {1,500,600,700}
// Verifies various lengths: 1, 4, 4, 3, 1 respectively.
void testEqualUINT16Arrays_basic()
{
    // Candidate arrays matching the original test vectors
    uint16_t p0[] = {1, 8, 987, 65132u};
    uint16_t p1[] = {1, 8, 987, 65132u};
    uint16_t p2[] = {1, 8, 987, 2};
    uint16_t p3[] = {1, 500, 600, 700};

    // All of these should be considered equal by the focal function
    ASSERT_TRUE(equal_uint16_arrays(p0, p0, 1), "p0 vs p0 should be equal for length 1");
    ASSERT_TRUE(equal_uint16_arrays(p0, p0, 4), "p0 vs p0 should be equal for length 4");
    ASSERT_TRUE(equal_uint16_arrays(p0, p1, 4), "p0 vs p1 should be equal for length 4");
    ASSERT_TRUE(equal_uint16_arrays(p0, p2, 3), "First three elements equal between p0 and p2");
    ASSERT_TRUE(equal_uint16_arrays(p0, p3, 1), "First element equal between p0 and p3");
}

// Test 2: Not-equal scenarios where differences exist within the compared length
// (should return false).
void testNotEqualUINT16Arrays1()
{
    uint16_t p0[] = {1, 8, 987, 65132u};
    uint16_t p1[] = {1, 8, 987, 65131u};
    ASSERT_FALSE(equal_uint16_arrays(p0, p1, 4), "Last element differs -> not equal");
}

// Test 3: Not-equal scenario with first element differing
void testNotEqualUINT16Arrays2()
{
    uint16_t p0[] = {1, 8, 987, 65132u};
    uint16_t p1[] = {2, 8, 987, 65132u};
    ASSERT_FALSE(equal_uint16_arrays(p0, p1, 4), "First element differs -> not equal");
}

// Test 4: Not-equal scenario with a middle element differing
void testNotEqualUINT16Arrays3()
{
    uint16_t p0[] = {1, 8, 987, 65132u};
    uint16_t p1[] = {1, 8, 986, 65132u};
    ASSERT_FALSE(equal_uint16_arrays(p0, p1, 4), "Third element differs -> not equal");
}

// Test 5: Length-zero should yield true (empty arrays are equal by definition)
void testEqualUINT16ArraysLengthZero()
{
    uint16_t a[] = {1, 2, 3};
    uint16_t b[] = {4, 5, 6};
    ASSERT_TRUE(equal_uint16_arrays(a, b, 0), "Length 0 should be equal regardless of contents");
}

// Run all tests and report summary
void run_all_tests()
{
    testEqualUINT16Arrays_basic();
    testNotEqualUINT16Arrays1();
    testNotEqualUINT16Arrays2();
    testNotEqualUINT16Arrays3();
    testEqualUINT16ArraysLengthZero();
}

int main()
{
    run_all_tests();

    std::cout << "\nTest summary: " << g_tests_run << " run, "
              << g_tests_failed << " failed.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}
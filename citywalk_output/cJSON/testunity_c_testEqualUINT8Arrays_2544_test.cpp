// Minimal C++11 test harness for the focal C function logic (testEqualUINT8Arrays)
// This reproduces the essential behavior of the Unity-based test in a lightweight
// C++ test runner without relying on GTest or Unity framework.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Compare two uint8_t arrays of given length.
// For NULL pointers, we adopt a conservative policy matching Unity semantics:
// equal if both are NULL; otherwise not equal.
static inline bool equal_uint8_arrays(const uint8_t* a, const uint8_t* b, size_t len)
{
    if (a == nullptr || b == nullptr)
        return (a == b); // both NULL -> true, else false
    return std::memcmp(a, b, len) == 0;
}

// Mimic Unity's assertion for uint8_t arrays without terminating the test program.
#define TEST_ASSERT_EQUAL_UINT8_ARRAY(A, B, LEN) do {                          \
    ++g_tests_run;                                                               \
    if (!equal_uint8_arrays((A), (B), (LEN))) {                                \
        ++g_tests_failed;                                                        \
        std::cerr << "Test failed: ASSERT_EQUAL_UINT8_ARRAY at "                    \
                  << __FILE__ << ":" << __LINE__                               \
                  << " - Arrays differ (len=" << (LEN) << ")" << std::endl;  \
    }                                                                            \
} while (0)

// Focal method translated into a test function (as in the provided testunity.c)
// It validates several combinations where arrays are expected to be equal.
void testEqualUINT8Arrays(void)
{
    // p0: original data
    uint8_t p0[] = {1, 8, 100, 127};
    // p1: identical to p0
    uint8_t p1[] = {1, 8, 100, 127};
    // p2: differs in the last element
    uint8_t p2[] = {1, 8, 100, 2};
    // p3: differs in the middle elements
    uint8_t p3[] = {1, 50, 60, 70};

    // The following assertions mirror the focal method's expectations:
    // 1) Compare p0 with itself for length 1
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 1);
    // 2) Compare p0 with itself for full length 4
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p0, 4);
    // 3) Compare p0 with p1 (identical) for length 4
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
    // 4) Compare p0 with p2 (one element different) for first 3 elements
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p2, 3);
    // 5) Compare p0 with p3 (differences) for first element
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p3, 1);
}

// Negative tests to exercise the failure paths (mirroring testNotEqualUINT8Arrays1..3)
// These demonstrate that the array comparison fails when content differs.
void testNotEqualUINT8Arrays1(void)
{
    uint8_t p0[] = {1, 8, 100, 127};
    uint8_t p1[] = {1, 8, 100, 255};
    // Expected to fail: p0 != p1 for full length
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
}

void testNotEqualUINT8Arrays2(void)
{
    uint8_t p0[] = {1, 8, 100, 127};
    uint8_t p1[] = {1, 8, 100, 255};
    // Repeated scenario to ensure consistency of failure reporting
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
}

void testNotEqualUINT8Arrays3(void)
{
    uint8_t p0[] = {1, 8, 100, 127};
    uint8_t p1[] = {1, 8, 100, 255};
    // Third variant to verify multiple call sites can fail
    TEST_ASSERT_EQUAL_UINT8_ARRAY(p0, p1, 4);
}

// Entry point: run all tests and report summary
int main()
{
    // Run tests (mirroring the original pair: a passing test and three failing-path tests)
    testEqualUINT8Arrays();
    testNotEqualUINT8Arrays1();
    testNotEqualUINT8Arrays2();
    testNotEqualUINT8Arrays3();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}
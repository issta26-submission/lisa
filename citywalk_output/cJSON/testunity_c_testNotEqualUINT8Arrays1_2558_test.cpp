// test_NotEqualUINT8Arrays1_suite.cpp
// Purpose: Provide a standalone C++11 test suite that covers the core idea of the
// focal method testNotEqualUINT8Arrays1 from testunity.c.
// Note: This is a lightweight, framework-free suite intended to exercise not-equality
// checks on UINT8 arrays. It is independent of GTest and Unity to ensure the
// tests are executable in environments where Unity isn't available.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Simple lightweight test harness (non-GTest) for executing test cases.
struct TestCase {
    const char* name;
    std::function<void()> func;
    bool passed;
};

// Helper macro to make test registration convenient
#define REGISTER_TEST(name) { name, name, false }

// Focal-like test: two 4-byte UINT8 arrays differ in the last element.
// This mirrors the behavior of testNotEqualUINT8Arrays1 in testunity.c.
void testNotEqualUINT8Arrays1_cpp()
{
    // Given arrays intentionally differ at the last element
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 100, 255u};

    // Act: compare the memory blocks
    int cmp = std::memcmp(p0, p1, 4);

    // Assert: they are not equal
    if (cmp == 0) {
        // If arrays are somehow considered equal, that's a failure for this test
        throw std::runtime_error("testNotEqualUINT8Arrays1_cpp: arrays unexpectedly equal");
    }
}

// Additional variants to broaden coverage around UINT8 array inequality

// Variant 2: differs in the first element
void testNotEqualUINT8Arrays_FirstByteDiff_cpp()
{
    unsigned char p0[] = {2, 8, 100, 127u};
    unsigned char p1[] = {3, 8, 100, 127u};
    int cmp = std::memcmp(p0, p1, 4);
    if (cmp == 0) {
        throw std::runtime_error("First byte difference not detected");
    }
}

// Variant 3: differs in the middle element
void testNotEqualUINT8Arrays_MidByteDiff_cpp()
{
    unsigned char p0[] = {1, 8, 100, 127u};
    unsigned char p1[] = {1, 8, 101, 127u};
    int cmp = std::memcmp(p0, p1, 4);
    if (cmp == 0) {
        throw std::runtime_error("Middle byte difference not detected");
    }
}

// Variant 4: all elements differ
void testNotEqualUINT8Arrays_AllDiff_cpp()
{
    unsigned char p0[] = {10, 20, 30, 40};
    unsigned char p1[] = {11, 21, 31, 41};
    int cmp = std::memcmp(p0, p1, 4);
    if (cmp == 0) {
        throw std::runtime_error("All-different arrays unexpectedly equal");
    }
}

// Variant 5: equal arrays (positive check for equality path)
// This confirms that a true equality would fail a "not equal" expectation.
void testEqualUINT8Arrays_EqualityCheck_cpp()
{
    unsigned char p0[] = {5, 6, 7, 8};
    unsigned char p1[] = {5, 6, 7, 8};
    int cmp = std::memcmp(p0, p1, 4);
    if (cmp != 0) {
        throw std::runtime_error("Equality check failed: arrays are not equal");
    }
}

// Main function runs all test cases and reports results.
int main()
{
    std::vector<TestCase> tests = {
        REGISTER_TEST(testNotEqualUINT8Arrays1_cpp),
        REGISTER_TEST(testNotEqualUINT8Arrays_FirstByteDiff_cpp),
        REGISTER_TEST(testNotEqualUINT8Arrays_MidByteDiff_cpp),
        REGISTER_TEST(testNotEqualUINT8Arrays_AllDiff_cpp),
        REGISTER_TEST(testEqualUINT8Arrays_EqualityCheck_cpp)
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Running NotEqual UINT8 Arrays test-suite (standalone C++11)" << std::endl;

    for (auto &tc : tests) {
        try {
            tc.func();
            tc.passed = true;
            ++passed;
            std::cout << "[PASS] " << tc.name << std::endl;
        } catch (const std::exception& ex) {
            tc.passed = false;
            ++failed;
            std::cout << "[FAIL] " << tc.name << " -- " << ex.what() << std::endl;
        } catch (...) {
            tc.passed = false;
            ++failed;
            std::cout << "[FAIL] " << tc.name << " -- unknown exception" << std::endl;
        }
    }

    // Summary
    std::cout << "\nTest results: " << passed << " passed, " << failed << " failed." << std::endl;
    if (failed > 0) {
        std::cerr << "Some tests failed. Aborting with non-zero status." << std::endl;
        return 1;
    }

    return 0;
}
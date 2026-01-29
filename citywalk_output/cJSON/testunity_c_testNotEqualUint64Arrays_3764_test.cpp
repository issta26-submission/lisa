// Candidate Keywords extracted from the focal method (testNotEqualUint64Arrays) and its context:
// - UINT64 types and 64-bit array handling
// - Array equality check (TEST_ASSERT_EQUAL_UINT64_ARRAY semantics)
// - p0, p1 array inputs and a length (4 in the focal example)
// - Aborting/Failing behavior is part of Unity, but here we implement a lightweight, non-Unity
//   C++11 test harness to validate underlying equality logic without terminating the test process.
// - We cover both equal and not-equal scenarios and edge cases like zero length and NULL pointers.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight, framework-agnosed assertion helpers
static void failWith(const std::string& msg) {
    throw std::runtime_error(msg);
}

static void expectTrue(bool cond, const std::string& msg) {
    if (!cond) failWith(msg);
}

static void expectFalse(bool cond, const std::string& msg) {
    if (cond) failWith(msg);
}

// Core utility: check equality of two uint64_t arrays of given length
// Mirrors the essential logic behind Unity's TEST_ASSERT_EQUAL_UINT64_ARRAY without framework side-effects.
static bool equalUint64Arrays(const uint64_t* a, const uint64_t* b, size_t len) {
    if (len == 0) return true;
    if (a == nullptr || b == nullptr) return false;
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Simple test harness
namespace FocalTestHarness {
    using TestFunc = std::function<void()>;

    struct TestCase {
        const char* name;
        TestFunc   func;
    };

    static std::vector<TestCase> tests;

    static void registerTest(const char* name, TestFunc func) {
        tests.push_back({name, func});
    }

    static int runAll() {
        int passed = 0;
        int failed = 0;
        for (const auto& t : tests) {
            try {
                t.func();
                std::cout << "[PASS] " << t.name << "\n";
                ++passed;
            } catch (const std::exception& ex) {
                std::cout << "[FAIL] " << t.name << " - Exception: " << ex.what() << "\n";
                ++failed;
            } catch (...) {
                std::cout << "[FAIL] " << t.name << " - Unknown exception\n";
                ++failed;
            }
        }
        std::cout << "Summary: " << passed << " passed, " << failed << " failed\n";
        return (failed == 0) ? 0 : 1;
    }
}

// Forward declarations of tests to allow registration in static initialization
static void testEqualUint64ArraysIdentical(void);
static void testNotEqualUint64Arrays_LastElementDiff(void);
static void testNotEqualUint64Arrays_MiddleDiff(void);
static void testEqualUint64Arrays_ZeroLength(void);
static void testNotEqualUint64Arrays_NullPointerWithNonZeroLen(void);

// Register tests (executed before main)
struct RegisterTests {
    RegisterTests() {
        FocalTestHarness::registerTest("testEqualUint64ArraysIdentical", testEqualUint64ArraysIdentical);
        FocalTestHarness::registerTest("testNotEqualUint64Arrays_LastElementDiff", testNotEqualUint64Arrays_LastElementDiff);
        FocalTestHarness::registerTest("testNotEqualUint64Arrays_MiddleDiff", testNotEqualUint64Arrays_MiddleDiff);
        FocalTestHarness::registerTest("testEqualUint64Arrays_ZeroLength", testEqualUint64Arrays_ZeroLength);
        FocalTestHarness::registerTest("testNotEqualUint64Arrays_NullPointerWithNonZeroLen", testNotEqualUint64Arrays_NullPointerWithNonZeroLen);
    }
} staticRegistrations;

// Test 1: Equal identical arrays should be reported as equal
// Rationale: validate the positive path for 64-bit array equality.
static void testEqualUint64ArraysIdentical(void) {
    uint64_t a[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t b[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    bool eq = equalUint64Arrays(a, b, 4);
    // Expect true (arrays are identical)
    expectTrue(eq, "Identical 64-bit arrays should be equal");
}

// Test 2: Not equal in the last element
// Rationale: core scenario from focal method where arrays differ at last item.
static void testNotEqualUint64Arrays_LastElementDiff(void) {
    uint64_t a[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t b[] = {1ULL, 8ULL, 987ULL, 65131ULL};
    bool eq = equalUint64Arrays(a, b, 4);
    // Expect false (arrays differ)
    expectFalse(eq, "Arrays differ in the last element should not be equal");
}

// Test 3: Not equal in the middle element
// Rationale: ensure mid-array difference is detected.
static void testNotEqualUint64Arrays_MiddleDiff(void) {
    uint64_t a[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t b[] = {1ULL, 9ULL, 987ULL, 65132ULL};
    bool eq = equalUint64Arrays(a, b, 4);
    // Expect false (difference at index 1)
    expectFalse(eq, "Arrays differ in the middle element should not be equal");
}

// Test 4: Zero length arrays should be considered equal
// Rationale: validates edge case; Unity treats length 0 as trivial equality.
static void testEqualUint64Arrays_ZeroLength(void) {
    // Pointers may be NULL for zero length in many APIs
    const uint64_t* a = nullptr;
    const uint64_t* b = nullptr;
    bool eq = equalUint64Arrays(a, b, 0);
    expectTrue(eq, "Zero-length arrays should be considered equal");
}

// Test 5: NULL pointer with non-zero length should be considered not equal
// Rationale: ensure proper handling of invalid pointer usage in equality checks.
static void testNotEqualUint64Arrays_NullPointerWithNonZeroLen(void) {
    const uint64_t* a = nullptr;
    uint64_t b[] = {1ULL, 2ULL, 3ULL, 4ULL};
    bool eq = equalUint64Arrays(a, b, 4);
    // Expect false (nullptr vs valid data with non-zero length)
    expectFalse(eq, "Null pointer with non-zero length should not be equal to a valid array");
}

// Main entry for the standalone test executable
int main() {
    // Run all registered tests
    return FocalTestHarness::runAll();
}
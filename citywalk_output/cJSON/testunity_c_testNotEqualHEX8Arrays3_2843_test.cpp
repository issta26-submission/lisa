// Minimal C++11 based unit-test suite for the focal behavior around the
// testNotEqualHEX8Arrays3 scenario (i.e., two HEX8 arrays that are not equal).
// This standalone suite does not rely on GTest. It uses a tiny harness to
// validate that non-equality is detected (pass) and equality is treated as a
// failure (for the "should fail" meta-test).

#include <unity.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test harness
class TestHarness {
public:
    struct TestCase {
        std::string name;
        std::function<void()> func;
        bool expectsThrow; // true if this test is designed to pass only when an exception is thrown
    };

    void addCase(const std::string& name,
                 const std::function<void()>& f,
                 bool expectsThrow = false) {
        cases.push_back({name, f, expectsThrow});
    }

    int runAll() {
        int total = 0;
        int passed = 0;
        int failed = 0;
        for (const auto& tc : cases) {
            ++total;
            bool threw = false;
            try {
                tc.func();
            } catch (...) {
                threw = true;
            }
            bool ok = (threw == tc.expectsThrow);
            if (ok) {
                ++passed;
            } else {
                ++failed;
                std::cerr << "Test failed: " << tc.name
                          << " (expectsThrow=" << (tc.expectsThrow ? "true" : "false")
                          << ", threw=" << (threw ? "yes" : "no") << ")\n";
            }
        }

        // Summary
        std::cout << "\nTest summary: "
                  << passed << " passed, "
                  << failed << " failed, "
                  << total << " total.\n";

        return (failed == 0) ? 0 : 1;
    }

private:
    std::vector<TestCase> cases;
};

// Helper: Compare two hex8 arrays (unsigned bytes) for equality
static bool hex8_arrays_equal(const unsigned char* a, const unsigned char* b, size_t len) {
    return std::memcmp(a, b, len) == 0;
}

// Focal logic (stand-in for the essence of testNotEqualHEX8Arrays3):
// The Unity test originally asserts two arrays are not equal and aborts on
// equality. Here, we model that: if arrays are equal, we throw to indicate a
// test failure (as per "should have failed" semantics).
static void focal_not_equal_hex8_arrays3_test_impl(const unsigned char* a, const unsigned char* b, size_t len) {
    if (hex8_arrays_equal(a, b, len)) {
        // Simulate an abort/failure condition when arrays are equal
        throw std::runtime_error("Arrays are unexpectedly equal (should be not equal).");
    }
    // If not equal, test passes (no exception)
}

// Test helpers for specific scenarios

// Test 1: Original scenario from the focal method (not equal at index 2)
void testNotEqualHEX8Arrays3_Basic() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Test 2: Not equal with a difference in the first element
void testNotEqualHEX8Arrays3_DiffFirst() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {2, 8, 254u, 253u};
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Test 3: Not equal with a difference in the second element
void testNotEqualHEX8Arrays3_DiffSecond() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 9, 254u, 253u};
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Test 4: Not equal with a difference in the third element (as in the original)
void testNotEqualHEX8Arrays3_DiffThird() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 255u, 253u};
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Test 5: Not equal with a difference in the last element
void testNotEqualHEX8Arrays3_DiffLast() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 254u, 252u};
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Test 6: Should fail when arrays are equal (meta-test for "should abort" case)
void testNotEqualHEX8Arrays3_ShouldFailIfEqual() {
    unsigned char p0[] = {1, 8, 254u, 253u};
    unsigned char p1[] = {1, 8, 254u, 253u};
    // This should throw according to the focal test logic
    focal_not_equal_hex8_arrays3_test_impl(p0, p1, 4);
}

// Entry point
int main() {
    TestHarness harness;

    // Register tests
    harness.addCase("NotEqualHEX8Arrays3 - Basic (diff at index 2)", testNotEqualHEX8Arrays3_Basic);
    harness.addCase("NotEqualHEX8Arrays3 - DiffFirst", testNotEqualHEX8Arrays3_DiffFirst);
    harness.addCase("NotEqualHEX8Arrays3 - DiffSecond", testNotEqualHEX8Arrays3_DiffSecond);
    harness.addCase("NotEqualHEX8Arrays3 - DiffThird (original scenario)", testNotEqualHEX8Arrays3_DiffThird);
    harness.addCase("NotEqualHEX8Arrays3 - DiffLast", testNotEqualHEX8Arrays3_DiffLast);
    // This test is a meta-test: it expects an abort (exception). We treat exception as pass.
    harness.addCase("NotEqualHEX8Arrays3_ShouldFailIfEqual (expect abort)", testNotEqualHEX8Arrays3_ShouldFailIfEqual, true);

    // Run all tests
    return harness.runAll();
}
// Test suite for the focal method CheckGamutCheckFloats located in testcms2.c
// This test suite is implemented in C++11 without using any external testing framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion style suitable for maximizing code coverage.

#include <iostream>
#include <string>
#include <testcms2.h>


// The focal method is implemented in the C source file (testcms2.c).
// We declare it here with C linkage so the C++ test code can call it.
extern "C" int CheckGamutCheckFloats(void);

// Lightweight assertion helper to avoid terminating tests on failure
static bool assert_true(bool condition, const std::string& msg) {
    if (!condition) {
        std::cerr << "ASSERT FAILED: " << msg << std::endl;
        return false;
    }
    return true;
}

// Test 1: Validate that CheckGamutCheckFloats returns 1 on the standard execution path.
// Rationale: The focal method constructs profiles, performs a transform, and
//  checks expected gamut values. Under normal circumstances, it should return 1.
static bool test_CheckGamutCheckFloats_ReturnsOne(void) {
    // Call the focal method. It is implemented in C and linked with the test suite.
    int result = CheckGamutCheckFloats();
    bool ok = assert_true(result == 1,
                          "CheckGamutCheckFloats should return 1 on the standard path");
    if (!ok) {
        std::cerr << "Note: The function returned " << result << " instead of 1." << std::endl;
    }
    return ok;
}

// Test 2: Validate idempotence by invoking CheckGamutCheckFloats multiple times.
// Rationale: Ensure repeated calls do not alter behavior or cause side effects.
// The function itself does not terminate on internal failures (Fail() is a no-op in the test harness),
// so we verify that repeated calls still return 1.
static bool test_CheckGamutCheckFloats_Idempotent(void) {
    const int runs = 2;
    for (int i = 0; i < runs; ++i) {
        int result = CheckGamutCheckFloats();
        if (result != 1) {
            std::cerr << "ASSERT FAILED: CheckGamutCheckFloats() call " << i
                      << " returned " << result << " (expected 1)." << std::endl;
            return false;
        }
    }
    std::cout << "[PASS] CheckGamutCheckFloats is idempotent across " << runs << " invocations." << std::endl;
    return true;
}

int main() {
    std::cout << "Running tests for CheckGamutCheckFloats (testcms2.c)" << std::endl;

    int total_tests = 0;
    int passed_tests = 0;

    // Test 1
    std::cout << "Test 1: Return value on standard path." << std::endl;
    total_tests++;
    if (test_CheckGamutCheckFloats_ReturnsOne()) {
        passed_tests++;
        std::cout << "PASS: Test 1" << std::endl;
    } else {
        std::cerr << "FAIL: Test 1" << std::endl;
    }

    // Test 2
    std::cout << "Test 2: Idempotence across multiple invocations." << std::endl;
    total_tests++;
    if (test_CheckGamutCheckFloats_Idempotent()) {
        passed_tests++;
        std::cout << "PASS: Test 2" << std::endl;
    } else {
        std::cerr << "FAIL: Test 2" << std::endl;
    }

    // Summary
    std::cout << "Summary: " << passed_tests << " / " << total_tests << " tests passed." << std::endl;
    return (passed_tests == total_tests) ? 0 : 1;
}
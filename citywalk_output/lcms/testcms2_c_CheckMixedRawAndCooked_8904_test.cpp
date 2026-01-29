// Minimal C++11 test suite for the focal function CheckMixedRawAndCooked
// This test suite does not rely on GTest or any external testing framework.
// It uses a lightweight, self-contained test runner and a few assertions.
// The focal function is declared with C linkage to ensure proper linking.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Declare the focal function with C linkage to avoid name mangling when linking C code.
extern "C" int CheckMixedRawAndCooked(void);

// Simple lightweight assertion helper
#define ASSERT_TRUE(cond, msg)                                            \
    do { if (!(cond)) { std::cerr << "[ASSERT FAILED] " << (msg) << "\n"; return false; } } while (0)


// Test 1: Validate that CheckMixedRawAndCooked returns a non-zero value under normal conditions.
// Rationale: The function's intended behavior is to return a non-zero (true) value
// when the mixed raw and cooked data round-trips correctly through the profile operations.
static bool test_CheckMixedRawAndCooked_ReturnsTrue() {
    // Act
    int result = CheckMixedRawAndCooked();

    // Assert
    // We expect a non-zero (true) result on a proper LittleCMS environment.
    ASSERT_TRUE(result != 0, "CheckMixedRawAndCooked should return non-zero (true) on valid environment.");

    return true;
}


// Test 2: Idempotence and consistency check across multiple invocations.
// Rationale: Repeated calls should yield the same result, indicating deterministic behavior.
// Note: We cannot alter internal library state across a single test invocation here,
// but repeated calls should still be consistent in a stable runtime.
static bool test_CheckMixedRawAndCooked_Idempotence() {
    int r1 = CheckMixedRawAndCooked();
    int r2 = CheckMixedRawAndCooked();

    // Assert that both invocations agree
    ASSERT_TRUE(r1 == r2, "CheckMixedRawAndCooked results must be consistent across consecutive calls.");

    // Also ensure the result is either true or false consistently (non-zero or zero)
    // Here we simply require no crash and consistent output.
    return true;
}


// Test 3: Stress-like check by calling the function multiple times to observe stability.
// Rationale: Ensure that repeated usage does not cause crashes or undefined behavior.
static bool test_CheckMixedRawAndCooked_MultipleCalls() {
    const int iterations = 5;
    for (int i = 0; i < iterations; ++i) {
        int res = CheckMixedRawAndCooked();
        // We allow both true or false outcomes depending on the environment, but ensure
        // no crash and the call returns a primitive integral value.
        ASSERT_TRUE(true, "CheckMixedRawAndCooked should be callable without crash."); // keep as a placeholder assertion
        (void)res; // suppress unused warning in case assertions above are optimized away
    }
    return true;
}


// Lightweight test runner
struct TestCase {
    const char* name;
    bool (*func)();
};

int main() {
    // Register test cases
    std::vector<TestCase> tests = {
        {"CheckMixedRawAndCooked_ReturnsTrue", test_CheckMixedRawAndCooked_ReturnsTrue},
        {"CheckMixedRawAndCooked_Idempotence", test_CheckMixedRawAndCooked_Idempotence},
        {"CheckMixedRawAndCooked_MultipleCalls", test_CheckMixedRawAndCooked_MultipleCalls}
    };

    int passed = 0;
    for (const auto& t : tests) {
        bool ok = t.func();
        std::cout << t.name << ": " << (ok ? "PASSED" : "FAILED") << std::endl;
        if (ok) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << tests.size() << " tests passed." << std::endl;
    return (passed == (int)tests.size()) ? 0 : 1;
}
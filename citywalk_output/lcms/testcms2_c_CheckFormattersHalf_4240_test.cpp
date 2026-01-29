// Test suite for the focal method: CheckFormattersHalf
// This test suite is written for C++11 (no Google Test) and relies on the existing C test harness
// provided by testcms2.h. It calls the focal function directly and uses a minimal, non-terminating
// assertion approach to maximize code coverage during execution.

#include <iostream>
#include <testcms2.h>
#include <iomanip>


// Import C code dependencies with C linkage to prevent name mangling issues
extern "C" {
}

// Lightweight non-terminating assertion macro for test reporting
#define ASSERT_TRUE(cond, msg) do { if (!(cond)) { \
    std::cerr << "ASSERT_TRUE failed: " << (msg) << " (Condition: " #cond ")" << std::endl; \
    return false; \
} } while(0)

#define ASSERT_EQ(a, b, msg) do { if ((a) != (b)) { \
    std::cerr << "ASSERT_EQ failed: " << (msg) << " (Expected " << (a) << " == " << (b) << ")" << std::endl; \
    return false; \
} } while(0)

// Test 1: Ensure the focal function completes and returns the expected success code (1)
static bool test_CheckFormattersHalf_returns_true() {
    // Rationale:
    // The function CheckFormattersHalf iterates over all 16-bit patterns, converts through
    // half-to-float and back, and validates non-finite round-trips. The expected behavior
    // on a correct implementation is to return 1 after complete iteration without any mismatch.
    int result = CheckFormattersHalf();
    ASSERT_EQ(result, 1, "CheckFormattersHalf should return 1 on success");
    return true;
}

// Test 2: Idempotence - calling the focal function multiple times should not alter behavior.
// This exercises potential static/global state interactions within the test harness.
static bool test_CheckFormattersHalf_idempotent() {
    int first = CheckFormattersHalf();
    int second = CheckFormattersHalf();
    ASSERT_EQ(first, 1, "First call to CheckFormattersHalf should return 1");
    ASSERT_EQ(second, 1, "Second call to CheckFormattersHalf should also return 1");
    return true;
}

// Test 3: Stress-like run - invoking the focal function twice within a single test ensures
// that repeated executions do not cause state corruption and both runs succeed.
static bool test_CheckFormattersHalf_twice() {
    int a = CheckFormattersHalf();
    int b = CheckFormattersHalf();
    ASSERT_TRUE(a == 1 && b == 1, "Both consecutive calls to CheckFormattersHalf must succeed (return 1)");
    return true;
}

int main(int argc, char* argv[]) {
    std::cout << "Starting unit tests for CheckFormattersHalf (no GTest, plain C++11)" << std::endl;

    bool overall_ok = true;

    // Run Test 1
    if (!test_CheckFormattersHalf_returns_true()) {
        std::cerr << "Test 1 failed." << std::endl;
        overall_ok = false;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    // Run Test 2
    if (!test_CheckFormattersHalf_idempotent()) {
        std::cerr << "Test 2 failed." << std::endl;
        overall_ok = false;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    // Run Test 3
    if (!test_CheckFormattersHalf_twice()) {
        std::cerr << "Test 3 failed." << std::endl;
        overall_ok = false;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (overall_ok) {
        std::cout << "ALL TESTS PASSED for CheckFormattersHalf." << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED for CheckFormattersHalf." << std::endl;
        // Return non-zero to indicate test failures
        return 1;
    }
}
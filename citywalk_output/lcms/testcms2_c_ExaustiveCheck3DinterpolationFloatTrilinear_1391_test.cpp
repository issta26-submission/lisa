// Lightweight C++11 unit test suite for the focal function:
// ExaustiveCheck3DinterpolationFloatTrilinear
// The focal function is defined in testcms2.c and is linked with this test.
// We avoid any external testing frameworks (no GTest) and provide a small
// assertion framework suitable for this project.
//
// Notes:
// - We call ExaustiveCheck3DinterpolationFloatTrilinear() and verify it returns 1.
// - We run a couple of simple sanity checks (single and repeated runs) to
//   exercise basic control flow and ensure no crash occurs.
// - All tests are non-terminating in the sense that a failed assertion only
//   records a failure and continues other tests to maximize coverage.

#include <iostream>
#include <testcms2.h>
#include <functional>


// Forward declaration of the focal function from testcms2.c
extern "C" int ExaustiveCheck3DinterpolationFloatTrilinear(void);

// Simple test harness
static int gFailures = 0;

#define TEST_ASSERT(cond, msg)                                 \
    do {                                                        \
        if (!(cond)) {                                          \
            std::cerr << "Assertion failed: " << (msg)         \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl;                             \
            ++gFailures;                                        \
        }                                                       \
    } while (0)

static void Test_ExaustiveCheck3DinterpolationFloatTrilinear_Returns1() {
    // Purpose:
    // - Validate that the focal function completes its exhaustive interpolation
    //   checks and reports success (returns 1) for the expected data.
    // - This covers the main true-path of the function under normal conditions.
    int ret = ExaustiveCheck3DinterpolationFloatTrilinear();
    TEST_ASSERT(ret == 1, "ExaustiveCheck3DinterpolationFloatTrilinear should return 1 on successful run");
}

static void Test_ExaustiveCheck3DinterpolationFloatTrilinear_TwoRunsAreConsistent() {
    // Purpose:
    // - Ensure that consecutive invocations do not crash and return a valid result.
    // - This helps detect potential side effects between runs (static state, etc.).
    int ret1 = ExaustiveCheck3DinterpolationFloatTrilinear();
    int ret2 = ExaustiveCheck3DinterpolationFloatTrilinear();
    TEST_ASSERT(ret1 == 1 && ret2 == 1,
                "Consecutive runs should both return 1; potential side-effects detected if not");
}

static void RunAllTests() {
    // Run individual tests. Each test is self-contained and aims to maximize coverage
    // of the function under test without relying on external test frameworks.
    Test_ExaustiveCheck3DinterpolationFloatTrilinear_Returns1();
    Test_ExaustiveCheck3DinterpolationFloatTrilinear_TwoRunsAreConsistent();
}

int main() {
    RunAllTests();

    if (gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " test(s) failed." << std::endl;
        return gFailures;
    }
}
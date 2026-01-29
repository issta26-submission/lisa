/*
Staged test suite for focal method: from8toDBL(void* dst, const void* src)

Step 1 (Context):
- Core transformation: converts an 8-bit unsigned value to a 64-bit floating-point value by dividing by 255.0.
- Core keywords: from8toDBL, cmsUInt8Number, cmsFloat64Number, dst, src, 255.0, conversion, double.
- Dependencies observed in the focal class dependencies: lcms2_internal.h and the types cmsUInt8Number / cmsFloat64Number.

Step 2 (Test intent):
- Verify correct conversion for representative inputs: 0 -> 0.0, 255 -> 1.0, 128 -> 128/255.0, 1 -> 1/255.0.
- Validate multiple sequential conversions from an array of inputs to ensure consistent behavior across successive calls.
- Ensure non-terminating assertions and a simple, dependency-free test harness compatible with C++11 (no GTest).

Step 3 (Test refinement considerations):
- Use std::fabs with a tight tolerance for double comparisons.
- Access cms* types via the library headers; declare extern "C" for the focal function to avoid linkage issues.
- Centralized reporting of failures with a final summary.
- All tests are executed from main() as required when GTest is not allowed.
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Dependency: provide access to cms* types and the focal function.
// The original code relies on lcms2_internal.h; include it to obtain typedefs like cmsUInt8Number and cmsFloat64Number.

// Ensure we can call the focal function from C linkage (the library is typically C).
extern "C" void from8toDBL(void* dst, const void* src);

// Utility: simple almost-equality check for doubles.
static bool almostEqual(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

// Global test counters to provide a non-terminating, cumulative report.
static int g_totalTests = 0;
static int g_failedTests = 0;

// Test 1: from8toDBL with source = 0 should yield 0.0
void test_from8toDBL_zero() {
    // Candidate Keywords: dst/src, cmsUInt8Number, cmsFloat64Number, 0, 0.0
    cmsFloat64Number dst;
    cmsUInt8Number src = 0;
    from8toDBL(&dst, &src);

    double actual = static_cast<double>(dst);
    double expected = 0.0;

    ++g_totalTests;
    if (!almostEqual(actual, expected)) {
        std::cerr << "[FAIL] test_from8toDBL_zero: expected " << std::setprecision(15)
                  << expected << " but got " << actual << "\n";
        ++g_failedTests;
    } else {
        std::cout << "[PASS] test_from8toDBL_zero\n";
    }
}

// Test 2: from8toDBL with source = 255 should yield 1.0
void test_from8toDBL_max() {
    // Candidate Keywords: 255, 1.0
    cmsFloat64Number dst;
    cmsUInt8Number src = 255;
    from8toDBL(&dst, &src);

    double actual = static_cast<double>(dst);
    double expected = 1.0;

    ++g_totalTests;
    if (!almostEqual(actual, expected)) {
        std::cerr << "[FAIL] test_from8toDBL_max: expected " << std::setprecision(15)
                  << expected << " but got " << actual << "\n";
        ++g_failedTests;
    } else {
        std::cout << "[PASS] test_from8toDBL_max\n";
    }
}

// Test 3: from8toDBL with a mid-value (128) -> 128/255.0
void test_from8toDBL_midvalue() {
    // Candidate Keywords: 128, 128.0/255.0
    cmsFloat64Number dst;
    cmsUInt8Number src = 128;
    from8toDBL(&dst, &src);

    double actual = static_cast<double>(dst);
    double expected = 128.0 / 255.0;

    ++g_totalTests;
    if (!almostEqual(actual, expected)) {
        std::cerr << "[FAIL] test_from8toDBL_midvalue: expected " << std::setprecision(15)
                  << expected << " but got " << actual << "\n";
        ++g_failedTests;
    } else {
        std::cout << "[PASS] test_from8toDBL_midvalue\n";
    }
}

// Test 4: from8toDBL with a low non-zero value (1) -> 1/255.0
void test_from8toDBL_minuscule() {
    // Candidate Keywords: 1, 1.0/255.0
    cmsFloat64Number dst;
    cmsUInt8Number src = 1;
    from8toDBL(&dst, &src);

    double actual = static_cast<double>(dst);
    double expected = 1.0 / 255.0;

    ++g_totalTests;
    if (!almostEqual(actual, expected)) {
        std::cerr << "[FAIL] test_from8toDBL_minuscule: expected " << std::setprecision(15)
                  << expected << " but got " << actual << "\n";
        ++g_failedTests;
    } else {
        std::cout << "[PASS] test_from8toDBL_minuscule\n";
    }
}

// Test 5: Array-based testing to ensure consistent behavior across multiple consecutive calls
void test_from8toDBL_multipleSequential() {
    // Candidate Keywords: array inputs, per-element conversion
    cmsUInt8Number srcs[] = {0, 255, 128, 1, 64, 200};
    const size_t count = sizeof(srcs) / sizeof(srcs[0]);
    cmsFloat64Number dst;

    std::cout << "[RUN] test_from8toDBL_multipleSequential: testing multiple consecutive conversions\n";
    for (size_t i = 0; i < count; ++i) {
        from8toDBL(&dst, &srcs[i]);
        double actual = static_cast<double>(dst);
        double expected = static_cast<double>(srcs[i]) / 255.0;

        ++g_totalTests;
        if (!almostEqual(actual, expected)) {
            std::cerr << "[FAIL] test_from8toDBL_multipleSequential index " << i
                      << ": expected " << std::setprecision(15) << expected
                      << " but got " << actual << "\n";
            ++g_failedTests;
        } else {
            std::cout << "[PASS] test_from8toDBL_multipleSequential index " << i << "\n";
        }
    }
}

int main() {
    // Run individual tests
    test_from8toDBL_zero();
    test_from8toDBL_max();
    test_from8toDBL_midvalue();
    test_from8toDBL_minuscule();
    test_from8toDBL_multipleSequential();

    // Summary
    std::cout << "\nTest Summary: " << g_totalTests << " tests run, "
              << g_failedTests << " failures.\n";

    // Return non-zero if any test failed, to integrate with scripting if desired.
    return (g_failedTests == 0) ? 0 : 1;
}
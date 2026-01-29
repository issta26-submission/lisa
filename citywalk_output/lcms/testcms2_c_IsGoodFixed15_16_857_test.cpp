// Test suite for IsGoodFixed15_16 using a lightweight, non-GTest C++11 approach.
// This suite targets the focal method IsGoodFixed15_16(const char* title, cmsFloat64Number in, cmsFloat64Number out)
// which is defined in testcms2.c/testcms2.h as a wrapper around IsGoodVal(..., FIXED_PRECISION_15_16).
// The test uses a dynamic boundary-finding strategy to adapt to the unknown FIXED_PRECISION_15_16 value at runtime.

#include <iostream>
#include <cmath>
#include <testcms2.h>
#include <limits>


// Domain assumption: The actual project defines cmsBool and cmsFloat64Number.
// To maximize compatibility without pulling in the full header, we declare the C linkage
// and match the common typedefs (cmsBool -> int, cmsFloat64Number -> double).
extern "C" {
    // prototypes for the focal function under test.
    // The exact typedefs in the real project are cmsBool and cmsFloat64Number, typically int and double.
    int IsGoodFixed15_16(const char* title, double in, double out);
}

// Lightweight test harness (non-terminating assertions consistent with the domain knowledge requirement)
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "ASSERT_TRUE FAILED: " << (msg) \
                      << " | Condition evaluated to false at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(false)

#define ASSERT_FALSE(cond, msg) \
    do { \
        if((cond)) { \
            std::cerr << "ASSERT_FALSE FAILED: " << (msg) \
                      << " | Condition evaluated to true at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(false)

#define RUN_TEST(name) \
    do { \
        std::cout << "RUNNING " #name "...\n"; \
        name(); \
    } while(false)

// Candidate Keywords extracted from Step 1 (for traceability in tests)
 // - IsGoodVal
 // - FIXED_PRECISION_15_16
 // - cmsBool / cmsFloat64Number types
 // - title parameter usage
 // - Boundary behavior around tolerance

// Test 1: Exact match should always be considered good (in == out => diff 0 <= max)
void test_IsGoodFixed15_16_ExactMatch() {
    // True condition: in equals out
    double in = 0.0;
    double out = 0.0;
    int res = IsGoodFixed15_16("ExactMatch", in, out);
    ASSERT_TRUE(res != 0, "IsGoodFixed15_16 should return true for identical in/out values (0.0, 0.0)");
}

// Test 2: Boundary detection using dynamic tolerance unknown at compile time.
// We iteratively push the "out" value away from "in" until the function reports false.
// This validates both the inside (<= tolerance) and outside (> tolerance) behavior.
void test_IsGoodFixed15_16_BoundaryDetection() {
    double in = 1234.56789;
    double out = in;
    double lastInside = in;
    const int maxIter = 100;
    int iter = 0;

    // Expand outward from 'in' until the predicate fails.
    // We construct successive outs at increasing distances from 'in'.
    while (iter < maxIter) {
        int good = IsGoodFixed15_16("BoundaryDetection", in, out);
        if (good != 0) {
            lastInside = out;
            // Increase distance from 'in' for the next test
            double distance = (std::abs(out - in) < 1e-300) ? 1.0 : std::abs(out - in);
            // Double the distance from 'in' each iteration to approach the boundary
            distance *= 2.0;
            out = in + ((out >= in) ? distance : -distance);
            ++iter;
        } else {
            // Outside the tolerance boundary reached
            break;
        }
    }

    // Sanity checks: lastInside should be considered good, and the current 'out' should be bad.
    int insideCheck = IsGoodFixed15_16("BoundaryInside", in, lastInside);
    int outsideCheck = IsGoodFixed15_16("BoundaryOutside", in, out);

    ASSERT_TRUE(insideCheck != 0, "Value just inside tolerance should be good");
    ASSERT_FALSE(outsideCheck != 0, "Value just beyond tolerance should be bad");
}

// Test 3: Negative values and symmetric behavior to ensure no sign-related bias.
void test_IsGoodFixed15_16_NegativeAndZero() {
    // In and out identical negative value
    double in1 = -42.0;
    double out1 = -42.0;
    int res1 = IsGoodFixed15_16("NegativeExact", in1, out1);
    ASSERT_TRUE(res1 != 0, "IsGoodFixed15_16 should be true for identical negative values");

    // Zero values
    double in2 = 0.0;
    double out2 = 0.0;
    int res2 = IsGoodFixed15_16("ZeroExact", in2, out2);
    ASSERT_TRUE(res2 != 0, "IsGoodFixed15_16 should be true for zero values");
}

// Test 4: Title variation should not affect the outcome behavior.
// This confirms that the function's logic is tied to numeric tolerance, not title content.
void test_IsGoodFixed15_16_TitleVariation() {
    double in = 5.25;
    double outSame = 5.25;
    double outWithin = 5.25 + 1e-12; // very small perturbation within likely tolerance
    double outBeyond = 5.25 + 1.0;   // larger perturbation, should exceed tolerance

    int r1 = IsGoodFixed15_16("ShortTitle", in, outSame);
    int r2 = IsGoodFixed15_16("Moderate length title to test stability", in, outWithin);
    int r3 = IsGoodFixed15_16("Long title that tests title handling in the IsGoodFixed15_16 wrapper", in, outBeyond);

    ASSERT_TRUE(r1 != 0, "Identical in/out with short title should be good");
    // The within-tolerance case should be good (likely within tolerance)
    ASSERT_TRUE(r2 != 0, "Small perturbation within tolerance should be good");
    // The beyond-tolerance case should be bad
    ASSERT_FALSE(r3 != 0, "Larger perturbation beyond tolerance should be bad");
}

// Runner
int main() {
    std::cout << "Starting test suite for IsGoodFixed15_16...\n";

    RUN_TEST(test_IsGoodFixed15_16_ExactMatch);
    RUN_TEST(test_IsGoodFixed15_16_BoundaryDetection);
    RUN_TEST(test_IsGoodFixed15_16_NegativeAndZero);
    RUN_TEST(test_IsGoodFixed15_16_TitleVariation);

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED: IsGoodFixed15_16\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED: IsGoodFixed15_16\n";
        return 1;
    }
}
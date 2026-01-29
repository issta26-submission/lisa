// Lightweight C++11 test suite for the focal method LineOf2Points in cmssm.c
// This test suite does not depend on Google Test. It uses a small in-house
// test harness that runs multiple tests and reports pass/fail without
// terminating on the first failure (non-terminating assertions).

#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Import CMS/LittleCMS core types and the target function with C linkage.
// The project is C-based; expose C declarations to C++.
extern "C" {
}

// The focal function is defined in cmssm.c. Expose its signature for linkage.
extern "C" void LineOf2Points(cmsLine* line, cmsVEC3* a, cmsVEC3* b);

// Small tolerance for floating point comparisons
static bool almost_equal(double x, double y, double tol = 1e-9) {
    return std::fabs(x - y) <= tol;
}

// Convenience: compare two cmsVEC3 against expected components
static bool vec3_equal(const cmsVEC3& v, const double exp[3], double tol = 1e-9) {
    return almost_equal(v.n[0], exp[0], tol) &&
           almost_equal(v.n[1], exp[1], tol) &&
           almost_equal(v.n[2], exp[2], tol);
}

// Test 1: Basic positive coordinates
// Intent: Ensure LineOf2Points copies a into line.a and computes line.u as (b - a).
static bool test_LineOf2Points_basic_positive() {
    // Setup input vectors
    cmsVEC3 a;
    cmsVEC3 b;
    a.n[0] = 1.0; a.n[1] = 2.0; a.n[2] = 3.0;
    b.n[0] = 4.0; b.n[1] = 6.0; b.n[2] = 8.0;

    cmsLine line;

    // Call the focal function (C linkage)
    LineOf2Points(&line, &a, &b);

    // Expected outputs
    double expectedA[3] = {1.0, 2.0, 3.0};
    double expectedU[3] = {3.0, 4.0, 5.0}; // b - a

    bool ok = vec3_equal(line.a, expectedA) &&
              vec3_equal(line.u, expectedU);

    if (!ok) {
        std::cout << "[FAIL] test_LineOf2Points_basic_positive: "
                  << "line.a = {" << line.a.n[0] << ", " << line.a.n[1] << ", " << line.a.n[2]
                  << "}, line.u = {" << line.u.n[0] << ", " << line.u.n[1] << ", " << line.u.n[2]
                  << "}\n";
    }

    return ok;
}

// Test 2: Mixed sign and non-integer values
// Intent: Validate correct copying and subtraction with negative and fractional components.
static bool test_LineOf2Points_basic_negative_and_fractional() {
    cmsVEC3 a;
    cmsVEC3 b;
    a.n[0] = -1.5; a.n[1] = 2.25; a.n[2] = -3.75;
    b.n[0] = 4.25; b.n[1] = -1.125; b.n[2] = 0.0;

    cmsLine line;
    LineOf2Points(&line, &a, &b);

    double expectedA[3] = {-1.5, 2.25, -3.75};
    double expectedU[3] = {5.75, -3.375, 3.75}; // b - a

    bool ok = vec3_equal(line.a, expectedA) &&
              vec3_equal(line.u, expectedU);

    if (!ok) {
        std::cout << "[FAIL] test_LineOf2Points_basic_negative_and_fractional: "
                  << "line.a = {" << line.a.n[0] << ", " << line.a.n[1] << ", " << line.a.n[2]
                  << "}, line.u = {" << line.u.n[0] << ", " << line.u.n[1] << ", " << line.u.n[2]
                  << "}\n";
    }

    return ok;
}

// Test 3: Zero difference (b == a)
// Intent: Ensure u becomes a zero vector when the two points coincide.
static bool test_LineOf2Points_zero_difference() {
    cmsVEC3 a;
    cmsVEC3 b;
    a.n[0] = 0.0; a.n[1] = 0.0; a.n[2] = 0.0;
    b.n[0] = 0.0; b.n[1] = 0.0; b.n[2] = 0.0;

    cmsLine line;
    LineOf2Points(&line, &a, &b);

    double expectedA[3] = {0.0, 0.0, 0.0};
    double expectedU[3] = {0.0, 0.0, 0.0};

    bool ok = vec3_equal(line.a, expectedA) &&
              vec3_equal(line.u, expectedU);

    if (!ok) {
        std::cout << "[FAIL] test_LineOf2Points_zero_difference: "
                  << "line.a = {" << line.a.n[0] << ", " << line.a.n[1] << ", " << line.a.n[2]
                  << "}, line.u = {" << line.u.n[0] << ", " << line.u.n[1] << ", " << line.u.n[2]
                  << "}\n";
    }

    return ok;
}

// Test 4: Large magnitude values
// Intent: Confirm stability with large numbers to avoid overflow/precision issues.
static bool test_LineOf2Points_large_values() {
    cmsVEC3 a;
    cmsVEC3 b;
    a.n[0] = 1e6; a.n[1] = -2e6; a.n[2] = 3e6;
    b.n[0] = -1e6; b.n[1] = 2e6; a.n[2] = -4e6; // Note: a.n[2] assigned above; keep consistent
    // Correcting to consistent values:
    b.n[0] = -1e6; b.n[1] = 2e6; b.n[2] = -4e6;

    cmsLine line;
    LineOf2Points(&line, &a, &b);

    double expectedA[3] = {1e6, -2e6, 3e6};
    double expectedU[3] = {-2e6, 4e6, -7e6}; // b - a

    bool ok = vec3_equal(line.a, expectedA) &&
              vec3_equal(line.u, expectedU);

    if (!ok) {
        std::cout << "[FAIL] test_LineOf2Points_large_values: "
                  << "line.a = {" << line.a.n[0] << ", " << line.a.n[1] << ", " << line.a.n[2]
                  << "}, line.u = {" << line.u.n[0] << ", " << line.u.n[1] << ", " << line.u.n[2]
                  << "}\n";
    }

    return ok;
}

// Simple test harness
int main() {
    int total = 0;
    int passed = 0;

    struct TestEntry {
        const char* name;
        bool (*func)();
    };

    TestEntry tests[] = {
        {"LineOf2Points_basic_positive", test_LineOf2Points_basic_positive},
        {"LineOf2Points_negative_and_fractional", test_LineOf2Points_basic_negative_and_fractional},
        {"LineOf2Points_zero_difference", test_LineOf2Points_zero_difference},
        {"LineOf2Points_large_values", test_LineOf2Points_large_values},
    };

    const int kNumTests = sizeof(tests) / sizeof(tests[0]);

    std::cout << "Running LineOf2Points test suite with " << kNumTests << " tests...\n";

    for (int i = 0; i < kNumTests; ++i) {
        total++;
        bool result = tests[i].func();
        if (result) {
            passed++;
            std::cout << "[PASS] " << tests[i].name << "\n";
        } else {
            std::cout << "[FAIL] " << tests[i].name << "\n";
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Non-terminating: return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
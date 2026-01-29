// Minimal C++11 test suite for cmsFloat64Number compute_n(cmsCIECAM02* pMod)
// This test does not rely on Google Test. It uses a small, non-terminating assertion style.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmath>


// Domain-specific assumption: cmsFloat64Number is a typedef for double.
typedef double cmsFloat64Number;

// Lightweight local definition matching the used fields in compute_n
typedef struct {
    cmsFloat64Number XYZ[3];
} cmsCIEXYZ;

// The focal class structure. We only model the fields required by compute_n:
// Yb and adoptedWhite.XYZ[1]. Other fields from the real library are ignored for tests.
typedef struct {
    cmsFloat64Number Yb;
    cmsCIEXYZ adoptedWhite;
} cmsCIECAM02;

// Declare the function under test with C linkage.
// The real implementation resides in cmscam02.c (part of the project under test).
extern "C" cmsFloat64Number compute_n(cmsCIECAM02* pMod);

// Simple, non-terminating assertion helper
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[ASSERT FAIL] " << (msg) << " (in " << __FUNCTION__ << ")" << std::endl; \
        all_pass = false; \
        g_failures++; \
    } \
} while(0)

static int g_failures = 0; // total failure count across all tests

// Helper to build the minimal cmsCIECAM02 object for compute_n
static inline cmsCIECAM02 makeMod(double Yb, double xyz1) {
    cmsCIECAM02 m;
    m.Yb = Yb;
    m.adoptedWhite.XYZ[0] = 0.0;
    m.adoptedWhite.XYZ[1] = xyz1; // This is the value used by compute_n
    m.adoptedWhite.XYZ[2] = 0.0;
    return m;
}

// Test 1: Basic valid input. Yb / adoptedWhite.XYZ[1] => 2.0
void test_compute_n_basic() {
    bool all_pass = true;
    // Arrange
    cmsCIECAM02 mod = makeMod(20.0, 10.0);
    // Act
    cmsFloat64Number res = compute_n(&mod);
    // Assert
    TEST_ASSERT(res == 2.0, "compute_n should return 2.0 for Yb=20, XYZ[1]=10");
    // Report
    if(all_pass) {
        std::cout << "[PASS] test_compute_n_basic" << std::endl;
    } else {
        std::cout << "[FAIL] test_compute_n_basic" << std::endl;
    }
}

// Test 2: Denominator zero with positive numerator => +Inf
void test_compute_n_zero_den_positive() {
    bool all_pass = true;
    cmsCIECAM02 mod = makeMod(20.0, 0.0); // Yb / 0.0 -> +Inf
    cmsFloat64Number res = compute_n(&mod);
    TEST_ASSERT(std::isinf(res) && (res > 0.0), "compute_n should return +Inf when denominator is 0 and numerator positive");
    if(all_pass) {
        std::cout << "[PASS] test_compute_n_zero_den_positive" << std::endl;
    } else {
        std::cout << "[FAIL] test_compute_n_zero_den_positive" << std::endl;
    }
}

// Test 3: Denominator zero with negative numerator => -Inf
void test_compute_n_zero_den_negative() {
    bool all_pass = true;
    cmsCIECAM02 mod = makeMod(-25.0, 0.0); // -25 / 0.0 -> -Inf
    cmsFloat64Number res = compute_n(&mod);
    TEST_ASSERT(std::isinf(res) && (res < 0.0), "compute_n should return -Inf when denominator is 0 and numerator negative");
    if(all_pass) {
        std::cout << "[PASS] test_compute_n_zero_den_negative" << std::endl;
    } else {
        std::cout << "[FAIL] test_compute_n_zero_den_negative" << std::endl;
    }
}

// Test 4: Denominator zero with zero numerator => NaN
void test_compute_n_zero_over_zero() {
    bool all_pass = true;
    cmsCIECAM02 mod = makeMod(0.0, 0.0); // 0 / 0 -> NaN
    cmsFloat64Number res = compute_n(&mod);
    TEST_ASSERT(std::isnan(res), "compute_n should return NaN for 0/0");
    if(all_pass) {
        std::cout << "[PASS] test_compute_n_zero_over_zero" << std::endl;
    } else {
        std::cout << "[FAIL] test_compute_n_zero_over_zero" << std::endl;
    }
}

// Test 5: Negative numerator with positive denominator
void test_compute_n_negative_numerator() {
    bool all_pass = true;
    cmsCIECAM02 mod = makeMod(-40.0, 10.0); // -40 / 10 -> -4
    cmsFloat64Number res = compute_n(&mod);
    TEST_ASSERT(res == -4.0, "compute_n should return -4.0 for Yb=-40, XYZ[1]=10");
    if(all_pass) {
        std::cout << "[PASS] test_compute_n_negative_numerator" << std::endl;
    } else {
        std::cout << "[FAIL] test_compute_n_negative_numerator" << std::endl;
    }
}

// Main runner
int main() {
    std::cout << "Starting compute_n unit tests (C++11 harness)" << std::endl;

    test_compute_n_basic();
    test_compute_n_zero_den_positive();
    test_compute_n_zero_den_negative();
    test_compute_n_zero_over_zero();
    test_compute_n_negative_numerator();

    std::cout << "Total tests executed: 5" << std::endl;
    std::cout << "Total assertions failed: " << g_failures << std::endl;

    // Return non-zero if any assertion failed
    return (g_failures == 0) ? 0 : 1;
}
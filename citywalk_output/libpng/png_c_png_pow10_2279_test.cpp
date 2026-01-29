// Lightweight C++11 test suite for png_pow10(int power) without using GTest.
// This test suite focuses on correctness and code-path coverage by exercising
// negative, zero, and positive exponents, including boundary behavior.

#include <cmath>
#include <cfloat>
#include <iostream>
#include <pngpriv.h>


// Declare the focal C function with C linkage to link properly from C++ test.
extern "C" double png_pow10(int power);

// Simple near-equality check for doubles with a relative tolerance.
static bool almost_equal(double a, double b, double tol = 1e-12) {
    double diff = std::fabs(a - b);
    double scale = std::fabs(b);
    if (scale < 1.0) scale = 1.0;
    return diff <= tol * scale;
}

// Test 1: power = 0 -> should return 1
// Branch coverage: the "power > 0" block is skipped, and d remains 1.
static bool test_zero_power() {
    int power = 0;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_zero_power failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 2: power = 1 -> 10
// Branch coverage: enter the exponentiation by squaring path.
static bool test_power_one() {
    int power = 1;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_power_one failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 3: power = 2 -> 100
static bool test_power_two() {
    int power = 2;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_power_two failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 4: power = 3 -> 1000
static bool test_power_three() {
    int power = 3;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_power_three failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 5: power = -1 -> 0.1 (reciprocal path)
static bool test_negative_one() {
    int power = -1;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_negative_one failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 6: power = -2 -> 0.01
static bool test_negative_two() {
    int power = -2;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_negative_two failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Test 7: boundary underflow for negative exponents: power < DBL_MIN_10_EXP should return 0
static bool test_negative_underflow_boundary() {
    int min_exp10 = DBL_MIN_10_EXP; // typically -307 on many platforms
    int power = min_exp10 - 1;      // strictly less than minimum
    double got = png_pow10(power);
    if (got != 0.0) {
        std::cerr << "test_negative_underflow_boundary failed: got " << got
                  << " expected 0.0 for power " << power << std::endl;
        return false;
    }
    return true;
}

// Test 8: power = 6 -> 1e6
static bool test_power_six() {
    int power = 6;
    double expected = std::pow(10.0, static_cast<double>(power));
    double got = png_pow10(power);
    if (!almost_equal(got, expected)) {
        std::cerr << "test_power_six failed: got " << got
                  << " expected " << expected << std::endl;
        return false;
    }
    return true;
}

// Simple test runner
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        total++;
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    // Run all tests
    run("test_zero_power", test_zero_power());
    run("test_power_one", test_power_one());
    run("test_power_two", test_power_two());
    run("test_power_three", test_power_three());
    run("test_negative_one", test_negative_one());
    run("test_negative_two", test_negative_two());
    run("test_negative_underflow_boundary", test_negative_underflow_boundary());
    run("test_power_six", test_power_six());

    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
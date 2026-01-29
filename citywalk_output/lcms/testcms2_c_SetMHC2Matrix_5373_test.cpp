/*
Unit test suite for the focal method:
  SetMHC2Matrix(cmsFloat64Number XYZ2XYZmatrix[3][4])

Context:
- The method writes a fixed 3x4 matrix into the provided array.
- Values written:
  Row 0: 0.5, 0.1, 0.1, 0.0
  Row 1: 0.0, 1.0, 0.0, 0.0
  Row 2: 0.3, 0.2, 0.4, 0.0
- This test suite uses a lightweight, non-Google-Test framework approach:
  - Pure C++11 code
  - Self-contained test harness with non-terminating expectations
  - Tests are invoked from main() as required when gtest is not allowed

Notes:
- The test includes testcms2.h to access the CMS data types and the focal function.
- testcms2.h in this project is a C header; we wrap its inclusion in extern "C" to ensure correct linkage when compiled as C++.
- We verify all 12 elements using a tolerance to account for floating-point representation (EPS = 1e-9).
- The tests are designed to be straightforward and deterministic.

Candidate Keywords (from Step 1 understanding):
- XYZ2XYZmatrix, SetMHC2Matrix, cmsFloat64Number, 3x4 matrix, fixed coefficients (0.5, 0.1, 0.1, 0.0 etc), matrix overwrite behavior, non-terminating assertions, testCMS2 dependency, C linkage, ECM tolerance, public test entrypoint.

Dependencies:
- testcms2.h located in the project (path adjustable if needed).

Compile instruction (example):
- g++ -std=c++11 -I. test_SetMHC2Matrix.cpp -o test_SetMHC2Matrix
- Ensure testcms2.h is accessible via -I or same directory.

*/

#include <iostream>
#include <cmath>
#include <testcms2.h>
#include <limits>


// Import C dependencies (function under test and related typedefs)
extern "C" {
}

// Minimal non-terminating test harness
// We count passes/fails but do not abort on first failure, to maximize coverage.

static const double EPS = 1e-9;

// Helper: compare two cmsFloat64Number values with tolerance
static bool almost_equal(cmsFloat64Number a, cmsFloat64Number b, double eps = EPS) {
    // Cast to double for a robust comparison
    return std::abs(static_cast<double>(a) - static_cast<double>(b)) <= eps;
}

// Print a standardized test header
static void print_test_header(const char* test_name) {
    std::cout << "[TEST] " << test_name << std::endl;
}

// Test 1: Basic correctness - the matrix after SetMHC2Matrix matches the exact expected values
static bool test_SetMHC2Matrix_basic_values() {
    print_test_header("test_SetMHC2Matrix_basic_values");

    cmsFloat64Number mat[3][4];

    // Execute the focal method
    SetMHC2Matrix(mat);

    // Expected values
    const cmsFloat64Number exp00 = 0.5, exp01 = 0.1, exp02 = 0.1, exp03 = 0.0;
    const cmsFloat64Number exp10 = 0.0, exp11 = 1.0, exp12 = 0.0, exp13 = 0.0;
    const cmsFloat64Number exp20 = 0.3, exp21 = 0.2, exp22 = 0.4, exp23 = 0.0;

    bool ok = true;

    ok &= almost_equal(mat[0][0], exp00);
    ok &= almost_equal(mat[0][1], exp01);
    ok &= almost_equal(mat[0][2], exp02);
    ok &= almost_equal(mat[0][3], exp03);

    ok &= almost_equal(mat[1][0], exp10);
    ok &= almost_equal(mat[1][1], exp11);
    ok &= almost_equal(mat[1][2], exp12);
    ok &= almost_equal(mat[1][3], exp13);

    ok &= almost_equal(mat[2][0], exp20);
    ok &= almost_equal(mat[2][1], exp21);
    ok &= almost_equal(mat[2][2], exp22);
    ok &= almost_equal(mat[2][3], exp23);

    if (!ok) {
        std::cout << "  Failure details:\n";
        std::cout << "  Row0: [" << static_cast<double>(mat[0][0]) << ", "
                  << static_cast<double>(mat[0][1]) << ", "
                  << static_cast<double>(mat[0][2]) << ", "
                  << static_cast<double>(mat[0][3]) << "]\n";
        std::cout << "  Row1: [" << static_cast<double>(mat[1][0]) << ", "
                  << static_cast<double>(mat[1][1]) << ", "
                  << static_cast<double>(mat[1][2]) << ", "
                  << static_cast<double>(mat[1][3]) << "]\n";
        std::cout << "  Row2: [" << static_cast<double>(mat[2][0]) << ", "
                  << static_cast<double>(mat[2][1]) << ", "
                  << static_cast<double>(mat[2][2]) << ", "
                  << static_cast<double>(mat[2][3]) << "]\n";
    }

    return ok;
}

// Test 2: Repeated invocation stability - ensure subsequent calls always produce the same output
static bool test_SetMHC2Matrix_repeated_calls() {
    print_test_header("test_SetMHC2Matrix_repeated_calls");

    cmsFloat64Number mat1[3][4];
    cmsFloat64Number mat2[3][4];

    // First call
    SetMHC2Matrix(mat1);

    // Second call (with potentially different pre-initialization state)
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            mat2[i][j] = static_cast<cmsFloat64Number>(-1.0 * (i * 10 + j)); // arbitrary non-default values

    SetMHC2Matrix(mat2);

    // Compare both results against the canonical expected matrix
    const cmsFloat64Number exp00 = 0.5, exp01 = 0.1, exp02 = 0.1, exp03 = 0.0;
    const cmsFloat64Number exp10 = 0.0, exp11 = 1.0, exp12 = 0.0, exp13 = 0.0;
    const cmsFloat64Number exp20 = 0.3, exp21 = 0.2, exp22 = 0.4, exp23 = 0.0;

    bool ok = true;
    ok &= almost_equal(mat2[0][0], exp00);
    ok &= almost_equal(mat2[0][1], exp01);
    ok &= almost_equal(mat2[0][2], exp02);
    ok &= almost_equal(mat2[0][3], exp03);

    ok &= almost_equal(mat2[1][0], exp10);
    ok &= almost_equal(mat2[1][1], exp11);
    ok &= almost_equal(mat2[1][2], exp12);
    ok &= almost_equal(mat2[1][3], exp13);

    ok &= almost_equal(mat2[2][0], exp20);
    ok &= almost_equal(mat2[2][1], exp21);
    ok &= almost_equal(mat2[2][2], exp22);
    ok &= almost_equal(mat2[2][3], exp23);

    // Additionally ensure that mat1 equals the canonical matrix as well
    ok &= almost_equal(mat1[0][0], exp00);
    ok &= almost_equal(mat1[0][1], exp01);
    ok &= almost_equal(mat1[0][2], exp02);
    ok &= almost_equal(mat1[0][3], exp03);

    ok &= almost_equal(mat1[1][0], exp10);
    ok &= almost_equal(mat1[1][1], exp11);
    ok &= almost_equal(mat1[1][2], exp12);
    ok &= almost_equal(mat1[1][3], exp13);

    ok &= almost_equal(mat1[2][0], exp20);
    ok &= almost_equal(mat1[2][1], exp21);
    ok &= almost_equal(mat1[2][2], exp22);
    ok &= almost_equal(mat1[2][3], exp23);

    if (!ok) {
        std::cout << "  Failure details observed in repeated calls:\n";
        std::cout << "  mat1 Row0: [" << static_cast<double>(mat1[0][0]) << ", "
                  << static_cast<double>(mat1[0][1]) << ", "
                  << static_cast<double>(mat1[0][2]) << ", "
                  << static_cast<double>(mat1[0][3]) << "]\n";
        std::cout << "  mat2 Row0: [" << static_cast<double>(mat2[0][0]) << ", "
                  << static_cast<double>(mat2[0][1]) << ", "
                  << static_cast<double>(mat2[0][2]) << ", "
                  << static_cast<double>(mat2[0][3]) << "]\n";
    }

    return ok;
}

// Test 3: Ensure correct values are written to the 4th column (column index 3) as per specification
static bool test_SetMHC2Matrix_column4_is_zero() {
    print_test_header("test_SetMHC2Matrix_column4_is_zero");

    cmsFloat64Number mat[3][4];
    SetMHC2Matrix(mat);

    bool ok = true;
    // 4th column should be 0.0 in all rows
    ok &= almost_equal(mat[0][3], 0.0);
    ok &= almost_equal(mat[1][3], 0.0);
    ok &= almost_equal(mat[2][3], 0.0);

    if (!ok) {
        std::cout << "  Unexpected non-zero values in 4th column:\n";
        std::cout << "  Col3: [" << static_cast<double>(mat[0][3]) << ", "
                  << static_cast<double>(mat[1][3]) << ", "
                  << static_cast<double>(mat[2][3]) << "]\n";
    }

    return ok;
}

// Entry point: run all tests and report overall status
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](bool (*test_func)(), const char* name) {
        ++total;
        bool res = test_func();
        if (res) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
        return res;
    };

    // Run tests (true/false assertions are non-terminating within tests)
    bool r1 = run(reinterpret_cast<bool (*)()>(&test_SetMHC2Matrix_basic_values), "test_SetMHC2Matrix_basic_values");
    bool r2 = run(reinterpret_cast<bool (*)()>(&test_SetMHC2Matrix_repeated_calls), "test_SetMHC2Matrix_repeated_calls");
    bool r3 = run(reinterpret_cast<bool (*)()>(&test_SetMHC2Matrix_column4_is_zero), "test_SetMHC2Matrix_column4_is_zero");

    // Summary
    std::cout << "\nTest Summary: " << passed << " passed, " << (total - passed) << " failed, out of "
              << total << " tests.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}
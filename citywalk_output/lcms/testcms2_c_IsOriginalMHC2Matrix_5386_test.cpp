/*
   C++11 test harness for the focal method:
   cmsBool IsOriginalMHC2Matrix(cmsFloat64Number XYZ2XYZmatrix[3][4])

   Notes:
   - Tests call the C functions SetMHC2Matrix and IsOriginalMHC2Matrix
     and use CloseEnough semantics via the called function.
   - No GoogleTest/GMock is used. A lightweight, non-terminating test runner is implemented.
   - All tests run from main() and print results; test progression does not abort on failure.
   - The tests assume TRUE is non-zero and FALSE is zero (as is common with cmsBool).
*/

#include <iostream>
#include <testcms2.h>
#include <iomanip>


// Forward declarations for the C API (provided in testcms2.c, included as part of the project dependencies).
// We declare them with C linkage to be callable from C++.
extern "C" {
    // Typedefs matching the CMS types used in the focal method
    typedef int cmsBool;
    typedef double cmsFloat64Number;

    // The function under test
    cmsBool IsOriginalMHC2Matrix(cmsFloat64Number XYZ2XYZmatrix[3][4]);

    // Dependencies used by the focal method
    void SetMHC2Matrix(cmsFloat64Number XYZ2XYZmatrix[3][4]);
    cmsBool CloseEnough(cmsFloat64Number a, cmsFloat64Number b);
}

// Lightweight test harness helpers
static inline bool to_bool(int v) { return v != 0; }

int main() {
    int total = 0;
    int passed = 0;

    auto log_result = [&](const char* test_name, bool expected_true, bool actual_true) {
        ++total;
        if (actual_true == expected_true) {
            ++passed;
            std::cout << "[PASS] " << test_name << "\n";
        } else {
            std::cout << "[FAIL] " << test_name << " (expected " 
                      << (expected_true ? "TRUE" : "FALSE")
                      << ", got " << (actual_true ? "TRUE" : "FALSE") << ")\n";
        }
    };

    // Test 1: Exact match with the internally-set MHC2 matrix should yield TRUE
    // Rationale: After populating m with SetMHC2Matrix, IsOriginalMHC2Matrix should
    // return TRUE when given the exact same matrix.
    {
        cmsFloat64Number m[3][4];
        SetMHC2Matrix(m);
        int res = IsOriginalMHC2Matrix(m);
        bool ok = to_bool(res);
        log_result("IsOriginalMHC2Matrix should return TRUE for the exact MHC2 matrix", true, ok);
    }

    // Test 2: If any single element is perturbed significantly, the result should be FALSE
    // Rationale: CloseEnough should detect a mismatch beyond its tolerance.
    {
        cmsFloat64Number m[3][4];
        SetMHC2Matrix(m);

        // Create a perturbation on one element
        cmsFloat64Number t[3][4];
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; ++j)
                t[i][j] = m[i][j];
        t[0][0] += 1.0; // large perturbation to ensure a mismatch

        int res = IsOriginalMHC2Matrix(t);
        bool ok = (res != 0);
        log_result("IsOriginalMHC2Matrix should return FALSE when one element is perturbed significantly", false, ok);
    }

    // Test 3: Zero matrix input should normally not match the MHC2 matrix
    // Rationale: A clearly different matrix should yield FALSE.
    {
        cmsFloat64Number z[3][4] = { {0} }; // All zeros
        int res = IsOriginalMHC2Matrix(z);
        bool ok = (res != 0);
        log_result("IsOriginalMHC2Matrix should return FALSE for zero matrix input", false, ok);
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";

    // Return 0 to indicate the harness ran to completion (non-terminating; does not abort on failures)
    return 0;
}
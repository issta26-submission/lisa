/*
Unit Test Suite for png_get_user_width_max

Step 1 (Program Understanding and Candidate Keywords):
- Core dependent components: png_ptr (pointer to png_struct), user_width_max (field inside png_struct),
  NULL/pointer-branch (null vs non-null cases), return value (png_uint_32).
- Target function: png_get_user_width_max(png_const_structrp png_ptr)
- Expected behavior: return png_ptr->user_width_max when png_ptr is non-NULL; otherwise return 0.

Step 2 (Test Case Generation):
- Tests cover:
  - Non-null pointer with zero user_width_max -> expect 0
  - Non-null pointer with a specific non-zero value -> expect that exact value
  - Null pointer -> expect 0

Step 3 (Test Case Refinement):
- Use only C++11 standard library, no GTest or Google Mock.
- Access static/dependent types via the library headers.
- Use non-terminating, self-contained tests and a minimal harness.
- All tests are designed to compile against the existing C code (pngget.c/pngpriv.h).

Note: This test suite directly manipulates the internal png_struct structure by leveraging the internal header definitions (pngpriv.h). It validates the exact logical branches in png_get_user_width_max.
*/

// Include standard libraries for I/O and fixed-width types
#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Include internal library headers to obtain required type definitions
extern "C" {
}

// Declare the focal C function with C linkage for correct linking from C++
extern "C" png_uint_32 png_get_user_width_max(png_const_structrp png_ptr);

namespace TestPngGetUserWidthMax {

// Test 1: Non-null pointer with user_width_max = 0 should return 0
bool test_non_null_ptr_zero() {
    // Create a png_struct instance and set its internal field
    png_struct s;
    // Initialize field to zero to verify direct propagation through the function
    s.user_width_max = 0u;

    // Cast to the function's expected opaque const pointer type
    png_const_structrp p = (png_const_structrp)&s;

    // Invoke the focal function
    png_uint_32 result = png_get_user_width_max(p);

    // Explain the test outcome
    if (result != 0u) {
        std::cerr << "[Test1] Failed: Expected 0, got " << result << std::endl;
        return false;
    }
    return true;
}

// Test 2: Non-null pointer with a specific non-zero value should return that value
bool test_non_null_ptr_specific_value() {
    png_struct s;
    s.user_width_max = 12345u;

    png_const_structrp p = (png_const_structrp)&s;
    png_uint_32 result = png_get_user_width_max(p);

    if (result != 12345u) {
        std::cerr << "[Test2] Failed: Expected 12345, got " << result << std::endl;
        return false;
    }
    return true;
}

// Test 3: Null pointer should return 0
bool test_null_ptr() {
    // Pass a null pointer to exercise the false branch
    png_uint_32 result = png_get_user_width_max((png_const_structrp)NULL);

    if (result != 0u) {
        std::cerr << "[Test3] Failed: Expected 0 for null input, got " << result << std::endl;
        return false;
    }
    return true;
}

// Convenience runner for all tests
bool run_all() {
    bool ok = true;
    ok &= test_non_null_ptr_zero();
    ok &= test_non_null_ptr_specific_value();
    ok &= test_null_ptr();
    return ok;
}

} // namespace TestPngGetUserWidthMax

int main() {
    bool all_passed = TestPngGetUserWidthMax::run_all();

    if (all_passed) {
        std::cout << "All tests passed for png_get_user_width_max." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED for png_get_user_width_max." << std::endl;
        return 1;
    }
}
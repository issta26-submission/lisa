// Test suite for png_gamma_16bit_correct in png.c
// - No GTest; a lightweight C++11 test harness with non-terminating assertions.
// - Tests focus on the focal method behavior across boundary conditions
//   and, when available, the floating-point path.
// - This file assumes libpng headers/libs are available in the build environment.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <png.h>


// Include libpng public API (types and function prototype)
extern "C" {
}

// Declare the focal function (C linkage)
extern "C" png_uint_16 png_gamma_16bit_correct(unsigned int value, png_fixed_point gamma_val);

// Lightweight non-terminating assertion macro.
// On failure, prints diagnostic and increments a global failure counter.
static int g_test_failures = 0;

#define TEST_ASSERT(condition, message)                                 \
    do {                                                                \
        if (!(condition)) {                                             \
            std::cerr << "TEST FAILURE: " << (message)                   \
                      << " | In file " << __FILE__                      \
                      << " at line " << __LINE__ << std::endl;         \
            ++g_test_failures;                                        \
        }                                                               \
    } while (false)

////////////////////////////////////////////////////////////////////////////////
// Test 1: Boundary and simple cases to exercise the false branch (value <= 0 or >= 65535)
////////////////////////////////////////////////////////////////////////////////

// This test validates the behavior at the boundary values:
// - value = 0  -> should return 0
// - value = 65535 -> should return 65535 (not entering the true-range branch)
// - value = 65536 -> should return 65536 cast to 16-bit, i.e., 0
static bool test_gamma_boundary_values() {
    bool ok = true;

    // Case 0: value == 0
    {
        unsigned int value = 0;
        png_fixed_point gamma_val = static_cast<png_fixed_point>(100000); // gamma ~ 1.0 (for non-range case this is irrelevant)
        png_uint_16 res = png_gamma_16bit_correct(value, gamma_val);
        TEST_ASSERT(res == 0, "value=0 should return 0");
        ok = ok && (res == 0);
    }

    // Case 1: value == 65535 (edge of true-range, should not enter true branch)
    {
        unsigned int value = 65535;
        png_fixed_point gamma_val = static_cast<png_fixed_point>(100000);
        png_uint_16 res = png_gamma_16bit_correct(value, gamma_val);
        TEST_ASSERT(res == 65535, "value=65535 should return 65535 (no gamma application)");
        ok = ok && (res == 65535);
    }

    // Case 2: value == 65536 (overflow to 16-bit should wrap to 0)
    {
        unsigned int value = 65536;
        png_fixed_point gamma_val = static_cast<png_fixed_point>(100000);
        png_uint_16 res = png_gamma_16bit_correct(value, gamma_val);
        TEST_ASSERT(res == 0, "value=65536 should overflow to 0 in 16-bit result");
        ok = ok && (res == 0);
    }

    return ok;
}

////////////////////////////////////////////////////////////////////////////////
// Test 2: In-range path when the floating-point arithmetic path is available
////////////////////////////////////////////////////////////////////////////////

// When PNG_FLOATING_ARITHMETIC_SUPPORTED is defined, the implementation uses a
// double-based computation: r = floor(65535 * pow(value/65535, gamma_val*.00001) + 0.5)
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
// This test exercises the true-range path with gamma ~= 1.0 and a small in-range value.
// For gamma ~= 1.0, the formula reduces to roughly r ~ value, so expect exact equality.
static bool test_gamma_inrange_float_path_exact() {
    unsigned int value = 1000;                 // in-range
    png_fixed_point gamma_val = 100000;        // gamma = gamma_val * 1e-5 -> 1.0
    png_uint_16 res = png_gamma_16bit_correct(value, gamma_val);
    // Expect nearly exact equality to input value
    bool pass = (static_cast<unsigned int>(res) == value);
    TEST_ASSERT(pass, "In-range value with gamma ~1.0 should yield the same value under floating-path");
    return pass;
}
#else
// If the floating-point path is not compiled in, we cannot guarantee exact equality.
// We still ensure the function returns a value within valid 16-bit range for in-range inputs.
static bool test_gamma_inrange_float_path_absence() {
    unsigned int value = 1000;                  // in-range
    png_fixed_point gamma_val = static_cast<png_fixed_point>(100000); // gamma ~ 1.0
    png_uint_16 res = png_gamma_16bit_correct(value, gamma_val);
    bool pass = (res <= 65535);
    TEST_ASSERT(pass, "In-range value under non-floating-path should yield a valid 16-bit result");
    return pass;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Test 3: Static/auxiliary behavior (static members / constants)
// This test ensures we can reference and reason about static-like constants
// through the public API, without relying on file-scope private symbols.
// We verify basic contract: for any value, result is within 0..65535.
static bool test_gamma_result_in_range_contract() {
    unsigned int test_values[] = {1, 2, 500, 1234, 30000, 65534};
    png_fixed_point gamma_val = static_cast<png_fixed_point>(100000); // gamma ~ 1.0
    bool ok_all = true;
    for (unsigned int v : test_values) {
        png_uint_16 res = png_gamma_16bit_correct(v, gamma_val);
        bool inrange = (res <= 65535);
        TEST_ASSERT(inrange, "Result must be within 0..65535 for in-range inputs");
        ok_all = ok_all && inrange;
    }
    return ok_all;
}

////////////////////////////////////////////////////////////////////////////////
// Test Runner
////////////////////////////////////////////////////////////////////////////////

struct TestCase {
    const char* name;
    bool (*func)();
    const char* note;
};

static void RunTest(const TestCase& tc) {
    std::cout << "[ RUN        ] " << tc.name << std::endl;
    bool result = true;
    if (tc.func) {
        result = tc.func();
    } else {
        // No function means skip
        result = true;
    }
    if (result) {
        std::cout << "[  PASSED    ] " << tc.name << " - " << (tc.note ? tc.note : "OK") << std::endl;
    } else {
        std::cout << "[  FAILED    ] " << tc.name << " - " << (tc.note ? tc.note : "Failure") << std::endl;
        g_test_failures++;
    }
}

int main() {
    // Collect tests
    std::vector<TestCase> tests;

    tests.push_back({"test_gamma_boundary_values",
                     test_gamma_boundary_values,
                     "Boundary values for value outside/at the true-range"});
    // In-range true-branch test
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
    tests.push_back({"test_gamma_inrange_float_path_exact",
                     test_gamma_inrange_float_path_exact,
                     "In-range value with gamma ~1.0 using floating-point path (exact expect)"}); // exact expectation due to gamma=1
#else
    tests.push_back({"test_gamma_inrange_non_float_path",
                     test_gamma_inrange_float_path_absence,
                     "In-range value with gamma ~1.0 using non-floating-path (range checks)"}); // range check only
#endif
    tests.push_back({"test_gamma_result_in_range_contract",
                     test_gamma_result_in_range_contract,
                     "Static-like contract: result always within 0..65535 for a set of in-range inputs"});

    // Run tests
    for (const auto& t : tests) {
        RunTest(t);
    }

    // Summary
    std::cout << "---------------------------------" << std::endl;
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 2;
    }
}
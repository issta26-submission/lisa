// cms_unit_test_L2Fix2.cpp
// Purpose: Unit test suite for the focal method L2Fix2 in cmspcs.c
// - Uses C++11 (no GoogleTest)
// - Uses a lightweight, non-terminating assertion style (print-based with a final summary)
// - Leverages the library's internal saturate helper _cmsQuickSaturateWord to compute expected results
// - Accesses the function under test via C linkage to avoid C++ name mangling

/*
Step 1 (Program Understanding and Candidate Keywords):
- Focal function: L2Fix2(cmsFloat64Number L)
- Core operation: return _cmsQuickSaturateWord(L * 652.8);
- Key dependent components: L2Fix2 (wrapper around saturate), _cmsQuickSaturateWord (saturating cast to 16-bit)
- Domain knowledge: saturates to 0..65535 (uint16), input is double (64-bit float)
- Testable behavior: for various L, L2Fix2(L) should equal _cmsQuickSaturateWord(L * 652.8)

/*
Step 2 (Unit Test Generation):
- Tests target:
  - Correctness for negative inputs (should saturate to 0)
  - Zero input (should be 0)
  - Typical inputs (e.g., 0.5, 1.0) mapping through multiply by 652.8 and saturate
  - Boundary behavior near max value where L * 652.8 approaches 65535
  - Values clearly exceeding the max so saturation yields 65535
- Approach: For a set of L values, compute actual = L2Fix2(L) and expected = _cmsQuickSaturateWord(L * 652.8); verify actual == expected
- Static/private/internal function behavior is not accessed directly; test uses the public wrapper and the internal helper as reference

/*
Step 3 (Test Case Refinement):
- Ensure deterministic, repeatable tests
- Use non-terminating checks: report failures but continue running other tests
- Use only standard library facilities
- Provide explanatory comments for each test case

Now providing the test code (no GTest, plain C++11, prints PASS/FAIL per test, summary at end)
*/

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Externally defined functions from the focal library.
// L2Fix2 has signature: cmsUInt16Number L2Fix2(cmsFloat64Number L)
// _cmsQuickSaturateWord has signature: cmsUInt16Number _cmsQuickSaturateWord(cmsFloat64Number v)
extern "C" uint16_t L2Fix2(double L);
extern "C" uint16_t _cmsQuickSaturateWord(double v);

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper: non-terminating assertion (prints diagnostic on failure, increments global counter)
static void assert_uint16_equal(const std::string& test_name,
                                uint16_t actual,
                                uint16_t expected)
{
    ++g_total_tests;
    if (actual != expected) {
        std::cerr << "[FAIL] " << test_name
                  << " | expected: " << static_cast<uint32_t>(expected)
                  << ", actual: " << static_cast<uint32_t>(actual) << std::endl;
        ++g_failed_tests;
    } else {
        std::cout << "[PASS] " << test_name
                  << " | value: " << static_cast<uint32_t>(actual) << std::endl;
    }
}

// Test 1: Basic sanity across a small set of representative L values
// Purpose: Ensure L2Fix2(L) matches _cmsQuickSaturateWord(L*652.8) for common values including 0, small positives, and a few larger ones
static void test_L2Fix2_basic_values()
{
    const double test_values[] = {
        -1.0,    // negative: expect saturation to 0
        -0.1,    // small negative
        0.0,     // zero
        0.5,     // small positive
        1.0,     // small positive
        100.0,   // boundary region far from max but below
        101.0,   // just beyond boundary where saturation may occur
        100.3,   // near boundary
        200.0    // well beyond maxs
    };

    const size_t n = sizeof(test_values)/sizeof(test_values[0]);
    for (size_t i = 0; i < n; ++i) {
        double L = test_values[i];
        uint16_t actual = L2Fix2(L);
        uint16_t expected = _cmsQuickSaturateWord(L * 652.8);
        std::string name = "L2Fix2_basic_values[ L=" + std::to_string(L) + " ]";
        assert_uint16_equal(name, actual, expected);
    }
}

// Test 2: Boundary edge case near the max saturation threshold
// Purpose: Verify behavior around the point where L*652.8 crosses 65535 (maximum for uint16)
static void test_L2Fix2_boundary_near_max()
{
    // Compute a couple of L values around the saturation threshold
    // Threshold is 65535 / 652.8 ≈ 100.293...
    const double L1 = 100.0;  // product = 65280.0 (below max)
    const double L2 = 100.293; // slightly below or around threshold
    const double L3 = 101.0; // product > 65535 -> saturate to 65535

    struct Pair { double L; }
    Pair arr[] = { {L1}, {L2}, {L3} };
    const size_t n = sizeof(arr)/sizeof(arr[0]);
    for (size_t i = 0; i < n; ++i) {
        double L = arr[i].L;
        uint16_t actual = L2Fix2(L);
        uint16_t expected = _cmsQuickSaturateWord(L * 652.8);
        std::string name = "L2Fix2_boundary_near_max[ L=" + std::to_string(L) + " ]";
        assert_uint16_equal(name, actual, expected);
    }
}

// Test 3: Extreme large values to verify full saturation to 65535
static void test_L2Fix2_extremely_large_values()
{
    const double large_values[] = {
        1000.0,   // far above saturation
        1e6,      // very large, should saturate to max
        1e9       // extremely large, still saturate to max
    };
    const size_t n = sizeof(large_values)/sizeof(large_values[0]);
    for (size_t i = 0; i < n; ++i) {
        double L = large_values[i];
        uint16_t actual = L2Fix2(L);
        uint16_t expected = _cmsQuickSaturateWord(L * 652.8);
        std::string name = "L2Fix2_extremely_large_values[ L=" + std::to_string(L) + " ]";
        assert_uint16_equal(name, actual, expected);
    }
}

// Test 4: Negative saturation behavior explicitly
// Purpose: Ensure negative L values saturate to 0
static void test_L2Fix2_negative_saturation()
{
    const double L = -1234.56;
    uint16_t actual = L2Fix2(L);
    uint16_t expected = _cmsQuickSaturateWord(L * 652.8);
    std::string name = "L2Fix2_negative_saturation[ L=" + std::to_string(L) + " ]";
    assert_uint16_equal(name, actual, expected);
}

// Test 5: Repeated calls consistency (sanity check for potential stateful behavior)
// Note: L2Fix2 is a pure function; repeated calls with identical inputs should produce identical results
static void test_L2Fix2_consistency()
{
    const double L = 42.4242;
    const int repeats = 5;
    for (int i = 0; i < repeats; ++i) {
        uint16_t actual = L2Fix2(L);
        uint16_t expected = _cmsQuickSaturateWord(L * 652.8);
        std::string name = "L2Fix2_consistency[ repeat=" + std::to_string(i) + " ]";
        assert_uint16_equal(name, actual, expected);
    }
}

// Entry point
int main()
{
    std::cout << "Starting L2Fix2 unit test suite (C++11, no GTest)" << std::endl;

    test_L2Fix2_basic_values();
    test_L2Fix2_boundary_near_max();
    test_L2Fix2_extremely_large_values();
    test_L2Fix2_negative_saturation();
    test_L2Fix2_consistency();

    std::cout << "\nTest Summary: Total = " << g_total_tests
              << ", Passed = " << (g_total_tests - g_failed_tests)
              << ", Failed = " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}
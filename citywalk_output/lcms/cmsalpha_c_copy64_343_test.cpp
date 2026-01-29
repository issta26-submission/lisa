// Unit test suite for the focal method copy64(void* dst, const void* src)
// ----------------------------------------
// Step 1: Program understanding summary (captured in comments)
// - The focal function copies a 64-bit floating-point number (cmsFloat64Number) from src to dst
// - It does so using memmove with size sizeof(cmsFloat64Number) (i.e., 8 bytes on typical platforms)
// - cmsFloat64Number is treated as a 64-bit floating-point type (double)
// - Key behaviors to verify:
//   1) Exact copy for finite values
//   2) Correct bit-for-bit copy for NaN values (since NaN == NaN is false)
//   3) Correct copy for Infinity
//   4) Correct behavior when source and destination buffers overlap (memmove guarantees correctness)
//   5) No change when src and dst point to the same location
// - Dependencies:
//   - The function is extern "C" linkage in cmsalpha.c (declared below for linking)
//   - memmove is used internally
//
// Step 2: Unit test generation (without GTest; platform-agnostic C++11 test harness)
// - Tests cover a range of numeric edge cases and overlapping memory scenarios
// - Use bit-pattern checks for NaN and general correctness checks for finite numbers
//
// Step 3: Test case refinement (domain knowledge applied)
// - Use a small, non-terminating test harness that prints per-test results
// - Verify both value equality for finite numbers and bit-pattern equality for NaN
// - Ensure static-like behavior by not relying on private internals
// - All tests run from main(); no external test framework required
//
// Note: This test suite assumes cmsFloat64Number is typedef'd as double in the project.
// We declare an extern "C" prototype for copy64 to link against the focal implementation.

#include <cstring>
#include <cstdint>
#include <limits>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// External C function under test
extern "C" void copy64(void* dst, const void* src);

// Local alias matching the project's typedef (double for 64-bit float)
using cmsFloat64Number = double;

// Helper: convert a double to its exact bit pattern (uint64_t)
static inline uint64_t double_to_bits(double d) {
    uint64_t bits;
    std::memcpy(&bits, &d, sizeof(bits));
    return bits;
}

// Global counter for test failures (non-terminating assertions)
static int g_failures = 0;

// Test 1: Finite value copy should be exact (dst == src) and bit-pattern equal
// This ensures straightforward copy64 correctness for regular numbers
static void test_copy64_basic_finite_values() {
    // Prepare source and destination
    cmsFloat64Number src = 1234.567890123;
    cmsFloat64Number dst = 0.0;

    // Execute the focal function
    copy64(&dst, &src);

    // Value-based assertion: exact equality for finite numbers
    if (dst != src) {
        std::cerr << "[FAIL] test_copy64_basic_finite_values: dst (" << dst
                  << ") != src (" << src << ") after copy64\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_basic_finite_values: value equality preserved\n";
    }

    // Bit-pattern equality check (sanity): memmove should copy exact bits
    uint64_t sBits = double_to_bits((double)src);
    uint64_t dBits = double_to_bits((double)dst);
    if (sBits != dBits) {
        std::cerr << "[FAIL] test_copy64_basic_finite_values: bit patterns differ (src=" 
                  << std::hex << sBits << ", dst=" << dBits << std::dec << ")\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_basic_finite_values: bit-pattern preserved\n";
    }
}

// Test 2: Negative finite value copy should be exact and bit-pattern equal
static void test_copy64_negative_values() {
    cmsFloat64Number src = -98765.4321;
    cmsFloat64Number dst = 0.0;

    copy64(&dst, &src);

    if (dst != src) {
        std::cerr << "[FAIL] test_copy64_negative_values: dst (" << dst
                  << ") != src (" << src << ") after copy64\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_negative_values: negative value copied exactly\n";
    }

    uint64_t sBits = double_to_bits((double)src);
    uint64_t dBits = double_to_bits((double)dst);
    if (sBits != dBits) {
        std::cerr << "[FAIL] test_copy64_negative_values: bit-pattern mismatch (src=" 
                  << std::hex << sBits << ", dst=" << dBits << std::dec << ")\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_negative_values: bit-pattern preserved\n";
    }
}

// Test 3: NaN handling should preserve bit-pattern (since NaN != NaN in value comparison)
static void test_copy64_nan_handling() {
    cmsFloat64Number src = std::numeric_limits<cmsFloat64Number>::quiet_NaN();
    cmsFloat64Number dst = 0.0;

    copy64(&dst, &src);

    // Compare bit patterns to ensure exact copy of NaN payload
    uint64_t sBits = double_to_bits((double)src);
    uint64_t dBits = double_to_bits((double)dst);
    if (sBits != dBits) {
        std::cerr << "[FAIL] test_copy64_nan_handling: NaN bit-pattern mismatch after copy64\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_nan_handling: NaN payload preserved\n";
    }
}

// Test 4: Infinity handling and overlapping memory scenario
static void test_copy64_infinity_and_overlap() {
    // Infinity should copy correctly and compare equal
    cmsFloat64Number src = std::numeric_limits<cmsFloat64Number>::infinity();
    cmsFloat64Number dst = 0.0;
    copy64(&dst, &src);
    if (!(dst == src)) {
        std::cerr << "[FAIL] test_copy64_infinity_and_overlap: infinity copy failed\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_infinity_and_overlap: infinity copied correctly\n";
    }

    // Overlapping memory: dst points into src region
    cmsFloat64Number arr[2] = { 3.14, 2.71 };
    copy64((void*)&arr[1], (const void*)&arr[0]); // copy first element into second
    if (!(arr[1] == arr[0])) {
        std::cerr << "[FAIL] test_copy64_infinity_and_overlap: overlap copy failed (arr[1]=" 
                  << arr[1] << ", arr[0]=" << arr[0] << ")\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_infinity_and_overlap: overlap copy preserved value\n";
    }
}

// Test 5: Self-copy (src and dst are the same address) should not modify value
static void test_copy64_same_src_dst() {
    cmsFloat64Number x = 42.0;
    copy64(&x, &x);
    if (!(x == 42.0)) {
        std::cerr << "[FAIL] test_copy64_same_src_dst: self-copy altered value\n";
        ++g_failures;
    } else {
        std::cout << "[OK] test_copy64_same_src_dst: self-copy left value unchanged\n";
    }
}

// Run all tests
static void run_all_tests() {
    test_copy64_basic_finite_values();
    test_copy64_negative_values();
    test_copy64_nan_handling();
    test_copy64_infinity_and_overlap();
    test_copy64_same_src_dst();
}

int main() {
    // Execute all tests and report summary
    run_all_tests();
    std::cout << "Total failures: " << g_failures << std::endl;
    return g_failures ? 1 : 0;
}
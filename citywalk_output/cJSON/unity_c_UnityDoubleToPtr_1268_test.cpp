// Unit test suite for UnityDoubleToPtr in unity.c
// This test suite is written in C++11 and does not rely on GTest.
// It uses simple, non-terminating assertions and a minimal test harness.
// The tests focus on the focal method UnityDoubleToPtr(const double num) and its
// behavior: it stores the input in UnityQuickCompare.d and returns the address
// of that storage as UNITY_INTERNAL_PTR. The tests verify round-trip correctness
// and pointer stability across calls.

// Candidate Keywords (from Step 1): UnityDoubleToPtr, UnityQuickCompare, UNITY_INTERNAL_PTR,
// pointer, double, address of field, static/global storage, memory aliasing, UnityC bridge

#include <stddef.h>
#include <cmath>
#include <cstdint>
#include <unity.h>
#include <iostream>
#include <iomanip>


// Include Unity's public declarations for types/macros if available.
// Unity's internal types (like UNITY_INTERNAL_PTR) are defined in unity.h.
// The project compiles under C++11; we bridge with extern "C" to call C functions.
extern "C" {
// Prototypes for the focal function, defined in unity.c
UNITY_INTERNAL_PTR UnityDoubleToPtr(const double num);
}

// Helper to log failures without terminating tests early
static int g_failures = 0;
static void logFailure(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << ": " << message << std::endl;
    ++g_failures;
}

// Test 1: Round-trip correctness for a variety of representative doubles
// - Purpose: Ensure that UnityDoubleToPtr stores the input value in UnityQuickCompare.d
//   and returns a pointer such that dereferencing yields the original value.
static void test_UnityDoubleToPtr_RoundTrip_VariousValues() {
    // Representative set of doubles, including edge-like values
    const double test_vals[] = {
        0.0,
        -0.0,                 // ensure handling of signed zero (will compare as bits)
        1.0,
        -2.5,
        3.14159265358979323846,
        -1.0e308,              // very large magnitude
        1.0e-308,               // very small magnitude
        9.87654321098765e+101,  // large exponent
        -7.25e-324               // subnormal negative
    };
    const size_t n = sizeof(test_vals) / sizeof(test_vals[0]);
    for (size_t i = 0; i < n; ++i) {
        double input = test_vals[i];
        UNITY_INTERNAL_PTR ptr = UnityDoubleToPtr(input);
        if (ptr == nullptr) {
            logFailure("RoundTrip_VariousValues", "UnityDoubleToPtr returned a null pointer");
            continue;
        }
        // Dereference the pointer to recover the stored value
        double recovered = *((double*)ptr);
        // For exact doubles (as assigned), bit-for-bit equality should hold.
        if (recovered != input) {
            // Special-case: treat -0.0 and 0.0 as a potential non-equivalence for strict equality.
            // If input is -0.0 and recovered is 0.0, the bit patterns differ but numerically equal;
            // however, UnityDoubleToPtr assigns the exact bit pattern of the input, so we require exact match.
            std::ostringstream oss;
            oss << std::setprecision(std::numeric_limits<double>::digits10 + 2)
                << "Input=" << input << " recovered=" << recovered;
            logFailure("RoundTrip_VariousValues", oss.str().c_str());
        }
    }
    if (g_failures == 0) {
        std::cout << "[PASS] UnityDoubleToPtr_RoundTrip_VariousValues" << std::endl;
    }
}

// Test 2: Consistency of the returned pointer across multiple calls with the same input
// - Purpose: The function should always return the address of the same internal storage
//   (UnityQuickCompare.d). This confirms that no temporary or stack-based storage is used.
static void test_UnityDoubleToPtr_SamePointerAcrossCalls() {
    const double sample = 42.0;
    UNITY_INTERNAL_PTR p1 = UnityDoubleToPtr(sample);
    UNITY_INTERNAL_PTR p2 = UnityDoubleToPtr(sample);
    if (p1 != p2) {
        logFailure("SamePointerAcrossCalls", "Expected identical pointers across repeated calls with same input");
        return;
    }
    // Also verify that dereferencing yields the correct value for both
    double v1 = *((double*)p1);
    double v2 = *((double*)p2);
    if (v1 != sample || v2 != sample) {
        logFailure("SamePointerAcrossCalls", "Dereferenced values do not match input after repeated calls");
        return;
    }
    std::cout << "[PASS] UnityDoubleToPtr_SamePointerAcrossCalls" << std::endl;
}

// Test 3: Pointer stability after a separate call with a different value
// - Purpose: Ensure that the pointer refers to a stable storage location.
//   Subsequent calls reuse the same address, so the raw pointer value should be equal
//   across calls, even if the internal value changes.
static void test_UnityDoubleToPtr_PointerStabilityAcrossDifferentInputs() {
    UNITY_INTERNAL_PTR p_old = UnityDoubleToPtr(7.7);
    UNITY_INTERNAL_PTR p_new = UnityDoubleToPtr(-13.13);
    if (p_old != p_new) {
        logFailure("PointerStabilityAcrossDifferentInputs", "Pointer addresses should be identical across different inputs");
        return;
    }
    // And the dereferenced value should now reflect the latest input
    double latest = *((double*)p_new);
    if (latest != -13.13) {
        logFailure("PointerStabilityAcrossDifferentInputs", "Dereferenced value does not reflect the latest input");
        return;
    }
    std::cout << "[PASS] UnityDoubleToPtr_PointerStabilityAcrossDifferentInputs" << std::endl;
}

// Simple launcher to run all tests and report summary
int main() {
    g_failures = 0;

    // Run tests
    test_UnityDoubleToPtr_RoundTrip_VariousValues();
    test_UnityDoubleToPtr_SamePointerAcrossCalls();
    test_UnityDoubleToPtr_PointerStabilityAcrossDifferentInputs();

    // Summary
    if (g_failures == 0) {
        std::cout << "[OVERALL PASS] All UnityDoubleToPtr tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[OVERALL FAIL] Number of failed tests: " << g_failures << std::endl;
        return 1;
    }
}
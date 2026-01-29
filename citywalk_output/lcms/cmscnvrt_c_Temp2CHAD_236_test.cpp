// C++11 unit test suite for the focal method Temp2CHAD in cmscnvrt.c
// This test suite uses a lightweight in-process test harness (no GTest).
// It exercises deterministic behavior, differences across temperatures,
// and ensures the function mutates its output as expected from its implementation.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstring>


// Bring C headers into the C++ test environment
extern "C" {
}

// Simple test harness (non-terminating assertions)
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << msg << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

// Helper: compare two cmsMAT3 matrices by memory bytes
static bool mat_equal(const cmsMAT3& a, const cmsMAT3& b) {
    return std::memcmp(&a, &b, sizeof(cmsMAT3)) == 0;
}

// Test 1: Deterministic output for the same temperature
// The Temp2CHAD function should produce the same Chad matrix when invoked
// with identical inputs (idempotent behavior for the same Temp).
static void test_Temp2CHAD_same_temp() {
    // Initialize Chad to a known zero state
    cmsMAT3 Chad;
    std::memset(&Chad, 0, sizeof(Chad));

    cmsFloat64Number Temp = 6500.0;

    // First invocation
    Temp2CHAD(&Chad, Temp);
    // Snapshot after first invocation
    cmsMAT3 ChadSnapshot;
    std::memset(&ChadSnapshot, 0, sizeof(ChadSnapshot));
    std::memcpy(&ChadSnapshot, &Chad, sizeof(Chad));

    // Second invocation with the same input
    Temp2CHAD(&Chad, Temp);

    // Expect Chad to be identical to the snapshot from the first call
    bool equal = mat_equal(Chad, ChadSnapshot);
    CHECK(equal, "Temp2CHAD should produce identical Chad for identical Temp (deterministic)");
}

// Test 2: Different temperatures should yield different Chad matrices
static void test_Temp2CHAD_diff_temps() {
    cmsMAT3 Chad6500;
    cmsMAT3 Chad5000;
    std::memset(&Chad6500, 0, sizeof(Chad6500));
    std::memset(&Chad5000, 0, sizeof(Chad5000));

    cmsFloat64Number TempA = 6500.0;
    cmsFloat64Number TempB = 5000.0;

    Temp2CHAD(&Chad6500, TempA);
    Temp2CHAD(&Chad5000, TempB);

    bool diff = !mat_equal(Chad6500, Chad5000);
    CHECK(diff, "Temp2CHAD should produce different Chad for different Temps");
}

// Test 3: Ensure Chad is modified from an uninitialized/zero state
static void test_Temp2CHAD_nonzero_from_zero() {
    cmsMAT3 Chad;
    std::memset(&Chad, 0, sizeof(Chad));

    // After a single call, Chad should not remain all-zero
    Temp2CHAD(&Chad, 6500.0);

    cmsMAT3 ChadZero;
    std::memset(&ChadZero, 0, sizeof(ChadZero));

    bool changed = !mat_equal(Chad, ChadZero);
    CHECK(changed, "Temp2CHAD should modify Chad from zero initialization");
}

// Test 4: Cross-check behavior across a broader temperature range
static void test_Temp2CHAD_extreme_temps() {
    cmsMAT3 Chad1000;
    cmsMAT3 Chad6000;
    std::memset(&Chad1000, 0, sizeof(Chad1000));
    std::memset(&Chad6000, 0, sizeof(Chad6000));

    Temp2CHAD(&Chad1000, 1000.0);
    Temp2CHAD(&Chad6000, 6000.0);

    bool diff = !mat_equal(Chad1000, Chad6000);
    CHECK(diff, "Temp2CHAD should yield different Chad when temps are far apart");
}

// Main test runner
int main() {
    // Run tests
    test_Temp2CHAD_same_temp();
    test_Temp2CHAD_diff_temps();
    test_Temp2CHAD_nonzero_from_zero();
    test_Temp2CHAD_extreme_temps();

    // Summary
    std::cout << "Temperature to CHAD tests completed." << std::endl;
    std::cout << "Passed: " << g_pass << ", Failed: " << g_fail
              << ", Total: " << (g_pass + g_fail) << std::endl;

    // Return non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}
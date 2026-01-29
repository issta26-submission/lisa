// Test suite for png_have_vsx in linux_aux.c
// This test uses a custom, non-terminating assertion approach to maximize coverage
// by simulating different getauxval() return values via a mock function with C linkage.

#include <iostream>
#include <sys/auxv.h>
#include <png.h>


// ----------------------------------------------------------------------------
// Mock getauxval to control the hardware capability flags seen by png_have_vsx
// We provide C linkage to override the system version during linking.
// ----------------------------------------------------------------------------

static unsigned long test_current_auxv = 0; // Controlled by tests

extern "C" unsigned long getauxval(unsigned long /*at*/) {
    // Return the value configured by tests
    return test_current_auxv;
}

// Helper to set the mock auxv value in tests
static void set_test_auxv(unsigned long v) {
    test_current_auxv = v;
}

// Simple non-terminating test result aggregator
static int g_failures = 0;
#define TEST_ASSERT_EQ(expected, actual, name) \
    do { \
        if (static_cast<long long>(expected) != static_cast<long long>(actual)) { \
            std::cerr << "[FAIL] " << name \
                      << " - Expected: " << (expected) \
                      << ", Actual: " << (actual) << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[OK] " << name << std::endl; \
        } \
    } while (0)

namespace pngvsx_tests {

    // Test 1: true branch when ALTIVEC flag is set
    // Goal: Ensure a positive (non-zero) result is returned if ALTIVEC is present.
    void test_true_with_altivec() {
        // Arrange
        set_test_auxv(PPC_FEATURE_HAS_ALTIVEC);
        // Act
        int res = png_have_vsx(nullptr);
        // Assert
        TEST_ASSERT_EQ(1, res, "png_have_vsx should return 1 when ALTIVEC flag is set");
    }

    // Test 2: true branch when VSX flag is set
    // Goal: Ensure a positive (non-zero) result is returned if VSX is present.
    void test_true_with_vsx() {
        // Arrange
        set_test_auxv(PPC_FEATURE_HAS_VSX);
        // Act
        int res = png_have_vsx(nullptr);
        // Assert
        TEST_ASSERT_EQ(1, res, "png_have_vsx should return 1 when VSX flag is set");
    }

    // Test 3: false branch when neither ALTIVEC nor VSX flags are set
    // Goal: Ensure 0 is returned when no capabilities are present.
    void test_false_when_no_flags() {
        // Arrange
        set_test_auxv(0);
        // Act
        int res = png_have_vsx(nullptr);
        // Assert
        TEST_ASSERT_EQ(0, res, "png_have_vsx should return 0 when no relevant flags are set");
    }

    // Test 4: true branch when both flags are set
    // Goal: Ensure non-zero is returned when multiple relevant flags are present.
    void test_true_with_both_flags() {
        // Arrange
        unsigned long combined = PPC_FEATURE_HAS_ALTIVEC | PPC_FEATURE_HAS_VSX;
        set_test_auxv(combined);
        // Act
        int res = png_have_vsx(nullptr);
        // Assert
        TEST_ASSERT_EQ(1, res, "png_have_vsx should return 1 when both ALTIVEC and VSX flags are set");
    }

} // namespace pngvsx_tests

// ----------------------------------------------------------------------------
// Main entry: run all tests and report total failures.
// The tests are designed to be executed from main (no GTest framework required).
// ----------------------------------------------------------------------------

int main() {
    using namespace pngvsx_tests;

    // Execute all test cases
    test_true_with_altivec();
    test_true_with_vsx();
    test_false_when_no_flags();
    test_true_with_both_flags();

    // Summary
    std::cout << "Test summary: "
              << (g_failures == 0 ? "0 failures" : std::to_string(g_failures) + " failures")
              << std::endl;

    // Return non-zero if any test failed
    return g_failures ? 1 : 0;
}
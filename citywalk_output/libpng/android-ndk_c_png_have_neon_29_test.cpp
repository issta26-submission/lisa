// Self-contained unit tests for the focal method png_have_neon.
// This test compiles the focal code in a controlled environment by
// providing lightweight mocks for cpu-features and the libpng types/macros.
// It does not rely on GTest; a simple main-based test runner is used.

#include <android-ndk.c>
#include <iostream>
#include <cpu-features.h>


// Lightweight mock of the necessary libpng symbol
typedef void* png_structp;
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// Lightweight mock of the Android cpu-features header (cpu-features.h).
// We provide just the constants and prototypes needed by the focal method.
#ifndef ANDROID_CPU_FAMILY_ARM
#define ANDROID_CPU_FAMILY_ARM 1
#endif
#ifndef ANDROID_CPU_ARM_FEATURE_NEON
#define ANDROID_CPU_ARM_FEATURE_NEON 0x01
#endif

// Global mock state to drive the focal function's behavior.
static int g_mock_cpu_family = 0;
static int g_mock_cpu_features = 0;

// Provide C linkage for the mock CPU feature functions so the C code in
// android-ndk.c can link against them correctly when included.
extern "C" {

// Mock implementations returning values controlled by the test.
static int android_getCpuFamily() {
    return g_mock_cpu_family;
}
static int android_getCpuFeatures() {
    return g_mock_cpu_features;
}

} // extern "C"

// Now include the focal method source file in a C-linkage block to ensure
// the function png_have_neon uses our mocks.
// We also define minimal PNG_UNUSED macro and the necessary type (png_structp)
// before inclusion, so the included code can compile in this test environment.
extern "C" {
} // extern "C"

// Helper: set mock CPU state
static void set_mock_state(int family, int features) {
    g_mock_cpu_family = family;
    g_mock_cpu_features = features;
}

// Simple test assertions (non-terminating)
static bool assert_true(bool cond, const char* note) {
    if (!cond) {
        std::cerr << "ASSERTION FAILED: " << note << std::endl;
        return false;
    }
    return true;
}

// Test 1: True branch - ARM family and NEON feature present
static bool test_true_case() {
    // Arrange: ARM with NEON
    set_mock_state(ANDROID_CPU_FAMILY_ARM, ANDROID_CPU_ARM_FEATURE_NEON);
    // Act
    int result = png_have_neon(nullptr);
    // Assert: result should be non-zero (true)
    return assert_true(result != 0, "png_have_neon should return true for ARM with NEON");
}

// Test 2: False branch - not ARM family
static bool test_not_arm_family_false() {
    // Arrange: Not ARM family
    set_mock_state(0 /* not ARM */, ANDROID_CPU_ARM_FEATURE_NEON);
    // Act
    int result = png_have_neon(nullptr);
    // Assert
    return assert_true(result == 0, "png_have_neon should return false when CPU family != ARM");
}

// Test 3: False branch - ARM family but NEON feature not present
static bool test_arm_but_no_neon_false() {
    // Arrange: ARM family, no NEON
    set_mock_state(ANDROID_CPU_FAMILY_ARM, 0x00);
    // Act
    int result = png_have_neon(nullptr);
    // Assert
    return assert_true(result == 0, "png_have_neon should return false when NEON feature is absent");
}

// Test 4: False branch - ARM family with different features mask (NEON missing)
static bool test_arm_with_other_features_false() {
    // Arrange: ARM family, features mask set to a non-NEON bit
    set_mock_state(ANDROID_CPU_FAMILY_ARM, 0x02); // 0x02 != NEON bit
    // Act
    int result = png_have_neon(nullptr);
    // Assert
    return assert_true(result == 0, "png_have_neon should return false when NEON bit is not set, even if other features exist");
}

// Main test runner
int main() {
    int failures = 0;

    std::cout << "Running png_have_neon unit tests (self-contained) ..." << std::endl;

    if (!test_true_case()) {
        ++failures;
    } else {
        std::cout << "Test 1 passed: True path with ARM+NEON detected." << std::endl;
    }

    if (!test_not_arm_family_false()) {
        ++failures;
    } else {
        std::cout << "Test 2 passed: False path when CPU family != ARM correctly handled." << std::endl;
    }

    if (!test_arm_but_no_neon_false()) {
        ++failures;
    } else {
        std::cout << "Test 3 passed: False path when ARM but NEON missing correctly handled." << std::endl;
    }

    if (!test_arm_with_other_features_false()) {
        ++failures;
    } else {
        std::cout << "Test 4 passed: False path when NEON bit is not set despite other features." << std::endl;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}
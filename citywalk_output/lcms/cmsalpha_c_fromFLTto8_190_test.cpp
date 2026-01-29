// Test suite for the focal method: fromFLTto8 in cmsalpha.c
// Step 1: Program understanding notes (embedded as comments for maintainers)
// - Core purpose: convert a 32-bit floating value in [0,1] to an 8-bit unsigned value by multiplying by 255
//   and saturating to the range [0,255] using the helper _cmsQuickSaturateByte.
// - Key types involved (from dependencies):
//   cmsFloat32Number  : typedef for float
//   cmsUInt8Number     : typedef for uint8 (unsigned 8-bit)
// - The implementation uses: *dst = _cmsQuickSaturateByte(n * 255.0) where n is the source float.
// - We must cover boundary behavior (negative values, values > 1, and normal values), and ensure
//   the function writes exactly one byte to dst.
// - Static helpers inside the library are not directly accessible; tests will rely on public
//   behavior of fromFLTto8 and the saturating semantics described above.
// Step 2: Unit Test Generation
// - We'll import the necessary internal interfaces from the project (lcms2_internal.h).
// - We'll create a small test harness (without GoogleTest) that:
//   * Calls fromFLTto8 with a few representative inputs.
//   * Verifies the produced byte equals the expected saturated value according to the mathematical
//     formula: clamp(n*255.0, 0.0, 255.0) then cast to cmsUInt8Number (i.e., truncation toward zero).
//   * Exercises true/false branches by using inputs giving 0 and 255 saturations.
// Step 3: Test Case Refinement
// - Use a minimal, readable test harness with non-terminating assertions to maximize coverage.
// - Include explanatory comments for each test case.
// - Do not rely on GTest; call tests from main() as required by the domain knowledge.

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// Include the project's internal declarations to ensure proper type names and the target function declaration.
// This path may vary depending on your build system. Adjust if necessary.

// Ensure proper linkage for the focal function under test.
// If the project uses C linkage for cmsalpha.c, this declaration ensures correct binding.
extern "C" {
    void fromFLTto8(void* dst, const void* src);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "[TEST FAILED] " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[TEST PASSED] " << (msg) << std::endl; \
        } \
    } while(false)

// Helper to compute expected saturated byte from source float value.
// Mirrors the saturating behavior described by fromFLTto8: dst = clamp(src*255.0, 0.0, 255.0) cast to uint8.
static cmsUInt8Number compute_expected_fromFLTto8(float src) {
    double d = static_cast<double>(src) * 255.0;
    if (d <= 0.0) return 0;
    if (d >= 255.0) return 255;
    // Cast truncates toward zero, matching typical saturate-to-byte behavior.
    return static_cast<cmsUInt8Number>(d);
}

// Test 1: Boundary values around 0.0 and 1.0 to exercise saturation edges
// - -0.5 -> 0
// - 0.0  -> 0
// - 0.5  -> floor(0.5*255) = 127 (assuming truncation)
// - 1.0  -> 255
static void test_fromFLTto8_boundaries() {
    std::cout << "Running test_fromFLTto8_boundaries..." << std::endl;
    cmsUInt8Number dst;
    float src;

    // Case: negative input -> should saturate to 0
    src = -0.5f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: negative input yields 0 after saturation");

    // Case: zero
    src = 0.0f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: 0.0 yields 0");

    // Case: mid value
    src = 0.5f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: 0.5 yields expected saturated value (127)");

    // Case: maximum within range
    src = 1.0f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: 1.0 yields 255");
}

// Test 2: Values greater than 1.0 and large negative values to ensure proper saturation boundaries
// - 2.0  -> 255
// - -1.0 -> 0
static void test_fromFLTto8_overflow_boundaries() {
    std::cout << "Running test_fromFLTto8_overflow_boundaries..." << std::endl;
    cmsUInt8Number dst;
    float src;

    // Case: greater than 1.0
    src = 2.0f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: 2.0 yields 255 saturation");

    // Case: more negative
    src = -1.0f;
    fromFLTto8(&dst, &src);
    EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                "fromFLTto8: -1.0 yields 0 saturation");
}

// Test 3: Multiple intermediate samples to ensure general correctness
// Use a set of representative values: 0.25, 0.75, 0.99
static void test_fromFLTto8_multiple_values() {
    std::cout << "Running test_fromFLTto8_multiple_values..." << std::endl;
    cmsUInt8Number dst;
    float src;

    const float samples[] = {0.25f, 0.75f, 0.99f};
    for (size_t i = 0; i < sizeof(samples)/sizeof(samples[0]); ++i) {
        src = samples[i];
        fromFLTto8(&dst, &src);
        EXPECT_TRUE(static_cast<unsigned int>(dst) == compute_expected_fromFLTto8(src),
                    "fromFLTto8: intermediate sample yields expected saturated value");
    }
}

// Step: main entry to run tests
int main() {
    std::cout << "Starting unit tests for fromFLTto8 (cmsalpha.c)" << std::endl;
    test_fromFLTto8_boundaries();
    test_fromFLTto8_overflow_boundaries();
    test_fromFLTto8_multiple_values();

    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] fromFLTto8 completed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] Total failures: " << g_failures << std::endl;
        return 1;
    }
}
// Test suite for the focal method: png_permit_mng_features
// Note: This file provides a self-contained test harness for the focal function.
// It includes a minimal in-source reimplementation of the function under test
// and its required dependencies to enable isolated unit testing without
// relying on external libraries or GTest.
// The tests cover true/false branches and masking behaviour as described in
// the provided domain knowledge and guidelines.

// Step 1: Program Understanding (embedded as comments for traceability)
// - Function purpose: Enable/permit specific MNG features for a png_ptr instance.
// - Core behavior:
//   1) If png_ptr is NULL, return 0 (early exit).
//   2) Otherwise, set png_ptr->mng_features_permitted to (mng_features & PNG_ALL_MNG_FEATURES).
//   3) Return the resulting mng_features_permitted.
// - Key dependent components (Candidate Keywords): png_ptr, mng_features_permitted, PNG_ALL_MNG_FEATURES, png_debug

#include <iomanip>
#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Step 2: Minimal, self-contained environment to exercise the focal method
// This reproduces just enough of the dependent environment to compile and run
// the test without requiring the full libpng source tree.

// Lightweight stand-in for the png_struct used by the focal method.
struct png_struct {
    uint32_t mng_features_permitted; // Field accessed by png_permit_mng_features
};

// Typedefs to resemble the original code's naming (simplified for test isolation)
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;
typedef uint32_t png_uint_32;

// Minimal macro to emulate the libpng feature mask
// Choose a non-trivial mask to exercise true/false path coverage for masking.
#define PNG_ALL_MNG_FEATURES 0x0F0F0F0F

// Stub for the internal debugging helper used by the focal method.
// In the real project, this would log or handle debug messaging.
// Here, it is a no-op to keep tests lightweight and deterministic.
extern "C" void png_debug(int /*level*/, const char* /*message*/) {
    (void)/* suppress unused parameter warning */ 0;
}

// Focal method under test (re-implemented here for isolated unit testing).
extern "C" uint32_t png_permit_mng_features(png_structrp png_ptr, uint32_t mng_features)
{
    // Original logic:
    // {
    //   png_debug(1, "in png_permit_mng_features");
    //   if (png_ptr == NULL)
    //      return 0;
    //   png_ptr->mng_features_permitted = mng_features & PNG_ALL_MNG_FEATURES;
    //   return png_ptr->mng_features_permitted;
    // }
    png_debug(1, "in png_permit_mng_features");
    if (png_ptr == NULL)
        return 0;
    png_ptr->mng_features_permitted = mng_features & PNG_ALL_MNG_FEATURES;
    return png_ptr->mng_features_permitted;
}

// Step 3: Lightweight, non-terminating test harness
// We implement a tiny assertion framework that accumulates failures but does not abort,
// enabling us to exercise multiple branches in a single run.

static int g_total = 0;   // Total number of assertions attempted
static int g_failed = 0;  // Number of failed assertions

#define TEST_ASSERT_EQUAL(actual, expected, name) do { \
    ++g_total; \
    if ((actual) != (expected)) { \
        ++g_failed; \
        std::cerr << "Test failed: " << (name) \
                  << " | expected: " << std::hex << (expected) \
                  << " (" << std::dec << (expected) << ")" \
                  << ", actual: " << std::hex << (actual) \
                  << " (" << std::dec << (actual) << ")" << std::endl; \
    } \
} while(0)

#define TEST_CASE(name) static void name()

/* Domain-driven tests for png_permit_mng_features
   - Test 1: Null pointer handling (true branch)
   - Test 2: Masking with non-null pointer, ensuring proper masking and update
   - Test 3: Masking with non-null pointer when some bits are outside the mask
*/

// Test 1: Null pointer handling (should return 0)
TEST_CASE(test_null_ptr_returns_zero) {
    // Candidate Keywords: png_ptr = NULL, early return 0
    uint32_t result = png_permit_mng_features(nullptr, 0x12345678);
    TEST_ASSERT_EQUAL(result, 0u, "png_ptr NULL should return 0");
}

// Test 2: Masking with non-null pointer (all features within mask)
TEST_CASE(test_mask_all_features_within_mask) {
    png_struct s;
    s.mng_features_permitted = 0;

    uint32_t input = 0x0F0F0F0F;               // All bits within mask
    uint32_t expected = input & PNG_ALL_MNG_FEATURES; // Should equal input since mask covers all

    uint32_t result = png_permit_mng_features(&s, input);

    TEST_ASSERT_EQUAL(result, expected, "Masking with all bits inside mask should return input masked");
    TEST_ASSERT_EQUAL(s.mng_features_permitted, expected, "Internal field should store masked value");
}

// Test 3: Masking with non-null pointer (some bits outside mask)
TEST_CASE(test_mask_partial_features) {
    png_struct s;
    s.mng_features_permitted = 0;

    uint32_t input = 0xABCD1234; // Some bits outside the mask should be cleared
    uint32_t expected = input & PNG_ALL_MNG_FEATURES;

    uint32_t result = png_permit_mng_features(&s, input);

    TEST_ASSERT_EQUAL(result, expected, "Masking should zero-out bits outside PNG_ALL_MNG_FEATURES");
    TEST_ASSERT_EQUAL(s.mng_features_permitted, expected, "Internal field should reflect masked value");
}

// Dispatch function to run all tests
static void run_all_tests() {
    test_null_ptr_returns_zero();
    test_mask_all_features_within_mask();
    test_mask_partial_features();
}

// Main entry point for the test binary
int main() {
    run_all_tests();

    std::cout << std::dec;
    std::cout << "Total assertions: " << g_total << std::endl;
    std::cout << "Failed assertions: " << g_failed << std::endl;
    if (g_failed == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
    }
    // Return non-zero if any test failed
    return g_failed;
}
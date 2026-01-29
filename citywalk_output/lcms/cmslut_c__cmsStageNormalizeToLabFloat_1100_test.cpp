// Test suite for cmsStageNormalizeToLabFloat
// This test suite targets the focal function:
//     cmsStage* _cmsStageNormalizeToLabFloat(cmsContext ContextID)
// It relies on LittleCMS (lcms2) public API for stage creation and inspection.
// The tests avoid any test framework (GTest etc.) and use a lightweight
// hand-written harness that continues execution after a failed assertion.
// Each test includes comments explaining its intent and the coverage it aims to provide.

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Import the C API from LittleCMS. We wrap the include to ensure proper linkage
// with C++ code and prevent name mangling when calling a C function.
extern "C" {
}

// Forward declaration of the focal function under test.
// It is assumed to be defined in cmslut.c (as provided) and exposed with C linkage.
extern "C" cmsStage* _cmsStageNormalizeToLabFloat(cmsContext ContextID);

// Lightweight assertion helpers to emulate non-terminating assertions.
// They print diagnostic messages but do not abort the test execution.
static int g_totalTests = 0;
static int g_failedTests = 0;

#define ASSERT_TRUE(cond, msg) do {                                 \
    ++g_totalTests;                                                   \
    if (!(cond)) {                                                    \
        std::cerr << "ASSERT_TRUE FAILED: " << (msg)                 \
                  << " [Line: " << __LINE__ << "]" << std::endl;    \
        ++g_failedTests;                                              \
    }                                                                 \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) do {                                \
    ++g_totalTests;                                                     \
    if ((ptr) == nullptr) {                                           \
        std::cerr << "ASSERT_NOT_NULL FAILED: " << (msg)              \
                  << " (null pointer) [Line: " << __LINE__ << "]"      \
                  << std::endl;                                       \
        ++g_failedTests;                                                \
    }                                                                 \
} while (0)

#define ASSERT_EQUAL(expected, actual, msg) do {                       \
    ++g_totalTests;                                                     \
    if (!((expected) == (actual))) {                                  \
        std::cerr << "ASSERT_EQUAL FAILED: " << (msg)                   \
                  << " | Expected: " << (expected)                     \
                  << " | Actual: " << (actual) << " [Line: " << __LINE__ << "]" \
                  << std::endl;                                       \
        ++g_failedTests;                                              \
    }                                                                 \
} while (0)

// Helper to print a short summary after running tests.
static void PrintSummary() {
    std::cout << "\nTest Summary: " << g_totalTests
              << " tests run, " << g_failedTests << " failures."
              << std::endl;
}

// Test 1: Verify that _cmsStageNormalizeToLabFloat returns a non-null stage
// and that the stage implements the expected conversion signature:
//     stage->Implements == cmsSigFloatPCS2Lab
// This confirms that the function builds the proper transform (PCS to Lab) with
// the correct interface indicator.
static bool test_NormalizeToLabFloat_ReturnsStageWithImplements() {
    // Use the default (null) context to exercise the path that does not rely on
    // a specific CMS context instance.
    cmsContext ctx = 0; // default/global context as used in typical usage
    cmsStage* stage = _cmsStageNormalizeToLabFloat(ctx);

    // Check for non-null stage
    if (stage == nullptr) {
        std::cerr << "Test 1 Failure: _cmsStageNormalizeToLabFloat returned NULL" << std::endl;
        return false;
    }

    // Verify the Implements field is exactly cmsSigFloatPCS2Lab
    // This is the key behavioral contract of the function under test.
    ASSERT_TRUE(stage->Implements == cmsSigFloatPCS2Lab,
                "Stage Implements should be cmsSigFloatPCS2Lab after normalization to Lab (float)");
    // Do not free stage here; the test framework intentionally focuses on function behavior.

    return true;
}

// Test 2: Idempotence / allocation behavior check
// Invoking the focal function twice should yield two distinct stages
// (each call allocates a new stage). This exercise helps ensure the code does not
// cache a single static instance and that multiple independent transformations
// can coexist in a pipeline.
static bool test_NormalizeToLabFloat_TwoDistinctStages() {
    cmsContext ctx = 0;
    cmsStage* s1 = _cmsStageNormalizeToLabFloat(ctx);
    cmsStage* s2 = _cmsStageNormalizeToLabFloat(ctx);

    if (s1 == nullptr || s2 == nullptr) {
        std::cerr << "Test 2 Failure: One or both returned stages are NULL" << std::endl;
        return false;
    }

    // Ensure distinct allocations
    bool distinct = (s1 != s2);
    ASSERT_TRUE(distinct, "Two successive calls should return distinct stage pointers");

    return distinct;
}

// Step 3 notes (domain knowledge) integration:
// - We rely on the public API for stage creation (cmsStageAllocMatrix) and the
//   public field 'Implements' to validate the correct transformation identity.
// - We avoid directly inspecting internal matrix data, as they are not part of
//   the public API and static/fscope details are implementation-dependent.
// - To maximize coverage without a mocking framework, we perform multiple calls
//   to exercise allocation and interface exposure across different Context IDs
//   (here we reuse the default context as a representative case).

int main() {
    // Run tests and collect results
    std::cout << "Running cmsStageNormalizeToLabFloat test suite (no GTest)..." << std::endl;

    // Clear any prior global state (if re-running in same process)
    g_totalTests = 0;
    g_failedTests = 0;

    // Execute tests with explanatory comments
    std::cout << "Test 1: Validate stage creation and Implements field." << std::endl;
    bool t1 = test_NormalizeToLabFloat_ReturnsStageWithImplements();
    if (t1) {
        std::cout << "PASS: Test 1 completed successfully." << std::endl;
    } else {
        std::cout << "FAIL: Test 1 failed." << std::endl;
    }

    std::cout << "Test 2: Validate that two consecutive calls yield distinct stage objects." << std::endl;
    bool t2 = test_NormalizeToLabFloat_TwoDistinctStages();
    if (t2) {
        std::cout << "PASS: Test 2 completed successfully." << std::endl;
    } else {
        std::cout << "FAIL: Test 2 failed." << std::endl;
    }

    PrintSummary();
    // Return non-zero if any test failed, to integrate with simple CI scripts.
    return (g_failedTests == 0) ? 0 : 1;
}
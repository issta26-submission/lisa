// Test suite for the focal method run_test found in cms_transform_extended_fuzzer.c
// This test suite is designed to be compiled with C++11 and linked against the project
// that provides the run_test function (likely via the lcMS2 library).
//
// Important notes:
// - The environment must have lcms2 (or equivalent) available for linking, as run_test
//   internally calls cmsOpenProfileFromMem and other lcms2 APIs.
// - Due to the complexity of constructing valid ICC profiles in memory, this test suite
//   concentrates on exercising safe, boundary behavior (e.g., size checks) and
//   basic invocation patterns without attempting to validate deep transformation results.
// - Tests are written without Google Test/GMock and rely on a simple non-terminating
//   assertion/reporting mechanism so that all tests can execute and report their outcome.
//
// Candidate Keywords extracted from the focal method (for test design reference):
// - cmsOpenProfileFromMem, cmsCreateLab4Profile, cmsCreateLab2Profile, cmsCreateGrayProfile,
//   cmsCreateXYZProfile, cmsCreate_OkLabProfile, cmsCreateNULLProfile,
//   cmsCreateTransformTHR, cmsDoTransform, cmsDeleteTransform, cmsCloseProfile,
//   cmsGetColorSpace, cmsChannelsOf, cmsCIEXYZ, cmsCIELab, TYPE_Lab_8, TYPE_RGB_8, etc.
//
// The tests below are intentionally defensive and will not crash if the internal
// library behavior is not as expected, since constructing valid ICC profiles at runtime
// is non-trivial in a unit-test context.

#include <vector>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// Declaration of the focal method under test.
// The focal code is written in C, and uses the lcms2 library; we declare it with C linkage.
extern "C" void run_test(const uint8_t *data,
                         size_t size,
                         uint32_t intent,
                         uint32_t flags,
                         int dstVal);

// Simple non-terminating assertion mechanism for test reporting.
// Keeps track of failures but does not call abort() to maximize test coverage.
static int g_test_failures = 0;
static void report_test_result(const std::string &test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cerr << "[FAIL] " << test_name << "\n";
        ++g_test_failures;
    }
}

// Test 1: Boundary condition - size is 0 (or less than 2) should cause early return.
// Rationale: The focal method checks size < 2 and returns immediately. This test ensures
// that the function does not crash when given a null pointer with zero length.
void test_run_test_size_zero() {
    // Case A: size = 0, data = nullptr
    const uint8_t *dataA = nullptr;
    size_t sizeA = 0;
    run_test(dataA, sizeA, 0, 0, 0);
    report_test_result("run_test_size_zero_null_data_zero_size", true);

    // Case B: size = 1, data with a dummy byte
    uint8_t dummy = 0xAA;
    const uint8_t *dataB = &dummy;
    size_t sizeB = 1;
    run_test(dataB, sizeB, 0, 0, 0);
    report_test_result("run_test_size_one_nonzero_small_data", true);
}

// Test 2: Boundary condition - size just at threshold (size >= 2) but with likely invalid ICC data.
// Rationale: We cannot easily craft a valid ICC profile in memory for the test environment.
// This test exercises that the function can be invoked with enough data to proceed to
// profile creation attempts, and ensures that the function handles non-crashing behavior
// when the downstream profile creation fails gracefully (which would be a common outcome
// for randomly generated data).
void test_run_test_size_two_with_random_data() {
    // Create a small buffer that mimics non-empty ICC-like data. It is NOT guaranteed to be a
    // valid ICC profile, but this test ensures the function is callable with such input.
    std::vector<uint8_t> buffer(2);
    buffer[0] = 0xDE;
    buffer[1] = 0xAD;

    // Use various dstVal values to exercise the branch selection (as much as possible without a valid profile).
    const uint32_t intents[] = {0, 1, 2}; // arbitrary non-zero values
    const uint32_t flags_list[] = {0, 1, 2}; // arbitrary non-zero values
    for (size_t i = 0; i < sizeof(intents)/sizeof(intents[0]); ++i) {
        for (size_t j = 0; j < sizeof(flags_list)/sizeof(flags_list[0]); ++j) {
            // dstVal in a broader range to touch different DST profiles
            for (int dv = 0; dv <= 12; ++dv) {
                run_test(buffer.data(), buffer.size(), intents[i], flags_list[j], dv);
            }
        }
    }

    // If we reach here, at least the function invocation path was exercised without crashing.
    report_test_result("run_test_size_two_with_random_data_invocations", true);
}

// Test 3: Repeated invocations with different dstVal values to exercise multiple branches
// of the dstProfile/dstFormat selection logic. This test does not assert on internal
// transformations (since ICC profiles are not guaranteed to be valid in tests), but ensures
// that many code paths can be entered without fatal errors.
void test_run_test_various_dstVal() {
    // A small dummy data buffer with 2 bytes to satisfy the size requirement.
    uint8_t data[2] = {0x01, 0x02};

    // Try a broad set of dstVal values including boundary conditions and the default case.
    const int dstVals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    for (size_t i = 0; i < sizeof(dstVals)/sizeof(dstVals[0]); ++i) {
        run_test(data, 2, 0, 0, dstVals[i]);
    }

    report_test_result("run_test_various_dstVal_invocations", true);
}

// Main function to run the test suite.
// It invokes individual tests and reports a final status.
// This follows the guideline that, when gtest is not allowed, test methods are invoked from main.
int main() {
    std::cout << "Starting test suite for cms_transform_extended_fuzzer run_test...\n";

    test_run_test_size_zero();
    test_run_test_size_two_with_random_data();
    test_run_test_various_dstVal();

    if (g_test_failures == 0) {
        std::cout << "All tests completed. SUMMARY: 0 failures.\n";
        return 0;
    } else {
        std::cerr << "Tests completed. SUMMARY: " << g_test_failures << " failure(s).\n";
        return 1;
    }
}
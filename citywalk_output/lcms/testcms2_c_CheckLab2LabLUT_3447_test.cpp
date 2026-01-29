// C++11 Unit Test Suite for CheckLab2LabLUT and related dependencies
// This test harness is designed to accompany the provided focal method
// CheckLab2LabLUT in the CMS/test cms environment (LittleCMS-like API).
// The tests avoid terminating on assertion failures and instead report results.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Include the C test/dependency header.
// Wrap in extern "C" if compiling under C++ to ensure proper linkage.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness
struct TestCase {
    std::string name;
    std::function<bool()> func;
    std::string notes;
};

// Test 1: Validate the positive path of CheckLab2LabLUT
// Rationale: Ensure that the focal method returns true under standard, unaltered
// conditions where the LUT is built with Lab->XYZ and XYZ->Lab stages and validated.
bool test_CheckLab2LabLUT_Positive() {
    cmsInt32Number rc = CheckLab2LabLUT();
    // Expect a non-zero (true) result
    return rc != 0;
}

// Test 2: Validate the dependent LUT checks individually using a crafted LUT
// Rationale: Build a LUT with Lab2XYZ and XYZ2Lab stages and verify that
// the dependent checks (CheckFloatLUT and CheckStagesLUT) behave as expected.
// This helps cover the code paths inside the dependent predicates.
bool test_CheckFloatAndStagesLUT_DependentChecks() {
    // Allocate a LUT with 3 inputs, 3 outputs, mirroring the focal method
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        // Allocation failure should be treated as test failure, not crash
        return false;
    }

    // Insert Lab->XYZ and XYZ->Lab stages to mirror the intended pipeline
    cmsPipelineInsertStage(lut, cmsAT_END, _cmsStageAllocLab2XYZ(DbgThread()));
    cmsPipelineInsertStage(lut, cmsAT_END, _cmsStageAllocXYZ2Lab(DbgThread()));

    // Evaluate the two dependent checks independently
    cmsInt32Number okFloat  = CheckFloatLUT(lut);
    cmsInt32Number okStages = CheckStagesLUT(lut, 2);

    cmsPipelineFree(lut);

    // Both checks must pass to mirror the positive path
    return (okFloat != 0) && (okStages != 0);
}

// Test 3: Check stability/consistency over multiple invocations
// Rationale: Ensure CheckLab2LabLUT yields a stable result across repeated calls.
// This helps exercise potential static state paths and guards against flakiness.
bool test_CheckLab2LabLUT_Consistency() {
    const int ITER = 3;
    cmsInt32Number last = -1;
    for (int i = 0; i < ITER; ++i) {
        last = CheckLab2LabLUT();
        if (last == 0) {
            // If any invocation returns false, test fails this consistency check
            return false;
        }
    }
    // All iterations returned true
    return last != 0;
}

int main() {
    // Assemble the test suite
    std::vector<TestCase> testSuite;

    testSuite.push_back({"CheckLab2LabLUT_Positive", test_CheckLab2LabLUT_Positive,
        "Verifies that the focal CheckLab2LabLUT returns true under standard conditions."});

    testSuite.push_back({"CheckFloatAndStagesLUT_DependentChecks", test_CheckFloatAndStagesLUT_DependentChecks,
        "Verifies dependent checks (CheckFloatLUT and CheckStagesLUT) on a crafted LUT."});

    testSuite.push_back({"CheckLab2LabLUT_Consistency", test_CheckLab2LabLUT_Consistency,
        "Ensures consistent results across multiple invocations of CheckLab2LabLUT."});

    int total = 0;
    int passed = 0;

    std::cout << "Starting unit test suite for CheckLab2LabLUT and related components...\n";

    for (const auto& tc : testSuite) {
        ++total;
        bool result = tc.func();
        if (result) {
            ++passed;
            std::cout << "[PASS] " << tc.name << ": " << tc.notes << "\n";
        } else {
            std::cout << "[FAIL] " << tc.name << ": " << tc.notes << "\n";
        }
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
    if (passed == total) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << "SOME TESTS FAILED\n";
    }

    // Return non-zero if any test failed to help integration with build systems
    return (passed == total) ? 0 : 1;
}
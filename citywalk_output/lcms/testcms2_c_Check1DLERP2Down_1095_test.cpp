/*
Step 1: Candidate Keywords (core components used in the focal method)
- Check1D: the underlying checker invoked by the focal function
- Down flag: TRUE in the focal function (interpreted as a down-sampling/interpolation direction)
- nNodesToCheck: 2 (the number of nodes considered for the check)
- max_err: 0 (allowed error threshold)

Step 2 & 3: A minimal C++11 test suite (no GTest) that exercises the focal method
- Uses a lightweight, non-terminating assertion style to maximize test coverage
- Accesses C functions from testcms2.c via testcms2.h; wraps C declarations with extern "C" for C++ linkage
- Covers deterministic behavior, equivalence to direct Check1D invocation, and basic validity of results
- Imports only standard library facilities
- Tests are invoked from main as required by the domain knowledge constraints

Note: This test suite assumes testcms2.h provides the necessary types (cmsInt32Number, cmsBool, etc.) and the focal function:
    cmsInt32Number Check1DLERP2Down(void);
and that testcms2.c is linked during build.

*/

#include <iostream>
#include <testcms2.h>


// Import C headers with C linkage
extern "C" {
}

// Simple test framework (non-GTest) using plain asserts-like behavior but without terminating the program on failure.
// We implement our own reporting to maximize coverage and continuation of tests.

static void ReportResult(const char* testName, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Test 1: Basic equivalence check
// Purpose: Verify that Check1DLERP2Down() returns the same value as directly calling Check1D(2, TRUE, 0).
// Rationale: This confirms that the focal wrapper correctly forwards parameters to the underlying check.
static bool Test_Check1DLERP2Down_Equals_DirectCheck1D() {
    // Call the focal function
    cmsInt32Number focalRes = Check1DLERP2Down();

    // Call the underlying function with Down = TRUE (cast to cmsBool to avoid macro dependency)
    cmsInt32Number directRes = Check1D(2, (cmsBool)1, 0);

    return (focalRes == directRes);
}

// Test 2: Determinism check
// Purpose: Ensure that multiple invocations of Check1DLERP2Down yield the same result in a row (no hidden state leakage).
static bool Test_Check1DLERP2Down_Deterministic() {
    cmsInt32Number expected = Check1DLERP2Down();
    for (int i = 0; i < 10; ++i) {
        cmsInt32Number val = Check1DLERP2Down();
        if (val != expected) {
            return false;
        }
    }
    return true;
}

// Test 3: Basic validity (non-negativity)
// Purpose: Ensure the result is a non-negative integer, which is a typical contract for test predicates in this suite.
static bool Test_Check1DLERP2Down_NonNegative() {
    cmsInt32Number val = Check1DLERP2Down();
    return val >= 0;
}

// Test 4: Direct Check1D variant robustness (sanity check on the related API)
// Purpose: Validate that Check1D with Down = FALSE does not crash and returns a sensible value.
// This complements the focal path by exploring the related API surface.
static bool Test_Check1D_DownFalse_Sanity() {
    cmsInt32Number val = Check1D(2, (cmsBool)0, 0);
    // Accept any non-negative numeric result as a basic sanity check
    return val >= 0;
}

int main(int argc, char* argv[]) {
    // Run all tests and report results
    int totalTests = 4;
    int failures = 0;

    // Test 1
    if (Test_Check1DLERP2Down_Equals_DirectCheck1D()) {
        ReportResult("Test_Check1DLERP2Down_Equals_DirectCheck1D", true);
    } else {
        ReportResult("Test_Check1DLERP2Down_Equals_DirectCheck1D", false);
        ++failures;
    }

    // Test 2
    if (Test_Check1DLERP2Down_Deterministic()) {
        ReportResult("Test_Check1DLERP2Down_Deterministic", true);
    } else {
        ReportResult("Test_Check1DLERP2Down_Deterministic", false);
        ++failures;
    }

    // Test 3
    if (Test_Check1DLERP2Down_NonNegative()) {
        ReportResult("Test_Check1DLERP2Down_NonNegative", true);
    } else {
        ReportResult("Test_Check1DLERP2Down_NonNegative", false);
        ++failures;
    }

    // Test 4
    if (Test_Check1D_DownFalse_Sanity()) {
        ReportResult("Test_Check1D_DownFalse_Sanity", true);
    } else {
        ReportResult("Test_Check1D_DownFalse_Sanity", false);
        ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests passed (" << totalTests << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed out of " << totalTests << " tests." << std::endl;
        return failures;
    }
}
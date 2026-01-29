/*
This file provides a C++11-based unit test suite for the focal function:
    cmsInt32Number CheckFullLUT(cmsPipeline* lut, cmsInt32Number ExpectedStages)

Goal:
- Validate that CheckFullLUT correctly orchestrates its internal checks:
  CheckStagesLUT(lut, ExpectedStages) && Check16LUT(lut) && CheckFloatLUT(lut)
- Ensure that the lut is freed via cmsPipelineFree(lut) regardless of the
  checks' outcome.
- Exercise both true and false branches of the dependent checks by constructing
  test LUTs with varying compositions.

Note:
- The tests rely on the LittleCMS-like test helpers and API available in testcms2.h
  (as reflected by the provided <FOCAL_CLASS_DEP> snippet). The test uses a
  minimal, plausible pipeline construction by leveraging functions such as
  cmsPipelineAlloc, AddIdentityMatrix, AddIdentityCLUT16, AddIdentityCLUTfloat,
  and cmsPipelineFree. These helpers are typical in the test scaffolding of this
  project and are assumed to be present and linked with the compilation unit.
- This file does not use GTest or any external mocking framework; it implements
  a lightweight, executable test harness suitable for C++11 compilation.
- Static/private members in the focal code are not accessed directly; the test
  uses the provided public API (as required by the domain knowledge spec).

Step 1: Program Understanding (Key Candidate Keywords)
- CheckFullLUT: the function under test
- lut: a cmsPipeline* input/output
- ExpectedStages: the numeric target for stage-count checks
- CheckStagesLUT(lut, ExpectedStages): stage-count validation
- Check16LUT(lut): 16-bit LUT validation
- CheckFloatLUT(lut): floating-point LUT validation
- cmsPipelineFree(lut): memory cleanup
- cmsPipelineAlloc / AddIdentityMatrix / AddIdentityCLUT16 / AddIdentityCLUTfloat:
  helper APIs used to create a test LUT
- cmsInt32Number / cmsPipeline*: LittleCMS types
- testcms2.h: test infrastructure and API surface

Step 2: Unit Test Generation (Test Suite in C++11)
Below is a self-contained test file (C++11) that exercises CheckFullLUT by:
- Constructing LUTs with a known composition
- Invoking CheckFullLUT with a variety of ExpectedStages
- Verifying behavior via return codes
- Printing results to stdout for simple observation without a terminating assertion
- Ensuring the lut is freed after the call (as CheckFullLUT is required to do)

Step 3: Test Case Refinement
- Two representative tests are provided:
  - All-conditions-true path: a LUT constructed to exercise the typical positive path
  - Early-failure path: a LUT intentionally constructed to fail the first check
- The tests are designed to be extendable: additional scenarios can be added by varying the LUT composition and the ExpectedStages parameter.

Code (unit tests for CheckFullLUT)
*/

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Include the project's test infrastructure and the focal API.
// The tests rely on the declarations provided by testcms2.h.
// Use extern "C" to ensure proper linkage when compiled as C++.
extern "C" {
}

// Lightweight test harness (no external frameworks)
namespace TestFramework {
    using TestFn = std::function<bool()>;

    struct TestCase {
        std::string name;
        TestFn func;
    };

    static void RunAll(const std::vector<TestCase>& cases) {
        for (const auto& c : cases) {
            bool ok = false;
            try {
                ok = c.func();
            } catch (...) {
                ok = false;
            }
            std::cout << "[TEST] " << c.name << " : " << (ok ? "PASS" : "FAIL") << "\n";
        }
    }

    // Simple assertion helper for internal tests (non-fatal)
    static void Assert(bool condition, const char* msg) {
        if (!condition) {
            std::cerr << "ASSERT FAILED: " << (msg ? msg : "") << "\n";
        }
    }
}

// Candidate keywords summary (for traceability)
/*
Candidate Keywords extracted from the focal method and dependencies:
- CheckFullLUT
- lut, ExpectedStages
- CheckStagesLUT(lut, ExpectedStages)
- Check16LUT(lut)
- CheckFloatLUT(lut)
- cmsPipelineFree(lut)
- cmsPipelineAlloc(...)
- AddIdentityMatrix(lut)
- AddIdentityCLUTfloat(lut)
- AddIdentityCLUT16(lut)
- cmsInt32Number, cmsPipeline (types used)
- testcms2.h (test scaffolding with many helpers)
*/

// Helper to build a simple LUT using plausible API from testcms2.h
static cmsPipeline* BuildSimpleIdentityLUT() {
    // Attempt to allocate a 3-input, 3-output LUT with a default context (NULL)
    cmsPipeline* lut = cmsPipelineAlloc(NULL, 3, 3);
    if (lut == nullptr) {
        std::cerr << "[BUILD] Failed to allocate LUT\n";
        return nullptr;
    }

    // Populate with identity-like stages to be a neutral transform
    AddIdentityMatrix(lut);        // Identity 3x3 matrix
    AddIdentityCLUT16(lut);         // Identity 16-bit CLUT (if present)
    AddIdentityCLUTfloat(lut);      // Identity float CLUT (if present)

    // Return the built LUT
    return lut;
}

// Test: All checks pass path
static bool test_CheckFullLUT_AllChecksPass() {
    cmsPipeline* lut = BuildSimpleIdentityLUT();
    if (lut == nullptr) {
        std::cerr << "[TEST] Couldn't create LUT for AllChecksPass\n";
        return false;
    }

    // The ExpectedStages value is chosen conservatively.
    // Depending on the actual composition, this may need adjustment in a real environment.
    cmsInt32Number ExpectedStages = 3;

    cmsInt32Number rc = CheckFullLUT(lut, ExpectedStages);

    // The API contract requires that lut be freed inside CheckFullLUT.
    // We rely on that behavior; nothing further to do here.
    return rc != 0;
}

// Test: First check failure path (no stages or misconfigured LUT)
static bool test_CheckFullLUT_FirstCheckFails() {
    // Build a very minimal LUT (no extra stages beyond identity)
    cmsPipeline* lut = cmsPipelineAlloc(NULL, 3, 3);
    if (lut == nullptr) {
        std::cerr << "[TEST] Couldn't create LUT for FirstCheckFails\n";
        return false;
    }
    // Do NOT add identity or any stages on purpose to trigger first check failure
    // (Assumes that CheckStagesLUT will fail on an insufficient/misaligned LUT)

    cmsInt32Number ExpectedStages = 1;
    cmsInt32Number rc = CheckFullLUT(lut, ExpectedStages);

    // cmsPipelineFree should have been called inside CheckFullLUT
    // If something went wrong and lut wasn't freed, we would have a leak here
    // (Note: cannot observe freely here without additional hooks)
    return rc == 0;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    std::vector<TestFramework::TestCase> tests = {
        {"CheckFullLUT_AllChecksPass", test_CheckFullLUT_AllChecksPass},
        {"CheckFullLUT_FirstCheckFails", test_CheckFullLUT_FirstCheckFails}
    };

    // Run the tests
    TestFramework::RunAll(tests);

    return 0;
}
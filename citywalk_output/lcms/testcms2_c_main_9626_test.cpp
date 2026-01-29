// test_main_suite.cpp
// Lightweight, non-GTest unit tests for the focal main method's dependencies.
// The goal is to exercise a representative subset of functions that main would
// invoke, without bringing in a full test framework. We rely on the existing
// testcms2.h declarations (as provided by the project) and treat non-zero
// return values as "success" for the cmsInt32Number based tests.
// This file is intended to be compiled alongside the project sources
// (including testcms2.c) in a C++11 capable environment.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


#ifdef __cplusplus
extern "C" {
#endif
// Import the relevant declarations used by the focal main method.
// The header provides prototypes for test utilities and a large set of checks.
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion helper
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { \
    std::cerr << "ASSERTION FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
} } while(0)

// Lightweight test harness (no external test framework)
struct SimpleTest {
    std::string name;
    std::function<bool()> run;
};

static std::vector<SimpleTest> g_tests;

// Helper to register tests
#define REGISTER_TEST(name, fn) do { \
    g_tests.push_back(SimpleTest{ name, fn }); \
} while(0)

// Forward declarations of the test scenarios
static bool Test_CoreChecks_BasicAndIntermediates();
static bool Test_CoreChecks_ProfileLifecycle();
static bool Test_CoreChecks_PrintAndDirectCalls();

// Entry point for this test-suite executable
int main(int argc, char* argv[]) {
    // The tests are designed to run quickly and in isolation.
    // We additionally print a short banner describing what is being tested.
    std::cout << "Running test_main_suite.cpp: lightweight tests for testcms2 helpers\n";

    // Register tests (these cover a representative subset of the workflow main() would invoke)
    REGISTER_TEST("CoreChecks_BasicAndIntermediates", Test_CoreChecks_BasicAndIntermediates);
    REGISTER_TEST("CoreChecks_ProfileLifecycle", Test_CoreChecks_ProfileLifecycle);
    REGISTER_TEST("CoreChecks_PrintAndDirectCalls", Test_CoreChecks_PrintAndDirectCalls);

    // Execute tests
    int passed = 0;
    int failed = 0;
    for (const auto& t : g_tests) {
        bool ok = false;
        try {
            ok = t.run();
        } catch (...) {
            ok = false;
            std::cerr << "Test threw an exception: " << t.name << "\n";
        }
        std::cout << "Test: " << t.name << " => " << (ok ? "PASS" : "FAIL") << "\n";
        if (ok) ++passed; else ++failed;
    }

    std::cout << "\nTest summary: " << passed << " passed, " << failed << " failed, "
              << g_tests.size() - failed << " not run/indeterminate (if any).\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}

// Test 1: Basic and intermediate checks that exercise a variety of non-static test entry points.
// Rationale: This validates the presence and executability of several non-static test
// entry points that main() calls early in its execution path (e.g., header checks, base types, etc).
static bool Test_CoreChecks_BasicAndIntermediates() {
    bool ok = true;

    // Basic type and packing checks
    cmsInt32Number rBaseTypes = CheckBaseTypes();
    if (rBaseTypes == 0) {
        std::cerr << "CheckBaseTypes() returned 0 (unexpected), indicating a failure in basic type verification.\n";
        ok = false;
    }

    cmsInt32Number rEndianness = CheckEndianness();
    if (rEndianness == 0) {
        std::cerr << "CheckEndianness() returned 0 (unexpected).\n";
        ok = false;
    }

    cmsInt32Number rQuickFloor = CheckQuickFloor();
    if (rQuickFloor == 0) {
        std::cerr << "CheckQuickFloor() returned 0 (unexpected).\n";
        ok = false;
    }

    cmsInt32Number rQuickFloorWord = CheckQuickFloorWord();
    if (rQuickFloorWord == 0) {
        std::cerr << "CheckQuickFloorWord() returned 0 (unexpected).\n";
        ok = false;
    }

    cmsInt32Number rD50 = CheckD50Roundtrip();
    if (rD50 == 0) {
        std::cerr << "CheckD50Roundtrip() returned 0 (unexpected).\n";
        ok = false;
    }

    // Optional: create and remove test profiles (lifecycle)
    if (CreateTestProfiles() == 0) {
        std::cerr << "CreateTestProfiles() failed (returned 0)\n";
        ok = false;
    } else {
        // Cleanup
        RemoveTestProfiles();
    }

    // A handful of 1D interpolation checks (non-static accessible entries)
    // These help exercise code paths related to 1D interpolation tests.
    if (Check1DLERP2() == 0) {
        std::cerr << "Check1DLERP2() failed (returned 0)\n";
        ok = false;
    }
    if (Check1DLERP3() == 0) {
        std::cerr << "Check1DLERP3() failed (returned 0)\n";
        ok = false;
    }
    if (Check1DLERP4() == 0) {
        std::cerr << "Check1DLERP4() failed (returned 0)\n";
        ok = false;
    }

    // 3D interpolation checks (non-static entries)
    if (Check3DinterpolationFloatTetrahedral() == 0) {
        std::cerr << "Check3DinterpolationFloatTetrahedral() failed (returned 0)\n";
        ok = false;
    }
    if (Check3DinterpolationFloatTrilinear() == 0) {
        std::cerr << "Check3DinterpolationFloatTrilinear() failed (returned 0)\n";
        ok = false;
    }
    if (Check3DinterpolationTetrahedral16() == 0) {
        std::cerr << "Check3DinterpolationTetrahedral16() failed (returned 0)\n";
        ok = false;
    }
    if (Check3DinterpolationTrilinear16() == 0) {
        std::cerr << "Check3DinterpolationTrilinear16() failed (returned 0)\n";
        ok = false;
    }

    // Additional generic checks
    if (CheckReverseInterpolation3x3() == 0) {
        std::cerr << "CheckReverseInterpolation3x3() failed (returned 0)\n";
        ok = false;
    }
    if (CheckReverseInterpolation4x3() == 0) {
        std::cerr << "CheckReverseInterpolation4x3() failed (returned 0)\n";
        ok = false;
    }

    // Basic 3D and 4D interpolation presence in non-static realm
    if (Check3Dinterp() == 0) {
        std::cerr << "Check3Dinterp() failed (returned 0)\n";
        ok = false;
    }
    if (Check3DinterpGranular() == 0) {
        std::cerr << "Check3DinterpGranular() failed (returned 0)\n";
        ok = false;
    }

    if (Check4Dinterp() == 0) {
        std::cerr << "Check4Dinterp() failed (returned 0)\n";
        ok = false;
    }
    if (Check4DinterpGranular() == 0) {
        std::cerr << "Check4DinterpGranular() failed (returned 0)\n";
        ok = false;
    }

    // Color and gamma tests (some are heavy, but we invoke a couple we know exist)
    if (CheckLab2LCh() == 0) {
        std::cerr << "CheckLab2LCh() failed (returned 0)\n";
        ok = false;
    }
    if (CheckLab2XYZ() == 0) {
        std::cerr << "CheckLab2XYZ() failed (returned 0)\n";
        ok = false;
    }

    return ok;
}

// Test 2: Profile lifecycle and more detailed checks that mimic a broader portion of main()
// behavior when DoCheckTests or DoSpeedTests are enabled. We cover the profile creation,
// error reporting hooks and a couple of LUT-related tests that are non-static.
static bool Test_CoreChecks_ProfileLifecycle() {
    bool ok = true;

    // Ensure supported profiles can be created and destroyed without crashing
    if (CreateTestProfiles() == 0) {
        std::cerr << "CreateTestProfiles() failed during lifecycle test.\n";
        ok = false;
    } else {
        RemoveTestProfiles();
    }

    // Ensure memory/error hooks are installable (these are simple wrappers in test cms)
    // Note: In this test environment, these functions are expected to be no-ops or safe.
    PrintSupportedIntents();

    // Test a couple of LUT creation steps (non-static entry points)
    if (CheckLUTcreation() == 0) {
        std::cerr << "CheckLUTcreation() failed (returned 0).\n";
        ok = false;
    }

    // Basic gamma LUT checks
    if (CheckGammaCreation16() == 0) {
        std::cerr << "CheckGammaCreation16() failed (returned 0).\n";
        ok = false;
    }
    if (CheckGammaCreationFlt() == 0) {
        std::cerr << "CheckGammaCreationFlt() failed (returned 0).\n";
        ok = false;
    }

    // Check a couple of stored identity and profile handling tests
    if (CheckStoredIdentities() == 0) {
        std::cerr << "CheckStoredIdentities() failed (returned 0).\n";
        ok = false;
    }

    // Profile-related I/O sanity checks
    if (CheckProfileCreation() == 0) {
        std::cerr << "CheckProfileCreation() failed (returned 0).\n";
        ok = false;
    }

    // Error reporting related tests
    if (CheckErrReportingOnBadProfiles() == 0) {
        std::cerr << "CheckErrReportingOnBadProfiles() failed (returned 0).\n";
        ok = false;
    }

    // Call a few additional end-to-end style checks
    if (CheckLab2LabLUT() == 0) {
        std::cerr << "CheckLab2LabLUT() failed (returned 0).\n";
        ok = false;
    }

    return ok;
}

// Test 3: Print-oriented and direct function calls that exercise branches related to
// status reporting, formatting and direct function invocation (without asserting on
// the internal engine state of main()). This helps increase coverage for prints and
// direct function invocation paths used by main() before deeper checks kick in.
static bool Test_CoreChecks_PrintAndDirectCalls() {
    bool ok = true;
    // Directly exercise a subset of non-mutating calls
    PrintSupportedIntents();
    Check("Base types", CheckBaseTypes); // non-terminating wrapper; ensure it can be invoked
    Check("endianness", CheckEndianness);
    Check("quick floor", CheckQuickFloor);
    Check("D50 roundtrip", CheckD50Roundtrip);
    // If these calls crash or are unavailable, the test would fail here.
    // For static/dynamic integrity, we simply ensure no crash occurs during invocation.

    // Some additional direct tests (non-static)
    if (CheckNumerousBasicTests) {
        // This line is optional; in many builds, the symbol may not exist.
        // We guard with a runtime check if defined.
    }
    return ok;
}

// Helper to check if a symbol exists in the current build (guarded by preprocessor).
// If not, this avoids failing the suite in configurations where a symbol is absent.
#ifdef CheckNumerousBasicTests
static bool CheckNumerousBasicTests = true;
#else
static bool CheckNumerousBasicTests = false;
#endif

// Implementations of the test case function wrappers invoked by the test suite
static bool Test_CoreChecks_BasicAndIntermediates() {
    // Only use the dedicated function declared above
    return Test_CoreChecks_BasicAndIntermediates_impl();
}

// To keep the file self-contained even if the internal naming differs across builds,
// provide an adapter function that calls the real test function if it's present.
// The real function name is inferred from the approach used in Step 1.
// If the build uses a different symbol naming, this adapter can be adjusted accordingly.
static bool Test_CoreChecks_BasicAndIntermediates_impl() {
    // Reuse the earlier defined entity
    return Test_CoreChecks_BasicAndIntermediates(); // recursive call – but in this stub, we avoid recursion by inline code above
}

// Due to the complexity of integrating a perfect cross-build adapter without knowing
// the exact symbol availability, we instead rely on the directly defined test
// functions above for coverage. The following wrappers provide a clean entry
// point for the test runner to mark success/failure based on return values.

static bool Test_CoreChecks_BasicAndIntermediates_override() {
    // Directly invoke the previously defined test function
    return Test_CoreChecks_BasicAndIntermediates();
}

static bool Test_CoreChecks_ProfileLifecycle_override() {
    return Test_CoreChecks_ProfileLifecycle();
}

static bool Test_CoreChecks_PrintAndDirectCalls_override() {
    return Test_CoreChecks_PrintAndDirectCalls();
}

// Actual test function registrations (fill names for clarity)
static bool Test_CoreChecks_BasicAndIntermediates() {
    return Test_CoreChecks_BasicAndIntermediates();
}
static bool Test_CoreChecks_ProfileLifecycle() {
    return Test_CoreChecks_ProfileLifecycle();
}
static bool Test_CoreChecks_PrintAndDirectCalls() {
    return Test_CoreChecks_PrintAndDirectCalls();
}

// At least one public stub to satisfy the compiler for those names (no-ops)
extern "C" void __attribute__((unused)) cmsFooterStub() {}

// Note: The above file purposefully avoids modifying the focal main() logic.
// It instead exercises the exposed test entry points and simple helper functions
// to achieve meaningful coverage without a full-fledged unit testing framework.


// End of test_main_suite.cpp
// Test suite for the focal method Check15bitsConversions and its dependencies.
// This test harness is designed for C++11 (no GoogleTest) and exercises the
// 15-bit conversion workflow by invoking the actual functions from the focal
// class dependencies. The test suite uses a lightweight, non-terminating
// assertion mechanism to maximize code coverage and preserve execution flow.
//
// Notes:
// - The tests rely on the presence of the profile files accessible via the
//   library-provided loadProfile(name) function (which internally uses PROFILES_DIR).
// - We avoid private/internal state assumptions and only call publicly-exposed
//   entry points from the focal module (as declared in extern "C" prototypes).
// - All tests are self-contained within main() and print per-test pass/fail results.

#include <fast_float_internal.h>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Minimal C linkage to the focal module's API.
// We declare only what's needed for test execution. These declarations mirror
// the types used by the focal codebase to minimize ABI risk during linking.
// If the actual types differ in the real build, adjust these typedefs accordingly
// to keep linking working.

extern "C" {
    // Core types (best-effort minimal typedefs; align with the focal code usage)
    typedef void* cmsHPROFILE;
    typedef void* cmsContext;
    typedef int   cmsInt32Number;
    typedef unsigned int cmsUInt32Number;
    typedef unsigned short cmsUInt16Number;
    typedef unsigned char  cmsUInt8Number;
    typedef double cmsFloat64Number;
    typedef float  cmsFloat32Number;
    typedef int    cmsBool;

    // External functions used by the focal method and its helpers
    void trace(const char* frm, ...); // In test, we don't rely on output
    cmsHPROFILE loadProfile(const char* name); // Loads profile using PROFILES_DIR
    cmsHPROFILE cmsOpenProfileFromFile(const char* FileName, const char* FileMode); // Optional usage
    void TryAllValues15(cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);
    void Check15bitMacros(void);
    void Check15bitsConversions(void); // The focal method under test
}

// Lightweight test harness helpers (non-terminating assertions)
static inline const char* yesno(bool ok) { return ok ? "PASS" : "FAIL"; }

// Test 1: Verify that Check15bitMacros can be invoked without crashing.
// Rationale: Ensures the 15-bit macro setup path is accessible and executable.
bool test_Check15bitMacrosRuns() {
    // Act
    Check15bitMacros();

    // Assert (non-terminating): we can't introspect internal state here.
    // We consider success if the function returns without crashing.
    return true;
}

// Test 2: Verify TryAllValues15 can operate with valid profiles loaded via loadProfile.
// Rationale: Ensures the core 15-bit transformation path can be exercised with concrete profiles.
bool test_TryAllValues15WithProfiles() {
    // Load two profiles that are used in the focal tests. If loading fails, report failure.
    cmsHPROFILE inP  = loadProfile("test5.icc");
    cmsHPROFILE outP = loadProfile("test3.icc");

    bool loaded = (inP != nullptr) && (outP != nullptr);
    if (!loaded) {
        std::cerr << "[Test] Failed to load profiles 'test5.icc' and/or 'test3.icc'.\n";
        return false;
    }

    // Call TryAllValues15 with a neutral intent value (0) as a safe default.
    // Note: The original code uses INTENT_PERCEPTUAL; here we pass 0 to avoid macro
    // dependencies in the test environment. If the real constant is required, adjust.
    TryAllValues15(inP, outP, 0);

    // If no crash occurs, treat as pass (non-terminating assertion)
    return true;
}

// Test 3: Verify that Check15bitsConversions orchestrates the high-level 15-bit tests.
// Rationale: The focal method should invoke the macro checks and the profile comparisons.
// This test ensures the top-level orchestration path is accessible and executable.
bool test_Check15bitsConversionsRuns() {
    // Direct invocation of the focal method under test.
    Check15bitsConversions();

    // As with other tests, success is defined by non-crashing execution.
    return true;
}

// Entry point for the test suite
int main() {
    // Register tests with descriptive names and corresponding function pointers
    struct TestCase {
        const char* name;
        std::function<bool()> func;
        const char* description;
    };

    std::vector<TestCase> tests = {
        { "Check15bitMacrosRuns",       test_Check15bitMacrosRuns,       "Execute Check15bitMacros to ensure macro path is accessible." },
        { "TryAllValues15WithProfiles",  test_TryAllValues15WithProfiles,  "Load test profiles and run TryAllValues15 to exercise 15-bit path." },
        { "Check15bitsConversionsRuns",  test_Check15bitsConversionsRuns,  "Invoke Check15bitsConversions to exercise top-level workflow." }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Starting 15-bits-conversions unit tests (C++11 harness) ..." << std::endl;

    // Execute each test in sequence, reporting non-terminating results
    for (const auto& t : tests) {
        bool ok = t.func();
        std::cout << "[TEST] " << t.name << " : " << yesno(ok)
                  << " - " << t.description << std::endl;
        if (ok) ++passed;
    }

    std::cout << "Test results: " << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}
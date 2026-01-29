// test_CheckFormattersFloat.cpp
// Purpose:
//   - Provide a small, self-contained test suite for the focal method CheckFormattersFloat()
//   - Avoid any third-party test framework (GTest) as requested
//   - Use only C++11 standard library facilities
//   - Access the focal function and related globals via the C interface provided by testcms2.h

// NOTE:
//   This suite relies on the surrounding test CMS infrastructure (testcms2.c / testcms2.h).
//   It assumes CheckFormattersFloat is exposed with C linkage and that a global
//   FormatterFailed variable exists (used by the original macros inside the focal method).

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Bring in the C headers that declare the focal function and related globals.
// The C header should declare:
//   - cmsInt32Number CheckFormattersFloat(void);
//   - extern int FormatterFailed;  (or compatible external symbol used by the macro C(...))
// We wrap the include in extern "C" to ensure correct linkage from C++.
extern "C" {
}

// Lightweight test harness (no external libraries)
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

static std::vector<TestResult> g_results;

// Simple helper to record a test result
static void record_result(const std::string& name, bool passed, const std::string& details = "") {
    g_results.push_back({name, passed, details});
}

// Helper to print a single result
static void print_result(const TestResult& r) {
    if (r.passed) {
        std::cout << "[PASS] " << r.name;
    } else {
        std::cerr << "[FAIL] " << r.name;
    }
    if (!r.details.empty()) {
        std::cout << " - " << r.details;
    }
    std::cout << std::endl;
}

// Test 1: Basic sanity - CheckFormattersFloat should return 1 in a normal environment
static bool test_CheckFormattersFloat_ReturnsOne() {
    // The focal function resets its internal FormatterFailed flag and then evaluates all known
    // formats. If all is well, it returns 1.
    cmsInt32Number res = CheckFormattersFloat();
    bool ok = (res == 1);
    if (!ok) {
        record_result("CheckFormattersFloat_ReturnsOne", false,
                      "Expected return value 1, got " + std::to_string(res));
    } else {
        record_result("CheckFormattersFloat_ReturnsOne", true);
    }
    return ok;
}

// Test 2: After calling CheckFormattersFloat, ensure that FormatterFailed remains zero
// This checks that the function did not encounter any formatter failures during its run.
// Note: This relies on the global symbol FormatterFailed exposed by testcms2.h.
static bool test_CheckFormattersFloat_FormatterFlagZero() {
    // Accessing the external flag to verify post-condition.
    // If the implementation always clears/keeps FormatterFailed at 0 on success, this should pass.
    extern int FormatterFailed;
    bool ok = (FormatterFailed == 0);
    if (!ok) {
        record_result("CheckFormattersFloat_FormatterFlagZero", false,
                      "FormatterFailed is non-zero after CheckFormattersFloat");
    } else {
        record_result("CheckFormattersFloat_FormatterFlagZero", true);
    }
    return ok;
}

// Test 3: Idempotence - Calling the function twice should produce consistent results
static bool test_CheckFormattersFloat_Idempotent() {
    cmsInt32Number first = CheckFormattersFloat();
    cmsInt32Number second = CheckFormattersFloat();
    bool ok = (first == 1) && (second == 1);
    if (!ok) {
        record_result("CheckFormattersFloat_Idempotent", false,
                      "First=" + std::to_string(first) +
                      ", Second=" + std::to_string(second));
    } else {
        record_result("CheckFormattersFloat_Idempotent", true);
    }
    return ok;
}

// Entry point to run all tests
int main() {
    // Clear results
    g_results.clear();

    // Run tests
    test_CheckFormattersFloat_ReturnsOne();
    test_CheckFormattersFloat_FormatterFlagZero();
    test_CheckFormattersFloat_Idempotent();

    // Print summary
    int passed = 0;
    int total = static_cast<int>(g_results.size());
    for (const auto& r : g_results) {
        print_result(r);
        if (r.passed) ++passed;
    }

    std::cout << "Test suite completed: " << passed << " / " << total << " tests passed." << std::endl;
    // Return code 0 for all tests pass, non-zero otherwise (simple CI-friendly signal)
    return (passed == total) ? 0 : 1;
}

/*
Step-by-step justification and mapping to the focal method:

- The tests exercise the focal method CheckFormattersFloat(void) which toggles a global
  FormatterFailed and then performs a series of format checks via the C(...) macro, across
  various floating-point type variants (FLT, DBL, and possibly HALF_FLT variants depending on
  CMS_NO_HALF_SUPPORT). The final result is FormatterFailed == 0 ? 1 : 0.

- Candidate Keywords extracted from the focal method:
  - FormatterFailed (global state tracked by the test harness)
  - C(...) macro invocations with the following Type variants:
    TYPE_XYZ_FLT, TYPE_Lab_FLT, TYPE_GRAY_FLT, TYPE_RGB_FLT, TYPE_BGR_FLT, TYPE_CMYK_FLT,
    TYPE_LabA_FLT, TYPE_RGBA_FLT, TYPE_ARGB_FLT, TYPE_BGRA_FLT, TYPE_ABGR_FLT,
    TYPE_XYZ_DBL, TYPE_Lab_DBL, TYPE_GRAY_DBL, TYPE_RGB_DBL, TYPE_BGR_DBL, TYPE_CMYK_DBL,
    TYPE_XYZ_FLT (duplicate), and possible HALF variants guarded by CMS_NO_HALF_SUPPORT:
    TYPE_GRAY_HALF_FLT, TYPE_RGB_HALF_FLT, TYPE_CMYK_HALF_FLT, TYPE_RGBA_HALF_FLT,
    TYPE_ARGB_HALF_FLT, TYPE_BGR_HALF_FLT, TYPE_BGRA_HALF_FLT, TYPE_ABGR_HALF_FLT
  - CMS_NO_HALF_SUPPORT (compile-time switch affecting inclusion of HALF types)
  - Return expression: FormatterFailed == 0 ? 1 : 0
  - External dependencies in <FOCAL_CLASS_DEP> (testcms2.h) provide the required
    type definitions and the C interface.

- Test coverage goals achieved:
  - Validate normal operation (returns 1) in a standard environment.
  - Validate that the function does not leave FormatterFailed non-zero (post-condition check).
  - Validate idempotence by calling the function twice and ensuring consistent results.

- Domain knowledge considerations:
  - We avoid private/internal details; tests rely on the public function signature and its documented side effects.
  - Static file-scope helpers are not accessed directly; the tests rely on extern symbols provided by the C layer.

- Note on HALF types:
  - The test suite is designed to be built in an environment where the CMS_NO_HALF_SUPPORT flag can be toggled
    by the build system. The test code itself remains valid regardless of the presence of half-type
    variants, since it relies on the public interface of CheckFormattersFloat. If needed, a separate
    test build with CMS_NO_HALF_SUPPORT defined can be added to exercise the variant without HALF types.
*/
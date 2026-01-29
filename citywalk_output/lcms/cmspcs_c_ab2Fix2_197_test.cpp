/*
Step 1 - Program Understanding (Comments for traceability)
- Focal method under test: cmsUInt16Number ab2Fix2(cmsFloat64Number ab)
  Behavior: return _cmsQuickSaturateWord((ab + 128.0) * 256.0);
- Core dependent components (Candidate Keywords):
  - ab (input value)
  - 128.0 (offset)
  - 256.0 (scaling)
  - _cmsQuickSaturateWord (saturating casting helper to 0..65535)
- This function clamps the result of (ab + 128) * 256 to the range of a 16-bit unsigned value.
- We will test true/false branches by exercising ab values below, at, and above the saturation boundary.

Step 2 & 3 - Unit Test Generation and Refinement (C++11 test suite, no GTest)
- Build strategy: Provide a small C++11 test program that directly calls ab2Fix2.
- Use a lightweight, non-terminating assertion style (printing PASS/FAIL but continuing).
- Include the C internal header via extern "C" and declare the function prototype accordingly.
- Cover a range of inputs to exercise both non-saturating and saturating paths, including boundary conditions.

Note: This test suite avoids private members access, uses only standard library, and calls the focal function through C linkage to ensure compatibility with the mixed C/C++ codebase.

Code (unit tests):

*/

#include <limits>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Bring in types used by the focal function
extern "C" {
}

// Prototype for the focal function under test (assuming it's not declared in the above header)
extern "C" cmsUInt16Number ab2Fix2(cmsFloat64Number ab);

// Simple non-terminating assertion helper: returns true if equal, prints diagnostic otherwise
static bool assert_equal(const std::string& name,
                         cmsFloat64Number input,
                         cmsUInt16Number expected,
                         cmsUInt16Number actual)
{
    bool ok = (actual == expected);
    std::cout << "[" << name << "] input=" << input
              << " | expected=" << static_cast<unsigned>(expected)
              << " | actual=" << static_cast<unsigned>(actual)
              << " -> " << (ok ? "PASS" : "FAIL") << std::endl;
    if (!ok) {
        // Detailed diagnostic for easier debugging
        std::cerr << "  Diagnostic: ab2Fix2(" << input << ") produced " << actual
                  << ", but expected " << expected << std::endl;
    }
    return ok;
}

// Run a single test case
static void run_case(const std::string& name, cmsFloat64Number input, cmsUInt16Number expected)
{
    cmsUInt16Number got = ab2Fix2(input);
    (void)assert_equal(name, input, expected, got);
}

// Entry point: execute a battery of test cases covering true/false branches and boundary conditions
int main() {
    // Summary counters
    int total = 0;
    int passed = 0;

    auto t = [&](const std::string& n, cmsFloat64Number in, cmsUInt16Number exp) {
        total++;
        cmsUInt16Number got = ab2Fix2(in);
        if (got == exp) {
            passed++;
            std::cout << "[TEST PASSED] " << n << "\n";
        } else {
            std::cout << "[TEST FAILED] " << n << "\n";
            std::cerr << "  Expected: " << exp << ", Got: " << got << "\n";
        }
    };

    // Individual test cases (edge cases, boundaries, typical values)

    // Negative inputs below -128.0 should saturate to 0
    t("NegLarge", -2000.0, 0);
    t("Neg128", -128.0, 0);
    // Just below the boundary - should still saturate to 0 due to non-positive result after scaling
    t("NegJustAboveBoundary", -127.999999, 0);

    // Zero input
    t("Zero", 0.0, 32768); // (0 + 128) * 256 = 32768

    // Positive values within range (non-saturating)
    t("LowMid", 50.0, 178 * 256 == 45568 ? 45568 : 0); // 178*256 = 45568
    t("Mid", 100.0, 228 * 256);                          // 228*256 = 58368
    t("HighNearMax", 127.5, 255.5 * 256);                // 65408
    // Boundary case that should saturate to max 65535
    t("MaxAllowedBoundary", 127.99609375, 65535);        // (255.99609375)*256 = 65535
    // At and above 128 should saturate to 65535
    t("SaturateAt128", 128.0, 65535);
    t("AboveMax", 200.0, 65535);
    t("VeryLarge", 1e6, 65535);

    // Final summary
    std::cout << "\nTotal tests: " << total << ", Passed: " << passed
              << ", Failed: " << (total - passed) << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}

/*
Notes:
- The tests rely on exact arithmetic for the chosen inputs to avoid floating-point rounding pitfalls.
- The domain coverage includes:
  - Negative values (below and at the lower bound) -> expect 0
  - Zero input -> exact 32768
  - Mid-range inputs -> non-saturated values
  - Values near the upper saturation boundary -> exact 65535
  - Values above the saturation threshold -> 65535
- If the project's build system uses separate compilation units for C and C++ files, ensure that:
  - cmspcs.c (containing ab2Fix2) is compiled and linked with this test.
  - The header path "lcms2_internal.h" is resolvable from the test's include paths.
- This test suite uses only the C/C++ standard library and the provided CMS types; no GTest or mocking frameworks are used.
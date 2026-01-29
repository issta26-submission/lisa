/*
  Test suite for the focal method Fake3D16 located in testplugin.c

  This file intentionally does not rely on any testing framework (e.g., GTest),
  in accordance with the project constraints. It provides a small, self-contained
  test harness that executes multiple unit tests for the function:

      void Fake3D16(CMSREGISTER const cmsUInt16Number Input[],
                    CMSREGISTER cmsUInt16Number Output[],
                    CMSREGISTER const struct _cms_interp_struc* p)

  Behavior of Fake3D16 under test:
  - It computes Output[0] = 0xFFFF - Input[2]
  - Output[1] = 0xFFFF - Input[1]
  - Output[2] = 0xFFFF - Input[0]
  - The function disregards the interpolation structure 'p' (the field is unused in the body).

  Key dependencies:
  - testcms2.h is the primary header providing types/macros like CMSREGISTER,
    cmsUInt16Number, and struct _cms_interp_struc.

  Design notes:
  - We exercise true/false branches implicitly by varying input values including
    edge cases (0, 0xFFFF), typical values, and mixed values.
  - We test with p as nullptr and with a non-null pointer to verify no reliance on p.
  - We keep tests lightweight and deterministic, focusing on functional correctness.
  - Output is printed to stdout with explanations to aid debugging, using a simple
    non-terminating assertion approach (continue after a failure).
*/

#include <cstring>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>
#include <cstdint>



// Declaration of the focal function (C linkage)
extern "C" void Fake3D16(CMSREGISTER const cmsUInt16Number Input[],
                       CMSREGISTER cmsUInt16Number Output[],
                       CMSREGISTER const struct _cms_interp_struc* p);

/* Domain-specific helpers */

// Compare two 3-element cmsUInt16Number arrays for exact equality
static bool equal3(const cmsUInt16Number a[3], const cmsUInt16Number b[3]) {
    for (int i = 0; i < 3; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Pretty-print a 3-element array for debugging
static std::string to_string3(const cmsUInt16Number a[3]) {
    std::ostringstream oss;
    oss << "[ " << std::setw(6) << static_cast<uint32_t>(a[0]) << ", "
        << std::setw(6) << static_cast<uint32_t>(a[1]) << ", "
        << std::setw(6) << static_cast<uint32_t>(a[2]) << " ]";
    return oss.str();
}

// Run a single test case with given inputs and expected outputs.
// Returns true on pass, false on fail. The test is named in 'name' for logging.
static bool run_case(const char* name,
                     const cmsUInt16Number input[3],
                     const cmsUInt16Number expected[3],
                     const std::string& preface = "",
                     bool use_non_null_p = false)
{
    cmsUInt16Number out[3] = {0, 0, 0};
    // Prepare a dummy interpolation structure if requested
    struct _cms_interp_struc dummy_interp;
    const struct _cms_interp_struc* p = nullptr;
    if (use_non_null_p) {
        p = &dummy_interp;
    }

    // Call the focal function
    Fake3D16(input, out, p);

    bool ok = equal3(out, expected);

    // Logging
    std::cout << preface;
    std::cout << "Test '" << name << "': Input = " << to_string3(input)
              << " -> Output = " << to_string3(out)
              << " | Expected = " << to_string3(expected)
              << " | p=" << (p ? "non-null" : "nullptr") << "\n";

    if (!ok) {
        std::cout << "  [FAIL] Mismatch detected in test '" << name << "'.\n";
    } else {
        std::cout << "  [PASS] Test '" << name << "' succeeded.\n";
    }

    return ok;
}

/* Individual test cases */

// 1) Basic sanity: Input {1, 2, 3} -> Output {0xFFFF-3, 0xFFFF-2, 0xFFFF-1}
static bool test_case_basic_reverse() {
    const cmsUInt16Number input[3]  = {1, 2, 3};
    const cmsUInt16Number expected[3] = {0xFFFF - 3, 0xFFFF - 2, 0xFFFF - 1};
    return run_case("BasicReverse", input, expected, "", false);
}

// 2) Edge values: Input {0, 1, 2} -> Output {0xFFFF-2, 0xFFFF-1, 0xFFFF-0}
static bool test_case_edge_values() {
    const cmsUInt16Number input[3]  = {0, 1, 2};
    const cmsUInt16Number expected[3] = {0xFFFF - 2, 0xFFFF - 1, 0xFFFF - 0};
    return run_case("EdgeValues", input, expected, "", false);
}

// 3) All zeros: Input {0, 0, 0} -> Output {0xFFFF, 0xFFFF, 0xFFFF}
static bool test_case_all_zeros() {
    const cmsUInt16Number input[3]  = {0, 0, 0};
    const cmsUInt16Number expected[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    return run_case("AllZeros", input, expected, "", false);
}

// 4) All max: Input {0xFFFF, 0xFFFF, 0xFFFF} -> Output {0, 0, 0}
static bool test_case_all_max() {
    const cmsUInt16Number input[3]  = {0xFFFF, 0xFFFF, 0xFFFF};
    const cmsUInt16Number expected[3] = {0, 0, 0};
    return run_case("AllMax", input, expected, "", false);
}

// 5) Random mixed values: Input {0x1234, 0xABCD, 0x0F0F}
static bool test_case_random_mixed() {
    const cmsUInt16Number input[3]  = {0x1234, 0xABCD, 0x0F0F};
    const cmsUInt16Number expected[3] = {0xFFFF - 0x0F0F, 0xFFFF - 0xABCD, 0xFFFF - 0x1234};
    return run_case("RandomMixed", input, expected, "", false);
}

// 6) Non-null p pointer should not affect results (p is ignored by design)
static bool test_case_non_null_p_no_effect() {
    const cmsUInt16Number input[3]  = {0x0102, 0x0304, 0x0506};
    const cmsUInt16Number expected[3] = {0xFFFF - 0x0506, 0xFFFF - 0x0304, 0xFFFF - 0x0102};
    // Run with p as non-null
    return run_case("NonNullP", input, expected, "(NonNullP) ", true);
}

/* Domain-specific notes incorporated in test comments:
   - We exercise inputs that exercise bit patterns at both ends of the range
     and typical values to ensure the inversion and reversed-index logic is correct.
   - Tests do not rely on the private/internal state of any classes; only the
     public focal function is exercised.
   - All tests are self-contained and do not require GTest; results are printed
     to stdout for easy inspection in environments where only standard C++ is allowed.
*/

int main() {
    std::cout << "Starting unit tests for Fake3D16...\n" << std::endl;

    int total = 0;
    int passed = 0;

    auto t = [&](const char* name, bool result) {
        ++total;
        if (result) {
            ++passed;
            std::cout << "[OK] " << name << std::endl;
        } else {
            std::cout << "[ERR] " << name << std::endl;
        }
        std::cout << std::endl;
    };

    t("BasicReverse", test_case_basic_reverse());
    t("EdgeValues", test_case_edge_values());
    t("AllZeros", test_case_all_zeros());
    t("AllMax", test_case_all_max());
    t("RandomMixed", test_case_random_mixed());
    t("NonNullP", test_case_non_null_p_no_effect());

    std::cout << "Unit test summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Non-zero exit code if any test failed
    return (passed == total) ? 0 : 1;
}
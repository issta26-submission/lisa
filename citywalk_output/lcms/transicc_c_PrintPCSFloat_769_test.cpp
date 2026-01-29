// Test suite for the focal method: PrintPCSFloat in transicc.c
// This test suite is designed to be compiled in a C++11 project without GTest.
// It provides lightweight C-style tests with explanatory comments.
// The tests rely on a mock of cmsDoTransform to avoid pulling the actual (external) library.
// Domain knowledge and constraints from the prompt are followed.

#include <cstring>
#include <iostream>
#include <cstdio>
#include <utils.h>


// ---------------------------------------------------------------------------------
// Domain and integration notes (informational, not code-required):
// - The focal method PrintPCSFloat uses global state Verbose, hTransXYZ, hTransLab.
// - It conditionally prints PCS values when Verbose > 1 and both transforms are non-null.
// - It calls cmsDoTransform for XYZ and Lab transforms if non-null, respectively.
// - Our test provides a mock cmsDoTransform to verify that the correct code paths are taken
//   without requiring the real Little CMS backend.
// ---------------------------------------------------------------------------------

// Minimal type definitions to align with the expected prototypes.
// We use a typedef double for cmsFloat64Number as is common in such codebases.
typedef double cmsFloat64Number;

// Forward declarations of the focal function and the external/global state used by it.
// These are provided to allow linking against transicc.c without including its headers.
extern "C" void PrintPCSFloat(cmsFloat64Number Input[]);

// Extern globals from the file under test (transicc.c). We declare them here to drive/test logic.
extern "C" int Verbose;
extern "C" void* hTransXYZ;
extern "C" void* hTransLab;

// Prototype for the cmsDoTransform used by transicc.c. We provide a mock implementation below.
extern "C" void cmsDoTransform(void* hTransform, const void* In, void* Out, unsigned int size);

// ---------------------------------------------------------------------------------
// Mock state and helpers to verify call paths without relying on the real CMS transforms.
// ---------------------------------------------------------------------------------
static void* g_expectedXYZ = NULL;
static void* g_expectedLab = NULL;

static int g_xyz_calls = 0;
static int g_lab_calls = 0;

static inline void reset_call_counts() {
    g_xyz_calls = 0;
    g_lab_calls = 0;
}

// The mock cmsDoTransform acts based on which transform handle is being used.
// It increments a counter for each recognized transform and writes deterministic values
// to the output buffers so the tests can exercise the code paths safely.
extern "C" void cmsDoTransform(void* hTransform, const void* In, void* Out, unsigned int size) {
    (void)In; // Unused in mock
    (void)size; // Unused in mock

    if (hTransform == g_expectedXYZ) {
        // Write deterministic XYZ-like memory layout: X, Y, Z as doubles.
        double* p = (double*)Out;
        p[0] = 0.1234;
        p[1] = 0.2345;
        p[2] = 0.3456;
        g_xyz_calls++;
    } else if (hTransform == g_expectedLab) {
        // Write deterministic Lab-like memory layout: L, a, b as doubles.
        double* p = (double*)Out;
        p[0] = 50.0;
        p[1] = -12.0;
        p[2] = 7.0;
        g_lab_calls++;
    }
}

// Helper to set expected handles so the mock can distinguish between XYZ and Lab transforms.
static inline void set_expected_handles(void* xyz, void* lab) {
    g_expectedXYZ = xyz;
    g_expectedLab = lab;
}

// ---------------------------------------------------------------------------------
// Lightweight test harness (no GTest). Uses simple boolean checks and prints results.
// We rely on the instruction to use non-terminating assertions and report per-test status.
// ---------------------------------------------------------------------------------

// Test A: Verbose > 1 with both hTransXYZ and hTransLab present. Expect both cmsDoTransform calls.
// This covers the true-branch of the condition: Verbose > 1 && hTransXYZ && hTransLab
bool test_A_verbose_both_transforms() {
    reset_call_counts();
    Verbose = 2;
    hTransXYZ = (void*)0x1001;
    hTransLab  = (void*)0x2002;
    set_expected_handles(hTransXYZ, hTransLab);

    cmsFloat64Number input[3] = {0.0, 0.0, 0.0};
    PrintPCSFloat(input);

    // We expect both transforms to have been invoked exactly once.
    bool ok = (g_xyz_calls == 1) && (g_lab_calls == 1);
    // Additionally, ensure that the function attempted to print (indirectly verified by calls).
    return ok;
}

// Test B: Verbose > 1 with only hTransXYZ present (hTransLab is NULL).
// This still exercises the part that calls cmsDoTransform for XYZ and skips Lab.
// It also verifies the top-level condition becomes false due to hTransLab being NULL.
bool test_B_verbose_only_xyz() {
    reset_call_counts();
    Verbose = 2;
    hTransXYZ = (void*)0x1010;
    hTransLab  = NULL;
    set_expected_handles(hTransXYZ, hTransLab);

    cmsFloat64Number input[3] = {0.0, 0.0, 0.0};
    PrintPCSFloat(input);

    // Expect only XYZ to be called.
    bool ok = (g_xyz_calls == 1) && (g_lab_calls == 0);
    return ok;
}

// Test C: Verbose <= 1 with both transforms present. Expect no cmsDoTransform calls.
// This covers the false branch of the top-level condition.
bool test_C_verbose_false_branch() {
    reset_call_counts();
    Verbose = 1;
    hTransXYZ = (void*)0x1111;
    hTransLab  = (void*)0x2222;
    set_expected_handles(hTransXYZ, hTransLab);

    cmsFloat64Number input[3] = {0.0, 0.0, 0.0};
    PrintPCSFloat(input);

    // No transform calls should be made.
    bool ok = (g_xyz_calls == 0) && (g_lab_calls == 0);
    return ok;
}

// Test D: Verbose > 1 with only hTransLab present (hTransXYZ NULL).
// This exercises the Lab-side transform path even when XYZ is missing.
bool test_D_verbose_only_lab() {
    reset_call_counts();
    Verbose = 3;
    hTransXYZ = NULL;
    hTransLab  = (void*)0x3333;
    set_expected_handles(hTransXYZ, hTransLab);

    cmsFloat64Number input[3] = {0.0, 0.0, 0.0};
    PrintPCSFloat(input);

    // Expect only Lab to be called.
    bool ok = (g_xyz_calls == 0) && (g_lab_calls == 1);
    return ok;
}

// Utility to print test result
static inline void print_result(const char* test_name, bool passed) {
    std::cout << test_name << ": " << (passed ? "PASS" : "FAIL") << std::endl;
}

// ---------------------------------------------------------------------------------
// Main entry point: run all tests in sequence and report.
// Per the guidelines, gtest macros are not used; tests are invoked from main.
// ---------------------------------------------------------------------------------

int main() {
    bool a = test_A_verbose_both_transforms();
    print_result("Test A (verbose>1 with both transforms)", a);

    bool b = test_B_verbose_only_xyz();
    print_result("Test B (verbose>1 with only XYZ)", b);

    bool c = test_C_verbose_false_branch();
    print_result("Test C (verbose<=1 with transforms)", c);

    bool d = test_D_verbose_only_lab();
    print_result("Test D (verbose>1 with only Lab)", d);

    // Summary
    int failures = 0;
    if (!a) ++failures;
    if (!b) ++failures;
    if (!c) ++failures;
    if (!d) ++failures;

    std::cout << "Summary: " << (4 - failures) << "/4 tests passed." << std::endl;

    return (failures == 0) ? 0 : 1;
}

// ---------------------------------------------------------------------------------
// Notes for maintainers:
// - This test suite avoids GTest and uses simple boolean checks with descriptive output.
// - The tests rely on a mock cmsDoTransform; ensure that transicc.c is linked with this test.
// - The domain knowledge prompt was used to guide coverage of true/false branches and
//   to avoid touching private/internal methods directly.
// - Static members: we interact with external/global state Verbose and hTransXYZ/HTransLab as provided.
// - The tests call PrintPCSFloat via C linkage to exercise the focal method.
// ---------------------------------------------------------------------------------
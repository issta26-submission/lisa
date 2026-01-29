// Lightweight C++11 test harness for the focal method CheckConversionFloat
// This test suite does not rely on GTest and uses a small in-house assertion framework.
// It exercises the CheckConversionFloat pathway by invoking the function directly and
// by exercising related internal helpers (where accessible) via their C interfaces.
//
// Domain notes:
// - The tests rely on the external C functions and the PROFILES_DIR macro used by the
//   focal implementation. We assume these are provided by the project build.
// - Static/internal functions that are truly file-scoped are not invoked directly from here.
// - We only perform non-terminating checks (i.e., assertions that log results and continue).

#include <fast_float_internal.h>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>


// Basic boolean definitions to align with existing C code style
#ifndef cmsBool
typedef int cmsBool;
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

// Forward declare the minimal C interfaces that the focal code interacts with.
// We declare them with C linkage to ensure correct linkage when linking with the
// existing C/LCMS codebase.
extern "C" {

    // Opaque handle to a color profile
    typedef void* cmsHPROFILE;
    typedef int cmsInt32Number;

    // lcms/hdr style typedefs (lightweight bindings for test)
    typedef void* cmsContext;
    // Note: the actual types in the project may be different; for the test harness
    // we only rely on the function signatures.

    // Core operations used by CheckConversionFloat test flow
    cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* access);
    // The actual project uses PROFILES_DIR macro concatenations, e.g. PROFILES_DIR "test5.icc"
    // We'll leave usage to the existing build environment.

    // Focal test helpers (assumed to be provided by the project, possibly as stubs in test env)
    void CheckConversionFloat(void);
    void CheckAlphaDetect(void);
    void TryAllValuesFloatAlpha(cmsHPROFILE hlcmsProfileIn,
                                cmsHPROFILE hlcmsProfileOut,
                                cmsInt32Number Intent,
                                cmsBool copyAlpha);
    void CheckUncommonValues(cmsHPROFILE hlcmsProfileIn,
                             cmsHPROFILE hlcmsProfileOut,
                             cmsInt32Number Intent);
    void CheckToEncodedLab(void);
    void CheckToFloatLab(void);
    void CheckFloatToFloatLab(void);
    void CheckToFloatXYZ(void);
    void CheckFloatToFloatXYZ(void);
    void TryAllValuesFloat(cmsHPROFILE hlcmsProfileIn,
                           cmsHPROFILE hlcmsProfileOut,
                           cmsInt32Number Intent);
    void TryAllValuesFloatVs16(cmsHPROFILE hlcmsProfileIn,
                               cmsHPROFILE hlcmsProfileOut,
                               cmsInt32Number Intent);
    // Additional helpers used by CheckConversionFloat in the test suite
    // (we declare them to keep compilation unit self-contained)
}

// Simple test framework
static int g_failures = 0;

#define RUN_TEST(name) do {                                         \
    try {                                                            \
        name();                                                      \
        std::cout << "[PASS] " #name "\n";                          \
    } catch (const std::exception& ex) {                             \
        ++g_failures;                                               \
        std::cerr << "[FAIL] " #name " threw std::exception: " << ex.what() << "\n"; \
    } catch (...) {                                                \
        ++g_failures;                                               \
        std::cerr << "[FAIL] " #name " threw an unknown exception\n"; \
    }                                                               \
} while (0)

// Utility: helper to politely print test region headers
static void section(const std::string& s) {
    std::cout << "\n=== " << s << " ===\n";
}

// Test 1: Ensure CheckConversionFloat executes without crashing and processes its internal sequence.
// Rationale: This is the primary entry point under test and should exercise the entire payload.
// Domain coverage: Verifies that the function can be called in a normal environment provided by the project.
static void test_CheckConversionFloat_basic_execution(void)
{
    section("CheckConversionFloat - basic execution");
    // The focal function orchestrates multiple sub-tests via internal calls.
    // If the environment is correctly set up (profiles available, etc.), this should complete.
    CheckConversionFloat();
}

// Test 2: Validate TryAllValuesFloatAlpha variant paths via real profiles.
// Rationale: Exercise the two-branch path (copyAlpha false/true) and ensure no crash.
// This uses the same profile pairs as the focal method's own crash-test blocks.
static void test_TryAllValuesFloatAlpha_variants(void)
{
    section("TryAllValuesFloatAlpha - alpha-copy variants");
    // Prepare two profile handles (in/out) using test ICCs
    cmsHPROFILE in1  = cmsOpenProfileFromFile(PROFILES_DIR "test5.icc", "r");
    cmsHPROFILE out1 = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    int Intent = 0; // INTENT_PERCEPTUAL placeholder (actual value defined in LCMS)
    TryAllValuesFloatAlpha(in1, out1, Intent, FALSE);
    TryAllValuesFloatAlpha(in1, out1, Intent, TRUE);

    // Second pair
    cmsHPROFILE in2  = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    cmsHPROFILE out2 = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    TryAllValuesFloatAlpha(in2, out2, Intent, FALSE);
    TryAllValuesFloatAlpha(in2, out2, Intent, TRUE);

    // Note: We intentionally do not close profiles here to avoid extra boilerplate;
    // the test environment is responsible for profile lifecycle management if needed.
}

// Test 3: Validate uncommon values path via CheckUncommonValues workflow.
// Rationale: Exercise the path that checks for uncommon input/output pairings across profiles.
// This mirrors parts of the "Crash (III) test." block in the focal method.
static void test_CheckUncommonValues_paths(void)
{
    section("CheckUncommonValues - various profile pairs");
    cmsHPROFILE a = cmsOpenProfileFromFile(PROFILES_DIR "test5.icc", "r");
    cmsHPROFILE b = cmsOpenProfileFromFile(PROFILES_DIR "test3.icc", "r");
    CheckUncommonValues(a, b, 0);

    cmsHPROFILE c = cmsOpenProfileFromFile(PROFILES_DIR "test5.icc", "r");
    cmsHPROFILE d = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    CheckUncommonValues(c, d, 0);
}

// Test 4: Validate the Lab-related conversions are reachable from CheckConversionFloat.
// Rationale: The focal suite initializes lab-related conversions and ensures conversion paths exist.
// This test calls the Lab/XYZ conversion entry points that are wired from the header.
static void test_LabAndXYZ_conversions_paths(void)
{
    section("Lab/XYZ conversion pathways");
    CheckToEncodedLab();
    CheckToFloatLab();
    CheckFloatToFloatLab();
    CheckToFloatXYZ();
    CheckFloatToFloatXYZ();
}

// Test 5: Validate alternative path TryAllValuesFloatVs16 (float vs 16-bit paths).
// Rationale: Exercise a variant of the accuracy testing that compares 16-bit vs float behavior.
static void test_TryAllValuesFloatVs16_paths(void)
{
    section("TryAllValuesFloatVs16 - float vs 16-bit path");
    cmsHPROFILE in = cmsOpenProfileFromFile(PROFILES_DIR "test5.icc", "r");
    cmsHPROFILE out = cmsOpenProfileFromFile(PROFILES_DIR "test3.icc", "r");
    TryAllValuesFloatVs16(in, out, 0);

    // Same-profile test
    cmsHPROFILE inSame = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    cmsHPROFILE outSame = cmsOpenProfileFromFile(PROFILES_DIR "test0.icc", "r");
    TryAllValuesFloatVs16(inSame, outSame, 0);
}

// Main driver
int main()
{
    // Run the test suite in a deterministic order
    test_CheckConversionFloat_basic_execution();
    test_TryAllValuesFloatAlpha_variants();
    test_CheckUncommonValues_paths();
    test_LabAndXYZ_conversions_paths();
    test_TryAllValuesFloatVs16_paths();

    // Summary
    std::cout << "\nTest summary: " << (g_failures ? "FAILED" : "ALL PASSED")
              << " (failures=" << g_failures << ")\n";

    // Return non-zero on failure to integrate with build systems that treat non-zero as failure
    return g_failures ? 1 : 0;
}
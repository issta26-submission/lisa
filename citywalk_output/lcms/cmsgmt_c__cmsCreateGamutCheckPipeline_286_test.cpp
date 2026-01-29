// Lightweight C++11 test harness for the focal function:
// _cmsCreateGamutCheckPipeline in cmsgmt.c
// This test suite avoids GTest and uses a small custom framework.
// It exercises a few basic control paths of the function.

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS public API

// Provide C linkage declaration for the focal function if not visible via header
extern "C" cmsPipeline* _cmsCreateGamutCheckPipeline(
    cmsContext ContextID,
    cmsHPROFILE hProfiles[],
    cmsBool  BPC[],
    cmsUInt32Number Intents[],
    cmsFloat64Number AdaptationStates[],
    cmsUInt32Number nGamutPCSposition,
    cmsHPROFILE hGamut);

// Simple non-terminating test framework
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

static bool runTest(const TestCase& tc) {
    bool result = true;
    try {
        result = tc.func();
    } catch (...) {
        std::cerr << "Test threw an exception: " << tc.name << std::endl;
        return false;
    }
    std::cout << (result ? "[PASS] " : "[FAIL] ") << tc.name << std::endl;
    return result;
}

// Helper: safe profile creation (returns NULL on failure, caller must check)
static cmsHPROFILE CreateSRGBProfile() {
    return cmsCreate_sRGBProfile();
}

// Test 1: Ensure that nGamutPCSposition = 0 yields NULL (early guard)
static bool test_zero_position() {
    // Explanatory:
    // The function should reject non-positive PCS position values and return NULL.
    std::cout << "Comment: Verifying early guard when nGamutPCSposition <= 0." << std::endl;

    cmsHPROFILE hProfiles[1] = { CreateSRGBProfile() };
    cmsHPROFILE hGamut = CreateSRGBProfile();

    cmsBool     BPC[1] = { 0 };
    cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    cmsFloat64Number AdaptationStates[1] = { 1.0 };
    cmsUInt32Number nGamutPCSposition = 0; // Trigger early exit

    cmsPipeline* p = _cmsCreateGamutCheckPipeline(
        NULL,            // Context
        hProfiles,
        BPC,
        Intents,
        AdaptationStates,
        nGamutPCSposition,
        hGamut);

    bool ok = (p == NULL);
    if (p) cmsPipelineFree(p);
    if (hGamut) cmsCloseProfile(hGamut);
    if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
    return ok;
}

// Test 2: Ensure that nGamutPCSposition > 255 yields NULL (out-of-range guard)
static bool test_out_of_range_position() {
    // Explanatory:
    // The function should reject positions greater than 255 and return NULL.
    std::cout << "Comment: Verifying guard when nGamutPCSposition > 255." << std::endl;

    cmsHPROFILE hProfiles[1] = { CreateSRGBProfile() };
    cmsHPROFILE hGamut = CreateSRGBProfile();

    cmsBool     BPC[1] = { 0 };
    cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    cmsFloat64Number AdaptationStates[1] = { 1.0 };
    cmsUInt32Number nGamutPCSposition = 256; // Out-of-range

    cmsPipeline* p = _cmsCreateGamutCheckPipeline(
        NULL,
        hProfiles,
        BPC,
        Intents,
        AdaptationStates,
        nGamutPCSposition,
        hGamut);

    bool ok = (p == NULL);
    if (p) cmsPipelineFree(p);
    if (hGamut) cmsCloseProfile(hGamut);
    if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
    return ok;
}

// Test 3: Basic positive path (minimal valid input) should produce a non-NULL pipeline
static bool test_basic_positive_path() {
    // Explanatory:
    // Provide a minimal valid setup: a single input profile and a gamut profile.
    // Expect a non-NULL pipeline to be returned.
    std::cout << "Comment: Verifying basic positive path with nGamutPCSposition = 1." << std::endl;

    cmsHPROFILE hGamut = CreateSRGBProfile();
    cmsHPROFILE hProfiles[1] = { CreateSRGBProfile() };

    cmsBool     BPC[1] = { 0 };
    cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    cmsFloat64Number AdaptationStates[1] = { 1.0 };
    cmsUInt32Number nGamutPCSposition = 1; // Lab is appended inside function

    cmsPipeline* p = _cmsCreateGamutCheckPipeline(
        NULL,
        hProfiles,
        BPC,
        Intents,
        AdaptationStates,
        nGamutPCSposition,
        hGamut);

    bool ok = (p != NULL);
    if (p) cmsPipelineFree(p);
    if (hGamut) cmsCloseProfile(hGamut);
    if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
    return ok;
}

int main() {
    // Assemble test suite
    std::vector<TestCase> tests = {
        {"cmsCreateGamutCheckPipeline: zero position", test_zero_position},
        {"cmsCreateGamutCheckPipeline: out-of-range position", test_out_of_range_position},
        {"cmsCreateGamutCheckPipeline: basic positive path", test_basic_positive_path}
    };

    int failures = 0;
    for (const auto& t : tests) {
        if (!runTest(t)) ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}
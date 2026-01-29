// C++11 test suite for InitCargo in mkcmy.c without using GTest
// This test directly exercises the InitCargo function by supplying a minimal
// LPCARGO-like structure and validating that the function initializes its fields
// (Lab/RGB profiles and the two color transforms) without crashing.
// The tests rely on Little CMS (lcms2) library functions which are linked at build time.

#include <cstring>
#include <iostream>
#include <lcms2.h>
#include <lcms.h>


// Include Little CMS header for types and functions

// Forward declaration of the focal function from mkcmy.c
// The exact LPCARGO definition is not provided here, so we recreate a compatible one
// that mirrors the usage inside InitCargo (hLab, hRGB, Lab2RGB, RGB2Lab).
extern "C" void InitCargo(/*LPCARGO*/ void* Cargo); // We'll provide a compatible prototype below

// Provide a compatible LPCARGO definition for tests.
// We assume the actual mkcmy.c defines something like:
// typedef struct _LPCARGO { cmsHPROFILE hLab; cmsHPROFILE hRGB; cmsHTRANSFORM Lab2RGB; cmsHTRANSFORM RGB2Lab; } *LPCARGO;
typedef struct _LPCARGO_COMPAT {
    cmsHPROFILE hLab;
    cmsHPROFILE hRGB;
    cmsHTRANSFORM Lab2RGB;
    cmsHTRANSFORM RGB2Lab;
} LPCARGO_COMPAT;
typedef LPCARGO_COMPAT* LPCARGO;

// Re-declare InitCargo with the compatible type for linkage
extern "C" void InitCargo(LPCARGO Cargo);

static bool test_InitCargo_MakesNonNullFields() {
    // Test that InitCargo initializes all four members to non-null values.
    // Explanatory: Ensure Lab/RGB profiles and both transforms are created.
    LPCARGO cargo = new LPCARGO_COMPAT();
    std::memset(cargo, 0, sizeof(LPCARGO_COMPAT));

    // Call the focal function
    InitCargo(cargo);

    bool labNonNull = (cargo->hLab != NULL);
    bool rgbNonNull = (cargo->hRGB != NULL);
    bool lab2rgbNonNull = (cargo->Lab2RGB != NULL);
    bool rgb2labNonNull = (cargo->RGB2Lab != NULL);

    // Clean-up resources if creation happened
    if (labNonNull) cmsCloseProfile(cargo->hLab);
    if (rgbNonNull) cmsCloseProfile(cargo->hRGB);
    if (lab2rgbNonNull) cmsDeleteTransform(cargo->Lab2RGB);
    if (rgb2labNonNull) cmsDeleteTransform(cargo->RGB2Lab);

    delete cargo;

    if (labNonNull && rgbNonNull && lab2rgbNonNull && rgb2labNonNull) {
        std::cout << "[PASS] InitCargo_MakesNonNullFields: All fields initialized (non-null).\n";
        return true;
    } else {
        std::cerr << "[FAIL] InitCargo_MakesNonNullFields: Some field(s) remained null. "
                  << "lab=" << labNonNull << ", rgb=" << rgbNonNull
                  << ", Lab2RGB=" << lab2rgbNonNull
                  << ", RGB2Lab=" << rgb2labNonNull << "\n";
        return false;
    }
}

// Explanatory: This test checks that the two created transforms are distinct handles.
// If the library returns the same handle for both, it could indicate a reuse bug.
static bool test_InitCargo_TransformsAreDistinct() {
    LPCARGO cargo = new LPCARGO_COMPAT();
    std::memset(cargo, 0, sizeof(LPCARGO_COMPAT));

    InitCargo(cargo);

    bool distinct = (cargo->Lab2RGB != NULL) && (cargo->RGB2Lab != NULL) && (cargo->Lab2RGB != cargo->RGB2Lab);

    // Clean-up
    if (cargo->Lab2RGB != NULL) cmsDeleteTransform(cargo->Lab2RGB);
    if (cargo->RGB2Lab != NULL) cmsDeleteTransform(cargo->RGB2Lab);
    if (cargo->hLab != NULL) cmsCloseProfile(cargo->hLab);
    if (cargo->hRGB != NULL) cmsCloseProfile(cargo->hRGB);

    delete cargo;

    if (distinct) {
        std::cout << "[PASS] InitCargo_TransformsAreDistinct: Lab2RGB and RGB2Lab are distinct handles.\n";
        return true;
    } else {
        std::cerr << "[FAIL] InitCargo_TransformsAreDistinct: Lab2RGB and RGB2Lab are the same handle or not created.\n";
        return false;
    }
}

// Minimal helper to satisfy extern "C" prototype if symbol name mangling differs.
// If the real mkcmy.c uses a different signature for InitCargo, you can adapt
// the function prototype accordingly in both the test and the build.
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok){
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[TEST PASSED] " << name << "\n";
        } else {
            std::cerr << "[TEST FAILED] " << name << "\n";
        }
    };

    // Run tests (explanatory comments accompany each test above)
    run("InitCargo_MakesNonNullFields", test_InitCargo_MakesNonNullFields());
    run("InitCargo_TransformsAreDistinct", test_InitCargo_TransformsAreDistinct());

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
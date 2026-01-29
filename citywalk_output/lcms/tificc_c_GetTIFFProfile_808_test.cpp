// Lightweight C++11 unit test suite for cmsHPROFILE GetTIFFProfile(TIFF* in)
// This test suite does not rely on GoogleTest or any external test framework.
// It uses a simple main() function to run tests and prints results to stdout.
// It relies on libtiff and LittleCMS (lcms2) being available in the environment.

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <iostream>
#include <stdint.h>
#include <cmath>
#include <cstdlib>
#include <lcms2.h>
#include <utils.h>
#include <tiffio.h>


// Include TIFF and LCMS headers for real testing of behavior.

// Declare the focal function from the C source so we can call it from C++.
// The function has C linkage; we wrap the declaration with extern "C".
extern "C" cmsHPROFILE GetTIFFProfile(TIFF* in);

// External global flags used by the focal method.
// We declare them here so tests can manipulate behavior if needed.
extern int IgnoreEmbedded;
extern int Verbose;
// SaveEmbedded is typically a pointer used by the code; we don't use it in tests.

// Utility: Create a minimal colorimetric TIFF on disk with the required tags.
// Returns true on success, false on failure.
static bool createColorimetricTIFF(const char* filename)
{
    // Open a new TIFF file for writing.
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        std::cerr << "Failed to create TIFF file: " << filename << "\n";
        return false;
    }

    // Primary chromaticities (5 numbers + 1 for Y of Red/Green/Blue; Y components set to 1.0)
    // Red.x, Red.y, Green.x, Green.y, Blue.x, Blue.y
    double primaries[6] = { 0.64, 0.33, 0.30, 0.60, 0.15, 0.06 };
    // White point
    double whitePoint[2] = { 0.3127, 0.3290 };

    // 256-entry 16-bit transfer functions for R, G, B (gamma approx 2.2)
    unsigned short gR[256];
    unsigned short gG[256];
    unsigned short gB[256];
    const double gamma = 2.2;
    for (int i = 0; i < 256; ++i) {
        double t = static_cast<double>(i) / 255.0;
        unsigned short v = static_cast<unsigned short>(std::floor(std::pow(t, gamma) * 65535.0 + 0.5));
        if (v > 65535) v = 65535;
        gR[i] = v;
        gG[i] = v;
        gB[i] = v;
    }

    // Write required tags to the TIFF to trigger the colorimetric path.
    // Primary chromaticities
    if (!TIFFSetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, 6, primaries)) {
        std::cerr << "Failed to set TIFFTAG_PRIMARYCHROMATICITIES\n";
        TIFFClose(tif);
        return false;
    }
    // White point
    if (!TIFFSetField(tif, TIFFTAG_WHITEPOINT, 2, whitePoint)) {
        std::cerr << "Failed to set TIFFTAG_WHITEPOINT\n";
        TIFFClose(tif);
        return false;
    }
    // Transfer function (R, G, B)
    if (!TIFFSetField(tif, TIFFTAG_TRANSFERFUNCTION, 3, gR, gG, gB)) {
        std::cerr << "Failed to set TIFFTAG_TRANSFERFUNCTION\n";
        TIFFClose(tif);
        return false;
    }

    // Close the file to flush tags.
    TIFFClose(tif);
    return true;
}

// Utility: Create an empty TIFF (no ICC profile and no colorimetric tags).
static bool createEmptyTIFF(const char* filename)
{
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        std::cerr << "Failed to create empty TIFF: " << filename << "\n";
        return false;
    }
    // Do not set any special tags; just close.
    TIFFClose(tif);
    return true;
}

// Helper: Delete a file if it exists.
static void removeIfExists(const char* filename)
{
    std::remove(filename);
}

// Test 1: No relevant tags present -> GetTIFFProfile should return NULL.
static bool test_no_tags_returns_null()
{
    const char* fname = "test_no_tags.tif";
    removeIfExists(fname);
    if (!createEmptyTIFF(fname)) {
        std::cerr << "Test 1: Failed to create empty TIFF.\n";
        return false;
    }

    TIFF* tif = TIFFOpen(fname, "r");
    if (!tif) {
        std::cerr << "Test 1: Failed to reopen TIFF for reading.\n";
        removeIfExists(fname);
        return false;
    }

    cmsHPROFILE prof = GetTIFFProfile(tif);
    TIFFClose(tif);

    removeIfExists(fname);

    if (prof != NULL) {
        std::cerr << "Test 1: Expected NULL profile when no tags present, got non-NULL.\n";
        return false;
    }

    // Test passed
    return true;
}

// Test 2: Colorimetric path with proper tags -> should return a valid cmsHPROFILE.
static bool test_colorimetric_profile_creation()
{
    const char* fname = "test_colorimetric.tif";
    removeIfExists(fname);
    if (!createColorimetricTIFF(fname)) {
        std::cerr << "Test 2: Failed to create colorimetric TIFF.\n";
        removeIfExists(fname);
        return false;
    }

    TIFF* tif = TIFFOpen(fname, "r");
    if (!tif) {
        std::cerr << "Test 2: Failed to reopen colorimetric TIFF for reading.\n";
        removeIfExists(fname);
        return false;
    }

    cmsHPROFILE prof = GetTIFFProfile(tif);
    TIFFClose(tif);

    removeIfExists(fname);

    if (prof == NULL) {
        std::cerr << "Test 2: Expected a non-NULL profile from colorimetric TIFF, got NULL.\n";
        return false;
    }

    // If we reach here, we successfully obtained a profile.
    // We should release the profile to avoid leaks (lcms2 provides cmsCloseProfile).
    // However, we should be careful about including the exact API here.
    // We'll attempt to close if function is available; otherwise skip.
#ifdef CMS_DLL_AVAILABLE
    cmsCloseProfile(prof);
#else
    // Best-effort: ignore if the symbol isn't available in this environment.
#endif

    return true;
}

// Test 3: When global flag IgnoreEmbedded is set, GetTIFFProfile should return NULL
// even if embedded data could be present. We simulate a colorimetric TIFF but expect NULL.
static bool test_ignore_embedded_flag_takes_precedence()
{
    // Externally defined in the focal file; declare to modify.
    extern int IgnoreEmbedded;
    extern int Verbose; // not used here, but tools expect it exist

    const char* fname = "test_ignore_embedded.tif";
    removeIfExists(fname);
    if (!createColorimetricTIFF(fname)) {
        std::cerr << "Test 3: Failed to create colorimetric TIFF for ignore-embedded test.\n";
        removeIfExists(fname);
        return false;
    }

    // Enable ignore embedded
    IgnoreEmbedded = 1;

    TIFF* tif = TIFFOpen(fname, "r");
    if (!tif) {
        std::cerr << "Test 3: Failed to reopen TIFF for reading.\n";
        removeIfExists(fname);
        return false;
    }

    cmsHPROFILE prof = GetTIFFProfile(tif);
    TIFFClose(tif);

    // Reset flag for subsequent tests
    IgnoreEmbedded = 0;

    removeIfExists(fname);

    if (prof != NULL) {
        std::cerr << "Test 3: Expected NULL profile when IgnoreEmbedded is set, got non-NULL.\n";
        // Attempt to close if a profile was returned
#ifdef CMS_DLL_AVAILABLE
        cmsCloseProfile(prof);
#endif
        return false;
    }

    return true;
}

// Main: run all tests and report results.
int main()
{
    std::cout << "Starting tests for GetTIFFProfile...\n";

    bool all_ok = true;

    // Test 1: No tags present
    bool t1 = test_no_tags_returns_null();
    std::cout << "Test 1 (no relevant tags) " << (t1 ? "PASSED" : "FAILED") << "\n";
    all_ok &= t1;

    // Test 2: Colorimetric path
    bool t2 = test_colorimetric_profile_creation();
    std::cout << "Test 2 (colorimetric path) " << (t2 ? "PASSED" : "FAILED") << "\n";
    all_ok &= t2;

    // Test 3: IgnoreEmbedded flag
    bool t3 = test_ignore_embedded_flag_takes_precedence();
    std::cout << "Test 3 (IgnoreEmbedded flag) " << (t3 ? "PASSED" : "FAILED") << "\n";
    all_ok &= t3;

    if (all_ok) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED. See details above.\n";
        return 1;
    }
}
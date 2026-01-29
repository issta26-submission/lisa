#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a null/default context (acceptable for THR variants to use the library default)
    cmsContext ctx = nullptr;

    // Prepare a whitepoint
    cmsCIExyY wp;
    wp.x = 0.3127;
    wp.y = 0.3290;
    wp.Y = 1.0;

    // Prepare RGB primaries
    cmsCIExyYTRIPLE prim;
    prim.Red.x   = 0.64; prim.Red.y   = 0.33; prim.Red.Y   = 1.0;
    prim.Green.x = 0.30; prim.Green.y = 0.60; prim.Green.Y = 1.0;
    prim.Blue.x  = 0.15; prim.Blue.y  = 0.06; prim.Blue.Y  = 1.0;

    // No custom tone curves (use device defaults)
    cmsToneCurve * TransferFunction[3] = { nullptr, nullptr, nullptr };

    // Create an RGB profile using the THR entry point
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(ctx, &wp, &prim, TransferFunction);

    // Create a NULL profile (identity-like) using THR entry point
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);

    // Create a transform from the RGB profile to the NULL profile.
    // Use zero for formats/intents/flags for a minimal valid call in this test context.
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, rgbProfile, 0, nullProfile, 0, 0, 0);

    // Query the encoded CMM version (demonstrates another API use)
    int cmmVersion = cmsGetEncodedCMMversion();
    (void)cmmVersion; // silence unused warning in this test

    // Exercise cmsDictNextEntry with null and with a local entry to ensure syntactic correctness
    const cmsDICTentry * firstEntry = cmsDictNextEntry(nullptr);

    cmsDICTentry localEntry;
    localEntry.Next = nullptr;
    localEntry.DisplayName = nullptr;
    localEntry.DisplayValue = nullptr;
    localEntry.Name = nullptr;
    localEntry.Value = nullptr;

    const cmsDICTentry * nextEntry = cmsDictNextEntry(&localEntry);

    // Silence unused-variable warnings for this test function
    (void)firstEntry;
    (void)nextEntry;
    (void)transform;
    (void)rgbProfile;
    (void)nullProfile;

    // Indicate success per the specification
    return 66;
}
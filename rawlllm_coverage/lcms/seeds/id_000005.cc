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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Allocate an IT8 handle (context NULL)
    cmsHANDLE hIT8 = cmsIT8Alloc((cmsContext)NULL);
    if (hIT8 == NULL) {
        return 2;
    }

    // Set an uncooked property on the IT8 handle
    // This uses the required cmsIT8SetPropertyUncooked API.
    if (!cmsIT8SetPropertyUncooked(hIT8, "Author", "LCMS Test")) {
        // Not fatal for this test, continue but note failure by returning a non-success code.
        return 3;
    }

    // Enumerate data formats (sample names) from the IT8 handle
    // cmsIT8EnumDataFormat expects a char***; provide the address of a char**
    char ** sampleNames = nullptr;
    int numSamples = cmsIT8EnumDataFormat(hIT8, &sampleNames);
    if (numSamples < 0) {
        return 4;
    }
    // sampleNames may be populated by the library; we won't rely on its contents here.

    // Create a profile and set its PCS using cmsSetPCS (required)
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return 5;
    }
    // cmsSetPCS expects a cmsColorSpaceSignature; use 0 (neutral) casted to the expected type.
    cmsSetPCS(hProfile, (cmsColorSpaceSignature)0);

    // Build a simple tabulated tone curve and duplicate it.
    // Use cmsBuildTabulatedToneCurve16 to obtain a cmsToneCurve* to duplicate.
    const unsigned short buildVals[2] = { 0u, 65535u };
    cmsToneCurve * originalTC = cmsBuildTabulatedToneCurve16((cmsContext)NULL, 2u,
                                                            (const cmsUInt16Number *)buildVals);
    if (originalTC == NULL) {
        return 6;
    }

    // Duplicate the tone curve using the required cmsDupToneCurve API
    cmsToneCurve * dupTC = cmsDupToneCurve(originalTC);
    if (dupTC == NULL) {
        return 7;
    }

    // Demonstrate color conversion from LCh to Lab using cmsLCh2Lab (required)
    cmsCIELCh lch;
    lch.L = 50.0;
    lch.C = 25.0;
    lch.h = 200.0;

    cmsCIELab lab;
    cmsLCh2Lab(&lab, &lch);

    // Use the produced lab values in a trivial way to avoid unused warnings
    if (lab.L < 0.0 || lab.L > 200.0) {
        // unrealistic value, but not fatal for test
        return 8;
    }

    // Success: return the required success code
    (void)sampleNames; // avoid unused-variable warnings if not used further
    (void)dupTC;
    (void)originalTC;
    (void)hProfile;
    (void)hIT8;
    return 66;
}
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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a simple NULL profile to have a valid cmsHPROFILE to query
    cmsHPROFILE profile = cmsCreateNULLProfile();

    // Query the first tag signature (may return 0 for empty profiles)
    auto tag0 = cmsGetTagSignature(profile, (cmsUInt32Number)0);

    // Prepare a Lab color and a white point, then convert to XYZ
    cmsCIELab lab = { 50.0, 10.0, -20.0 };
    cmsCIEXYZ whitePoint = { 95.047, 100.0, 108.883 }; // typical D65-like values
    cmsCIEXYZ xyz;
    cmsLab2XYZ(&whitePoint, &xyz, &lab);

    // Build a gamma tone curve (using default context by passing NULL)
    cmsToneCurve * curve = cmsBuildGamma(NULL, 2.2);
    // Estimate the gamma from the generated tone curve
    cmsFloat64Number estimated = 0.0;
    if (curve) {
        estimated = cmsEstimateGamma(curve, 1e-6);
    }

    // Attempt to load an IT8 file (may return NULL if file not present)
    cmsHANDLE it8 = cmsIT8LoadFromFile(NULL, "example.it8");

    // Use variables to avoid unused warnings (no-op casts)
    (void) tag0;
    (void) xyz;
    (void) estimated;
    (void) it8;
    (void) curve;
    (void) profile;

    return 66;
}
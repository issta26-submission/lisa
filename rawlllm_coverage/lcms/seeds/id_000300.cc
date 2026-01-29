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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for thread-safe calls
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        return 1; // failed to create context
    }

    // Build a small tabulated tone curve (3 entries)
    cmsFloat32Number tableVals[3] = { 0.0f, 0.5f, 1.0f };
    cmsToneCurve * curve = cmsBuildTabulatedToneCurveFloat(ctx, 3u, tableVals);

    // Check whether the created tone curve is linear (uses required API)
    cmsBool linear = 0;
    if (curve) {
        linear = cmsIsToneCurveLinear(curve);
    }

    // Create a Lab4 profile using a D65-like white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE hLab = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    // Create a BCHSW abstract profile with some example parameters
    cmsHPROFILE hBch = cmsCreateBCHSWabstractProfileTHR(ctx,
                                                       (cmsUInt32Number)33,   // nLUTPoints
                                                       (cmsFloat64Number)0.0, // Bright
                                                       (cmsFloat64Number)1.0, // Contrast
                                                       (cmsFloat64Number)0.0, // Hue
                                                       (cmsFloat64Number)1.0, // Saturation
                                                       (cmsUInt32Number)5000, // TempSrc
                                                       (cmsUInt32Number)5000  // TempDest
                                                       );

    // Allocate an IT8 handle and query a (possibly non-existing) property double
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsFloat64Number propVal = 0.0;
    if (hIT8) {
        // Use required API to get a double property (may return 0.0 if not present)
        propVal = cmsIT8GetPropertyDbl(hIT8, "SampleProperty");
    }

    // Use values to avoid unused-variable warnings (no-op)
    if (linear) {
        (void)propVal;
    } else {
        (void)propVal;
    }

    // Clean up profiles if created
    if (hLab) cmsCloseProfile(hLab);
    if (hBch) cmsCloseProfile(hBch);

    // Note: some objects (tone curve, IT8 handle) do not have explicit frees listed
    // in the provided API subset; rely on process termination or other API paths.

    // Return 66 on success as requested
    return 66;
}
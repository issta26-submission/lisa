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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a dedicated context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) return 1;

    // Create a profile tied to the context (sRGB) and another profile (XYZ)
    cmsHPROFILE prof1 = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE prof2 = cmsCreateXYZProfile();
    if (prof1 == nullptr || prof2 == nullptr) return 2;

    // Retrieve the context associated with prof1 (sanity check)
    cmsContext gotCtx = cmsGetProfileContextID(prof1);
    if (gotCtx == nullptr) {
        // Not expected, but treat as failure
        return 3;
    }

    // Query a formatter size/flag for the profile's PCS
    cmsUInt32Number fmt = cmsFormatterForPCSOfProfile(prof1, 8 /*nBytes*/, 1 /*lIsFloat*/);
    (void)fmt; // keep value to show it was used; no further action required here

    // Compute a white point from a common color temperature (D65 ~6504 K)
    cmsCIExyY wp;
    cmsBool gotWP = cmsWhitePointFromTemp(&wp, 6504.0);
    if (!gotWP) return 4;

    // Prepare an array of profiles and create a multiprofile transform using the context
    cmsHPROFILE profiles[2];
    profiles[0] = prof1;
    profiles[1] = prof2;

    cmsHTRANSFORM xform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2,
                                                           0 /*InputFormat (placeholder)*/,
                                                           0 /*OutputFormat (placeholder)*/,
                                                           0 /*Intent*/,
                                                           0 /*dwFlags*/);
    if (xform == nullptr) return 5;

    // Clean up the created transform
    cmsDeleteTransform(xform);

    // Success
    return 66;
}
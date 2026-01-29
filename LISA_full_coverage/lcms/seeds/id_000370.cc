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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations and profile/curve creation
    cmsContext ctx = NULL;
    cmsHPROFILE profile_srgb = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profile_null = cmsCreateNULLProfile();
    cmsToneCurve * curves[3];
    curves[0] = cmsBuildGamma(ctx, 1.0);
    curves[1] = cmsBuildGamma(ctx, 1.0);
    curves[2] = cmsBuildGamma(ctx, 1.0);
    cmsHPROFILE profile_rgb = cmsCreateRGBProfileTHR(ctx, NULL, NULL, (cmsToneCurve *const *)curves);

    // step 2: Create a transform between the RGB profile and sRGB
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile_srgb);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile_rgb, (cmsUInt32Number)0, profile_srgb, (cmsUInt32Number)0, intent, (cmsUInt32Number)0);

    // step 3: Use some profile/query APIs to mimic earlier logic
    cmsFloat64Number ver = cmsGetProfileVersion(profile_rgb);
    cmsBool md5_ok = cmsMD5computeID(profile_rgb);
    (void) ver;
    (void) md5_ok;

    // step 4: Free tone curves and use a transform-related query (no deletion API available in supplied list)
    cmsFreeToneCurveTriple(curves);
    cmsContext tx_ctx = cmsGetTransformContextID(transform);
    (void) tx_ctx;

    // step 5: Cleanup profiles
    cmsCloseProfile(profile_rgb);
    cmsCloseProfile(profile_srgb);
    cmsCloseProfile(profile_null);

    return 66;
}
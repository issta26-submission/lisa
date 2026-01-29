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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE srcProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * tone = NULL;
    cmsFloat64Number devlinkVersion = (cmsFloat64Number)4.3;
    cmsUInt32Number zeroFlags = 0U;

    srcProfile = cmsCreateXYZProfileTHR(ctx);
    tone = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    xform = cmsCreateTransformTHR(ctx, srcProfile, 0U, srcProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, devlinkVersion, zeroFlags);

    cmsFreeToneCurve(tone);
    cmsDeleteTransform(xform);
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(devlink);

    (void)ctx;
    (void)srcProfile;
    (void)devlink;
    (void)xform;
    (void)tone;
    (void)devlinkVersion;
    (void)zeroFlags;

    return 66;
}
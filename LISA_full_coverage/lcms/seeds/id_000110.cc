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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHPROFILE outProfile = NULL;
    cmsStage * stage = NULL;
    cmsToneCurve * tone = NULL;
    cmsHTRANSFORM xform = (cmsHTRANSFORM)NULL;
    cmsUInt32Number creator = 0U;
    cmsFloat64Number version = 4.4;
    cmsUInt32Number flags = 0U;
    cmsUInt32Number dwFlags = 0U;

    // step 2: Setup (Create profile, tone curve and stage)
    tone = cmsBuildGamma(ctx, 2.2);
    hProfile = cmsCreate_sRGBProfile();
    outProfile = cmsCreateNULLProfileTHR(ctx);
    stage = cmsStageAllocIdentity(ctx, 3U);

    // step 3: Core operations (create transform, create device link from transform, sample stage CLUT, read profile header creator)
    xform = cmsCreateTransform(hProfile, 0U, outProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, flags);
    cmsStageSampleCLut16bit(stage, (cmsSAMPLER16)NULL, NULL, dwFlags);
    creator = cmsGetHeaderCreator(hProfile);

    // step 4: Cleanup (release created resources and silence unused warnings)
    cmsDeleteTransform(xform);
    cmsCloseProfile(hProfile);
    cmsCloseProfile(outProfile);
    cmsStageFree(stage);
    (void)tone;
    (void)devlink;
    (void)creator;
    (void)version;
    (void)flags;
    (void)dwFlags;
    (void)ctx;

    return 66;
}
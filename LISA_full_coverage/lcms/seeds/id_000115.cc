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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsToneCurve * tone = NULL;
    cmsStage * stage = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsHPROFILE devlink = NULL;
    cmsUInt8Number profileID[16];
    cmsFloat64Number version = 4.3;
    cmsUInt16Number sampleCargo[4];
    cmsBool sampled = 0;

    // step 2: Setup (create context, profile and initialize buffers)
    ctx = cmsCreateContext(NULL, NULL);
    hProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsSetProfileVersion(hProfile, version);
    tone = NULL;
    memset(profileID, 0, sizeof(profileID));
    memset(sampleCargo, 0, sizeof(sampleCargo));
    stage = cmsStageAllocIdentity(ctx, 3U);

    // step 3: Core operations (sample stage CLUT, create a transform, then device link)
    sampled = cmsStageSampleCLut16bit(stage, (cmsSAMPLER16)NULL, sampleCargo, 0U);
    xform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    (void)cmsGetHeaderManufacturer(hProfile);

    // step 4: Cleanup and validation (free tone and ensure variables referenced)
    cmsFreeToneCurve(tone);
    (void)ctx;
    (void)hProfile;
    (void)stage;
    (void)xform;
    (void)devlink;
    (void)profileID;
    (void)version;
    (void)sampleCargo;
    (void)sampled;

    return 66;
}
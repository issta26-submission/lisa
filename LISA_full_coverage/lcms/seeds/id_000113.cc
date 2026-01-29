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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srcProfile = NULL;
    cmsHPROFILE dstProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * gamma = NULL;
    cmsStage * stage = NULL;
    cmsSAMPLER16 sampler = NULL;
    cmsUInt8Number profileID[16];
    cmsFloat64Number devlinkVersion = 4.3;
    cmsUInt32Number encodedVersion = (cmsUInt32Number)0x04300000;
    void * cargo = NULL;

    // step 2: Setup (create profiles and tone curve, initialize buffers)
    srcProfile = cmsCreateXYZProfileTHR(ctx);
    dstProfile = cmsCreateNULLProfileTHR(ctx);
    gamma = cmsBuildGamma(ctx, 2.2);
    cmsSetEncodedICCversion(dstProfile, encodedVersion);
    memset(profileID, 0, sizeof(profileID));

    // step 3: Core operations (create transform, device link, inspect header id, sample a stage)
    xform = cmsCreateTransform(srcProfile, 0U, dstProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, devlinkVersion, 0U);
    cmsGetHeaderProfileID(devlink, profileID);

    stage = cmsStageAllocIdentity(ctx, 3U);
    sampler = [](const cmsUInt16Number * in, cmsUInt16Number * out, void * /*cargo*/) -> int {
        out[0] = in[0];
        out[1] = in[1];
        out[2] = in[2];
        return 1;
    };
    cmsStageSampleCLut16bit(stage, sampler, cargo, 0U);

    // step 4: Cleanup (destroy objects and ensure values are referenced)
    cmsFreeToneCurve(gamma);
    cmsDeleteTransform(xform);
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(dstProfile);
    cmsCloseProfile(devlink);
    (void)ctx;
    (void)stage;
    (void)sampler;
    (void)profileID;
    (void)devlinkVersion;
    (void)encodedVersion;
    (void)cargo;

    return 66;
    // API sequence test completed successfully
}
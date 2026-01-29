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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsStage * stage = NULL;
    cmsToneCurve * tone = NULL;
    cmsUInt8Number profileID[16];
    cmsFloat64Number version = 4.3;
    void * cargo = NULL;

    // step 2: Setup (create profile, build gamma, set encoded version, clear profile ID buffer)
    hProfile = cmsCreateXYZProfile();
    tone = cmsBuildGamma(ctx, 2.200000);
    cmsSetEncodedICCversion(hProfile, (cmsUInt32Number)version);
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(hProfile, profileID);

    // step 3: Core operations (create transform, produce device link, sample stage CLUT)
    xform = cmsCreateTransform(hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    stage = cmsStageAllocIdentity(ctx, 3U);
    cmsStageSampleCLut16bit(stage, (cmsSAMPLER16)NULL, cargo, 0U);

    // step 4: Cleanup (delete transform, free tone curve, close profiles and silence unused vars)
    cmsDeleteTransform(xform);
    cmsFreeToneCurve(tone);
    cmsCloseProfile(hProfile);
    cmsCloseProfile(devlink);
    (void)ctx;
    (void)stage;
    (void)profileID;
    (void)version;
    (void)cargo;

    return 66;
    // API sequence test completed successfully
}
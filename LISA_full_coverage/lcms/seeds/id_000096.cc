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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE sRGB = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * tone = NULL;
    cmsFloat64Number params[1];
    cmsFloat64Number version = 4.5;
    cmsUInt8Number profileID[16];
    cmsBool isMonotonic = 0;
    cmsContext ctx_from_pipeline = NULL;
    cmsPipeline * lut = NULL;
    cmsUInt32Number flags = 0;

    // Step 2: Setup
    ctx = cmsCreateContext(NULL, NULL);
    sRGB = cmsCreate_sRGBProfileTHR(ctx);
    cmsSetProfileVersion(sRGB, 4.0);
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(sRGB, profileID);

    params[0] = 2.2;
    tone = cmsBuildGamma(ctx, params[0]);
    isMonotonic = cmsIsToneCurveMonotonic(tone);

    // Step 3: Core operations
    xform = cmsCreateTransformTHR(ctx, sRGB, 0U, sRGB, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, flags);
    cmsSetProfileVersion(devlink, 4.5);
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(devlink, profileID);
    lut = (cmsPipeline *) devlink;
    ctx_from_pipeline = cmsGetPipelineContextID((const cmsPipeline *) lut);

    // Step 4: Cleanup
    cmsDeleteTransform(xform);
    xform = NULL;
    tone = NULL;
    sRGB = NULL;
    devlink = NULL;
    lut = NULL;
    (void)ctx;
    (void)params;
    (void)isMonotonic;
    (void)ctx_from_pipeline;
    (void)profileID;
    (void)flags;
    (void)version;

    return 66;
    // API sequence test completed successfully
}
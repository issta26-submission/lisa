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
//<ID> 92
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
    cmsHTRANSFORM xform = NULL;
    cmsHPROFILE devlink = NULL;
    cmsUInt8Number profileID[16];
    cmsBool isMonotonic = 0;
    const cmsPipeline * pipeline = NULL;
    cmsContext pipeCtx = NULL;
    cmsFloat64Number version = 4.300000;
    cmsUInt32Number flags = 0;
    cmsUInt16Number table[4];

    // step 2: Setup
    memset(profileID, 0, sizeof(profileID));
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)16384;
    table[2] = (cmsUInt16Number)49152;
    table[3] = (cmsUInt16Number)65535;
    hProfile = cmsCreate_sRGBProfileTHR(ctx);
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, table);
    xform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);

    // step 3: Operate and Validate
    devlink = cmsTransform2DeviceLink(xform, version, flags);
    cmsSetProfileVersion(devlink, version);
    cmsGetHeaderProfileID(devlink, profileID);
    isMonotonic = cmsIsToneCurveMonotonic(tone);
    pipeline = cmsPipelineAlloc(ctx, 3U, 3U);
    pipeCtx = cmsGetPipelineContextID(pipeline);

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    cmsPipelineFree((cmsPipeline *) pipeline);
    (void)hProfile;
    (void)devlink;
    (void)tone;
    (void)profileID;
    (void)isMonotonic;
    (void)pipeCtx;
    (void)table;
    (void)version;
    (void)flags;
    return 66;
    // API sequence test completed successfully
}
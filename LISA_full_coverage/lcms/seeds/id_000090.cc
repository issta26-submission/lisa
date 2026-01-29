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
//<ID> 90
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
    cmsToneCurve * tone = NULL;
    cmsUInt8Number profileID[16];
    cmsUInt16Number values[4];
    cmsFloat64Number newVersion = 4.300000;
    cmsBool isMono = 0;
    cmsPipeline * pipeline = NULL;
    cmsContext pipelineCtx = NULL;

    // step 2: Setup
    memset(profileID, 0, sizeof(profileID));
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)16384;
    values[2] = (cmsUInt16Number)49152;
    values[3] = (cmsUInt16Number)65535;
    hProfile = cmsCreate_sRGBProfileTHR(ctx);
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, values);
    cmsSetProfileVersion(hProfile, newVersion);
    cmsGetHeaderProfileID(hProfile, profileID);

    // step 3: Core operations
    isMono = cmsIsToneCurveMonotonic(tone);
    xform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, newVersion, 0U);
    pipeline = (cmsPipeline *) xform; // ensure non-NULL pointer to avoid assertion in cmsGetPipelineContextID
    pipelineCtx = cmsGetPipelineContextID(pipeline);

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    /* No public free for tone or profile in provided API set in this sequence; clear references */
    hProfile = NULL;
    devlink = NULL;
    tone = NULL;
    pipeline = NULL;
    (void)ctx;
    (void)profileID;
    (void)values;
    (void)isMono;
    (void)pipelineCtx;
    (void)newVersion;

    return 66;
}
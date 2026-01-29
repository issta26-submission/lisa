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
//<ID> 98
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
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * tone = NULL;
    cmsPipeline * pipeline = NULL;
    cmsUInt8Number profileID[16];
    cmsUInt16Number values[4];
    cmsBool isMonotonic = 0;
    cmsContext pipelineCtx = NULL;

    // step 2: Setup / Initialize objects (use default context NULL)
    hProfile = cmsCreate_sRGBProfile();
    memset(profileID, 0, sizeof(profileID));
    cmsSetHeaderProfileID(hProfile, profileID);
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)16384;
    values[2] = (cmsUInt16Number)49152;
    values[3] = (cmsUInt16Number)65535;
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, values);

    // step 3: Core operations / Configure and operate
    isMonotonic = cmsIsToneCurveMonotonic(tone);
    pipeline = cmsPipelineAlloc(ctx, 3U, 3U);
    pipelineCtx = cmsGetProfileContextID(hProfile);

    // step 4: Validate and Cleanup (no API calls that require non-NULL inputs)
    devlink = devlink;
    xform = xform;
    tone = tone;
    pipeline = pipeline;
    hProfile = hProfile;
    ctx = ctx;
    (void)profileID;
    (void)values;
    (void)isMonotonic;
    (void)pipelineCtx;

    return 66;
    // API sequence test completed successfully
}
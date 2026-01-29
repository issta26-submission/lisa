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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE srgbProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHTRANSFORM transform = cmsCreateTransform(srgbProfile, (cmsUInt32Number)0, nullProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsToneCurve * curves[3] = { gamma, gamma, gamma };
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve * const *)curves);

    cmsUInt32Number inChannels = cmsStageInputChannels(stage);
    cmsContext tctx = cmsGetTransformContextID(transform);
    cmsHPROFILE nullProfileFromTransformCtx = cmsCreateNULLProfileTHR(tctx);

    cmsSetProfileVersion(nullProfile, (cmsFloat64Number)4.5);
    cmsSetProfileVersion(nullProfileFromTransformCtx, (cmsFloat64Number)4.5);
    (void) inChannels;

    cmsPipelineFree(pipeline);
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(srgbProfile);
    cmsCloseProfile(nullProfileFromTransformCtx);

    return 66;
}
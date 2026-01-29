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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsStage * st = cmsStageAllocIdentity(ctx, (cmsUInt32Number)3);
    void * stageData = cmsStageData(st);
    cmsUInt32Number outChannels = cmsStageOutputChannels(st);
    cmsHPROFILE srgb = cmsCreate_sRGBProfile();
    cmsSetProfileVersion(srgb, (cmsFloat64Number)4.3);
    cmsUInt32Number creator = cmsGetHeaderCreator(srgb);
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(srgb);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsFloat64Number estimated = cmsEstimateGamma(gamma, (cmsFloat64Number)0.001);
    cmsBool isLinear = cmsIsToneCurveLinear(gamma);
    cmsUInt32Number estEntries = cmsGetToneCurveEstimatedTableEntries(gamma);
    cmsFloat32Number src[3];
    cmsFloat32Number dst[3];
    src[0] = 0.1f; src[1] = 0.2f; src[2] = 0.3f;
    dst[0] = dst[1] = dst[2] = 0.0f;
    cmsPipelineEvalFloat(src, dst, lut);
    cmsContext ctxFromLut = cmsGetPipelineContextID(lut);
    cmsStageFree(st);
    cmsFreeToneCurve(gamma);
    (void)ctx;
    (void)stageData;
    (void)outChannels;
    (void)creator;
    (void)intent;
    (void)estimated;
    (void)isLinear;
    (void)estEntries;
    (void)dst;
    (void)ctxFromLut;
    (void)srgb;
    (void)lut;
    return 66;
}
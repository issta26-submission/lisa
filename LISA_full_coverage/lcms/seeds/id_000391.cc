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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = NULL;
    cmsContext dupCtx = cmsDupContext(ctx, NULL);
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * tc0 = cmsBuildGamma(dupCtx, 1.0);
    cmsToneCurve * tc1 = cmsBuildGamma(dupCtx, 1.0);
    cmsToneCurve * tc2 = cmsBuildGamma(dupCtx, 1.0);
    cmsToneCurve * toneArray[3];
    toneArray[0] = tc0;
    toneArray[1] = tc1;
    toneArray[2] = tc2;

    // step 2: Configure
    cmsStage * stage = cmsStageAllocToneCurves(dupCtx, (cmsUInt32Number)3, (cmsToneCurve *const *)toneArray);
    cmsPipeline * pipeline = cmsPipelineAlloc(dupCtx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsPipeline * pipelineDup = cmsPipelineDup(pipeline);
    cmsPipelineSetSaveAs8bitsFlag(pipelineDup, (cmsBool)1);

    // step 3: Operate (evaluate) & Validate
    cmsFloat32Number in[3];
    cmsFloat32Number out[3];
    in[0] = 0.1f;
    in[1] = 0.2f;
    in[2] = 0.3f;
    out[0] = out[1] = out[2] = 0.0f;
    cmsPipelineEvalFloat(in, out, pipelineDup);
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(labProfile);
    unsigned validation = (unsigned)((int)intent ^ (int)(out[0] * 100.0f) ^ (int)(pipeline != pipelineDup));
    (void)validation;

    // step 4: Cleanup
    cmsPipelineFree(pipeline);
    cmsPipelineFree(pipelineDup);
    cmsStageFree(stage);
    cmsFreeToneCurve(tc0);
    cmsFreeToneCurve(tc1);
    cmsFreeToneCurve(tc2);
    cmsCloseProfile(labProfile);

    // API sequence test completed successfully
    return 66;
}
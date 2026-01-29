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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext baseCtx = NULL;
    cmsContext ctx = cmsDupContext(baseCtx, NULL);
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3333333333333333;
    whitePoint.y = 0.3333333333333333;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * curveR = cmsBuildGamma(ctx, 1.8);
    cmsToneCurve * curveG = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve * curveB = cmsBuildGamma(ctx, 1.0);
    cmsToneCurve * curves[3];
    curves[0] = curveR;
    curves[1] = curveG;
    curves[2] = curveB;

    // step 2: Setup
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, curves);
    cmsUInt32Number stageOut = cmsStageOutputChannels(stage);
    cmsPipeline * pipe = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, stageOut);
    cmsPipelineSetSaveAs8bitsFlag(pipe, 0);

    // step 3: Core operations
    cmsFloat32Number in[3];
    in[0] = 0.25f;
    in[1] = 0.5f;
    in[2] = 0.75f;
    cmsFloat32Number out[4];
    memset(out, 0, sizeof(out));
    cmsPipelineEvalFloat(in, out, pipe);
    cmsContext pipeCtx = cmsGetPipelineContextID(pipe);
    cmsContext dupFromPipe = cmsDupContext(pipeCtx, NULL);
    (void)dupFromPipe;

    // step 4: Cleanup
    cmsPipelineFree(pipe);
    cmsPipelineFree(cmsPipelineDup(pipe));
    cmsCloseProfile(labProfile);

    // API sequence test completed successfully
    return 66;
}
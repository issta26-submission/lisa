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
//<ID> 392
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
    memset(&whitePoint, 0, sizeof(whitePoint));
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * gammaCurve = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve * curves[3];
    curves[0] = gammaCurve;
    curves[1] = gammaCurve;
    curves[2] = gammaCurve;

    // step 2: Setup
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve *const*)curves);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 3: Core operations
    cmsFloat32Number in[3];
    in[0] = 0.100000f;
    in[1] = 0.200000f;
    in[2] = 0.300000f;
    cmsFloat32Number out[3];
    memset(out, 0, sizeof(out));
    cmsPipelineEvalFloat(in, out, pipeline);
    cmsUInt32Number stageOutCh = cmsStageOutputChannels(stage);
    cmsUInt32Number model = cmsGetHeaderModel(labProfile);
    cmsUInt32Number validation = stageOutCh ^ model ^ (cmsUInt32Number)((int)(out[0] * 1000.0f));
    (void)validation;

    // step 4: Cleanup
    cmsPipelineFree(pipeline);
    cmsStageFree(stage);
    cmsFreeToneCurve(gammaCurve);
    cmsCloseProfile(labProfile);

    // API sequence test completed successfully
    return 66;
}
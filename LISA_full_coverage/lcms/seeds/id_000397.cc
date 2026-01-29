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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsDupContext(NULL, NULL);
    cmsContext ctx_dup = cmsDupContext(ctx, NULL);
    cmsCIExyY whitePoint;
    whitePoint.x = 0.34567;
    whitePoint.y = 0.35850;
    whitePoint.Y = 1.0;
    cmsHPROFILE lab2 = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * curves[3];
    curves[0] = cmsBuildGamma(ctx, 1.0);
    curves[1] = cmsBuildGamma(ctx, 1.0);
    curves[2] = cmsBuildGamma(ctx, 1.0);
    cmsStage * toneStage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve *const *)curves);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 2: Setup
    cmsPipeline * pipeline_dup = cmsPipelineDup(pipeline);
    cmsPipelineSetSaveAs8bitsFlag(pipeline_dup, 1);
    cmsFloat32Number input[3];
    input[0] = 0.12f;
    input[1] = 0.34f;
    input[2] = 0.56f;
    cmsFloat32Number output[3];
    memset(output, 0, sizeof(output));

    // step 3: Core operations & Validate
    cmsPipelineEvalFloat(input, output, pipeline_dup);
    cmsFloat32Number validation = output[0] + output[1] + output[2] + (cmsFloat32Number)whitePoint.Y;
    (void)validation;

    // step 4: Cleanup
    cmsPipelineFree(pipeline);
    cmsPipelineFree(pipeline_dup);
    cmsStage * next_stage = toneStage;
    (void)next_stage;
    cmsFreeToneCurve(curves[0]);
    cmsFreeToneCurve(curves[1]);
    cmsFreeToneCurve(curves[2]);
    cmsCloseProfile(lab2);

    // API sequence test completed successfully
    return 66;
}
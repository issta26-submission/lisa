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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    cmsPipeline * pipeline = NULL;
    cmsStage * stage = NULL;
    cmsToneCurve * gamma1 = NULL;
    cmsToneCurve * gamma2 = NULL;
    cmsToneCurve * gamma3 = NULL;
    cmsUInt32Number intentCodes[1];
    char * intentDesc[1];
    memset(intentCodes, 0, sizeof(intentCodes));
    memset(intentDesc, 0, sizeof(intentDesc));

    // step 2: Setup (Create profile, query intents, build tone curves and allocate pipeline/stage)
    profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)33, (cmsFloat64Number)1.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsUInt32Number returnedIntents = cmsGetSupportedIntentsTHR(ctx, (cmsUInt32Number)1, intentCodes, intentDesc);
    gamma1 = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    gamma2 = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    gamma3 = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    {
        cmsToneCurve * curves[3];
        curves[0] = gamma1;
        curves[1] = gamma2;
        curves[2] = gamma3;
        stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve * const *)curves);
    }
    pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 3: Operate and Validate (perform a pipeline evaluation to exercise objects)
    {
        cmsUInt16Number in16[3];
        cmsUInt16Number out16[3];
        in16[0] = (cmsUInt16Number)0;
        in16[1] = (cmsUInt16Number)32768;
        in16[2] = (cmsUInt16Number)65535;
        memset(out16, 0, sizeof(out16));
        cmsPipelineEval16(in16, out16, pipeline);
        (void)out16[0];
        (void)returnedIntents;
        (void)intentCodes[0];
        (void)intentDesc[0];
    }

    // step 4: Cleanup (free stage and pipeline, leave profile/context as opaque)
    if (stage) cmsStageFree(stage);
    if (pipeline) cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}
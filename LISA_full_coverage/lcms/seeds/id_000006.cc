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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsHPROFILE profile = (cmsHPROFILE) NULL;
    cmsUInt32Number intents[1];
    char * descriptions[1];
    memset(intents, 0, sizeof(intents));
    memset(descriptions, 0, sizeof(descriptions));
    cmsPipeline * pipeline = (cmsPipeline *) NULL;
    cmsStage * stage = (cmsStage *) NULL;
    cmsToneCurve * gamma = (cmsToneCurve *) NULL;

    // step 2: Setup - create an abstract BCHSW profile
    profile = cmsCreateBCHSWabstractProfileTHR(ctx, 17u, 0.0, 1.0, 0.0, 1.0, 6500u, 5000u);

    // step 3: Configure & Operate - query supported intents and build a pipeline + stage
    cmsUInt32Number found = cmsGetSupportedIntentsTHR(ctx, 1u, intents, descriptions);
    pipeline = cmsPipelineAlloc(ctx, 3u, 3u);
    gamma = cmsBuildGamma(ctx, 2.2);
    {
        cmsToneCurve * curves[3];
        curves[0] = gamma;
        curves[1] = gamma;
        curves[2] = gamma;
        stage = cmsStageAllocToneCurves(ctx, 3u, curves);
    }

    // step 4: Validate & Cleanup - free allocated stage and pipeline
    cmsStageFree(stage);
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    (void) found;
    (void) profile;
    return 66;
}
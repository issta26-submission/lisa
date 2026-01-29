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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = nullptr;
    cmsUInt32Number intentCodes[4];
    char *intentDescriptions[4];
    memset(intentCodes, 0, sizeof(intentCodes));
    memset(intentDescriptions, 0, sizeof(intentDescriptions));
    cmsHPROFILE profile = nullptr;
    cmsPipeline *pipe = nullptr;
    cmsStage *stage = nullptr;

    // step 2: Setup - create an abstract BCHSW profile and query supported intents
    profile = cmsCreateBCHSWabstractProfileTHR(ctx,
                                               (cmsUInt32Number)17,
                                               (cmsFloat64Number)1.0,
                                               (cmsFloat64Number)0.5,
                                               (cmsFloat64Number)0.0,
                                               (cmsFloat64Number)0.0,
                                               (cmsUInt32Number)6500,
                                               (cmsUInt32Number)6500);
    cmsUInt32Number foundIntents = cmsGetSupportedIntentsTHR(ctx, (cmsUInt32Number)4, intentCodes, intentDescriptions);

    // step 3: Operate - allocate a pipeline and a tone-curve stage (3-channel example)
    pipe = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve *const *)NULL);

    // step 4: Cleanup - free allocated stage and pipeline
    cmsStageFree(stage);
    cmsPipelineFree(pipe);

    // API sequence test completed successfully
    (void)foundIntents;
    (void)intentCodes;
    (void)intentDescriptions;
    (void)profile;
    return 66;
}
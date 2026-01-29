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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & initialization
    cmsContext ctx = nullptr;
    cmsUInt32Number intentsArray[4];
    char * descriptions[4];
    memset(intentsArray, 0, sizeof(intentsArray));
    memset(descriptions, 0, sizeof(descriptions));
    cmsUInt32Number foundIntents = cmsGetSupportedIntentsTHR(ctx, 4, intentsArray, descriptions);

    // step 2: Setup - create an abstract profile and a pipeline
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx,
                                                           intentsArray[0],
                                                           (cmsFloat64Number)1.0,
                                                           (cmsFloat64Number)1.0,
                                                           (cmsFloat64Number)0.0,
                                                           (cmsFloat64Number)1.0,
                                                           intentsArray[1],
                                                           intentsArray[2]);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 3: Core operations - build a tone curve, allocate a stage, insert and inspect pipeline
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsToneCurve * curves[3];
    curves[0] = gamma;
    curves[1] = gamma;
    curves[2] = gamma;
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, curves);
    cmsPipelineInsertStage(pipeline, (cmsStageLoc)0, stage);
    cmsStage * firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    cmsStage * lastStage = cmsPipelineGetPtrToLastStage(pipeline);
    (void) foundIntents;
    (void) descriptions;
    (void) firstStage;
    (void) lastStage;

    // step 4: Cleanup - unlink and free stage, free pipeline
    cmsPipelineUnlinkStage(pipeline, (cmsStageLoc)0, &stage);
    cmsStageFree(stage);
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}
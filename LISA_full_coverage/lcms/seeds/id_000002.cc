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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize & Configure
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number codes[4];
    char *descriptions[4];
    memset(codes, 0, sizeof(codes));
    memset(descriptions, 0, sizeof(descriptions));
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)17, (cmsFloat64Number)1.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 2: Operate
    cmsUInt32Number nReturned = cmsGetSupportedIntentsTHR(ctx, (cmsUInt32Number)4, codes, descriptions);
    cmsStage *stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, NULL);
    cmsStageFree(stage);

    // step 3: Validate
    cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    (void)firstStage;
    (void)nReturned;
    (void)profile;

    // step 4: Cleanup
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}
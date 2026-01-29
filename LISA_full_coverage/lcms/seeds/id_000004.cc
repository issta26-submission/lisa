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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsContext ctx = (cmsContext) NULL;
    cmsUInt32Number intentCodes[4];
    char * intentDescriptions[4];
    memset(intentCodes, 0, sizeof(intentCodes));
    memset(intentDescriptions, 0, sizeof(intentDescriptions));
    cmsUInt32Number supportedCount = cmsGetSupportedIntentsTHR(ctx, 4, intentCodes, intentDescriptions);

    // step 2: Configure
    cmsFloat64Number bright = 1.0;
    cmsFloat64Number contrast = 1.0;
    cmsFloat64Number hue = 0.0;
    cmsFloat64Number saturation = 1.0;
    cmsUInt32Number tempSrc = (supportedCount > 0) ? intentCodes[0] : 0;
    cmsUInt32Number tempDest = (supportedCount > 1) ? intentCodes[1] : 0;
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, 17, bright, contrast, hue, saturation, tempSrc, tempDest);

    // step 3: Operate
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, 3u, 3u);
    cmsToneCurve * gammaCurve = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve * curves3[3];
    curves3[0] = gammaCurve;
    curves3[1] = gammaCurve;
    curves3[2] = gammaCurve;
    cmsStage * stage = cmsStageAllocToneCurves(ctx, 3u, curves3);

    // step 4: Validate & Cleanup
    (void) cmsGetProfileVersion(profile); // validate we can query profile metadata
    (void) cmsEstimateGamma(gammaCurve, 0.001);

    cmsStageFree(stage);
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}
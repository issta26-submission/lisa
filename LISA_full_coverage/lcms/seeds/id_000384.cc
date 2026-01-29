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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsFloat32Number values[4];
    memset(values, 0, sizeof(values));
    values[0] = (cmsFloat32Number)0.0f;
    values[1] = (cmsFloat32Number)0.33333334f;
    values[2] = (cmsFloat32Number)0.66666669f;
    values[3] = (cmsFloat32Number)1.0f;
    cmsToneCurve * tc = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)4, values);

    // step 2: Setup
    cmsUInt32Number model = cmsGetHeaderModel(profile);
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature) model);
    cmsFloat64Number mat[9] = {
        (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)0.0,
        (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0,
        (cmsFloat64Number)0.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0
    };
    cmsStage * stage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)channels, (cmsUInt32Number)channels, mat, NULL);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)channels, (cmsUInt32Number)channels);
    cmsPipelineInsertStage(pipeline, (cmsStageLoc)0, stage);

    // step 3: Core operations & Validate
    cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsBool changed = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0x19000716u, (cmsUInt32Number)0x20000004u);
    cmsUInt32Number validation = (cmsUInt32Number)(channels ^ outChannels ^ (changed ? 1u : 0u) ^ model);
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurve(tc);
    cmsCloseProfile(profile);
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}
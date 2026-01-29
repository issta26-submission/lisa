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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE prof_in = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE prof_out = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number model_in = cmsGetHeaderModel(prof_in);
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature) model_in);
    cmsFloat32Number curve_vals[2];
    curve_vals[0] = 0.0f;
    curve_vals[1] = 1.0f;
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)2, curve_vals);

    // step 2: Setup
    cmsUInt32Number nchan = channels;
    cmsFloat64Number matrix[16];
    cmsFloat64Number offset[16];
    memset(matrix, 0, sizeof(matrix));
    memset(offset, 0, sizeof(offset));
    matrix[0]  = 1.0;
    matrix[5]  = 1.0;
    matrix[10] = 1.0;
    matrix[15] = 1.0;
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, nchan, nchan);
    cmsStage * stage = cmsStageAllocMatrix(ctx, nchan, nchan, matrix, offset);
    cmsPipelineInsertStage(pipeline, (cmsStageLoc)0, stage);

    // step 3: Core operations
    cmsUInt32Number stage_out_channels = cmsStageOutputChannels(stage);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, prof_in, (cmsUInt32Number)0, prof_out, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsBool changed = cmsChangeBuffersFormat(transform, (cmsUInt32Number)1, (cmsUInt32Number)2);
    cmsUInt32Number model_out = cmsGetHeaderModel(prof_out);
    (void)changed;
    (void)stage_out_channels;
    (void)model_out;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsPipelineFree(pipeline);
    cmsFreeToneCurve(tone);
    cmsCloseProfile(prof_in);
    cmsCloseProfile(prof_out);

    // API sequence test completed successfully
    return 66;
}
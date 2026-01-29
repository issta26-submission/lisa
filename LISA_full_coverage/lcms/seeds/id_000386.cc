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
//<ID> 386
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
    cmsUInt32Number model = cmsGetHeaderModel(profile);
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature)model);
    cmsFloat32Number table[4];
    table[0] = 0.0f;
    table[1] = 0.333333f;
    table[2] = 0.666667f;
    table[3] = 1.0f;
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)4, table);
    cmsFloat64Number matrix[16];
    matrix[0]  = 1.0; matrix[1]  = 0.0; matrix[2]  = 0.0; matrix[3]  = 0.0;
    matrix[4]  = 0.0; matrix[5]  = 1.0; matrix[6]  = 0.0; matrix[7]  = 0.0;
    matrix[8]  = 0.0; matrix[9]  = 0.0; matrix[10] = 1.0; matrix[11] = 0.0;
    matrix[12] = 0.0; matrix[13] = 0.0; matrix[14] = 0.0; matrix[15] = 1.0;
    cmsFloat64Number offset[4];
    offset[0] = 0.0; offset[1] = 0.0; offset[2] = 0.0; offset[3] = 0.0;
    cmsStage * stage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)channels, (cmsUInt32Number)channels, matrix, offset);

    // step 2: Setup
    cmsUInt32Number stageOut = cmsStageOutputChannels(stage);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate & Validate
    cmsUInt32Number outFormat = cmsGetTransformOutputFormat(transform);
    cmsBool change_ok = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0, outFormat);
    cmsUInt32Number validation = (cmsUInt32Number)((stageOut ^ channels) ^ (outFormat ^ (change_ok ? 0x1u : 0u)));
    (void)validation;

    // step 4: Cleanup
    cmsPipeline * pipe = cmsPipelineAlloc(ctx, (cmsUInt32Number)channels, (cmsUInt32Number)channels);
    cmsPipelineInsertStage(pipe, (cmsStageLoc)0, stage);
    cmsPipelineFree(pipe);
    cmsDeleteTransform(transform);
    cmsFreeToneCurve(tone);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}
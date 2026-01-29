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
//<ID> 385
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
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature) model);
    cmsFloat32Number table_vals[4];
    memset(table_vals, 0, sizeof(table_vals));
    table_vals[0] = 0.0f;
    table_vals[1] = 0.33333333f;
    table_vals[2] = 0.66666667f;
    table_vals[3] = 1.0f;
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)4, table_vals);

    // step 2: Setup
    cmsFloat64Number matrix[9];
    memset(matrix, 0, sizeof(matrix));
    matrix[0] = 1.0; matrix[4] = 1.0; matrix[8] = 1.0;
    cmsFloat64Number offset[3];
    memset(offset, 0, sizeof(offset));
    cmsStage * stage = cmsStageAllocMatrix(ctx, channels, channels, matrix, offset);

    // step 3: Operate & Validate
    cmsUInt32Number stage_out = cmsStageOutputChannels(stage);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsBool changed = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0x21000000, (cmsUInt32Number)0x22000000);
    cmsUInt32Number validation = (cmsUInt32Number)((cmsUInt32Number)changed ^ stage_out ^ channels ^ (tone ? 0x55u : 0u));
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurve(tone);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile_in = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profile_out = cmsCreateNULLProfileTHR(ctx);
    cmsFloat32Number gamma_vals[3] = { 0.0f, 0.5f, 1.0f };
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)3, gamma_vals);
    const cmsFloat64Number matrix[9] = { 1.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0,
                                         0.0, 0.0, 1.0 };
    const cmsFloat64Number offset[3] = { 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3, matrix, offset);

    // step 2: Setup
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, intent, (cmsUInt32Number)0);

    // step 3: Core operations
    cmsBool changed = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0x00010000u, (cmsUInt32Number)0x00020000u);
    cmsUInt32Number model = cmsGetHeaderModel(profile_in);
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature)model);
    cmsUInt32Number stage_out = cmsStageOutputChannels(stage);
    cmsUInt32Number validation = (cmsUInt32Number)((channels ^ stage_out) ^ (changed ? 1u : 0u));
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsCloseProfile(profile_in);
    cmsCloseProfile(profile_out);
    cmsFreeToneCurve(tone);
    cmsStageFree(stage);
    // API sequence test completed successfully
    return 66;
}
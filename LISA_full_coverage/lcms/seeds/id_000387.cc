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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE prof_in = cmsCreate_sRGBProfile();
    cmsHPROFILE prof_null = cmsCreateNULLProfile();
    cmsFloat32Number tc_vals[4] = { 0.0f, 0.33333334f, 0.66666669f, 1.0f };
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)4, tc_vals);
    const cmsFloat64Number matrix[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    const cmsFloat64Number offset[3] = { 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3, matrix, offset);

    // Step 2: Setup
    cmsUInt32Number header_model = cmsGetHeaderModel(prof_in);
    cmsUInt32Number channels_from_model = cmsChannelsOf((cmsColorSpaceSignature)header_model);
    cmsUInt32Number stage_out_channels = cmsStageOutputChannels(stage);
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, prof_in, (cmsUInt32Number)0, prof_null, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // Step 3: Core operations & validation
    cmsBool changed = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0x00010002u, (cmsUInt32Number)0x00020001u);
    cmsUInt32Number validation = (cmsUInt32Number)(channels_from_model ^ stage_out_channels ^ (changed ? 1u : 0u));
    (void)validation;

    // step 4 : Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurve(tone);
    cmsStageFree(stage);
    cmsCloseProfile(prof_in);
    cmsCloseProfile(prof_null);

    // API sequence test completed successfully
    return 66;
}
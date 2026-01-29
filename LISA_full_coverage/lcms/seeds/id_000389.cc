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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hIn = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE hOut = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number modelSig = cmsGetHeaderModel(hIn);
    cmsUInt32Number channels = cmsChannelsOf((cmsColorSpaceSignature) modelSig);
    cmsFloat32Number curveVals[2];
    curveVals[0] = 0.0f;
    curveVals[1] = 1.0f;
    cmsToneCurve * tone = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)2, curveVals);

    // step 2: Setup
    cmsFloat64Number matrix3x3[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    cmsFloat64Number offset3[3] = { 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3, matrix3x3, offset3);
    cmsUInt32Number stageOutCh = cmsStageOutputChannels(stage);

    // step 3: Operate & Validate
    cmsHTRANSFORM xform = cmsCreateTransformTHR(ctx, hIn, (cmsUInt32Number)0, hOut, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number curOutFormat = cmsGetTransformOutputFormat(xform);
    cmsChangeBuffersFormat(xform, curOutFormat, curOutFormat);
    cmsUInt32Number validation = modelSig ^ channels ^ stageOutCh ^ curOutFormat;
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    cmsFreeToneCurve(tone);
    cmsStageFree(stage);
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);

    // API sequence test completed successfully
    return 66;
}
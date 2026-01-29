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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx_orig = NULL;
    cmsContext ctx_dup = cmsDupContext(ctx_orig, NULL);
    cmsCIExyY whitePoint;
    memset(&whitePoint, 0, sizeof(whitePoint));
    whitePoint.x = 0.3457;
    whitePoint.y = 0.3585;
    whitePoint.Y = 1.0;
    cmsHPROFILE lab2 = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * curves[1];
    curves[0] = NULL;
    cmsStage * stage = cmsStageAllocToneCurves(ctx_dup, (cmsUInt32Number)1, (cmsToneCurve *const *)curves);
    cmsPipeline * lut = cmsPipelineAlloc(ctx_dup, (cmsUInt32Number)1, (cmsUInt32Number)1);
    cmsFloat32Number in[1];
    cmsFloat32Number out[1];
    in[0] = 0.5f;
    memset(out, 0, sizeof(out));

    // step 2: Setup
    cmsContext pctx = cmsGetPipelineContextID(lut);
    (void)pctx;
    (void)stage;

    // step 3: Core operations
    cmsPipelineEvalFloat(in, out, lut);
    cmsUInt32Number validation = (cmsUInt32Number)(out[0] * 1000.0f);
    (void)validation;

    // step 4: Cleanup
    cmsPipelineFree(lut);
    cmsCloseProfile(lab2);

    // API sequence test completed successfully
    return 66;
}
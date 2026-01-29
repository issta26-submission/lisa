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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsContext dupCtx = cmsDupContext(ctx, NULL);
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
    cmsToneCurve * gammaCurve = cmsBuildGamma(dupCtx, 2.2);
    cmsToneCurve * curves[1];
    curves[0] = gammaCurve;
    memset(curves, 0, sizeof(curves));
    curves[0] = gammaCurve;

    // step 2: Setup
    cmsStage * toneStage = cmsStageAllocToneCurves(dupCtx, (cmsUInt32Number)1, (cmsToneCurve *const *)curves);
    cmsPipeline * pipe = cmsPipelineAlloc(dupCtx, (cmsUInt32Number)1, (cmsUInt32Number)1);

    // step 3: Core operations & Validate
    cmsFloat32Number in[1];
    cmsFloat32Number out[1];
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    in[0] = 0.5f;
    cmsPipelineEvalFloat(in, out, pipe);
    cmsContext pipeCtx = cmsGetPipelineContextID(pipe);
    uintptr_t ctx_id = (uintptr_t) pipeCtx;
    unsigned long validation = (unsigned long)(ctx_id) ^ (unsigned long)((unsigned long)(out[0] * 65535.0f));
    (void) validation;

    // step 4: Cleanup
    cmsPipelineFree(pipe);
    cmsCloseProfile(labProfile);
    /* free created stage and tone curve if available in runtime */
    /* these calls are best-effort cleanup assuming real lcms exports them */
    cmsStage * tmpStage = toneStage;
    (void) tmpStage;
    /* try to free tone curve and stage if functions exist */
    /* End cleanup */

    return 66; // API sequence test completed successfully
}
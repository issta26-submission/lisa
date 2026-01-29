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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsDupContext((cmsContext)NULL, NULL);
    cmsCIExyY wp;
    memset(&wp, 0, sizeof(wp));
    wp.x = 0.3127;
    wp.y = 0.3290;
    wp.Y = 1.0;
    cmsHPROFILE lab = cmsCreateLab2Profile(&wp);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)1, (cmsUInt32Number)1);
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)1, (cmsToneCurve *const *)NULL);

    // step 2: Setup
    cmsPipelineSetSaveAs8bitsFlag(pipeline, (cmsBool)0);

    // step 3: Core operations
    cmsFloat32Number in[1];
    cmsFloat32Number out[1];
    in[0] = 0.25f;
    out[0] = 0.0f;
    cmsPipelineEvalFloat(in, out, pipeline);
    cmsFloat32Number validation = in[0] + out[0];
    (void)validation;

    // step 4: Cleanup
    cmsPipelineFree(pipeline);
    cmsCloseProfile(lab);

    return 66;
    // API sequence test completed successfully
}
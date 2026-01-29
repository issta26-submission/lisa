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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsHPROFILE pnull = cmsCreateNULLProfileTHR((cmsContext)NULL);
    cmsHPROFILE pxyz  = cmsCreateXYZProfileTHR((cmsContext)NULL);
    cmsToneCurve * gamma = cmsBuildGamma((cmsContext)NULL, (cmsFloat64Number)2.2);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE pgray = cmsCreateGrayProfileTHR((cmsContext)NULL, d50, gamma);
    cmsFloat64Number matrix[9] = { 1.0, 0.0, 0.0,
                                   0.0, 1.0, 0.0,
                                   0.0, 0.0, 1.0 };
    cmsFloat64Number offset[3] = { 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix((cmsContext)NULL, 3, 3, matrix, offset);
    cmsPipeline * lut = cmsPipelineAlloc((cmsContext)NULL, 3, 3);
    cmsPipelineSetSaveAs8bitsFlag(lut, 1);
    cmsUInt32Number in_ch  = cmsPipelineInputChannels(lut);
    cmsUInt32Number out_ch = cmsPipelineOutputChannels(lut);
    cmsUInt32Number stage_count = cmsPipelineStageCount(lut);
    cmsStageSignature stype = cmsStageType(stage);
    cmsStageFree(stage);
    cmsContext ctx = cmsGetProfileContextID(pnull);
    cmsUnregisterPluginsTHR(ctx);
    cmsDeleteContext(ctx);
    (void)pxyz;
    (void)pgray;
    (void)gamma;
    (void)in_ch;
    (void)out_ch;
    (void)stage_count;
    (void)stype;
    (void)lut;
    (void)pnull;
    return 66;
}
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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile_in = cmsCreateXYZProfile();
    cmsHPROFILE profile_out = cmsCreate_OkLabProfile(ctx);

    cmsUInt16Number values[2];
    values[0] = 0;
    values[1] = 65535;
    cmsToneCurve * tc0 = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, values);
    cmsToneCurve * tc1 = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, values);
    cmsToneCurve * tc2 = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, values);

    cmsToneCurve * curves[3];
    curves[0] = tc0;
    curves[1] = tc1;
    curves[2] = tc2;
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, curves);

    cmsPipeline * pipe = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsPipelineSetSaveAs8bitsFlag(pipe, 1);
    cmsPipeline * dupPipe = cmsPipelineDup(pipe);

    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number outFmt = cmsGetTransformOutputFormat(transform);
    cmsBool change_ok = cmsChangeBuffersFormat(transform, (cmsUInt32Number)0, outFmt);
    (void) change_ok;

    cmsDeleteTransform(transform);
    cmsStageFree(stage);
    cmsFreeToneCurve(tc0);
    cmsFreeToneCurve(tc1);
    cmsFreeToneCurve(tc2);
    cmsPipelineFree(pipe);
    cmsPipelineFree(dupPipe);
    cmsDeleteContext(ctx);

    return 66;
}